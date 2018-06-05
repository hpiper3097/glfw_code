#include <glad/glad.h>
#include <glfw3.h> //must inculde after glad.h
#include <iostream>
#include <string>
#include <fstream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void shaderCompilationCheck(GLuint shader);
void programCompilationCheck(GLuint program);

const unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;

float verticies[] =
{//ndc coords			colors
	0.0f, 0.75f, 0.0f,	1.0f, 0.0f, 0.0f,
	0.5f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f,
	-0.5f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f
};

float verticies2[] =
{
	0.5f, -0.25f, 0.0f,
	-0.5f, -0.75f, 0.0f,
	0.5f, -0.75f, 0.0f
};

GLuint indices[] =
{
	0, 1, 2,
	1, 2, 3
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //version (major)3.(minor)3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //explicitly using core

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLFW OpenGL Test", nullptr, nullptr); //window creation
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW Window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); //register callbacks after window creation and before gameloop

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //load glad
	{
		std::cout << "Failed to initialize glad!\n";
		return -1;
	}

	//vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream ifs("basicVertexShader.glsl");
	std::string vertexSource((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	ifs.close();
	const GLchar* source = (const GLchar*)vertexSource.c_str();

	glShaderSource(vertexShader, 1, &source, nullptr);
	glCompileShader(vertexShader);
	shaderCompilationCheck(vertexShader);

	//fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	ifs.open("basicFragmentShader.glsl");
	std::string fragmentSource((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	ifs.close();
	source = (const GLchar*)fragmentSource.c_str();

	glShaderSource(fragmentShader, 1, &source, nullptr);
	glCompileShader(fragmentShader);
	shaderCompilationCheck(fragmentShader);

	//fragment shader 2
	GLuint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	ifs.open("basicFragmentShader2.glsl");
	std::string fragmentSource2((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	ifs.close();
	source = (const GLchar*)fragmentSource2.c_str();

	glShaderSource(fragmentShader2, 1, &source, nullptr);
	glCompileShader(fragmentShader2);
	shaderCompilationCheck(fragmentShader2);
	
	//shader program linking
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	programCompilationCheck(shaderProgram);

	GLuint shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);
	programCompilationCheck(shaderProgram2);

	//done with shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);

	GLuint VBO, VAO, EBO, VBO2, VAO2; //vertex array object, vertex buffer object, element buffer object
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO);
	//bind vertex array first, then bind buffer, then configure buffer
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); //typically not done; ony unbind when you need to, however this is showing that you can

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //allowed because all that has been done so far was while VBO was bound

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies2), verticies2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

	while (!glfwWindowShouldClose(window))//game loop
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //set color to clear with... sets state
		glClear(GL_COLOR_BUFFER_BIT); //clears with color... uses state

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO2);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram2, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();//free resources
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int height, int width)
{
	glViewport(0, 0, height, width);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void shaderCompilationCheck(GLuint shader)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
		std::cout << "ERROR:SHADER:COMPILATION:FAILED\n" << infoLog << std::endl;
	}
}

void programCompilationCheck(GLuint program)
{
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);
		std::cout << "ERROR:PROGRAM:COMPILATION:FAILED\n" << infoLog << std::endl;
	}
}