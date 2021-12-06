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
		box[1] += 0.1; box[4] += 0.1; box[7] += 0.1;
		box[10] += 0.1; box[13] += 0.1; box[16] += 0.1; 
	}

	void moveDown(){
		box[1] -= 0.1; box[4] -= 0.1; box[7] -= 0.1;
		box[10] -= 0.1; box[13] -= 0.1; box[16] -= 0.1; 
	}

	void moveLeft(){
		box[0] -= 0.1; box[3] -= 0.1; box[6] -= 0.1;
		box[9] -= 0.1; box[12] -= 0.1; box[15] -= 0.1; 
	}

	void moveRight(){
		box[0] += 0.1; box[3] += 0.1; box[6] += 0.1;
		box[9] += 0.1; box[12] += 0.1; box[15] += 0.1;
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

float detCollide(float ax, float ay, float bx, float by){
	float dx = ax - bx;
	float dy = ay - by;
	float distance = sqrt((dx*dx)+(dy*dy));
	return distance; // for collision in structs.
}

struct gridSpace{
	bool isSnake = false;
	bool isApple = false;
	float x;
	float y;

	GLfloat gameCoords[18];

	void setGameCoords(int index){
		float xIn = index % 20, yIn = (index - xIn)/20;
		xIn = xIn/10 - 0.899; yIn = yIn/10 - 0.999; yIn = -yIn;
		gameCoords[0] = xIn; gameCoords[1] = yIn; gameCoords[2] =  0;
		gameCoords[3] = xIn; gameCoords[4] = yIn - 0.1; gameCoords[5] = 0;
		gameCoords[6] = xIn - 0.1; gameCoords[7] = yIn; gameCoords[8] = 0;

		gameCoords[9] = xIn; gameCoords[10] = yIn - 0.1; gameCoords[11] = 0;
		gameCoords[12] = xIn - 0.1; gameCoords[13] = yIn - 0.1; gameCoords[14] = 0;
		gameCoords[15] = xIn - 0.1; gameCoords[16] = yIn; gameCoords[17] = 0;
	}
	//yin * 20 + xin
	void setSnake(){
		isSnake = true;
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


	Square* snake = new Square[400];
	gridSpace* grid = new gridSpace[400];
	for(int i = 0; i < 400; i++){
		grid[i].setGameCoords(i);
		if(i < 400){
			grid[i].setSnake();
		}
	}

	grid[0].setSnake();

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
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
			break;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
			snake[0].moveUp();
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			snake[0].moveDown();
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			snake[0].moveLeft();
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
			snake[0].moveRight();
		}
		
		
		shaderProgram.Activate();
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// for(int i = 0; i < num; i++){
		// 	glBufferData(GL_ARRAY_BUFFER, sizeof(snake[i].box), snake[i].box, GL_STATIC_DRAW);
		// 	glDrawArrays(GL_TRIANGLES, 0, 3);
		// 	glDrawArrays(GL_TRIANGLES, 3, 3);
		// }

		for(int i = 0; i < 400; i++){
			if(grid[i].isSnake){
				//printf("%f\n", i);
				glBufferData(GL_ARRAY_BUFFER, sizeof(grid[i].gameCoords), grid[i].gameCoords, GL_STATIC_DRAW);
				glDrawArrays(GL_TRIANGLES, 0, 3);
				glDrawArrays(GL_TRIANGLES, 3, 3);
			}
		}

        glfwSwapBuffers(window);
		glfwPollEvents();
		Sleep(10);

    }
}