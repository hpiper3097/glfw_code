#ifndef APPICATION_H
#define APPLICATION_H

#include "glad\glad.h"
#include "glfw3.h"
#include "assert.h"
#include <memory>
#include <iostream>

const auto delWindow = [](GLFWwindow* window)
{
	glfwDestroyWindow(window);
};

class Application
{
	std::unique_ptr<GLFWwindow, decltype(delWindow)> window;
	int SCR_WIDTH = 0, SCR_HEIGHT = 0;

	bool GLFW = false, GLAD = false;

	void init();
	void kill();
	static void framebuffer_size_callback(GLFWwindow* window, int height, int width);
public:
	Application(const unsigned int w,const unsigned int h);

	~Application();

	inline GLFWwindow* Window()
	{
		return window.get();
	}
};

#endif
