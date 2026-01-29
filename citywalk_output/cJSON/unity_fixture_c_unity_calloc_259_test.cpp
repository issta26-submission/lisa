// Candidate Keywords (Step 1): 
// - unity_malloc, malloc, NULL
// - memset
// - memory, mem
// - num, size, num * size
// - unity_free
// - UnityMalloc_StartTest, UnityMalloc_EndTest, UnityMalloc_MakeMallocFailAfterCount
// - allocation success/failure branches (mem != NULL vs mem == NULL)

#include <unity_internals.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unity_fixture.h>


extern "C" {
    // Focal function under test
    void* unity_calloc(size_t num, size_t size);

    // Unity memory helpers (from unity_fixture.c)
    void* unity_malloc(size_t size);
    void  unity_free(void* mem);

    // Test harness helpers (from unity_fixture.c)
    void UnityMalloc_StartTest(void);
    void UnityMalloc_EndTest(void);
    void UnityMalloc_MakeMallocFailAfterCount(int countdown);
}

// Simple non-terminating test harness (no GTest/GMock). Each test reports pass/fail
// and continues execution to maximize coverage (per Domain Knowledge guidance).

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Utility to log test results
static void log_test_result(const std::string& testName, bool passed) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << testName << std::endl;
    if (!passed) ++g_failed_tests;
    ++g_total_tests;
}

// Test 1: Verifies that unity_calloc allocates non-NULL memory and zero-initializes it
// This covers the true-branch of the mem != NULL path and the memset call.
bool test_unity_calloc_allocates_and_zeroes() {
    const size_t N = 8;
    const size_t SZ = 1; // 8 bytes total
    UnityMalloc_StartTest();

    void* mem = unity_calloc(N, SZ);
    bool ok = (mem != NULL);
    if (ok) {
        unsigned char* bytes = static_cast<unsigned char*>(mem);
        for (size_t i = 0; i < N * SZ; ++i) {
            if (bytes[i] != 0) {
                ok = false;
                break;
            }
        }
    }

    if (mem != NULL) {
        unity_free(mem); // free to avoid leaks
    }
    UnityMalloc_EndTest();

    log_test_result("unity_calloc allocates and zeroes memory", ok);
    return ok;
}

// Test 2: Verifies that unity_calloc returns NULL when memory allocation fails
// This covers the false-branch mem == NULL in the focal method.
bool test_unity_calloc_propagates_null_on_failure() {
    UnityMalloc_StartTest();
    // Force the next malloc to fail
    UnityMalloc_MakeMallocFailAfterCount(1);

    void* mem = unity_calloc(4, 4); // attempts 16 bytes
    bool ok = (mem == NULL);

    // Even in failure, EndTest should be called to reset internal state
    UnityMalloc_EndTest();

    log_test_result("unity_calloc returns NULL on allocation failure", ok);
    return ok;
}

// Entry point: run all tests and report summary
int main() {
    bool t1 = test_unity_calloc_allocates_and_zeroes();
    bool t2 = test_unity_calloc_propagates_null_on_failure();

    // Summary
    std::cout << "===============================" << std::endl;
    std::cout << "Total tests: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;
    std::cout << "===============================" << std::endl;

    // Return number of failed tests as exit code (non-zero indicates failure)
    return g_failed_tests;
}