//
//  Object.hpp
//
//  Created by Xindong Cai on 3/15/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include "Core.h"

class Object {
    
protected:
    glm::mat4 model;
    glm::vec3 color;
    
public:
    glm::mat4 getModel() { return model; }
    glm::vec3 getColor() { return color; }

    virtual void draw(const glm::mat4& viewProjMtx, GLuint shader) = 0;
    virtual void update() = 0;
    virtual void translate(float xOffset, float yOffset, float zOffset) = 0;
    virtual ~Object() {}
};

#endif /* Object_hpp */
