#include "component_manager.h"

#include <src/engine/graphics/renderer.h>

void ComponentManager::addComponent(std::shared_ptr<UIComponent> component) {
	assert(componentMapping.find(component->getID()) == componentMapping.end() && "No duplicate IDs allowed. First remove the id then add.");
	this->components.push_back(component);
	this->componentMapping[component->getID()] = component;
	if (this->hasAppliedLayout) component->applyLayout(this->window, componentMapping);
}

void ComponentManager::removeComponent(std::shared_ptr<UIComponent> component) {
	if (componentMapping.find(component->getID()) != componentMapping.end()) {
		std::remove(this->components.begin(), this->components.end(), component);
		this->componentMapping.erase(component->getID());
	}
}

std::optional<std::shared_ptr<UIComponent>> ComponentManager::removeComponent(const std::string_view& id) {
	if (componentMapping.find(id) != componentMapping.end()) {
		auto opt = std::make_optional<std::shared_ptr<UIComponent>>(componentMapping[id]);
		componentMapping.erase(id);
		std::remove(this->components.begin(), this->components.end(), opt.value());
	}
	return std::nullopt;
}

std::optional<std::shared_ptr<UIComponent>> ComponentManager::findComponent(const std::string_view& id) const {
	if (componentMapping.find(id) != componentMapping.end())
		return std::make_optional(componentMapping.at(id));
	return std::nullopt;
}

void ComponentManager::applyLayouts(std::shared_ptr<Window> window) {
	this->window = window;
	for (auto& component : components) {
		component->applyLayout(window, componentMapping, true);
	}
	this->hasAppliedLayout = true;
}

void ComponentManager::drawComponents(float delta, const Camera& camera) {
	Renderer::Begin2DScene(camera);
	for (auto& component : components) {
		if (component->isVisible()) component->draw(delta);
	}
	Renderer::End2DScene();
}

bool ComponentManager::onMouseButtonPressed(const MouseButton& button) {
	for (auto component : components) {
		if (component->onMouseButtonPressed(button)) return true;
	}
	return false;
}
bool ComponentManager::onMouseButtonReleased(const MouseButton& button) {
	for (auto& component : components) {
		if (component->onMouseButtonReleased(button)) return true;
	}
	return false;
}