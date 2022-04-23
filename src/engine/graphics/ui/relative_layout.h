#pragma once

#include <variant>

#include <src/engine/graphics/ui/layout.h>

class RelativeLayout : public Layout {
public:
	using Literal = float;
	using Percentage = float;
	RelativeLayout(const std::string_view& left, const std::string_view& topY, const std::string_view& right = "", const std::string_view& bottom = "", const std::string_view& infront="window:0.0");
	
	virtual glm::vec3 position(
		const std::shared_ptr<Window> window, const UIComponent& self, 
		const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers
	) override;

	virtual glm::vec2 size(
		const std::shared_ptr<Window> window, const UIComponent& self, 
		const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers
	) override;

private:
	const std::string_view leftID, topID, rightID, bottomID, infrontID;
	const std::optional<std::variant<Literal, Percentage>> leftX, topY, rightX, bottomY, infrontZ;

	enum class Side { NONE, LEFT, RIGHT, TOP, BOTTOM, INFRONT };
	Side leftSide, topSide, rightSide, bottomSide;

	static Side FromSide(const std::string_view& side);
	static float GetRelativeValue(const std::shared_ptr<Window> window, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers, const std::string_view& id, const Side side);
	static std::optional<std::variant<Literal, Percentage>> GetValue(const std::string_view& view);
	static float GetValue(float targetX, const std::variant<Literal, Percentage>& position);
};
