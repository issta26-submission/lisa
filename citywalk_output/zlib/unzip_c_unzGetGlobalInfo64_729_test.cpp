// Unit test suite for the focal method: unzGetGlobalInfo64
// This test suite is written in C++11 and does not rely on any external test framework.
// It uses a lightweight in-code harness with non-terminating assertions to maximize coverage.

#include <cstring>
#include <string.h>
#include <stdio.h>
#include <crypt.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>


// Include the header that defines the focal types and function prototypes.
// This header is part of the unzip project and provides: unzFile, unz64_s, unz_global_info64,
// UNZ_OK, UNZ_PARAMERROR, and the prototype for unzGetGlobalInfo64.

// Simple, non-terminating assertion helpers are embedded in test functions.
// The tests report their own pass/fail status, allowing all tests to run in a single pass.


// Test 1: Verify that providing a NULL file pointer returns UNZ_PARAMERROR
// and that the output parameter remains unmodified.
static bool test_unzGetGlobalInfo64_paramError_null_file() {
    // Prepare a non-zero in-memory value for pglobal_info to detect any unintended modification.
    size_t gi_size = sizeof(unz_global_info64);
    if (gi_size == 0) {
        std::cerr << "Test 1: UNZ_GLOBAL_INFO64 size is zero, cannot run test.\n";
        return false;
    }

    // Initialize a dummy unz_global_info64 with a recognizable pattern.
    unz_global_info64 gi;
    std::vector<char> pattern(gi_size);
    for (size_t i = 0; i < gi_size; ++i) {
        pattern[i] = static_cast<char>((i * 13) & 0xFF);
    }
    std::memcpy(&gi, pattern.data(), gi_size);

    // Keep a snapshot to compare after the function call.
    unz_global_info64 gi_before;
    std::memcpy(&gi_before, &gi, gi_size);

    // Call the function with a NULL file pointer.
    int res = unzGetGlobalInfo64(nullptr, &gi);

    // Result must indicate a parameter error.
    bool pass = (res == UNZ_PARAMERROR);

    // The output parameter must remain unchanged.
    if (pass) {
        if (std::memcmp(&gi, &gi_before, gi_size) != 0) {
            std::cerr << "Test 1: FAILED - pglobal_info was modified on NULL file input.\n";
            return false;
        }
    } else {
        std::cerr << "Test 1: FAILED - Expected UNZ_PARAMERROR, got " << res << ".\n";
        return false;
    }

    std::cout << "Test 1: test_unzGetGlobalInfo64_paramError_null_file PASSED\n";
    return true;
}


// Test 2: Verify that a valid file object populates pglobal_info with the same content as the
// internal gi member of the unz64_s structure, and returns UNZ_OK.
static bool test_unzGetGlobalInfo64_successful_copy() {
    // Prepare a non-zero pattern to assign into s.gi so that we can verify an exact copy.
    size_t gi_size = sizeof(unz_global_info64);
    if (gi_size == 0) {
        std::cerr << "Test 2: UNZ_GLOBAL_INFO64 size is zero, cannot run test.\n";
        return false;
    }

    // Create a local unz64_s instance and fill its gi member with a distinct pattern.
    unz64_s s;
    std::vector<char> pattern(gi_size);
    for (size_t i = 0; i < gi_size; ++i) {
        pattern[i] = static_cast<char>((i * 31) & 0xFF);
    }
    std::memcpy(&s.gi, pattern.data(), gi_size);

    // Cast the address of s to unzFile, as the API expects.
    unzFile f = (unzFile)&s;

    // Output container to receive the global info.
    unz_global_info64 gi_out;

    // Invoke the focal method.
    int res = unzGetGlobalInfo64(f, &gi_out);

    // Expect success.
    if (res != UNZ_OK) {
        std::cerr << "Test 2: FAILED - Expected UNZ_OK, got " << res << ".\n";
        return false;
    }

    // The contents of gi_out should exactly match s.gi (byte-for-byte compare).
    if (std::memcmp(&gi_out, &s.gi, gi_size) != 0) {
        std::cerr << "Test 2: FAILED - Output global info differs from source gi.\n";
        return false;
    }

    std::cout << "Test 2: test_unzGetGlobalInfo64_successful_copy PASSED\n";
    return true;
}


// Main driver: run all tests and emit a summary.
int main() {
    int total = 0;
    int passed = 0;

    bool ok;

    ok = test_unzGetGlobalInfo64_paramError_null_file();
    total += 1;
    if (ok) ++passed;

    ok = test_unzGetGlobalInfo64_successful_copy();
    total += 1;
    if (ok) ++passed;

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero if any test failed to aid CI systems.
    return (passed == total) ? 0 : 1;
}