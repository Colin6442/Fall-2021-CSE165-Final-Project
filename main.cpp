#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <math.h>
#include <malloc.h>
#include "shaderClass.h"

#define PI 3.14159265

void Rotate(int turn, float* length, GLfloat *vertices, float* currentAngle, double* angleA, double* angleB) {
	*currentAngle += turn;
	*angleA += turn;
	*angleB += turn;
	vertices[0] = (GLfloat)(*length * cos(*angleA * PI / 180.0) + vertices[6]);
	vertices[1] = (GLfloat)(*length * sin(*angleA * PI / 180.0) + vertices[7]);
	vertices[3] = (GLfloat)(*length * cos(*angleB * PI / 180.0) + vertices[6]);
	vertices[4] = (GLfloat)(*length * sin(*angleB * PI / 180.0) + vertices[7]);
}

void Movement(double xVel, double yVel, GLfloat *vertices, GLfloat* other, float currentAngle) {
	// other[0] = vertices[0] - 0.2; other[1] = vertices[1]; other[3] = vertices[3]- 0.2; 
	// other[4] = vertices[4]; other[6] = vertices[6]- 0.2; other[7] = vertices[7]; 
	
	vertices[0] += (GLfloat)(xVel * cos(currentAngle * PI / 180.0)); vertices[1] += (GLfloat)(yVel * sin(currentAngle * PI / 180.0));
	vertices[3] += (GLfloat)(xVel * cos(currentAngle * PI / 180.0)); vertices[4] += (GLfloat)(yVel * sin(currentAngle * PI / 180.0));
	vertices[6] += (GLfloat)(xVel * cos(currentAngle * PI / 180.0)); vertices[7] += (GLfloat)(yVel * sin(currentAngle * PI / 180.0));

}

void inputDetection(GLFWwindow* window, GLfloat* vertices, GLfloat* other, double* xVel, double* yVel, float* length, float* currentAngle, double* angleA, double* angleB){
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (*xVel < 1000.05) {
			*xVel += 0.001;
		}
		if (*yVel < 1000.05) {
			*yVel += 0.001;
		}
	}else{
		if (*xVel > 0) {
			*xVel -= 0.0005;
		}
		else if (*xVel < 0) {
			*xVel = 0;
		}
		if (*yVel > 0) {
			*yVel -= 0.0005;
		}else if (*yVel < 0) {
			*yVel = 0;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		if (*xVel > 0 || *yVel > 0) {
			Rotate(5, length, vertices, currentAngle, angleA, angleB);
		}

	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		if (*xVel > 0 || *yVel > 0) {
			Rotate(-5, length, vertices, currentAngle, angleA, angleB);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		*xVel = 0;
		*yVel = 0;
	}

	Movement(*xVel, *yVel, vertices, other, *currentAngle);
	//x6,y7
	if (vertices[6] > 1) {
		vertices[6] = -1;//-0.001;
	}
	else if (vertices[6] < -1) {
		vertices[6] = 1;//+0.001;
	}
	if (vertices[7] > 1) {
		vertices[7] = -1;// -0.001;
	}
	else if (vertices[7] < -1) {
		vertices[7] = 1;// +0.001;
	}
	Rotate(0, length, vertices, currentAngle, angleA, angleB);
}

struct Square{
	GLfloat *box;
	bool set;
	Square(){
		set = false;
	}
	Square(GLfloat *in){
		box = in;
		set = true;
	}

	void setBox(GLfloat *in){
		box = in;
		set = true;
	}

};

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
	glewExperimental = true;
	glewInit();

	GLfloat vertices[] = {
		-0.05f, 0.1f, 0.0f, // Lower left corner	PointA
		0.05f, 0.1f, 0.0f, // Lower right corner	PointB
		0.0f, 0.0f, 0.0f // Upper corner			PointC
	};

	GLfloat box[] = {
		// first triangle
		0.05f,  0.05f, 0.0f,  // top right
		0.05f, -0.05f, 0.0f,  // bottom right
		-0.05f,  0.05f, 0.0f,  // top left 
		// second triangle
		0.05f, -0.05f, 0.0f,  // bottom right
		-0.05f, -0.05f, 0.0f,  // bottom left
		-0.05f,  0.05f, 0.0f   // top left
	};

	Square* snake = new Square[100];
	int num = 0;

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	}; 

	float currentAngle = 270;
	float length = sqrt(pow((float)vertices[0], 2.0f) + pow((float)vertices[1], 2.0f));
	double angleA = acos(vertices[0] / length) * 180.0/PI;
	double angleB = acos(vertices[3] / length) * 180.0/PI;
	double xVel = 0;
	double yVel = 0;

	Shader shaderProgram("vertexShaders/default.vert", "fragmentShaders/default.frag");

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);  


    float one = 0.07f, two = 0.13f, three = 0.17f;

    while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(one, two, three, 1.0f);

		inputDetection(window, vertices, box, &xVel, &yVel, &length, &currentAngle, &angleA, &angleB);

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
			num++;
			box[0] += 0.001;
			snake[num].setBox(box);
		}

		shaderProgram.Activate();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		for(int i = 0; i < num; i++){
			if(snake[i].set){
				glBufferData(GL_ARRAY_BUFFER, sizeof(snake[i].box), snake[i].box, GL_DYNAMIC_DRAW);
				glDrawArrays(GL_TRIANGLES, 0, 3);
				glDrawArrays(GL_TRIANGLES, 3, 3);
			}
		}
        glfwSwapBuffers(window);
		glfwPollEvents();
		Sleep(10);

    }
}