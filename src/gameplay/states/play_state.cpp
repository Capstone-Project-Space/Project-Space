#include "play_state.h"

#include <src/engine/console/console.h>
#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

PlayState::PlayState(std::shared_ptr<Window> window) : GameState(window, "play_state") {
	system = std::make_shared<BodySystem>(0);
	gameCamera.focusOn(glm::vec3{0.f});
	#if defined(DEBUG)
		printf("DEBUG MODE -- PRINTING SYSTEM DATA\n");
		system->printDebugInfo();
	#endif
}

void PlayState::update(float delta) {
	if (doubleClick1Check) {
		if (doubleClick1Time > 0.15f) {
			doubleClick1Check = false;
			doubleClick1Time = 0.0f;
		}
		else {
			doubleClick1Time += delta;
		}
	}

	if (Mouse::isButtonDown(GLFW_MOUSE_BUTTON_1)) {
		if (mouse1DownTime < 1.0f) mouse1DownTime += delta;
		if (mouse1DownTime > 1.0f) mouse1DownTime = 1.0f;
	}


	if (!Console::Get().getVisible()) {
		if (Keyboard::IsKeyDown(GLFW_KEY_UP)) {
			if (gameCamera.getTarget() == std::nullopt) {
				gameCamera.move(1.5 * delta, MoveDirection::FORWARDS);
			}
			else {

			}
		}
		if (Keyboard::IsKeyDown(GLFW_KEY_LEFT)) {
			if (gameCamera.getTarget() == std::nullopt) {
				gameCamera.move(1.5 * delta, MoveDirection::LEFT);
			}
		}
		if (Keyboard::IsKeyDown(GLFW_KEY_DOWN)) {
			if (gameCamera.getTarget() == std::nullopt) {
				gameCamera.move(1.5 * delta, MoveDirection::BACKWARDS);
			}
		}
		if (Keyboard::IsKeyDown(GLFW_KEY_RIGHT)) {
			if (gameCamera.getTarget() == std::nullopt) {
				gameCamera.move(1.5 * delta, MoveDirection::RIGHT);
			}
		}
	}
	this->delta = delta;
}

void PlayState::render(float delta) {
	Renderer::Begin3DScene(gameCamera.getCamera()); {
		//Submit Skybox

		//front
		Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/plane.obj", AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/front.bmp")),
			glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::identity<glm::mat4>(),
				{ 0.0f, 0.0f, -50.0f }),										//position
				(float)(-90.0f * (M_PI / 180.0f)), glm::vec3{ 1.0f, 0.0f, 0.0f }),	//x rotation
				(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 1.0f, 0.0f }),		//y rotation
				(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 0.0f, 1.0f }),		//z rotation
				glm::vec3{ 50.0f, 1.0f, 50.0f }),								//scale
			glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });								//color

		//back
		Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/plane.obj", AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/back.bmp")),
			glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::identity<glm::mat4>(),
				{ 0.0f, 0.0f, 50.0f }),										//position
				(float)(90.0f * (M_PI / 180.0f)), glm::vec3{ 1.0f, 0.0f, 0.0f }),	//x rotation
				(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 1.0f, 0.0f }),		//y rotation
				(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 0.0f, 1.0f }),		//z rotation
				glm::vec3{ 50.0f, 1.0f, 50.0f }),								//scale
			glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });								//color

		//left
		Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/plane.obj", AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/left.bmp")),
			glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::identity<glm::mat4>(),
				{ -50.0f, 0.0f, 0.0f }),										//position
				(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 1.0f, 0.0f, 0.0f }),		//x rotation
				(float)(90.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 1.0f, 0.0f }),	//y rotation
				(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 0.0f, 1.0f }),		//z rotation
				glm::vec3{ 50.0f, 1.0f, 50.0f }),								//scale
			glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });								//color

		//right
		Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/plane.obj", AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/right.bmp")),
			glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::identity<glm::mat4>(),
				{ 50.0f, 0.0f, 0.0f }),										//position
				(float)(-180.0f * (M_PI / 180.0f)), glm::vec3{ 1.0f, 0.0f, 0.0f }),	//x rotation
				(float)(0.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 1.0f, 0.0f }),		//y rotation
				(float)(-90.0f * (M_PI / 180.0f)), glm::vec3{ 0.0f, 0.0f, 1.0f }),	//z rotation
				glm::vec3{ 50.0f, 1.0f, 50.0f }),								//scale
			glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });								//color

		//Render Camera Target Location
		if (gameCamera.getTarget() != std::nullopt) {
			Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/plane.obj"),
				glm::scale(glm::translate(glm::identity<glm::mat4>(),
					gameCamera.hasTarget() ? gameCamera.getTarget().value()
					: glm::vec3{ 0.0f, 0.0f, 0.0f }),				//position
					glm::vec3{ 1.0f, 1.0f, 1.0f }),			//scale
				glm::vec4{ 1.0f, 0.45f, 0.45f, 1.0f });			//color
		}


		//Render Light source at star
		Renderer::SubmitLightSource({ {0.0f, 0.0f, 0.0f, 1.0f} });

		//Render Sun
		Star star = system->getStar();
		Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/sun.obj"),
			glm::scale(glm::translate(glm::identity<glm::mat4>(), star.star->getPosition()),
				glm::vec3{ star.star->getScale() }));

		//Render all system bodies
		std::vector<std::shared_ptr<Body>> bodies = system->getBodyList();
		for (auto i : bodies) {
			Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/32x32.obj"),
				glm::scale(glm::translate(glm::identity<glm::mat4>(), i->getPosition()),
					glm::vec3(i->getScale())), glm::vec4{ i->getColor(), 1.0f });
		}
	}
	Renderer::End3DScene();


	Renderer::Begin2DScene(window->getData().orthographicCamera); {
		Renderer::ChangeFont(AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"));

		if (!Console::Get().getVisible()) {
			//Render GameState Information
			Renderer::SubmitText("Camera Position:  x = " + std::to_string(gameCamera.getPosition().x)
				+ ";  y = " + std::to_string(gameCamera.getPosition().y)
				+ ";  z = " + std::to_string(gameCamera.getPosition().z),
				{ -(window->getData().size.x / 2.f) + 1.0f,
					(window->getData().size.y / 2.f) - 2.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.3);

			//Render Mouse Information
			/*Renderer::SubmitText("Mouse:  x - " + std::to_string(Mouse::x) + ";  y - " + std::to_string(Mouse::y)
				+ ";  dx - " + std::to_string(mouseDX) + ";  dy - " + std::to_string(mouseDY),
				{ -(window->getData().size.x / 2.f) + 1.0f,
					(window->getData().size.y / 2.f) - 15.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.3);*/

			Renderer::SubmitText("Mouse 1 Down Time: " + std::to_string(mouse1DownTime),
				{ -(window->getData().size.x / 2.f) + 1.0f,
					(window->getData().size.y / 2.f) - 28.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.3);

			Renderer::SubmitText("Mouse 1 Double Click: " + (std::string)(doubleClick1Check ? "True" : "False"),
				{ -(window->getData().size.x / 2.f) + 1.0f,
					(window->getData().size.y / 2.f) - 41.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.3);
			if (doubleClick1Check) {
				Renderer::SubmitText("Mouse 1 Double Click Time: " + std::to_string(doubleClick1Time),
					{ -(window->getData().size.x / 2.f) + 1.0f,
						(window->getData().size.y / 2.f) - 54.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.3);
			}
		}
		else {
			glm::vec2 size = window->getData().size;
			size.y *= (3.f / 5.f);
			const float bottom{ (window->getData().size.y / 2.f) - (size.y) };
			Renderer::SubmitQuad({ 0.0f, window->getData().size.y / 2.f - (size.y / 2.f), 0.0f }, size, AssetManager::GetOrCreate<Texture>("./resources/textures/console/consoleBackground.png"), 0.0f);

			Renderer::SubmitText("> " + Console::Get().getCmdLine(),
				{ -(window->getData().size.x / 2.f) + 1.0f, bottom + (.02f * size.y), 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::CENTER_VERTICAL, 0.3);

			glm::vec2 pos{ window->getData().size.x / -2.f, bottom + size.y * .075 };
			for (int i = 1; i <= Console::Get().getArchiveSize(); i++) {
				Renderer::SubmitText(Console::Get().getArchiveAt(i - 1),
					pos, { 1.0f, 1.0f, 1.0f }, Gravity::CENTER_VERTICAL, 0.3);
				pos.y += Renderer::GetFont()->getTextHeight(Console::Get().getArchiveAt(i - 1)) * .42;
			}
		}
	}
	Renderer::End2DScene();

	componentManager.drawComponents(delta, window->getData().orthographicCamera);
}

void PlayState::onWindowResize(float oldWidth, float oldHeight, float newWidth, float newHeight) {
	GameState::onWindowResize(oldWidth, oldHeight, newWidth, newHeight);
	if (newWidth != 0 && newHeight != 0) this->gameCamera.updateProjection({ newWidth, newHeight }, 70.0f, .1f, 1000.0f);
}

bool PlayState::onKeyPressed(const Key& key) {

	if ((Keyboard::IsShiftDown()) && key == GLFW_KEY_GRAVE_ACCENT) {
		printf("PlayState: Grave Accent Pressed -- Swapping Console Visibility\n");
		#if defined(DEBUG)
			printf("\tConsole Visibility: ");
			printf(console->getVisible() ? "True -> " : "False -> ");
		#endif
		Console::Get().setVisible(!Console::Get().getVisible());
		#if defined(DEBUG)
			printf(console->getVisible() ? "True\n" : "False\n");
		#endif
	}

	if (Console::Get().getVisible()) {
		if (key >= 'A' && key <= 'Z') {
			(Keyboard::IsShiftDown()) ? Console::Get().pushChar(key) : Console::Get().pushChar(key + 32);
			#if defined(DEBUG)
				printf("PlayState: Console -- Character Added: ");
				(Keyboard::isShiftDown()) ? printf("%c", key) : printf("%c", (key + 32));
				printf("\tNew Command: %s\n", console->getCmdLine().c_str());
			#endif
		}

		if ((key >= '0' && key <= '9') || (key == ' ') || (key == '.')) {
			Console::Get().pushChar(key);
			#if defined(DEBUG)
				printf("PlayState: Console -- Character Added: ");
				printf("%c", key);
				printf("\tNew Command: %s\n", console->getCmdLine().c_str());
			#endif
		}


		if (key == GLFW_KEY_BACKSPACE) {
			Console::Get().popChar();
			#if defined(DEBUG)
				printf("PlayState: Console -- Character Removed\n");
			#endif
		}
		if (key == GLFW_KEY_ENTER) {
			if (Console::Get().getCmdLine() != "") {
				#if defined(DEBUG)
					printf("PlayState: Console -- Command Pushed:\n%s\n", console->getCmdLine().c_str());
				#endif
				Console::Get().pushString(Console::Get().getCmdLine().c_str());
			}
		}
	}
	else {
		if (key == GLFW_KEY_ESCAPE) {
			printf("PlayState: Escape Key Pressed -- Ending PlayState\n");
			State::RestoreState();
		}
		if (key == GLFW_KEY_F) {
			gameCamera.focusOn(this->system->getStar().star->getPosition());
		}
	}
	return true;
}

bool PlayState::onMouseButtonReleased(const MouseButton& button) {
	if (button == GLFW_MOUSE_BUTTON_1) {
		if (mouse1DownTime < 0.15f) {
			if (!doubleClick1Check) {
				doubleClick1Check = true;
			}
			else {
				//On Double Click

			}
		}

		mouse1DownTime = 0.0f;
	}
	return true;
}

bool PlayState::onMouseWheelScroll(float xOffset, float yOffset) {
	gameCamera.scale += yOffset / 12.0f;
	gameCamera.setYaw(gameCamera.getYaw());
	if (Keyboard::IsAltDown()  == true) {
		// gameCamera.setCameraFOV(gameCamera.getCameraFOV() - yOffset);
	}
	if (Keyboard::IsShiftDown() == true) {
		// gameCamera.setCameraSpeed(gameCamera.getCameraSpeed() + yOffset);
	}
	return true;
}

bool PlayState::onMouseMoved(float x, float y, float dx, float dy) {
	if (Mouse::isButtonDown(GLFW_MOUSE_BUTTON_1) == true && Mouse::isButtonDown(GLFW_MOUSE_BUTTON_2) == true) {
		gameCamera.move(dx, MoveDirection::LEFT);
		gameCamera.move(dy, MoveDirection::FORWARDS);
	}
	else {
		if (Mouse::isButtonDown(GLFW_MOUSE_BUTTON_1) == true) {

		}
		if (Mouse::isButtonDown(GLFW_MOUSE_BUTTON_2) == true) {
			gameCamera.setPitch(gameCamera.getPitch() + dy * .4f);
			gameCamera.setYaw(gameCamera.getYaw() + dx * .4f);
		}
	}

	/*mouseDX = dx;
	mouseDY = dy;*/
	return true;
}
