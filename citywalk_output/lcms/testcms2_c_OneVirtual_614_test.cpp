/*
    Lightweight C++11 unit test suite for the focal method:
    cmsInt32Number OneVirtual(cmsHPROFILE h, const char* SubTestTxt, const char* FileName)

    Assumptions and approach:
    - The test targets the observable behavior of OneVirtual as exercised by the provided
      code path. We rely on the actual Little CMS (lcms) style API availability
      (cmsHPROFILE, cmsInt32Number, cmsSaveProfileToFile, cmsOpenProfileFromFile, cmsCloseProfile, etc.)
      being available in the build environment. This test suite uses Create_AboveRGB() to
      obtain a valid profile handle when needed.
    - We do not use Google Test or Google Mock (per instructions).
    - We implement a minimal, non-terminating test harness that reports pass/fail per test
      and summarizes results at the end.
    - We exercise true/false branches where feasible:
      1) h == NULL path
      2) cmsSaveProfileToFile failure (simulated by FileName = NULL)
      3) cmsSaveProfileToFile success path followed by cmsOpenProfileFromFile success path

    Notes:
    - If cmsOpenProfileFromFile returns NULL in a test, the test will reflect that outcome.
    - Clean up any files created during tests to avoid side effects.
    - The tests print per-test results and a final summary; they do not terminate the test run on first failure.
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <lcms2.h>


// Include Little CMS header to get proper type definitions and function prototypes
// Adjust the include path as necessary for your environment.
// If your environment uses <lcms2.h> or a different header name, update accordingly.

// Forward declarations for functions likely provided by the test harness or the library.
// If these symbols are not available in your environment, remove or adjust them accordingly.
extern "C" {
    // The focal function under test
    cmsInt32Number OneVirtual(cmsHPROFILE h, const char* SubTestTxt, const char* FileName);

    // Helper to generate a sample profile for testing (assumed to exist in the environment)
    cmsHPROFILE Create_AboveRGB(void);

    // The test harness provides SubTest() for logging; we keep using it as-is from testcms2.h
    // If not present in your build, you can safely remove reliance on SubTestTxt by adjusting tests.
}

// Simple cross-platform file existence check
static bool FileExists(const char* path) {
    if (!path) return false;
    FILE* f = std::fopen(path, "rb");
    if (f) { std::fclose(f); return true; }
    return false;
}

// Test 1: Pass NULL profile handle; OneVirtual should return 0 (false path)
static bool test_OneVirtual_NullHandle() {
    cmsHPROFILE h = NULL;
    const char* fname = "ov_nullhandle.prof"; // FileName is not used due to early return
    cmsInt32Number res = OneVirtual(h, "Null handle test", fname);

    bool ok = (res == 0);
    if (!ok) {
        std::fprintf(stderr, "test_OneVirtual_NullHandle: expected 0, got %d\n", res);
    }
    // No file should be created; no cleanup needed
    return ok;
}

// Test 2: Valid profile handle with a valid FileName should save, reopen, and return 1
// Verifies the "success path" where all branches complete.
static bool test_OneVirtual_ValidProfile_WritesFile() {
    cmsHPROFILE h = Create_AboveRGB();
    if (h == NULL) {
        std::fprintf(stderr, "test_OneVirtual_ValidProfile_WritesFile: Create_AboveRGB() returned NULL\n");
        return false;
    }

    const char* fname = "ov_valid_profile.prof";
    cmsInt32Number res = OneVirtual(h, "Valid profile test", fname);

    bool ret_ok = (res == 1);
    bool file_created = FileExists(fname);

    // Cleanup: attempt to remove the file if created
    if (file_created) {
        std::remove(fname);
    }

    if (!ret_ok) {
        std::fprintf(stderr, "test_OneVirtual_ValidProfile_WritesFile: expected 1, got %d\n", res);
        return false;
    }
    if (!file_created) {
        std::fprintf(stderr, "test_OneVirtual_ValidProfile_WritesFile: expected file '%s' to be created, but it was not\n", fname);
        return false;
    }

    return true;
}

// Test 3: Non-NULL profile with NULL FileName should fail at cmsSaveProfileToFile stage
// This covers the false branch of the cmsSaveProfileToFile predicate.
static bool test_OneVirtual_NullFileName() {
    cmsHPROFILE h = Create_AboveRGB();
    if (h == NULL) {
        std::fprintf(stderr, "test_OneVirtual_NullFileName: Create_AboveRGB() returned NULL\n");
        return false;
    }

    cmsInt32Number res = OneVirtual(h, "Null FileName test", NULL);

    bool ok = (res == 0);
    if (!ok) {
        std::fprintf(stderr, "test_OneVirtual_NullFileName: expected 0, got %d\n", res);
    }
    return ok;
}

// Minimal test runner
int main() {
    std::cout << "Starting OneVirtual unit tests (C++11) ..." << std::endl;

    int total = 0;
    int passed = 0;

    // Test 1
    ++total;
    if (test_OneVirtual_NullHandle()) {
        ++passed;
        std::cout << "[PASSED] test_OneVirtual_NullHandle" << std::endl;
    } else {
        std::cout << "[FAILED] test_OneVirtual_NullHandle" << std::endl;
    }

    // Test 2
    ++total;
    if (test_OneVirtual_ValidProfile_WritesFile()) {
        ++passed;
        std::cout << "[PASSED] test_OneVirtual_ValidProfile_WritesFile" << std::endl;
    } else {
        std::cout << "[FAILED] test_OneVirtual_ValidProfile_WritesFile" << std::endl;
    }

    // Test 3
    ++total;
    if (test_OneVirtual_NullFileName()) {
        ++passed;
        std::cout << "[PASSED] test_OneVirtual_NullFileName" << std::endl;
    } else {
        std::cout << "[FAILED] test_OneVirtual_NullFileName" << std::endl;
    }

    std::cout << "OneVirtual test results: " << passed << " / " << total << " tests passed." << std::endl;

    return (passed == total) ? 0 : 1;
}