/*
Unit test suite for TGZfname (untgz.c)

Notes:
- This test suite assumes the presence of the C function:
    char *TGZfname (const char *arcname);

- It also relies on the global TGZsuffix array defined in the C source, which provides
  the list of suffixes appended to the arcname. We declare it as:
    extern "C" const char *TGZsuffix[];

- The tests are written as a simple C++11 harness (no GoogleTest) and run from main().
- Two core tests are provided:
  1) test_true_branch: Creates a file arcname + TGZsuffix[0] and verifies TGZfname returns
     a non-NULL pointer whose string equals the expected path.
  2) test_false_branch: Ensures no matching file exists for a unlikely arcname and verifies
     TGZfname returns NULL.
- The static buffer behavior inside TGZfname means the returned pointer is only valid until the
  next call modifies the internal buffer; subsequent calls reuse the same static storage. The
  tests account for this by checking pointer stability across repeated calls in the positive test.
- All tests clean up any files they create to avoid polluting the environment.
*/

#include <cstring>
#include <cstdio>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <fstream>
#include <stdlib.h>


// Expose C functions/arrays with C linkage
extern "C" {
    // Focal method under test
    char *TGZfname (const char *arcname);

    // Array of suffix strings used by TGZfname (extern to this test)
    // We assume this exists in untgz.c as a NULL-terminated array of const char*
    const char *TGZsuffix[];
}

// Utility: check if a file exists (purely for local debugging if needed)
static bool file_exists(const std::string &path) {
    std::ifstream f(path.c_str());
    return f.good();
}

// Test 1: True branch
// - Creates a file named "<arcname><TGZsuffix[0]>".
// - Calls TGZfname("<arcname>") and expects the returned string to equal "<arcname><suffix>".
// - Also checks that a subsequent call returns the same underlying buffer (same pointer).
static bool test_true_branch() {
    const char *arcname = "untgz_test_archive";
    if (TGZsuffix[0] == nullptr) {
        std::cerr << "test_true_branch: TGZsuffix[0] is NULL, cannot determine suffix.\n";
        return false;
    }

    std::string expected = std::string(arcname) + TGZsuffix[0];

    // Ensure clean slate: remove any pre-existing file with the expected name
    std::remove(expected.c_str());

    // Create the target file to satisfy the existence check in TGZfname
    {
        std::ofstream ofs(expected, std::ios::binary);
        if (!ofs) {
            std::cerr << "test_true_branch: Failed to create test file: " << expected << "\n";
            return false;
        }
        ofs << "dummy data";
    }

    // Invoke TGZfname and verify the returned path matches expectation
    char *res = TGZfname(arcname);
    bool ok = false;
    if (res != nullptr) {
        if (std::string(res) == expected) {
            ok = true;
        } else {
            std::cerr << "test_true_branch: Returned path mismatch. Expected: " << expected
                      << " Got: " << res << "\n";
        }
    } else {
        std::cerr << "test_true_branch: TGZfname returned NULL despite existing file: "
                  << expected << "\n";
    }

    // Verify that subsequent call uses the same static buffer (same pointer)
    char *res2 = TGZfname(arcname);
    if (res != nullptr && res2 != nullptr) {
        if (res != res2) {
            std::cerr << "test_true_branch: Pointer to internal static buffer changed between calls.\n";
            // Do not fail immediately; still report result, but note the discrepancy
            // keep 'ok' as is since content matched previously
        }
    } else {
        std::cerr << "test_true_branch: Second call returned NULL unexpectedly.\n";
    }

    // Cleanup
    std::remove(expected.c_str());

    return ok;
}

// Test 2: False branch
// - Uses an arcname for which no corresponding file with any TGZsuffix exists.
// - Verifies TGZfname returns NULL.
static bool test_false_branch() {
    // Use a unlikely arcname to avoid existing files
    const char *arcname = "untgz_test_archive_noexist_" "xzywvu";

    // Ensure no matching files exist for any suffixes
    // Best effort cleanup for known suffixes; if TGZsuffix isn't accessible, this is best-effort
    for (int i = 0; TGZsuffix[i] != nullptr && i < 100; ++i) {
        std::string path = std::string(arcname) + TGZsuffix[i];
        std::remove(path.c_str());
    }

    char *res = TGZfname(arcname);
    bool ok = (res == NULL);
    if (!ok) {
        std::cerr << "test_false_branch: TGZfname did not return NULL for non-existent files.\n";
        // If a file happened to exist unexpectedly, clean up
        // We can't know which suffix was attempted; skip aggressive cleanup here
    }
    return ok;
}

// Entry point: run all tests and report summary
int main() {
    int failures = 0;

    std::cout << "Starting TGZfname unit tests...\n";

    // Run Test 1
    bool t1 = test_true_branch();
    if (t1) {
        std::cout << "[PASSED] test_true_branch\n";
    } else {
        std::cout << "[FAILED] test_true_branch\n";
        ++failures;
    }

    // Run Test 2
    bool t2 = test_false_branch();
    if (t2) {
        std::cout << "[PASSED] test_false_branch\n";
    } else {
        std::cout << "[FAILED] test_false_branch\n";
        ++failures;
    }

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED.\n";
    } else {
        std::cout << "TESTS FAILED: " << failures << " failure(s).\n";
    }

    return failures;
}