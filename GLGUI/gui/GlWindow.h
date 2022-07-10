#pragma once
#include "GlContainer.h"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include "Text.h"

class GlWindow : public GlComponent { //When working on this make this a GlContainer again
public:
	GLFWdropfun dropCallback;

	std::vector<GlComponent*> components;

	std::string windowTitle;

	GLFWwindow* glfwWindow;

	//TextManager textManager;

	GlWindow(std::string _windowTitle, int _width, int _height) : GlComponent(0, 0, _width, _height) { windowTitle = _windowTitle; } //GlContainer(0, 0, _width, _height)

	int startWindow();

	void initialize();

	void tick();

	void render();

	//void drop_callback(GLFWwindow* window, int path_count, const char** paths);
};