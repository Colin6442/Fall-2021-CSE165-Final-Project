#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <math.h>
#include <malloc.h>
#include "shaderClass.h"
#include <time.h>

struct Square{
	GLfloat box[18];
	bool set = false;

	void setBox(GLfloat *in){
		for(int i = 0; i < 18; i++){
			box[i] = in[i];
		}
		set = true;
	}

	void moveUp(){
		box[0] += 0.1; box[3] += 0.1; box[6] += 0.1;
		box[9] += 0.1; box[12] += 0.1; box[15] += 0.1; 
	}

	void moveDown(){
		box[0] -= 0.1; box[3] -= 0.1; box[6] -= 0.1;
		box[9] -= 0.1; box[12] -= 0.1; box[15] -= 0.1; 
	}

	void setBoxRand(){
		float x = ((rand() % (20 * 2) - 20) / 10.0f);
		float y = ((rand() % (20 * 2) - 20) / 10.0f);

		box[0] = x; box[1] = y; box[2] =  0;
		box[3] = x; box[4] = y - 0.1; box[5] = 0;
		box[6] = x - 0.1; box[7] = y; box[8] = 0;

		box[9] = x; box[10] = y - 0.1; box[11] = 0;
		box[12] = x - 0.1; box[13] = y - 0.1; box[14] = 0;
		box[15] = x - 0.1; box[16] = y; box[17] = 0;
		set = true;
	}

};

int main(){
	srand(time(0));
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
	glewExperimental = true;
	glewInit();

	// GLfloat box[] = {
	// 	// first triangle
	// 	0.05f,  0.05f, 0.0f,  // top right
	// 	0.05f, -0.05f, 0.0f,  // bottom right
	// 	-0.05f,  0.05f, 0.0f,  // top left 
	// 	// second triangle
	// 	0.05f, -0.05f, 0.0f,  // bottom right
	// 	-0.05f, -0.05f, 0.0f,  // bottom left
	// 	-0.05f,  0.05f, 0.0f   // top left
	// };


	Square* snake = new Square[500];
	int num = 0;

	Shader shaderProgram("vertexShaders/default.vert", "fragmentShaders/default.frag");

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(3, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// unsigned int EBO;
	// glGenBuffers(1, &EBO);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);  


    float one = 0.07f, two = 0.13f, three = 0.17f;

    while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(one, two, three, 1.0f);

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
			snake[num].setBoxRand();
			num++;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
			snake[0].moveUp();
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			snake[0].moveDown();
		}

		shaderProgram.Activate();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		for(int i = 0; i < num; i++){
			glBufferData(GL_ARRAY_BUFFER, sizeof(snake[i].box), snake[i].box, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawArrays(GL_TRIANGLES, 3, 3);
		}
        glfwSwapBuffers(window);
		glfwPollEvents();
		Sleep(10);

    }
}