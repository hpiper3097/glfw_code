#include "application.h"

Application::Application(const unsigned int w, const unsigned int h)
	: window(nullptr, delWindow)
{
	SCR_WIDTH = w;
	SCR_HEIGHT = h;
	init();
}

Application::~Application()
{
	delWindow;
	kill();
}

void Application::init()
{
	ASSERT(glfwInit() == GL_TRUE); GLFW = true;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //version (major)3.(minor)3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //explicitly using core

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window.reset(glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLFW OpenGL Test", nullptr, nullptr)); //window creation
	ASSERT(window.get() != nullptr);
	glfwMakeContextCurrent(window.get());
	glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback); //register callbacks after window creation and before gameloop

	ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)); GLAD = true;//load glad
}

void Application::kill()
{
	glfwTerminate();
	GLFW = false;
	GLAD = false;
}

void Application::framebuffer_size_callback(GLFWwindow* window, int height, int width)
{
	glViewport(0, 0, height, width);
}

