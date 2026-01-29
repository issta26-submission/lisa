// Test suite for the focal function: cmsInt32Number Check_cicp(cmsInt32Number Pass, cmsHPROFILE hProfile)
// This test harness is written in C++11 without Google Test (no GMock).
// It uses the actual Little CMS API (lcms2.h) to create profiles, write/read the cicp tag,
// and validate the behavior of Check_cicp as described in the focal method.
// Explanatory comments accompany each unit test for clarity and coverage rationale.

#include <cstdio>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <lcms2.h>


// Include Little CMS headers with C linkage
extern "C" {
}

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_fail(const char* test_name, const char* msg) {
    std::cerr << "[FAIL] " << test_name << ": " << msg << std::endl;
}

// Test 1: Check_cicp with Pass = 1 should write tag successfully and return 1
bool test_CheckCicp_WriteTag_Succeeds() {
    const char* test_name = "CheckCicp_WriteTag_Succeeds (Pass=1)";
    g_total_tests++;

    cmsHPROFILE h = cmsCreate_sRGBProfile();
    if (h == nullptr) {
        log_fail(test_name, "Failed to create test profile (srgb).");
        g_failed_tests++;
        return false;
    }

    cmsInt32Number res = Check_cicp(1, h);
    if (res != 1) {
        log_fail(test_name, "Expected return 1 when writing cicp tag, got different value.");
        cmsCloseProfile(h);
        g_failed_tests++;
        return false;
    }

    cmsCloseProfile(h);
    return true;
}

// Test 2: Check_cicp with Pass = 2 should read back the tag written by Pass = 1 and return 1
bool test_CheckCicp_ReadTag_SucceedsAfterWrite() {
    const char* test_name = "CheckCicp_ReadTag_SucceedsAfterWrite (Pass=2 after Pass=1)";
    g_total_tests++;

    cmsHPROFILE h = cmsCreate_sRGBProfile();
    if (h == nullptr) {
        log_fail(test_name, "Failed to create test profile (srgb).");
        g_failed_tests++;
        return false;
    }

    // First, write the tag (as in Pass=1)
    if (Check_cicp(1, h) != 1) {
        log_fail(test_name, "Failed to write cicp tag in preliminary Pass=1.");
        cmsCloseProfile(h);
        g_failed_tests++;
        return false;
    }

    // Now, read back and verify Pass=2 returns success
    if (Check_cicp(2, h) != 1) {
        log_fail(test_name, "Expected Read-back success (Pass=2) after writing; got failure.");
        cmsCloseProfile(h);
        g_failed_tests++;
        return false;
    }

    cmsCloseProfile(h);
    return true;
}

// Test 3: Check_cicp with Pass = 2 should fail if cicp tag is present but with incorrect values
bool test_CheckCicp_ReadTag_WithWrongValues_Fails() {
    const char* test_name = "CheckCicp_ReadTag_WithWrongValues_Fails (Pass=2, bad tag)";
    g_total_tests++;

    cmsHPROFILE h = cmsCreate_sRGBProfile();
    if (h == nullptr) {
        log_fail(test_name, "Failed to create test profile (srgb).");
        g_failed_tests++;
        return false;
    }

    // Manually write a tag with incorrect values
    cmsVideoSignalType bad;
    bad.ColourPrimaries = 2;           // Wrong on purpose
    bad.TransferCharacteristics = 13;
    bad.MatrixCoefficients = 0;
    bad.VideoFullRangeFlag = 1;

    if (!cmsWriteTag(h, cmsSigcicpTag, &bad)) {
        log_fail(test_name, "Failed to manually write a bad cicp tag.");
        cmsCloseProfile(h);
        g_failed_tests++;
        return false;
    }

    // Now, Check_cicp with Pass=2 should detect mismatch and return 0
    if (Check_cicp(2, h) != 0) {
        log_fail(test_name, "Expected 0 when cicp tag values do not match expected; got 1.");
        cmsCloseProfile(h);
        g_failed_tests++;
        return false;
    }

    cmsCloseProfile(h);
    return true;
}

// Test 4: Check_cicp with Pass = 2 should fail when there is no cicp tag present
bool test_CheckCicp_ReadTag_NoTag_Fails() {
    const char* test_name = "CheckCicp_ReadTag_NoTag_Fails (Pass=2, no tag present)";
    g_total_tests++;

    cmsHPROFILE h = cmsCreate_sRGBProfile();
    if (h == nullptr) {
        log_fail(test_name, "Failed to create test profile (srgb).");
        g_failed_tests++;
        return false;
    }

    // Ensure no cicp tag exists (fresh profile). Directly call Pass=2.
    if (Check_cicp(2, h) != 0) {
        log_fail(test_name, "Expected 0 when no cicp tag exists; got 1.");
        cmsCloseProfile(h);
        g_failed_tests++;
        return false;
    }

    cmsCloseProfile(h);
    return true;
}

// Test 5: Check_cicp default/unsupported Pass value should return 0
bool test_CheckCicp_InvalidPass_ReturnsZero() {
    const char* test_name = "CheckCicp_InvalidPass_ReturnsZero (Pass=3, default)";
    g_total_tests++;

    cmsHPROFILE h = cmsCreate_sRGBProfile();
    if (h == nullptr) {
        log_fail(test_name, "Failed to create test profile (srgb).");
        g_failed_tests++;
        return false;
    }

    if (Check_cicp(3, h) != 0) {
        log_fail(test_name, "Expected 0 for unsupported Pass value; got non-zero.");
        cmsCloseProfile(h);
        g_failed_tests++;
        return false;
    }

    cmsCloseProfile(h);
    return true;
}

// Entry point for the test suite
int main() {
    bool overall = true;

    std::cout << "Running unit tests for cmsInt32Number Check_cicp..." << std::endl;

    overall = test_CheckCicp_WriteTag_Succeeds() && overall;
    overall = test_CheckCicp_ReadTag_SucceedsAfterWrite() && overall;
    overall = test_CheckCicp_ReadTag_WithWrongValues_Fails() && overall;
    overall = test_CheckCicp_ReadTag_NoTag_Fails() && overall;
    overall = test_CheckCicp_InvalidPass_ReturnsZero() && overall;

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total tests run: " << g_total_tests << std::endl;
    std::cout << "Tests passed:    " << (g_total_tests - g_failed_tests) << std::endl;
    std::cout << "Tests failed:    " << g_failed_tests << std::endl;

    if (overall) {
        std::cout << "[ALL TESTS PASSED]" << std::endl;
        return 0;
    } else {
        std::cout << "[SOME TESTS FAILED]" << std::endl;
        return 1;
    }
}