#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <functional>

#include <src/engine/graphics/camera.h>
#include <src/engine/graphics/ui/layout.h>

class ComponentManager {
public:
	ComponentManager() = default;

	void addComponent(std::shared_ptr<UIComponent> component);
	inline void addComponent(UIComponent* component) { addComponent(std::shared_ptr<UIComponent>(component)); }
	void removeComponent(std::shared_ptr<UIComponent> component);
	std::optional<std::shared_ptr<UIComponent>> removeComponent(const std::string_view& id);
	
	template<typename T, typename...Args>
	void modifyComponent(const std::string_view& id, std::function<bool(std::shared_ptr<T>, Args...)> modification, Args... args) {
		if (componentMapping.find(id) != componentMapping.end()) {
			std::shared_ptr<T> casted = std::dynamic_pointer_cast<T>(componentMapping[id]);
			assert(casted && "Type mismatch between components.");
			if (modification(casted, args...))
				applyLayouts(this->window);
		}
	}

	template<typename T, typename... Args>
	void modifyComponent(std::shared_ptr<UIComponent> component, std::function<bool(std::shared_ptr<T>, Args...)> modification, Args... args) {
		if (componentMapping.find(component->getID()) != componentMapping.end()) {
			std::shared_ptr<T> casted = std::dynamic_pointer_cast<T>(component);
			assert(casted && "Type mismatch between components.");
			if (modification(casted, args...))
				applyLayouts(this->window);
		}
	}

	std::optional<std::shared_ptr<UIComponent>> findComponent(const std::string_view& id) const;

	void applyLayouts(std::shared_ptr<Window> window);

	void drawComponents(float delta, const Camera& camera);

	inline std::vector<std::shared_ptr<UIComponent>>::iterator begin() { return components.begin(); }
	inline std::vector<std::shared_ptr<UIComponent>>::iterator end() { return components.end(); }

	inline std::vector<std::shared_ptr<UIComponent>>::const_iterator cbegin() const { return components.cbegin(); }
	inline std::vector<std::shared_ptr<UIComponent>>::const_iterator cend() const { return components.cend(); }

private:
	bool hasAppliedLayout = false;
	std::shared_ptr<Window> window = nullptr;
	std::vector<std::shared_ptr<UIComponent>> components;
	std::unordered_map<std::string_view, std::shared_ptr<UIComponent>> componentMapping;
};
