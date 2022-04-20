#include "body.h"

//Used for generating a system
Body::Body() {
     setName("");
     setBodyType(BodyType::Planet);
     setHabitType(HabitType::Habitable);
     setPosition(glm::vec3(0.0f));
     setScale(1.0f);
     setColor(glm::vec3(0.5f));
     setMoonCount(0);
}

//Used when reading save game data
Body::Body(std::string name, int bodyType, int habitType, glm::vec3 position, float scale, glm::vec3 color, int moonCount) {
     setName(name);
     setBodyType((BodyType)bodyType);
     setHabitType((HabitType)habitType);
     setPosition(position);
     setScale(scale);
     setColor(color);
     setMoonCount(moonCount);
}

Body::~Body() {
     if (this->moonCount > 0) {
          this->moonList.clear();
     }
}

void Body::setName(std::string name) {
     this->name = name;
}
std::string Body::getName() {
     return this->name;
}

void Body::setBodyType(BodyType bodyType) {
     this->bodyType = bodyType;
}
BodyType Body::getBodyType() {
     return this->bodyType;
}

void Body::setHabitType(HabitType habitType) {
     this->habitType = habitType;
}
HabitType Body::getHabitType() {
     return this->habitType;
}

void Body::setPosition(glm::vec3 position) {
     this->position = position;
}
glm::vec3 Body::getPosition() {
     return this->position;
}

void Body::setScale(float scale) {
     this->scale = scale;
}
float Body::getScale() {
     return this->scale;
}

void Body::setColor(glm::vec3 color) {
     this->color = color;
}
glm::vec3 Body::getColor() {
     return this->color;
}

void Body::setMoonCount(int moonCount) {
     this->moonCount = moonCount;
}
int Body::getMoonCount() {
     return this->moonCount;
}

void Body::setMoonList(std::vector<std::shared_ptr<Body>> moonList) {
     if (!this->moonList.empty()) {
          this->moonList.clear();
     }

     for (auto i : moonList) {
          this->moonList.push_back(i);
     }
}

std::vector<std::shared_ptr<Body>> Body::getMoonList() {
     return moonList;
}

void Body::setBodyID(std::string bodyID) {
     this->bodyID = bodyID;
}

std::string Body::getBodyID() {
     return bodyID;
}

void Body::printDebugInfo() {
     printf("\t\t\tBody - NAME: ''%s'' ID: %s\n", (const std::string)this->getName(), (const std::string)this->getBodyID());
     printf("\t\t\t\tBodyType: %d\n", this->getBodyType());
     printf("\t\t\t\tHabitable: %d\n", this->getHabitType());
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