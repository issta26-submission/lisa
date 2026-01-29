// test_myMtxLock.cpp
// Unit tests for cmsBool MyMtxLock(cmsContext id, void* mtx)
// Follows the requested steps to generate a focused test suite without using GTest.

#include <vector>
#include <iostream>
#include <testcms2.h>
#include <cassert>
#include <cstdint>


// Include domain-specific CMS definitions (types like cmsBool, cmsContext, TRUE, etc.)
// This header is provided by the project (per <DOMAIN_KNOWLEDGE>), ensure the include path is correct.

// Ensure linkage compatibility with the focal C function.
// The function is implemented in C (likely in testplugin.c), so declare with C linkage.
extern "C" {
    cmsBool MyMtxLock(cmsContext id, void* mtx);
}

// Minimal replica of the expected MyMtx structure for tests.
// The focal code casts the provided pointer to MyMtx* and increments nlocks.
// We assume that the first member in the real MyMtx is nlocks.
// If the real layout differs, this test may need adjustment to reflect the actual struct.
struct MyMtx {
    int nlocks;
};

// Simple non-terminating assertion helper to maximize test coverage without exiting on failure.
static int g_totalTests = 0;
static int g_failedTests = 0;

#define TEST_LOG(msg) do { std::cout << "[TEST] " << msg << std::endl; } while (0)
#define ASSERT(cond, msg) do { \
    ++g_totalTests; \
    if (!(cond)) { \
        ++g_failedTests; \
        std::cerr << "Failure: " << (msg) \
                  << " | Function: " << __FUNCTION__ \
                  << " | Line: " << __LINE__ << std::endl; \
    } \
} while (0)

// Test 1: Basic single-increment behavior and return value for a freshly initialized mutex.
void test_MyMtxLock_IncrementsFromZero() {
    TEST_LOG("Test 1: MyMtxLock should increment nlocks from 0 to 1 and return TRUE");
    MyMtx mtx { 0 };
    cmsBool res = MyMtxLock(0, &mtx);

    // The function should return TRUE and increment nlocks to 1.
    ASSERT(res == TRUE, "MyMtxLock should return TRUE on success");
    ASSERT(mtx.nlocks == 1, "mtx.nlocks should be 1 after first lock");
}

// Test 2: Multiple consecutive locks should increment nlocks accordingly and keep returning TRUE.
void test_MyMtxLock_MultipleIncrements() {
    TEST_LOG("Test 2: MyMtxLock should increment nlocks with multiple calls and always return TRUE");
    MyMtx mtx { 0 };

    const int iterations = 7;
    for (int i = 0; i < iterations; ++i) {
        cmsBool res = MyMtxLock(0, &mtx);
        ASSERT(res == TRUE, "MyMtxLock should return TRUE on each call");
    }

    ASSERT(mtx.nlocks == iterations, "mtx.nlocks should equal the number of lock calls");
}

// Test 3: Verify that the id parameter is not used by the implementation (side-effect free with respect to id).
void test_MyMtxLock_IdParameterUnused() {
    TEST_LOG("Test 3: MyMtxLock should ignore the id parameter and only modify mtx.nlocks");
    MyMtx mtx { 0 };
    cmsBool res = MyMtxLock((cmsContext)12345, &mtx);

    ASSERT(res == TRUE, "MyMtxLock should return TRUE regardless of id value");
    ASSERT(mtx.nlocks == 1, "mtx.nlocks should be incremented despite id value");
}

// Test driver: run all tests and report final results.
int main() {
    TEST_LOG("Starting test suite for cmsBool MyMtxLock");

    test_MyMtxLock_IncrementsFromZero();
    test_MyMtxLock_MultipleIncrements();
    test_MyMtxLock_IdParameterUnused();

    // Summary
    std::cout << "Total tests run: " << g_totalTests << std::endl;
    std::cout << "Total failures: " << g_failedTests << std::endl;
    if (g_failedTests != 0) {
        std::cerr << "Some tests FAILED." << std::endl;
    } else {
        std::cout << "All tests PASSED." << std::endl;
    }

    // Return non-zero if any test failed.
    return (g_failedTests != 0) ? 1 : 0;
}