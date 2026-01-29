/*
 * Lightweight C++11 unit test suite for the focal method:
 *     int CheckForgedMPE(void)
 * 
 * Notes and constraints:
 * - Uses the real production function CheckForgedMPE (from the project under test).
 * - No Google Test framework; tests are executed from main().
 * - Tests are designed to be executable with only the C/C++ standard library and the linked project library.
 * - We focus on observable behavior (return values) under controlled file-system conditions.
 * - Because the production code depends on external ICC profile files and internal state, we implement
 *   conservative tests that exercise the most deterministic branches available without requiring
 *   specific ICC profile contents.
 * - Each test is annotated with comments describing the intent and the observable outcome.
 */

#include <cstdio>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cstdlib>


extern "C" int CheckForgedMPE(void); // Declaration of the focal method from the production code

// Simple test harness utilities

static bool equals(int a, int b) {
    return a == b;
}

static void log_pass(const std::string& testName) {
    std::cout << "[PASS] " << testName << "\n";
}

static void log_fail(const std::string& testName, const std::string& reason) {
    std::cerr << "[FAIL] " << testName << " - " << reason << "\n";
}

// Test 1: CheckForgedMPE returns 0 when the required file "bad_mpe.icc" does not exist.
// Rationale: The method begins by attempting to open "bad_mpe.icc". If the file is missing,
// cmsOpenProfileFromFile should fail and the function should return 0 (falsey path).
static bool Test_CheckForgedMPE_MissingFile_ReturnsZero() {
    const char* fname = "bad_mpe.icc";

    // Ensure the file does not exist in the current working directory.
    std::remove(fname); // best-effort; ignore return value

    // Invoke the function under test
    int res = CheckForgedMPE();

    // Expect 0 (failure to open profile)
    if (equals(res, 0)) {
        log_pass("Test_CheckForgedMPE_MissingFile_ReturnsZero");
        return true;
    } else {
        log_fail("Test_CheckForgedMPE_MissingFile_ReturnsZero",
                 "Expected 0 when profile file is missing, got " + std::to_string(res));
        return false;
    }
}

// Test 2: CheckForgedMPE returns 0 when the file exists but is not a valid ICC profile.
// Rationale: If "bad_mpe.icc" exists but is not a valid ICC profile, cmsOpenProfileFromFile should fail
// and the function should return 0. This exercises the negative path where opening the profile fails.
static bool Test_CheckForgedMPE_InvalidFile_ReturnsZero() {
    const char* fname = "bad_mpe.icc";

    // Create an empty (invalid) file to simulate a corrupt/invalid ICC profile.
    FILE* f = std::fopen(fname, "wb");
    if (!f) {
        log_fail("Test_CheckForgedMPE_InvalidFile_ReturnsZero",
                 "Unable to create test file " + std::string(fname));
        return false;
    }
    std::fclose(f);

    // Invoke the function under test
    int res = CheckForgedMPE();

    // Cleanup to avoid side effects on subsequent tests
    std::remove(fname);

    // Expect 0 (failure to open a valid profile should yield 0)
    if (equals(res, 0)) {
        log_pass("Test_CheckForgedMPE_InvalidFile_ReturnsZero");
        return true;
    } else {
        log_fail("Test_CheckForgedMPE_InvalidFile_ReturnsZero",
                 "Expected 0 for invalid ICC file, got " + std::to_string(res));
        return false;
    }
}

// Note:
// - Additional tests to cover the LabData branch (srcCS == cmsSigLabData) or the
//   hTransform NULL branch would require constructing or supplying specific ICC profiles
//   that trigger those code paths. Without access to a controlled ICC generation facility
//   within the test harness, those branches are difficult to deterministically exercise
//   in a portable, self-contained test. The two tests above establish baseline
//   behavior for failure paths that are reliably reproducible in most environments.

int main() {
    int failures = 0;

    // Run Test 1
    if (!Test_CheckForgedMPE_MissingFile_ReturnsZero()) {
        ++failures;
    }

    // Run Test 2
    if (!Test_CheckForgedMPE_InvalidFile_ReturnsZero()) {
        ++failures;
    }

    // Summary
    if (failures == 0) {
        std::cout << "[SUMMARY] All tests passed.\n";
        return 0;
    } else {
        std::cerr << "[SUMMARY] " << failures << " test(s) failed.\n";
        return failures;
    }
}