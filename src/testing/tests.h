#pragma once

#include <chrono>
#include <string>

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

using hr_clock = std::chrono::high_resolution_clock;

template<typename T>
std::pair<T, PerfMetric> MeasurePerf(T(*func)()) {
	hr_clock::time_point start = hr_clock::now();
	T res = func();
	hr_clock::duration elapsed = hr_clock::now() - start;
	PerfMetric p;
	p.seconds = elapsed.count() / 1000000000;
	p.milli = (elapsed.count() - (p.seconds * 1000000000)) / 1000000;
	p.nano = (elapsed.count() - (p.seconds * 1000000000) - (p.milli / 1000000));
	return { res, p };
}
void Test();