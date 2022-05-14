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
