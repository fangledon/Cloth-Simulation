//
//  Particle.cpp
//
//  Created by Xindong Cai on 2/17/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#include "Particle.hpp"

Particle::Particle() {}

Particle::Particle(float x, float y, float z, float m) {
    this->m = m;
    p = glm::vec3(x, y, z);
    v = glm::vec3(0.0f);
    f = glm::vec3(0.0f);
    n = glm::vec3(0.0f);
    fixed = false;
}

void Particle::update(float deltaTime) {
    
    if (fixed) return;
    
    // use forward euler integration
    v += deltaTime * (f / m);
    p += deltaTime * v;
    f = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Particle::applyForce(glm::vec3& f) {
    this->f += f;
}

Particle::~Particle() {}
