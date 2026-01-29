// Test suite for sqlite3MallocSize (focal method) in malloc.c
// This file provides a small, self-contained C++11 test harness (no GTest) to verify
// the behavior of sqlite3MallocSize. The tests are written to be executable
// by a standard C++11 compiler and linked with the SQLite-like C code described
// in the provided dependencies.
//
// Step 1 - Program Understanding (high level, embedded as comments)
// - Focal method: sqlite3MallocSize(const void *p)
//     - Asserts that the pointer p has MEMTYPE_HEAP via sqlite3MemdebugHasType(p, MEMTYPE_HEAP)
//     - Returns the size of the allocation by calling sqlite3GlobalConfig.m.xSize((void*)p)
// - Core dependencies observed from the provided code snippet:
//     - MEMTYPE_HEAP, sqlite3MemdebugHasType, sqlite3GlobalConfig.m.xSize
//     - sqlite3MallocInit, sqlite3Malloc, sqlite3_free (to obtain and release heap blocks)
// - Candidate Keywords to guide test coverage:
//     MEMTYPE_HEAP, sqlite3MemdebugHasType, sqlite3GlobalConfig, m, xSize, sqlite3Malloc, sqlite3_free
// - Test intent: validate that sqlite3MallocSize returns the same value as xSize for heap allocations
// - Note: True/false branches for the assert cannot be cleanly exercised without modifying library
//         (the assert would abort). Therefore tests focus on the true-path behavior and consistency
//         with xSize, and additional checks that the returned size is at least the requested size.

// Step 2 & 3 - Unit Test Generation and Refinement
// The tests below are written as plain C++ functions with a tiny test harness.
// They rely on the following external C interfaces (provided by the focal class dependencies):
//   - void *sqlite3Malloc(u64 n);
//   - void sqlite3_free(void *p);
//   - int sqlite3MallocSize(const void *p);
//   - void sqlite3MallocInit(void);
//   - extern sqlite3GlobalConfig sqlite3GlobalConfig;  // and its member: m.xSize
//   - int MEMTYPE_HEAP;  // used by sqlite3MemdebugHasType in the assert
// The tests are designed to be non-terminating (they do not abort on failure) and report results.

// We include necessary C headers via sqliteInt.h to obtain correct declarations.
#include <cstdio>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


extern "C" {
}

// Simple, non-terminating test harness
static int g_tests_run = 0;
static int g_tests_passed = 0;

// Utility to log a test result
static void log_result(const char* testName, bool passed, const char* message = nullptr) {
    ++g_tests_run;
    if (passed) {
        ++g_tests_passed;
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName;
        if (message) std::cout << " - " << message;
        std::cout << std::endl;
    }
}

// Test 1: Verify that sqlite3MallocSize(p) matches sqlite3GlobalConfig.m.xSize(p) for a variety of heap allocations.
// This ensures the focal method's primary contract is fulfilled when a heap pointer is provided.
static bool test_sqlite3MallocSize_matches_xSize_for_various_allocations() {
    const int testSizes[] = { 1, 2, 7, 16, 64, 128, 256, 1024, 4096 };
    const int numTests = sizeof(testSizes) / sizeof(testSizes[0]);
    bool all_ok = true;

    // Initialize the malloc subsystem to ensure sqlite3GlobalConfig is ready.
    sqlite3MallocInit();

    for (int i = 0; i < numTests; ++i) {
        int n = testSizes[i];
        void *p = sqlite3Malloc((u64)n);
        if (p == nullptr) {
            log_result("test_sqlite3MallocSize_matches_xSize_for_various_allocations",
                       false,
                       ("Allocation failed for n=" + std::to_string(n)).c_str());
            all_ok = false;
            continue;
        }

        // Core assertion under test: sqlite3MallocSize(p) should equal xSize(p)
        int size_from_api = sqlite3MallocSize(p);
        int expected_size = sqlite3GlobalConfig.m.xSize(p);

        if (size_from_api != expected_size) {
            log_result("test_sqlite3MallocSize_matches_xSize_for_various_allocations",
                       false,
                       ("Mismatch for n=" + std::to_string(n) +
                        ": sqlite3MallocSize=" + std::to_string(size_from_api) +
                        ", xSize=" + std::to_string(expected_size)).c_str());
            all_ok = false;
        }

        // Additional safeguard: allocated size should be at least the requested size
        if (size_from_api < n) {
            log_result("test_sqlite3MallocSize_matches_xSize_for_various_allocations",
                       false,
                       ("Allocated size " + std::to_string(size_from_api) +
                        " is less than requested " + std::to_string(n)).c_str());
            all_ok = false;
        }

        sqlite3_free(p);
    }

    return all_ok;
}

// Test 2: Validate that multiple allocations of varying sizes behave consistently in terms of sqlite3MallocSize.
// This checks internal bookkeeping consistency when several blocks are allocated and freed.
static bool test_sqlite3MallocSize_consistency_across_multiple_allocs() {
    sqlite3MallocInit();

    const int A = 24;
    const int B = 128;
    const int C = 1023;

    void *pA = sqlite3Malloc((u64)A);
    void *pB = sqlite3Malloc((u64)B);
    void *pC = sqlite3Malloc((u64)C);

    bool ok = true;

    if (!pA || !pB || !pC) {
        log_result("test_sqlite3MallocSize_consistency_across_multiple_allocs",
                   false,
                   "Failed to allocate one or more blocks");
        ok = false;
    } else {
        int sA = sqlite3MallocSize(pA);
        int sB = sqlite3MallocSize(pB);
        int sC = sqlite3MallocSize(pC);

        int eA = sqlite3GlobalConfig.m.xSize(pA);
        int eB = sqlite3GlobalConfig.m.xSize(pB);
        int eC = sqlite3GlobalConfig.m.xSize(pC);

        if (sA != eA || sB != eB || sC != eC) {
            log_result("test_sqlite3MallocSize_consistency_across_multiple_allocs",
                       false,
                       ("Mismatch in sizes: A(" + std::to_string(sA) + " vs " + std::to_string(eA) +
                        "), B(" + std::to_string(sB) + " vs " + std::to_string(eB) +
                        "), C(" + std::to_string(sC) + " vs " + std::to_string(eC) + ")").c_str());
            ok = false;
        }

        if (sA < A || sB < B || sC < C) {
            log_result("test_sqlite3MallocSize_consistency_across_multiple_allocs",
                       false,
                       "One or more sizes smaller than requested");
            ok = false;
        }
    }

    if (pA) sqlite3_free(pA);
    if (pB) sqlite3_free(pB);
    if (pC) sqlite3_free(pC);

    return ok;
}

// Test 3: Large allocation test to ensure sqlite3MallocSize handles large blocks without overflow or crashes.
// This ensures behavior across boundary conditions of allocator sizing.
static bool test_sqlite3MallocSize_large_allocation_behaviour() {
    sqlite3MallocInit();

    // A reasonably large size; adjust if the environment has memory constraints.
    const size_t large_n = 1 << 20; // 1 MiB
    void *p = sqlite3Malloc((u64)large_n);
    if (!p) {
        log_result("test_sqlite3MallocSize_large_allocation_behaviour",
                   false,
                   "Allocation failed for large_n");
        return false;
    }

    int size_from_api = sqlite3MallocSize(p);
    int expected = sqlite3GlobalConfig.m.xSize(p);

    bool ok = true;
    if (size_from_api != expected) {
        log_result("test_sqlite3MallocSize_large_allocation_behaviour",
                   false,
                   ("Mismatch: sqlite3MallocSize=" + std::to_string(size_from_api) +
                    ", xSize=" + std::to_string(expected)).c_str());
        ok = false;
    }

    if (size_from_api < (int)large_n) {
        log_result("test_sqlite3MallocSize_large_allocation_behaviour",
                   false,
                   ("Allocated size " + std::to_string(size_from_api) +
                    " is smaller than requested " + std::to_string(large_n)).c_str());
        ok = false;
    }

    sqlite3_free(p);
    return ok;
}

// Entry point
int main() {
    // Run tests and aggregate results
    bool t1 = test_sqlite3MallocSize_matches_xSize_for_various_allocations();
    if (!t1) {
        log_result("test_suite_overall", false, "Test 1 failed");
    } else {
        log_result("test_sqlite3MallocSize_matches_xSize_for_various_allocations", true);
    }

    bool t2 = test_sqlite3MallocSize_consistency_across_multiple_allocs();
    if (!t2) {
        log_result("test_suite_overall", false, "Test 2 failed");
    } else {
        log_result("test_sqlite3MallocSize_consistency_across_multiple_allocs", true);
    }

    bool t3 = test_sqlite3MallocSize_large_allocation_behaviour();
    if (!t3) {
        log_result("test_suite_overall", false, "Test 3 failed");
    } else {
        log_result("test_sqlite3MallocSize_large_allocation_behaviour", true);
    }

    // Summary
    std::cout << "Test results: " << g_tests_passed << " / " << g_tests_run << " tests passed." << std::endl;
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}