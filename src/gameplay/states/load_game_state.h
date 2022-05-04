#pragma once

#include <src/engine/states/game_state.h>

class LoadGameState : public GameState {
public:
     LoadGameState(std::shared_ptr<Window> window);

     virtual void update(float delta) override;
     virtual void render(float delta) override;
private:
};