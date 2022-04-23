#include "test_randomgen.h"

#include <vector>
#include <map>

constexpr signed int lo = -1000;
constexpr int mid = 0;
constexpr signed int hi = 1000;

constexpr double lodub = -1000.0;
constexpr double hidub = 1000.0;

constexpr float lof = -1000.0;
constexpr float hif = 1000.0;

TestResult* RG_TestRandomSignedInt() {
     TestResult* result = new TestResult{ "Random Signed Integer Generation" };
     std::map<signed int, unsigned int> valuecount;

     for (int i = lo; i <= hi; i++) {
          valuecount.emplace(i, 0);
     }

     for (int i = 0; i < 100; i++) {
          signed int x = RandomGen::RangedRandomSignedInt(lo, hi);
          if ((x < lo) || (x > hi)) {
               result->msg = "signed int value not generated within correct range";
               return result;
          }
          valuecount.at(x) += 1;
     }

     for (auto [i, j] : valuecount) {
          if (j > 2) {
               result->msg = "signed int value generated too much";
               return result;
          }
     }

     result->succeeded = true;
     return result;
}

TestResult* RG_TestRandomUnSignedInt() {
     TestResult* result = new TestResult{ "Random UnSigned Integer Generation" };
     std::map<unsigned int, unsigned int> valuecount;

     for (int i = mid; i <= hi*2; i++) {
          valuecount.emplace(i, 0);
     }

     for (int i = 0; i < 100; i++) {
          unsigned int x = RandomGen::RangedRandomUnSignedInt(mid, hi*2);
          if ((x < mid) || (x > hi*2)) {
               result->msg = "unsigned int value not generated within correct range";
               return result;
          }
          valuecount.at(x) += 1;
     }

     for (auto [i, j] : valuecount) {
          if (j > 2) {
               result->msg = "unsigned int value generated too much";
               return result;
          }
     }

     result->succeeded = true;
     return result;
}

TestResult* RG_TestRandomFloat() {
     TestResult* result = new TestResult{ "Random Float Generation" };
     std::vector<float> record;

     for (int i = 0; i < 100; i++) {
          float x = RandomGen::RangedRandomFloat(lof, hif);
          if ((x < lof) || (x > hif)) {
               result->msg = "float value not generated within correct range";
               return result;
          }

          if (record.empty()) {
               record.push_back(x);
          }
          else if (std::find(record.begin(), record.end(), x) != record.end()) {
               result->msg = "float value already generated";
               return result;
          }
          else {
               record.push_back(x);
          }
     }

     result->succeeded = true;
     return result;
}

TestResult* RG_TestRandomDouble() {
     TestResult* result = new TestResult{ "Random Float Generation" };
     std::vector<double> record;

     for (int i = 0; i < 100; i++) {
          float x = RandomGen::RangedRandomDouble(lodub, hidub);
          if ((x < lodub) || (x > hidub)) {
               result->msg = "float value not generated within correct range";
               return result;
          }

          if (record.empty()) {
               record.push_back(x);
          }
          else if (std::find(record.begin(), record.end(), x) != record.end()) {
               result->msg = "float value already generated";
               return result;
          }
          else {
               record.push_back(x);
          }
     }

     result->succeeded = true;
     return result;
}