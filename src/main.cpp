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
#include <src/engine/graphics/ui/explicit_layout.h>

#include <src/engine/randgen/randomgen.h>

#include <src/engine/body_systems/body_system.h>

#include <src/engine/console/console.h>

#include <src/gameplay/event.h>

#define DEBUG

using Clock = std::chrono::high_resolution_clock;

#include <src/testing/tests.h>

#define SIZE_DUMP(type) fprintf(stderr, "sizeof(%s): %llu\n", #type, sizeof(type))

#define LOG_GL_ERROR for (int glErrorGL = glGetError(); glErrorGL != 0;) { fprintf(stderr, "GLError: %d\n", glErrorGL); assert(false);}

int height = 1020, width = 720;
std::shared_ptr<GameState> menuState = nullptr, playState = nullptr;
std::shared_ptr<Console> console = nullptr;

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
			new ConstraintLayout("", "window:top", "window:right", "", "window"),
			std::string{ "This is some text." },
			AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf")
		));

		componentManager.addComponent(new TextComponent(
			"colored_text",
			new ConstraintLayout("", "", "simple_text:left", "window:bottom", "window"),
			"This text changes color.",
			AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf")
		));

		componentManager.addComponent(new TextComponent(
			"explicit_text",
			new ExplicitLayout("300", "50%", "100%", ""),
			"This text is explicit.",
			AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf")
		));

		
	}

	virtual void update(float delta) override {
		static float totalTime = 0.0f;
		totalTime += delta;
		if (totalTime >= 5.0f && totalTime <= 5.2f) {
			componentManager.modifyComponent<TextComponent, const std::string&>("simple_text", std::function{ TextComponent::SetText }, "Smol text.");
			componentManager.modifyComponent<TextComponent, Gravity>("explicit_text", std::function{ TextComponent::SetTextGravity }, Gravity::CENTER);
		}
		componentManager.modifyComponent<TextComponent, glm::vec4>("colored_text", std::function((bool(*)(std::shared_ptr<TextComponent>, glm::vec4))TextComponent::SetColor), glm::vec4{delta * 500.0f, delta * 200.0f, delta * 700.0f, 1.0f});
		componentManager.modifyComponent<TextComponent, float>("simple_text", std::function(TextComponent::SetScale), std::clamp(totalTime, .1f, 1.4f));
	}

	virtual void render(float delta) override {
		Renderer::Begin2DScene(orthoCamera);
		{
			Renderer::SubmitQuad({ 0.0f, 0.0f }, window->getData().size, AssetManager::GetOrCreate<Texture>("./resources/textures/projectspacefull.png"));
		}
		Renderer::End2DScene();

		Renderer::Begin3DScene(perspectiveCamera);
		{
			Renderer::SubmitLightSource({{0.0f, 10.0f, 0.0f, 1.0f}});
			Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/spacepod.obj"), transforms[0]);
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
			window->close();
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

	CameraObject gameCamera{glm::vec3(10.0f), glm::vec3(0.0f)};

	std::shared_ptr<BodySystem> system;
	bool mouse1Down = false, mouse2Down = false, lShiftMod = false, lAltMod = false;
	float delta;

	float mouseX, mouseY, mouseDX, mouseDY;
public:
	PlayState(const std::shared_ptr<Window> window, const std::string& name) : GameState(window, name) {
		system = std::shared_ptr<BodySystem>(new BodySystem(0));
		//perspectiveCamera.setView(glm::lookAt(gameCamera.getPosition(), gameCamera.getTarget(), gameCamera.getCameraUp()));

#if defined(DEBUG)
	printf("DEBUG MODE -- PRINTING SYSTEM DATA\n");
	system->printDebugInfo();
#endif
	}

	virtual void update(float delta) override {
		if (!console->getVisible()) {
			if (Keyboard::isKeyDown(GLFW_KEY_UP)) {
				gameCamera.setCameraLock(false);
				gameCamera.moveCamera(gameCamera.getCameraSpeed() * delta, gameCamera.getCameraFront());
			}
			if (Keyboard::isKeyDown(GLFW_KEY_LEFT)) {
				gameCamera.setCameraLock(false);
				gameCamera.moveCamera(gameCamera.getCameraSpeed() * delta, glm::vec3(-1.0f) * glm::normalize(glm::cross(gameCamera.getCameraFront(), gameCamera.getCameraUp())));
			}
			if (Keyboard::isKeyDown(GLFW_KEY_DOWN)) {
				gameCamera.setCameraLock(false);
				gameCamera.moveCamera(gameCamera.getCameraSpeed() * delta, glm::vec3(-1.0f) * gameCamera.getCameraFront());
			}
			if (Keyboard::isKeyDown(GLFW_KEY_RIGHT)) {
				gameCamera.setCameraLock(false);
				gameCamera.moveCamera(gameCamera.getCameraSpeed() * delta, glm::normalize(glm::cross(gameCamera.getCameraFront(), gameCamera.getCameraUp())));
			}
		}
		perspectiveCamera.setView(glm::lookAt(gameCamera.getPosition(), gameCamera.getTarget(), gameCamera.getCameraUp()));
		this->delta = delta;
	}

	virtual void render(float delta) override {
		Renderer::Begin2DScene(orthoCamera); {
			//Render Skybox
			//front
			Renderer::SubmitQuad({0.0f, 0.0f, -25.0f}, {window->getData().size.x, window->getData().size.y}, AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/front.bmp"), 0.0f);
			//left
			//Renderer::SubmitQuad({-25.0f, 0.0f, 0.0f}, {50.0f, 50.0f}, AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/left.bmp"), 270.0f);
			//right
			//Renderer::SubmitQuad({25.0f, 0.0f, 0.0f}, {50.0f, 50.0f}, AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/right.bmp"), 90.0f);
			//back
			//Renderer::SubmitQuad({0.0f, 0.0f, 25.0f}, {50.0f, 50.0f}, AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/back.bmp"), 180.0f);
		}
		Renderer::End2DScene();

		Renderer::Begin3DScene(perspectiveCamera);
		{
			//Render Light source at star
			Renderer::SubmitLightSource({ {0.0f, 0.0f, 0.0f, 1.0f} });

			//Render Sun
			Star star = system->getStar();
			Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/16x16.obj"),
				glm::scale(glm::translate(glm::identity<glm::mat4>(),star.star->getPosition()),
					glm::vec3{ star.star->getScale() }),glm::vec4{star.star->getColor(), 1.0f});

			//Render all system bodies
			std::vector<std::shared_ptr<Body>> bodies = system->getBodyList();
			for (auto i : bodies) {
				Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/32x32.obj"),
					glm::scale(glm::translate(glm::identity<glm::mat4>(),i->getPosition()),
						glm::vec3(i->getScale())),glm::vec4{ i->getColor(), 1.0f });
			}
		}
		Renderer::End3DScene();

		Renderer::Begin2DScene(orthoCamera); {
			Renderer::ChangeFont(AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"));
			//Render GameState Information
			Renderer::SubmitText("Camera Position:  x = " + std::to_string(gameCamera.getPosition().x)
				+ ";  y = " + std::to_string(gameCamera.getPosition().y)
				+ ";  z = " + std::to_string(gameCamera.getPosition().z),
				{ -(window->getData().size.x / 2.f) + 1.0f,
				  (window->getData().size.y / 2.f) - 2.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.3);

			//Render Mouse Information
			Renderer::SubmitText("Mouse:  x - " + std::to_string(mouseX) + ";  y - " + std::to_string(mouseY)
				+ ";  dx - " + std::to_string(mouseDX) + ";  dy - " + std::to_string(mouseDY),
				{ -(window->getData().size.x / 2.f) + 1.0f,
				  (window->getData().size.y / 2.f) - 15.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.3);

			if (console->getVisible()) {
				Renderer::SubmitQuad({ 0.0f, 0.4f, 0.0f }, { window->getData().size.x, (int)(window->getData().size.y * (3.0f / 5.0f))}, AssetManager::GetOrCreate<Texture>("./resources/textures/console/consoleBackground.png"), 0.0f);
			
				for (int i = 0; i < console->getArchiveSize(); i++) {

				}
			}

		}
		Renderer::End2DScene();

		componentManager.drawComponents(delta, orthoCamera);
	}

	virtual void onWindowResize(float oldWidth, float oldHeight, float newWidth, float newHeight) override {
		this->orthoCamera = { newWidth / -2.0f, newWidth / 2.0f, newHeight / -2.0f, newHeight / 2.0f };
		this->perspectiveCamera = { newWidth, newHeight, 70.0f, .01f, 1000.0f };
	}

	virtual bool onKeyPressed(const Key& key) {
		if (key == GLFW_KEY_LEFT_SHIFT) {
			lShiftMod = true;
		}
		if (key == GLFW_KEY_LEFT_ALT) {
			lAltMod = true;
		}

		if (lShiftMod && key == GLFW_KEY_GRAVE_ACCENT) {
			printf("PlayState: Grave Accent Pressed -- Swapping Console Visibility\n");
#if defined(DEBUG)
			printf("\tConsole Visibility: ");
			printf(console->getVisible() ? "True -> " : "False -> ");
#endif
			console->setVisible(!console->getVisible());
#if defined(DEBUG)
			printf(console->getVisible() ? "True\n" : "False\n");
#endif
		}

		if (console->getVisible()) {
			if (key >= 'A' && key <= 'Z') {
				lShiftMod ? console->pushChar(key) : console->pushChar(key + 32);
#if defined(DEBUG)
				printf("PlayState: Console -- Character Added: ");
				lShiftMod ? printf("%c", key) : printf("%c", (key + 32));
				printf("\tNew Command: %s\n", console->getCmdLine());
#endif
			}
			if (key == GLFW_KEY_BACKSPACE) {
				console->popChar();
#if defined(DEBUG)
				printf("PlayState: Console -- Character Removed\n");
#endif
			}
			if (key == GLFW_KEY_ENTER) {
				if (console->getCmdLine() != "") {
#if defined(DEBUG)
					printf("PlayState: Console -- Command Pushed:\n%s\n", (std::string)console->getCmdLine());
#endif
					console->pushString(console->getCmdLine());
				}
			}
		}
		else {
			if (key == GLFW_KEY_ESCAPE) {
				printf("PlayState: Escape Key Pressed -- Ending PlayState\n");
				State::ResetStateTo(menuState);
			}
		}
		return true;
	}

	virtual bool onKeyReleased(const Key& key) {
		if (key == GLFW_KEY_LEFT_SHIFT) {
			lShiftMod = false;
		}
		if (key == GLFW_KEY_LEFT_ALT) {
			lAltMod = false;
		}
		return true;
	}

	virtual bool onMouseButtonPressed(const MouseButton& button) {
		if (button == GLFW_MOUSE_BUTTON_1) {
			mouse1Down = true;
		}
		if (button == GLFW_MOUSE_BUTTON_2) {
			mouse2Down = true;
		}
		return true;
	}

	virtual bool onMouseButtonReleased(const MouseButton& button) {
		if (button == GLFW_MOUSE_BUTTON_1) {
			mouse1Down = false;
		}
		if (button == GLFW_MOUSE_BUTTON_2) {
			mouse2Down = false;
		}
		return true;
	}

	virtual bool onMouseWheelScroll(float xOffset, float yOffset) {
		if (lAltMod == true) {
			gameCamera.setCameraFOV(gameCamera.getCameraFOV() - yOffset);
		}
		if (lShiftMod == true) {
			gameCamera.setCameraSpeed(gameCamera.getCameraSpeed() + yOffset);
		}
		return true;
	}

	virtual bool onMouseMoved(float x, float y, float dx, float dy) {
		if (mouse1Down == true && mouse2Down == true) {
			//gameCamera.moveCamera(gameCamera.getCameraSpeed() * delta, gameCamera.getCameraFront());
			gameCamera.moveCameraMouse(dx, dy, gameCamera.getCameraSpeed() * delta);
		}
		else {
			if (mouse1Down == true) {

			}
			if (mouse2Down == true) {
				gameCamera.setCameraYaw(gameCamera.getCameraYaw() + dx);
			}
		}

		mouseX = x;
		mouseY = y;
		mouseDX = dx;
		mouseDY = dy;
		return true;
	}
};

int main(int argc, char** args) {
	// Test();
	std::shared_ptr<Window> window = Window::CreateGLWindow("Model Test", 1280, 720);

	console = std::shared_ptr<Console>(new Console());

	GameEvent gameEvents[TOTAL_EVENTS];
	for (int i = 0; i < TOTAL_EVENTS; i++) {
		gameEvents[i] = cleanEvent(gameEvents[i]);
		gameEvents[i] = readEvent(gameEvents[i], i);
		printEvent(gameEvents[i]);
	}

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

	console.~shared_ptr();

	State::Close();
	AssetManager::Clear();
	Renderer::Shutdown();
	return 0;
}
