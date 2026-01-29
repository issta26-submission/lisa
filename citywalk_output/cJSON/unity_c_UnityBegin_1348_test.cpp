// Test suite for UnityBegin in unity.c using a lightweight, non-terminating test harness.
// This test is designed to run with an existing Unity framework (unity.c + unity.h) compiled
// in C++11 mode. It verifies that UnityBegin properly initializes/resets Unity's global state.

// Note: This file assumes the Unity test framework is available in the build environment.
// The tests avoid terminating on failure, instead collecting and reporting failures at the end.

#include <stddef.h>
#include <cstdio>
#include <cstddef>
#include <unity.h>


// Include the Unity C header in a way compatible with C++
extern "C" {
}

// Simple non-terminating test harness
static int g_total_failures = 0;

// Helper assertion macros (non-terminating)
#define ASSERT_EQ_INT(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        fprintf(stderr, "FAIL: %s | expected: %d, actual: %d\n", (msg), (int)(expected), (int)(actual)); \
        ++g_total_failures; \
    } else { \
        printf("PASS: %s\n", (msg)); \
    } \
} while (0)

#define ASSERT_PTR_EQ(expected_ptr, actual_ptr, msg) do { \
    if ((expected_ptr) != (actual_ptr)) { \
        fprintf(stderr, "FAIL: %s | expected pointer: %p, actual pointer: %p\n", (msg), (void*)(expected_ptr), (void*)(actual_ptr)); \
        ++g_total_failures; \
    } else { \
        printf("PASS: %s\n", (msg)); \
    } \
} while (0)

// Forward declarations for test cases to be called from main
static void test_UnityBegin_sets_initial_state(void);
static void test_UnityBegin_with_null_filename(void);
static void test_UnityBegin_resets_before_call(void);

// Entry point for the test suite
int main(void)
{
    printf("Starting UnityBegin test suite (non-terminating assertions)...\n");

    test_UnityBegin_sets_initial_state();
    test_UnityBegin_with_null_filename();
    test_UnityBegin_resets_before_call();

    if (g_total_failures == 0) {
        printf("All UnityBegin tests PASSED.\n");
    } else {
        printf("UnityBegin tests completed with %d failure(s).\n", g_total_failures);
    }

    // Return non-zero if any failures occurred
    return g_total_failures;
}

// Test 1: Verify UnityBegin correctly initializes all fields to starting values
// after a normal filename is provided. This ensures the function sets up the
// test harness state properly and resets counters to zero.
static void test_UnityBegin_sets_initial_state(void)
{
    // Arrange: Pre-fill Unity state with non-zero values to ensure UnityBegin overwrites them.
    // The Unity framework provides a global 'Unity' variable with these fields.
    const char* fname = "unit_test_file.c";

    Unity.TestFile = "previous_file.c";
    Unity.CurrentTestName = (const char*)0xDEADBEEF; // non-NULL to ensure reset to NULL
    Unity.CurrentTestLineNumber = 12345;
    Unity.NumberOfTests = 17;
    Unity.TestFailures = 3;
    Unity.TestIgnores = 2;
    Unity.CurrentTestFailed = 1;
    Unity.CurrentTestIgnored = 1;

    // Act
    UnityBegin(fname);

    // Assert
    ASSERT_PTR_EQ(fname, Unity.TestFile, "TestFile should be set to the provided filename pointer");
    ASSERT_PTR_EQ(NULL, Unity.CurrentTestName, "CurrentTestName should be reset to NULL by UnityBegin");
    ASSERT_EQ_INT(0, Unity.CurrentTestLineNumber, "CurrentTestLineNumber should be reset to 0 by UnityBegin");
    ASSERT_EQ_INT(0, Unity.NumberOfTests, "NumberOfTests should be reset to 0 by UnityBegin");
    ASSERT_EQ_INT(0, Unity.TestFailures, "TestFailures should be reset to 0 by UnityBegin");
    ASSERT_EQ_INT(0, Unity.TestIgnores, "TestIgnores should be reset to 0 by UnityBegin");
    ASSERT_EQ_INT(0, Unity.CurrentTestFailed, "CurrentTestFailed should be reset to 0 by UnityBegin");
    ASSERT_EQ_INT(0, Unity.CurrentTestIgnored, "CurrentTestIgnored should be reset to 0 by UnityBegin");
}

// Test 2: Verify UnityBegin handles a NULL filename gracefully.
// It should set TestFile to NULL and reset all counters as in the normal path.
static void test_UnityBegin_with_null_filename(void)
{
    // Arrange: set some non-zero state again to verify reset with NULL filename
    Unity.TestFile = "something.c";
    Unity.CurrentTestName = "SomeTest";
    Unity.CurrentTestLineNumber = 777;
    Unity.NumberOfTests = 9;
    Unity.TestFailures = 2;
    Unity.TestIgnores = 1;
    Unity.CurrentTestFailed = 1;
    Unity.CurrentTestIgnored = 0;

    // Act
    UnityBegin(NULL);

    // Assert
    ASSERT_PTR_EQ(NULL, Unity.TestFile, "TestFile should be NULL when UnityBegin is called with NULL filename");
    ASSERT_PTR_EQ(NULL, Unity.CurrentTestName, "CurrentTestName should be NULL after UnityBegin(NULL)");
    ASSERT_EQ_INT(0, Unity.CurrentTestLineNumber, "CurrentTestLineNumber should be reset to 0 by UnityBegin(NULL)");
    ASSERT_EQ_INT(0, Unity.NumberOfTests, "NumberOfTests should be reset to 0 by UnityBegin(NULL)");
    ASSERT_EQ_INT(0, Unity.TestFailures, "TestFailures should be reset to 0 by UnityBegin(NULL)");
    ASSERT_EQ_INT(0, Unity.TestIgnores, "TestIgnores should be reset to 0 by UnityBegin(NULL)");
    ASSERT_EQ_INT(0, Unity.CurrentTestFailed, "CurrentTestFailed should be reset to 0 by UnityBegin(NULL)");
    ASSERT_EQ_INT(0, Unity.CurrentTestIgnored, "CurrentTestIgnored should be reset to 0 by UnityBegin(NULL)");
}

// Test 3: Ensure UnityBegin resets existing state even if it was already reset.
// This confirms that UnityBegin performs a clean reinitialization regardless of prior state.
static void test_UnityBegin_resets_before_call(void)
{
    // Arrange: Leave Unity in an already-reset state to test idempotency
    Unity.TestFile = NULL;
    Unity.CurrentTestName = NULL;
    Unity.CurrentTestLineNumber = 0;
    Unity.NumberOfTests = 0;
    Unity.TestFailures = 0;
    Unity.TestIgnores = 0;
    Unity.CurrentTestFailed = 0;
    Unity.CurrentTestIgnored = 0;

    // Take a non-trivial filename
    const char* fname = "another_test.c";

    // Act
    UnityBegin(fname);

    // Assert
    ASSERT_PTR_EQ(fname, Unity.TestFile, "TestFile should be updated to the new filename on UnityBegin");
    ASSERT_PTR_EQ(NULL, Unity.CurrentTestName, "CurrentTestName should remain NULL after UnityBegin");
    ASSERT_EQ_INT(0, Unity.CurrentTestLineNumber, "CurrentTestLineNumber should be 0 after UnityBegin");
    ASSERT_EQ_INT(0, Unity.NumberOfTests, "NumberOfTests should be 0 after UnityBegin");
    ASSERT_EQ_INT(0, Unity.TestFailures, "TestFailures should be 0 after UnityBegin");
    ASSERT_EQ_INT(0, Unity.TestIgnores, "TestIgnores should be 0 after UnityBegin");
    ASSERT_EQ_INT(0, Unity.CurrentTestFailed, "CurrentTestFailed should be 0 after UnityBegin");
    ASSERT_EQ_INT(0, Unity.CurrentTestIgnored, "CurrentTestIgnored should be 0 after UnityBegin");
}