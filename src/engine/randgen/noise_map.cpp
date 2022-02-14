#include "noise_map.h"

NoiseMap::NoiseMap() {
     this->width = pow(2,4);
     this->height = pow(2, 4);

     this->xoffset = 0;
     this->yoffset = 0;

     this->persistence = 0.0f;
     this->scale = 1.0f;

}

NoiseMap::NoiseMap(int width, int height) {
     this->width = width;
     this->height = height;

     this->xoffset = 0;
     this->yoffset = 0;

     this->persistence = 0.0f;
     this->scale = 1.0f;
}

void NoiseMap::GenerateNoiseMap() {
     for (int y = 0; y < height; y++) {
          for (int x = 0; x < width; x++) {
               noisemap.push_back(RandomGen::Noise(4, x, y, this->xoffset, this->yoffset, this->persistence, this->scale, 0.0f, 1.0f));
          }
     }
}

void NoiseMap::SetWidth(int width) {
     this->width = width;
}

int NoiseMap::GetWidth() {
     return this->width;
}

void NoiseMap::SetHeight(int height) {
     this->height = height;
}

int NoiseMap::GetHeight() {
     return this->height;
}

void NoiseMap::SetXOffset(int xoffset) {
     this->xoffset = xoffset;
}

int NoiseMap::GetXOffset() {
     return this->xoffset;
}

void NoiseMap::SetYOffset(int yoffset) {
     this->yoffset = yoffset;
}

int NoiseMap::GetYOffset() {
     return this->yoffset;
}

void NoiseMap::SetPersistence(float persistence) {
     this->persistence = persistence;
}

float NoiseMap::GetPersistence() {
     return this->persistence;
}

void NoiseMap::SetScale(float scale) {
     this->scale = scale;
}

float NoiseMap::GetScale() {
     return this->scale;
}