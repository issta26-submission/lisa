// Test suite for cmsBool defMtxLock in cmserr.c
// - This test suite is written in C++11 without GoogleTest.
// - It includes the focal implementation cmserr.c and provides a small
//   test double for _cmsLockPrimitive to control the behavior of defMtxLock.
// - It uses non-terminating assertions (test continues after failures) and prints
//   a summary at the end.

#include <lcms2_internal.h>
#include <iostream>
#include <cmserr.c>


// -----------------------------------------------------------------------------
// Control of the focal function's dependency _cmsLockPrimitive
// We provide a small, predictable mock of the function _cmsLockPrimitive.
// The real implementation resides in the CMS/LittleCMS core; for unit tests we
// override it to drive the behavior of defMtxLock.
// We declare the function before including cmserr.c so that the compiler can
// resolve calls from defMtxLock to our mock.
// -----------------------------------------------------------------------------

static int g_lockReturn = 0; // 0 -> lock successful, non-zero -> lock failed

extern "C" int _cmsLockPrimitive(void* /*mtx*/) {
    // Return value controlled by test cases
    return g_lockReturn;
}

// -----------------------------------------------------------------------------
// Include the focal class/module (cmserr.c) containing defMtxLock.
// We rely on this inclusion so that defMtxLock is defined in this translation unit.
// The tests will manipulate g_lockReturn to drive deterministic behavior.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// -----------------------------------------------------------------------------

static int g_testFailures = 0;

static void expect_true(bool condition, const char* description) {
    if (condition) {
        std::cout << "[OK] " << description << std::endl;
    } else {
        std::cerr << "[FAIL] " << description << std::endl;
        ++g_testFailures;
    }
}

// Utility to run all tests
static void runAllTests() {
    // Prepare a dummy mutex buffer; we never dereference it in our mock
    alignas(alignof(void*)) char dummyBuf[1] = {0};
    void* mtxNonNull = (void*)dummyBuf;
    void* mtxNull = nullptr;

    // A generic cmsContext value (type comes from the included cmserr.c header)
    // We cast 0 to cmsContext to represent a neutral/context-agnostic value.
    cmsContext ctxNeutral = (cmsContext)0;
    cmsContext ctxDiffers = (cmsContext)12345; // ensure id value is ignored

    // Test 1: true branch - _cmsLockPrimitive returns 0
    // Expect defMtxLock to be true (non-zero) when lock succeeds (0)
    g_lockReturn = 0;
    bool res1 = defMtxLock(ctxNeutral, mtxNonNull);
    expect_true(res1 == true, "defMtxLock returns true when _cmsLockPrimitive returns 0 (lock success)");

    // Test 2: false branch - _cmsLockPrimitive returns non-zero
    // Expect defMtxLock to be false when lock fails (non-zero)
    g_lockReturn = 1;
    bool res2 = defMtxLock(ctxNeutral, mtxNonNull);
    expect_true(res2 == false, "defMtxLock returns false when _cmsLockPrimitive returns non-zero (lock failure)");

    // Test 3: id parameter is unused by defMtxLock (verify independence from id)
    // The result should depend solely on the lock primitive's return value
    g_lockReturn = 0;
    bool res3 = defMtxLock(ctxDiffers, mtxNonNull);
    expect_true(res3 == true, "defMtxLock ignores cmsContext/id value and relies on mtx primitive");

    // Test 4: mtx pointer is NULL; ensure no crash and behavior matches primitive return
    g_lockReturn = 0;
    bool res4 = defMtxLock(ctxNeutral, mtxNull);
    expect_true(res4 == true, "defMtxLock handles NULL mtx pointer when primitive returns 0");

    // Summary
    std::cout << "Test summary: " << (g_testFailures == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED")
              << std::endl;
}

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

int main() {
    runAllTests();
    return g_testFailures; // non-zero on failure
}