#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <functional>

#include <src/engine/graphics/camera.h>
#include <src/engine/graphics/ui/layout.h>

class ComponentManager : public EventReceiver {
public:
	ComponentManager(std::shared_ptr<Window> window);

	/**
	 * @brief Add a UIComponent to this manager.
	 * 
	 * @param component The UIComponent to add.
	 */
	void addComponent(std::shared_ptr<UIComponent> component);

	/**
	 * @brief Add a UIComponent to this manager.
	 * 
	 * @param component The UIComponent to add.
	 */
	inline void addComponent(UIComponent* component) { addComponent(std::shared_ptr<UIComponent>(component)); }

	/**
	 * @brief Remove a specific UIComponent from this manager if it exists.
	 * 
	 * @param component The specific component to remove.
	 */
	void removeComponent(std::shared_ptr<UIComponent> component);

	/**
	 * @brief Remove a UIComponent with a specific id.
	 * 
	 * @param id The id of the UIComponent to remove.
	 * @return An optional containing a UIComponent if a UIComponent with id has been removed.
	 */
	std::optional<std::shared_ptr<UIComponent>> removeComponent(const std::string_view& id);
	
	/**
	 * @brief Modify a UIComponent with a specific id with the supplied function and arguments. 
	 * 
	 * @tparam T The type of UIComponent which will be modifying.
	 * @tparam Args The arguments other than the UIComponent needed to call modification.
	 * @param id The id of the UIComponent to modify.
	 * @param modification The function which operates on the UIComponent. If it returns true layouts will be reapplied.
	 * @param args The additional arguments to pass to modification.
	 */
	template<typename T, typename...Args>
	void modifyComponent(const std::string_view& id, std::function<bool(std::shared_ptr<T>, Args...)> modification, Args... args) {
		if (componentMapping.find(id) != componentMapping.end()) {
			std::shared_ptr<T> casted = std::reinterpret_pointer_cast<T>(componentMapping[id]);
			assert(casted && "Type mismatch between components.");
			if (modification(casted, args...))
				applyLayouts(this->window);
		}
	}

	/**
	 * @brief Modify a specific UIComponent with the supplied function and arguments.
	 * 
	 * @tparam T The type of UIComponent which will be modifying.
	 * @tparam Args The arguments other than the UIComponent needed to call modification.
	 * @param id The id of the UIComponent to modify.
	 * @param modification The function which operates on the UIComponent. If it returns true layouts will be reapplied.
	 * @param args The additional arguments to pass to modification.
	 */
	template<typename T, typename... Args>
	void modifyComponent(std::shared_ptr<UIComponent> component, std::function<bool(std::shared_ptr<T>, Args...)> modification, Args... args) {
		if (componentMapping.find(component->getID()) != componentMapping.end()) {
			std::shared_ptr<T> casted = std::reinterpret_pointer_cast<T>(component);
			assert(casted && "Type mismatch between components.");
			if (modification(casted, args...))
				applyLayouts(this->window);
		}
	}

	/**
	 * @brief Obtain a UIComponent if one exists with the specific id.
	 * 
	 * @param id The id of the UIComponent to find.
	 * @return An optional containing the UIComponent if one exists with the specified id. 
	 */
	std::optional<std::shared_ptr<UIComponent>> findComponent(const std::string_view& id) const;

	/**
	 * @brief Apply the layout(s) for each UIComponent inside the manager.
	 * 
	 * @param window The window where the UIComponent's will be rendered.
	 */
	void applyLayouts(std::shared_ptr<Window> window);

	/**
	 * @brief Display all of the UIComponents using Renderer.
	 * 
	 * @param delta The time since the last frame.
	 * @param camera The camera(Orthographic) with which to display the UIComponents.
	 */
	void drawComponents(float delta, const Camera& camera);

	/** Allow for iterating over each UIComponent with a range based for loop. **/
	inline std::vector<std::shared_ptr<UIComponent>>::iterator begin() { return components.begin(); }
	inline std::vector<std::shared_ptr<UIComponent>>::iterator end() { return components.end(); }

	/** Allow for iterating over each UIComponent with a range based for loop. **/
	inline std::vector<std::shared_ptr<UIComponent>>::const_iterator cbegin() const { return components.cbegin(); }
	inline std::vector<std::shared_ptr<UIComponent>>::const_iterator cend() const { return components.cend(); }

	/** Used to dispatch the events to each component. **/
	virtual bool onKeyPressed(const Key& key) override;
	virtual bool onKeyReleased(const Key& key) override;
	virtual bool onKeyRepeated(const Key& key) override;

	virtual bool onKeyChar(const char* text) override;

	virtual bool onMouseButtonPressed(const MouseButton& button) override;
	virtual bool onMouseButtonReleased(const MouseButton& button) override;
	virtual bool onMouseDoubleClick(const MouseButton& button) override;
	virtual bool onMouseWheelScroll(float xOffset, float yOffset) override;
	virtual bool onMouseMoved(float x, float y, float dx, float dy) override;

private:
	bool hasAppliedLayout = false;
	std::shared_ptr<Window> window = nullptr;
	std::vector<std::shared_ptr<UIComponent>> components;
	std::unordered_map<std::string_view, std::shared_ptr<UIComponent>> componentMapping;
};
