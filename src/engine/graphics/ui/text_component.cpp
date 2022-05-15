#include "text_component.h"

#include <src/engine/graphics/renderer.h>

TextComponent::TextComponent(
	const std::string_view& id, Layout* layout, const std::string& text, std::shared_ptr<Font> font, 
	float scale, Gravity gravity, const glm::vec4& color)
	: UIComponent(id, layout), text(text), font(font), gravity(gravity), color(color), scale(scale) {}

float TextComponent::getContentWidth() const {
	return font->getTextWidth(text) * scale;
}

float TextComponent::getContentHeight() const {
	return font->getTextHeight(text) * scale;
}

const glm::vec3& TextComponent::getPosition() const {
	glm::vec3 pos = this->position.value();
	const glm::vec2& size = this->size.value();
	if (gravity & Gravity::CENTER_HORIZONTAL) {
		pos.x -= size.x / 2.f;
	}
	if (gravity & Gravity::CENTER_VERTICAL) {
		pos.y -= size.y / 2.f;
	}
	if (gravity & Gravity::RIGHT) {
		pos.x -= size.x;
	}
	if (gravity & Gravity::TOP) {
		pos.y -= size.y / 2.f;
	}

	return pos;
}

void TextComponent::draw(std::shared_ptr<Window> window, float delta) {
	const glm::vec2 scale = window->getData().scale * this->scale;
	//glm::vec3 position = this->position.value();
	Renderer::SubmitText(text, this->position.value(), color, this->font, gravity, scale);
}

bool TextComponent::SetText(std::shared_ptr<TextComponent> component, const std::string& text) {
	component->text = text;
	return true;
}

bool TextComponent::SetFont(std::shared_ptr<TextComponent> component, std::shared_ptr<Font> font) {
	component->font = font;
	return true;
}

bool TextComponent::SetScale(std::shared_ptr<TextComponent> component, float scale) {
	component->scale = scale;
	return true;
}

bool TextComponent::SetColor(std::shared_ptr<TextComponent> component, glm::vec3 color) {
	return TextComponent::SetColor(component, glm::vec4{color, 1.0f});
}

bool TextComponent::SetColor(std::shared_ptr<TextComponent> component, glm::vec4 color) {
	component->color = color;
	return false;
}

bool TextComponent::SetTextGravity(std::shared_ptr<TextComponent> component, Gravity gravity) {
	component->gravity = gravity;
	return false;
}


//bool TextComponent::SetLayoutGravity(std::shared_ptr<TextComponent> component, Gravity gravity) {
//	component->layoutGravity = gravity;
//	return false;
//}