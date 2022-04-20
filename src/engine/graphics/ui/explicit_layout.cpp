#pragma once
#include "explicit_layout.h"

#include <string>

ExplicitLayout::ExplicitLayout(Literal x, Literal y, Literal width, Literal height, Literal z)
	: x(std::variant<Literal, Percentage>{ std::in_place_index<0>, x }),
	  y(std::variant<Literal, Percentage>{ std::in_place_index<0>, y }),
	  z(std::variant<Literal, Percentage>{ std::in_place_index<0>, z }),
	  width(std::variant<Literal, Percentage>{ std::in_place_index<0>, width }),
	  height(std::variant<Literal, Percentage>{ std::in_place_index<0>, height }) {}

ExplicitLayout::ExplicitLayout(const std::string_view& x, const std::string_view& y, const std::string_view& width, const std::string_view& height, const std::string_view& z) {
	assert(!x.empty() && !y.empty() && !z.empty() && "x, y, and z cannot be empty.");
	this->x = ExplicitLayout::GetValue(x).value();
	this->y = ExplicitLayout::GetValue(y).value();
	this->z = ExplicitLayout::GetValue(z).value();
	this->width = ExplicitLayout::GetValue(width);
	this->height = ExplicitLayout::GetValue(height);
}

glm::vec3 ExplicitLayout::position(const std::shared_ptr<Window> window, const UIComponent& self, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers) {
	return glm::vec3{
		this->x.index() == 0 ? std::get<0>(this->x) : window->getData().size.x * std::get<1>(this->x) - window->getData().size.x / 2.0f,
		this->y.index() == 0 ? std::get<0>(this->y) : window->getData().size.y * std::get<1>(this->y) - window->getData().size.y / 2.0f,
		this->z.index() == 0 ? std::get<0>(this->z) : std::get<1>(this->z)
	};
}

glm::vec2 ExplicitLayout::size(const std::shared_ptr<Window> window, const UIComponent& self, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers) {
	const std::variant<Literal, Percentage> defaultWidth{std::in_place_index<0>, self.getContentWidth()};
	const std::variant<Literal, Percentage> defaultHeight{std::in_place_index<0>, self.getContentHeight()};
	return glm::vec2{
		this->width.value_or(defaultWidth).index() == 0 ? std::get<0>(this->width.value_or(defaultWidth)) : window->getData().size.x * std::get<1>(this->width.value()),
		this->height.value_or(defaultHeight).index() == 0 ? std::get<0>(this->height.value_or(defaultHeight)) : window->getData().size.y * std::get<1>(this->height.value())
	};

}

std::optional<std::variant<ExplicitLayout::Literal, ExplicitLayout::Percentage>> ExplicitLayout::GetValue(const std::string_view& view) {
	if (view.empty()) {
		return std::nullopt;
	} else if (*view.rbegin() == '%') {
		return std::variant<ExplicitLayout::Literal, ExplicitLayout::Percentage>{std::in_place_index<1>, std::stof(std::string{ view.substr(0, view.size() - 1) }) / 100.0f};
	} else {
		return std::variant<ExplicitLayout::Literal, ExplicitLayout::Percentage>{std::in_place_index<0>, std::stof(std::string{ view })};
	}
}
