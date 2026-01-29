/*
Test suite for the focal method:
    cmsBool WriteDoubles(cmsIOHANDLER* io, cmsUInt32Number n, cmsFloat64Number* Values)

Context:
- The function iterates i from 0 to n-1 and calls _cmsWrite15Fixed16Number(io, *Values++)
- It returns FALSE if any call returns FALSE, otherwise TRUE.

Given constraints:
- No GTest; implement a small, self-contained test harness.
- Provide minimal dependencies and mocks for cmsIOHANDLER and _cmsWrite15Fixed16Number.
- Test true/false branches, edge cases, and order/number of invocations.
- Tests are written in C++11, but exercise a C-style focal function (extern "C" linkage).

Notes about the test harness:
- We provide a test-friendly mock of _cmsWrite15Fixed16Number that records written values and can simulate failure for a sentinel value.
- We assume the real WriteDoubles is linked from cmstypes.c; in this test, we provide a compatible extern "C" prototype for it.
- The mock's behavior:
  - For any value != sentinel, it records the value and returns TRUE.
  - If value equals the sentinel (here 13.37), it returns FALSE to simulate a write failure mid-stream.
- This approach gives coverage for the true/false branches and the loop's early exit.

Code structure:
- Lightweight testing framework embedded in the file (no external testing libraries).
- Three test cases:
  1) test_WriteDoubles_BasicWrites: ensures all values are written in order and function returns TRUE.
  2) test_WriteDoubles_StopsOnFailure: ensures on first failure, the function returns FALSE and stops.
  3) test_WriteDoubles_ZeroElements: ensures n == 0 returns TRUE and writes nothing.

Usage:
- Compile this test together with the implementation cmstypes.c (or a compatible translation unit providing WriteDoubles).
- Ensure the linker selects the test-provided _cmsWrite15Fixed16Number (see note in comments). In many setups, you may need to provide a weak alias or adjust link order so that the test's mock is used.

Compilation caveats:
- The test defines types and prototypes compatible with the focal function’s expected declarations. If your build environment uses a different typedef for cmsBool/cmsUInt32Number/cmsFloat64Number, align the definitions accordingly.
- If your environment already provides _cmsWrite15Fixed16Number, you may need to link with a weak-symbol alternative/replacement strategy to ensure the mock is used for testing.

Now the code:

*/

#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cmath>


// Minimal type definitions to match the focal method's interface.
// These mirror typical cms types used by the LittleCMS library.
// If your environment provides these in a header, replace with those definitions.

typedef int cmsBool;
#define TRUE 1
#define FALSE 0

typedef unsigned int cmsUInt32Number;
typedef double cmsFloat64Number;

// Forward declaration of the IO handler used by the focal function.
typedef struct cmsIOHANDLER_struct cmsIOHANDLER;

// Declaration of the focal function (C linkage).
extern "C" cmsBool WriteDoubles(cmsIOHANDLER* io, cmsUInt32Number n, cmsFloat64Number* Values);

// Mock/proxy for the _cmsWrite15Fixed16Number function used by WriteDoubles.
// In a real build, this symbol would be provided by the cms library.
// For unit testing, we provide our own implementation to capture inputs and simulate success/failure.
extern "C" cmsBool _cmsWrite15Fixed16Number(cmsIOHANDLER* io, cmsFloat64Number value);

// Simple test IO handler to capture written values.
struct cmsIOHANDLER_struct {
    std::vector<cmsFloat64Number> written; // captures values passed to _cmsWrite15Fixed16Number
};

// Global sentinel value to simulate a write failure in tests that need it.
const cmsFloat64Number SENTINEL_FAILURE = 13.37;

// Implementation of the mocked _cmsWrite15Fixed16Number.
// - Records the value into io->written.
// - Returns FALSE if value equals SENTINEL_FAILURE to simulate a failure.
// - Returns TRUE otherwise.
extern "C" cmsBool _cmsWrite15Fixed16Number(cmsIOHANDLER* io, cmsFloat64Number value)
{
    if (!io) return FALSE;
    // Simulate failure for a specific sentinel value to exercise false-branch behavior.
    if (value == SENTINEL_FAILURE) {
        return FALSE;
    }
    // Track the value written for verification in tests.
    io->written.push_back(value);
    return TRUE;
}

// Forward declaration for the tests to run without needing real cmstypes.c linkage issues.
// The actual implementation should be linked from cmstypes.c (the focal method).
// We assume the prototype matches: cmsBool WriteDoubles(cmsIOHANDLER*, cmsUInt32Number, cmsFloat64Number*);

extern "C" cmsBool WriteDoubles(cmsIOHANDLER* io, cmsUInt32Number n, cmsFloat64Number* Values);

// Lightweight test framework (no external dependencies)
struct TestResult {
    std::string name;
    bool passed;
    std::string details;
};

static void logResult(std::vector<TestResult>& results, const TestResult& r) {
    results.push_back(r);
}

// Helper to compare two vectors of cmsFloat64Number with exact equality (sufficient for test doubles).
static bool vectorsEqual(const std::vector<cmsFloat64Number>& a, const std::vector<cmsFloat64Number>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Test 1: Basic writes - ensure all values are written in order and function returns TRUE.
static bool test_WriteDoubles_BasicWrites() {
    cmsIOHANDLER io;
    io.written.clear();

    cmsFloat64Number values[5] = {0.0, 1.5, -2.25, 3.14, 100.0};
    cmsUInt32Number n = 5;

    cmsBool result = WriteDoubles(&io, n, values);

    bool pass = (result == TRUE);
    std::vector<cmsFloat64Number> expected = {0.0, 1.5, -2.25, 3.14, 100.0};
    if (!vectorsEqual(io.written, expected)) pass = false;

    if (!pass) {
        // Detailed failure explanation
        std::cout << "[FAIL] test_WriteDoubles_BasicWrites: "
                  << "result=" << (result==TRUE ? "TRUE" : "FALSE")
                  << ", expected TRUE and values written in order.\n";
    }
    if (pass) {
        std::cout << "[PASS] test_WriteDoubles_BasicWrites\n";
    }
    return pass;
}

// Test 2: Stop on failure - ensure that if _cmsWrite15Fixed16Number returns FALSE (sentinel),
// WriteDoubles stops early and returns FALSE, with only previous values written.
static bool test_WriteDoubles_StopsOnFailure() {
    cmsIOHANDLER io;
    io.written.clear();

    // Arrange: values where the second value is the sentinel that triggers failure.
    // Expectation: First value is written, second write fails, function returns FALSE.
    cmsFloat64Number values[3] = {10.0, SENTINEL_FAILURE, 20.0};
    cmsUInt32Number n = 3;

    cmsBool result = WriteDoubles(&io, n, values);

    bool pass = (result == FALSE);
    // Only the first value should have been written
    std::vector<cmsFloat64Number> expected = {10.0};
    if (!vectorsEqual(io.written, expected)) pass = false;

    if (!pass) {
        std::cout << "[FAIL] test_WriteDoubles_StopsOnFailure: "
                  << "result=" << (result==TRUE ? "TRUE" : "FALSE")
                  << ", expected FALSE and only first value written.\n";
    } else {
        std::cout << "[PASS] test_WriteDoubles_StopsOnFailure\n";
    }
    return pass;
}

// Test 3: Zero elements - ensure n == 0 returns TRUE and writes nothing.
static bool test_WriteDoubles_ZeroElements() {
    cmsIOHANDLER io;
    io.written.clear();

    cmsUInt32Number n = 0;
    cmsFloat64Number values[1] = {42.0}; // should be ignored

    cmsBool result = WriteDoubles(&io, n, values);

    bool pass = (result == TRUE);
    if (!io.written.empty()) {
        pass = false;
    }
    if (!pass) {
        std::cout << "[FAIL] test_WriteDoubles_ZeroElements: "
                  << "result=" << (result==TRUE ? "TRUE" : "FALSE")
                  << ", expected TRUE and no writes.\n";
    } else {
        std::cout << "[PASS] test_WriteDoubles_ZeroElements\n";
    }
    return pass;
}

// Main entry for running tests
int main() {
    std::vector<TestResult> results;

    bool b1 = test_WriteDoubles_BasicWrites();
    logResult(results, {"test_WriteDoubles_BasicWrites", b1, ""});

    bool b2 = test_WriteDoubles_StopsOnFailure();
    logResult(results, {"test_WriteDoubles_StopsOnFailure", b2, ""});

    bool b3 = test_WriteDoubles_ZeroElements();
    logResult(results, {"test_WriteDoubles_ZeroElements", b3, ""});

    // Summary
    int passed = 0;
    for (const auto& r : results) if (r.passed) ++passed;
    std::cout << "\nTest Summary: " << passed << " / " << results.size() << " passed.\n";

    return (passed == (int)results.size()) ? 0 : 1;
}

/* 
Explanatory notes for reviewers:
- The tests mock _cmsWrite15Fixed16Number via a simple C proxy function that records the values written to an io instance and can simulate a failure by returning FALSE when the value equals SENTINEL_FAILURE.
- Test 1 validates the success path for a typical non-edge case with multiple values; it asserts:
  - The function returns TRUE.
  - All values were written in the exact same order as provided.
- Test 2 validates the early exit behavior:
  - The function returns FALSE after the first failure.
  - Only values preceding the failure were written.
- Test 3 validates the edge case where n == 0:
  - The function returns TRUE.
  - No values were written.
- This test suite uses a minimal, self-contained approach suitable for environments without GoogleTest or GMock.
- If your build uses a different signature for _cmsWrite15Fixed16Number or cmstypes.h, adjust the typedefs and the mock accordingly.
*/