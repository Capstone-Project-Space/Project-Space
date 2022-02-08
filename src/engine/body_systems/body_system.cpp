#include "body_system.h"

body_system::body_system(std::string name, int sun_count, int body_count) {
     this->name = name;

     if (sun_count == NULL) {
          this->sun_count = 1;
     }
     else {
          this->sun_count = sun_count;
     }

     if (sun_count == 1) {
          centralSun = true;
     }
     else {
          centralSun = (RandomGen::RangedRandomFloat(0.f, 1.f) > 0.95f ? true : false);
     }

     for (int i = 0; i < sun_count; i++) {
          body* s = new body();
          s->name = name + " " + ("a" + i);
          suns.push_back(s);
     }

     if (body_count == NULL) {
          this->body_count = 1;
     }
     else {
          this->body_count = body_count;
     }
     for (int i = 0; i < body_count; i++) {
          body* b = new body();
          b->name = name + " " + intToNumeral(i + 1);
          bodies.push_back(b);
     }

     systemRange = RandomGen::RangedRandomFloat(10.f, 10.f + (2.f * body_count));
     GenerateSystem();
}

body_system::~body_system() {
     for (auto s : suns) {
          delete s;
     }
     for (auto b : bodies) {
          delete b;
     }
     suns.clear();
     bodies.clear();
}

void body_system::GenerateSystem() {
     //Set Suns
     for (size_t i = 0; i < suns.size(); i++) {
          if (centralSun && i == 0) {
               suns[i]->scale = glm::vec3(1.);

               suns[i]->apoapsis = 0.f;
               suns[i]->periapsis = 0.f;
               //suns[i]->DrawEllipseOn = 0;
          }
          else {
               suns[i]->scale = glm::vec3(RandomGen::RangedRandomFloat(0.5f, 0.8f));

               suns[i]->apoapsis = 1.f;
               suns[i]->periapsis = -1.f;

          }
          suns[i]->rotation = glm::vec3(0.);
          suns[i]->color = glm::vec3((255. / 255.), (182. / 255.), (17. / 255.));
     }

     //Set Bodies
     for (size_t i = 0; i < bodies.size(); i++) {
          //Set Periapsis
          if (i == 0) {
               //Lowest Orbit is 2.f from origin
               bodies[i]->periapsis = 2.f;
          }
          else {
               //Every other planet will be at least it's radius and the last bodies radius away from the previous apoapsis
               float x = bodies[i - 1]->apoapsis + bodies[i - 1]->scale.x + bodies[i]->scale.x;
               float y = x + (systemRange - x) / (bodies.size() - i + 1);
               if (y <= x) y = x;
               bodies[i]->periapsis = RandomGen::RangedRandomFloat(x, y);
          }
          //Set Apoapsis
          float a = bodies[i]->periapsis + (systemRange - bodies[i]->periapsis) / (bodies.size() - i + 1);
          if (a >= systemRange) a = systemRange;
          if (a <= bodies[i]->periapsis) a = bodies[i]->periapsis;
          bodies[i]->apoapsis = RandomGen::RangedRandomFloat(bodies[i]->periapsis, a);

          bodies[i]->width = (bodies[i]->periapsis + bodies[i]->apoapsis) / 2;
          if (i == 0) {
               bodies[i]->height = RandomGen::RangedRandomFloat(bodies[i]->width / 2, bodies[i]->width);
          }
          else {
               bodies[i]->height = RandomGen::RangedRandomFloat((bodies[i]->width / 2 > bodies[i - 1]->apoapsis ? bodies[i]->width / 2 : bodies[i - 1]->apoapsis), bodies[i]->width);
          }

          bodies[i]->scale = glm::vec3(RandomGen::RangedRandomFloat(0.15f * (bodies[i]->periapsis / systemRange), 0.75f * (bodies[i]->periapsis / systemRange)));
          bodies[i]->color = glm::vec3(RandomGen::RangedRandomFloat((70. / 255.), (185. / 255.)), RandomGen::RangedRandomFloat((70. / 255.), (185. / 255.)), RandomGen::RangedRandomFloat((70. / 255.), (185. / 255.)));

          bodies[i]->heliocentricAngle = RandomGen::RangedRandomFloat(0.f, 360.f);

          bodies[i]->initPosition.x = bodies[i]->width * cosf(bodies[i]->currentAngle * (M_PI / 180.f)) + (bodies[i]->width - bodies[i]->periapsis);
          bodies[i]->initPosition.y = 0.f;
          bodies[i]->initPosition.z = bodies[i]->height * sinf(bodies[i]->currentAngle * (M_PI / 180.f) + M_PI);

          bodies[i]->position = bodies[i]->initPosition;

          //bodies[i]->orbitPeriod = pow(bodies[i]->apoapsis + bodies[i]->periapsis, 3. / 2.) / 5;
     }

     systemRange = bodies[bodies.size() - 1]->apoapsis + (bodies[bodies.size() - 1]->scale.x * 2);
}

void body_system::Restart() {
     for (auto b : bodies) {
          b->position = b->initPosition;
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