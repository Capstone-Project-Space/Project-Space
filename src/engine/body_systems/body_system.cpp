#include "body_system.h"


BodySystem::BodySystem(int systemID) {
     setName("Blank");
     setStar(StarClass::K, systemID);
     setIsNebula(false);
     setBodyCount(RandomGen::RangedRandomUnSignedInt(2, 10));
     setSystemID(systemID);

     generateSystem();
}

BodySystem::BodySystem(std::string name, int starClass, bool isNebula, int bodyCount, int systemID) {
     setName(name);
     setStar((StarClass)starClass, systemID);
     setIsNebula(isNebula);
     setBodyCount(bodyCount);
     setSystemID(systemID);

     generateSystem();
}

BodySystem::~BodySystem() {
     if (!this->bodyList.empty()) {
          this->bodyList.clear();
     }
}

void BodySystem::generateSystem() {
     for (int i = 0; i < getBodyCount(); i++) {
          std::shared_ptr<Body> body = std::shared_ptr<Body>(new Body());

          body->setName(this->getName() + " " + intToNumeral(i));
          body->setBodyType((BodyType)RandomGen::RangedRandomUnSignedInt(0, 2));
          body->setHabitType((HabitType)RandomGen::RangedRandomUnSignedInt(0, 1));

          //Set ID, e.g. 0.0, 4.6, 120.3
          body->setBodyID(std::to_string(this->getSystemID()) + "." + std::to_string(i));

          //Set color, possible to be black or white
          body->setColor(glm::vec3(
               RandomGen::RangedRandomFloat(0.0f, 1.0f),
               RandomGen::RangedRandomFloat(0.0f, 1.0f),
               RandomGen::RangedRandomFloat(0.0f, 1.0f)));

          //Generate random angle and distance
          float angle = RandomGen::RangedRandomFloat(0.0f, (float)(2 * M_PI));
          float distance = getStar().star->getScale() + RandomGen::RangedRandomFloat((float)(i + 1.0f), (float)(i + 1.4f));
          
          //Calculate and set position from angle and distance
          body->setPosition(glm::vec3(
               (float)(distance * cos(angle)),
               0.0f,
               (float)(distance * sin(angle))));

          //No Moonception
          if (body->getBodyType() != BodyType::Moon) {
               //body->setMoonCount(RandomGen::RangedRandomUnSignedInt(0,0));
               //No moons for now
               body->setMoonCount(0);
          }

          this->bodyList.push_back(body);
     }
}

void BodySystem::setName(std::string name) {
     this->name = name;
}

std::string BodySystem::getName() {
     return this->name;
}

void BodySystem::setStar(StarClass starClass, unsigned int systemID) {
     this->star = Star(starClass, systemID);
}
void BodySystem::setStar(int starClass, unsigned int systemID) {
     this->star = Star(starClass, systemID);
}

Star BodySystem::getStar() {
     return this->star;
}

void BodySystem::setIsNebula(bool isNebula) {
     this->isNebula = isNebula;
}

bool BodySystem::getIsNebula() {
     return this->isNebula;
}

void BodySystem::setBodyCount(int bodyCount) {
     this->bodyCount = bodyCount;
}

int BodySystem::getBodyCount() {
     return this->bodyCount;
}

void BodySystem::setBodyList(std::vector<std::shared_ptr<Body>> bodyList) {
     if (!this->bodyList.empty()) {
          this->bodyList.clear();
     }

     for (auto i : bodyList) {
          this->bodyList.push_back(i);
     }
}

std::vector<std::shared_ptr<Body>> BodySystem::getBodyList() {
     return bodyList;
}

void BodySystem::setSystemID(int systemID) {
     this->systemID = systemID;
}

int BodySystem::getSystemID() {
     return this->systemID;
}

void BodySystem::printDebugInfo() {
     printf("\tSystem - NAME: ''%s'' ID: %s\n", this->getName(), std::to_string(this->getSystemID()));
     printf("\t\tNebula?: %s\n", this->getIsNebula() ? "true" : "false");
     printf("\t\tOribiting Bodies: %d\n", this->getBodyCount());
     printf("\t\tBodies:\n");

     for (auto i : this->getBodyList()) {
          i->printDebugInfo();
     }
}


std::string intToNumeral(int x) {
     std::string str_numerals[] = { "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I" };
     std::string return_numerals = "";
     int integers[] = { 1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1 };

     std::string result = "";
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