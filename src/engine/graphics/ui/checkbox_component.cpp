#include "checkbox_component.h"

#include <string>

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>

constexpr std::string_view WIDGETS_TEXTURE = "./resources/textures/ui/widgets.png";

constexpr glm::vec4 WHITE_COLOR_ALPHA = glm::vec4{ 1.f, 1.f, 1.f, .4f };

constexpr glm::vec4 UNCHECKED_STPQ = glm::vec4{ 0.f, 0.f, 32.f / 256.f, 32.f / 256.f  };
constexpr glm::vec4 CHECKED_STPQ = glm::vec4{ 32.f / 256.f, 0.f, 64.f / 256.f, 32.f / 256.f  };

CheckboxComponent::CheckboxComponent(const std::string_view& id, Layout* layout, bool checked, Gravity gravity)
	: UIComponent(id, layout), checked(checked), gravity(gravity), contentSize(glm::vec2{ 32.f, 32.f }) {}

CheckboxComponent::CheckboxComponent(const std::string_view& id, Layout* layout, const glm::vec2 contentSize, bool checked, Gravity gravity)
	: UIComponent(id, layout), checked(checked), gravity(gravity), contentSize(contentSize) {}

float CheckboxComponent::getContentWidth() const {
	return this->contentSize.x;
}

float CheckboxComponent::getContentHeight() const {
	return this->contentSize.y;
}

const glm::vec3& CheckboxComponent::getPosition() const {
	static glm::vec3 pos;
	pos = UIComponent::getPosition();
	if ((gravity & Gravity::CENTER_HORIZONTAL) == Gravity::CENTER_HORIZONTAL) {
		pos.x -= this->getWidth() / 2.f;
	}
	if ((gravity & Gravity::CENTER_VERTICAL) == Gravity::CENTER_VERTICAL) {
		pos.y -= this->getHeight() / 2.f;
	}

	if ((gravity & Gravity::RIGHT) == Gravity::RIGHT) {
		pos.x -= this->getWidth();
	}

	if ((gravity & Gravity::TOP) == Gravity::TOP) {
		pos.y -= this->getHeight();
	}

	return pos;
}

void CheckboxComponent::draw(std::shared_ptr<Window> window, float delta) {
	const glm::vec2 size = this->size.value();
	glm::vec3 pos = position.value();
	if (gravity & Gravity::RIGHT) {
		pos.x -= size.x / 2.f;
	}
	else if (!(gravity & Gravity::CENTER_HORIZONTAL)) {
		pos.x += size.x / 2.f;
	}
	if (gravity & Gravity::TOP) {
		pos.y -= size.y / 2.f;
	}
	else if (!(gravity & Gravity::CENTER_VERTICAL)) {
		pos.y += size.y / 2.f;
	}

	if (this->checked) {
		Renderer::SubmitQuad(pos, this->size.value(), CHECKED_STPQ, AssetManager::GetOrCreate<Texture>(WIDGETS_TEXTURE));
	} else {
		Renderer::SubmitQuad(pos, this->size.value(), UNCHECKED_STPQ, AssetManager::GetOrCreate<Texture>(WIDGETS_TEXTURE));
	}
	if (this->isWithin({ Mouse::x, Mouse::y })) {
		Renderer::SubmitQuad(pos, this->size.value(), AssetManager::GetOrCreate<Texture>(WHITE_COLOR_ALPHA));
	}

}

bool CheckboxComponent::onMouseButtonReleased(const MouseButton& button) {
	if (this->isWithin(glm::vec2{ Mouse::x, Mouse::y })) {
		this->checked = !this->checked;
		return true;
	}
	return false;
}

bool CheckboxComponent::SetChecked(std::shared_ptr<CheckboxComponent> component, bool checked) {
	component->checked = checked;
	return false;
}
