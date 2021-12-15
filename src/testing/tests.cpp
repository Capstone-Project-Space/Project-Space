#include <stdio.h>
#include <math.h>

#include <map>
#include <string>

#include "tests.h"


TestResults::TestResults(size_t total) : success(0), failed(0), total(total) {}

void PrintTestResults(const TestResults& results) {
	fprintf(stderr, "    \x1b[1;32mSuccess:\x1b[32m %llu \x1b[0m/ %llu (\x1b[32m%llu%%\x1b[0m)\n", results.success, results.total, (size_t)roundf((float)results.success / (float)results.total) * 100);
	fprintf(stderr, "    \x1b[1;31mFailed:\x1b[31m  %llu \x1b[0m/ %llu (\x1b[31m%llu%%\x1b[0m)\n", results.failed, results.total, (size_t)roundf((float)results.failed / (float)results.total) * 100);
}

TestResults TestFileStorage();

std::map<std::string, TestResults(*)()> Tests {
	{ "FileStorage", TestFileStorage }
};

void Test() {
	for (auto[name, func] : Tests) {
		fprintf(stderr, "%s:\n", name.c_str());
		PrintTestResults(func());
	}
}
