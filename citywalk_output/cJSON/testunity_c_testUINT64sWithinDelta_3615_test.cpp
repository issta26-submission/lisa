// This test suite targets the focal method testUINT64sWithinDelta
// located in testunity.c. We keep the tests lightweight and compatible
// with Unity's C framework while providing a C++11 test-friendly runner.
// The suite verifies 64-bit within-delta behavior (when 64-bit support is available)
// and exercises the focal test via an external entry point.

// Notes on terminology (from Step 1):
// - Candidate Keywords include: TEST_ASSERT_UINT64_WITHIN, UNITY_SUPPORT_64, TEST_IGNORE,
//   EXPECT_ABORT_BEGIN, VERIFY_FAILS_END, and the focal function testUINT64sWithinDelta.
// - The 64-bit path is conditionally compiled when UNITY_SUPPORT_64 is defined.

#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal test under test. This function is defined in testunity.c.
// It uses Unity's 64-bit assertion macros when UNITY_SUPPORT_64 is available.
void testUINT64sWithinDelta(void);
#ifdef __cplusplus
}
#endif

// Forward declaration for a small wrapper test to invoke the focal test.
// This wrapper allows the focal test to be executed as part of a Unity test run.
static void testWrapper_testUINT64sWithinDelta(void);

// Optional direct 64-bit usage tests (only compiled when 64-bit support exists).
// These tests exercise the 64-bit assertion macros independently of the focal function.
// They help ensure coverage of the 64-bit code path on platforms where 64-bit support is available.
#ifdef UNITY_SUPPORT_64

// This test directly uses the 64-bit within-delta macro to verify expected behavior.
// It mirrors the scenarios exercised inside the focal test to increase coverage
// of the 64-bit code path.
static void testDirectUint64WithinDelta_Pass(void)
{
    // True positives: within the allowed delta for 64-bit values
    TEST_ASSERT_UINT64_WITHIN(1, 0x7FFFFFFFFFFFFFFFULL, 0x7FFFFFFFFFFFFFFEULL);
    TEST_ASSERT_UINT64_WITHIN(5, 5000ULL, 4996ULL);
    TEST_ASSERT_UINT64_WITHIN(5, 5000ULL, 5005ULL);
}

// This test would verify a negative/false path for 64-bit within-delta.
// It is optional in this standalone runner since Unity's failure path relies
// on the test framework's abort handling. We omit an explicit abort-check here
// to keep the runner self-contained and avoid dependencies on spy/abort plumbing
// that may not exist in all builds.
// If the environment supports EXPECT_ABORT_BEGIN/VERIFY_FAILS_END, these could be added.
static void testDirectUint64WithinDelta_BoundaryEquals(void)
{
    // Boundary case: identical values should trivially pass with delta 0
    TEST_ASSERT_UINT64_WITHIN(0, 0x123456789ABCDEF0ULL, 0x123456789ABCDEF0ULL);
}

// Wrapper to expose a consistent test name for extra direct 64-bit tests
static void testDirectUint64WithinDelta_All(void)
{
    testDirectUint64WithinDelta_Pass();
    testDirectUint64WithinDelta_BoundaryEquals();
}

#endif // UNITY_SUPPORT_64

// Implementation of the wrapper that calls the focal test.
// This ensures the focal test is exercised within Unity's test harness.
static void testWrapper_testUINT64sWithinDelta(void)
{
    // The focal test is defined in testunity.c and contains several 64-bit asserts.
    // Invoking it here allows us to verify its behavior as part of the suite.
    testUINT64sWithinDelta();
}

// If 64-bit support exists, expose additional tests to run under the Unity harness.
#ifdef UNITY_SUPPORT_64
static void testWrapper_testUINT64sWithinDelta_DirectAll(void)
{
    testDirectUint64WithinDelta_All();
}
#endif

// Standard Unity test runner: main() entry.
// This runner uses RUN_TEST to execute the wrappers.
// It is designed to be compiled as a standalone test executable in environments
// where testunity.c may not provide a runner, or where a separate runner is desired.

int main(void)
{
    UNITY_BEGIN();

    // Run the focal test via a wrapper to ensure proper integration with Unity.
    RUN_TEST(testWrapper_testUINT64sWithinDelta);

#ifdef UNITY_SUPPORT_64
    // Additional direct 64-bit tests to cover the 64-bit code path explicitly.
    // These run only when 64-bit support is available.
    RUN_TEST(testDirectUint64WithinDelta_Pass);
    // Note: We include a boundary test as part of the 64-bit coverage.
    RUN_TEST(testDirectUint64WithinDelta_BoundaryEquals);
    // Run the aggregate direct tests wrapper if available.
    RUN_TEST(testDirectUint64WithinDelta_All);
#endif

    // End Unity test run and return the number of failures.
    return UNITY_END();
}