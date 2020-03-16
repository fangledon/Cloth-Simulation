//
//  SpringDamper.hpp
//
//  Created by Xindong Cai on 2/22/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#ifndef SpringDamper_hpp
#define SpringDamper_hpp

#include <stdio.h>
#include "Particle.hpp"

class SpringDamper {
public:
    
    float Ks;   // spring constant
    float Kd;   // damping factor
    float l;    // rest length
    Particle * p1;
    Particle * p2;
    
    SpringDamper();
    
    SpringDamper(Particle* p1, Particle* p2, float l, float Ks, float Kd);
    
    void applyForce();
    
    ~SpringDamper();
    
};

#endif /* SpringDamper_hpp */
