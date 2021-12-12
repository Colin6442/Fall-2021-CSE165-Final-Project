#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <math.h>
#include <malloc.h>
#include "shaderClass.h"
#include <time.h>


enum direction{ //Variables hold value to move snake in right direction
	LEFT = -1,
	RIGHT = 1,
	UP = -20,
	DOWN = 20,
};

int currentDirection = RIGHT; //Global variable for which way the snake is currently moving

struct gridSpace{ //The main class of the game 
	//set variables for when gridSpace is initialized
	int index;
	int snakeRemaining = -1;
	bool isSnake = false;
	bool isApple = false;
	bool isSnakeHead = false;
	float x;
	float y;


	//OpenGL Grid
	//|---------------------|
	//|(-1,1) |(0,1) |(1,1) |
	//|-------|------|------|
	//|(-1,0) |(0,0) |(1,0) |
	//|-------|------|------|
	//|(-1,-1)|(0,-1)|(0,-1)|
	//|---------------------|

	//Our Made-Up Grid
	//|------------------|
	//|0***************20|
	//|------------------|
	//|******************|
	//|------------------|
	//|380************400|
	//|------------------|

	GLfloat gameCoords[18]; 
	//The coordinates for OpenGL Grid
	//Length of 18 to create two triangles combined together into a square
	//Each square has an OpenGL Grid size of 0.1 and takes 1 space in made-up grid

	void setGameCoords(int in){ //Take in made-up grid index
		index = in;
		
		//Some fancy math to convert index into OpenGL coordinates
		float xIn = in % 20, yIn = (in - xIn)/20;
		xIn = xIn/10 - 0.899; yIn = yIn/10 - 0.999; yIn = -yIn;

		//Set OpenGL coordinates
			//Triangle 1
		gameCoords[0] = xIn; gameCoords[1] = yIn; gameCoords[2] =  0;
		gameCoords[3] = xIn; gameCoords[4] = yIn - 0.1; gameCoords[5] = 0;
		gameCoords[6] = xIn - 0.1; gameCoords[7] = yIn; gameCoords[8] = 0;
			//Triangle 2
		gameCoords[9] = xIn; gameCoords[10] = yIn - 0.1; gameCoords[11] = 0;
		gameCoords[12] = xIn - 0.1; gameCoords[13] = yIn - 0.1; gameCoords[14] = 0;
		gameCoords[15] = xIn - 0.1; gameCoords[16] = yIn; gameCoords[17] = 0;
	}

	void setSnake(){				//Set grid to draw snake block
		isSnake = true;
	}
	void removeSnake(){				//Remove snake block from gird
		isSnake = false;
	}
	void decrementSnake(){ 			//Decrease TTL of snake body
		snakeRemaining--;
		if(snakeRemaining == 0){
			isSnake = false;
		}
	}
	void setSnakeHead(int length){ 	//Called to set future square as snake head
		isSnakeHead = true;
		snakeRemaining = length;
	}
	void removeSnakeHead(){ 		//Set snake head to false
		isSnakeHead = false;
	}
	void setApple(){ 				//Set grid to draw apple
		isApple = true;
	}
	void removeApple(){ 			//Remove apple block from gird
		isApple = false;
	}


};

void spawnApple(gridSpace* grid){ 	//Spawn apple in random location
	float x = rand() % 20;
	float y = rand() % 20;
	int index = y*20 + x;
	grid[index].setApple();
}


bool setMovement(gridSpace* grid, int* snakeHead, bool* eatApple, int* length){ //Snake Movement
	switch(currentDirection){
		case UP:
			grid[*snakeHead].isSnakeHead = false; //Set current head to false
			if(grid[*snakeHead + UP].isApple){	  //If there is an apple eat it
				*length += 1;
				*eatApple = true;
				grid[*snakeHead + UP].removeApple();
			}
			if(grid[*snakeHead + UP].isSnake){	 //If next space is snake end game
				return true;
			}
			grid[*snakeHead + UP].setSnake();	//Set next space to snake and snake head
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

void Movement(GLFWwindow* window){	//Input Detection
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

void Draw(gridSpace* grid, bool* eatApple){ //Draw function
	for(int i = 0; i < 400; i++){			//Go over each grid space and check if it needs to be drawn
		if(grid[i].isSnake){				//If space is Snake draw it
			glBufferData(GL_ARRAY_BUFFER, sizeof(grid[i].gameCoords), grid[i].gameCoords, GL_STATIC_DRAW);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glDrawArrays(GL_TRIANGLES, 3, 3);
			if(*eatApple){
				spawnApple(grid);
				*eatApple = false;
				grid[i].removeApple();
			}else{
				grid[i].decrementSnake();
			}
		}
		if(grid[i].isApple){				//If space is Apple draw it 
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
	gridSpace* grid = new gridSpace[400];
	for(int i = 0; i < 400; i++){
		grid[i].setGameCoords(i);
	}

	//Initial snake set up
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

	//Set shaders
	Shader snakeShader("vertexShaders/default.vert", "fragmentShaders/snake.frag");
	snakeShader.Activate();

	//Set VAOs and VBOs
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	unsigned int VBO;
	glGenBuffers(3, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);  

	//Set background color
    float one = 0.07f, two = 0.13f, three = 0.17f;

    //Game Loop
    while (!glfwWindowShouldClose(window)) {
    	//Reset Screen
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(one, two, three, 1.0f);
		
		//Check key presses and change direction
		Movement(window);

		//Run movement and end game loop if snake collides with itself
		if(setMovement(grid, &snakeHead, &eatApple, &length)){
			break;
		}

		//Draw nescessary grid spaces
		Draw(grid, &eatApple);

		//If out of bounds end game
		if(snakeHead < 0 || snakeHead > 399){
			break;
		}

		//Set new drawn screen
        glfwSwapBuffers(window);
		glfwPollEvents();
		Sleep(100);

    }
}