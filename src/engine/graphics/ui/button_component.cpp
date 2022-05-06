#include "button_component.h"

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>

ButtonComponent::ButtonComponent(
	const std::string_view& id, Layout* layout, std::function<bool(ButtonComponent&)> clickCallback,
	const std::string& text, std::shared_ptr<Font> font, 
	const Color& textColor, const Color& textHighlightColor,
	Gravity gravity, const glm::vec2& minimumDimensions
) : UIComponent(id, layout), text(text), textFont(font), gravity(gravity), texture(nullptr),
	textureHighlightTint({1.0f}), minimumDimensions(minimumDimensions), clickCallback(clickCallback) {
	if (textColor.index() == 0) this->textColor = glm::vec4{ std::get<0>(textColor), 1.0f };
	else this->textColor = std::get<1>(textColor);
	if (textColor.index() == 0) this->textHighlightColor = { std::get<0>(textHighlightColor), 1.0f };
	else this->textHighlightColor = std::get<1>(textHighlightColor);
}

ButtonComponent::ButtonComponent(
	const std::string_view& id, Layout* layout, std::function<bool(ButtonComponent&)> clickCallback,
	std::shared_ptr<Texture> texture, const Color& tintColor, Gravity gravity, const glm::vec2& minimumDimensions
) : UIComponent(id, layout), text(""), textFont(nullptr), gravity(gravity), textColor({1.0f}),
	textHighlightColor({ 0.6f }), texture(texture), minimumDimensions(minimumDimensions), clickCallback(clickCallback) {
	if (tintColor.index() == 0) this->textureHighlightTint = { std::get<0>(tintColor), 1.0f };
	else this->textureHighlightTint = std::get<1>(tintColor);
}

ButtonComponent::ButtonComponent(
	const std::string_view& id, Layout* layout, std::function<bool(ButtonComponent&)> clickCallback,
	const std::string& text, std::shared_ptr<Font> font, std::shared_ptr<Texture> texture,
	const Color& textColor, const Color& textHighlightColor, const Color& tintColor, Gravity gravity,
	const glm::vec2& minimumDimensions
) : UIComponent(id, layout), text(text), textFont(font), gravity(gravity), texture(texture),
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

const glm::vec3& ButtonComponent::getPosition() const {
	glm::vec3 pos = this->position.value();
	if ((gravity & Gravity::CENTER_HORIZONTAL) == Gravity::CENTER_HORIZONTAL) {
		pos.x -= this->getWidth() / 2.f;
	}
	if ((gravity & Gravity::CENTER_VERTICAL) == Gravity::CENTER_VERTICAL) {
		pos.y -= this->getHeight() / 2.f;
	}

	if ((gravity & Gravity::RIGHT) == Gravity::RIGHT) {
		pos.x -= this->getWidth();
	}

	if ((gravity & Gravity::TOP) == Gravity::TOP) {
		pos.y -= this->getHeight();
	}
	
	return pos;
}

void ButtonComponent::draw(std::shared_ptr<Window> window, float delta) {
	const glm::vec2 size = this->size.value();
	glm::vec3 pos = position.value();
	if (gravity & Gravity::RIGHT) {
		pos.x -= size.x / 2.f;
	} else if (!(gravity & Gravity::CENTER_HORIZONTAL)) {
		pos.x += size.x / 2.f;
	}
	if (gravity & Gravity::TOP) {
		pos.y -= size.y / 2.f;
	} else if (!(gravity & Gravity::CENTER_VERTICAL)) {
		pos.y += size.y / 2.f;
	}
	bool within = this->isWithin({ Mouse::x, Mouse::y });
	if (texture) {
		if (within) Renderer::SubmitQuad(pos, size, AssetManager::GetOrCreate<Texture>(glm::vec4{1.f}), textureHighlightTint);
		else Renderer::SubmitQuad(pos, size, {0.f, 236.f / 256.f, 200.f / 256.f, 1.f}, texture);
	}
	if (textFont) {
		if (within) {
			Renderer::SubmitText(text, this->position.value(), textHighlightColor, textFont, gravity, window->getData().scale);
		} else 
			Renderer::SubmitText(text, this->position.value(), textColor, textFont, gravity, window->getData().scale);
	}

}

bool ButtonComponent::onMouseButtonReleased(const MouseButton& button) {
	if (this->isWithin({Mouse::x, Mouse::y})) return clickCallback(*this);
	return false;
}
