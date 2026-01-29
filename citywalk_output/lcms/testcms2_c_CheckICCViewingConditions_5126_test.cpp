/*
Unit test suite for the focal method:
    cmsInt32Number CheckICCViewingConditions(cmsInt32Number Pass,  cmsHPROFILE hProfile)

Step 1 - Program Understanding (Candidate Keywords)
- cmsICCViewingConditions: structure containing IlluminantType, IlluminantXYZ, SurroundXYZ
- cmsSigViewingConditionsTag: tag signature used for reading/writing viewing conditions
- cmsWriteTag / cmsReadTag: profile tag write/read operations
- IsGoodVal: helper used in verification of numeric values
- IlluminantXYZ.X/Y/Z and SurroundXYZ.X/Y/Z: expected numeric fields
- Pass switch: case 1 writes, case 2 reads & validates, default returns 0

Step 2 - Test Focus
Tests cover:
- Pass = 1: profile receives a ViewingConditions tag with specified values
- Pass = 2: reading back the tag after a prior Pass 1 writes should succeed and values must match within tolerance
- Pass = 0 (default): ensures it returns 0 for unhandled Pass values
- Pass = 2 without prior Pass 1 should return 0 (no tag)

Step 3 - Test Design Notes
- Use the provided test harness helpers (Create_AboveRGB, RemoveTestProfiles, etc.) from testcms2.h
- Use non-terminating assertions (EXPECT_*) to maximize code execution and coverage
- Tests are implemented in C++11 file style but call C APIs from the focal code
- Clean up test profiles after tests
- All tests are executed from main()

This test suite is designed to be compiled with the existing testcms2.h infrastructure and does not rely on GTest or other external test frameworks.
*/

#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


// Simple non-terminating assertion helpers to maximize test coverage
static int g_test_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { fprintf(stderr, "EXPECT_TRUE failed: %s\n", msg); ++g_test_failures; } } while(0)

#define EXPECT_EQ_INT(a, b, msg) \
    do { if(((int)(a)) != ((int)(b))) { fprintf(stderr, "EXPECT_EQ_INT failed: %s (actual=%d, expected=%d)\n", msg, (int)(a), (int)(b)); ++g_test_failures; } } while(0)


// Test 1: Pass = 1 should write the ViewingConditions tag successfully
//         Followed by Pass = 2 to read back and validate exact values.
bool Test_CheckICCViewingConditions_WriteThenRead() {
    // Create a fresh test profile to operate on
    cmsHPROFILE hProfile = Create_AboveRGB();
    if (hProfile == NULL) {
        fprintf(stderr, "Test 1: Failed to create test profile.\n");
        ++g_test_failures;
        return false;
    }

    // Case Pass = 1: write a ViewingConditions structure into the profile
    int resWrite = CheckICCViewingConditions(1, hProfile);
    EXPECT_EQ_INT(resWrite, 1, "Pass=1 should return 1 after writing");

    // Case Pass = 2: read back the ViewingConditions tag and validate fields
    int resRead = CheckICCViewingConditions(2, hProfile);
    EXPECT_EQ_INT(resRead, 1, "Pass=2 should return 1 after successful read and validation");

    // Cleanup
    RemoveTestProfiles(); // best-effort cleanup of test resources
    return (g_test_failures == 0);
}


// Test 2: Pass = 2 without prior Pass = 1 should fail (no tag present)
bool Test_CheckICCViewingConditions_ReadWithoutWrite() {
    cmsHPROFILE hProfile = Create_AboveRGB();
    if (hProfile == NULL) {
        fprintf(stderr, "Test 2: Failed to create test profile.\n");
        ++g_test_failures;
        return false;
    }

    int resReadNoWrite = CheckICCViewingConditions(2, hProfile);
    EXPECT_TRUE(resReadNoWrite == 0, "Pass=2 without prior write should return 0 (no tag present)");

    RemoveTestProfiles();
    return (g_test_failures == 0);
}


// Test 3: Default case (any Pass value not 1 or 2 should return 0)
bool Test_CheckICCViewingConditions_DefaultCase() {
    cmsHPROFILE hProfile = Create_AboveRGB();
    if (hProfile == NULL) {
        fprintf(stderr, "Test 3: Failed to create test profile.\n");
        ++g_test_failures;
        return false;
    }

    int resDefault = CheckICCViewingConditions(0, hProfile);
    EXPECT_EQ_INT(resDefault, 0, "Default case should return 0 for Pass=0");

    RemoveTestProfiles();
    return (g_test_failures == 0);
}


// Main entry to run tests
int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // suppress unused parameter warnings

    // Run tests
    bool t1 = Test_CheckICCViewingConditions_WriteThenRead();
    bool t2 = Test_CheckICCViewingConditions_ReadWithoutWrite();
    bool t3 = Test_CheckICCViewingConditions_DefaultCase();

    // Summary
    if (g_test_failures == 0) {
        printf("All CheckICCViewingConditions tests passed.\n");
        return 0;
    } else {
        fprintf(stderr, "%d test failure(s) detected in CheckICCViewingConditions suite.\n", g_test_failures);
        return 1;
    }
}