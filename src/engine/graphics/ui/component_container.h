#pragma once

#include <src/engine/graphics/ui/layout.h>

class ComponentContainer : public UIComponent {
public:
	struct iterator {
		virtual iterator& operator ++() { return *this; };
		virtual std::shared_ptr<UIComponent>& operator *() { return ComponentContainer::Empty; };
	};

	struct const_iterator {
		virtual const_iterator& operator ++() { return *this; };
		virtual const std::shared_ptr<UIComponent>& operator *() { return ComponentContainer::Empty; };
	};
	
	ComponentContainer(const std::string_view& id, Layout* layout)
		: UIComponent(id, layout) {}

	virtual iterator begin() = 0;
	virtual iterator end() = 0;

	virtual const_iterator cbegin() const = 0;
	virtual const_iterator cend() const = 0;

	virtual std::size_t getCount() = 0;

	virtual void add(std::shared_ptr<UIComponent> component) = 0;
	virtual void insert(iterator& it, std::shared_ptr<UIComponent> component) = 0;
	virtual void insert(const_iterator& it, std::shared_ptr<UIComponent> component) = 0;
	virtual void remove(iterator& it) = 0;
	virtual void remove(const_iterator& it) = 0;

	virtual void sort(std::function<bool(std::shared_ptr<UIComponent>, std::shared_ptr<UIComponent>)> compare) = 0;

	static bool AddComponent(std::shared_ptr<ComponentContainer> container, std::shared_ptr<UIComponent> component) {
		container->add(component);
		return true;
	}

	static bool InsertComponent(std::shared_ptr<ComponentContainer> container, iterator& it, std::shared_ptr<UIComponent> component) {
		container->insert(it, component);
		return true;
	}

	static bool InsertComponent(std::shared_ptr<ComponentContainer> container, const_iterator& it, std::shared_ptr<UIComponent> component) {
		container->insert(it, component);
		return true;
	}

	static bool RemoveComponent(std::shared_ptr<ComponentContainer> container, iterator& it) {
		container->remove(it);
		return true;
	}

	static bool RemoveComponent(std::shared_ptr<ComponentContainer> container, const_iterator& it) {
		container->remove(it);
		return true;
	}

	static bool SortComponents(std::shared_ptr<ComponentContainer> container, std::function<bool(std::shared_ptr<UIComponent>, std::shared_ptr<UIComponent>)> compare) {
		container->sort(compare);
		return true;
	}
private:
	static std::shared_ptr<UIComponent> Empty;
};
