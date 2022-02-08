#include "body.h"

body::body() {
     this->name = "";
     this->initPosition = glm::vec3(0.f);
     this->position = glm::vec3(0.f);
     this->rotation = glm::vec3(0.f);
     this->scale = glm::vec3(1.f);
     this->color = glm::vec3(0.5f);
     this->apoapsis = 0.f;
     this->periapsis = 0.f;
     this->width = 0.f;
     this->height = 0.f;
     this->heliocentricAngle = 0.f;
     this->currentAngle = 0.f;
//     this->orbitPeriod = 0.f;     //Seconds
//     this->currentOrbit = 0.f;
//     this->DrawEllipseOn = 1;
}

body::~body() {

}

/*void body::Animate(float dt) {
     currentOrbit += dt;
     if (currentOrbit >= orbitPeriod) {
          currentOrbit -= orbitPeriod;
     }
     float orbitAngle = (currentOrbit / orbitPeriod) * 360.f;
     SetPosition(orbitAngle);
}*/

void body::SetPosition(float angle) {
     position.x = width * cosf(angle * (M_PI / 180.f)) + (width - periapsis);
     position.z = height * sinf(angle * (M_PI / 180.f) + M_PI);
}