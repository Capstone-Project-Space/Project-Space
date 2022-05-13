#pragma once

#include <algorithm>
#include <vector>

#include <src/engine/graphics/ui/component_container.h>

class LinearComponentContainer : public ComponentContainer {
public:
	class linear_iterator : public ComponentContainer::iterator {
	public:
		linear_iterator(std::vector<std::shared_ptr<UIComponent>>::iterator iter);

		virtual iterator& operator ++() override;
		virtual std::shared_ptr<UIComponent>& operator *() override;

		virtual std::vector<std::shared_ptr<UIComponent>>::iterator& iterator() { return this->iter; };

	private:
		std::vector<std::shared_ptr<UIComponent>>::iterator iter;
	};
	class const_linear_iterator : public ComponentContainer::const_iterator {
	public:
		const_linear_iterator(std::vector<std::shared_ptr<UIComponent>>::const_iterator iter);

		virtual const_iterator& operator ++() override;
		virtual const std::shared_ptr<UIComponent>& operator *() override;

		virtual std::vector<std::shared_ptr<UIComponent>>::const_iterator& iterator() { return this->iter; };

	private:
		std::vector<std::shared_ptr<UIComponent>>::const_iterator iter;
	};

	// TODO: Add some way to determine linear direction.
	LinearComponentContainer(const std::string_view& id, Layout* layout);
	LinearComponentContainer(const std::string_view& id, Layout* layout, std::initializer_list<std::shared_ptr<UIComponent>> contents);

	virtual iterator begin() override { return linear_iterator(this->contents.begin()); };
	virtual iterator end() override { return linear_iterator(this->contents.end()); };

	virtual const_iterator cbegin() const override { return const_linear_iterator(this->contents.cbegin()); };
	virtual const_iterator cend() const override { return const_linear_iterator(this->contents.cend()); };

	virtual std::size_t getCount() override { return this->contents.size(); };

	virtual float getContentWidth() const override;
	virtual float getContentHeight() const override;

	virtual void applyLayout(std::shared_ptr<Window> window, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers, bool update = false) override;

	virtual void draw(std::shared_ptr<Window> window, float delta) override;

	virtual void add(std::shared_ptr<UIComponent> component) override;
	virtual void insert(iterator& it, std::shared_ptr<UIComponent> component) override {
		if (dynamic_cast<linear_iterator*>(&it))
		this->contents.insert(dynamic_cast<linear_iterator&>(it).iterator(), component);
	};
	virtual void insert(const_iterator& it, std::shared_ptr<UIComponent> component) override {
		if (dynamic_cast<const_linear_iterator*>(&it)) {
			this->contents.insert(dynamic_cast<const_linear_iterator&>(it).iterator(), component);
		}
	};
	virtual void remove(iterator& it) override {
		if (dynamic_cast<linear_iterator*>(&it)) {
			this->contents.erase(dynamic_cast<linear_iterator&>(it).iterator());
		}
	};
	virtual void remove(const_iterator& it) override {
		if (dynamic_cast<const_linear_iterator*>(&it)) {
			this->contents.erase(dynamic_cast<const_linear_iterator&>(it).iterator());
		}
	};

	virtual inline void sort(std::function<bool(std::shared_ptr<UIComponent>, std::shared_ptr<UIComponent>)> compare) override { /*std::sort(, this->end(), compare);*/ };

private:
	std::vector<std::shared_ptr<UIComponent>> contents;
	
};
