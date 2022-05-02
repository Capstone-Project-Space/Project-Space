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
	ButtonComponent(
		const std::string_view& id, Layout* layout, std::function<void(ButtonComponent&)> clickCallback,
		const std::string& text, std::shared_ptr<Font> font,
		const Color& textColor = Color{ glm::vec4{1.0f} }, const Color& textHighlightColor = Color{ glm::vec4{ 0.6f } },
		Gravity gravity = Gravity::CENTER, const glm::vec2& minimumDimensions = { 0.0f, 0.0f }
	);

	ButtonComponent(
		const std::string_view& id, Layout* layout, std::function<void(ButtonComponent&)> clickCallback,
		std::shared_ptr<Texture> texture, const Color& tint = Color{ glm::vec4{ 1.0f } },
		const glm::vec2& minimumDimensions = { 0.0f, 0.0f }
	);

	ButtonComponent(
		const std::string_view& id, Layout* layout, std::function<void(ButtonComponent&)> clickCallback,
		const std::string& text, std::shared_ptr<Font>, std::shared_ptr<Texture> texture,
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
	std::function<void(ButtonComponent&)> clickCallback;
	glm::vec2 minimumDimensions;

	std::string text;
	glm::vec4 textColor, textHighlightColor;
	std::shared_ptr<Font> textFont;
	Gravity textGravity;

	std::shared_ptr<Texture> texture;
	glm::vec4 textureHighlightTint;
};
