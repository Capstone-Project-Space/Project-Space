#pragma once
#ifndef BODY_H
#define BODY_H

#include <string>
#include "glm/glm.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

class body {
public:
     std::string name;
     glm::vec3 initPosition;
     glm::vec3 position;
     glm::vec3 rotation;
     glm::vec3 scale;
     glm::vec3 color;

     //Orbit Variables
     float apoapsis;
     float periapsis;
     float width;
     float height;
     float heliocentricAngle;
     float currentAngle;

     body();
     ~body();
     void SetPosition(float angle);
};
#endif BODY_H