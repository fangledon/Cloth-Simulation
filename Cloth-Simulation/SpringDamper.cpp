//
//  SpringDamper.cpp
//
//  Created by Xindong Cai on 2/22/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#include "SpringDamper.hpp"

SpringDamper::SpringDamper() {}

SpringDamper::SpringDamper(Particle* p1, Particle* p2, float l, float Ks, float Kd) {
    this->p1 = p1;
    this->p2 = p2;
    this->l = l;
    this->Ks = Ks;
    this->Kd = Kd;
}

void SpringDamper::applyForce() {
    glm::vec3 e = p2->p - p1->p;
    float eLen = glm::length(e);
    e = glm::normalize(e); // normalize
    
    // compute the force act on p1
    float fspring = -Ks * (l - eLen);
    float fdamp = -Kd * (glm::dot(p1->v, e) - glm::dot(p2->v, e));
    glm::vec3 f1 = (fspring + fdamp) * e;
    glm::vec3 f2 = -f1;
        
    p1->applyForce(f1);
    p2->applyForce(f2);
}

SpringDamper::~SpringDamper() {}
