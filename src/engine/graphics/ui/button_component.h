#pragma once
#include <functional>
#include <optional>
#include <string_view>
#include <variant>

#include <src/engine/graphics/font.h>
#include <src/engine/graphics/texture.h>
#include <src/engine/graphics/ui/layout.h>

using Color = std::variant<glm::vec3, glm::vec4>;

class ButtonComponent : public UIComponent {
public:
	/**
	 * @brief Construct a new Button Component object with only text.
	 * 
	 * @param id The id of this UIComponent.
	 * @param layout The layout which determines the position and size of the UIComponent.
	 * @param clickCallback A callback for when the button is clicked.
	 * @param text The text displayed on the button.
	 * @param font The font to display the text with.
	 * @param textColor The color of the text.
	 * @param textHighlightColor The color of the text when the mouse is hovering over it.
	 * @param gravity The gravity of the button.
	 * @param minimumDimensions The minimum dimensions returned from getContentWidth and getContentHeight.
	 */
	ButtonComponent(
		const std::string_view& id, Layout* layout, std::function<bool(ButtonComponent&)> clickCallback,
		const std::string& text, std::shared_ptr<Font> font,
		const Color& textColor = Color{ glm::vec4{1.0f} }, const Color& textHighlightColor = Color{ glm::vec4{ 0.6f } },
		Gravity gravity = Gravity::CENTER, const glm::vec2& minimumDimensions = { 0.0f, 0.0f }
	);

	/**
	 * @brief Construct a new Button Component object with only a texture.
	 * 
	 * @param id The id of this UIComponent.
	 * @param layout The layout which determines the position and size of the UIComponent.
	 * @param clickCallback A callback for when the button is clicked.
	 * @param texture The texture to render for the button.
	 * @param tintColor The tint color to apply over the texture when the mouse is hovering over it.
	 * @param minimumDimensions The minimum dimensions returned from getContentWidth and getContentHeight.
	 */
	ButtonComponent(
		const std::string_view& id, Layout* layout, std::function<bool(ButtonComponent&)> clickCallback,
		std::shared_ptr<Texture> texture, const Color& tintColor = Color{ glm::vec4{ 1.0f } },
		Gravity gravity = Gravity::CENTER, const glm::vec2& minimumDimensions = { 0.0f, 0.0f }
	);

	/**
	 * @brief Construct a new Button Component object with both text and texture.
	 * 
	 * @param id The id of this UIComponent.
	 * @param layout The layout which determines the position and size of the UIComponent.
	 * @param clickCallback A callback for when the button is clicked.
	 * @param text The text displayed on the button.
	 * @param font The font to display the text with.
	 * @param texture The texture to render for the button.
	 * @param textColor The color of the text.
	 * @param textHighlightColor The color of the text when the mouse is hovering over it.
	 * @param tintColor The tint color to apply over the texture when the mouse is hovering over it.
	 * @param gravity The gravity of the button.
	 * @param minimumDimensions The minimum dimensions returned from getContentWidth and getContentHeight.
	 */
	ButtonComponent(
		const std::string_view& id, Layout* layout, std::function<bool(ButtonComponent&)> clickCallback,
		const std::string& text, std::shared_ptr<Font> font, std::shared_ptr<Texture> texture,
		const Color& textColor = Color{ glm::vec4{ 1.0f } }, const Color& textHighlightColor = Color{ glm::vec4{.6f} },
		const Color& tintColor = Color{ glm::vec4{ 1.0f } },
		Gravity gravity = Gravity::CENTER, const glm::vec2& minimumDimensions = { 0.0f, 0.0f }
	);

	virtual float getContentWidth() const override;
	virtual float getContentHeight() const override;

	virtual void draw(float delta) override;

	virtual bool isWithin(const glm::vec2& position) const override;

	virtual bool onMouseButtonReleased(const MouseButton& button) override;

protected:
	std::function<bool(ButtonComponent&)> clickCallback;
	glm::vec2 minimumDimensions;
	Gravity gravity;

	std::string text;
	glm::vec4 textColor, textHighlightColor;
	std::shared_ptr<Font> textFont;

	std::shared_ptr<Texture> texture;
	glm::vec4 textureHighlightTint;
};
