//
//  Line.cpp
//
//  Created by Xindong Cai on 3/16/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#include "Line.hpp"

Line::Line(glm::vec3 p1, glm::vec3 p2, glm::vec3 color) {
    this->model = glm::mat4(1.0f);
    this->color = color;
    
    positions = {p1, p2};
    indices = {0, 1};
    
    initBuffers();
}

void Line::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    // actiavte the shader program
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
    
    // Bind the VAO
    glBindVertexArray(VAO);

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_LINES, (unsigned int) indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void Line::update() {}

void Line::translate(glm::vec3 offset) {
    model = glm::translate(model, offset);
}

void Line::initBuffers() {
    // generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &EBO);
    
    // bind to the VAO.
    glBindVertexArray(VAO);
    
    // bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // bind the EBO to the bound VAO and send the data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Line::~Line() {
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    //glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}
