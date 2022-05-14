#include "body.h"

#include <src/engine/graphics/asset_manager.h>
#include <src/engine/graphics/renderer.h>
#include <src/engine/randgen/randomgen.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

HabitType GetRandomHabitFromBodyType(BodyType type) {
	switch (type) {
		case BodyType::ROCKY_PLANET: return RandomGen::RangedRandomDouble(0, 1.) > .8 ? HabitType::HABITABLE : HabitType::UNHABITABLE;
		case BodyType::DEAD_PLANET: return RandomGen::RangedRandomDouble(0, 1.) > .96 ? HabitType::HABITABLE : HabitType::UNHABITABLE;
		case BodyType::EARTH_PLANET: return RandomGen::RangedRandomDouble(0, 1.) > .04 ? HabitType::HABITABLE : HabitType::UNHABITABLE;
		case BodyType::GREEN_GAS_GIANT: case BodyType::BROWN_GAS_GIANT: return HabitType::UNHABITABLE;
		case BodyType::WATER: return RandomGen::RangedRandomDouble(0., 1.) > .2 ? HabitType::HABITABLE : HabitType::UNHABITABLE;
		case BodyType::MOON: case BodyType::SUN: return HabitType::UNHABITABLE;
	}
}

float GetRandomScaleFromBodyType(BodyType type) {
	switch (type) {
	case BodyType::ROCKY_PLANET: case BodyType::DEAD_PLANET: case BodyType::EARTH_PLANET: case BodyType::WATER:
		return RandomGen::RangedRandomFloat(0.06f, 0.2f);
	case BodyType::GREEN_GAS_GIANT: case BodyType::BROWN_GAS_GIANT:
		return RandomGen::RangedRandomFloat(0.2f, 0.5f);
	case BodyType::MOON: 
		return RandomGen::RangedRandomFloat(0.01f, 0.06f);
	}
}

float GetStarRadiusFromClass(StarClass cl) {
	switch (cl) {
		case StarClass::A: return 2.5f;
		case StarClass::F: return 2.0f;
		case StarClass::G: return 1.5f;
		case StarClass::K: return 1.0f;
		case StarClass::M: return 0.5f;
	}
}

const glm::vec3& GetStarColorFromClass(StarClass cl) {
	switch (cl) {
		case StarClass::A: return { 0.0f, 0.7f, 0.8f };
		case StarClass::F: return { 0.5f, 0.4f, 0.8f };
		case StarClass::G: return { 0.9f, 0.8f, 0.0f };
		case StarClass::K: return { 0.9f, 0.6f, 0.0f };
		case StarClass::M: return { 0.9f, 0.1f, 0.1f };
	}
}

Body Body::CreateSingleStarSystem(const std::string& name) {
	StarClass starClass = (StarClass) RandomGen::RangedRandomUnSignedInt(0, STAR_CLASS_COUNT - 1);
	ChildrenConstraints constraint = {
		4, 12,
		5.f,
		{ BodyType::ROCKY_PLANET, BodyType::DEAD_PLANET, BodyType::EARTH_PLANET, BodyType::GREEN_GAS_GIANT, BodyType::BROWN_GAS_GIANT, BodyType::WATER }
	};
	Body star = Body(name, BodyType::SUN, HabitType::UNHABITABLE, glm::vec3{ 0.f }, GetStarRadiusFromClass(starClass), GetStarColorFromClass(starClass), constraint);

	// TODO: Generate moons.
	for (auto& child : star.children) {
		int moonCount = RandomGen::RangedRandomUnSignedInt(0, 4);
		for (int i = 0; i < moonCount; i++) {
			float distance = child.scale + RandomGen::RangedRandomFloat(.06, .1);
			float angle = RandomGen::RangedRandomFloat(0.f, 2 * glm::pi<float>());
			glm::vec3 position = glm::rotate(glm::identity<glm::mat4>(), angle, glm::vec3{ 0.f, 1.f, 0.f }) * glm::vec4{ 1.f, 0.f, 1.f, 1.f } * distance;
			child.children.emplace_back(
				child.getName() + " Moon " + intToNumeral(i),
				BodyType::MOON, GetRandomHabitFromBodyType(BodyType::MOON), 
				position + child.position, GetRandomScaleFromBodyType(BodyType::MOON), 
				glm::vec3{1.f} 
			);
		}
	}
	return star;
}

Body::Body(const std::string& name, BodyType bodyType, HabitType habitType, const glm::vec3& position, float scale, const glm::vec3& color, std::optional<ChildrenConstraints> childConstraints)
	: name(name), bodyType(bodyType), habitType(habitType), position(position), lastPosition(position), scale(scale), color(color, 1.f) {
	if (childConstraints) {
		ChildrenConstraints constraint = *childConstraints;
		uint32_t children = RandomGen::RangedRandomUnSignedInt(constraint.minChilds, constraint.maxChilds);
		this->children.reserve(children);
		for (uint32_t i = 0; i < children; i++) {
			BodyType bodyType = *std::next(constraint.allowedBodyTypes.begin(), RandomGen::RangedRandomUnSignedInt(0, constraint.allowedBodyTypes.size() - 1));
			
			float distance = scale + RandomGen::RangedRandomFloat(.1f * constraint.distance, constraint.distance);
			float angle = RandomGen::RangedRandomFloat(0.f, 2 * glm::pi<float>());
			glm::vec3 position = glm::rotate(glm::identity<glm::mat4>(), angle, glm::vec3{ 0.f, 1.f, 0.f }) * glm::vec4{1.f, 0.f, 1.f, 1.f} * distance;
			float scale = GetRandomScaleFromBodyType(bodyType);
			this->children.emplace_back(name + intToNumeral(i), bodyType, GetRandomHabitFromBodyType(bodyType), position + this->position, scale, glm::vec3{1.f});
		}
	}
}

void Body::update(float delta) {
	for (auto& child : this->children) {
		child.position += this->position - this->lastPosition;
		float distance = glm::length(child.position - this->position);
		glm::vec3 direction = glm::normalize(glm::cross(glm::vec3{0.f,1.f,0.f}, glm::normalize(child.position - this->position)));
		child.lastPosition = child.position;
		child.position += direction * ((1.f / distance) * 2.f) * delta;
		child.update(delta);
	}
}

void Body::draw(float delta, bool children) const {
	glm::mat4 transform = glm::scale(glm::translate(glm::identity<glm::mat4>(), this->getPosition()), glm::vec3{ this->getScale() });
	glm::vec4 color = this->getColor();
	Material material;
	switch (bodyType) {
		case BodyType::SUN:
			Renderer::SubmitLightSource({ glm::vec4{this->getPosition(),1.f} });
			material = {
				glm::vec3{1.f}, glm::vec3{0.f}, glm::vec3{0.f}, 1.f, 1.f,
				AssetManager::GetOrCreate<Texture>("./resources/textures/planets/star2.png")
			};
			color = glm::vec4{ 1.f };
			break;
		case BodyType::ROCKY_PLANET:
			material = {
				glm::vec3{.6f}, glm::vec3{.8f}, glm::vec3{.3f}, 90.f, 1.f,
				AssetManager::GetOrCreate<Texture>("./resources/textures/planets/rocky.png")
			};
		break;
		case BodyType::DEAD_PLANET:
			material = {
				glm::vec3{.6f}, glm::vec3{.8f}, glm::vec3{.3f}, 90.f, 1.f,
				AssetManager::GetOrCreate<Texture>("./resources/textures/planets/dead_earth.png")
			};
			break;
		case BodyType::EARTH_PLANET:
			material = {
				glm::vec3{.6f}, glm::vec3{.8f}, glm::vec3{.3f}, 90.f, 1.f,
				AssetManager::GetOrCreate<Texture>("./resources/textures/planets/alive_earth.png")
			};
			break;
		case BodyType::GREEN_GAS_GIANT:
			material = {
				glm::vec3{.6f}, glm::vec3{.8f}, glm::vec3{.3f}, 90.f, 1.f,
				AssetManager::GetOrCreate<Texture>("./resources/textures/planets/green_gas.png")
			};
			break;
		case BodyType::BROWN_GAS_GIANT:
			material = {
				glm::vec3{.6f}, glm::vec3{.8f}, glm::vec3{.3f}, 90.f, 1.f,
				AssetManager::GetOrCreate<Texture>("./resources/textures/planets/brown_gas.png")
			};
			break;
		case BodyType::WATER:
			material = {
				glm::vec3{.6f}, glm::vec3{.8f}, glm::vec3{.3f}, 90.f, 1.f,
				AssetManager::GetOrCreate<Texture>("./resources/textures/planets/alive_water.png")
			};
			break;
		case BodyType::MOON:
			material = {
				glm::vec3{.6f}, glm::vec3{.8f}, glm::vec3{.3f}, 90.f, 1.f,
				AssetManager::GetOrCreate<Texture>("./resources/textures/planets/moon.png")
			};
			break;
	}
	Renderer::SubmitModel(AssetManager::GetOrCreate<Model>("./resources/models/planet.obj", material), transform, color);
	if (children) {
		for (auto& child : this->children) {
			child.draw(delta, true);
		}
	}
}

std::string intToNumeral(uint32_t x) {
	constexpr std::string_view str_numerals[] = { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I" };
	std::string return_numerals = "";
	int integers[] = { 1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1 };

	int i = 0;
	while (x > 0) {
		int d = x / integers[i];
		x = x % integers[i];
		while (d--) {
			return_numerals += str_numerals[i];
		}
		i++;
	}
	return return_numerals;
}

//Used for generating a system
//Body::Body() {
//     setName("");
//     setBodyType(BodyType::ROCKY_PLANET);
//     setHabitType(HabitType::Habitable);
//     setPosition(glm::vec3(0.0f));
//     setScale(0.3f);
//     setColor(glm::vec3(0.5f));
//     setMoonCount(0);
//}
//
////Used when reading save game data
//Body::Body(const std::string& name, int bodyType, int habitType, const glm::vec3& position, float scale, const glm::vec3& color, int moonCount) {
//     setName(name);
//     setBodyType((BodyType)bodyType);
//     setHabitType((HabitType)habitType);
//     setPosition(position);
//     setScale(scale);
//     setColor(color);
//     setMoonCount(moonCount);
//}
//
//void Body::setName(const std::string& name) {
//     this->name = name;
//}
//const std::string& Body::getName() const {
//     return this->name;
//}
//
//void Body::setBodyType(BodyType bodyType) {
//     this->bodyType = bodyType;
//}
//BodyType Body::getBodyType() const {
//     return this->bodyType;
//}
//
//void Body::setHabitType(const HabitType habitType) {
//     this->habitType = habitType;
//}
//HabitType Body::getHabitType() const {
//     return this->habitType;
//}
//
//void Body::setPosition(const glm::vec3& position) {
//     this->position = position;
//}
//const glm::vec3& Body::getPosition() const {
//     return this->position;
//}
//
//void Body::setScale(const float scale) {
//     this->scale = scale;
//}
//float Body::getScale() const {
//     return this->scale;
//}
//
//void Body::setColor(const glm::vec3& color) {
//     this->color = color;
//}
//const glm::vec3& Body::getColor() const {
//     return this->color;
//}
//
//void Body::setMoonCount(const int moonCount) {
//     this->moonCount = moonCount;
//}
//int Body::getMoonCount() const {
//     return this->moonCount;
//}
//
//void Body::setMoonList(const std::vector<std::shared_ptr<Body>>& moonList) {
//     if (!this->moonList.empty()) {
//          this->moonList.clear();
//     }
//
//     for (auto i : moonList) {
//          this->moonList.push_back(i);
//     }
//}
//
//const std::vector<std::shared_ptr<Body>>& Body::getMoonList() const {
//     return moonList;
//}
//
//void Body::setBodyID(const std::string& bodyID) {
//     this->bodyID = bodyID;
//}
//
//const std::string& Body::getBodyID() const {
//     return bodyID;
//}
//
//void Body::printDebugInfo() const {
//     printf("\t\t\tBody - NAME: ''%s'' ID: %s\n", this->getName().c_str(), this->getBodyID().c_str());
//     printf("\t\t\t\tBodyType: %d\n", (int) this->getBodyType());
//     printf("\t\t\t\tHabitable: %d\n", (int)this->getHabitType());
//     printf("\t\t\t\tPosition:  %f   %f   %f\n", this->getPosition().x,
//                                                 this->getPosition().y,
//                                                 this->getPosition().z);
//     printf("\t\t\t\tScale:  %f\n", this->getScale());
//     printf("\t\t\t\tMoonCount: %d\n", this->getMoonCount());
//
//     if (this->getMoonCount() > 0) {
//          printf("\t\t\t\tMoons:\n");
//          printf("\t\t\t\tThere are no moons here.\n");
//     }
//}