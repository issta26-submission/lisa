// Test suite for the focal method: CheckProfileSequenceTag
// This test is implemented as a lightweight C++11 test harness (no GTest).
// It relies on the provided test harness stubs (from testcms2.h) to create a test
// profile and exercise the CheckProfileSequenceTag function.
// The tests cover true/false branches where applicable and ensure execution continues
// after assertions (non-terminating assertions).

#include <iostream>
#include <testcms2.h>


// Import C test harness dependencies with C linkage
extern "C" {
}

// Simple non-terminating assertions to maximize coverage without aborting on failure
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do {                                      \
    if (static_cast<bool>(cond)) {                                        \
        std::cout << "[OK] " << msg << std::endl;                       \
    } else {                                                              \
        std::cerr << "[FAIL] " << msg << " (condition: " #cond ")"      \
                  << std::endl;                                         \
        ++g_failures;                                                   \
    }                                                                     \
} while (0)

// Test 1: Basic Write (Pass = 1) followed by Read/Verify (Pass = 2)
// This test ensures the sequence description can be allocated, filled with data,
// written to the profile, and subsequently read back with exact data checks.
// It also implicitly exercises the string/multi-locale handling via SetOneStr/CheckOneStr.
void test_ProfileSequenceTag_BasicWriteRead()

{
    // Create a test profile to operate on
    cmsHPROFILE hProfile = Create_AboveRGB();
    if (hProfile == NULL) {
        std::cerr << "[ERR] Create_AboveRGB() returned NULL, cannot proceed with Test 1." << std::endl;
        ++g_failures;
        return;
    }

    // Case Pass = 1: Write a Profile Sequence Description tag into the profile
    int r = CheckProfileSequenceTag(1, hProfile);
    EXPECT_TRUE(r == 1, "CheckProfileSequenceTag with Pass=1 should return 1 (successful write).");

    // Case Pass = 2: Read back and validate the data from the tag
    r = CheckProfileSequenceTag(2, hProfile);
    EXPECT_TRUE(r == 1, "CheckProfileSequenceTag with Pass=2 should return 1 (successful read/validate).");

    // Cleanup test profiles to avoid leaks/state leakage between tests
    RemoveTestProfiles();
}

// Test 2: Invalid Pass value should trigger default branch and return 0
// This ensures the else/default path is exercised (branch where no specific case matches).
void test_ProfileSequenceTag_InvalidPassBranch()

{
    // Create a fresh profile for this isolated test
    cmsHPROFILE hProfile = Create_AboveRGB();
    if (hProfile == NULL) {
        std::cerr << "[ERR] Create_AboveRGB() returned NULL, cannot proceed with Test 2." << std::endl;
        ++g_failures;
        return;
    }

    // Pass value that does not match 1 or 2 (default case)
    int r = CheckProfileSequenceTag(3, hProfile);
    EXPECT_TRUE(r == 0, "CheckProfileSequenceTag with unsupported Pass value should return 0 (default case).");

    RemoveTestProfiles();
}

int main(int argc, char* argv[])
{
    // Run the defined unit tests
    test_ProfileSequenceTag_BasicWriteRead();
    test_ProfileSequenceTag_InvalidPassBranch();

    // Report overall result
    if (g_failures) {
        std::cerr << "[RESULT] Tests finished with " << g_failures << " failure(s)." << std::endl;
        return 1;
    } else {
        std::cout << "[RESULT] All tests passed." << std::endl;
        return 0;
    }
}