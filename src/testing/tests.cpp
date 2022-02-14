#include <stdio.h>

#include <map>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <chrono>

#include "tests.h"
#include "test_filestorage.h"
#include "test_randomgen.h"
#include "test_noisemap.h"
#include "test_gamestate.h"
#include "test_window.h"

TestResult::TestResult(const std::string& name)
	: name(name), msg(""), succeeded(false) {}
TestResult::TestResult(const std::string& name, const std::string& msg, bool succeeded)
	: name(name), msg(msg), succeeded(succeeded) {}

Pair::Pair(const std::string& error, const PerfMetric& metric)
	: result(error), metric(metric) {}

Pair::Pair(const TestResult& result, const PerfMetric& metric) 
	: result(result), metric(metric) {}

Exception::Exception() {}
Exception::Exception(Exception& e) {
	this->isErr = e.isErr;
	if (isErr) this->error = e.error;
	else this->result = e.result;
}

Exception PerfHelper(TestResult(*func)()) {
	Exception ex;
	ex.result = malloc(sizeof(TestResult));
	try {
		if (ex.result)
			memcpy(ex.result, &(func()), sizeof(TestResult));
		else throw "Out of Memory";
	} catch (const char* s) {
		free(ex.result);
		ex.error = s;
		ex.isErr = true;
	} catch (const std::exception& e) {
		free(ex.result);
		ex.error = e.what();
		ex.isErr = true;
	}
	return ex;
}

Exception PerfExcept(TestResult(*func)()) {
	Exception e;
	__try {
		return PerfHelper(func);
	}
	__except (GetExceptionCode() != EXCEPTION_BREAKPOINT) {
		e.isErr = true;
		switch (GetExceptionCode()) {
		case EXCEPTION_ACCESS_VIOLATION:
			e.error = "Access Violation Exception";
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			e.error = "Array Bounds Exceeded Exception";
			break;
		case EXCEPTION_COLLIDED_UNWIND:
			e.error = "Collided Unwind Exception";
			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			e.error = "Floating Point Divide by Zero Exception";
			break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			e.error = "Integer Divide by Zero Exception";
			break;
		case EXCEPTION_READ_FAULT:
			e.error = "Read Fault Exception";
			break;
		case EXCEPTION_STACK_OVERFLOW:
			e.error = "Stack Overflow Exception";
			break;
		default:
			e.error = "Unknown Exceptioon";
			break;
		}
	}
	return e;
}

Pair MeasurePerf(TestResult(*func)()) {
	hr_clock::time_point start = hr_clock::now();
	Exception e = PerfExcept(func);
	hr_clock::duration elapsed = hr_clock::now() - start;
	PerfMetric p;
	p.seconds = elapsed.count() / 1000000000;
	p.milli = (elapsed.count() - (p.seconds * 1000000000)) / 1000000;
	p.nano = (elapsed.count() - (p.seconds * 1000000000) - (p.milli / 1000000));

	if (e.isErr) return Pair{ std::string(e.error), p };
	else return Pair{ *((TestResult*)e.result), p };
}

void PrintTestResult(const TestResult& result, const PerfMetric& metrics) {
	if (result.succeeded) {
		fprintf(
			stderr, "    %-44s \x1b[1;32mSucceeded\x1b[0m. Time: %llu:%.3llu:%.9llu\n",
			result.name.c_str(), metrics.seconds, metrics.milli, metrics.nano
		);
	} else {
		fprintf(
			stderr, "    %-44s \x1b[1;31m%9s\x1b[0m. Time: %llu:%.3llu:%.9llu\n        \x1b[1;31mERROR\x1b[0m: %s.\n", 
			result.name.c_str(), "Failed", metrics.seconds, metrics.milli, metrics.nano, result.msg.c_str()
		);
	}
}

std::map<std::string, std::vector<TestResult (*)()>> Tests {
	{ "FileStorage", { FS_TestAllDataTypes, FS_TestMultipleSaveables, FS_TestLargeFileSaveable, FS_TestManySmallSaveable } },
	{ "RandomGen", { RG_TestRandomSignedInt, RG_TestRandomUnSignedInt, RG_TestRandomFloat, RG_TestRandomDouble} },
	{ "NoiseMap", { NM_TestNoiseMapDefaultConstructor,  NM_TestNoiseMapCustomConstructor, NM_TestNoiseMapDestructor, NM_TestNoiseMapGetterSetter, NM_TestNoiseMapGeneration } },
	{ "GameStates", { GS_TestStateChanges, GS_TestEventReceiving } },
	{ "Window", { WN_Test_Creation }}
};

void Test() {
	for (auto[name, funcs] : Tests) {
		fprintf(stderr, "%s:\n", name.c_str());
		std::vector<TestResult> results;
		results.reserve(funcs.size());
		size_t successful = 0;
		for (const auto func : funcs) {
			auto[either, metrics] = MeasurePerf(func);
			if (either.index() == 1)
				results.push_back(std::get<TestResult>(either));
			else
				results.emplace_back("Unknown", std::get<std::string>(either), false);
			PrintTestResult(results.back(), metrics);
			successful += (size_t) results.back().succeeded;
		}
		size_t total = results.size(), failed = total - successful;
		fprintf(stderr, "    \x1b[1;32mSucceeded:\x1b[32m %llu \x1b[0m/ %llu (\x1b[32m%llu%%\x1b[0m)\n", successful, total, (size_t) ((float) successful / (float) total * 100.0f));
		fprintf(stderr, "    \x1b[1;31mFailed:\x1b[31m    %llu \x1b[0m/ %llu (\x1b[31m%llu%%\x1b[0m)\n", failed, total, (size_t) ((float) failed / (float) total * 100.0f));
	}
}
