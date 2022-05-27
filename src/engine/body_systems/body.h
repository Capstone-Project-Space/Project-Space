#pragma once

#include <string>
#include <memory>
#include <optional>
#include <set>
#include <vector>

#include <glm/glm.hpp>

//Solar system body types
constexpr uint32_t BODY_TYPE_COUNT = 8;
enum class BodyType {ROCKY_PLANET, DEAD_PLANET, EARTH_PLANET, GREEN_GAS_GIANT, BROWN_GAS_GIANT, WATER, MOON, SUN};
//Body habitability
enum class HabitType {HABITABLE, UNHABITABLE};

//A-Class: Radius = 2.5       Color = Cyan
//F-Class: Radius = 2.0       Color = Pale Purple
//G-Class: Radius = 1.5       Color = Pale Yellow
//K-Class: Radius = 1.0       Color = Orange
//M-Class: Radius = 0.5       Color = Red
constexpr uint32_t STAR_CLASS_COUNT = 5;
enum class StarClass { A, F, G, K, M };

HabitType GetRandomHabitFromBodyType(BodyType type);
float GetRandomScaleFromBodyType(BodyType type);
float GetStarRadiusFromClass(StarClass cl);
glm::vec3 GetStarColorFromClass(StarClass cl);

class Body {
public:
    struct ChildrenConstraints {
        uint32_t minChilds, maxChilds;
        float distance;
        std::set<BodyType> allowedBodyTypes;
    };
    static Body CreateSingleStarSystem(const std::string& name);

    Body(const std::string& name, BodyType bodyType, HabitType habitType, const glm::vec3& position, float scale, const glm::vec3& color, std::optional<ChildrenConstraints> childConstraints=std::nullopt);

    void update(float delta);
    void draw(float delta, bool children=false) const;

    const inline std::string& getName() const { return this->name; }
    inline void setName(const std::string& name) { this->name = name; }

    inline BodyType getBodyType() const { return this->bodyType; }
    inline HabitType getHabitType() const { return this->habitType; }

    const inline glm::vec3& getPosition() const { return this->position; }
    inline float getScale() const { return this->scale; }
    const inline glm::vec4& getColor() const { return this->color; }

    const inline std::vector<Body>& getChildren() const { return this->children; }

private:
    std::string name;
    BodyType bodyType;
    HabitType habitType;
    glm::vec3 position, lastPosition;
    float scale;
    glm::vec4 color;
    std::vector<Body> children;
};

std::string intToNumeral(uint32_t x);

//Body Class
//class Body {
//public:
//     //Default Constructor
//     Body();
//     //Custom Constructor (Should be used when reading file data)
//     Body(const std::string&, int, int, const glm::vec3&, float, const glm::vec3&, int);
//     
//     //Name Get Set
//     void setName(const std::string&);
//     const std::string& getName() const;
//
//     //BodyType Get Set
//     void setBodyType(const BodyType);
//     BodyType getBodyType() const;
//     
//     //HabitType Get Set
//     void setHabitType(const HabitType);
//     HabitType getHabitType() const;
//
//     //Position Get Set
//     void setPosition(const glm::vec3&);
//     const glm::vec3& getPosition() const;
//
//     //Scale Get Set
//     void setScale(const float);
//     float getScale() const;
//
//     //Color Get Set
//     void setColor(const glm::vec3&);
//     const glm::vec3& getColor() const;
//
//     //MoonCount Get Set
//     void setMoonCount(const int);
//     int getMoonCount() const;
//
//     //Moon List Get Set
//     void setMoonList(const std::vector<std::shared_ptr<Body>>&);
//     const std::vector<std::shared_ptr<Body>>& getMoonList() const;
//
//     //BodyID Get Set
//     void setBodyID(const std::string&);
//     const std::string& getBodyID() const;
//
//     void printDebugInfo() const;
//
//private:
//     std::string         name;          //Name of Body
//     BodyType            bodyType;      //What type of body
//     HabitType           habitType;     //Is it habitable
//     glm::vec3           position;      //Position (solar system center is the origin)
//     float               scale;         //What to scale the sphere object by
//     glm::vec3           color;         //Color used for unitialized textures and lighting
//     int                 moonCount;     //How many moons orbit the body
//     std::vector<std::shared_ptr<Body>>  moonList;      //List of moons
//     std::string         bodyID;        //Identification for this body
//};
