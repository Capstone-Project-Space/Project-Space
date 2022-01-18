#pragma once

#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

namespace RandomGen {
     signed int RangedRandomSignedInt(int lo, int hi);
     unsigned int RangedRandomUnSignedInt(unsigned int lo, unsigned int hi);
     float RangedRandomFloat(float lo, float hi);
     double RangedRandomDouble(double lo, double hi);

     float Noise(int octave, int x, int y, int xOffset, int yOffset, float persistence, float scale, float lo, float hi);
};