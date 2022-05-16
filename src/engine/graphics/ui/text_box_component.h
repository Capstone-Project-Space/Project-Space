#pragma once

#include <src/engine/graphics/font.h>
#include <src/engine/graphics/ui/layout.h>

class TextBoxComponent : public UIComponent {
public:
	TextBoxComponent(
		const std::string_view& id, Layout* layout, const std::string& defaultText, std::shared_ptr<Font> font,
		glm::vec3 color, glm::vec3 defaultColor, float scale = 1.f, Gravity gravity = Gravity::LEFT
	);
	TextBoxComponent(
		const std::string_view& id, Layout* layout, const std::string& defaultText, std::shared_ptr<Font> font,
		glm::vec3 color, glm::vec3 defaultColor, glm::vec2 dimensions, float scale = 1.f, Gravity gravity = Gravity::LEFT
	);

	virtual float getContentWidth() const override;
	virtual float getContentHeight() const override;

	virtual const glm::vec3& getPosition() const override;

	virtual void draw(std::shared_ptr<Window> window, float delta) override;

	virtual bool onMouseButtonPressed(const MouseButton& buttno) override;
	virtual bool onKeyChar(const char* text) override;
	virtual bool onKeyPressed(const Key& key) override;
	virtual bool onKeyRepeated(const Key& key) override;

private:
	bool focused = false;
	float cursorTimer = 0.f;
	int32_t lineCount = 1;
	glm::ivec2 cursorPos{0u};
	std::string text, defaultText;
	std::shared_ptr<Font> font;
	float scale;
	Gravity gravity;
	glm::vec3 color, defaultColor;
	glm::vec2 dimensions;

	std::string getLine(uint32_t lineno);
	uint32_t getOffset(const glm::ivec2& position);
};
