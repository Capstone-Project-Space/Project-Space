#include "constraint_layout.h"

// TODO: Modify this to work all the time. Currently crashes if size is the difference between two locations.
ConstraintLayout::ConstraintLayout(const std::string_view& left, const std::string_view& top, const std::string_view& right, const std::string_view& bottom, const std::string_view& infront)
	: top(top), bottom(bottom), left(left), right(right), infront(infront.empty() ? "window" : infront) {
	assert(!top.empty() || !bottom.empty() && "Missing vertical constraint.");
	assert(!left.empty() || !right.empty() && "Missing horizontal constraint.");
}


glm::vec3 ConstraintLayout::position(const std::shared_ptr<Window> window, const UIComponent& self, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers) {
	glm::vec3 pos{ 0.0f };
	bool empties[] = {
		!this->left.empty(),
		!this->top.empty()
	};

	const auto horizontal = empties[0] ? &this->left : &this->right;
	const auto vertical = empties[1] ? &this->top : &this->bottom;

	auto colonHoriz = horizontal->find_last_of(':');
	auto idHoriz = horizontal->substr(0, colonHoriz);
	auto relationHoriz = horizontal->substr(colonHoriz+1);
	if (idHoriz == std::string_view{ "window" }) {
		if (relationHoriz == std::string_view{ "left" }) {
			pos.x = -window->getData().size.x / 2.0f;
		} else if (relationHoriz == std::string_view{ "right" }) {
			pos.x = window->getData().size.x / 2.0f;
		} else if (relationHoriz == std::string_view{ "center" }) {
			// pos.x = 0.0f;
		} else assert(false && "The only applicable alignments are left, right, and center.");
	} else {
		auto& peerHoriz = peers.at(idHoriz);
		peerHoriz->applyLayout(window, peers);
		pos.x = ConstraintLayout::GetXFromSide(peerHoriz, relationHoriz);
	}
	if (!empties[0])
		pos.x -= self.getWidth();

	auto colonVert = vertical->find_last_of(':');
	auto idVert = vertical->substr(0, colonVert);
	auto relationVert = vertical->substr(colonVert + 1);
	if (idVert == std::string_view{ "window" }) {
		if (relationVert == std::string_view{ "top" }) {
			pos.y = window->getData().size.y / 2.0f;
		} else if (relationVert == std::string_view{ "bottom" }) {
			pos.y = -window->getData().size.y / 2.0f;
		} else if (relationVert == std::string_view{ "center" }) {
			// pos.y = 0.0f;
		} else assert(false && "The only applicable alignments are left, right, and center.");
	} else {
		auto& peerVert = peers.at(idVert);
		peerVert->applyLayout(window, peers);
		pos.y = ConstraintLayout::GetYFromSide(peerVert, relationVert);
	}
	if (empties[1])
		pos.y -= self.getHeight();

	if (this->infront == std::string_view{ "window" }) {
		// pos.z = 0.0f;
	} else {
		auto& peer = peers.at(this->infront);
		peer->applyLayout(window, peers);
		pos.z = peer->getZ() + 0.01f;
	}

	return pos;
}

glm::vec2 ConstraintLayout::size(const std::shared_ptr<Window> window, const UIComponent& self, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers) {
	glm::vec2 size{ 0.0f };

	if (!this->left.empty() && !this->right.empty()) {
		auto colon = this->right.find_last_of(':');
		auto& rightPeer = peers.at(this->right.substr(0, colon));
		rightPeer->applyLayout(window, peers);
		auto relation = this->right.substr(colon + 1);
		size.x = ConstraintLayout::GetXFromSide(rightPeer, relation);

		colon = this->left.find_last_of(':');
		auto& leftPeer = peers.at(this->left.substr(0, colon));
		leftPeer->applyLayout(window, peers);
		relation = this->left.substr(colon + 1);
		size.x -= ConstraintLayout::GetXFromSide(leftPeer, relation);
	} else {
		size.x = self.getContentWidth();
	}

	if (!this->top.empty() && !this->bottom.empty()) {
		auto colon = this->top.find_last_of(':');
		auto& topPeer = peers.at(this->top.substr(0, colon));
		topPeer->applyLayout(window, peers);
		auto relation = this->top.substr(colon + 1);
		size.y = ConstraintLayout::GetYFromSide(topPeer, relation);

		colon = this->bottom.find_last_of(':');
		auto& bottomPeer = peers.at(this->bottom.substr(0, colon));
		bottomPeer->applyLayout(window, peers);
		relation = this->bottom.substr(colon + 1);
		size.y -= ConstraintLayout::GetYFromSide(bottomPeer, relation);
	} else {
		size.y = self.getContentHeight();
	}

	return size;
}


float ConstraintLayout::GetXFromSide(const std::shared_ptr<UIComponent> component, const std::string_view& side) {
	if (side == std::string_view{ "left" }) {
		return component->getX();
	} else if (side == std::string_view{ "right" }) {
		return component->getX() + component->getWidth();
	} else if (side == std::string_view{ "center" }) {
		return component->getX() + (component->getWidth() / 2.0f);
	} else throw "The only applicable alignments are left, right, and center.";
}

float ConstraintLayout::GetYFromSide(const std::shared_ptr<UIComponent> component, const std::string_view& side) {
	if (side == std::string_view{ "top" }) {
		return component->getY() + component->getHeight();
	} else if (side == std::string_view{ "bottom" }) {
		return component->getY();
	} else if (side == std::string_view{ "center" }) {
		return component->getY() - (component->getHeight() / 2.0f);
	} else throw "The only applicable alignments are top, bottom, and center.";
}