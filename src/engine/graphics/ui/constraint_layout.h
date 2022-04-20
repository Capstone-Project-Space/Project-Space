#pragma once

#include <src/engine/graphics/ui/layout.h>

/**
 * @brief ConstraintLayout constrains the x, y, and z position to the boundary of another UIComponent.
 * Format for a constraint is "id:side". Horizontal sides: left, right, center. Vertical sides: top, bottom, center.
 * A ConstraintLayout must have at least 1 horizontal and vertical constraint.
 * The id for the window/screen is just "window".
 * Having both of the vertical or both of the horizontal constraints will set the height or width to the distance between them.
 */
class ConstraintLayout : public Layout {
public:
	ConstraintLayout(const std::string_view& left, const std::string_view& top, const std::string_view& right, const std::string_view& bottom, const std::string_view& infront = "window");

	operator Layout*() {
		return reinterpret_cast<Layout*>(this);
	}

	virtual glm::vec3 position(
		const std::shared_ptr<Window> window, const UIComponent& self,
		const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers
	) override;

	virtual glm::vec2 size(
		const std::shared_ptr<Window> window, const UIComponent& self,
		const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers
	) override;

private:
	const std::string_view top, bottom, left, right, infront;

	static float GetXFromSide(const std::shared_ptr<UIComponent> component, const std::string_view& relation);
	static float GetYFromSide(const std::shared_ptr<UIComponent> component, const std::string_view& relation);

};
