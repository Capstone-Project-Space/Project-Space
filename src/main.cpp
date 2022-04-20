#include <cstdio>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <src/engine/states/game_state.h>

#include <src/engine/graphics/model.h>
#include <src/engine/graphics/buffers.h>
#include <src/engine/graphics/vertex_array.h>
#include <src/engine/graphics/shader.h>
#include <src/engine/graphics/window.h>
#include <src/engine/graphics/renderer.h>
#include <src/engine/graphics/asset_manager.h>

#include <src/engine/graphics/ui/text_component.h>
#include <src/engine/graphics/ui/constraint_layout.h>

#include <src/engine/randgen/randomgen.h>

#include <src/engine/body_systems/body_system.h>

#define TESTING
#define DEBUG

using Clock = std::chrono::high_resolution_clock;

#if defined(TESTING)

#include <src/testing/tests.h>

#define SIZE_DUMP(type) fprintf(stderr, "sizeof(%s): %llu\n", #type, sizeof(type))

#define LOG_GL_ERROR for (int glErrorGL = glGetError(); glErrorGL != 0;) { fprintf(stderr, "GLError: %d\n", glErrorGL); assert(false);}


std::shared_ptr<GameState> menuState = nullptr, playState = nullptr;

constexpr int count = 64;
class TempState : public GameState {

	Camera orthoCamera{ -640.0f, 640.0f, -360.0f, 360.0f };
	Camera perspectiveCamera{ 1280.0f, 720.0f, 70.0f, .01f, 1000.0f };

	glm::mat4 transforms[count];

public:
	TempState(const std::shared_ptr<Window> window, const std::string& name) : GameState(window, name) {
		for (int i = 0; i < count; i++) {
			transforms[i] = glm::translate(glm::scale(glm::identity<glm::mat4>(), { .1f, .1f, .1f }), { RandomGen::RangedRandomFloat(-12, 12), -.9f, RandomGen::RangedRandomFloat(-7, 7)});
		}

		componentManager.addComponent(new TextComponent(
			std::string_view{ "simple_text" },
			new ConstraintLayout(std::string_view{"window:top"}, std::string_view{""}, std::string_view{""}, std::string_view{"window:right"}, std::string_view{"window"}),
			std::string{ "This is some text." },
			AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf")
		));
	}

	virtual void update(float delta) override { }

	virtual void render(float delta) override {
		Renderer::Begin2DScene(orthoCamera);
		{
			Renderer::SubmitQuad({ 0.0f, 0.0f }, window->getData().size, AssetManager::GetOrCreate<Texture>("./resources/textures/projectspacefull.png"));
		}
		Renderer::End2DScene();

		Renderer::Begin3DScene(perspectiveCamera);
		{
			Renderer::SubmitLightSource({{0.0f, 10.0f, 0.0f, 1.0f}});
			Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/freight.obj"), transforms[0]);
		}
		Renderer::End3DScene();

		
		componentManager.drawComponents(delta, orthoCamera);

	}

	virtual void onWindowResize(float oldWidth, float oldHeight, float newWidth, float newHeight) override {
		this->orthoCamera = {newWidth / -2.0f, newWidth / 2.0f, newHeight / -2.0f, newHeight / 2.0f};
		this->perspectiveCamera = { newWidth, newHeight, 70.0f, .01f, 1000.0f };
	}

	//Handle Single Button Press
	virtual bool onKeyPressed(const Key& key) {
		if (key == GLFW_KEY_ESCAPE) {
			printf("Escape Key Was Pressed\n");
			//HOW DO I ESCAPE THE PROGRAM!?
			State::Close();	//Causes a crash at game_state.cpp:49
		}
		if (key == GLFW_KEY_SPACE) {
			printf("Space Key Was Pressed\n");
			State::ChangeState(playState);
		}
		return true;
	}
};

class PlayState : public GameState {
	Camera orthoCamera{ -640.0f, 640.0f, -360.0f, 360.0f };
	Camera perspectiveCamera{ 1280.0f, 720.0f, 70.0f, .01f, 1000.0f };

	std::shared_ptr<BodySystem> system;
public:
	PlayState(const std::shared_ptr<Window> window, const std::string& name) : GameState(window, name) {
		system = std::shared_ptr<BodySystem>(new BodySystem(0));

#if defined(DEBUG)
		printf("DEBUG MODE -- PRINTING SYSTEM DATA\n");
		system->printDebugInfo();
#endif
	}

	virtual void update(float delta) override { }

	virtual void render(float delta) override {
		Renderer::Begin3DScene(perspectiveCamera);
		{
			//Render Light source at star
			Renderer::SubmitLightSource({ {0.0f, 0.0f, 0.0f, 1.0f} });
			
			//Render Sun
			Star star = system->getStar();
			Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/16x16.obj"),
				glm::translate(
					glm::scale(
						glm::identity<glm::mat4>(),
						glm::vec3(star.star->getScale())),
					star.star->getPosition()));

			//Render all system bodies
			std::vector<std::shared_ptr<Body>> bodies = system->getBodyList();
			for (auto i : bodies) {
				Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/16x16.obj"),
					glm::translate(
						glm::scale(
							glm::identity<glm::mat4>(),
							glm::vec3(i->getScale())),
						i->getPosition()));
			}
		}
		Renderer::End3DScene();


		componentManager.drawComponents(delta, orthoCamera);
	}

	virtual void onWindowResize(float oldWidth, float oldHeight, float newWidth, float newHeight) override {
		this->orthoCamera = { newWidth / -2.0f, newWidth / 2.0f, newHeight / -2.0f, newHeight / 2.0f };
		this->perspectiveCamera = { newWidth, newHeight, 70.0f, .01f, 1000.0f };
	}

	virtual bool onKeyPressed(const Key& key) {
		if (key == GLFW_KEY_ESCAPE) {
			printf("PlayState: Escape Key Pressed -- Ending PlayState\n");
			State::ResetStateTo(menuState);
		}
		return true;
	}
};

int main(int argc, char** args) {
	// Test();
	std::shared_ptr<Window> window = Window::CreateGLWindow("Model Test", 1280, 720);

	menuState = GameState::CreateState<TempState>(window, std::string{ "Temporary State" });
	playState = GameState::CreateState<PlayState>(window, std::string{ "Play Test State" });
	LOG_GL_ERROR;
	
	State::ChangeState(menuState);

	LOG_GL_ERROR;
	glEnable(GL_BLEND);
	LOG_GL_ERROR;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	LOG_GL_ERROR;
	glEnable(GL_DEPTH_TEST);
	LOG_GL_ERROR;
	glClearColor(.12f, .12f, .12f, 1.0f);
	LOG_GL_ERROR;
	double lastTime = Clock::now().time_since_epoch().count();
	double currentTime, timeSpent = 0;
	double frames = 0.0;

	while (window->getData().isOpen) {
		currentTime = Clock::now().time_since_epoch().count();
		double delta = (currentTime - lastTime) / 1000000000.0;
		lastTime = currentTime;
		timeSpent += delta;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		LOG_GL_ERROR;

		State::Draw(delta);

		glfwPollEvents();
		window->flush();
		frames++;
		if (timeSpent >= 1.0) {
			double fps = frames / timeSpent;
			const std::string frameRate = std::to_string(fps);
			window->updateTitle(" Frames: " + frameRate.substr(0, frameRate.find_first_of('.') + 3));
			timeSpent -= 1.0;
			frames -= fps;
		}
	}

	State::Close();
	AssetManager::Clear();
	Renderer::Shutdown();
	return 0;
}
#else

#include "src/engine/states/game_state.h"
#include "src/engine/graphics/window.h"
#include "src/engine/body_systems/body.h"
#include "src/engine/io/keyboard.h"
#include "src/engine/io/mouse.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Global Variables to Track Keys and Mouse
Key Last_Key_Pressed, Last_Key_Released, Last_Key_Repeated;
MouseButton Last_MouseButton_Pressed, Last_MouseButton_Released;
float Mouse_X, Mouse_Y;

GLuint VAO, VBO;
unsigned int shaderProgram;

std::shared_ptr<Window> window;

std::shared_ptr<GameState> gameState;

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


//GameState for Game State
class Game : public GameState{
public:
	const std::string n;

	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
	glm::mat4 mvp;

	int success;
	char infoLog[512];

	unsigned int vertexShader;
	unsigned int fragmentShader;

	GLuint MatrixID;
	GLuint DrawColorID;
	GLuint elementBuffer;

	int drawColor;
	float cameraX;
	float cameraZ;

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

	float vertices[24] = {
		-1.f,  1.f,  1.f ,	//0
		 1.f,  1.f,  1.f ,	//1
		 1.f, -1.f,  1.f ,	//2
		-1.f, -1.f,  1.f ,	//3
		-1.f,  1.f, -1.f ,	//4
		 1.f,  1.f, -1.f ,	//5
		 1.f, -1.f, -1.f ,	//6
		-1.f, -1.f, -1.f	//7
	};

	//Constructor
	Game(const std::string& name) : GameState(name), n(name) {

		Projection = glm::perspective(glm::radians(45.0f), (float)window->width / (float)window->height, 0.1f, 100.0f);
		View = glm::lookAt(glm::vec3(4.f, 3.f, 3.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
		Model = glm::mat4(1.0f);
		mvp = glm::mat4(1.0f);

		drawColor = 0;
		cameraX = 0.0f;
		cameraZ = 0.0f;

		*infoLog = NULL;

		//Build and Compile Shaders
		//Vertex Shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			fprintf(stderr, "ERROR [Vertex Shader]: Failed to Compile\n%s", infoLog);
			exit(1);
		}

		//Fragment Shader
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			fprintf(stderr, "ERROR [Fragment Shader]: Failed to compile\n%s", infoLog);
			exit(1);
		}

		//Link Shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			fprintf(stderr, "ERROR [Shader Program]: Failed to Link\n%s", infoLog);
			exit(1);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//Give Matrices to GLSL
		MatrixID = glGetUniformLocation(shaderProgram, "MVP");
		DrawColorID = glGetUniformLocation(shaderProgram, "drawColor");

		//Set Vertex Array & Buffers
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
		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	}

	virtual void render(float delta) {
		glClearColor(0.f, 0.1f, 0.15f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		View = glm::lookAt(glm::vec3(4.f + cameraX, 3.f, 3.0f + cameraZ), glm::vec3(0.f + cameraX, 0.f, 0.f + cameraZ), glm::vec3(0.f, 1.f, 0.f));
		mvp = Projection * View * Model;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
		glUniform1i(DrawColorID, drawColor);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

	}

	virtual void update(float delta) {
		if (Keyboard::isKeyDown(GLFW_KEY_ESCAPE)) {
			printf("Closing Window\n");
			glfwSetWindowShouldClose(window->getWindowPtr(), true);
		}
		if (Keyboard::isKeyDown(GLFW_KEY_W)) {
			cameraZ += (1.f * delta);
		}
		if (Keyboard::isKeyDown(GLFW_KEY_A)) {
			cameraX += (1.f * delta);
		}
		if (Keyboard::isKeyDown(GLFW_KEY_S)){
			cameraZ -= (1.f * delta);
		}
		if (Keyboard::isKeyDown(GLFW_KEY_D)) {
			cameraX -= (1.f * delta);
		}
		printf("Delta: %f\n", delta);
	}

	virtual bool onKeyPressed(const Key& key) {
		if (key == GLFW_KEY_0) {
			printf("gameState: changing drawColor to 0\n");
			drawColor = 0;
		}
		if (key == GLFW_KEY_1) {
			printf("gameState: changing drawColor to 1\n");
			drawColor = 1;
		}
		if (key == GLFW_KEY_2) {
			printf("gameState: changing drawColor to 2\n");
			drawColor = 2;
		}
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

//Main Loop
int main(int argc, char** args) {
	uint32_t frames = 0;
	double lastTime = Clock::now().time_since_epoch().count();
	double currentTime, timeSpent = 0;

	//Create and Open the Main Menu Window
	printf("Opening Window\n");
	window = Window::CreateGLWindow("Project Space", 800, 600);

	//Initialize The GameStates
	printf("Initializing GameState\n");
	gameState = GameState::CreateState<Game>("Project Space");

	State::ChangeState(gameState);

	//Model, Projection, and ModelViewProjection Matrices

	/*
	MAIN LOOP
	WHILE THE WINDOW IS OPEN 
		1st - Process Key Input By State
		2nd - Render The Window
		3rd - Draw The Scene
		4th - Poll Events and Swap Buffers
	*/

	while (!glfwWindowShouldClose(window->getWindowPtr())) {
		currentTime = Clock::now().time_since_epoch().count();
		double delta = (currentTime - lastTime) / 1000000000.0;
		lastTime = currentTime;
		timeSpent += delta;

		//Update
		State::CurrentState->update(delta);

		//Render
		State::CurrentState->render(delta);

		//Check for Events and Swap Buffers
		glfwSwapBuffers(window->getWindowPtr());
		glfwPollEvents();
		window->flush();
		frames++;
		if (timeSpent >= 1.0) {
			window->updateTitle(" Frames: " + std::to_string(frames));
			timeSpent -= 1.0;
			frames = 0;
		}
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	window.~shared_ptr();
	glfwTerminate();
	return 0;
}

#endif