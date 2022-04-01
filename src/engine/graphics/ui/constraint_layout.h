#pragma once

#include <src/engine/graphics/ui/layout.h>

class ConstraintLayout : public Layout {
public:
	ConstraintLayout(const std::string_view& top, const std::string_view& bottom, const std::string_view& left, const std::string_view& right, const std::string_view& infront);

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
