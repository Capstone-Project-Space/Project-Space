#pragma once

#include <src/engine/states/game_state.h>

class NewGameState : public GameState {
public:
     NewGameState(std::shared_ptr<Window> window);

     virtual void update(float delta) override;
     virtual void render(float delta) override;
private:
};