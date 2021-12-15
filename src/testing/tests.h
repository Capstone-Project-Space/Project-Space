#pragma once

struct TestResults {
	size_t success, failed, total;
	TestResults(size_t total);
};

void Test();