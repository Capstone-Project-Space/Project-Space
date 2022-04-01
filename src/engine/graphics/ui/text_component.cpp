#include "text_component.h"

#include <src/engine/graphics/renderer.h>

TextComponent::TextComponent(const std::string_view& id, Layout* layout, const std::string& text, std::shared_ptr<Font> font)
	: UIComponent(id, layout), text(text), font(font) {}

float TextComponent::getContentWidth() const {
	return font->getTextWidth(text);
}

float TextComponent::getContentHeight() const {
	return font->getTextHeight(text);
}

void TextComponent::draw(float delta) {
	Renderer::SubmitText(text, this->position.value(), {1.0f, 1.0f, 1.0f, 1.0f}, this->font);
}

bool TextComponent::SetText(std::shared_ptr<TextComponent> component, const std::string& text) {
	component->text = text;
	return true;
}
