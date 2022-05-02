#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include <src/engine/io/event_receiver.h>
#include <src/engine/graphics/window.h>

#include <glm/glm.hpp>

enum class Gravity : uint32_t {
	LEFT = 0, BOTTOM = 0,
	RIGHT = 1, CENTER_HORIZONTAL = 2,
	TOP = 4, CENTER_VERTICAL = 8,
	CENTER = 10, RIGHT_TOP = 5, CENTER_HOR_TOP = 6, CENTER_VER_RIGHT = 9
};

inline uint32_t operator |(const Gravity& g1, const Gravity& g2) {
	return static_cast<uint32_t>(g1) | static_cast<uint32_t>(g2);
}

inline uint32_t operator &(const Gravity& g1, const Gravity& g2) {
	return static_cast<uint32_t>(g1) & static_cast<uint32_t>(g2);
}

inline bool operator ==(const uint32_t& u, const Gravity& g) {
	return u == static_cast<uint32_t>(g);
}

inline bool operator ==(const Gravity& g, const uint32_t& u) {
	return static_cast<uint32_t>(g) == u;
}


class Layout;

class UIComponent : public EventReceiver {
public:
	UIComponent(const std::string_view& id, Layout* layout);
	UIComponent(const std::string_view& id, std::shared_ptr<Layout> layout);
	virtual ~UIComponent() = default;

	virtual float getContentWidth() const = 0;
	virtual float getContentHeight() const = 0;

	virtual void applyLayout(const std::shared_ptr<Window> window, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers, bool update = false);
	
	virtual void draw(float delta) = 0;

	inline const std::string_view& getID() const { return id; }
	inline const std::shared_ptr<Layout> getLayout() const { return layout; }

	virtual inline const glm::vec3& getPosition() const { return position.value(); }
	virtual inline const glm::vec2& getSize() const { return size.value(); }

	virtual inline float getX() const { return position.value().x; }
	virtual inline float getY() const { return position.value().y; }
	virtual inline float getZ() const { return position.value().z; }

	virtual inline float getWidth() const { return size.value().x; }
	virtual inline float getHeight() const { return size.value().y; }

	virtual inline bool isVisible() const { return visible; }
	// virtual inline void setVisible(bool visible) { this->visible = visible; }

	virtual inline bool isWithin(const glm::vec2& pos) const {
		const glm::vec3 position = this->position.value();
		const glm::vec2 size = this->size.value();
		return pos.x >= position.x && pos.y >= position.y && pos.x < position.x + size.x && pos.y < position.y + size.y;
	}
	
	static bool ChangeLayout(std::shared_ptr<UIComponent> component, Layout* layout);
	template<typename T>
	static bool ChangeLayout(std::shared_ptr<T> component, std::function<Layout*(std::shared_ptr<T>)> func) {
		static_assert(std::is_base_of<UIComponent, T>);
		auto layout = func(component);
		if (layout) {
			component->layout = layout;
			return true;
		}
		return false;
	}

	static bool ChangeVisibility(std::shared_ptr<UIComponent> component, bool visibility);
	template<typename T>
	static bool ChangeVisibility(std::shared_ptr<UIComponent> component, std::function<bool(std::shared_ptr<T>)> func) {
		static_assert(std::is_base_of<UIComponent, T>);
		bool visible = func(component);
		component->visible = visible;
		return false;
	}

protected:
	const std::string_view id;
	bool evaluating = false;
	std::shared_ptr<Layout> layout;

	std::optional<glm::vec3> position;
	std::optional<glm::vec2> size;
	bool visible = true;
};


class Layout {
public:
	virtual ~Layout() = default;

	virtual glm::vec3 position(
		const std::shared_ptr<Window> window, const UIComponent& self,
		const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers
	) = 0;

	virtual glm::vec2 size(
		const std::shared_ptr<Window> window, const UIComponent& self,
		const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers
	) = 0;

};
