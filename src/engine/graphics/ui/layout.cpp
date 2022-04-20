#include "layout.h"

UIComponent::UIComponent(const std::string_view& id, Layout* layout) : id(id), layout(layout) {}
UIComponent::UIComponent(const std::string_view& id, std::shared_ptr<Layout> layout) : id(id), layout(layout) {}

void UIComponent::applyLayout(const std::shared_ptr<Window> window, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers, bool update) {
	assert(!this->evaluating && "Circular dependency when evaluating layout.");
	this->evaluating = true;
	if (update) {
		this->size = layout->size(window, *this, peers);
		this->position = layout->position(window, *this, peers);
	} else {
		if (!this->size.has_value())
			this->size = layout->size(window, *this, peers);
		if (!this->position.has_value())
			this->position = layout->position(window, *this, peers);
	}
	this->evaluating = false;
}

bool UIComponent::ChangeLayout(std::shared_ptr<UIComponent> component, Layout* layout) {
	if (layout) {
		component->layout = std::shared_ptr<Layout>(layout);
		return true;
	}
	return false;
}