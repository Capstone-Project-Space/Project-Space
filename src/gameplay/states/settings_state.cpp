#include "settings_state.h"

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>
#include <src/engine/graphics/ui/button_component.h>
#include <src/engine/graphics/ui/relative_layout.h>

SettingsState::SettingsState(std::shared_ptr<Window> window)
     : GameState(window, "Project Space") {
     componentManager.addComponent(new ButtonComponent(
          "return_to_mainmenu",
          new RelativeLayout("window:right:80%", "window:top:100%"),
          [&](ButtonComponent& button) {
               State::ResetStateTo(states[0]);
          }, "Return", AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf"),
               Color{ glm::vec3{ 0.6f } }, Color{ glm::vec3{ 1.0f, 0.76953125f, 0.1484375f } }
          ));
}

void SettingsState::update(float delta) {

}

void SettingsState::render(float delta) {
     Renderer::Begin2DScene(window->getData().orthographicCamera);

     Renderer::ChangeFont(AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"));

     Renderer::SubmitQuad(glm::vec3{ 0.f }, window->getData().size, AssetManager::GetOrCreate<Texture>("./resources/textures/skybox/front.bmp"));

     Renderer::SubmitText("This page will contain options for settings of the game",
          { -200.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.6);

     Renderer::End2DScene();
     componentManager.drawComponents(delta, window->getData().orthographicCamera);
}