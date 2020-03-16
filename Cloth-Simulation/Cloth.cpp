//
//  Cloth.cpp
//
//  Created by Xindong Cai on 2/23/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#include "Cloth.hpp"

Cloth::Cloth() {}

Cloth::Cloth(unsigned int height, unsigned int width, float offset,
             float totalMass, glm::vec3 color, bool verticalLayOut) {
    
    this->height = height;
    this->width = width;
    this->offset = offset;
    this->color = color;
    this->totalMass = totalMass;
    this->wind = glm::vec3(0.0f, 0.0f, 0.0f);
    this->groundHeight = -2.5f; // default height of the ground
    
    positions = vector<glm::vec3>(width * height);
    normals = vector<glm::vec3>(width * height);
    
    model = glm::mat4(1.0f); // local matrix
    
    initParticles(verticalLayOut);
    initSpringDampers(1, SPRING_CONST, DAMPING_CONST);
    initTriangles();
    updateNormals();
    initBuffers();
}

void Cloth::initParticles(bool verticalLayout) {
    float mass = totalMass / (height * width);
    float halfHeight = (float) height / 2.0f;
    float halfWidth = (float) width / 2.0f;
    
    if (verticalLayout) {
        for (unsigned int h = 0; h < height; h++) {
            for (unsigned int w = 0; w < width; w++) {
                Particle* particle = new Particle(offset * (w - halfWidth), offset * (h - halfHeight), 0.0f, mass);
                positions[particles.size()] = particle->p;
                particles.push_back(particle);
            }
        }
    }
    else {
        for (unsigned int h = 0; h < height; h++) {
            for (unsigned int w = 0; w < width; w++) {
                Particle* particle = new Particle(offset * (w - halfWidth), 0.0f, -offset * (h - halfHeight), mass);
                positions[particles.size()] = particle->p;
                particles.push_back(particle);
            }
        }
    }
}

void Cloth::initSpringDampers(unsigned int numBetween, float Ks, float Kd) {
    float len = numBetween * this->offset;
    float diagLen = sqrt(2) * len;
    for (unsigned int h = 0; h < height; h += numBetween) {
        unsigned int rowOffset = h * width;
        for (unsigned int w = 0; w < width; w += numBetween) {
            // not at top most col
            if (h < height - numBetween) {
                // always have up: connect curr -> up
                Particle* curr = particles[rowOffset + w];
                Particle* up = particles[rowOffset + width + w];
                springDampers.push_back(new SpringDamper(curr, up, len, Ks, Kd));
                
                // not at last row: connect curr -> right, up -> right, curr -> upright
                if (w < width - numBetween) {
                    Particle* right = particles[rowOffset + w + numBetween];
                    Particle* upRight = particles[rowOffset + width + w + numBetween];
                    springDampers.push_back(new SpringDamper(curr, right, len, Ks, Kd));
                    springDampers.push_back(new SpringDamper(up, right, diagLen, Ks, Kd));
                    springDampers.push_back(new SpringDamper(curr, upRight, diagLen, Ks, Kd));
                }
            }
            else { // at top most col: only connect curr -> right
                if (w < width - numBetween) {
                    Particle* curr = particles[rowOffset + w];
                    Particle* right = particles[rowOffset + w + numBetween];
                    springDampers.push_back(new SpringDamper(curr, right, len, Ks, Kd));
                }
            }
        }
    }
}

void Cloth::initTriangles() {
    for (unsigned int h = 0; h < height - 1; h++) {
         unsigned int rowOffset = h * width;
        for (unsigned int w = 0; w < width - 1; w++) {
            unsigned int currId = rowOffset + w;
            unsigned int rightId = rowOffset + w + 1;
            unsigned int upId = rowOffset + width + w;
            unsigned int upRightId = rowOffset + width + w + 1;
            triangles.push_back(new Triangle(particles[currId], particles[upRightId], particles[upId]));
            triangles.push_back(new Triangle(particles[currId], particles[rightId], particles[upRightId]));
            indices.insert(indices.end(), {currId, upRightId, upId});
            indices.insert(indices.end(), {currId, rightId, upRightId});
        }
    }
}

void Cloth::updateNormals() {
    // first clear the normal for each particle
    for (Particle* p : particles) {
        p->n = glm::vec3(0.0f);
    }
    
    // dynamic smooth shading: use normal of triangle to compute averaged normal for each particle
    for (Triangle* t : triangles) {
        t->computeNormal();
        t->a->n += t->n;
        t->b->n += t->n;
        t->c->n += t->n;
    }
    
    // normalize the normals for each particle
    for (unsigned int i = 0; i < particles.size(); i++) {
        particles[i]->n = glm::normalize(particles[i]->n);
        normals[i] = particles[i]->n;
    }
}

void Cloth::initBuffers() {
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

void Cloth::updateBuffers() {
    glBindVertexArray(VAO);
    
    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
}

void Cloth::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, GL_FALSE, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

    // Bind the VAO
    glBindVertexArray(VAO);
    
    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, (unsigned int) indices.size(), GL_UNSIGNED_INT, 0);
    
    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void Cloth::update() {
    // oversampling to improve system stability
    for (unsigned int i = 0; i < NUM_SAMPLE; i++) {
        // apply forces and update position of each vertex
        for (SpringDamper* s : springDampers) {
            s->applyForce();
        }
        for (Triangle* t : triangles) {
            t->applyAeroForce(wind, AIR_DENSITY, DRAG);
        }
        for (Particle* particle : particles) {
            glm::vec3 gForce = particle->m * G;
            particle->applyForce(gForce);
            particle->update(TIME_STEP);
            handleCollision(particle);
        }
    }
    
    // update the buffers for rendering
    for (unsigned int i = 0; i < particles.size(); i++) {
        positions[i] = particles[i]->p;
    }
    updateNormals();
    updateBuffers();
}

void Cloth::handleCollision(Particle* particle) {
    if (particle->p.y < groundHeight) {
        particle->p.y = 2.0f * groundHeight - particle->p.y;
        particle->v.y = -ELASTICITY * particle->v.y;
        particle->v.x = (1.f - FRICTION) * particle->v.x;
        particle->v.z = (1.f - FRICTION) * particle->v.z;
    }
}

void Cloth::setFixedRow(int r) {
    if (r < 0 || r > height - 1) return;
    r = (height - 1) - r; // user counts the row from the top
    
    for (unsigned int i = r * width; i < r * width + width; i++) {
        particles[i]->fixed = true;
        fixedId.push_back(i);
    }
}

void Cloth::setFixedCol(int c) {
    if (c < 0 || c > width - 1) return;
    
    for (unsigned int i = c; i < particles.size(); i += width) {
        particles[i]->fixed = true;
        fixedId.push_back(i);
    }
}

glm::vec3 Cloth::setFixedPoint(int r, int c) {
    if (r < 0 || r > height - 1 || c < 0 || c > width - 1) return glm::vec3(0.0f);
    r = (height - 1) - r; // user counts the row from the top
    
    particles[r * width + c]->fixed = true;
    fixedId.push_back(r * width + c);
    return particles[r * width + c]->p;
}

void Cloth::translate(float xOffset, float yOffset, float zOffset) {
    for (unsigned int id : fixedId) {
        particles[id]->p.x += xOffset;
        particles[id]->p.y += yOffset;
        particles[id]->p.z += zOffset;
        positions[id] = particles[id]->p;
    }
    updateBuffers();
}

Cloth::~Cloth() {
    for (Particle* p : particles) {
        delete p;
    }
    for (SpringDamper* s : springDampers) {
        delete s;
    }
    for (Triangle* t : triangles) {
        delete t;
    }
    
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}
