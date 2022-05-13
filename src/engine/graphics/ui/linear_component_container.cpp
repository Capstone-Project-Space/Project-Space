#include "linear_component_container.h"

#include <numeric>

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>
#include <src/engine/graphics/ui/constraint_layout.h>

using iterator = ComponentContainer::iterator;
using const_iterator = ComponentContainer::const_iterator;
using linear_iterator = LinearComponentContainer::linear_iterator;
using const_linear_iterator = LinearComponentContainer::const_linear_iterator;

linear_iterator::linear_iterator(std::vector<std::shared_ptr<UIComponent>>::iterator iter) : iter(iter) {}

iterator& linear_iterator::operator ++() {
	this->iter++;
	return *this;
}

std::shared_ptr<UIComponent>& linear_iterator::operator *() {
	return *this->iter;
}

const_linear_iterator::const_linear_iterator(std::vector<std::shared_ptr<UIComponent>>::const_iterator iter) : iter(iter) {}

const_iterator& const_linear_iterator::operator ++() {
	this->iter++;
	return *this;
}

const std::shared_ptr<UIComponent>& const_linear_iterator::operator *() {
	return *(this->iter);
}

LinearComponentContainer::LinearComponentContainer(const std::string_view& id, Layout* layout)
	: ComponentContainer(id, layout) {}

LinearComponentContainer::LinearComponentContainer(const std::string_view& id, Layout* layout, std::initializer_list<std::shared_ptr<UIComponent>> contents)
	: ComponentContainer(id, layout), contents(contents) {
	int i = 0;
	for (auto component : this->contents) {
		std::string top = i == 0 ? "parent" : std::string(this->contents[i-1]->getID()) + ":bottom";
		UIComponent::ChangeLayout(component, (Layout*) new ConstraintLayout("parent:left", top));
		i++;
	}
	std::vector<std::shared_ptr<UIComponent>>::value_type;
}

float LinearComponentContainer::getContentWidth() const {
	return std::accumulate(this->contents.begin(), this->contents.end(), 0.f, [&](float s, std::shared_ptr<UIComponent> component) { return s + component->getWidth(); });
}

float LinearComponentContainer::getContentHeight() const {
	return std::accumulate(this->contents.begin(), this->contents.end(), 0.f, [&](float s, std::shared_ptr<UIComponent> component) { return s + component->getHeight(); });
}

void LinearComponentContainer::applyLayout(std::shared_ptr<Window> window, const std::unordered_map<std::string_view, std::shared_ptr<UIComponent>>& peers, bool update) {
	ComponentContainer::applyLayout(window, peers, update);
	if (update) {
		auto self = peers.at(this->id);
		std::unordered_map<std::string_view, std::shared_ptr<UIComponent>> childPeers;
		childPeers.insert({ "parent", self });

		for (auto child : this->contents) {
			childPeers.insert({ child->getID(), child });
		}

		// Apply the childrens layouts the first time.
		for (auto child : this->contents) {
			child->applyLayout(window, childPeers, update);
		}
		// Apply the parents layout the second time.
		ComponentContainer::applyLayout(window, peers, update);
		// Apply the childrens layouts the second time.
		for (auto child : this->contents) {
			child->applyLayout(window, childPeers, update);
		}
	}
}

void LinearComponentContainer::draw(std::shared_ptr<Window> window, float delta) {
	// TODO: Add some rendering stuff for the container. Stuff like background.
	// TODO: Add component culling.
	// Renderer::SubmitQuad(this->position.value() + glm::vec3{this->size.value(), 0} / 2.f, this->size.value(), AssetManager::GetOrCreate<Texture>(glm::vec4{0.f, 0.f, 0.f, 1.f}));
	for (auto component : this->contents) {
		if (component->isVisible()) {
			component->draw(window, delta);
		}
	}
}

void LinearComponentContainer::add(std::shared_ptr<UIComponent> component) {
	std::string top = (this->contents.size() == 0 ? "parent:top" : std::string(this->contents.back()->getID()) + ":bottom");
	UIComponent::ChangeLayout(component, new ConstraintLayout("parent:left", top));
	this->contents.push_back(component);
}
