#pragma once

#include "tests.h"
#include "../engine/randgen/randomgen.h"
#include "../engine/randgen/noise_map.h"

TestResult* NM_TestNoiseMapDefaultConstructor();
TestResult* NM_TestNoiseMapCustomConstructor();
TestResult* NM_TestNoiseMapDestructor();
TestResult* NM_TestNoiseMapGetterSetter();
TestResult* NM_TestNoiseMapGeneration();