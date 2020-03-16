//
//  Cloth.hpp
//
//  Created by Xindong Cai on 2/23/20.
//  Copyright © 2020 Xindong Cai. All rights reserved.
//

#ifndef Cloth_hpp
#define Cloth_hpp

#include <stdio.h>
#include <vector>

#include "Object.hpp"
#include "SpringDamper.hpp"
#include "Triangle.hpp"

#define ELASTICITY      0.5f
#define FRICTION        0.1f
#define SPRING_CONST    40.0f
#define DAMPING_CONST   0.05f
#define G               glm::vec3(0.0f, -9.8f, 0.0f)
#define NUM_SAMPLE      2
#define TIME_STEP       1.0f / 1200.0f
#define AIR_DENSITY     1.225f
#define DRAG            1.0f
#define EPSILON         0.001f;

using namespace std;

class Cloth : public Object {
private:
    
    // buffers for rendering
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;
    
    vector<glm::vec3> positions;
    vector<glm::vec3> normals;
    vector<unsigned int> indices;
    
    vector<Particle*> particles;
    vector<SpringDamper*> springDampers;
    vector<Triangle*> triangles;
    vector<unsigned int> fixedId;
    
    unsigned int width;     // number of particles on x axis
    unsigned int height;    // number of particles on y axis
    float offset;           // offset between two particles positions
    float totalMass;        // total mass of the cloth
    glm::vec3 wind;         // wind that creates aero dynamics
    float groundHeight;     // the height of the ground
    
    void initParticles(bool verticalLayout);
    
    void initSpringDampers(unsigned int numBetween, float Ks, float Kd);
    
    void initTriangles();
    
    void updateNormals();
    
    void initBuffers();
    
    void updateBuffers();
    
    void handleCollision(Particle* particle);
    
public:
    
    Cloth();
    
    Cloth(unsigned int height, unsigned int width, float offset,
          float totalMass, glm::vec3 color, bool verticalLayOut);
    
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    
    void update();
    
    void setFixedRow(int r);
    
    void setFixedCol(int c);
    
    glm::vec3 setFixedPoint(int r, int c);
    
    void translate(float xOffset, float yOffset, float zOffset);
    
    void setGroundHeight(float height) { this->groundHeight = height + EPSILON; }
    
    void setWind(glm::vec3 wind) { this->wind = wind; };
    
    glm::vec3 getWind() { return wind; };
    
    ~Cloth();
};

#endif /* Cloth_hpp */
