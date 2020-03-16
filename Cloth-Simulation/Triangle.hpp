//
//  Triangle.hpp
//
//  Created by Xindong Cai on 2/22/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include <stdio.h>
#include "Particle.hpp"

class Triangle {
public:
    Particle * a;
    Particle * b;
    Particle * c;
    glm::vec3 n;    // normal of the triangle
    
    Triangle();
    
    Triangle(Particle* a, Particle* b, Particle* c);
    
    void applyAeroForce(glm::vec3& vair, float density, float drag);
    
    void computeNormal();
    
    ~Triangle();
};

#endif /* Triangle_hpp */
