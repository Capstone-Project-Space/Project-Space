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
#include <src/engine/graphics/ui/button_component.h>
#include <src/engine/graphics/ui/constraint_layout.h>
#include <src/engine/graphics/ui/explicit_layout.h>
#include <src/engine/graphics/ui/relative_layout.h>

#include <src/gameplay/events/event.h>
#include <src/gameplay/states/main_menu_state.h>
#include <src/gameplay/states/new_game_state.h>
#include <src/gameplay/states/load_game_state.h>
#include <src/gameplay/states/settings_state.h>

#define DEBUG
#define RUN_TESTS

using Clock = std::chrono::high_resolution_clock;

#include <src/testing/tests.h>

#define SIZE_DUMP(type) fprintf(stderr, "sizeof(%s): %llu\n", #type, sizeof(type))

#define LOG_GL_ERROR for (int glErrorGL = glGetError(); glErrorGL != 0;) { fprintf(stderr, "GLError: %d\n", glErrorGL); assert(false);}

//class PlayState : public GameState {
//
//	CameraObject gameCamera{ {1280.0f, 720.0f}, 70.0f, .01f, 1000.0f, glm::vec3(10.0f), glm::vec3{0.0f}, glm::vec3(50.0f) };
//
//	std::shared_ptr<BodySystem> system;
//	bool mouse1Down = false, mouse2Down = false, lShiftMod = false, rShiftMod = false, lAltMod = false;
//	float delta;
//
//	float mouseX, mouseY, mouseDX, mouseDY;
//
//	float mouse1DownTime, doubleClick1Time;
//	bool doubleClick1Check;
//
//	std::shared_ptr<void> targetObject;
//
//public:
//	PlayState(const std::shared_ptr<Window> window, const std::string& name) : GameState(window, name) {
//		system = std::shared_ptr<BodySystem>(new BodySystem(0));
//		gameCamera.focusOn({ 0.0f, 0.0f, 0.0f });
//
//		mouse1DownTime = 0.0f;
//		doubleClick1Time = 0.0f;
//		doubleClick1Check = false;
//		#if defined(DEBUG)
//			printf("DEBUG MODE -- PRINTING SYSTEM DATA\n");
//			system->printDebugInfo();
//		#endif
//	}
//
//	virtual void update(float delta) override {
//		if (doubleClick1Check) {
//			if (doubleClick1Time > 0.15f) {
//				doubleClick1Check = false;
//				doubleClick1Time = 0.0f;
//			}
//			else {
//				doubleClick1Time += delta;
//			}
//		}
//
//		if (mouse1Down) {
//			if (mouse1DownTime < 1.0f) mouse1DownTime += delta;
//			if (mouse1DownTime > 1.0f) mouse1DownTime = 1.0f;
//		}
//
//
//		if (!console->getVisible()) {
//			if (Keyboard::IsKeyDown(GLFW_KEY_UP)) {
//				if (gameCamera.getTarget() == std::nullopt) {
//					gameCamera.move(1.5 * delta, MoveDirection::FORWARDS);
//				}
//				else {
//
//				}
//			}
//			if (Keyboard::IsKeyDown(GLFW_KEY_LEFT)) {
//				if (gameCamera.getTarget() == std::nullopt) {
//					gameCamera.move(1.5 * delta, MoveDirection::LEFT);
//				}
//			}
//			if (Keyboard::IsKeyDown(GLFW_KEY_DOWN)) {
//				if (gameCamera.getTarget() == std::nullopt) {
//					gameCamera.move(1.5 * delta, MoveDirection::BACKWARDS);
//				}
//			}
//			if (Keyboard::IsKeyDown(GLFW_KEY_RIGHT)) {
//				if (gameCamera.getTarget() == std::nullopt) {
//					gameCamera.move(1.5 * delta, MoveDirection::RIGHT);
//				}
//			}
//		}
//		this->delta = delta;
//	}
//
//	virtual void render(float delta) override {
//		Renderer::Begin3DScene(gameCamera.getCamera());{
//			//Submit Skybox
//
//			//front
//			Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/plane.obj", AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/front.bmp")),
//				glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::identity<glm::mat4>(),
//					{ 0.0f, 0.0f, -50.0f }),										//position
//					(float)(-90.0f * (M_PI / 180.0f)), glm::vec3{ 1.0f, 0.0f, 0.0f }),	//x rotation
//					(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 1.0f, 0.0f }),		//y rotation
//					(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 0.0f, 1.0f }),		//z rotation
//					glm::vec3{ 50.0f, 1.0f, 50.0f }),								//scale
//				glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });								//color
//
//			//back
//			Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/plane.obj", AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/back.bmp")),
//				glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::identity<glm::mat4>(),
//					{ 0.0f, 0.0f, 50.0f }),										//position
//					(float)(90.0f * (M_PI / 180.0f)), glm::vec3{ 1.0f, 0.0f, 0.0f }),	//x rotation
//					(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 1.0f, 0.0f }),		//y rotation
//					(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 0.0f, 1.0f }),		//z rotation
//					glm::vec3{ 50.0f, 1.0f, 50.0f }),								//scale
//				glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });								//color
//
//			//left
//			Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/plane.obj", AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/left.bmp")),
//				glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::identity<glm::mat4>(),
//					{ -50.0f, 0.0f, 0.0f }),										//position
//					(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 1.0f, 0.0f, 0.0f }),		//x rotation
//					(float)(90.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 1.0f, 0.0f }),	//y rotation
//					(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 0.0f, 1.0f }),		//z rotation
//					glm::vec3{ 50.0f, 1.0f, 50.0f }),								//scale
//				glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });								//color
//
//			//right
//			Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/plane.obj", AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/right.bmp")),
//				glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::identity<glm::mat4>(),
//					{ 50.0f, 0.0f, 0.0f }),										//position
//					(float)(-180.0f * (M_PI / 180.0f)), glm::vec3{ 1.0f, 0.0f, 0.0f }),	//x rotation
//					(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 1.0f, 0.0f }),		//y rotation
//					(float)(-90.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 0.0f, 1.0f }),	//z rotation
//					glm::vec3{ 50.0f, 1.0f, 50.0f }),								//scale
//				glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });								//color
//
//			//Render Camera Target Location
//			if (gameCamera.getTarget() != std::nullopt) {
//				Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/plane.obj"),
//					glm::scale(glm::translate(glm::identity<glm::mat4>(),
//						gameCamera.hasTarget() ? gameCamera.getTarget().value()
//						: glm::vec3{ 0.0f, 0.0f, 0.0f }),				//position
//						glm::vec3{ 1.0f, 1.0f, 1.0f }),			//scale
//					glm::vec4{ 1.0f, 0.45f, 0.45f, 1.0f });			//color
//			}
//
//
//			//Render Light source at star
//			Renderer::SubmitLightSource({ {0.0f, 0.0f, 0.0f, 1.0f} });
//
//			//Render Sun
//			Star star = system->getStar();
//			Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/sun.obj"),
//				glm::scale(glm::translate(glm::identity<glm::mat4>(),star.star->getPosition()),
//					glm::vec3{ star.star->getScale() }));
//
//			//Render all system bodies
//			std::vector<std::shared_ptr<Body>> bodies = system->getBodyList();
//			for (auto i : bodies) {
//				Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/32x32.obj"),
//					glm::scale(glm::translate(glm::identity<glm::mat4>(),i->getPosition()),
//						glm::vec3(i->getScale())),glm::vec4{ i->getColor(), 1.0f });
//			}
//		}
//		Renderer::End3DScene();
//
//		
//		Renderer::Begin2DScene(orthoCamera); {
//			Renderer::ChangeFont(AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"));
//				
//			if (!console->getVisible()) {
//				//Render GameState Information
//				Renderer::SubmitText("Camera Position:  x = " + std::to_string(gameCamera.getPosition().x)
//					+ ";  y = " + std::to_string(gameCamera.getPosition().y)
//					+ ";  z = " + std::to_string(gameCamera.getPosition().z),
//					{ -(window->getData().size.x / 2.f) + 1.0f,
//						(window->getData().size.y / 2.f) - 2.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.3);
//
//				//Render Mouse Information
//				Renderer::SubmitText("Mouse:  x - " + std::to_string(mouseX) + ";  y - " + std::to_string(mouseY)
//					+ ";  dx - " + std::to_string(mouseDX) + ";  dy - " + std::to_string(mouseDY),
//					{ -(window->getData().size.x / 2.f) + 1.0f,
//						(window->getData().size.y / 2.f) - 15.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.3);
//
//				Renderer::SubmitText("Mouse 1 Down Time: " + std::to_string(mouse1DownTime),
//					{ -(window->getData().size.x / 2.f) + 1.0f,
//						(window->getData().size.y / 2.f) - 28.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, Gravity::LEFT, 0.3);
//
//				Renderer::SubmitText("Mouse 1 Double Click: " + (std::string)(doubleClick1Check ? "True" : "False"),
//					{ -(window->getData().size.x / 2.f) + 1.0f,
//						(window->getData().size.y / 2.f) - 41.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.3);
//				if (doubleClick1Check) {
//					Renderer::SubmitText("Mouse 1 Double Click Time: " + std::to_string(doubleClick1Time),
//						{ -(window->getData().size.x / 2.f) + 1.0f,
//							(window->getData().size.y / 2.f) - 54.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.3);
//				}
//			}
//			else {
//				glm::vec2 size = window->getData().size;
//				size.y *= (3.f / 5.f);
//				const float bottom{ (window->getData().size.y / 2.f) - (size.y) };
//				Renderer::SubmitQuad({ 0.0f, window->getData().size.y / 2.f - (size.y / 2.f), 0.0f}, size, AssetManager::GetOrCreate<Texture>("./resources/textures/console/consoleBackground.png"), 0.0f);
//
//				Renderer::SubmitText("> " + console->getCmdLine(),
//					{ -(window->getData().size.x / 2.f) + 1.0f, bottom + (.02f * size.y), 0.0f}, {1.0f, 1.0f, 1.0f}, Gravity::CENTER_VERTICAL, 0.3);
//
//				glm::vec2 pos{ window->getData().size.x / -2.f, bottom + size.y * .075 };
//				for (int i = 1; i <= console->getArchiveSize(); i++) {
//					Renderer::SubmitText(console->getArchiveAt(i - 1),
//						pos, { 1.0f, 1.0f, 1.0f }, Gravity::CENTER_VERTICAL, 0.3);
//					pos.y += Renderer::GetFont()->getTextHeight(console->getArchiveAt(i - 1)) * .42;
//				}
//			}
//		}
//		Renderer::End2DScene();
//
//		componentManager.drawComponents(delta, orthoCamera);
//	}
//
//	virtual void onWindowResize(float oldWidth, float oldHeight, float newWidth, float newHeight) override {
//		this->orthoCamera = { newWidth / -2.0f, newWidth / 2.0f, newHeight / -2.0f, newHeight / 2.0f };
//		if (newWidth != 0 && newHeight != 0) this->gameCamera.updateProjection({ newWidth, newHeight }, 70.0f, .1f, 1000.0f);
//	}
//
//	virtual bool onKeyPressed(const Key& key) {
//		if (key == GLFW_KEY_LEFT_SHIFT) {
//			lShiftMod = true;
//		}
//		if (key == GLFW_KEY_RIGHT_SHIFT) {
//			rShiftMod = true;
//		}
//		if (key == GLFW_KEY_LEFT_ALT) {
//			lAltMod = true;
//		}
//
//		if ((lShiftMod || rShiftMod) && key == GLFW_KEY_GRAVE_ACCENT) {
//			printf("PlayState: Grave Accent Pressed -- Swapping Console Visibility\n");
//			#if defined(DEBUG)
//				printf("\tConsole Visibility: ");
//				printf(console->getVisible() ? "True -> " : "False -> ");
//			#endif
//			console->setVisible(!console->getVisible());
//			#if defined(DEBUG)
//				printf(console->getVisible() ? "True\n" : "False\n");
//			#endif
//		}
//
//		if (console->getVisible()) {
//			if (key >= 'A' && key <= 'Z') {
//				(lShiftMod || rShiftMod) ? console->pushChar(key) : console->pushChar(key + 32);
//				#if defined(DEBUG)
//					printf("PlayState: Console -- Character Added: ");
//					(lShiftMod || rShiftMod) ? printf("%c", key) : printf("%c", (key + 32));
//					printf("\tNew Command: %s\n", console->getCmdLine().c_str());
//				#endif
//			}
//
//			if ((key >= '0' && key <= '9') || (key == ' ') || (key == '.')) {
//				console->pushChar(key);
//				#if defined(DEBUG)
//					printf("PlayState: Console -- Character Added: ");
//					printf("%c", key);
//					printf("\tNew Command: %s\n", console->getCmdLine().c_str());
//				#endif
//			}
//
//
//			if (key == GLFW_KEY_BACKSPACE) {
//				console->popChar();
//				#if defined(DEBUG)
//					printf("PlayState: Console -- Character Removed\n");
//				#endif
//			}
//			if (key == GLFW_KEY_ENTER) {
//				if (console->getCmdLine() != "") {
//					#if defined(DEBUG)
//						printf("PlayState: Console -- Command Pushed:\n%s\n", console->getCmdLine().c_str());
//					#endif
//					console->pushString(console->getCmdLine().c_str());
//				}
//			}
//		}
//		else {
//			if (key == GLFW_KEY_ESCAPE) {
//				printf("PlayState: Escape Key Pressed -- Ending PlayState\n");
//				State::RestoreState();
//			}
//			if (key == GLFW_KEY_F) {
//				gameCamera.focusOn(this->system->getStar().star->getPosition());
//			}
//		}
//		return true;
//	}
//
//	virtual bool onKeyReleased(const Key& key) {
//		if (key == GLFW_KEY_LEFT_SHIFT) {
//			lShiftMod = false;
//		}
//		if (key == GLFW_KEY_RIGHT_SHIFT) {
//			rShiftMod = false;
//		}
//		if (key == GLFW_KEY_LEFT_ALT) {
//			lAltMod = false;
//		}
//		return true;
//	}
//
//	virtual bool onMouseButtonPressed(const MouseButton& button) {
//		if (button == GLFW_MOUSE_BUTTON_1) {
//			mouse1Down = true;
//		}
//		if (button == GLFW_MOUSE_BUTTON_2) {
//			mouse2Down = true;
//		}
//		return true;
//	}
//
//	virtual bool onMouseButtonReleased(const MouseButton& button) {
//		if (button == GLFW_MOUSE_BUTTON_1) {
//			mouse1Down = false;
//			if (mouse1DownTime < 0.15f) {
//				if (!doubleClick1Check) {
//					doubleClick1Check = true;
//				}
//				else {
//					//On Double Click
//
//				}
//			}
//
//			mouse1DownTime = 0.0f;
//		}
//		if (button == GLFW_MOUSE_BUTTON_2) {
//			mouse2Down = false;
//		}
//		return true;
//	}
//
//	virtual bool onMouseWheelScroll(float xOffset, float yOffset) {
//		gameCamera.scale += yOffset / 12.0f;
//		gameCamera.setYaw(gameCamera.getYaw());
//		if (lAltMod == true) {
//			// gameCamera.setCameraFOV(gameCamera.getCameraFOV() - yOffset);
//		}
//		if (lShiftMod == true) {
//			// gameCamera.setCameraSpeed(gameCamera.getCameraSpeed() + yOffset);
//		}
//		return true;
//	}
//
//	virtual bool onMouseMoved(float x, float y, float dx, float dy) {
//		if (mouse1Down == true && mouse2Down == true) {
//			gameCamera.move(dx, MoveDirection::LEFT);
//			gameCamera.move(dy, MoveDirection::FORWARDS);
//		} else {
//			if (mouse1Down == true) {
//
//			}
//			if (mouse2Down == true) {
//				gameCamera.setPitch(gameCamera.getPitch() + dy * .4f);
//				gameCamera.setYaw(gameCamera.getYaw() + dx * .4f);
//			}
//		}
//
//		mouseX = x;
//		mouseY = y;
//		mouseDX = dx;
//		mouseDY = dy;
//		return true;
//	}
//};

int main(int argc, char** args) {
	#ifdef RUN_TESTS
		Test();
	#endif
	std::shared_ptr<Window> window = Window::CreateGLWindow("Project Space Engine v0.1a", 1280, 720);

	GameEvent gameEvents[TOTAL_EVENTS];
	for (int i = 0; i < TOTAL_EVENTS; i++) {
		gameEvents[i] = cleanEvent(gameEvents[i]);
		gameEvents[i] = readEvent(gameEvents[i], i);
		printEvent(gameEvents[i]);
	}

	std::shared_ptr<GameState> mainMenuState = GameState::CreateState<MainMenuState>(window);
	LOG_GL_ERROR;

	State::ChangeState(mainMenuState);

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
