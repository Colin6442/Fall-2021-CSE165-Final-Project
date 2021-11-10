#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <Windows.h>


int main(){
    int windowX = 900;
	int windowY = 900;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(windowX, windowY, "Test", NULL, NULL);
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glViewport(0, 0, windowX, windowY);

	GLfloat vertices[] = {
		-0.05f, 0.1f, 0.0f, // Lower left corner	PointA
		0.05f, 0.1f, 0.0f, // Lower right corner	PointB
		0.0f, 0.0f, 0.0f // Upper corner			PointC
	};

    float one = 0.07f, two = 0.13f, three = 0.17f;

    while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(one, two, three, 1.0f);

        glfwSwapBuffers(window);
		glfwPollEvents();
		Sleep(10);

    }
}