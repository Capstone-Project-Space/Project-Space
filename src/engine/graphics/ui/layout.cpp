#include "layout.h"

glm::vec3 GravitateBottomLeft(const glm::vec3& pos, const glm::vec2& size, Gravity g) {
	glm::vec3 position = pos;
	if (g & Gravity::CENTER_HORIZONTAL) {
		position.x += size.x / 2.f;
	}
	if (g & Gravity::CENTER_VERTICAL) {
		position.y += size.y / 2.f;
	}
	if (g & Gravity::RIGHT) {
		position.x += size.x;
	}
	if (g & Gravity::TOP) {
		position.y += size.y;
	}
	return position;
}

UIComponent::UIComponent(const std::string_view& id, Layout* layout) : id(id), layout(layout) {}
UIComponent::UIComponent(const std::string_view& id, std::shared_ptr<Layout> layout) : id(id), layout(layout) {}

void UIComponent::applyLayout(const std::shared_ptr<Window> window, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers, bool update) {
	assert(!this->evaluating && "Circular dependency when evaluating layout.");
	this->evaluating = true;
	if (update) {
		this->size = layout->size(window, *this, peers) * window->getData().scale;
		this->position = layout->position(window, *this, peers);
	} else {
		if (!this->size.has_value())
			this->size = layout->size(window, *this, peers) * window->getData().scale;
		if (!this->position.has_value())
			this->position = layout->position(window, *this, peers);
	}
	this->evaluating = false;
}

bool UIComponent::isWithin(const glm::vec2& pos) const {
	const glm::vec3 position = this->getPosition();
	const glm::vec2 size = this->getSize();
	return pos.x >= position.x && pos.y >= position.y && pos.x < position.x + size.x && pos.y < position.y + size.y;
}

bool UIComponent::ChangeLayout(std::shared_ptr<UIComponent> component, Layout* layout) {
	if (layout) {
		component->layout = std::shared_ptr<Layout>(layout);
		return true;
	}
	return false;
}

bool UIComponent::ChangeVisibility(std::shared_ptr<UIComponent> component, bool visible) {
	component->visible = visible;
	return false;
}
