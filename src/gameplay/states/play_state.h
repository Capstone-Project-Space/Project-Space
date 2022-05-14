#pragma once

#include <src/engine/states/game_state.h>

#include <src/engine/body_systems/body.h>

class PlayState : public GameState {
public:
	PlayState(std::shared_ptr<Window> window);

	virtual void update(float delta) override;
	virtual void render(float delta) override;

	virtual bool onKeyPressed(const Key& key) override;
	virtual bool onMouseButtonReleased(const MouseButton& button) override;
	virtual bool onMouseWheelScroll(float xOffset, float yOffset) override;
	virtual bool onMouseMoved(float x, float y, float dx, float dy) override;
	virtual void onWindowResize(float oldWidth, float oldHeight, float newWidth, float newHeight) override;

private:
	//std::shared_ptr<BodySystem> system;
	Body starBody;
	std::shared_ptr<void> targetObject;
	CameraObject gameCamera{ {1280.0f, 720.0f}, 70.0f, .01f, 1000.0f, glm::vec3(10.0f), glm::vec3{0.0f}, glm::vec3(50.0f) };

	float delta=0.f;
	float mouse1DownTime = 0.f, doubleClick1Time = 0.f;
	bool doubleClick1Check = false;
};