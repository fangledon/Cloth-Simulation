#ifndef Window_hpp
#define Window_hpp

#include "main.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Cloth.hpp"
#include "Plane.hpp"
#include "Cube.hpp"
#include "Line.hpp"

class Window {
public:
	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Objects to render
    static vector<Object*> objects;
    static Cloth* cloth;

	// Shader Program 
	static GLuint shaderProgram;

	// Act as Constructors and desctructors 
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// for the Window
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// update and draw functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// helper to reset the camera
	static void resetCamera();

	// callbacks - for interaction
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorCallback(GLFWwindow* window, double currX, double currY);
    
private:
    static void setScene(int sceneNum);
};
#endif
