#include "new_game_state.h"

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>
#include <src/engine/graphics/ui/button_component.h>
#include <src/engine/graphics/ui/text_component.h>
#include <src/engine/graphics/ui/text_box_component.h>
#include <src/engine/graphics/ui/checkbox_component.h>
#include <src/engine/graphics/ui/relative_layout.h>
#include <src/engine/graphics/ui/constraint_layout.h>

#include <src/gameplay/states/play_state.h>

const std::string NEW_GAME_SCREEN_TEXTURE = std::string{ "./resources/textures/skybox/front.bmp" };

NewGameState::NewGameState(std::shared_ptr<Window> window)
     : GameState(window, "New Game") {
	componentManager.addComponent(new ButtonComponent(
		"return_to_mainmenu",
		new RelativeLayout("window:left:100", "window:top:0"),
		[&](ButtonComponent& button) {
			State::RestoreState();
			return true;
		}, "Return", AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf"),
		Color{ glm::vec3{ 0.6f } }, Color{ glm::vec3{ 1.0f, 0.76953125f, 0.1484375f } }
	));
	componentManager.addComponent(new ButtonComponent(
		"play_state",
		new RelativeLayout("window:left:50%", "", "", "window:bottom:25"),
		[=](ButtonComponent& button) {
			auto lastState = State::RestoreState(); // Gotta save this because this is us and if we don't we vanish.
			State::ChangeState(GameState::CreateState<PlayState>(window));
			return true;
		}, "Play", AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf"),
		Color{ glm::vec3{ 0.6f } }, Color{ glm::vec3{ 1.0f, 0.76953125f, 0.1484375f } }
	));

	auto font = AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf");
	componentManager.addComponent(new TextComponent(
		"title",
		new RelativeLayout("window:center:0", "window:top:-15%"),
		"New Game", font,
		.7f, Gravity::CENTER, glm::vec4{.8f}
	));
	componentManager.addComponent(new TextComponent(
		"solar_system_name_title",
		new RelativeLayout("window:center:0", "title:bottom:-15"),
		"Solar System Name", font, .6, Gravity::CENTER_HORIZONTAL, glm::vec4{.87, .87, .87, 1.f }
	));
	componentManager.addComponent(new TextBoxComponent(
		"solar_system_name",
		new RelativeLayout("window:center:0", "solar_system_name_title:bottom:-15"),
		"Enter Name", AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"), 
		glm::vec3{.6f}, glm::vec3{.9f}, .56, Gravity::CENTER_HORIZONTAL
	));
	componentManager.addComponent(new TextComponent(
		"generation_seed_title",
		new RelativeLayout("window:center:0", "solar_system_name:bottom:-15"),
		"Generation Seed", font, .6, Gravity::CENTER_HORIZONTAL, glm::vec4{ .87, .87, .87, 1.f }
	));
	componentManager.addComponent(new TextBoxComponent(
		"generation_seed",
		new RelativeLayout("window:center:0", "generation_seed_title:bottom:-15"),
		"Enter Seed", AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"),
		glm::vec3{ .6f }, glm::vec3{ .9f }, .56, Gravity::CENTER_HORIZONTAL
	));

	componentManager.addComponent(new CheckboxComponent(
		"single_sun",
		new RelativeLayout("window:left:34%", "generation_seed:bottom:-15"),
		true
	));
	componentManager.addComponent(new TextComponent(
		"single_sun_title", 
		new RelativeLayout("single_sun:right:10", "", "", "single_sun:center:0"),
		"Generate A Single Sun.", font, .4, Gravity::CENTER_VERTICAL, glm::vec4{.85}
	));

	componentManager.addComponent(new CheckboxComponent(
		"generate_moons",
		new RelativeLayout("window:left:34%", "single_sun:bottom:0"),
		false
	));
	componentManager.addComponent(new TextComponent(
		"generate_moons_title",
		new RelativeLayout("generate_moons:right:10", "", "", "generate_moons:center:0"),
		"Generate Moons Around Planetoids.", font, .4, Gravity::CENTER_VERTICAL, glm::vec4{ .85 }
	));

}

void NewGameState::update(float delta) { }

void NewGameState::render(float delta) {
	Renderer::Begin2DScene(window->getData().orthographicCamera);
	Renderer::ChangeFont(AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"));
	Renderer::SubmitQuad(glm::vec3{ 0.f }, window->getData().size, AssetManager::GetOrCreate<Texture>(NEW_GAME_SCREEN_TEXTURE));
	
	Renderer::SubmitQuad(glm::vec3{ 0.f }, {window->getData().size.x * .4, window->getData().size.y * .7}, AssetManager::GetOrCreate<Texture>("./resources/textures/ui/background.png"));
	
	Renderer::End2DScene();
	componentManager.drawComponents(delta, window->getData().orthographicCamera);
}