/*
 * Test Suite for defMtxUnlock (cmserr.c)
 *
 * This test harness targets the focal method:
 *   void defMtxUnlock(cmsContext id, void* mtx)
 * by providing a lightweight test double for the dependency:
 *   _cmsUnlockPrimitive(_cmsMutex* m)
 *
 * Strategy:
 * - Provide a C-compatible stub for _cmsUnlockPrimitive and a minimal _cmsMutex type.
 * - Call defMtxUnlock with various mtx pointers (valid, NULL) and a dummy cmsContext id.
 * - Verify that _cmsUnlockPrimitive is invoked exactly once per call and that the pointer passed
 *   to it matches the original mtx argument, demonstrating the correct forwarding behavior.
 * - Use non-terminating checks (not aborting on failure) and report a succinct summary.
 *
 * Notes:
 * - We assume the real project provides defMtxUnlock and cmsContext types. We declare the
 *   necessary symbols to link against the project's implementation.
 * - This test does not rely on GoogleTest; it uses a simple main-based harness with
 *   explicit pass/fail reporting.
 * - Static analysis and coverage are improved by multiple scenarios including NULL pointers.
 */

/* Domain knowledge: Use only C++ standard library and provide non-terminating assertions
   via manual failure counters rather than terminating the test abruptly. */

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cstddef>


// Forward declaration of the types used by the focal function.
// We keep them minimal to avoid requiring the full library headers.
typedef void* cmsContext;

// The _cmsMutex type is opaque for our test double; we only need a pointer to pass through.
struct _cmsMutex {};

// The focal function to be tested is expected to use C linkage.
// We declare it here to ensure we can link against the project's implementation.
extern "C" void defMtxUnlock(cmsContext id, void* mtx);

// Test doubles (stubs) for library internals used by defMtxUnlock.
// Implement _cmsUnlockPrimitive to capture its input for verification.
static void* capturedMutexPtr = nullptr;
static int unlockPrimitiveCallCount = 0;

// C linkage to match the expected symbol in the CMSS err library.
// The real code will cast the provided mtx to _cmsMutex* and pass to this function.
extern "C" void _cmsUnlockPrimitive(struct _cmsMutex* m)
{
    capturedMutexPtr = static_cast<void*>(m);
    ++unlockPrimitiveCallCount;
}

/*
 * Test 1: Verify that defMtxUnlock forwards the exact mtx pointer
 * to _cmsUnlockPrimitive. This ensures the wrapper does not modify the
 * pointer and properly delegates unlocking.
 */
bool test_defMtxUnlock_forwards_given_pointer()
{
    // Reset test state
    capturedMutexPtr = nullptr;
    unlockPrimitiveCallCount = 0;

    // Prepare a dummy mutex and an arbitrary context id
    int dummyMutex;
    cmsContext testContext = nullptr;

    // Act
    defMtxUnlock(testContext, &dummyMutex);

    // Assert (non-terminating): ensure one call and correct pointer forwarding
    bool passed = (unlockPrimitiveCallCount == 1) && (capturedMutexPtr == &dummyMutex);

    // Return result for harness
    return passed;
}

/*
 * Test 2: Verify that defMtxUnlock handles NULL mtx gracefully by forwarding NULL
 * to _cmsUnlockPrimitive and still performing exactly one call.
 */
bool test_defMtxUnlock_forward_null_pointer()
{
    // Reset test state
    capturedMutexPtr = reinterpret_cast<void*>(0xDEADBEEF); // sentinel
    unlockPrimitiveCallCount = 0;

    // Act: pass NULL as mtx
    defMtxUnlock(nullptr, nullptr);

    // Assert: one call and NULL forwarded
    bool passed = (unlockPrimitiveCallCount == 1) && (capturedMutexPtr == nullptr);

    return passed;
}

/*
 * Test 3: Verify repeated invocations produce independent forwarding with each call.
 * This checks that the forwarding is consistent across multiple calls.
 */
bool test_defMtxUnlock_multiple_calls_forward_unique_pointers()
{
    // First call setup
    capturedMutexPtr = nullptr;
    unlockPrimitiveCallCount = 0;

    int mutexA;
    defMtxUnlock(nullptr, &mutexA);
    bool firstCallOk = (unlockPrimitiveCallCount == 1) && (capturedMutexPtr == &mutexA);

    // Second call setup
    capturedMutexPtr = nullptr;
    unlockPrimitiveCallCount = 0;
    int mutexB;
    defMtxUnlock(nullptr, &mutexB);
    bool secondCallOk = (unlockPrimitiveCallCount == 1) && (capturedMutexPtr == &mutexB);

    return firstCallOk && secondCallOk;
}

/*
 * Simple test harness
 * - Runs the tests and reports results.
 * - Uses non-terminating checks; continues even if a test fails to gather more information.
 */
int main()
{
    int failures = 0;

    // Run Test 1
    if (!test_defMtxUnlock_forwards_given_pointer()) {
        std::cerr << "[FAIL] test_defMtxUnlock_forwards_given_pointer\n";
        ++failures;
    } else {
        std::cout << "[PASS] test_defMtxUnlock_forwards_given_pointer\n";
    }

    // Run Test 2
    if (!test_defMtxUnlock_forward_null_pointer()) {
        std::cerr << "[FAIL] test_defMtxUnlock_forward_null_pointer\n";
        ++failures;
    } else {
        std::cout << "[PASS] test_defMtxUnlock_forward_null_pointer\n";
    }

    // Run Test 3
    if (!test_defMtxUnlock_multiple_calls_forward_unique_pointers()) {
        std::cerr << "[FAIL] test_defMtxUnlock_multiple_calls_forward_unique_pointers\n";
        ++failures;
    } else {
        std::cout << "[PASS] test_defMtxUnlock_multiple_calls_forward_unique_pointers\n";
    }

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
    } else {
        std::cerr << failures << " TEST(S) FAILED\n";
    }

    // Return number of failed tests to aid automation
    return failures;
}