#include "text_component.h"

#include <src/engine/graphics/renderer.h>

TextComponent::TextComponent(const std::string_view& id, Layout* layout, const std::string& text, std::shared_ptr<Font> font)
	: UIComponent(id, layout), text(text), font(font) {}

float TextComponent::getContentWidth() const {
	return font->getTextWidth(text) * scale;
}

float TextComponent::getContentHeight() const {
	return font->getTextHeight(text) * scale;
}

void TextComponent::draw(float delta) {
	glm::vec3 pos = this->position.value();
	if ((this->layoutGravity & Gravity::CENTER_HORIZONTAL) == Gravity::CENTER_HORIZONTAL) {
		pos.x -= this->getWidth() / 2.0f;
	} else if (this->layoutGravity & Gravity::RIGHT) {
		pos.x -= this->getWidth();
	}
	if ((layoutGravity & Gravity::CENTER_VERTICAL) == Gravity::CENTER_VERTICAL) {
		pos.y += this->getHeight() / 2.0f;
	} else if (layoutGravity & Gravity::TOP) {
		pos.y -= this->getHeight();
	}

	Renderer::SubmitText(text, pos, color, this->font, textGravity, scale);
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
	component->textGravity = gravity;
	return false;
}


bool TextComponent::SetLayoutGravity(std::shared_ptr<TextComponent> component, Gravity gravity) {
	component->layoutGravity = gravity;
	return false;
}