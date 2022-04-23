#pragma once

#include <assert.h>

#include <chrono>
#include <string>
#include <variant>

#include <Windows.h>

struct TestResult {
	std::string name;
	std::string msg;
	bool succeeded;
	TestResult(const std::string& name);
	TestResult(const std::string& name, const std::string& msg, bool succeeded);
};

struct PerfMetric {
	size_t seconds, milli, nano;
};

struct Pair {
	std::variant<std::string, TestResult*> result;
	PerfMetric metric;
	Pair(const std::string& error, const PerfMetric& metric);
	Pair(TestResult* result, const PerfMetric& metric);
};

using hr_clock = std::chrono::high_resolution_clock;

struct Exception {
	bool isErr = false;
	union {
		const char* error = NULL;
		TestResult* result;
	};
	Exception();
	//Exception(Exception& e);
};

Exception PerfHelper(TestResult(*func)());

Exception PerfExcept(TestResult(*func)());

Pair MeasurePerf(TestResult(*func)());

void Test();
