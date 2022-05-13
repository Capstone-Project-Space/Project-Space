#include "body_system.h"


BodySystem::BodySystem(int systemID) {
     setName("Blank");
     setStar(StarClass::K, systemID);
     setIsNebula(false);
     setBodyCount(RandomGen::RangedRandomUnSignedInt(2, 10));
     setSystemID(systemID);

     generateSystem();
}

BodySystem::BodySystem(const std::string& name, StarClass starClass, bool isNebula, int bodyCount, int systemID) {
     setName(name);
     setStar(starClass, systemID);
     setIsNebula(isNebula);
     setBodyCount(bodyCount);
     setSystemID(systemID);

     generateSystem();
}

void BodySystem::generateSystem() {
     for (int i = 0; i < getBodyCount(); i++) {
          std::shared_ptr<Body> body = std::shared_ptr<Body>(new Body());

          body->setName(this->getName() + " " + intToNumeral(i + 1));
          body->setBodyType((BodyType)RandomGen::RangedRandomUnSignedInt(0, BODY_TYPE_COUNT));
          body->setHabitType((HabitType)RandomGen::RangedRandomUnSignedInt(0, 1));

          switch (body->getBodyType()) {
          case BodyType::EARTH_PLANET: case BodyType::DEAD_PLANET: case BodyType::ROCKY_PLANET: case BodyType::WATER:
               body->setScale(RandomGen::RangedRandomFloat(0.06f, 0.2f));
               break;
          case BodyType::MOON:
               body->setScale(RandomGen::RangedRandomFloat(0.01f, 0.06f));
               break;
          case BodyType::GREEN_GAS_GIANT: case BodyType::BROWN_GAS_GIANT:
               body->setScale(RandomGen::RangedRandomFloat(0.2f, 0.5f));
               break;
          default:
               //body->setScale(RandomGen::RangedRandomFloat(0.4f, 0.8f));
               break;
          }

          //Set ID, e.g. 0.0, 4.6, 120.3
          body->setBodyID(std::to_string(this->getSystemID()) + "." + std::to_string(i));

          //Set color, possible to be black or white
          body->setColor(glm::vec3(
               RandomGen::RangedRandomFloat(0.0f, 1.0f),
               RandomGen::RangedRandomFloat(0.0f, 1.0f),
               RandomGen::RangedRandomFloat(0.0f, 1.0f)));

          //Generate random angle and distance
          float angle = RandomGen::RangedRandomFloat(0.0f, (float)(2 * M_PI));
          float distance = getStar().star->getScale() + RandomGen::RangedRandomFloat((float)((i+1) * 1.5f), (float)((i+1) * 2.0f));
          
          //Calculate and set position from angle and distance
          body->setPosition(glm::vec3(
               (float)(distance * cos(angle)),
               0.0f,
               (float)(distance * sin(angle))));

          //No Moonception
          if (body->getBodyType() != BodyType::MOON) {
               //body->setMoonCount(RandomGen::RangedRandomUnSignedInt(0,0));
               //No moons for now
               body->setMoonCount(0);
          }

          this->bodyList.push_back(body);
     }
}

void BodySystem::setName(const std::string& name) {
     this->name = name;
}

const std::string& BodySystem::getName() const {
     return this->name;
}

void BodySystem::setStar(StarClass starClass, unsigned int systemID) {
     this->star = Star(starClass, systemID);
}

Star BodySystem::getStar() const {
     return this->star;
}

void BodySystem::setIsNebula(bool isNebula) {
     this->isNebula = isNebula;
}

bool BodySystem::getIsNebula() const {
     return this->isNebula;
}

void BodySystem::setBodyCount(int bodyCount) {
     this->bodyCount = bodyCount;
}

int BodySystem::getBodyCount() const {
     return this->bodyCount;
}

void BodySystem::setBodyList(const std::vector<std::shared_ptr<Body>>& bodyList) {
     if (!this->bodyList.empty()) {
          this->bodyList.clear();
     }

     for (auto i : bodyList) {
          this->bodyList.push_back(i);
     }
}

const std::vector<std::shared_ptr<Body>>& BodySystem::getBodyList() const {
     return bodyList;
}

void BodySystem::setSystemID(int systemID) {
     this->systemID = systemID;
}

int BodySystem::getSystemID() const {
     return this->systemID;
}

void BodySystem::printDebugInfo() const {
     printf("\tSystem - NAME: ''%s'' ID: %s\n", this->name.c_str(), std::to_string(this->systemID).c_str());
     printf("\t\tNebula?: %s\n", this->isNebula ? "true" : "false");
     printf("\t\tOribiting Bodies: %d\n", this->getBodyCount());
     printf("\t\tBodies:\n");

     for (auto i : this->getBodyList()) {
          if (i) i->printDebugInfo();
     }
}


std::string intToNumeral(int x) {
     std::string str_numerals[] = { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I" };
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

char* StrToChar(std::string str) {
     char* t = new char[str.length() + 1];
     for (int i = 0; i < str.length(); i++) {
          t[i] = str[i];
     }
     t[str.length() + 1] = '\0';
     return t;
}