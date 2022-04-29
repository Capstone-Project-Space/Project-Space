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
     Body(const std::string&, int, int, const glm::vec3&, float, const glm::vec3&, int);
     
     //Name Get Set
     void setName(const std::string&);
     const std::string& getName() const;

     //BodyType Get Set
     void setBodyType(const BodyType);
     BodyType getBodyType() const;
     
     //HabitType Get Set
     void setHabitType(const HabitType);
     HabitType getHabitType() const;

     //Position Get Set
     void setPosition(const glm::vec3&);
     const glm::vec3& getPosition() const;

     //Scale Get Set
     void setScale(const float);
     float getScale() const;

     //Color Get Set
     void setColor(const glm::vec3&);
     const glm::vec3& getColor() const;

     //MoonCount Get Set
     void setMoonCount(const int);
     int getMoonCount() const;

     //Moon List Get Set
     void setMoonList(const std::vector<std::shared_ptr<Body>>&);
     const std::vector<std::shared_ptr<Body>>& getMoonList() const;

     //BodyID Get Set
     void setBodyID(const std::string&);
     const std::string& getBodyID() const;

     void printDebugInfo() const;

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