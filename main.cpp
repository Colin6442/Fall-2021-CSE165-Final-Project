#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <math.h>
#include <malloc.h>
#include "shaderClass.h"
#include <time.h>
#include "soil\inc\SOIL\SOIL.h"

float detCollide(float ax, float ay, float bx, float by){
	float dx = ax - bx;
	float dy = ay - by;
	float distance = sqrt((dx*dx)+(dy*dy));
	return distance; // for collision in structs.
}

struct gridSpace{
	int index;
	int snakeRemaining = -1;
	bool isSnake = false;
	bool isApple = false;
	bool isSnakeHead = false;
	float x;
	float y;

	GLfloat gameCoords[18];
	void setGameCoords(int in){
		index = in;
		float xIn = in % 20, yIn = (in - xIn)/20;
		xIn = xIn/10 - 0.899; yIn = yIn/10 - 0.999; yIn = -yIn;
		gameCoords[0] = xIn; gameCoords[1] = yIn; gameCoords[2] =  0;
		gameCoords[3] = xIn; gameCoords[4] = yIn - 0.1; gameCoords[5] = 0;
		gameCoords[6] = xIn - 0.1; gameCoords[7] = yIn; gameCoords[8] = 0;

		gameCoords[9] = xIn; gameCoords[10] = yIn - 0.1; gameCoords[11] = 0;
		gameCoords[12] = xIn - 0.1; gameCoords[13] = yIn - 0.1; gameCoords[14] = 0;
		gameCoords[15] = xIn - 0.1; gameCoords[16] = yIn; gameCoords[17] = 0;
	}

	void setSnake(){
		isSnake = true;
	}
	void removeSnake(){
		isSnake = false;
	}
	void decrementSnake(){
		snakeRemaining--;
		if(snakeRemaining == 0){
			isSnake = false;
		}
	}
	void setSnakeHead(int length){
		isSnakeHead = true;
		snakeRemaining = length;
	}
	void removeSnakeHead(){
		isSnakeHead = false;
	}
	void setApple(){
		isApple = true;
	}
	void removeApple(){
		isApple = false;
	}


};

void spawnApple(gridSpace* grid){
	float x = rand() % 20;
	float y = rand() % 20;
	int index = y*20 + x;
	grid[index].setApple();
}

bool setMovement(gridSpace* grid, int* snakeHead, bool* eatApple, int* length){
	switch(currentDirection){
		case UP:
			grid[*snakeHead].isSnakeHead = false;
			if(grid[*snakeHead + UP].isApple){
				*length += 1;
				*eatApple = true;
				grid[*snakeHead + UP].removeApple();
			}
			if(grid[*snakeHead + UP].isSnake){
				return true;
			}
			grid[*snakeHead + UP].setSnake();
			grid[*snakeHead + UP].setSnakeHead(*length);
			*snakeHead += UP;
			break;

		case DOWN:
			grid[*snakeHead].isSnakeHead = false;
			if(grid[*snakeHead + DOWN].isApple){
				*length += 1;
				*eatApple = true;
				grid[*snakeHead + DOWN].removeApple();
			}
			if(grid[*snakeHead + DOWN].isSnake){
				return true;
			}
			grid[*snakeHead + DOWN].setSnake();
			grid[*snakeHead + DOWN].setSnakeHead(*length);
			*snakeHead += DOWN;
			break;

		case LEFT:
			grid[*snakeHead].isSnakeHead = false;
			if(grid[*snakeHead + LEFT].isApple){
				*length += 1;
				*eatApple = true;
				grid[*snakeHead - 1].removeApple();
			}
			if(grid[*snakeHead + LEFT].isSnake){
				return true;
			}
			grid[*snakeHead + LEFT].setSnake();
			grid[*snakeHead + LEFT].setSnakeHead(*length);
			*snakeHead -= 1;
			break;

		case RIGHT:
			grid[*snakeHead].isSnakeHead = false;
			if(grid[*snakeHead + RIGHT].isApple){
				*length += 1;
				*eatApple = true;
				grid[*snakeHead + RIGHT].removeApple();
			}
			if(grid[*snakeHead + RIGHT].isSnake){
				return true;
			}
			grid[*snakeHead + RIGHT].setSnake();
			grid[*snakeHead + RIGHT].setSnakeHead(*length);
			*snakeHead += 1;
			break;
	}
	return false;
}

void Movement(GLFWwindow* window){
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		currentDirection = UP;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		currentDirection = DOWN;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		currentDirection = LEFT;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		currentDirection = RIGHT;
	}
}

void Draw(gridSpace* grid, bool* eatApple){
	for(int i = 0; i < 400; i++){
		if(grid[i].isSnake){
			glBufferData(GL_ARRAY_BUFFER, sizeof(grid[i].gameCoords), grid[i].gameCoords, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawArrays(GL_TRIANGLES, 3, 3);
			if(*eatApple){
				spawnApple(grid);
				*eatApple = false;
				//printf("apple\n");
				grid[i].removeApple();
			}else{
				grid[i].decrementSnake();
			}
		}
		if(grid[i].isApple){
			glBufferData(GL_ARRAY_BUFFER, sizeof(grid[i].gameCoords), grid[i].gameCoords, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawArrays(GL_TRIANGLES, 3, 3);
		}
	}
	
}

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

	int length = 4;
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

	gridSpace* grid = new gridSpace[400];
	for(int i = 0; i < 400; i++){
		grid[i].setGameCoords(i);
	}
	grid[190].setSnake();
	grid[189].setSnake();
	grid[188].setSnake();
	grid[187].setSnake();
	grid[190].setSnakeHead(length);
	grid[189].setSnakeHead(length);
	grid[188].setSnakeHead(length);
	grid[187].setSnakeHead(length);
	grid[48].setApple();

	int snakeHead = 190;
	bool eatApple = false;

	Shader snakeShader("vertexShaders/default.vert", "fragmentShaders/snake.frag");
	snakeShader.Activate();

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(3, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);  

    float one = 0.07f, two = 0.13f, three = 0.17f;

    while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(one, two, three, 1.0f);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		Movement(window);

		if(setMovement(grid, &snakeHead, &eatApple, &length)){
			break;
		}

		Draw(grid, &eatApple);

		if(snakeHead < 0 || snakeHead > 399){
			break;
		}

        glfwSwapBuffers(window);
		glfwPollEvents();
		Sleep(100);

    }
}