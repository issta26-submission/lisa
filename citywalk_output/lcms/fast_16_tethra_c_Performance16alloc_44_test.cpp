// File: fast_float_internal.h
// Minimal stub header to satisfy the focal file's dependencies for testing.
// This header is crafted solely for unit-testing the Performance16alloc function
// in a controlled environment without pulling in the full project.

#ifndef FAST_FLOAT_INTERNAL_H
#define FAST_FLOAT_INTERNAL_H

#include <cstring>
#include <fast_float_internal.h>
#include <cstddef>
#include <iostream>
#include <stddef.h>
#include <cstdlib>


// Domain-specific typedefs (simplified for test purposes)
typedef int cmsContext;

// Forward declaration for interpolation parameters structure.
// The real project defines a larger struct; we only need a placeholder here.
typedef struct cmsInterpParams cmsInterpParams;

// The focal class/struct that is allocated by Performance16alloc.
// We mirror the relevant portion that the test needs to verify.
typedef struct Performance16Data {
    cmsContext ContextID;
    const cmsInterpParams* p;
} Performance16Data;

#endif // FAST_FLOAT_INTERNAL_H

// End of fast_float_internal.h


// File: test_performance16alloc.cpp
// Lightweight, self-contained unit tests for Performance16alloc().
// - Do not use Google Test or any other testing framework.
// - Use non-terminating assertions by collecting and reporting results in main.
// - Provide a mock _cmsMallocZero() to simulate allocation success and failure.


// Include the header that defines the required types used by Performance16alloc.
// This header is also included by the focal C file, ensuring layout compatibility.

// Forward declaration of the focal function (defined in fast_16_tethra.c).
extern "C" Performance16Data* Performance16alloc(cmsContext ContextID, const cmsInterpParams* p);

// Provide a C linkage mock for the internal allocator used by Performance16alloc.
// This mock will either allocate zeroed memory or simulate a failure based on a flag.
static bool g_failMalloc = false;

extern "C" void* _cmsMallocZero(cmsContext ContextID, size_t size)
{
    (void)ContextID; // The test doesn't need to reason about ContextID here
    if (g_failMalloc) {
        return NULL; // Simulate allocation failure
    }
    // Allocate zero-initialized memory to mirror intended behavior.
    return std::calloc(1, size);
}

// Utility to reset global test state
static void resetTestState() {
    g_failMalloc = false;
}

// Test 1: Allocation succeeds and returns a valid pointer.
// Verifies that Performance16alloc returns a non-NULL pointer and that
// the produced object contains the expected ContextID and p fields.
static bool test_SuccessAlloc()
{
    // Arrange
    resetTestState();
    cmsContext testContext = 42;
    cmsInterpParams params;
    // Act
    Performance16Data* result = Performance16alloc(testContext, &params);
    // Assert (non-terminating: record result and return)
    bool passed = true;
    if (result == NULL) {
        std::cerr << "[FAIL] test_SuccessAlloc: expected non-NULL pointer, got NULL\n";
        passed = false;
    } else {
        // Validate that the struct fields were set by Performance16alloc.
        if (result->ContextID != testContext) {
            std::cerr << "[FAIL] test_SuccessAlloc: ContextID mismatch (expected "
                      << testContext << ", got " << result->ContextID << ")\n";
            passed = false;
        }
        if (result->p != &params) {
            std::cerr << "[FAIL] test_SuccessAlloc: pointer field 'p' mismatch\n";
            passed = false;
        }
    }

    // Cleanup
    if (result != NULL) {
        std::free(result);
    }

    return passed;
}

// Test 2: Allocation fails (simulated) and returns NULL.
// Verifies that Performance16alloc correctly handles allocation failure.
static bool test_FailAlloc()
{
    // Arrange: configure allocator to fail
    g_failMalloc = true;
    cmsContext testContext = 7;
    cmsInterpParams params;

    // Act
    Performance16Data* result = Performance16alloc(testContext, &params);

    // Assert
    bool passed = (result == NULL);
    if (!passed) {
        std::cerr << "[FAIL] test_FailAlloc: expected NULL on allocation failure\n";
        // If a non-NULL pointer is returned, report diagnostic
        if (result != NULL) {
            // Best effort diagnostic
            std::cerr << "       Received non-NULL pointer despite simulated failure.\n";
        }
    }

    // No need to free since result should be NULL
    return passed;
}

// Main test runner: executes tests and prints a concise summary.
// Follows the directive to call test methods from main and not terminate on first failure.
int main()
{
    int totalTests = 0;
    int failedTests = 0;

    std::cout << "Running Performance16alloc unit tests...\n";

    if (test_SuccessAlloc()) {
        std::cout << "[PASS] test_SuccessAlloc\n";
    } else {
        std::cout << "[FAIL] test_SuccessAlloc\n";
        ++failedTests;
    }
    ++totalTests;

    if (test_FailAlloc()) {
        std::cout << "[PASS] test_FailAlloc\n";
    } else {
        std::cout << "[FAIL] test_FailAlloc\n";
        ++failedTests;
    }
    ++totalTests;

    std::cout << "Summary: " << (totalTests - failedTests) << "/" << totalTests
              << " tests passed.\n";

    // Return non-zero if any test failed (non-terminating assertion pattern preserved)
    return failedTests == 0 ? 0 : 1;
}

// Notes:
// - The tests rely on the presence of the same struct layout defined in fast_float_internal.h.
// - We override _cmsMallocZero with a mock to simulate both success and failure branches.
// - We do not terminate on first failure to maximize coverage, per domain knowledge.
// - Static members or private internals of the focal implementation are not accessed directly.