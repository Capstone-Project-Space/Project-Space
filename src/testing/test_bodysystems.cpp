#include "test_bodysystems.h"

TestResult* BS_TestBodySystemConstructor() {
     TestResult* result = new TestResult{"Body System Default Contructor"};

     result->succeeded = true;
     return result;
}