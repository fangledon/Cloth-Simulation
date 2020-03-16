#ifndef Cube_hpp
#define Cube_hpp

#include "Core.h"
#include "Object.hpp"

using namespace std;

class Cube : public Object {
private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;
    
	vector<glm::vec3> positions;
	vector<glm::vec3> normals;
	vector<unsigned int> indices;
    
    void initBuffers();

public:

	Cube(glm::vec3 cubeMin = glm::vec3(-1,-1,-1), glm::vec3 cubeMax = glm::vec3(1, 1, 1),
         glm::vec3 color = glm::vec3(1, 1, 1));

	void draw(const glm::mat4& viewProjMtx, GLuint shader);
    
	void update();
    
    void translate(float xOffset, float yOffset, float zOffset);
    
    ~Cube();
};

#endif
