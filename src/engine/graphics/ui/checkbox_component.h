#pragma once

#include <src/engine/graphics/ui/layout.h>

class CheckboxComponent : public UIComponent {
public:
	CheckboxComponent(const std::string_view& id, Layout* layout, bool checked = false, Gravity gravity = Gravity::CENTER);
	CheckboxComponent(const std::string_view& id, Layout* layout, const glm::vec2 contentSize, bool checked = false, Gravity gravity = Gravity::CENTER);

	virtual float getContentWidth() const override;
	virtual float getContentHeight() const override;

	virtual const glm::vec3& getPosition() const override;

	virtual void draw(std::shared_ptr<Window> window, float delta) override;

	virtual bool onMouseButtonReleased(const MouseButton& button) override;

	inline bool getChecked() const { return this->checked; }

	static bool SetChecked(std::shared_ptr<CheckboxComponent> component, bool checked);

protected:
	bool checked;
	Gravity gravity;
	glm::vec2 contentSize;
};
