#pragma once

#include <src/engine/states/game_state.h>

class MainMenuState : public GameState {
public:
	MainMenuState(std::shared_ptr<Window> window);

	virtual void update(float delta) override;
	virtual void render(float delta) override;

private:

};
