#pragma once

#include <string>

struct TestResult {
	std::string name;
	std::string msg;
	bool succeeded;
	TestResult(const std::string& name);
	TestResult(const std::string& name, const std::string& msg, bool succeeded);
};

void Test();