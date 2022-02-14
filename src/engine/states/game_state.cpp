#include "game_state.h"
#include <src/engine/io/event.h>

GameState::GameState(const std::string& saveFileName) : FileStorage(saveFileName) { }

GameState::~GameState() {
	Events::RemoveReceiver(this);
}

void GameState::renderUIs(float delta) {
	auto& it = uis.begin();
	for (; it != uis.end(); it++) {
		it->draw(delta);
	}
}

std::shared_ptr<GameState> State::CurrentState = nullptr;
std::stack<std::shared_ptr<GameState>> State::StateStack;

void State::ChangeState(std::shared_ptr<GameState> state) {
	State::StateStack.push(CurrentState);
	State::CurrentState = state;
}

void State::RestoreState() {
	CurrentState = State::StateStack.top();
	State::StateStack.pop();
}

void State::ResetStateTo(std::shared_ptr<GameState> state) {
	while (!State::StateStack.empty()) State::StateStack.pop();
	State::StateStack.push(std::shared_ptr<GameState>(nullptr));
	State::CurrentState = state;
}

void State::Draw(float delta) {
	State::CurrentState->update(delta);
	State::CurrentState->render(delta);
}
