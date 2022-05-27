#include "component_manager.h"

#include <iostream>

#include <src/engine/graphics/renderer.h>

ComponentManager::ComponentManager(std::shared_ptr<Window> window) {
	this->window = window;
}

void ComponentManager::addComponent(std::shared_ptr<UIComponent> component) {
	assert(componentMapping.find(component->getID()) == componentMapping.end() && "No duplicate IDs allowed. First remove the id then add.");
	this->components.push_back(component);
	this->componentMapping[component->getID()] = component;
	if (this->hasAppliedLayout) component->applyLayout(this->window, componentMapping);
}

void ComponentManager::removeComponent(std::shared_ptr<UIComponent> component) {
	if (componentMapping.find(component->getID()) != componentMapping.end()) {
		this->components.erase(std::remove(this->components.begin(), this->components.end(), component), this->components.end());
		this->componentMapping.erase(component->getID());
	}
}

std::optional<std::shared_ptr<UIComponent>> ComponentManager::removeComponent(const std::string_view& id) {
	if (componentMapping.find(id) != componentMapping.end()) {
		auto opt = std::make_optional<std::shared_ptr<UIComponent>>(componentMapping[id]);
		componentMapping.erase(id);
		this->components.erase(std::remove(this->components.begin(), this->components.end(), opt.value()), this->components.end());
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
	for (auto& component : components) {
		std::cerr << component->getID() << ": {" << component->getPosition().x << ", " << component->getPosition().y << "} {" << component->getSize().x << ", " << component->getSize().y << "}\n";
	}
	this->hasAppliedLayout = true;
}

void ComponentManager::drawComponents(float delta, const Camera& camera) {
	Renderer::Begin2DScene(camera);
	for (auto& component : components) {
		if (component->isVisible()) component->draw(this->window, delta);
	}
	Renderer::End2DScene();
}

bool ComponentManager::onKeyPressed(const Key& key) {
	for (auto& component : components) {
		if (component->onKeyPressed(key)) return true;
	}
	return false;
}

bool ComponentManager::onKeyReleased(const Key& key) {
	for (auto& component : components) {
		if (component->onKeyReleased(key)) return true;
	}
	return false;
}

bool ComponentManager::onKeyRepeated(const Key& key) {
	for (auto& component : components) {
		if (component->onKeyRepeated(key)) return true;
	}
	return false;
}

bool ComponentManager::onKeyChar(const char* text) {
	for (auto& component : components) {
		if (component->onKeyChar(text)) return true;
	}
	return false;
}

bool ComponentManager::onMouseButtonPressed(const MouseButton& button) {
	for (auto& component : components) {
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

bool ComponentManager::onMouseDoubleClick(const MouseButton& button) {
	for (auto& component : components) {
		if (component->onMouseDoubleClick(button)) return true;
	}
	return false;
}

bool ComponentManager::onMouseWheelScroll(float xOffset, float yOffset) {
	for (auto& component : components) {
		if (component->onMouseWheelScroll(xOffset, yOffset)) return true;
	}
	return false;
}

bool ComponentManager::onMouseMoved(float x, float y, float dx, float dy) {
	for (auto& component : components) {
		if (component->onMouseMoved(x, y, dx, dy)) return true;
	}
	return false;
}
