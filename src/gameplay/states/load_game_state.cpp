#include "load_game_state.h"

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>
#include <src/engine/graphics/ui/button_component.h>
#include <src/engine/graphics/ui/text_component.h>
#include <src/engine/graphics/ui/relative_layout.h>
#include <src/engine/graphics/ui/constraint_layout.h>

const std::string LOAD_GAME_SCREEN_TEXTURE = std::string{ "./resources/textures/skybox/front.bmp" };

LoadGameState::LoadGameState(std::shared_ptr<Window> window)
	: GameState(window, "Load Game") {
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
		"This is the screen to load saved games.", AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"),
		.6f, Gravity::CENTER, glm::vec4{ 1.f }
	));
}

void LoadGameState::update(float delta) {

}

void LoadGameState::render(float delta) {
	Renderer::Begin2DScene(window->getData().orthographicCamera);
	Renderer::ChangeFont(AssetManager::GetOrCreate<Font>("./resources/fonts/Arial.ttf"));
	Renderer::SubmitQuad(glm::vec3{ 0.f }, window->getData().size, AssetManager::GetOrCreate<Texture>(LOAD_GAME_SCREEN_TEXTURE));
	
	/*Renderer::SubmitText("This is the screen to load saved games",
		{ -200.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, Gravity::LEFT, 0.6);*/
	
	Renderer::End2DScene();
	componentManager.drawComponents(delta, window->getData().orthographicCamera);
}