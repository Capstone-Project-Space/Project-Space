#include "randomgen.h"

signed int RandomGen::RangedRandomSignedInt(int lo, int hi) {
     std::random_device r;
     std::mt19937 rng(r());
     std::uniform_int_distribution<int> dis(lo, hi);
     return dis(rng);
}

unsigned int RandomGen::RangedRandomUnSignedInt(unsigned int lo, unsigned int hi) {
     std::random_device r;
     std::mt19937 rng(r());
     std::uniform_int_distribution<unsigned int> dis(lo, hi);
     return dis(rng);
}

float RandomGen::RangedRandomFloat(float lo, float hi) {
     std::random_device r;
     std::mt19937 rng(r());
     std::uniform_real_distribution<float> dis(lo, hi);
     return dis(rng);
}

double RandomGen::RangedRandomDouble(double lo, double hi) {
     std::random_device r;
     std::mt19937 rng(r());
     std::uniform_real_distribution<double> dis(lo, hi);
     return dis(rng);
}


//This function will be updated in the future
float RandomGen::Noise(int octave, int x, int y, int xoffset, int yoffset, float persistence, float scale, float lo, float hi) {
     float maxamp = 0.;
     float amp = 1.f;
     float freq = scale;
     float n = 0.f;

     for (int i = 0; i < octave; i++) {
          n += glm::simplex(glm::vec2((float)(x * freq) + (float)(xoffset * scale * 4),
                                      (float)(y * freq) - (float)(yoffset * scale * 4)));

          maxamp += amp;
          amp *= persistence;
          freq *= 2;
     }

     n /= maxamp;
     n = n * (hi - lo) / 2 + (hi + lo) / 2;

     return n;
}