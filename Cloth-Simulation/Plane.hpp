//
//  Plane.hpp
//
//  Created by Xindong Cai on 3/15/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#ifndef Plane_hpp
#define Plane_hpp

#include <stdio.h>
#include "Object.hpp"

using namespace std;

class Plane : public Object {
private:
    // buffers for rendering
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    
    vector<glm::vec3> positions;
    vector<glm::vec3> normals;
    vector<unsigned int> indices;
    
    void initBuffers();
    
public:
    Plane();
    
    Plane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 color);
    
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    
    void update();
    
    void translate(glm::vec3 offset);
    
    ~Plane();
    
};

#endif /* Plane_hpp */
