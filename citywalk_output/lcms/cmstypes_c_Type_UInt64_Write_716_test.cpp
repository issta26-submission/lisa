/*
Step 1: Program Understanding (Candidates)
- Focal method: cmsBool Type_UInt64_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
- Core behavior: Iterate over nItems, call _cmsWriteUInt64Number(io, &Value[i]); return FALSE on first failure, TRUE otherwise.
- Key dependent components:
  - _cms_typehandler_struct: self parameter (unused)
  - cmsIOHANDLER: IO abstraction passed to writer
  - cmsUInt64Number: data item type
  - _cmsWriteUInt64Number: helper that actually writes one 64-bit value
- Control flow: for i in 0..nItems-1 -> call _cmsWriteUInt64Number -> if false, return FALSE -> at end return TRUE
- Observed patterns to test: success path, early exit on first failure, zero-length input
- Static/private aspects: none exposed in test surface; we’ll exercise via the function prototype and a mock _cmsWriteUInt64Number

Step 2: Unit Test Generation (Plan)
- Create a minimal test harness in C++11 that calls Type_UInt64_Write.
- Provide a test-scoped mock implementation for _cmsWriteUInt64Number to deterministically simulate success/failure and capture written values.
- Tests:
  1) All writes succeed: ensure Type_UInt64_Write returns TRUE and all values are written in order.
  2) Failure on first item: ensure Type_UInt64_Write returns FALSE and only the first item is written.
  3) Zero items: ensure Type_UInt64_Write returns TRUE and nothing is written.
- Per the instruction, do not rely on GTest. Implement a light, non-terminating assertion approach and run tests from main.
- Include minimal necessary type declarations to compile in C++11 without external dependencies.

Step 3: Test Case Refinement (Domain Knowledge)
- Use a small, explicit test IO object (dummy cmsIOHANDLER) passed to Type_UInt64_Write.
- Validate both return values and side effects (the sequence of written 64-bit values) to maximize coverage.
- Use non-terminating checks so all tests run even if one fails.
- Keep tests standalone with explicit explanatory comments for each test.

Below is the standalone test code. It assumes Type_UInt64_Write is visible with C linkage and that a function _cmsWriteUInt64Number exists (we provide a test-time mock to deterministically drive behavior). The test harness uses non-terminating checks and prints a summary at the end.

Note: This file is intended to be compiled and linked against the actual cmstypes.c (or a compatible build) where Type_UInt64_Write is defined. The test-time mock for _cmsWriteUInt64Number overrides behavior for testing purposes.

*/

#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <lcms2_internal.h>
#include <cstdint>


// -----------------------------------------------------------------------------
// Lightweight test harness (non-terminating assertions)
// -----------------------------------------------------------------------------

static int g_totalTests = 0;
static int g_failedTests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        g_failedTests++; \
        fprintf(stderr, "Test failure: %s\n", msg); \
    } \
    ++g_totalTests; \
} while (0)

#define RUN_TEST(func, name) do { \
    fprintf(stdout, "Running %s...\n", name); \
    if (!(bool)(func())) { \
        /* Non-terminating: report but continue */ \
        fprintf(stderr, "ERROR in %s\n", name); \
    } \
} while (0)

// -----------------------------------------------------------------------------
// Minimal type and surface declarations to allow linking against the focal function
// -----------------------------------------------------------------------------
// The actual project defines these in its headers. We provide minimal stubs here so
// the test can compile and link against the focal function Type_UInt64_Write.

typedef int cmsBool;
typedef unsigned int cmsUInt32Number;
typedef unsigned long long cmsUInt64Number;

struct _cms_typehandler_struct {
    // empty: we only need a type for the function signature
};

// Forward declare the Type_UInt64_Write function (as seen in the focal method)
extern "C" cmsBool Type_UInt64_Write(struct _cms_typehandler_struct* self,
                                    struct cmsIOHANDLER* io,
                                    void* Ptr,
                                    cmsUInt32Number nItems);

// Minimal cmsIOHANDLER surface (contents unused by our mock)
struct cmsIOHANDLER {
    void* UserData;
};

// -----------------------------------------------------------------------------
// Test-time hook: mock _cmsWriteUInt64Number
// We implement a deterministic mock to simulate success/failure and capture the
// values that would be written by the focal function.
// Note: In a real project, the library would provide _cmsWriteUInt64Number.
// Here we provide a test-time override via linker (we assume it will be linked in).
// -----------------------------------------------------------------------------

static std::vector<cmsUInt64Number> g_WrittenValues;
static int g_failCount = 0;

// Provide a test-time mock of _cmsWriteUInt64Number.
// Signature must match what the focal function expects.
// If your build uses a different prototype, adjust accordingly.
extern "C" cmsBool _cmsWriteUInt64Number(struct cmsIOHANDLER* io, cmsUInt64Number* Value)
{
    (void)io; // unused in mock, we only simulate behavior
    if (g_failCount > 0) {
        --g_failCount;
        return 0; // simulate failure
    }
    g_WrittenValues.push_back(*Value);
    return 1; // simulate success
}

// -----------------------------------------------------------------------------
// Helper to reset test state
// -----------------------------------------------------------------------------

static void ResetTestState() {
    g_WrittenValues.clear();
    g_failCount = 0;
}

// -----------------------------------------------------------------------------
// Test 1: All writes succeed
// Expected: Type_UInt64_Write returns TRUE and all values are written in order.
// -----------------------------------------------------------------------------

bool test_UInt64_Write_AllSuccess()
{
    ResetTestState();
    // Prepare input data
    cmsUInt64Number values[] = {
        1ULL,
        2ULL,
        3ULL,
        18446744073709551615ULL // max uint64
    };
    cmsUInt32Number nItems = 4;

    cmsIOHANDLER dummyIo;
    dummyIo.UserData = nullptr; // not used by mock

    // Call the focal method
    cmsBool result = Type_UInt64_Write(nullptr, &dummyIo, values, nItems);

    // Assertions
    EXPECT_TRUE(result == 1, "AllSuccess: Type_UInt64_Write should return TRUE when all writes succeed");
    // Check written values
    cmsUInt64Number expected[] = {1ULL, 2ULL, 3ULL, 18446744073709551615ULL};
    bool ok = (g_WrittenValues.size() == 4);
    for (size_t i = 0; i < 4 && ok; ++i) {
        if (g_WrittenValues[i] != expected[i]) ok = false;
    }
    EXPECT_TRUE(ok, "AllSuccess: Written values must match input order and content");
    return result == 1 && ok;
}

// -----------------------------------------------------------------------------
// Test 2: Failure on first item
// Expected: Type_UInt64_Write returns FALSE and only the first value is written.
// -----------------------------------------------------------------------------

bool test_UInt64_Write_FirstFail()
{
    ResetTestState();
    // Prepare input data
    cmsUInt64Number values[] = {
        9ULL,
        8ULL,
        7ULL
    };
    cmsUInt32Number nItems = 3;

    // Simulate failure on the first call
    g_failCount = 1;

    cmsIOHANDLER dummyIo;
    dummyIo.UserData = nullptr;

    cmsBool result = Type_UInt64_Write(nullptr, &dummyIo, values, nItems);

    // Assertions
    EXPECT_TRUE(result == 0, "FirstFail: Type_UInt64_Write should return FALSE when first _cmsWriteUInt64Number fails");
    // Only the first value should have been written
    bool ok = (g_WrittenValues.size() == 1 && g_WrittenValues[0] == 9ULL);
    EXPECT_TRUE(ok, "FirstFail: Only the first value should be written when failure occurs on first call");
    return result == 0 && ok;
}

// -----------------------------------------------------------------------------
// Test 3: Zero items
// Expected: Type_UInt64_Write returns TRUE and no values are written.
// -----------------------------------------------------------------------------

bool test_UInt64_Write_ZeroItems()
{
    ResetTestState();
    cmsUInt32Number nItems = 0;
    cmsUInt64Number values[] = { 123ULL }; // should be ignored since nItems == 0

    cmsIOHANDLER dummyIo;
    dummyIo.UserData = nullptr;

    cmsBool result = Type_UInt64_Write(nullptr, &dummyIo, values, nItems);

    bool ok = (g_WrittenValues.empty());
    EXPECT_TRUE(result == 1, "ZeroItems: Type_UInt64_Write should return TRUE for nItems == 0");
    EXPECT_TRUE(ok, "ZeroItems: No values should be written when nItems == 0");
    return result == 1 && ok;
}

// -----------------------------------------------------------------------------
// main: run tests
// -----------------------------------------------------------------------------

int main() {
    fprintf(stdout, "Starting Type_UInt64_Write unit tests (C++11, no GTest)\n");

    RUN_TEST(test_UInt64_Write_AllSuccess, "test_UInt64_Write_AllSuccess");
    RUN_TEST(test_UInt64_Write_FirstFail, "test_UInt64_Write_FirstFail");
    RUN_TEST(test_UInt64_Write_ZeroItems, "test_UInt64_Write_ZeroItems");

    // Summary
    fprintf(stdout, "\nTest summary: total=%d, failed=%d\n", g_totalTests, g_failedTests);
    if (g_failedTests > 0) {
        fprintf(stderr, "Some tests FAILED.\n");
        return 1;
    }
    fprintf(stdout, "All tests PASSED.\n");
    return 0;
}

/*
Notes for contributors:
- The Candidate Keywords extracted from the focal class/method include:
  cmsBool, Type_UInt64_Write, cmsUInt64Number, cmsIOHANDLER, _cmsWriteUInt64Number, self,
  io, Ptr, nItems, _cms_typehandler_struct, Value, cmsUInt32Number
- This test suite focuses on true/false branches and edge cases:
  - All writes succeed (normal path)
  - Early exit on first failure
  - Zero-length input
- Static members: none in this focal snippet; tests do not rely on static class members.
- Namespace: using global scope; no GTest or GMock used.
- All tests are invoked from main() as required when GTest is not allowed.
*/