#include "body.h"

//Used for generating a system
Body::Body() {
     setName("");
     setBodyType(BodyType::Planet);
     setHabitType(HabitType::Habitable);
     setPosition(glm::vec3(0.0f));
     setScale(0.3f);
     setColor(glm::vec3(0.5f));
     setMoonCount(0);
}

//Used when reading save game data
Body::Body(const std::string& name, int bodyType, int habitType, const glm::vec3& position, float scale, const glm::vec3& color, int moonCount) {
     setName(name);
     setBodyType((BodyType)bodyType);
     setHabitType((HabitType)habitType);
     setPosition(position);
     setScale(scale);
     setColor(color);
     setMoonCount(moonCount);
}

void Body::setName(const std::string& name) {
     this->name = name;
}
const std::string& Body::getName() const {
     return this->name;
}

void Body::setBodyType(BodyType bodyType) {
     this->bodyType = bodyType;
}
BodyType Body::getBodyType() const {
     return this->bodyType;
}

void Body::setHabitType(const HabitType habitType) {
     this->habitType = habitType;
}
HabitType Body::getHabitType() const {
     return this->habitType;
}

void Body::setPosition(const glm::vec3& position) {
     this->position = position;
}
const glm::vec3& Body::getPosition() const {
     return this->position;
}

void Body::setScale(const float scale) {
     this->scale = scale;
}
float Body::getScale() const {
     return this->scale;
}

void Body::setColor(const glm::vec3& color) {
     this->color = color;
}
const glm::vec3& Body::getColor() const {
     return this->color;
}

void Body::setMoonCount(const int moonCount) {
     this->moonCount = moonCount;
}
int Body::getMoonCount() const {
     return this->moonCount;
}

void Body::setMoonList(const std::vector<std::shared_ptr<Body>>& moonList) {
     if (!this->moonList.empty()) {
          this->moonList.clear();
     }

     for (auto i : moonList) {
          this->moonList.push_back(i);
     }
}

const std::vector<std::shared_ptr<Body>>& Body::getMoonList() const {
     return moonList;
}

void Body::setBodyID(const std::string& bodyID) {
     this->bodyID = bodyID;
}

const std::string& Body::getBodyID() const {
     return bodyID;
}

void Body::printDebugInfo() const {
     printf("\t\t\tBody - NAME: ''%s'' ID: %s\n", this->getName().c_str(), this->getBodyID().c_str());
     printf("\t\t\t\tBodyType: %d\n", (int) this->getBodyType());
     printf("\t\t\t\tHabitable: %d\n", (int)this->getHabitType());
     printf("\t\t\t\tPosition:  %f   %f   %f\n", this->getPosition().x,
                                                 this->getPosition().y,
                                                 this->getPosition().z);
     printf("\t\t\t\tScale:  %f\n", this->getScale());
     printf("\t\t\t\tMoonCount: %d\n", this->getMoonCount());

     if (this->getMoonCount() > 0) {
          printf("\t\t\t\tMoons:\n");
          printf("\t\t\t\tThere are no moons here.\n");
     }
}