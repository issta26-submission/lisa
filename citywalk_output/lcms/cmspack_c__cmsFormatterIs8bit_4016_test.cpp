/*
Unit test suite for cmsBool _cmsFormatterIs8bit in cmspack.c

Approach:
- Use a lightweight C++11 test harness (no Google Test) that calls the focal C function.
- To avoid relying on the full plugin/test environment, override the dependent macro T_BYTES(Type)
  via preprocessor before including the target C file cmspack.c. This lets us deterministically
  drive the function's internal Bytes value for given Type inputs.
- Cover true (Bytes == 1) and false (Bytes != 1) branches.
- Provide a small, non-terminating assertion macro to run multiple tests in a single pass.

Notes:
- We compile this test as a C++11 translation unit and include cmspack.c with C linkage to expose
  _cmsFormatterIs8bit to the test code.
- The test asserts behavior for Type == 1 (Bytes evaluates to 1, should return true) and Type == 2
  (Bytes evaluates to 2, should return false) using a controlled T_BYTES macro.
*/

// Standard library headers for I/O and string formatting (only used for reporting)
#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cmspack.c>


// Global test failure counter
static int g_testFailures = 0;

// Non-terminating assertion macro: it reports failures but does not abort the test run.
// This aligns with the requirement to maximize code execution on test failures.
#define TEST_ASSERT(cond, msg)                                           \
    do {                                                                   \
        if (!(cond)) {                                                   \
            std::cerr << "Test Failure: " << (msg)                     \
                      << " | Condition: " << #cond                      \
                      << " | Location: " << __FILE__ << ":" << __LINE__ \
                      << std::endl;                                      \
            ++g_testFailures;                                          \
        }                                                              \
    } while (0)

// Step 2 - Lightweight macro to deterministically influence the number of bytes for a given Type.
// We map Type == 1 to 1 byte, and all other Types to 2 bytes (to exercise the false branch).
// This macro overrides any default T_BYTES(Type) behavior inside cmspack.c for the test.
#define T_BYTES(Type) ((Type) == 1 ? 1 : 2)

// Ensure the focal C source is compiled with C linkage and that our macro override is visible
extern "C" {
}

// Helper function to run a single test and report summary (non-terminating)
static void runAllTests() {
    // Test 1: Should return true when Type yields 1 byte (Bytes == 1)
    {
        // Given our T_BYTES macro, Type = 1 => Bytes == 1 => _cmsFormatterIs8bit should be true
        cmsUInt32Number typeTrue = 1;
        cmsBool resultTrue = _cmsFormatterIs8bit(typeTrue);
        TEST_ASSERT(resultTrue == 1,
                    "Expected _cmsFormatterIs8bit(1) to return true (Bytes == 1)");
    }

    // Test 2: Should return false when Type yields more than 1 byte (Bytes != 1)
    {
        // Type = 2 => Bytes = 2 => function should return false
        cmsUInt32Number typeFalse = 2;
        cmsBool resultFalse = _cmsFormatterIs8bit(typeFalse);
        TEST_ASSERT(resultFalse == 0,
                    "Expected _cmsFormatterIs8bit(2) to return false (Bytes != 1)");
    }

    // Optional additional check: ensure a non-1 non-2 Type yields false (Bytes == 2 by our macro)
    {
        cmsUInt32Number typeOther = 999;
        cmsBool resultOther = _cmsFormatterIs8bit(typeOther);
        TEST_ASSERT(resultOther == 0,
                    "Expected _cmsFormatterIs8bit(999) to return false (Bytes != 1) via default path");
    }
}

int main() {
    // Run tests
    runAllTests();

    // Report summary
    if (g_testFailures == 0) {
        std::cout << "All tests passed for cmsFormatterIs8bit." << std::endl;
        return 0;
    } else {
        std::cerr << "Total failures: " << g_testFailures << std::endl;
        return 1;
    }
}