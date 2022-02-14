#include "test_noisemap.h"

TestResult NM_TestNoiseMapDefaultConstructor() {
     TestResult result{ "Noise Map Default Contructor" };
     NoiseMap* nm = new NoiseMap();

     if (nm->GetWidth() != pow(2, 4)) {
          result.msg = "default width was no initialized correctly";
          return result;
     }

     if (nm->GetHeight() != pow(2, 4)) {
          result.msg = "default height was no initialized correctly";
          return result;
     }

     delete nm;
     result.succeeded = true;
     return result;
}

TestResult NM_TestNoiseMapCustomConstructor() {
     TestResult result{ "Noise Map Custom Contructor" };
     unsigned int x = RandomGen::RangedRandomUnSignedInt(16, 64);
     NoiseMap* nm = new NoiseMap(x, x);

     if (nm->GetWidth() != x) {
          result.msg = "width was no initialized correctly";
          return result;
     }

     if (nm->GetHeight() != x) {
          result.msg = "height was no initialized correctly";
          return result;
     }

     delete nm;
     result.succeeded = true;
     return result;
}

TestResult NM_TestNoiseMapDestructor() {
     TestResult result{ "Noise Map Detructor" };
     NoiseMap* nm = new NoiseMap();
     delete nm;

     if (&nm == nullptr) {
          result.msg = "noise map destruction not successful";
          return result;
     }
     result.succeeded = true;
     return result;
}

TestResult NM_TestNoiseMapGetterSetter() {
     TestResult result{ "Noise Map Getters and Setters" };
     NoiseMap* nm = new NoiseMap;
     int width = RandomGen::RangedRandomUnSignedInt(16, 64);
     int height = RandomGen::RangedRandomUnSignedInt(16, 64);
     int xoffset = RandomGen::RangedRandomUnSignedInt(8, 32);
     int yoffset = RandomGen::RangedRandomUnSignedInt(8, 32);
     float persistence = RandomGen::RangedRandomFloat(0.0f, 10.0f);
     float scale = RandomGen::RangedRandomFloat(0.0f, 10.0f);
     
     //Test GetWidth and SetWidth
     if (nm->GetWidth() != pow(2,4)) {
          result.msg = "GetWidth() failed to acquire value";
          return result;
     }
     nm->SetWidth(width);
     if (nm->GetWidth() != width) {
          result.msg = "SetWidth() failed to set value";
          return result;
     }

     //Test GetHeight and SetHeight
     if (nm->GetHeight() != pow(2,4)) {
          result.msg = "GetHeight() failed to acquire value";
          return result;
     }
     nm->SetHeight(height);
     if (nm->GetHeight() != height) {
          result.msg = "SetHeight() failed to set value";
          return result;
     }

     //Test GetXOffset and SetXOffset
     if (nm->GetXOffset() != 0) {
          result.msg = "GetXOffset() failed to acquire value";
          return result;
     }
     nm->SetXOffset(xoffset);
     if (nm->GetXOffset() != xoffset) {
          result.msg = "SetXOffset() failed to set value";
          return result;
     }

     //Test GetYOffset and SetYOffset
     if (nm->GetYOffset() != 0) {
          result.msg = "GetYOffset() failed to acquire value";
          return result;
     }
     nm->SetYOffset(yoffset);
     if (nm->GetYOffset() != yoffset) {
          result.msg = "SetYOffset() failed to set value";
          return result;
     }
     
     //Test GetPersistence and SetPersistence
     if (nm->GetPersistence() != 0.0f) {
          result.msg = "GetPersistence() failed to acquire value";
          return result;
     }
     nm->SetPersistence(persistence);
     if (nm->GetPersistence() != persistence) {
          result.msg = "SetPersistence() failed to set value";
          return result;
     }

     //Test GetScale and SetScale
     if (nm->GetScale() != 1.0f) {
          result.msg = "GetScale() failed to acquire value";
          return result;
     }
     nm->SetScale(scale);
     if (nm->GetScale() != scale) {
          result.msg = "SetScale() failed to set value";
          return result;
     }

     delete nm;
     result.succeeded = true;
     return result;
}

TestResult NM_TestNoiseMapGeneration() {
     TestResult result{ "Noise Map Generation" };
     NoiseMap* nm = new NoiseMap(64, 64);

     nm->SetPersistence(1.5f);
     nm->SetScale(0.5f);
     nm->SetXOffset(10);
     nm->SetYOffset(64);

     nm->GenerateNoiseMap();

     int x = nm->GetWidth() * nm->GetHeight();
     if (nm->noisemap.size() != x) {
          result.msg = "noise map generation failed";
     }
     for (float f : nm->noisemap) {
          if ((f < 0.0f) || (f > 1.0f)) {
               result.msg = ("noise map value not within range [0.0, 1.0]: " + std::to_string(f));
               return result;
          }
     }

     delete nm;
     result.succeeded = true;
     return result;
}