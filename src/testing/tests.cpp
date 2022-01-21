#include <stdio.h>
#include <math.h>

#include <map>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

#include "tests.h"
#include "test_filestorage.h"
#include "test_randomgen.h"
#include "test_noisemap.h"

TestResult::TestResult(const std::string& name) : name(name), msg(""), succeeded(false) {}
TestResult::TestResult(const std::string& name, const std::string& msg, bool succeeded) : name(name), msg(msg), succeeded(succeeded) {}

void PrintTestResult(const TestResult& result) {
	if (result.succeeded) {
		fprintf(stderr, "    %s \x1b[1;32mSucceeded\x1b[0m.\n", result.name.c_str());
	} else {
		fprintf(stderr, "    %s \x1b[1;31mFailed\x1b[0m.\n        \x1b[1;31mERROR\x1b[0m: %s.\n", result.name.c_str(), result.msg.c_str());
	}
}

std::map<std::string, std::vector<TestResult (*)()>> Tests {
	{ "FileStorage", { FS_TestAllDataTypes, FS_TestMultipleSaveables } },
	{ "RandomGen", { RG_TestRandomSignedInt, RG_TestRandomUnSignedInt, RG_TestRandomFloat, RG_TestRandomDouble} },
	{ "NoiseMap", { NM_TestNoiseMapDefaultConstructor,  NM_TestNoiseMapCustomConstructor, NM_TestNoiseMapDestructor, NM_TestNoiseMapGetterSetter, NM_TestNoiseMapGeneration } }
};

void Test() {
	for (auto[name, funcs] : Tests) {
		fprintf(stderr, "%s:\n", name.c_str());
		std::vector<TestResult> results;
		results.reserve(funcs.size());
		size_t successful = 0;
		// TODO: Attempt to add exception error support.
		for (const auto func : funcs) {
			TestResult result = func();
			results.push_back(result);
			PrintTestResult(results.back());
			successful += (size_t) results.back().succeeded;
		}
		size_t total = results.size(), failed = total - successful;
		fprintf(stderr, "    \x1b[1;32mSucceeded:\x1b[32m %llu \x1b[0m/ %llu (\x1b[32m%llu%%\x1b[0m)\n", successful, total, (size_t)roundf((float)successful / (float)total) * 100);
		fprintf(stderr, "    \x1b[1;31mFailed:\x1b[31m    %llu \x1b[0m/ %llu (\x1b[31m%llu%%\x1b[0m)\n", failed, total, (size_t)roundf((float)failed / (float)total) * 100);
	}
}
