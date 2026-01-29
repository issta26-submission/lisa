// Unit test suite for the focal method fill_win32_filefunc in iowin32.c
// This test is written for a C/C++ mixed build where iowin32.c is compiled together
// with this test. It uses the real zlib ioapi structs/types from zlib.h.
// No GoogleTest is used; a simple C++11 test harness is provided.
// Build guidance (example):
// g++ test_fill_win32_filefunc.cpp iowin32.c -lz -std=c++11 -o test_fill_win32_filefunc
// Ensure zlib headers are accessible (the include path may vary based on your environment).

#include <cstring>
#include <iowin32.h>
#include <cstdint>
#include <iostream>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>


// Include zlib ioapi types and definitions
extern "C" {
}

// The iowin32.c implementation defines several function pointers like zopen_file, zread_file, etc.
// The actual Windows-specific functions (win32_open_file_func, etc.) live in that translation unit.
// For test purposes, we only verify that fill_win32_filefunc assigns non-null function pointers
// and sets opaque to NULL as intended by the code under test.

// Test 1: Basic correctness of fill_win32_filefunc
// - Verifies that after calling fill_win32_filefunc, opaque is NULL.
// - Verifies that all relevant function pointers are non-NULL (i.e., assigned).
bool test_fill_win32_filefunc_basic() {
    // Prepare a zero-initialized descriptor similar to how the library would use it.
    zlib_filefunc_def def;
    std::memset(&def, 0, sizeof(def));

    // Call the focal method to set up the Win32 file functions.
    fill_win32_filefunc(&def);

    // Check that opaque is explicitly set to NULL.
    bool ok = (def.opaque == NULL);

    // Check that all required function pointers have been assigned (i.e., non-NULL).
    bool pointers_non_null =
        (def.zopen_file != NULL) &&
        (def.zread_file != NULL) &&
        (def.zwrite_file != NULL) &&
        (def.ztell_file != NULL) &&
        (def.zseek_file != NULL) &&
        (def.zclose_file != NULL) &&
        (def.zerror_file != NULL);

    if (!ok) {
        std::cerr << "[Test1] FAIL: opaque should be NULL after fill_win32_filefunc().\n";
    }
    if (!pointers_non_null) {
        std::cerr << "[Test1] FAIL: One or more file function pointers are NULL after fill_win32_filefunc().\n";
    }

    return ok && pointers_non_null;
}

// Test 2: Idempotence / overwrite behavior (basic sanity check)
// - Ensures that re-invoking fill_win32_filefunc on an already-populated struct overwrites
//   its member function pointers with the expected non-NULL values again.
// - In this test, we pre-initialize the struct with a known sentinel value for a function pointer.
//   Since the actual type is a function pointer, we avoid directly assigning arbitrary values.
// - The test then calls fill_win32_filefunc and asserts that the pointers become non-NULL again.
bool test_fill_win32_filefunc_overwrite() {
    zlib_filefunc_def def;
    // Initialize with zeros first
    std::memset(&def, 0, sizeof(def));

    // First call to fill_win32_filefunc
    fill_win32_filefunc(&def);

    // Basic sanity check after first call
    if (def.opaque != NULL) {
        std::cerr << "[Test2] WARN: After first fill, opaque was not NULL as expected in some environments.\n";
    }
    if (!(def.zopen_file && def.zread_file && def.zwrite_file && def.ztell_file &&
          def.zseek_file && def.zclose_file && def.zerror_file)) {
        std::cerr << "[Test2] WARN: Pointers are not all non-NULL after first fill.\n";
    }

    // Re-invoke fill_win32_filefunc to verify overwrite behavior (no crash expected)
    fill_win32_filefunc(&def);

    // After second fill, pointers should still be non-NULL and opaque should be NULL
    bool ok = (def.opaque == NULL) &&
              (def.zopen_file != NULL) &&
              (def.zread_file != NULL) &&
              (def.zwrite_file != NULL) &&
              (def.ztell_file != NULL) &&
              (def.zseek_file != NULL) &&
              (def.zclose_file != NULL) &&
              (def.zerror_file != NULL);

    if (!ok) {
        std::cerr << "[Test2] FAIL: fill_win32_filefunc did not properly overwrite existing pointers.\n";
    }

    return ok;
}

// Test 3: Callability safety (non-functional check)
// - We intentionally do not call the function pointers themselves since the code snippet
//   for the Win32 functions in iowin32.c uses stubs in this test environment.
// - We instead ensure that the API surface is present and that the test harness can at least
//   safely execute fill_win32_filefunc without crashing.
bool test_fill_win32_filefunc_safety() {
    zlib_filefunc_def def;
    std::memset(&def, 0, sizeof(def));

    // This should not crash
    fill_win32_filefunc(&def);

    // Basic safety: pointers were set to something non-null
    bool safety = (def.zopen_file != NULL) &&
                  (def.zread_file != NULL) &&
                  (def.zwrite_file != NULL) &&
                  (def.ztell_file != NULL) &&
                  (def.zseek_file != NULL) &&
                  (def.zclose_file != NULL) &&
                  (def.zerror_file != NULL);

    if (!safety) {
        std::cerr << "[Test3] FAIL: Safety check failed; some function pointers are NULL after fill.\n";
    }
    return safety;
}

int main() {
    int tests_passed = 0;
    int total_tests = 3;

    std::cout << "Running tests for fill_win32_filefunc (iowin32.c) with C/C++ harness...\n";

    if (test_fill_win32_filefunc_basic()) {
        std::cout << "[Test 1] PASS: basic pointers and opaque initialization are correct.\n";
        ++tests_passed;
    } else {
        std::cout << "[Test 1] FAIL: basic pointers/opaque initialization incorrect.\n";
    }

    if (test_fill_win32_filefunc_overwrite()) {
        std::cout << "[Test 2] PASS: overwrite behavior preserved non-NULL pointers after re-fill.\n";
        ++tests_passed;
    } else {
        std::cout << "[Test 2] FAIL: overwrite behavior incorrect.\n";
    }

    if (test_fill_win32_filefunc_safety()) {
        std::cout << "[Test 3] PASS: safety checks for function pointers OK.\n";
        ++tests_passed;
    } else {
        std::cout << "[Test 3] FAIL: safety checks failed.\n";
    }

    std::cout << "Summary: " << tests_passed << " / " << total_tests << " tests passed.\n";

    // Return non-zero if any test failed
    return (tests_passed == total_tests) ? 0 : 1;
}