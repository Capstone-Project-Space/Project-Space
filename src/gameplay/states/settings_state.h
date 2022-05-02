#pragma once

#include <src/engine/states/game_state.h>

class SettingsState : public GameState {
public:
     SettingsState(std::shared_ptr<Window> window);

     virtual void update(float delta) override;
     virtual void render(float delte) override;
private:
};