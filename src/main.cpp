#include <cstdio>

#include <glm/glm.hpp>
#include <stb_image.h>

#include <src/engine/graphics/model.h>

//#define TESTING

#if defined(TESTING)

#include <src/testing/tests.h>

#define SIZE_DUMP(type) fprintf(stderr, "sizeof(%s): %llu\n", #type, sizeof(type))

int main(int argc, char** args) {
	Test();

	std::shared_ptr<Model> model = Model::CreateModel("./resources/models/capsule.obj");

	return 0;
}
#else

#include "src/engine/states/game_state.h"
#include "src/engine/graphics/window.h"
#include "src/engine/body_systems/body.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Global Variables to Track Keys and Mouse
Key Last_Key_Pressed, Last_Key_Released, Last_Key_Repeated;
MouseButton Last_MouseButton_Pressed, Last_MouseButton_Released;
float Mouse_X, Mouse_Y;

//GameState for menu state
class MenuState : public GameState {
public:
	const std::string n;

	MenuState(const std::string& name) : GameState(name), n(name) {}

	virtual void render(float delta) {}
	virtual void update(float delta) {}

	virtual bool onKeyPressed(const Key& key) {
		Last_Key_Pressed = key;
		return true;
	}

	virtual bool onKeyReleased(const Key& key) {
		Last_Key_Released = key;
		return true;
	}

	virtual bool onKeyRepeated(const Key& key) {
		Last_Key_Repeated = key;
		return true;
	}

	virtual bool onMouseButtonPressed(const MouseButton& button) {
		Last_MouseButton_Pressed = button;
		return true;
	}

	virtual bool onMouseButtonReleased(const MouseButton& button) {
		Last_MouseButton_Released = button;
		return true;
	}

	virtual bool onMouseMoved(const float x, const float y, const float dx, const float dy) {
		Mouse_X = x;
		Mouse_Y = y;
		return true;
	}

};

//GameState for Game State
class Game : public MenuState{
public:
	const std::string n;

	Game(const std::string& name) : MenuState(name), n(name) {}

	virtual void render(float delta) {}
	virtual void update(float delta) {}

	virtual bool onKeyPressed(const Key& key) {
		Last_Key_Pressed = key;
		return true;
	}

	virtual bool onKeyReleased(const Key& key) {
		Last_Key_Released = key;
		return true;
	}

	virtual bool onKeyRepeated(const Key& key) {
		Last_Key_Repeated = key;
		return true;
	}

	virtual bool onMouseButtonPressed(const MouseButton& button) {
		Last_MouseButton_Pressed = button;
		return true;
	}

	virtual bool onMouseButtonReleased(const MouseButton& button) {
		Last_MouseButton_Released = button;
		return true;
	}

	virtual bool onMouseMoved(const float x, const float y, const float dx, const float dy) {
		Mouse_X = x;
		Mouse_Y = y;
		return true;
	}

};

//Shaders
const char* vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 vertexPosition_modelspace;\n"
	"uniform mat4 MVP;\n"
	"void main() {\n"
	"   gl_Position = MVP * vec4(vertexPosition_modelspace, 1);\n"
	"}\0";
const char* fragmentShaderSource =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform int drawColor;\n"
	"void main() {\n"
	"	vec4 newColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"	if(drawColor == 0){\n"
	"		newColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"	}else if(drawColor == 1){\n"
	"		newColor = vec4(0.5f, 1.0f, 0.2f, 1.0f);\n"
	"	}else if(drawColor == 2){\n"
	"		newColor = vec4(0.5f, 0.2f, 1.0f, 1.0f);\n"
	"	}\n"
	"   FragColor = newColor;\n"
	"}\0";

//Prototypes
void processInput(GLFWwindow *);

//Main Loop
int main(int argc, char** args) {
	//Hello World
	printf("Hello World!\n");

	//Initialize The GameStates
	printf("Initializing GameState\n");
	std::shared_ptr<GameState> menuState = GameState::CreateState<MenuState>("MainMenu");
	std::shared_ptr<GameState> gameState = GameState::CreateState<Game>("Project Space");
	State::ChangeState(menuState);

	//Create and Open the Main Menu Window
	printf("Opening Window\n");
	std::shared_ptr<Window> window = Window::CreateGLWindow("Project Space", 800, 600);

	//Model, Projection, and ModelViewProjection Matrices
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)window->width / (float)window->height, 0.1f, 100.0f);
	glm::mat4 View = glm::lookAt(glm::vec3(4.f,3.f,3.0f), glm::vec3(0.f,0.f,0.f), glm::vec3(0.f,1.f,0.f));
	glm::mat4 Model = glm::mat4(1.0f);

	int drawColor = 0;
	float cameraX = 0.0f;
	float cameraZ = 0.0f;

	//Build and Compile Shaders
	int success;
	char infoLog[512];
	//Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR [Vertex Shader]: Failed to Compile\n%s", infoLog);
	}

	//Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR [Fragment Shader]: Failed to compile\n%s", infoLog);
	}

	//Link Shaders
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR [Shader Program]: Failed to Link\n%s", infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Give Matrices to GLSL
	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
	GLuint DrawColorID = glGetUniformLocation(shaderProgram, "drawColor");

	//Box Data
	std::vector<unsigned int> indices = {
		0, 1, 2, //Z+
		2, 0, 3,
		1, 5, 6, //X+
		6, 1, 2,
		5, 4, 7, //Z-
		7, 5, 6,
		4, 0, 3, //X-
		3, 4, 7,
		4, 5, 1, //Y+
		1, 4, 0,
		3, 2, 6, //Y-
		6, 3, 7
	};

	float vertices[] = {
		-1.f,  1.f,  1.f ,	//0
		 1.f,  1.f,  1.f ,	//1
		 1.f, -1.f,  1.f ,	//2
		-1.f, -1.f,  1.f ,	//3
		-1.f,  1.f, -1.f ,	//4
		 1.f,  1.f, -1.f ,	//5
		 1.f, -1.f, -1.f ,	//6
		-1.f, -1.f, -1.f	//7
	};

	//Set Vertex Array & Buffers
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//Set Element Array Buffer
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	/*
	MAIN LOOP
	WHILE THE WINDOW IS OPEN 
		1st - Process Key Input By State
		2nd - Render The Window
		3rd - Draw The Scene
		4th - Poll Events and Swap Buffers
	*/

	while (!glfwWindowShouldClose(window->getWindowPtr())) {
		//Process Input Based on State
		if (State::CurrentState == menuState) {
			switch (Last_Key_Pressed) {
			//Escape Key Close Window
			case GLFW_KEY_ESCAPE:
				printf("Closing Window\n");
				glfwSetWindowShouldClose(window->getWindowPtr(), true);
				break;
			//Space Key Change to Game State
			case GLFW_KEY_SPACE:
				printf("Changing State to gameState\n");
				State::ChangeState(gameState);
				break;
			default:
				break;
			}
		}else if(State::CurrentState == gameState) {
			switch (Last_Key_Pressed) {
			//Escape Key Change to Menu State
			case GLFW_KEY_ESCAPE:
				printf("Changing State to menuState\n");
				State::ChangeState(menuState);
				break;
			case GLFW_KEY_0:
				printf("gameState: changing drawColor to 0\n");
				drawColor = 0;
				break;
			case GLFW_KEY_1:
				printf("gameState: changing drawColor to 1\n");
				drawColor = 1;
				break;
			case GLFW_KEY_2:
				printf("gameState: changing drawColor to 2\n");
				drawColor = 2;
				break;
			case GLFW_KEY_W:
				cameraZ += 0.030f;
				break;
			case GLFW_KEY_A:
				cameraX -= 0.030f;
				break;
			case GLFW_KEY_S:
				cameraZ -= 0.030f;
				break;
			case GLFW_KEY_D:
				cameraX += 0.030f;
				break;
			default:
				break;
			}
		}
		Last_Key_Pressed = NULL;
		//I want to change the above into a function [processInput]
		// for each state [CurrentState] to allow the following function call
		//This could allow for custom keybinds per GameState

		//State::CurrentState.processInput(window->getWindowPtr());



		//Render
		if (State::CurrentState == menuState) {
			glClearColor(0.f, 0.f, 0.f, 1.f);
		}else if (State::CurrentState == gameState) {
			glClearColor(0.f, 0.1f, 0.15f, 1.f);
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//State::CurrentState.Render();



		//Draw
		if (State::CurrentState == gameState) {
			View = glm::lookAt(glm::vec3(4.f + cameraX, 3.f, 3.0f + cameraZ), glm::vec3(0.f + cameraX, 0.f, 0.f + cameraZ), glm::vec3(0.f, 1.f, 0.f));
			glm::mat4 mvp = Projection * View * Model;

			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
			glUniform1i(DrawColorID, drawColor);

			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
		}
		//Could change above to function for state [CurrentState]
		//State::CurrentState.Draw();
		


		//Check for Events and Swap Buffers
		glfwSwapBuffers(window->getWindowPtr());
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	window.~shared_ptr();
	glfwTerminate();
	return 0;
}

#endif
