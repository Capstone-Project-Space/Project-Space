#include "relative_layout.h"
#include <functional>

RelativeLayout::RelativeLayout(const std::string_view& left, const std::string_view& top, const std::string_view& right, const std::string_view& bottom, const std::string_view& infront)
	: leftID(left.substr(0, left.find_first_of(':'))), rightID(right.substr(0, right.find_first_of(':'))),
	  topID(top.substr(0, top.find_first_of(':'))), bottomID(bottom.substr(0, bottom.find_first_of(':'))),
	  infrontID(infront.empty() ? "window" : infront.substr(0, infront.find_first_of(':'))),
	  leftSide(RelativeLayout::FromSide(left.substr(left.find_first_of(':') + 1, left.find_last_of(':') - left.find_first_of(':') - 1))),
	  rightSide(RelativeLayout::FromSide(right.substr(right.find_first_of(':') + 1, right.find_last_of(':') - right.find_first_of(':') - 1))),
	  topSide(RelativeLayout::FromSide(top.substr(top.find_first_of(':') + 1, top.find_last_of(':') - top.find_first_of(':') - 1))),
	  bottomSide(RelativeLayout::FromSide(bottom.substr(bottom.find_first_of(':') + 1, bottom.find_last_of(':') - bottom.find_first_of(':') - 1))),
	  leftX(RelativeLayout::GetValue(left.substr(left.find_last_of(':') + 1))),
	  rightX(RelativeLayout::GetValue(right.substr(right.find_last_of(':') + 1))),
	  topY(RelativeLayout::GetValue(top.substr(top.find_last_of(':') + 1))),
	  bottomY(RelativeLayout::GetValue(bottom.substr(bottom.find_last_of(':') + 1))),
	  infrontZ(RelativeLayout::GetValue(infront.substr(infront.find_last_of(':') + 1))) {
	assert(infrontZ.has_value());
	assert(leftX.has_value() || rightX.has_value());
	assert(topY.has_value() || bottomY.has_value());
}

glm::vec3 RelativeLayout::position(const std::shared_ptr<Window> window, const UIComponent& self, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers) {
	glm::vec3 pos{INFINITY, INFINITY, 0.0f};

	if (leftX.has_value()) {
		pos.x = RelativeLayout::GetValue(RelativeLayout::GetRelativeValue(window, peers, this->leftID, this->leftSide), leftX.value());
	}
	if (rightX.has_value()) {
		pos.x = std::min(pos.x, RelativeLayout::GetValue(RelativeLayout::GetRelativeValue(window, peers, this->rightID, this->rightSide), rightX.value()) - self.getWidth());
	}

	if (topY.has_value()) {
		pos.y = RelativeLayout::GetValue(RelativeLayout::GetRelativeValue(window, peers, this->topID, this->topSide), topY.value()) - self.getHeight();
	}
	if (bottomY.has_value()) {
		pos.y = std::min(pos.y, RelativeLayout::GetValue(RelativeLayout::GetRelativeValue(window, peers, this->bottomID, this->bottomSide), bottomY.value()));
	}

	pos.z = RelativeLayout::GetValue(RelativeLayout::GetRelativeValue(window, peers, this->infrontID, Side::INFRONT), infrontZ.value());

	return pos;
}

glm::vec2 RelativeLayout::size(const std::shared_ptr<Window> window, const UIComponent& self, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers) {
	glm::vec2 size{0.0f};

	if (leftX.has_value() && rightX.has_value()) {
		float lx = RelativeLayout::GetValue(RelativeLayout::GetRelativeValue(window, peers, this->leftID, this->leftSide), leftX.value());
		float rx = RelativeLayout::GetValue(RelativeLayout::GetRelativeValue(window, peers, this->rightID, this->rightSide), rightX.value());
		size.x = std::fabsf(lx - rx);
	} else {
		size.x = self.getContentWidth();
	}

	if (topY.has_value() && bottomY.has_value()) {
		float ty = RelativeLayout::GetValue(RelativeLayout::GetRelativeValue(window, peers, this->topID, this->topSide), topY.value());
		float by = RelativeLayout::GetValue(RelativeLayout::GetRelativeValue(window, peers, this->bottomID, this->bottomSide), bottomY.value());
		size.y = std::fabsf(ty - by);
	}
	else {
		size.y = self.getContentHeight();
	}

	return size;
}


RelativeLayout::Side RelativeLayout::FromSide(const std::string_view& side) {
	if (side == std::string_view{ "left" }) {
		return Side::LEFT;
	} else if (side == std::string_view{ "right" }) {
		return Side::RIGHT;
	} else if (side == std::string_view{ "top" }) {
		return Side::TOP;
	} else if (side == std::string_view{ "bottom" }) {
		return Side::BOTTOM;
	} else {
		return Side::NONE;
	}
}

std::pair<float, float> RelativeLayout::GetRelativeValue(const std::shared_ptr<Window> window, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers, const std::string_view& id, const Side side) {
	if (id == std::string_view{ "window" }) {
		switch (side) {
		case Side::LEFT:
			return { -window->getData().size.x / 2.0f, window->getData().size.x };
		case Side::RIGHT:
			return { window->getData().size.x / 2.0f, window->getData().size.x };
		case Side::TOP:
			return { window->getData().size.y / 2.0f, window->getData().size.y };
		case Side::BOTTOM:
			return { -window->getData().size.y / 2.0f, window->getData().size.y };
		case Side::INFRONT:
			return { 0.0f, 0.0f };
		case Side::NONE:
			throw "Got Side::NONE when evaluating relative value.";
		}
	}
	else {
		auto peer = peers.at(id);
		peer->applyLayout(window, peers);
		switch (side) {
		case Side::LEFT:
			return { peer->getX(), peer->getWidth() };
		case Side::RIGHT:
			return { peer->getX() + peer->getWidth(), peer->getWidth()};
		case Side::TOP:
			return { peer->getY() + peer->getHeight(), peer->getHeight() };
			case Side::BOTTOM:
			return { peer->getY(), peer->getHeight() };
		case Side::INFRONT:
			return { peer->getZ(), 0.0f };
		case Side::NONE:
			throw "Got Side::NONE when evaluating relative value.";
		}
	}
}

float RelativeLayout::GetRelativeSize(const std::shared_ptr<Window> window, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers, const std::string_view& id, const Side side) {
	if (id == std::string_view{ "window" }) {
		switch (side) {
		case Side::LEFT:
			return -window->getData().size.x / 2.0f;
		case Side::RIGHT:
			return window->getData().size.x / 2.0f;
		case Side::TOP:
			return window->getData().size.y / 2.0f;
		case Side::BOTTOM:
			return -window->getData().size.y / 2.0f;
		case Side::INFRONT:
			return 0.0f;
		case Side::NONE:
			throw "Got Side::NONE when evaluating relative value.";
		}
	}
	else {
		auto peer = peers.at(id);
		peer->applyLayout(window, peers);
		switch (side) {
		case Side::LEFT: case Side::RIGHT:
			return peer->getWidth();
		case Side::TOP: case Side::BOTTOM:
			return peer->getHeight();
		case Side::INFRONT:
			return peer->getZ();
		case Side::NONE:
			throw "Got Side::NONE when evaluating relative value.";
		}
	}
}


using Literal = RelativeLayout::Literal;
using Percentage = RelativeLayout::Percentage;
std::optional<std::variant<Literal, Percentage>> RelativeLayout::GetValue(const std::string_view& view) {
	std::optional<std::variant<Literal, Percentage>> value = std::nullopt;
	if (view.empty()) return value;
	if (*view.rbegin() == '%') {
		value.emplace(std::variant<Literal, Percentage>{std::in_place_index<1>, std::stof(std::string{ view.substr(0, view.size() - 1)}) / 100.0f});
	} else {
		value.emplace(std::variant<Literal, Percentage>{std::in_place_index<0>, std::stof(std::string{ view.substr(0, view.size())})});
	}
	return value;
}


float RelativeLayout::GetValue(const std::pair<float, float> target, const std::variant<Literal, Percentage>& position) {
	if (position.index() == 0) {
		return target.first + std::get<0>(position);
	}
	return target.first + target.second * std::get<1>(position);
}
