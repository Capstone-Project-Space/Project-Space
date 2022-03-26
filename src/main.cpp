#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <src/engine/states/game_state.h>

#include <src/engine/graphics/model.h>
#include <src/engine/graphics/buffers.h>
#include <src/engine/graphics/vertex_array.h>
#include <src/engine/graphics/shader.h>
#include <src/engine/graphics/window.h>
#include <src/engine/graphics/model_renderer.h>

#include <src/engine/randgen/randomgen.h>

#define TESTING

using Clock = std::chrono::high_resolution_clock;

#if defined(TESTING)

#include <src/testing/tests.h>

#define SIZE_DUMP(type) fprintf(stderr, "sizeof(%s): %llu\n", #type, sizeof(type))

#define LOG_GL_ERROR for (int glErrorGL = glGetError(); glErrorGL != 0;) { fprintf(stderr, "GLError: %d\n", glErrorGL); assert(false);}

constexpr int count = 8192;
class TempState : GameState {

	Camera camera;
	ModelRenderer renderer = ModelRenderer(camera);
	

	glm::mat4 transforms[count];

	std::shared_ptr<Model> model = Model::CreateModel("./resources/models/spacepod.obj");
public:
	TempState(const std::string& name) : GameState(name) {
		LOG_GL_ERROR;
		for (int i = 0; i < count; i++) {
			transforms[i] = glm::translate(glm::scale(glm::identity<glm::mat4>(), { .1f, .1f, .1f }), { RandomGen::RangedRandomFloat(-12, 12), -.9f, RandomGen::RangedRandomFloat(-7, 7)});
		}
	}

	virtual void update(float delta) override { }

	virtual void render(float delta) override {
		renderer.submitLight({{0.0f, 10.0f, 0.0f, 1.0f}});
		for (int i = 0; i < count; i++) {
			renderer.submitModel(model, transforms[i]);
		}
		renderer.draw();
	}

	virtual bool onKeyPressed(const Key& key) { return false; }

	virtual bool onKeyReleased(const Key& key) { return false; }

	virtual bool onKeyRepeated(const Key& key) { return false; }

	virtual bool onMouseButtonPressed(const MouseButton& button) { return false; }

	virtual bool onMouseButtonReleased(const MouseButton& button) { return false; }

	virtual bool onMouseMoved(const float x, const float y, const float dx, const float dy) {return false;}

};

int main(int argc, char** args) {
	// Test();
	std::shared_ptr<Window> window = Window::CreateGLWindow("Model Test", 1280, 720);
	LOG_GL_ERROR;
	std::shared_ptr<GameState> state = GameState::CreateState<TempState>(std::string{ "Temporary State" });
	LOG_GL_ERROR;
	State::ChangeState(state);

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
	uint32_t frames = 0;
	while (window->isOpen) {
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
			window->updateTitle(" Frames: " + std::to_string(frames));
			timeSpent -= 1.0;
			frames = 0;
		}
	}
	State::RestoreState();
	Texture::Clear();
	state.~shared_ptr();
	return 0;
}
#else

int main(int argc, char** args) {
	printf("Hello World!\n");
	return 0;
}

#endif
