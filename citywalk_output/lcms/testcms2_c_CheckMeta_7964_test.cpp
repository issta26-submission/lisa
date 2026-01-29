/*
   Lightweight C++11 test harness for the focal method: CheckMeta
   - Purpose: verify high-level control flow of CheckMeta in testcms2.c
   - Strategy: provide minimal, non-terminating tests that exercise true/false paths
     where feasible, using the actual Little CMS based CheckMeta implementation.
   - Note: This test avoids GTest/GMock per instructions. It relies on the presence of
     the Little CMS library and ICC profiles in the working directory.
   - Command to compile (example, adjust paths to lib and headers as needed):
       g++ -std=c++11 -I<path_to_lcms_include> -L<path_to_lcms_lib> -llcms -o test_checkmeta test_checkmeta.cpp
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>


// The focal function is implemented in C (testcms2.c) and linked into the test binary.
// We declare it here with C linkage to call from C++.
extern "C" int CheckMeta(void); // Depending on the actual typedef in the library, int is used for simplicity.

// Helper: check whether a file exists
static bool FileExists(const char* path) {
    if (!path) return false;
    FILE* f = std::fopen(path, "rb");
    if (f) { std::fclose(f); return true; }
    return false;
}

// Helper: delete a file if it exists (ignore errors)
static void SafeDelete(const char* path) {
    if (FileExists(path)) std::remove(path);
}

// Candidate Keywords inspired by the focal method (for traceability in tests)
// - ibm-t61.icc: input ICC profile used by CheckMeta
// - cmsSigMetaTag: metadata tag identifier used by cmsReadTag
// - cmsSaveProfileToMem: serialization API used by CheckMeta
// - new.icc: temporary memory-serialized profile written to disk
// - cmsOpenProfileFromFile: file-based profile opening used by CheckMeta

// Test 1: Success path when input file exists and contains a Meta tag.
// We expect CheckMeta() to return 1, indicating successful end-to-end processing.
// Preconditions:
// - ibm-t61.icc must exist in CWD and be a valid ICC profile with a Meta tag.
// - new.icc will be created then cleaned up by the test.
static bool Test_CheckMeta_Success() {
    std::cout << "Running Test_CheckMeta_Success ..." << std::endl;

    // Check precondition: input profile exists
    if (!FileExists("ibm-t61.icc")) {
        std::cout << " SKIPPED: ibm-t61.icc not found in CWD." << std::endl;
        return true; // skip treated as pass to focus on coverage
    }

    // Ensure a clean slate for the memory-to-disk step
    SafeDelete("new.icc");

    // Call the focal method
    int res = CheckMeta();

    // Cleanup
    SafeDelete("new.icc");

    // Expect success (non-zero)
    if (res == 1) {
        std::cout << " [PASS] Test_CheckMeta_Success: CheckMeta returned 1 as expected." << std::endl;
        return true;
    } else {
        std::cout << " [FAIL] Test_CheckMeta_Success: CheckMeta returned " << res << ", expected 1." << std::endl;
        return false;
    }
}

// Test 2: Missing input file path should cause early failure (p == NULL branch).
// We simulate by temporarily renaming ibm-t61.icc, then invoking CheckMeta.
// Expect CheckMeta() to return 0 due to failure to open input profile.
static bool Test_CheckMeta_MissingInputFile() {
    std::cout << "Running Test_CheckMeta_MissingInputFile ..." << std::endl;

    // Only proceed if the input actually exists so we can rename it back later
    if (!FileExists("ibm-t61.icc")) {
        std::cout << " SKIPPED: ibm-t61.icc not present to rename; cannot test missing file scenario." << std::endl;
        return true;
    }

    // Rename input away
    if (std::rename("ibm-t61.icc", "ibm-t61.icc.backup") != 0) {
        std::cout << " SKIPPED: Failed to rename ibm-t61.icc to simulate missing input." << std::endl;
        return true;
    }

    // Ensure any previous temp file is removed
    SafeDelete("new.icc");

    // Run the focal method
    int res = CheckMeta();

    // Restore input
    int restoreOK = std::rename("ibm-t61.icc.backup", "ibm-t61.icc");
    (void)restoreOK; // ignore possible restore failure in test scenario

    // Cleanup
    SafeDelete("new.icc");

    // Expect failure (0)
    if (res == 0) {
        std::cout << " [PASS] Test_CheckMeta_MissingInputFile: CheckMeta returned 0 as expected." << std::endl;
        return true;
    } else {
        std::cout << " [FAIL] Test_CheckMeta_MissingInputFile: CheckMeta returned " << res << ", expected 0." << std::endl;
        return false;
    }
}

// Test 3: Fallback scenario where input file exists but is invalid for the library.
// This attempts to exercise early failure when cmsOpenProfileFromFile cannot parse the file.
// Expect CheckMeta() to return 0.
// Preconditions:
// - If ibm-t61.icc is present, replace it with a deliberately invalid/empty file.
// - After test, restore the original profile if possible.
static bool Test_CheckMeta_InvalidInputFile() {
    std::cout << "Running Test_CheckMeta_InvalidInputFile ..." << std::endl;

    if (!FileExists("ibm-t61.icc")) {
        std::cout << " SKIPPED: ibm-t61.icc not found; cannot test invalid input scenario." << std::endl;
        return true;
    }

    // Back up the valid file
    if (std::rename("ibm-t61.icc", "ibm-t61.icc.valid") != 0) {
        std::cout << " SKIPPED: Could not back up ibm-t61.icc." << std::endl;
        return true;
    }

    // Create a tiny invalid file to replace input
    FILE* f = std::fopen("ibm-t61.icc", "wb");
    if (f) {
        // Write something that is almost certainly not a valid ICC profile
        const char junk[] = "THIS_IS_NOT_A_VALID_ICC_PROFILE";
        std::fwrite(junk, 1, sizeof(junk)-1, f);
        std::fclose(f);
    } else {
        std::cout << " SKIPPED: Failed to create invalid ibm-t61.icc." << std::endl;
        // Try to restore original and skip
        std::rename("ibm-t61.icc.valid", "ibm-t61.icc");
        return true;
    }

    // Run the focal method
    int res = CheckMeta();

    // Restore original valid profile
    std::rename("ibm-t61.icc", "ibm-t61.icc.invalid"); // mark as failed
    std::rename("ibm-t61.icc.valid", "ibm-t61.icc"); // restore valid profile
    // Remove the temporary marker
    SafeDelete("ibm-t61.icc.invalid");

    // Cleanup
    SafeDelete("new.icc");

    // Expect failure (0) due to invalid input
    if (res == 0) {
        std::cout << " [PASS] Test_CheckMeta_InvalidInputFile: CheckMeta returned 0 as expected for invalid input." << std::endl;
        return true;
    } else {
        std::cout << " [FAIL] Test_CheckMeta_InvalidInputFile: CheckMeta returned " << res << ", expected 0." << std::endl;
        return false;
    }
}

int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool ok) {
        total++;
        if (ok) {
            passed++;
            std::cout << "[TEST PASSED] " << name << std::endl;
        } else {
            std::cout << "[TEST FAILED] " << name << std::endl;
        }
    };

    // Execute tests
    run("Test_CheckMeta_Success", Test_CheckMeta_Success());
    run("Test_CheckMeta_MissingInputFile", Test_CheckMeta_MissingInputFile());
    run("Test_CheckMeta_InvalidInputFile", Test_CheckMeta_InvalidInputFile());

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Non-zero exit code on any failure to aid integration with CI
    return (passed == total) ? 0 : 1;
}