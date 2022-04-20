#pragma once

#include <vector>

#include "body.h"
#include "../randgen/randomgen.h"

//Star Classifications
enum class StarClass {A, F, G, K, M};
//A-Class: Radius = 2.5       Color = Cyan
//F-Class: Radius = 2.0       Color = Pale Purple
//G-Class: Radius = 1.5       Color = Pale Yellow
//K-Class: Radius = 1.0       Color = Orange
//M-Class: Radius = 0.5       Color = Red

//Star Struct
struct Star {
     StarClass starClass;
     std::shared_ptr<Body> star;
     std::string starID;

     Star() {
          this->starClass = StarClass::M;
          this->starID = "0.0";

          this->star = std::shared_ptr<Body>(new Body());
          this->star->setPosition(glm::vec3(0.0f));

          this->star->setColor({0.65f, 0.1f, 0.1f});
          this->star->setScale(0.5f + RandomGen::RangedRandomFloat(0.0f, 0.49f));
     }

     Star(StarClass starClass, unsigned int systemID) {
          this->starClass = starClass;
          this->starID = std::to_string(systemID) + "I";

          this->star = std::shared_ptr<Body>(new Body());
          this->star->setPosition(glm::vec3(0.0f));

          switch (starClass) {
          case StarClass::A:
               this->star->setColor({ 0.0f, 0.7f, 0.8f });
               this->star->setScale(2.5f + RandomGen::RangedRandomFloat(-0.44f, 0.44f));
               break;
          case StarClass::F:
               this->star->setColor({ 0.5f, 0.4f, 0.8f });
               this->star->setScale(2.0f + RandomGen::RangedRandomFloat(-0.44f, 0.44f));
               break;
          case StarClass::G:
               this->star->setColor({ 0.9f, 0.8f, 0.0f });
               this->star->setScale(1.5f + RandomGen::RangedRandomFloat(-0.44f, 0.44f));
               break;
          case StarClass::K:
               this->star->setColor({ 0.9f, 0.6f, 0.0f });
               this->star->setScale(1.0f + RandomGen::RangedRandomFloat(-0.44f, 0.44f));
               break;
          case StarClass::M:
               this->star->setColor({ 0.9f, 0.1f, 0.1f });
               this->star->setScale(0.5f + RandomGen::RangedRandomFloat(0.0f, 0.44f));
               break;
          default:
               this->star->setColor({ 0.9f, 0.6f, 0.0f });
               this->star->setScale(1.5f + RandomGen::RangedRandomFloat(-0.44f, 0.44f));
               break;
          }
     }

     Star(int starClass, unsigned int systemID) {
          this->starClass = (StarClass)starClass;
          this->starID = std::to_string(systemID) + "I";

          this->star = std::shared_ptr<Body>(new Body());
          this->star->setPosition(glm::vec3(0.0f));

          switch (starClass) {
          case 0:
               this->star->setColor({ 0.0f, 0.7f, 0.8f });
               this->star->setScale(2.5f + RandomGen::RangedRandomFloat(-0.44f, 0.44f));
               break;
          case 1:
               this->star->setColor({ 0.5f, 0.4f, 0.8f });
               this->star->setScale(2.0f + RandomGen::RangedRandomFloat(-0.44f, 0.44f));
               break;
          case 2:
               this->star->setColor({ 0.9f, 0.8f, 0.0f });
               this->star->setScale(1.5f + RandomGen::RangedRandomFloat(-0.44f, 0.44f));
               break;
          case 3:
               this->star->setColor({ 0.9f, 0.6f, 0.0f });
               this->star->setScale(1.0f + RandomGen::RangedRandomFloat(-0.44f, 0.44f));
               break;
          case 4:
               this->star->setColor({ 0.9f, 0.1f, 0.1f });
               this->star->setScale(0.5f + RandomGen::RangedRandomFloat(0.0f, 0.44f));
               break;
          default:
               this->star->setColor({ 0.9f, 0.6f, 0.0f });
               this->star->setScale(1.5f + RandomGen::RangedRandomFloat(-0.44f, 0.44f));
               break;
          }
     }

     ~Star() {
          star.~shared_ptr();
     }
};

class BodySystem {
public:
     //systemID
     BodySystem(int);
     //name, starClass, isNebula, bodyCount, systemID
     BodySystem(std::string, int, bool, int, int);

     ~BodySystem();

     void generateSystem();

     void setName(std::string);
     std::string getName();

     void setStar(StarClass, unsigned int);
     void setStar(int, unsigned int);
     Star getStar();

     void setIsNebula(bool);
     bool getIsNebula();

     void setBodyCount(int);
     int getBodyCount();

     void setBodyList(std::vector<std::shared_ptr<Body>>);
     std::vector<std::shared_ptr<Body>> getBodyList();

     void setSystemID(int);
     int getSystemID();

private:
     std::string         name;          //Name of System
     Star                star;          //System Star
     bool                isNebula;      //Is it within a nebula
     int                 bodyCount;     //How many bodies orbit the center
     std::vector<std::shared_ptr<Body>>  bodyList;      //List of bodies
     unsigned int        systemID;      //Identitification for this system
};

std::string intToNumeral(int x);
char* StrToChar(std::string str);