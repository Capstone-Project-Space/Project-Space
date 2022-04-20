#pragma once

#include <string>
#include <memory>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "glm/glm.hpp"

//Solar system body types
enum class BodyType {Planet, Moon, GasGiant, Sun};
//Body habitability
enum class HabitType {Habitable, UnInhabitable};

//Body Class
class Body {
public:
     //Default Constructor
     Body();
     //Custom Constructor (Should be used when reading file data)
     Body(std::string, int, int, glm::vec3, float, glm::vec3, int);
     //Destructor
     ~Body();

     //Name Get Set
     void setName(std::string);
     std::string getName();

     //BodyType Get Set
     void setBodyType(BodyType);
     BodyType getBodyType();
     
     //HabitType Get Set
     void setHabitType(HabitType);
     HabitType getHabitType();

     //Position Get Set
     void setPosition(glm::vec3);
     glm::vec3 getPosition();

     //Scale Get Set
     void setScale(float);
     float getScale();

     //Color Get Set
     void setColor(glm::vec3);
     glm::vec3 getColor();

     //MoonCount Get Set
     void setMoonCount(int);
     int getMoonCount();

     //Moon List Get Set
     void setMoonList(std::vector<std::shared_ptr<Body>>);
     std::vector<std::shared_ptr<Body>> getMoonList();

     //BodyID Get Set
     void setBodyID(std::string);
     std::string getBodyID();

     void printDebugInfo();

private:
     std::string         name;          //Name of Body
     BodyType            bodyType;      //What type of body
     HabitType           habitType;     //Is it habitable
     glm::vec3           position;      //Position (solar system center is the origin)
     float               scale;         //What to scale the sphere object by
     glm::vec3           color;         //Color used for unitialized textures and lighting
     int                 moonCount;     //How many moons orbit the body
     std::vector<std::shared_ptr<Body>>  moonList;      //List of moons
     std::string         bodyID;        //Identification for this body
};