//
//  Particle.hpp
//
//  Created by Xindong Cai on 2/17/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include "Core.h"

class Particle {
public:
    float m;        // mass
    glm::vec3 p;    // position
    glm::vec3 v;    // velocity
    glm::vec3 f;    // force
    glm::vec3 n;    // normal
    bool fixed;
    
    Particle();
    
    Particle(float x, float y, float z, float mass);
    
    void update(float deltaTime);
    
    void applyForce(glm::vec3& f);
    
    ~Particle();
};

#endif /* Particle_hpp */
