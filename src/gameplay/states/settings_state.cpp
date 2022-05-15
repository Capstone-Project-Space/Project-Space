#include "settings_state.h"

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>
#include <src/engine/graphics/ui/button_component.h>
#include <src/engine/graphics/ui/checkbox_component.h>
#include <src/engine/graphics/ui/text_component.h>
#include <src/engine/graphics/ui/linear_component_container.h>
#include <src/engine/graphics/ui/relative_layout.h>
#include <src/engine/graphics/ui/constraint_layout.h>

constexpr std::string_view SETTINGS_GAME_SCREEN_TEXTURE = "./resources/textures/skybox/front.bmp";


SettingsState::SettingsState(std::shared_ptr<Window> window)
     : GameState(window, "Project Space") {
    componentManager.addComponent(new ButtonComponent(
        "return_to_mainmenu",
        new RelativeLayout("window:left:100", "window:top:0"),
        [&](ButtonComponent& button) {
            State::RestoreState();
            return true;
        }, "Return", AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf"),
        Color{ glm::vec3{ 0.6f } }, Color{ glm::vec3{ 1.0f, 0.76953125f, 0.1484375f } }
    ));
    componentManager.addComponent(new TextComponent(
        "desc",
        new ConstraintLayout("window:center", "window:center"),
        "This is the screen to change settings of the game.", AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"),
        .6f, Gravity::CENTER, glm::vec4{ 1.f }
    ));
    componentManager.addComponent(new CheckboxComponent(
        "onoff",
        new RelativeLayout("window:center:0", "window:center:10%"), glm::vec2{ 32.f, 32.f }
    ));
    componentManager.addComponent(new TextComponent(
        "onoff_desc",
        new RelativeLayout("onoff:right:10", "onoff:center:0"),
        "Sound On/Off", AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"),
        .4f, Gravity::CENTER_VERTICAL
    ));


    componentManager.addComponent(new TextComponent(
        "many_lines",
        new RelativeLayout("window:center:0", "window:center:0"),
        "This text will contain new lines.\nThis line should appear below the other.\n This line should exist at the bottom of this text.",
        AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"),
        .3f, Gravity::CENTER
    ));


    /*LinearComponentContainer* container = new LinearComponentContainer("container", new RelativeLayout("window:left:10%", "window:top:-10%", "window:right:-10%", "window:bottom:10%"));
    container->add(std::shared_ptr<UIComponent>(new TextComponent(
        "onoff_desc1", nullptr,
        "Sound On/Off", AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"),
        .4f, Gravity::BOTTOM
    )));
    container->add(std::shared_ptr<UIComponent>(new TextComponent(
        "onoff_desc2", nullptr,
        "Sound On/Off", AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"),
        .4f, Gravity::BOTTOM
    )));
    container->add(std::shared_ptr<UIComponent>(new TextComponent(
        "onoff_desc3", nullptr,
        "Sound On/Off", AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"),
        .4f, Gravity::BOTTOM
    )));
    componentManager.addComponent(container);*/

}

void SettingsState::update(float delta) {}

void SettingsState::render(float delta) {
     Renderer::Begin2DScene(window->getData().orthographicCamera);
     Renderer::ChangeFont(AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"));
     Renderer::SubmitQuad(glm::vec3{ 0.f }, window->getData().size, AssetManager::GetOrCreate<Texture>(SETTINGS_GAME_SCREEN_TEXTURE));

     /*Renderer::SubmitText("This is the screen to change settings of the game",
          { -200.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.6);*/

     Renderer::End2DScene();
     componentManager.drawComponents(delta, window->getData().orthographicCamera);
}