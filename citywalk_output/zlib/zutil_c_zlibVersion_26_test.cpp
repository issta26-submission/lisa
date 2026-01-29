/*
Step 1 (Program Understanding and Candidate Keywords):
- Focal method: zlibVersion (C function)
- Returns: a pointer to a string constant representing the library version
- Core dependent components (from <FOCAL_CLASS_DEP> and surrounding context):
  - ZLIB_VERSION macro (string literal)
  - zutil.h (declarations for zlib utilities)
  - zlibVersion function (signature: const char * ZEXPORT zlibVersion(void))
  - ZEXPORT (calling convention macro, may affect linkage)
  - Possible interplay with Z_SOLO, z_errmsg array (not directly exercised here)

Candidate Keywords identified for test design: ZLIB_VERSION, zlibVersion, ZEXPORT, zutil.h, ZLIB_DEBUG (conditional compilation), ZLIB_VERSION content.

Step 2 (Unit Test Generation):
- Objective: Validate zlibVersion returns a non-null pointer and that the returned string content matches the ZLIB_VERSION macro.
- Approach: Provide two targeted tests:
  1) Non-nully verify: zlibVersion() should not return a null pointer.
  2) Content match verify: std::strcmp(zlibVersion(), ZLIB_VERSION) should be 0.
- Environment notes:
  - The test is written in C++11 but uses C linkage to call the C function.
  - We rely on ZLIB_VERSION being defined (as a macro) by zutil.h/associated zlib headers.
  - Static linking concerns or additional debug variants (e.g., ZLIB_DEBUG) should not affect the basic behavior under normal compilation flags.

Step 3 (Test Case Refinement):
- Implement a lightweight, self-contained test harness (no Google Test) that runs the two tests from main.
- Use only standard library facilities (iostream, cstring) and C linkage to call zlibVersion.
- Provide clear, explanatory comments for each test to ensure maintainability and readability.
- Ensure tests are non-terminating in the sense that they print outcomes and do not call abort on failure (we emulate non-terminating reporting while still returning a nonzero exit status if failures occur).

Below is the test suite code (C++11) validating zlibVersion without relying on GTest.

*/

#include <iostream>
#include <stdlib.h>
#include <zutil.h>
#include <cstring>


// Bring in the C interfaces for zlibVersion and related macro.
// The focal method under test resides in zutil.c and is declared in zutil.h.
// We compile this C++ test with C linkage to avoid name mangling issues.
extern "C" {
}

// Test 1: zlibVersion should never return a null pointer.
// Rationale: The function returns a compile-time string literal (ZLIB_VERSION); a null
// pointer would indicate an unexpected linkage or ABI issue.
int test_zlibVersion_returns_non_null() {
    const char *ver = zlibVersion();
    if (ver == nullptr) {
        std::cerr << "Test zlibVersion_returns_non_null: FAILED - zlibVersion() returned null." << std::endl;
        return 1;
    }
    // Non-null result is a necessary condition for further checks.
    return 0;
}

// Test 2: zlibVersion should return the string identical in content to the ZLIB_VERSION macro.
// Rationale: The function is defined as returning ZLIB_VERSION; content equality ensures the
// function and macro are in sync, regardless of pointer identity across translation units.
int test_zlibVersion_matches_macro() {
    const char *ver = zlibVersion();
    if (ver == nullptr) {
        std::cerr << "Test zlibVersion_matches_macro: FAILED - zlibVersion() returned null." << std::endl;
        return 1;
    }

    // Compare the actual string content, not pointer identity.
    // ZLIB_VERSION is a macro expanding to a string literal.
    if (std::strcmp(ver, ZLIB_VERSION) != 0) {
        std::cerr << "Test zlibVersion_matches_macro: FAILED - content mismatch. "
                  << "Expected: " << ZLIB_VERSION << ", Got: " << ver << std::endl;
        return 1;
    }

    return 0;
}

// Simple test harness
int main() {
    std::cout << "Running unit tests for zlibVersion (C linkage)..." << std::endl;

    int failures = 0;

    // Execute Test 1
    int t1 = test_zlibVersion_returns_non_null();
    if (t1 == 0) {
        std::cout << "[PASSED] test_zlibVersion_returns_non_null" << std::endl;
    } else {
        std::cout << "[FAILED] test_zlibVersion_returns_non_null" << std::endl;
        ++failures;
    }

    // Execute Test 2
    int t2 = test_zlibVersion_matches_macro();
    if (t2 == 0) {
        std::cout << "[PASSED] test_zlibVersion_matches_macro" << std::endl;
    } else {
        std::cout << "[FAILED] test_zlibVersion_matches_macro" << std::endl;
        ++failures;
    }

    int total = 2;
    std::cout << "Summary: " << (total - failures) << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed, to integrate with typical build/test pipelines.
    return (failures == 0) ? 0 : 1;
}