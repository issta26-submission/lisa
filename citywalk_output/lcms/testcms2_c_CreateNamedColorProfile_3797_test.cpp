#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <testcms2.h>


// Declaration of the focal function from the provided source.
// It is defined with C linkage in the test file (testcms2.c).
extern "C" int CreateNamedColorProfile(void);

// Simple assertion macro that does not terminate the test on failure.
// It logs the failure and increments a global counter to allow continued execution.
static int gFailures = 0;
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Assertion failed: " << (msg) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++gFailures; \
    } \
} while (0)

// Utility: check if a file exists in the current working directory.
static bool file_exists(const std::string& path) {
    std::ifstream f(path.c_str(), std::ios::in | std::ios::binary);
    return f.good();
}

// Domain knowledge notes (for reference in comments):
// - The focal function interacts with multiple LittleCMS APIs to construct a Named Color Profile.
// - Core dependent components (Candidate Keywords) include: cmsAllocNamedColorList, cmsOpenProfileFromFile,
//   cmsSetProfileVersion, cmsSetDeviceClass, cmsSetColorSpace, cmsSetPCS, cmsSetHeaderRenderingIntent,
//   cmsMLUalloc/cmsMLUsetWide, cmsWriteTag, cmsD50_XYZ, cmsFloat2LabEncodedV2, cmsAppendNamedColor,
//   cmsCloseProfile, cmsFreeNamedColorList, cmsMLUfree, and file removal via remove().


// Test 1: Validate that CreateNamedColorProfile returns 1 on successful creation.
// Rationale: The function's path in the provided code always ends with returning 1 after cleanup.
// Ensures the primary contract of the function is satisfied without crashing.
static void test_CreateNamedColorProfile_ReturnsOne(void)
{
    // When invoked, the function should return 1 indicating success.
    int res = CreateNamedColorProfile();
    ASSERT_TRUE(res == 1, "CreateNamedColorProfile should return 1 on success");
}

// Test 2: Ensure that any pre-existing named.icc file is removed by the focal function.
// Rationale: The function ends with remove(\"named.icc\");. This test creates a dummy file to verify
// that the focal function actually removes it (i.e., cleans up) rather than leaving artifacts.
static void test_CreateNamedColorProfile_RemovesPreExistingNamedICC(void)
{
    // Prepare a dummy named.icc file to simulate an existing artifact.
    {
        std::ofstream pre("named.icc", std::ios::out | std::ios::binary);
        pre << "dummy content";
    }
    // Sanity: the file should exist at this point.
    ASSERT_TRUE(file_exists("named.icc"), "Pre-existing named.icc should exist before test");

    // Run the focal function. It is responsible for removing named.icc in its cleanup path.
    int res = CreateNamedColorProfile();
    ASSERT_TRUE(res == 1, "CreateNamedColorProfile should return 1 even when pre-existing file is present");

    // After execution, the file should be removed by the focal function.
    bool existsAfter = file_exists("named.icc");
    ASSERT_TRUE(!existsAfter, "named.icc should be removed by CreateNamedColorProfile");
}

// Test 3: Call the focal function multiple times to ensure it is robust against repeated usage.
// Rationale: Verifies that repeated invocations do not crash or produce inconsistent state.
// Also checks that each call still returns 1.
static void test_CreateNamedColorProfile_MultipleCalls(void)
{
    // First call
    int res1 = CreateNamedColorProfile();
    ASSERT_TRUE(res1 == 1, "First invocation should return 1");

    // Second call
    int res2 = CreateNamedColorProfile();
    ASSERT_TRUE(res2 == 1, "Second invocation should return 1");
}


// Simple test harness
struct TestCase {
    const char* name;
    void (*fn)(void);
};

// Forward declarations of tests (to populate test table)
static void test_CreateNamedColorProfile_ReturnsOne(void);
static void test_CreateNamedColorProfile_RemovesPreExistingNamedICC(void);
static void test_CreateNamedColorProfile_MultipleCalls(void);

// Registry of tests
static TestCase gTests[] = {
    {"CreateNamedColorProfile_ReturnsOne", test_CreateNamedColorProfile_ReturnsOne},
    {"CreateNamedColorProfile_RemovesPreExistingNamedICC", test_CreateNamedColorProfile_RemovesPreExistingNamedICC},
    {"CreateNamedColorProfile_MultipleCalls", test_CreateNamedColorProfile_MultipleCalls},
};

// Main: runs all tests and reports a summary.
// Behavior: non-terminating assertions allow all tests to run and report their results.
int main() {
    std::cout << "Starting test suite for CreateNamedColorProfile (C++11 test harness)" << std::endl;
    const int total = sizeof(gTests) / sizeof(gTests[0]);
    int passed = 0;

    for (int i = 0; i < total; ++i) {
        const char* tname = gTests[i].name;
        // Reset per-test failure counter to detect per-test failures accurately.
        int beforeFailures = gFailures;
        std::cout << "[ RUNNING ] " << tname << std::endl;

        // Execute test
        gTests[i].fn();

        // Determine if test passed (no new failures occurred)
        if (gFailures == beforeFailures) {
            ++passed;
            std::cout << "[  PASSED ] " << tname << std::endl;
        } else {
            std::cout << "[  FAILED ] " << tname << " (see assertion failures above)" << std::endl;
        }
    }

    std::cout << "Test results: " << passed << " / " << total << " tests passed." << std::endl;
    if (gFailures > 0) {
        std::cout << gFailures << " assertion failure(s) observed." << std::endl;
    } else {
        std::cout << "All assertions passed." << std::endl;
    }

    // Return non-zero if any test failed to aid integration with CI systems.
    return (gFailures == 0) ? 0 : 1;
}