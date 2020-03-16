//
//  Line.hpp
//
//  Created by Xindong Cai on 3/16/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#ifndef Line_hpp
#define Line_hpp

#include <stdio.h>
#include "Object.hpp"

using namespace std;

class Line : public Object {
private:
    GLuint VAO;
    GLuint VBO_positions, EBO;
    
    vector<glm::vec3> positions;
    vector<unsigned int> indices;
    
    void initBuffers();
    
public:
    
    Line(glm::vec3 p1, glm::vec3 p2, glm::vec3 color);
    
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    
    void update();
    
    void translate(float xOffset, float yOffset, float zOffset);
    
    ~Line();
    
};
#endif /* Line_hpp */
