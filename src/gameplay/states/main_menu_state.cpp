#include "main_menu_state.h"

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>
#include <src/engine/graphics/ui/button_component.h>
#include <src/engine/graphics/ui/relative_layout.h>

#include <src/gameplay/states/new_game_state.h>
#include <src/gameplay/states/load_game_state.h>
#include <src/gameplay/states/settings_state.h>

const std::string TITLE_SCREEN_TEXTURE_NAME = std::string{ "./resources/textures/title_screen.png" };

MainMenuState::MainMenuState(std::shared_ptr<Window> window)
	: GameState(window, "main_menu") {

	//Setup Interactive Buttons
	////New Game
	componentManager.addComponent(new ButtonComponent(
		"play_game",
		new RelativeLayout("window:left:50%", "window:bottom:28%"),
		[=](ButtonComponent& button) {
			State::ChangeState(GameState::CreateState<NewGameState>(window));
			return true;
		}, "New Game", AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf"),
		Color{ glm::vec3{ 0.6f } }, Color{ glm::vec3{ 1.0f, 0.76953125f, 0.1484375f } },
		Gravity::CENTER_HORIZONTAL
	));
	////Load Game
	componentManager.addComponent(new ButtonComponent(
		"load_game",
		new RelativeLayout("play_game:center:0", "play_game:bottom:-20%"),
		[=](ButtonComponent& button) {
			State::ChangeState(GameState::CreateState<LoadGameState>(window));
			return true;
		}, "Load Game", AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf"),
		Color{ glm::vec3{ 0.6f } }, Color{ glm::vec3{ 1.0f, 0.76953125f, 0.1484375f } },
		Gravity::CENTER_HORIZONTAL
	));
	////Settings
	componentManager.addComponent(new ButtonComponent(
		"settings",
		new RelativeLayout("play_game:center:0", "load_game:bottom:-20%"),
		[=](ButtonComponent& button) {
			State::ChangeState(GameState::CreateState<SettingsState>(window));
			return true;
		}, "Settings", AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf"),
		Color{ glm::vec3{ 0.6f } }, Color{ glm::vec3{ 1.0f, 0.76953125f, 0.1484375f } },
		Gravity::CENTER_HORIZONTAL
	));
	////Exit
	componentManager.addComponent(new ButtonComponent(
		"exit",
		new RelativeLayout("play_game:center:0", "settings:bottom:-20%"),
		[=](ButtonComponent& button) {
			window->close();
			return true;
		}, "Exit", AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf"),
		Color{ glm::vec3{ 0.6f } }, Color{ glm::vec3{ 1.0f, 0.76953125f, 0.1484375f } },
		Gravity::CENTER_HORIZONTAL
	));
}

void MainMenuState::update(float delta) {

}

void MainMenuState::render(float delta) {
	Renderer::Begin2DScene(window->getData().orthographicCamera);
	Renderer::SubmitQuad(glm::vec3{0.f}, window->getData().size, AssetManager::GetOrCreate<Texture>(TITLE_SCREEN_TEXTURE_NAME));
	Renderer::End2DScene();
	componentManager.drawComponents(delta, window->getData().orthographicCamera);
}