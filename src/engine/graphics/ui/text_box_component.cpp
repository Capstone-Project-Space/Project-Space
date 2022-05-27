#include "text_box_component.h"

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>

TextBoxComponent::TextBoxComponent(
	const std::string_view& id, Layout* layout, const std::string& defaultText, std::shared_ptr<Font> font,
	glm::vec3 color, glm::vec3 defaultColor, float scale, Gravity gravity
) : UIComponent(id, layout), text(), defaultText(defaultText), font(font),
	color(color), defaultColor(defaultColor), scale(scale), gravity(gravity),
	dimensions({defaultText.size(), 1}) {}

TextBoxComponent::TextBoxComponent(
	const std::string_view& id, Layout* layout, const std::string& defaultText, std::shared_ptr<Font> font,
	glm::vec3 color, glm::vec3 defaultColor, glm::vec2 dimensions, float scale, Gravity gravity
) : UIComponent(id, layout), text(), defaultText(defaultText), font(font),
	color(color), defaultColor(defaultColor), scale(scale), gravity(gravity),
	dimensions(dimensions) {}

float TextBoxComponent::getContentWidth() const {
	return this->dimensions.x * this->font->getFontWidth() * this->scale;
}

float TextBoxComponent::getContentHeight() const {
	return this->dimensions.y * this->font->getNewLineHeight() * this->scale;
}

const glm::vec3& TextBoxComponent::getPosition() const {
	static glm::vec3 pos;
	pos = UIComponent::getPosition();
	const glm::vec2& size = this->size.value();
	if (gravity & Gravity::CENTER_HORIZONTAL) {
		pos.x -= size.x / 2.f;
	}
	if (gravity & Gravity::CENTER_VERTICAL) {
		pos.y -= size.y / 2.f;
	}
	if (gravity & Gravity::RIGHT) {
		pos.x -= size.x;
	}
	if (gravity & Gravity::TOP) {
		pos.y -= size.y / 2.f;
	}

	return pos;
}

void TextBoxComponent::draw(std::shared_ptr<Window> window, float delta) {
	const glm::vec2 scale = this->scale * window->getData().scale;
	glm::vec3 pos = position.value();
	if (gravity & Gravity::RIGHT) {
		pos.x -= this->size.value().x / 2.f;
	}
	else if (!(gravity & Gravity::CENTER_HORIZONTAL)) {
		pos.x += this->size.value().x / 2.f;
	}
	if (gravity & Gravity::TOP) {
		pos.y -= this->size.value().y / 2.f;
	}
	else if (!(gravity & Gravity::CENTER_VERTICAL)) {
		pos.y += this->size.value().y / 2.f;
	}
	Renderer::SubmitQuad(pos, this->size.value() * 1.3f, AssetManager::GetOrCreate<Texture>("./resources/textures/ui/border.png"));
	if (this->text.empty()) {
		Renderer::SubmitText(this->defaultText, this->position.value(), defaultColor, this->font, gravity, scale);
	} else {
		Renderer::SubmitText(this->text, this->position.value(), color, this->font, gravity, scale);
	}

	if (this->focused) {
		if (this->cursorTimer < .6) {
			glm::vec3 position = this->position.value();
			float yOffset = -((float) font->getNewLineHeight() * (float) cursorPos.y);
			float xOffset = font->getTextWidth(getLine(this->cursorPos.y).substr(0, this->cursorPos.x));
			if (gravity & Gravity::CENTER_HORIZONTAL) xOffset /= 2;
			else if (gravity & Gravity::RIGHT) xOffset = 0;
			position += glm::vec3{ xOffset, yOffset, 0.f } * glm::vec3{ scale, .0f};
			Renderer::SubmitText(
				"|", position,
				color, this->font, gravity, scale
			);
		} else if (this->cursorTimer > 1.2) this->cursorTimer = 0.f;
		this->cursorTimer += delta;
	}
}

bool TextBoxComponent::onMouseButtonPressed(const MouseButton& button) {
	if (button == GLFW_MOUSE_BUTTON_1 && isWithin({ Mouse::x, Mouse::y })) {
		this->focused = true;
	} else if (button == GLFW_MOUSE_BUTTON_1) {
		this->focused = false;
		this->cursorTimer = 0.f;
	}
	return false;
}

bool TextBoxComponent::onKeyChar(const char* text) {
	if (this->focused) {
		this->text.insert(getOffset(this->cursorPos), text);
		this->cursorPos.x++;
		return true;
	}
	return false;
}

bool TextBoxComponent::onKeyPressed(const Key& key) {
	if (this->focused) {
		if (key.keyCode == GLFW_KEY_BACKSPACE) {
			if (!this->text.empty() && (this->cursorPos.x != 0 || this->cursorPos.y != 0)) {
				uint32_t offset = getOffset(this->cursorPos) - 1;
				if (this->text[offset] == '\n') {
					this->lineCount--;
					this->cursorPos.y--;
					this->cursorPos.x = getLine(this->cursorPos.y).size();
				} else this->cursorPos.x--;
				this->text.erase(this->text.begin() + offset);
			}
		} else if (key.keyCode == GLFW_KEY_ENTER || key.keyCode == GLFW_KEY_KP_ENTER) {
			this->text.insert(this->text.begin() + getOffset(this->cursorPos), '\n');
			this->lineCount++;
			this->cursorPos.y++;
			this->cursorPos.x = 0;
		} else if (key.keyCode == GLFW_KEY_UP) {
			this->cursorPos.y = std::max(0, this->cursorPos.y - 1);
			this->cursorPos.x = std::min((size_t) this->cursorPos.x, getLine(this->cursorPos.y).size());
		} else if (key.keyCode == GLFW_KEY_DOWN) {
			this->cursorPos.y = std::min(this->lineCount-1, this->cursorPos.y + 1);
			this->cursorPos.x = std::min((size_t)this->cursorPos.x, getLine(this->cursorPos.y).size());
		} else if (key.keyCode == GLFW_KEY_LEFT) {
			if (this->cursorPos.x == 0) {
				uint32_t cached = this->cursorPos.y;
				this->cursorPos.y = std::max(0, this->cursorPos.y - 1);
				if (cached != this->cursorPos.y)
					this->cursorPos.x = getLine(this->cursorPos.y).size();
			} else {
				this->cursorPos.x--;
			}
		} else if (key.keyCode == GLFW_KEY_RIGHT) {
			if (this->cursorPos.x == getLine(this->cursorPos.y).size()) {
				uint32_t cached = this->cursorPos.y;
				this->cursorPos.y = std::min(this->lineCount-1, this->cursorPos.y + 1);
				if (cached != this->cursorPos.y)
					this->cursorPos.x = 0;
			} else {
				this->cursorPos.x++;
			}
		}
		return true;
	}
	return false;
}

bool TextBoxComponent::onKeyRepeated(const Key& key) {
	return onKeyPressed(key);
}

std::string TextBoxComponent::getLine(uint32_t lineno) {
	std::size_t offset = 0;
	while (offset < this->text.size() && lineno > 0) {
		offset = this->text.find_first_of('\n', offset) + 1;
		lineno--;
	}
	return this->text.substr(offset, this->text.find_first_of('\n', offset)-offset);
}

uint32_t TextBoxComponent::getOffset(const glm::ivec2& position) {
	uint32_t offset = 0;
	uint32_t lineCount = 0;
	for (auto c : this->text) {
		if (lineCount == position.y) break;
		offset++;
		lineCount += (c == '\n');
	}
	return offset + position.x;
}
