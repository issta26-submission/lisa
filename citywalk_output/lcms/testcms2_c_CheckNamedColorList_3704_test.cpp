// test_named_color_list.cpp
// Lightweight unit tests for the focal method: CheckNamedColorList
// This test suite is designed to be compiled with a C++11 compiler alongside the C source
// containing CheckNamedColorList (provided in testcms2.c).
// No GoogleTest or other third-party testing frameworks are used.
// The tests rely on a small in-house test harness and simple console output.

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <io.h>
#include <testcms2.h>
#include <sys/types.h>
#include <cstdlib>
#include <unistd.h>
#include <direct.h>
#include <cerrno>


#ifdef _WIN32
#else
#endif

// Forward declaration of the focal function (C linkage)
extern "C" int CheckNamedColorList(void);

// Lightweight test harness (non-terminating assertions)
static void LogTestResult(const char* testName, bool passed, const char* detail = nullptr) {
    if (passed) {
        printf("[PASS] %s\n", testName);
        if (detail) printf("  %s\n", detail);
    } else {
        printf("[FAIL] %s", testName);
        if (detail) printf(" - %s", detail);
        printf("\n");
    }
}

// Test 1: Ensure the focal function succeeds under normal conditions.
// This corresponds to the "true" branch of the main path.
static bool Test_CheckNamedColorList_Success() {
    // Call the focal function; it should return 1 on success (as per original code rc=1)
    int rc = CheckNamedColorList();
    if (rc == 1) {
        LogTestResult("Test_CheckNamedColorList_Success", true, "CheckNamedColorList returned 1 as expected.");
        return true;
    } else {
        LogTestResult("Test_CheckNamedColorList_Success", false, "Expected rc == 1, got rc != 1.");
        return false;
    }
}

// Test 2: Force a failure path by creating a directory where the function tries to
// write the named color profile file namedcol.icc. This should cause the function
// to fail when opening the profile for writing, returning 0.
static bool Test_CheckNamedColorList_Fail_On_OpenOutput() {
    bool dir_created = false;

    // Create a directory named "namedcol.icc" to block file creation
#ifdef _WIN32
    int res = _mkdir("namedcol.icc");
#else
    int res = mkdir("namedcol.icc", 0777);
#endif
    if (res == 0) {
        dir_created = true;
        LogTestResult("Test_CheckNamedColorList_Fail_On_OpenOutput", true, "Created directory to simulate write failure.");
    } else {
        // If the directory already exists or creation failed for another reason, we still
        // proceed but note the limitation.
        if (errno == EEXIST) {
            LogTestResult("Test_CheckNamedColorList_Fail_On_OpenOutput", true, "Directory namedcol.icc already exists; proceeding may still exercise failure path.");
        } else {
            LogTestResult("Test_CheckNamedColorList_Fail_On_OpenOutput", false, "Could not create directory namedcol.icc; test may be inconclusive.");
        }
    }

    // Run the focal function; it should fail due to inability to create namedcol.icc file
    int rc = CheckNamedColorList();
    bool passed = (rc == 0);
    if (!passed) {
        LogTestResult("Test_CheckNamedColorList_Fail_On_OpenOutput", false, "Expected rc == 0 when write-open fails, but got non-zero.");
    } else {
        LogTestResult("Test_CheckNamedColorList_Fail_On_OpenOutput", true, "Function correctly returned 0 on simulated write failure.");
    }

    // Cleanup: remove the directory if we created it
    if (dir_created) {
#ifdef _WIN32
        _rmdir("namedcol.icc");
#else
        rmdir("namedcol.icc");
#endif
    } else {
        // If the directory existed prior, we should not remove it to avoid side effects.
    }

    return passed;
}

// Main function to run tests
int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    printf("Running unit tests for CheckNamedColorList...\n");

    bool all_passed = true;

    // Execute Test 1
    all_passed = all_passed && Test_CheckNamedColorList_Success();

    // Execute Test 2 (failure path via directory collision)
    all_passed = all_passed && Test_CheckNamedColorList_Fail_On_OpenOutput();

    if (all_passed) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Some tests failed.\n");
        return 1;
    }
}