// Test suite for the focal method CheckDictionary16 in testcms2.c
// This test uses the provided test harness interfaces (via testcms2.h) to
// create test profiles and validate both branches of CheckDictionary16.
// The tests are executed from main (no GTest). Explanations are provided
// as comments for each unit test.

#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Note: We rely on the project-provided functions from testcms2.h:
// - Create_AboveRGB(void) -> cmsHPROFILE
// - RemoveTestProfiles(void)
// - CheckDictionary16(cmsInt32Number Pass, cmsHPROFILE hProfile) -> cmsInt32Number

int main() {
    int failures = 0;

    std::cout << "Starting CheckDictionary16 unit tests..." << std::endl;

    // Prepare a test profile for dictionary tests
    cmsHPROFILE hProfile1 = Create_AboveRGB();
    if (!hProfile1) {
        std::cerr << "ERROR: Failed to create test profile for CheckDictionary16 tests." << std::endl;
        return 1;
    }

    // Test 1: Pass = 1
    // Purpose: Exercise the true-branch where a dictionary is allocated,
    // entries are added, and the tag is written to the profile.
    // Expected: CheckDictionary16 returns 1 on success.
    {
        cmsInt32Number result = CheckDictionary16(1, hProfile1);
        if (result != 1) {
            std::cerr << "Test 1 FAILED: Expected 1, got " << result << std::endl;
            ++failures;
        } else {
            std::cout << "Test 1 PASSED: CheckDictionary16(1, ...) returned 1 as expected." << std::endl;
        }
    }

    // Test 2: Pass = 2
    // Purpose: After writing the dictionary in Pass 1, read it back and validate
    // the ordering and contents of the dictionary entries.
    // Expected: CheckDictionary16 returns 1 if all validations succeed.
    {
        cmsInt32Number result = CheckDictionary16(2, hProfile1);
        if (result != 1) {
            std::cerr << "Test 2 FAILED: Expected 1, got " << result << std::endl;
            ++failures;
        } else {
            std::cout << "Test 2 PASSED: CheckDictionary16(2, ...) returned 1 as expected." << std::endl;
        }
    }

    // Test 3: Pass = 0 (default path)
    // Purpose: Ensure that the default path (case not handled) returns 0.
    // Expected: CheckDictionary16 returns 0.
    {
        cmsInt32Number result = CheckDictionary16(0, hProfile1);
        if (result != 0) {
            std::cerr << "Test 3 FAILED: Expected 0, got " << result << std::endl;
            ++failures;
        } else {
            std::cout << "Test 3 PASSED: CheckDictionary16(0, ...) returned 0 as expected." << std::endl;
        }
    }

    // Test 4: Pass = 2 on a new profile with no Meta tag
    // Purpose: Validate that reading a non-tagged/profile without a meta tag fails gracefully.
    // Expected: CheckDictionary16 returns 0 (since no MetaTag was written).
    cmsHPROFILE hProfile2 = Create_AboveRGB();
    if (!hProfile2) {
        std::cerr << "ERROR: Failed to create second test profile for negative test." << std::endl;
        ++failures;
    } else {
        cmsInt32Number result = CheckDictionary16(2, hProfile2);
        if (result != 0) {
            std::cerr << "Test 4 FAILED: Expected 0 for no MetaTag, got " << result << std::endl;
            ++failures;
        } else {
            std::cout << "Test 4 PASSED: CheckDictionary16(2, ...) returned 0 when no MetaTag present." << std::endl;
        }
    }

    // Cleanup test profiles
    RemoveTestProfiles();

    // Summary
    if (failures == 0) {
        std::cout << "All CheckDictionary16 tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "CheckDictionary16 tests completed with " << failures << " failure(s)." << std::endl;
        return 1;
    }
}