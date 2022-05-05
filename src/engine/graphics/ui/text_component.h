#pragma once

#include <src/engine/graphics/font.h>
#include <src/engine/graphics/ui/layout.h>

class TextComponent : public UIComponent {
public:
	/**
	 * @brief Construct a new Text Component object.
	 * 
	 * @param id The id of this UIComponent.
	 * @param layout The layout which determines the position and size of the UIComponent.
	 * @param text The text to be displayed.
	 * @param font The font to display the text with.
	 */
	TextComponent(const std::string_view& id, Layout* layout, const std::string& text, std::shared_ptr<Font> font);

	operator UIComponent*() { return this; }

	virtual float getContentWidth() const override;
	virtual float getContentHeight() const override;

	virtual void draw(float delta) override;

	inline const std::string& getText() const { return text; }

	/** Functions to be used for modifying the TextComponent. **/
	static bool SetText(std::shared_ptr<TextComponent> component, const std::string& text);
	static bool SetFont(std::shared_ptr<TextComponent> component, std::shared_ptr<Font> font);
	static bool SetScale(std::shared_ptr<TextComponent> component, float scale);
	static bool SetColor(std::shared_ptr<TextComponent> component, glm::vec3 color);
	static bool SetColor(std::shared_ptr<TextComponent> component, glm::vec4 color);
	static bool SetTextGravity(std::shared_ptr<TextComponent> component, Gravity gravity);
	static bool SetLayoutGravity(std::shared_ptr<TextComponent> component, Gravity gravity);

protected:
	std::string text;
	std::shared_ptr<Font> font;
	float scale = 1.0f;
	glm::vec4 color{1.0f};
	Gravity textGravity = Gravity::LEFT;
	Gravity layoutGravity = Gravity::LEFT;
};