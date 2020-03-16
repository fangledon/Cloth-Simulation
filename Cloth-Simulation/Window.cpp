#include "Window.hpp"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Cloth Simulation";

// Objects to render
vector<Object*> Window::objects;
Cloth* Window::cloth;

// Camera Properties
Camera* cam;
float groundHeight;

// Interaction Variables
bool leftDown, rightDown;
int mouseX, mouseY;

// The shader program id
GLuint Window::shaderProgram;

// Constructors and desctructors 
bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shader.vert", "shader.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	return true;
}

bool Window::initializeObjects()
{
	// Create and initilize the object to render
    groundHeight = -2.5f;
    float halfGroundSize = 10.0f;
    
    glm::vec3 planeColor = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 p1 = glm::vec3(-halfGroundSize, groundHeight, halfGroundSize);
    glm::vec3 p2 = glm::vec3(halfGroundSize, groundHeight, halfGroundSize);
    glm::vec3 p3 = glm::vec3(halfGroundSize, groundHeight, -halfGroundSize);
    glm::vec3 p4 = glm::vec3(-halfGroundSize, groundHeight, -halfGroundSize);
    Plane* plane = new Plane(p1, p2, p3, p4, planeColor);
    objects.push_back(plane);
    
    setScene(1);
    
	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
    for (Object* obj : objects) {
        delete obj;
    }

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
}

// for the Window
GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// set up the camera
	cam = new Camera();
	cam->setAspect(float(width) / float(height));

	// initialize the interaction variables
	leftDown = rightDown = false;
	mouseX = mouseY = 0;

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	cam->setAspect(float(width) / float(height));
}

// update and draw functions
void Window::idleCallback()
{
	// Perform any updates as necessary. 
	cam->update();
    
    for (Object* obj : objects) {
        obj->update();
    }
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the object.
    for (Object* obj : objects) {
        obj->draw(cam->getViewProjectMtx(), Window::shaderProgram);
    }

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

// helper to reset the camera
void Window::resetCamera() {
	cam->reset();
	cam->setAspect(float(Window::width) / float(Window::height));
}

// callbacks - for Interaction 
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key) 
		{
            case GLFW_KEY_ESCAPE: {
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            }
            case GLFW_KEY_A: {
                for (unsigned int i = 1; i < objects.size(); i++) {
                    objects[i]->translate(-0.1f, 0.0f, 0.0f);
                }
                break;
            }
            case GLFW_KEY_D: {
                for (unsigned int i = 1; i < objects.size(); i++) {
                    objects[i]->translate(0.1f, 0.0f, 0.0f);
                }
                break;
            }
            case GLFW_KEY_W: {
                for (unsigned int i = 1; i < objects.size(); i++) {
                    objects[i]->translate(0.0f, 0.0f, -0.1f);
                }
                break;
            }
            case GLFW_KEY_S: {
                for (unsigned int i = 1; i < objects.size(); i++) {
                    objects[i]->translate(0.0f, 0.0f, 0.1f);
                }
                break;
            }
            case GLFW_KEY_O: {
                for (unsigned int i = 1; i < objects.size(); i++) {
                    objects[i]->translate(0.0f, 0.1f, 0.0f);
                }
                break;
            }
            case GLFW_KEY_L: {
                for (unsigned int i = 1; i < objects.size(); i++) {
                    objects[i]->translate(0.0f, -0.1f, 0.0f);
                }
                break;
            }
            case GLFW_KEY_UP: {
                glm::vec3 newWind = cloth->getWind() + glm::vec3(0.0f, 0.0f, -0.2f);
                cloth->setWind(newWind);
                break;
            }
            case GLFW_KEY_DOWN: {
                glm::vec3 newWind = cloth->getWind() + glm::vec3(0.0f, 0.0f, 0.2f);
                cloth->setWind(newWind);
                break;
            }
            case GLFW_KEY_LEFT: {
                glm::vec3 newWind = cloth->getWind() + glm::vec3(-0.2f, 0.0f, 0.0f);
                cloth->setWind(newWind);
                break;
            }
            case GLFW_KEY_RIGHT: {
                glm::vec3 newWind = cloth->getWind() + glm::vec3(0.2f, 0.0f, 0.0f);
                cloth->setWind(newWind);
                break;
            }
            case GLFW_KEY_1: {
                setScene(1);
                break;
            }
            case GLFW_KEY_2: {
                setScene(2);
                break;
            }
            case GLFW_KEY_3: {
                setScene(3);
                break;
            }
            default: {
                break;
            }
		}
	}
}

void Window::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		leftDown = (action == GLFW_PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		rightDown = (action == GLFW_PRESS);
	}
}

void Window::cursorCallback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - mouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - mouseY), -maxDelta, maxDelta);

	mouseX = (int)currX;
	mouseY = (int)currY;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if (leftDown) {
		const float rate = 1.0f;
		cam->setAzimuth(cam->getAzimuth() + dx * rate);
		cam->setIncline(glm::clamp(cam->getIncline() - dy * rate, -90.0f, 90.0f));
	}
	if (rightDown) {
		const float rate = 0.005f;
		float dist = glm::clamp(cam->getDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
		cam->setDistance(dist);
	}
}

void Window::setScene(int sceneNum) {
    switch (sceneNum) {
        case 1: { // scene 1: vertical cloth with fixed first row (curtain)
            resetCamera();
            
            while (objects.size() > 1) { // delete non-plane object
                delete objects.back();
                objects.pop_back();
            }
            
            glm::vec3 clothColor = glm::vec3(1.0f, 0.95f, 0.1f);
            cloth = new Cloth(50, 50, 0.06f, 1.0f, clothColor, true);
            cloth->setFixedRow(0);
            cloth->setWind(glm::vec3(1.2f, 0.0f, 1.0f)); // initial wind speed
            cloth->setGroundHeight(groundHeight);
            objects.push_back(cloth);
            break;
        }
        case 2: { // scene 2: vertical cloth with 3 fixed points (flag)
            resetCamera();
            
            while (objects.size() > 1) { // delete non-plane object
                delete objects.back();
                objects.pop_back();
            }
            
            glm::vec3 color = glm::vec3(0.95f, 0.08f, 0.0f);
            cloth = new Cloth(50, 60, 0.06f, 1.0f, color, true);
            cloth->setFixedPoint(0, 0);
            cloth->setFixedPoint(24, 0);
            cloth->setFixedPoint(49, 0);
            cloth->setWind(glm::vec3(4.5f, 0.0f, 1.2f)); // initial wind speed
            cloth->setGroundHeight(groundHeight);
            objects.push_back(cloth);
            break;
        }
        case 3: { // scene 3: horizontal cloth with fixed corners (parachute)
            resetCamera();
            
            while (objects.size() > 1) { // delete non-plane object
                delete objects.back();
                objects.pop_back();
            }
            
            unsigned int height = 40;
            unsigned int width = 50;
            
            glm::vec3 clothColor = glm::vec3(0.81f, 0.98f, 0.53f);
            cloth = new Cloth(height, width, 0.06f, 1.0f, clothColor, false);
            glm::vec3 a0 = cloth->setFixedPoint(0, 0);
            glm::vec3 b0 = cloth->setFixedPoint(0, width - 1);
            glm::vec3 c0 = cloth->setFixedPoint(height - 1, width - 1);
            glm::vec3 d0 = cloth->setFixedPoint(height - 1, 0);
            cloth->setWind(glm::vec3(0.0f, 5.0f, -0.2f)); // initial wind speed
            cloth->setGroundHeight(groundHeight);
            objects.push_back(cloth);
            
            glm::vec3 cubeColor = glm::vec3(0.28f, 0.14f, 0.04f);
            glm::vec3 cubeMin = glm::vec3(-0.25f, -1.5f, -0.25f);
            glm::vec3 cubeMax = glm::vec3(0.25f, -1.0f, 0.25f);
            Cube* cube = new Cube(cubeMin, cubeMax, cubeColor);
            objects.push_back(cube);
            
            glm::vec3 lineColor = glm::vec3(0.1f, 0.1f, 0.1f);
            glm::vec3 a1 = glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z);
            glm::vec3 b1 = glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z);
            glm::vec3 c1 = glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z);
            glm::vec3 d1 = glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z);
            
            Line* l1 = new Line(a0, a1, lineColor);
            Line* l2 = new Line(b0, b1, lineColor);
            Line* l3 = new Line(c0, c1, lineColor);
            Line* l4 = new Line(d0, d1, lineColor);
            
            objects.insert(objects.end(), {l1, l2, l3, l4});
            
            break;
        }
        default: {
            break;
        }
    }
}
