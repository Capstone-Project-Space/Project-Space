#include "button_component.h"

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>

ButtonComponent::ButtonComponent(
	const std::string_view& id, Layout* layout, std::function<void(ButtonComponent&)> clickCallback,
	const std::string& text, std::shared_ptr<Font> font, 
	const Color& textColor, const Color& textHighlightColor,
	Gravity gravity, const glm::vec2& minimumDimensions
) : UIComponent(id, layout), text(text), textFont(font), textGravity(gravity), texture(nullptr),
	textureHighlightTint({1.0f}), minimumDimensions(minimumDimensions), clickCallback(clickCallback) {
	if (textColor.index() == 0) this->textColor = glm::vec4{ std::get<0>(textColor), 1.0f };
	else this->textColor = std::get<1>(textColor);
	if (textColor.index() == 0) this->textHighlightColor = { std::get<0>(textHighlightColor), 1.0f };
	else this->textHighlightColor = std::get<1>(textHighlightColor);
}

ButtonComponent::ButtonComponent(
	const std::string_view& id, Layout* layout, std::function<void(ButtonComponent&)> clickCallback,
	std::shared_ptr<Texture> texture, const Color& tintColor, const glm::vec2& minimumDimensions
) : UIComponent(id, layout), text(""), textFont(nullptr), textGravity(Gravity::CENTER), textColor({ 1.0f }),
	textHighlightColor({ 0.6f }), texture(texture), minimumDimensions(minimumDimensions), clickCallback(clickCallback) {
	if (tintColor.index() == 0) this->textureHighlightTint = { std::get<0>(tintColor), 1.0f };
	else this->textureHighlightTint = std::get<1>(tintColor);
}

ButtonComponent::ButtonComponent(
	const std::string_view& id, Layout* layout, std::function<void(ButtonComponent&)> clickCallback,
	const std::string& text, std::shared_ptr<Font> font, std::shared_ptr<Texture> texture,
	const Color& textColor, const Color& textHighlightColor, const Color& tintColor, Gravity gravity,
	const glm::vec2& minimumDimensions
) : UIComponent(id, layout), text(text), textFont(font), textGravity(gravity), texture(texture),
	minimumDimensions(minimumDimensions), clickCallback(clickCallback) {
	if (textColor.index() == 0) this->textColor = glm::vec4{ std::get<0>(textColor), 1.0f };
	else this->textColor = std::get<1>(textColor);
	if (textColor.index() == 0) this->textHighlightColor = { std::get<0>(textHighlightColor), 1.0f };
	else this->textHighlightColor = std::get<1>(textHighlightColor);
	if (tintColor.index() == 0) this->textureHighlightTint = { std::get<0>(tintColor), 1.0f };
	else this->textureHighlightTint = std::get<1>(tintColor);
}

float ButtonComponent::getContentWidth() const {
	return std::max((float) textFont->getTextWidth(text), minimumDimensions.x);
}

float ButtonComponent::getContentHeight() const {
	return std::max((float) textFont->getTextHeight(text), minimumDimensions.y);
}

void ButtonComponent::draw(float delta) {
	glm::vec3 pos = this->position.value();
	const glm::vec2 size = this->size.value();
	bool within = this->isWithin({ Mouse::x, Mouse::y });
	if (texture) {
		if (within) Renderer::SubmitQuad(pos, size, AssetManager::GetOrCreate<Texture>(glm::vec4{1.f}), textureHighlightTint);
		else Renderer::SubmitQuad(pos, size, {0.f, 236.f / 256.f, 200.f / 256.f, 1.f}, texture);
	}
	if (textFont) {
		if (within) {
			Renderer::SubmitText(text, pos, textHighlightColor, textFont, textGravity);
		} else 
			Renderer::SubmitText(text, pos, textColor, textFont, textGravity);
	}

}

bool ButtonComponent::isWithin(const glm::vec2& pos) const {
	const glm::vec2 size = this->size.value();
	const glm::vec3 position = this->position.value() - (glm::vec3{ size / 2.f, 0.0f });
	return pos.x >= position.x && pos.y >= position.y && pos.x < position.x + size.x && pos.y < position.y + size.y;
}

bool ButtonComponent::onMouseButtonReleased(const MouseButton& button) {
	if (this->isWithin({Mouse::x, Mouse::y})) clickCallback(*this);
	return false;
}
