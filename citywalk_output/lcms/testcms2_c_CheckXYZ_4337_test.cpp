/*
Unit Test Suite for the focal method:
    cmsInt32Number CheckXYZ(cmsInt32Number Pass, cmsHPROFILE hProfile, cmsTagSignature tag)

Overview:
- Tests are written in C++11 without Google Test.
- A lightweight test harness is implemented to run tests from main().
- Tests exercise both branches of the CheckXYZ switch:
  - Pass == 1: writes an XYZ value to the profile.
  - Pass == 2: reads the XYZ value and validates it against expected fixed-point representation.
  - Default: any other Pass returns 0.
- The tests rely on the provided dependencies (testcms2.h and lcms2.h) and the functions Create_AboveRGB(), cmsSigXYZTag, cmsWriteTag, cmsReadTag, and IsGoodFixed15_16.
- All tests are designed to be non-terminating: failures are reported but do not stop subsequent tests.
- The static helpers inside testcms2.c (static at file scope) are not accessed; tests interact only with the externally visible API.

Candidate Keywords extracted from the focal code and dependencies:
- cmsInt32Number, cmsHPROFILE, cmsTagSignature
- cmsCIEXYZ
- cmsWriteTag, cmsReadTag
- IsGoodFixed15_16
- cmsSigXYZTag
- Create_AboveRGB, cmsCloseProfile
- cmsInt32Number return semantics, NULL checks
- Basic success/failure expectations around Tag IO
*/

#include <functional>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <lcms2.h>


// Forward-declare C API from the project under test
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

static void logFailure(const std::string& msg) {
    std::cerr << "Test Failure: " << msg << std::endl;
}

static void ASSERT(bool condition, const char* description) {
    if (!condition) {
        logFailure(description);
        ++g_failures;
    }
}

// Test 1: Write XYZ to a profile (Pass = 1) and verify write succeeded.
static void TestXYZ_WriteXYZ_WritesSuccessfully() {
    // Arrange
    cmsHPROFILE hProfile = Create_AboveRGB();
    cmsTagSignature tag = cmsSigXYZTag; // Target XYZ tag

    // Act
    cmsInt32Number ret = CheckXYZ(1, hProfile, tag);

    // Assert (non-terminating): ensure write happened
    ASSERT(ret != 0, "CheckXYZ with Pass=1 should return non-zero (write success)");

    // Cleanup
    cmsCloseProfile(hProfile);
}

// Test 2: Read XYZ from a profile (Pass = 2) and verify correctness against expected values.
static void TestXYZ_ReadXYZ_MatchesExpected() {
    // Arrange: ensure a profile with a known XYZ value exists
    cmsHPROFILE hProfile = Create_AboveRGB();
    cmsTagSignature tag = cmsSigXYZTag;

    // First, perform the write so that a subsequent read has data to verify
    cmsInt32Number writeRet = CheckXYZ(1, hProfile, tag);
    ASSERT(writeRet != 0, "CheckXYZ (Pass=1) should succeed before read test");

    // Act: read back and compare
    cmsInt32Number readRet = CheckXYZ(2, hProfile, tag);

    // Assert: ensure the read values match expected fixed-point representation
    ASSERT(readRet != 0, "CheckXYZ (Pass=2) should succeed and return true when values match");

    // Cleanup
    cmsCloseProfile(hProfile);
}

// Test 3: Default case (Pass not 1 or 2) should return 0
static void TestXYZ_DefaultCaseReturnsZero() {
    // Arrange
    cmsHPROFILE hProfile = Create_AboveRGB();
    cmsTagSignature tag = cmsSigXYZTag;

    // Act
    cmsInt32Number ret = CheckXYZ(3, hProfile, tag);

    // Assert
    ASSERT(ret == 0, "CheckXYZ with an unsupported Pass value should return 0");

    // Cleanup
    cmsCloseProfile(hProfile);
}

// Runner to execute all tests
static int RunAllTests() {
    TestXYZ_WriteXYZ_WritesSuccessfully();
    TestXYZ_ReadXYZ_MatchesExpected();
    TestXYZ_DefaultCaseReturnsZero();

    return g_failures;
}

// Main function as the entry point for the test suite
int main() {
    int failures = RunAllTests();
    if (failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << failures << " test(s) failed." << std::endl;
        return failures;
    }
}