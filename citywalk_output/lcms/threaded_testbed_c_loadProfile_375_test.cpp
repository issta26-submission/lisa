// Test suite for cmsHPROFILE loadProfile(const char* name) in threaded_testbed.c
// This test uses a lightweight, non-GTest approach as requested.
// It mocks dependent C functions to simulate library behavior and validates all branches.

#include <cstring>
#include <threaded_internal.h>
#include <iostream>
#include <time.h>
#include <cstdarg>
#include <memory.h>
#include <stdlib.h>


// Pointer type alias to mimic cmsHPROFILE used in the focal file.
typedef void* cmsHPROFILE;

// Forward declaration of the focal function (C linkage)
extern "C" cmsHPROFILE loadProfile(const char* name);

// Mocked dependency functions with C linkage to override real library calls during tests.
extern "C" {

    // Sentinel values returned by the mocked profile creation/opening functions
    static cmsHPROFILE g_lab_sentinel     = reinterpret_cast<cmsHPROFILE>(0x11111111);
    static cmsHPROFILE g_xyz_sentinel     = reinterpret_cast<cmsHPROFILE>(0x22222222);
    static cmsHPROFILE g_curves_sentinel  = reinterpret_cast<cmsHPROFILE>(0x33333333);
    static cmsHPROFILE g_open_sentinel    = reinterpret_cast<cmsHPROFILE>(0x44444444);

    // Flags to verify path taken in loadProfile
    static bool g_fail_called       = false;
    static bool g_openFromFile_called = false;
    static const char* g_openFromFile_name = nullptr;
    static const char* g_openFromFile_mode = nullptr;

    // Reset all mock state before each test
    void reset_mocks() {
        g_fail_called = false;
        g_openFromFile_called = false;
        g_openFromFile_name = nullptr;
        g_openFromFile_mode = nullptr;
    }

    // Mock implementations of the dependencies
    cmsHPROFILE cmsCreateLab4Profile(void* /*ctx*/) {
        // Return sentinel to indicate Lab profile creation path was taken
        return g_lab_sentinel;
    }

    cmsHPROFILE cmsCreateXYZProfile(void) {
        // Return sentinel to indicate XYZ profile creation path was taken
        return g_xyz_sentinel;
    }

    cmsHPROFILE CreateCurves(void) {
        // Return sentinel to indicate Curves creation path was taken
        return g_curves_sentinel;
    }

    void Fail(const char* /*frm*/, ...) {
        // Record that a failure path was invoked
        g_fail_called = true;
        // Do not terminate; keep tests non-terminating as requested
    }

    cmsHPROFILE cmsOpenProfileFromFile(const char* name, const char* mode) {
        // Record call details for assertion and return a sentinel to indicate file path usage
        g_openFromFile_called = true;
        g_openFromFile_name = name;
        g_openFromFile_mode = mode;
        return g_open_sentinel;
    }
}

// Helper to report test outcomes
static int g_total_tests_run = 0;
static int g_tests_failed = 0;

static void report_result(const std::string& test_name, bool success) {
    ++g_total_tests_run;
    if (success) {
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        ++g_tests_failed;
        std::cout << "[FAIL] " << test_name << std::endl;
    }
}

// Tests

// Test 1: Builtin "*lab" should return Lab4 profile sentinel and should not call OpenFromFile
void test_loadProfile_builtin_lab() {
    reset_mocks();
    cmsHPROFILE prof = loadProfile("*lab");

    bool passed = (prof == reinterpret_cast<cmsHPROFILE>(0x11111111)) // g_lab_sentinel
                  && !g_openFromFile_called
                  && !g_fail_called;

    report_result("loadProfile(\"*lab\") returns Lab4 sentinel and does not call open", passed);
}

// Test 2: Builtin "*xyz" should return XYZ profile sentinel and should not call OpenFromFile
void test_loadProfile_builtin_xyz() {
    reset_mocks();
    cmsHPROFILE prof = loadProfile("*xyz");

    bool passed = (prof == reinterpret_cast<cmsHPROFILE>(0x22222222)) // g_xyz_sentinel
                  && !g_openFromFile_called
                  && !g_fail_called;

    report_result("loadProfile(\"*xyz\") returns XYZ sentinel and does not call open", passed);
}

// Test 3: Builtin "*curves" should return Curves profile sentinel and should not call OpenFromFile
void test_loadProfile_builtin_curves() {
    reset_mocks();
    cmsHPROFILE prof = loadProfile("*curves");

    bool passed = (prof == reinterpret_cast<cmsHPROFILE>(0x33333333)) // g_curves_sentinel
                  && !g_openFromFile_called
                  && !g_fail_called;

    report_result("loadProfile(\"*curves\") returns Curves sentinel and does not call open", passed);
}

// Test 4: Unknown builtin with '*' should trigger Fail and then fall back to OpenFromFile
void test_loadProfile_builtin_unknown_triggers_fail_and_falls_back() {
    reset_mocks();
    cmsHPROFILE prof = loadProfile("*unknown_builtin");

    bool passed = (prof == reinterpret_cast<cmsHPROFILE>(0x44444444)) // g_open_sentinel
                  && g_fail_called; // Fail should have been invoked

    report_result("loadProfile(\"*unknown_builtin\") calls Fail and then opens file path", passed);
}

// Test 5: Non-builtin path should call OpenFromFile and return its sentinel
void test_loadProfile_non_builtin_opens_file() {
    reset_mocks();
    const char* testName = "sample_profile.icc";
    cmsHPROFILE prof = loadProfile(testName);

    bool passed =
        (prof == reinterpret_cast<cmsHPROFILE>(0x44444444)) // g_open_sentinel
        && g_openFromFile_called
        && g_openFromFile_name == testName
        && g_openFromFile_mode != nullptr
        && !g_fail_called;

    report_result("loadProfile(\"sample_profile.icc\") opens file and returns OpenFromFile sentinel", passed);
}

// Main function to run tests
int main() {
    std::cout << "Starting tests for loadProfile(...)" << std::endl;

    test_loadProfile_builtin_lab();
    test_loadProfile_builtin_xyz();
    test_loadProfile_builtin_curves();
    test_loadProfile_builtin_unknown_triggers_fail_and_falls_back();
    test_loadProfile_non_builtin_opens_file();

    std::cout << "Tests completed. Total: " << g_total_tests_run
              << ", Passed: " << (g_total_tests_run - g_tests_failed)
              << ", Failed: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return g_tests_failed ? 1 : 0;
}