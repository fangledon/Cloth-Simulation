//
//  ClothTriangle.cpp
//
//  Created by Xindong Cai on 2/22/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#include "Triangle.hpp"

Triangle::Triangle() {}

Triangle::Triangle(Particle* a, Particle* b, Particle* c) {
    this->a = a;
    this->b = b;
    this->c = c;
}

void Triangle::applyAeroForce(glm::vec3& vair, float density, float drag) {
    
    // compute the averaged velocity of the triangle
    glm::vec3 v = ((a->v + b->v + c->v) / 3.0f) - vair;
    float vLen = glm::length(v);
    if (vLen == 0) return;
    
    // compute normal of triangle
    glm::vec3 ab = b->p - a->p;
    glm::vec3 ac = c->p - a->p;
    glm::vec3 normal = glm::cross(ab, ac);
    float normalLen = glm::length(normal);
    n = normal / normalLen;
    
    // compute aerodynamic force
    glm::vec3 faero = -0.25f * density * normalLen * glm::dot(v, n) * vLen * drag * n;
    
    // apply force to each particle
    glm::vec3 feach = faero / 3.0f;
    a->applyForce(feach);
    b->applyForce(feach);
    c->applyForce(feach);
}

void Triangle::computeNormal() {
    glm::vec3 ab = b->p - a->p;
    glm::vec3 ac = c->p - a->p;
    n = glm::normalize(glm::cross(ab, ac));
}

Triangle::~Triangle() {}

