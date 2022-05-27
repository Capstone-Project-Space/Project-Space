#include "settings_state.h"

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>
#include <src/engine/graphics/ui/button_component.h>
#include <src/engine/graphics/ui/checkbox_component.h>
#include <src/engine/graphics/ui/text_component.h>
#include <src/engine/graphics/ui/text_box_component.h>
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
        "title",
        new RelativeLayout("window:center:0", "window:top:-12%"),
        "Settings", AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"),
        .8f, Gravity::CENTER, glm::vec4{ 1.f }
    ));

    componentManager.addComponent(new CheckboxComponent(
        "soundfx_on_off",
        new RelativeLayout("window:left:15%", "title:bottom:-10"), glm::vec2{ 32.f, 32.f }
    ));
    componentManager.addComponent(new TextComponent(
        "soundfx_on_off_desc",
        new RelativeLayout("soundfx_on_off:right:10", "", "", "soundfx_on_off:center:0"),
        "Turn Some Setting On or Off.", AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"),
        .4f, Gravity::CENTER_VERTICAL, glm::vec4{ .8f, .8f, .8f, 1.f }
    ));

    componentManager.addComponent(new CheckboxComponent(
        "bgm_on_off",
        new RelativeLayout("window:left:15%", "soundfx_on_off:bottom:-10"), glm::vec2{ 32.f, 32.f }
    ));
    componentManager.addComponent(new TextComponent(
        "bmg_on_off_desc",
        new RelativeLayout("bgm_on_off:right:10", "", "", "bgm_on_off:center:0"),
        "Turn Some Other Setting On or Off.", AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"),
        .4f, Gravity::CENTER_VERTICAL, glm::vec4{.8f, .8f, .8f, 1.f}
    ));

}

void SettingsState::update(float delta) {}

void SettingsState::render(float delta) {
    Renderer::Begin2DScene(window->getData().orthographicCamera);
    Renderer::ChangeFont(AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"));
    Renderer::SubmitQuad(glm::vec3{ 0.f }, window->getData().size, AssetManager::GetOrCreate<Texture>(SETTINGS_GAME_SCREEN_TEXTURE));

    Renderer::SubmitQuad(glm::vec3{ 0.f }, window->getData().size / 1.3f , AssetManager::GetOrCreate<Texture>("./resources/textures/ui/background.png"));

    Renderer::End2DScene();
    componentManager.drawComponents(delta, window->getData().orthographicCamera);
}