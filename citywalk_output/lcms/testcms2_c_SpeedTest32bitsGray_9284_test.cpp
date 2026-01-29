// C++11 unit test harness for the focal method: SpeedTest32bitsGray
// This test suite targets the function defined in testcms2.c and its
// related dependencies declared in the provided <FOCAL_CLASS_DEP> section.
// The tests are designed to be executable without GoogleTest and rely only
// on the C standard library and the provided test harness functions.

#include <cstdio>
#include <vector>
#include <string>
#include <testcms2.h>
#include <cstdlib>


// Include the C test infrastructure (prototyped to be compatible with C linkage)
extern "C" {
}

// If the header testcms2.h already declares these, the following externs
// may be redundant, but they ensure explicit linkage for the tests.
extern "C" cmsHPROFILE Create_Gray22(void);
extern "C" cmsHPROFILE Create_Gray30(void);
extern "C" void SpeedTest32bitsGray(const char* Title,
                                  cmsHPROFILE hlcmsProfileIn,
                                  cmsHPROFILE hlcmsProfileOut,
                                  cmsInt32Number Intent);
extern "C" void RemoveTestProfiles(void);

// Lightweight non-terminating assertion helper for test cases
#define ASSERT_TRUE(cond, msg)                                      \
    do {                                                              \
        if(!(cond)) {                                                 \
            fprintf(stderr, "ASSERT_TRUE failed: %s\n", (msg));      \
            return false;                                           \
        }                                                             \
    } while(0)

// Candidate Keywords (Step 1): leveraging the focal method's core components
// - SpeedTest32bitsGray: main method under test
// - Interval, NumPixels, In buffer: internal data preparation for the transform
// - cmsCreateTransformTHR, cmsDoTransform, cmsDeleteTransform: core CMS operations
// - TYPE_GRAY_FLT, cmsFLAGS_NOCACHE: transform configuration specifics
// - Create_Gray22, Create_Gray30: grayscale input/output profiles (dependencies)
// - cmsCloseProfile, PrintPerformance, TitlePerformance: I/O and reporting
// - DbgThread: debug context hook used by SpeedTest32bitsGray
// - chknull, Die: utility and error handling helpers
// - CreateTestProfiles / RemoveTestProfiles: test fixture management
// - Intent: color management rendering intent (represented as cmsInt32Number in tests)

// Basic test 1: Validate that SpeedTest32bitsGray completes without crash
// when provided with two valid Gray profiles (22 gamma to 22 gamma) using a typical intent.
// This covers the false-branch of the profile non-null predicate and exercises the
// primary code path (data preparation, transform creation, transform execution, cleanup).
static bool test_SpeedTest32bitsGray_BasicGray22toGray22() {
    printf("TEST: SpeedTest32bitsGray_BasicGray22toGray22\n");
    // Create input/output grayscale profiles (types compatible with TYPE_GRAY_FLT)
    cmsHPROFILE in  = Create_Gray22();
    cmsHPROFILE out = Create_Gray22();

    if (in == NULL || out == NULL) {
        fprintf(stderr, "Failed to create Gray22 profiles for test_BasicGray22toGray22\n");
        // Do not proceed if profiles cannot be created
        return false;
    }

    // Use perceptual-like intent (cmsInt32Number typically 0 for cmsIntentPerceptual)
    // We pass 0 explicitly to avoid depending on specific macro visibility here.
    SpeedTest32bitsGray("Gray32Bits_Test_BasicGray22toGray22", in, out, (cmsInt32Number)0);

    // The focal method closes the input/output profiles internally
    // If the call returns, we assume it did not crash.
    return true;
}

// Basic test 2: Validate SpeedTest32bitsGray with a different target gray profile output
// This ensures the method handles in/out profile variance gracefully and still executes.
static bool test_SpeedTest32bitsGray_Gray22toGray30() {
    printf("TEST: SpeedTest32bitsGray_Gray22toGray30\n");
    cmsHPROFILE in  = Create_Gray22();
    cmsHPROFILE out = Create_Gray30();

    if (in == NULL || out == NULL) {
        fprintf(stderr, "Failed to create Gray22/Gray30 profiles for test_Gray22toGray30\n");
        return false;
    }

    SpeedTest32bitsGray("Gray32Bits_Test_Gray22toGray30", in, out, (cmsInt32Number)0);

    // Cleanup handled inside the focal method for the provided profiles
    return true;
}

// Optional: a small utility to run all tests and report overall result
int main(int argc, char* argv[]) {
    // Explain the test purpose
    printf("Starting unit tests for SpeedTest32bitsGray (no GTest required)\n");

    // Gather tests
    struct TestCase {
        const char* name;
        bool (*fn)();
    };

    std::vector<TestCase> tests = {
        {"BasicGray22toGray22", test_SpeedTest32bitsGray_BasicGray22toGray22},
        {"Gray22toGray30",   test_SpeedTest32bitsGray_Gray22toGray30}
    };

    // Run tests
    int overall = 0;
    for (const auto& t : tests) {
        printf("Running test: %s\n", t.name);
        bool ok = t.fn();
        if (ok) {
            printf("PASS: %s\n", t.name);
        } else {
            printf("FAIL: %s\n", t.name);
            overall = 1;
        }
    }

    // Cleanup test fixture state if necessary
    RemoveTestProfiles();

    return overall;
}