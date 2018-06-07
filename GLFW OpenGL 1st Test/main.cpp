#include <glad/glad.h>
#include <glfw3.h> //must inculde after glad.h
#include "shader.h"
#include "texture.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "assert.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, float &mixVar);
void shaderCompilationCheck(GLuint shader);
void programCompilationCheck(GLuint program); 
const unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;

float verticies[] = {
	// positions          // colors           // texture coords
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

/*float verticies2[] =
{
	-0.5f, -0.75f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};*/

GLuint indices[] =
{
	0, 1, 3,
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

	auto delWindow = [](GLFWwindow* window)
	{
		glfwDestroyWindow(window);
	};

	std::unique_ptr<GLFWwindow, decltype(delWindow)> window(glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLFW OpenGL Test", nullptr, nullptr), delWindow); //window creation
	ASSERT(window.get() != nullptr);
	glfwMakeContextCurrent(window.get());
	glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback); //register callbacks after window creation and before gameloop

	ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)); //load glad

	GLuint VBO, VAO, EBO; //vertex array object, vertex buffer object, element buffer object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//bind vertex array first, then bind buffer, then configure buffer
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture coords attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //typically not done; ony unbind when you need to, however this is showing that you can

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //allowed because all that has been done so far was while VBO was bound
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

	Shader shader1("basicVertexShader.vs", "basicFragmentShader.fs");
	Texture tex1("Textures/container.jpg", GL_RGB, GL_TEXTURE_2D, GL_TEXTURE0);
	stbi_set_flip_vertically_on_load(true);
	Texture tex2("Textures/awesomeface.png", GL_RGBA, GL_TEXTURE_2D, GL_TEXTURE1);

	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);//don't use mipmaps for mag; mipmaps only used on smaller objects

	shader1.use();
	shader1.setInt("texture1", 0);
	shader1.setInt("texture2", 1);

	auto mixVar = 0.f;

	while (!glfwWindowShouldClose(window.get()))//game loop
	{
		processInput(window.get(), mixVar);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //set color to clear with... sets state
		glClear(GL_COLOR_BUFFER_BIT); //clears with color... uses state

		shader1.use();
		glActiveTexture(GL_TEXTURE0);
		tex1.bind();
		glActiveTexture(GL_TEXTURE1);
		tex2.bind();
		glBindVertexArray(VAO);
		shader1.setFloat("mixVar", mixVar);
		glm::mat4 trans;
		trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.25f, 0.0f));
		trans = glm::rotate(trans, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		unsigned int transLoc = glGetUniformLocation(shader1.ID, "transform");
		glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		trans = glm::mat4();
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.3f, 0.0f));
		//trans = glm::rotate(trans, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		trans = glm::scale(trans, glm::vec3(glm::sin(static_cast<float>(glfwGetTime())), glm::sin(static_cast<float>(glfwGetTime())), 0.5f));
		glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glfwSwapBuffers(window.get());
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

void processInput(GLFWwindow* window, float &mixVar)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixVar += 0.01;
		if (mixVar > 1.0)
			mixVar = 1.0;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixVar -= 0.01;
		if (mixVar < 0)
			mixVar = 0;
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