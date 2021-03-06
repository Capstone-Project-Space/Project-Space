#include "play_state.h"

#include <src/engine/console/console.h>
#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

PlayState::PlayState(std::shared_ptr<Window> window) : GameState(window, "play_state"), starBody(Body::CreateSingleStarSystem("Solis")) {
	gameCamera.focusOn(glm::vec3{0.f});
	#if defined(DEBUG)
		printf("DEBUG MODE -- PRINTING SYSTEM DATA\n");
		system->printDebugInfo();
	#endif
}

void PlayState::update(float delta) {
	starBody.update(delta);

	if (!Console::Get().getVisible()) {
		if (Keyboard::IsKeyDown(GLFW_KEY_UP)) {
			if (!gameCamera.getTarget()) {
				gameCamera.move(1.5 * delta, MoveDirection::FORWARDS);
			}
		}
		if (Keyboard::IsKeyDown(GLFW_KEY_LEFT)) {
			if (!gameCamera.getTarget()) {
				gameCamera.move(1.5 * delta, MoveDirection::LEFT);
			}
		}
		if (Keyboard::IsKeyDown(GLFW_KEY_DOWN)) {
			if (!gameCamera.getTarget()) {
				gameCamera.move(1.5 * delta, MoveDirection::BACKWARDS);
			}
		}
		if (Keyboard::IsKeyDown(GLFW_KEY_RIGHT)) {
			if (!gameCamera.getTarget()) {
				gameCamera.move(1.5 * delta, MoveDirection::RIGHT);
			}
		}
	}
}

void PlayState::render(float delta) {
	Renderer::Begin3DScene(gameCamera.getCamera()); {
		//Submit Skybox
		auto get_plane = [](const std::string_view& texture) {
			return AssetManager::GetOrCreate<Model>(
				"./resources/models/plane.obj",
				{ {
					glm::vec3{1.f}, glm::vec3{0.f}, glm::vec3{0.f}, 1.f, 1.f,
					AssetManager::GetOrCreate<Texture>(texture)
				} }
			);
		};


		//Skybox
		//Top
		Renderer::SubmitModel(
			get_plane("./resources/textures/skybox/top.bmp"),
			glm::scale(
				glm::rotate(
					glm::translate(glm::identity<glm::mat4>(), glm::vec3{ 0.f, 50.f, 0.f }),
					glm::radians(180.f), glm::vec3{ 1.f, 0.f, 0.f }
				),
				glm::vec3{ 50.f }
			)
		);
		//Bottom
		Renderer::SubmitModel(
			get_plane("./resources/textures/skybox/bot.bmp"),
			glm::scale(
				glm::translate(glm::identity<glm::mat4>(), glm::vec3{ 0.f, -50.f, 0.f }),
				glm::vec3{ 50.f }
			)
		);
		//Front
		Renderer::SubmitModel(
			get_plane("./resources/textures/skybox/front.bmp"),
			glm::scale(
				glm::rotate(
					glm::translate(glm::identity<glm::mat4>(), glm::vec3{ 0.f, 0.f, -50.f }),
					glm::radians(90.f), glm::vec3{1.f, 0.f, 0.f}
				),
				glm::vec3{ 50.f }
			)
		);
		//Back
		Renderer::SubmitModel(
			get_plane("./resources/textures/skybox/back.bmp"),
			glm::scale(
				glm::rotate(
					glm::rotate(
						glm::translate(glm::identity<glm::mat4>(), glm::vec3{ 0.f, 0.f, 50.f }),
						glm::radians(-90.f), glm::vec3{ 1.f, 0.f, 0.f }
					),
					glm::radians(180.f), glm::vec3{ 0.f, 1.f, 0.f }
				),
				glm::vec3{ 50.f }
			)
		);
		//Right
		Renderer::SubmitModel(
			get_plane("./resources/textures/skybox/right.bmp"),
			glm::scale(
				glm::rotate(
					glm::rotate(
						glm::translate(glm::identity<glm::mat4>(), glm::vec3{ 50.f, 0.f, 0.f }),
						glm::radians(90.f), glm::vec3{ 0.f, 0.f, 1.f }
					),
					glm::radians(-90.f), glm::vec3{ 0.f, 1.f, 0.f }
				),
				glm::vec3{ 50.f }
			)
		);
		//Left
		Renderer::SubmitModel(
			get_plane("./resources/textures/skybox/left.bmp"),
			glm::scale(
				glm::rotate(
					glm::rotate(
						glm::translate(glm::identity<glm::mat4>(), glm::vec3{ -50.f, 0.f, 0.f }),
						glm::radians(-90.f), glm::vec3{ 0.f, 0.f, 1.f }
					),
					glm::radians(90.f), glm::vec3{ 0.f, 1.f, 0.f }
				),
				glm::vec3{ 50.f }
			)
		);

		//Render Camera Target Location
		if (gameCamera.getTarget() != std::nullopt) {
			Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/plane.obj"),
				glm::scale(glm::translate(glm::identity<glm::mat4>(),
					gameCamera.hasTarget() ? gameCamera.getTarget().value()
					: glm::vec3{ 0.0f, 0.0f, 0.0f }),				//position
					glm::vec3{ 1.0f, 1.0f, 1.0f }),			//scale
				glm::vec4{ 1.0f, 0.45f, 0.45f, 1.0f });			//color
		}


		// Moved rendering of Body into Body class.
		starBody.draw(delta, true);
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
					(window->getData().size.y / 2.f) - 15.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, glm::vec2{ 0.3f });
		} else {
			glm::vec2 size = window->getData().size;
			size.y *= (3.f / 5.f);
			const float bottom{ (window->getData().size.y / 2.f) - (size.y) };
			Renderer::SubmitQuad({ 0.0f, window->getData().size.y / 2.f - (size.y / 2.f), 0.0f }, size, AssetManager::GetOrCreate<Texture>("./resources/textures/console/consoleBackground.png"), 0.0f);

			Renderer::SubmitText("> " + Console::Get().getCmdLine(),
				{ -(window->getData().size.x / 2.f) + 1.0f, bottom + (.02f * size.y), 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::CENTER_VERTICAL, glm::vec2{ 0.3f });

			glm::vec2 pos{ window->getData().size.x / -2.f, bottom + size.y * .075 };
			for (int i = 1; i <= Console::Get().getArchiveSize(); i++) {
				Renderer::SubmitText(Console::Get().getArchiveAt(i - 1),
					pos, { 1.0f, 1.0f, 1.0f }, Gravity::CENTER_VERTICAL, glm::vec2{ 0.3f });
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
		} else {
			Console::Get().pushString(glfwGetKeyName(key.keyCode, key.scanCode));
		}
	} else {
		if (key == GLFW_KEY_ESCAPE) {
			printf("PlayState: Escape Key Pressed -- Ending PlayState\n");
			State::RestoreState();
		} else if (key == GLFW_KEY_F) {
			gameCamera.focusOn(starBody.getPosition());
		} else {
			return GameState::onKeyPressed(key);
		}
	}
	return true;
}

bool PlayState::onMouseWheelScroll(float xOffset, float yOffset) {
	if (Keyboard::IsAltDown()  == true) {
		// gameCamera.setCameraFOV(gameCamera.getCameraFOV() - yOffset);
	}
	if (Keyboard::IsShiftDown() == true) {
		// gameCamera.setCameraSpeed(gameCamera.getCameraSpeed() + yOffset);
	}
	return GameState::onMouseWheelScroll(xOffset, yOffset);
}

bool PlayState::onMouseMoved(float x, float y, float dx, float dy) {
	if (Mouse::IsButtonDown(GLFW_MOUSE_BUTTON_1) == true && Mouse::IsButtonDown(GLFW_MOUSE_BUTTON_2) == true) {
		gameCamera.move(dx, MoveDirection::LEFT);
		gameCamera.move(dy, MoveDirection::FORWARDS);
		return true;
	} else {
		if (Mouse::IsButtonDown(GLFW_MOUSE_BUTTON_1) == true) {

		}
		if (Mouse::IsButtonDown(GLFW_MOUSE_BUTTON_2) == true) {
			gameCamera.setPitch(gameCamera.getPitch() + dy * .4f);
			gameCamera.setYaw(gameCamera.getYaw() + dx * .4f);
			return true;
		}
	}
	return GameState::onMouseMoved(x, y, dx, dy);
}

