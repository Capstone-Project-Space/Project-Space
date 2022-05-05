#include "game_state.h"

#include <src/engine/io/event.h>

// GameState::GameState(const std::string& saveFileName) : FileStorage(saveFileName), window(nullptr) { }
GameState::GameState(const std::shared_ptr<Window> window, const std::string& saveFileName) : FileStorage(saveFileName), window(window) { }

GameState::~GameState() {
	Events::RemoveReceiver(this);
}

void GameState::onResume(std::shared_ptr<Window> window) {
	if (window) this->window = window;
	componentManager.applyLayouts(this->window);
}

void GameState::onPause() {}

void GameState::changeWindow(std::shared_ptr<Window> window) {
	this->window = window;
	this->componentManager.applyLayouts(window);
}

bool GameState::onMouseButtonPressed(const MouseButton& button) {
	return componentManager.onMouseButtonPressed(button);
}

bool GameState::onMouseButtonReleased(const MouseButton& button) {
	return componentManager.onMouseButtonReleased(button);
}

void GameState::onWindowResize(float oldWidth, float oldHeight, float newWidth, float newHeight) {
	componentManager.applyLayouts(window);
}

std::shared_ptr<GameState> State::CurrentState = nullptr;
std::stack<std::shared_ptr<GameState>> State::StateStack;

void State::ChangeState(std::shared_ptr<GameState> state) {
	State::StateStack.push(CurrentState);
	State::CurrentState = state;
	auto lastState = State::StateStack.top();
	if (State::CurrentState) State::CurrentState->onResume(lastState ? lastState->window : nullptr);
}

std::shared_ptr<GameState> State::RestoreState() {
	auto lastState = State::CurrentState;
	State::CurrentState = State::StateStack.top();
	State::StateStack.pop();
	if (State::CurrentState) State::CurrentState->onResume(lastState ? lastState->window : nullptr);
	return lastState;
}

void State::ResetStateTo(std::shared_ptr<GameState> state) {
	while (!State::StateStack.empty()) State::StateStack.pop();
	auto lastState = State::CurrentState;
	State::StateStack.push(std::shared_ptr<GameState>(nullptr));
	State::CurrentState = state;
	if (State::CurrentState) State::CurrentState->onResume(lastState ? lastState->window : nullptr);
}

void State::Close() {
	while (!State::StateStack.empty()) State::StateStack.pop();
	State::StateStack.push(std::shared_ptr<GameState>(nullptr));
	State::CurrentState = nullptr;
}

void State::Draw(float delta) {
	State::CurrentState->update(delta);
	State::CurrentState->render(delta);
}
