#include "main_menu_state.h"

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>
#include <src/engine/graphics/ui/button_component.h>
#include <src/engine/graphics/ui/relative_layout.h>

const std::string TITLE_SCREEN_TEXTURE_NAME = std::string{ "./resources/textures/title_screen.png" };

MainMenuState::MainMenuState(std::shared_ptr<Window> window)
	: GameState(window, "main_menu") {

	//Setup Interactive Buttons
	////New Game
	componentManager.addComponent(new ButtonComponent(
		"play_game",
		new RelativeLayout("window:right:0%", "window:bottom:50%"),
		[](ButtonComponent& button) {

		}, "New Game", AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf"),
		Color{ glm::vec3{ 0.6f } }, Color{ glm::vec3{ 1.0f, 0.76953125f, 0.1484375f } }
	));
	////Load Game
	componentManager.addComponent(new ButtonComponent(
		"load_game",
		new RelativeLayout("window:right:0%", "play_game:bottom:-10"),
		[](ButtonComponent& button) {

		}, "Load Game", AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf"),
			Color{ glm::vec3{ 0.6f } }, Color{ glm::vec3{ 1.0f, 0.76953125f, 0.1484375f } }
	));
	////Settings
	componentManager.addComponent(new ButtonComponent(
		"settings",
		new RelativeLayout("window:right:0%", "load_game:bottom:-10"),
		[&](ButtonComponent& button) {
			State::ChangeState(states[0]);
		}, "Settings", AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf"),
			Color{ glm::vec3{ 0.6f } }, Color{ glm::vec3{ 1.0f, 0.76953125f, 0.1484375f } }
	));
	////Exit
	componentManager.addComponent(new ButtonComponent(
		"exit",
		new RelativeLayout("window:right:0%", "settings:bottom:-10"),
		[&](ButtonComponent& button) {
			this->window->close();
		}, "Exit", AssetManager::GetOrCreate<Font>("./resources/fonts/Movement.ttf"),
			Color{ glm::vec3{ 0.6f } }, Color{ glm::vec3{ 1.0f, 0.76953125f, 0.1484375f } }
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