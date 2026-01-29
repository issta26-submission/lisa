// Minimal C++11 unit tests for the focal method: cmsBool isabsolutepath(const char *path)
// This test suite avoids GTest and uses a tiny, self-contained test harness that continues
// execution after a failed assertion to maximize coverage.
// The tests assume the usual semantics of cmsBool (non-zero = TRUE, zero = FALSE).
//
// Note: The test relies on the external C function isabsolutepath being linked in.
// We declare it with C linkage to avoid name mangling.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Assume cmsBool is an unsigned int in the original codebase.
// We mirror that in the test to provide a compatible signature.
typedef unsigned int cmsBool;

// Declare the focal function with C linkage to link correctly with the C source.
extern "C" cmsBool isabsolutepath(const char* path);

// Simple non-terminating test assertion helper
static int g_pass = 0;
static int g_fail = 0;

// Record the result of a boolean test without stopping execution
static void RunBoolTest(const char* testName, bool actual, bool expected) {
    if (actual == expected) {
        ++g_pass;
    } else {
        ++g_fail;
        std::cerr << "FAIL: " << testName
                  << " - expected " << (expected ? "TRUE" : "FALSE")
                  << ", got " << (actual ? "TRUE" : "FALSE") << "\n";
    }
}

// Public test cases

// Test 1: NULL path should return FALSE
// Reasoning: The function explicitly checks if path == NULL and returns FALSE.
static void test_null_path() {
    // Test description:
    // Ensure that passing NULL does not crash and yields FALSE.
    cmsBool res = isabsolutepath(NULL);
    bool actual = (res != 0);
    RunBoolTest("test_null_path (NULL input)", actual, false);
}

// Test 2: Empty string should return FALSE
// Reasoning: The function checks if path[0] == 0 and returns FALSE.
static void test_empty_path() {
    // Test description:
    // Ensure that an empty string is treated as non-absolute.
    const char* empty = "";
    cmsBool res = isabsolutepath(empty);
    bool actual = (res != 0);
    RunBoolTest("test_empty_path (empty string)", actual, false);
}

// Test 3: Absolute Unix-style path (leading DIR_CHAR) should return TRUE
// Reasoning: The function copies first 3 chars and checks if ThreeChars[0] equals DIR_CHAR.
// DIR_CHAR is typically '/', so "/absolute/path" should yield TRUE in non-Windows builds.
static void test_absolute_unix_path() {
    // Test description:
    // If DIR_CHAR is '/', a path starting with '/' must be considered absolute.
    const char* path = "/usr/local/bin";
    cmsBool res = isabsolutepath(path);
    bool actual = (res != 0);
    RunBoolTest("test_absolute_unix_path (leading /)", actual, true);
}

// Test 4: Relative path should return FALSE
// Reasoning: A path not starting with DIR_CHAR and without Windows drive syntax should be FALSE.
static void test_relative_path() {
    // Test description:
    // Relative path like "folder/file.txt" should not be treated as absolute.
    const char* path = "folder/file.txt";
    cmsBool res = isabsolutepath(path);
    bool actual = (res != 0);
    RunBoolTest("test_relative_path (relative path)", actual, false);
}

// Test 5: Windows drive letter path (C:) should return TRUE when compiled for Windows
// Guarded to compile only when CMS_IS_WINDOWS_ is defined in the focal translation unit.
// Reasoning: If CMS_IS_WINDOWS_ is defined, the function also checks for a drive-letter syntax.
#ifdef CMS_IS_WINDOWS_
static void test_windows_drive_letter() {
    // Test description:
    // On Windows builds, "C:" should be recognized as absolute.
    const char* path = "C:";
    cmsBool res = isabsolutepath(path);
    bool actual = (res != 0);
    RunBoolTest("test_windows_drive_letter (C:)", actual, true);
}
#endif

// Driver: run all tests and report a summary
int main() {
    std::cout << "Running cmsBool isabsolutepath unit tests...\n";

    test_null_path();
    test_empty_path();
    test_absolute_unix_path();
    test_relative_path();

#ifdef CMS_IS_WINDOWS_
    test_windows_drive_letter();
#endif

    std::cout << "Test results: Passed = " << g_pass << ", Failed = " << g_fail << "\n";

    // Return non-zero if any test failed
    return g_fail;
}