//
//  Plane.cpp
//
//  Created by Xindong Cai on 3/15/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#include "Plane.hpp"

Plane::Plane() {}

Plane::Plane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 color) {
    model = glm::mat4(1.0f);
    this->color = color;
    
    positions.insert(positions.end(), {p1, p2, p3, p4});
    glm::vec3 p1p2 = p2 - p1;
    glm::vec3 p1p3 = p3 - p1;
    glm::vec3 normal = glm::normalize(glm::cross(p1p2, p1p3));
    normals.insert(normals.end(), {normal, normal});
    indices.insert(indices.end(), {0, 1, 2, 2, 3, 0});
    
    initBuffers();
}

void Plane::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, GL_FALSE, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

    // Bind the VAO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, (unsigned int) indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void Plane::update() {}

void Plane::initBuffers() {
    // generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);
    glGenBuffers(1, &EBO);
    
    // bind to the VAO.
    glBindVertexArray(VAO);
    
    // bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // bind the EBO to the bound VAO and send the data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Plane::translate(glm::vec3 offset) {}

Plane::~Plane() {
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}
