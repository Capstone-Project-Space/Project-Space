#pragma once

#include <src/engine/graphics/font.h>
#include <src/engine/graphics/ui/layout.h>

class TextComponent : public UIComponent {
public:
	TextComponent(const std::string_view& id, Layout* layout, const std::string& text, std::shared_ptr<Font> font);

	operator UIComponent*() { return static_cast<UIComponent*>(this); }

	virtual float getContentWidth() const override;
	virtual float getContentHeight() const override;

	virtual void draw(float delta) override;

	inline const std::string& getText() const { return text; }
	static bool SetText(std::shared_ptr<TextComponent> component, const std::string& text);

private:
	std::string text;
	std::shared_ptr<Font> font;
};