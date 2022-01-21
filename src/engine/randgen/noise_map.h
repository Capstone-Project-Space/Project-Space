#pragma once
#include "randomgen.h"

#include <vector>

class NoiseMap {
public:
     std::vector<float> *noisemap;

     NoiseMap();
     NoiseMap(int width, int height);
     ~NoiseMap();

     void GenerateNoiseMap();

     void SetWidth(int width);
     int GetWidth();
     void SetHeight(int height);
     int GetHeight();
     void SetXOffset(int xoffset);
     int GetXOffset();
     void SetYOffset(int yoffset);
     int GetYOffset();
     void SetPersistence(float persistence);
     float GetPersistence();
     void SetScale(float scale);
     float GetScale();

private:
     unsigned int width, height;
     int xoffset, yoffset;
     float persistence, scale;
};