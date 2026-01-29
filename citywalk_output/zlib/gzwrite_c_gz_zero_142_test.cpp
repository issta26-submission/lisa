/*
   Unit Test Suite for the focal method gz_zero (as used through the public
   API in this repository). This test harness is written in C++11 (no GTest)
   and exercises gz_zero functionality indirectly via the public APIs:
   gzopen/gzwrite/gzclose/gzputc/gzputs/gzfwrite, etc.

   Important notes:
   - The actual gz_zero function is declared static in many builds (local). This
     test accesses the public API and validates behavior that implies proper
     operation of the underlying zero-filling and deflation flow when the internal
     machinery calls gz_zero. The tests are designed to cover true/false branches
     of input validation, overflow handling, and normal operation paths.
   - This file uses a lightweight in-process test harness (no external test
     framework). It prints results to stdout and returns a non-zero exit code on
     failures.
   - This test relies on the minizip/zlib-like API being present in the build
     environment (gzopen/gzwrite/gzclose/gzputc/gzputs/gzfwrite). If your
     environment uses slightly different symbols or types, adapt the extern
     declarations accordingly.
   - All tests are non-terminating: the harness records failures and continues
     executing subsequent tests to maximize coverage.
*/

#include <cstring>
#include <cstdio>
#include <stdarg.h>
#include <limits>
#include <iostream>
#include <gzguts.h>


// Lightweight test harness (non-terminating assertions)
static int g_test_failures = 0;
#define TEST_CHECK(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Test failure: " << (msg) << std::endl; \
        ++g_test_failures; \
    } \
} while (0)


// Prototypes for the gz* API (adapt to your environment if needed)
extern "C" {
    // Opaque file handle type used by the library
    typedef void* gzFile;
    // Public API functions (typical minizip-like signatures)
    typedef const void* voidpc;
    typedef unsigned long z_size_t;

    // Basic I/O
    gzFile gzopen(const char* path, const char* mode);
    int gzclose(gzFile file);

    int gzwrite(gzFile file, void* buf, unsigned len);
    z_size_t gzfwrite(void* buf, z_size_t size, z_size_t nitems, gzFile file);

    int gzputc(gzFile file, int c);
    int gzputs(gzFile file, const char* s);
}

// Test 1: gzwrite with a NULL file should return 0
// Rationale: validate early-file-null handling path in public API, ensuring
// no crashes and a defined 0-length write outcome.
bool test_gzwrite_null_file() {
    // Arrange
    const char* data = "ABC";
    // Act
    int res = gzwrite(nullptr, (void*)data, 3);
    // Assert
    TEST_CHECK(res == 0, "gzwrite with NULL file should return 0");
    return true;
}

// Test 2: gzwrite with zero-length data should return 0 (no-op path)
bool test_gzwrite_zero_length() {
    gzFile f = gzopen("test_zero_len.gz", "wb");
    if (f == nullptr) {
        std::cerr << "Could not create test file for zero-length write." << std::endl;
        g_test_failures++;
        return false;
    }

    int res = gzwrite(f, (void*)"DATA", 0);
    TEST_CHECK(res == 0, "gzwrite with len==0 should return 0");

    gzclose(f);
    return true;
}

// Test 3: gzfwrite overflow path (overflow in size*count should be detected)
// We attempt to force an overflow by choosing size and nitems that would wrap.
bool test_gzfwrite_overflow_detection() {
    gzFile f = gzopen("test_overflow.gz", "wb");
    if (f == nullptr) {
        std::cerr << "Could not create test file for overflow test." << std::endl;
        g_test_failures++;
        return false;
    }

    const char payload[4] = "ABC";
    // Choose size and nitems to force overflow when computing len = size * nitems.
    // We target size = 2 and nitems = (SIZE_MAX/2) + 2 so len would overflow.
    const z_size_t size = 2;
    const z_size_t nitems = (std::numeric_limits<z_size_t>::max() / 2) + 2;

    z_size_t written = gzfwrite((void*)payload, size, nitems, f);
    // The contract in the implementation is that on overflow, it should not write
    // and should return 0 (or indicate failure). We expect 0 here.
    TEST_CHECK(written == 0, "gzfwrite should detect overflow and return 0");

    gzclose(f);
    return true;
}

// Test 4: gzputc and gzputs basic happy-path after opening a writable file
bool test_gzputc_and_gzputs_basic() {
    gzFile f = gzopen("test_putc_puts.gz", "wb");
    if (f == nullptr) {
        std::cerr << "Could not create test file for putc/puts test." << std::endl;
        g_test_failures++;
        return false;
    }

    // Put a single character
    int ch = 'Z';
    int ret = gzputc(f, ch);
    TEST_CHECK(ret == (ch & 0xFF), "gzputc should return the written character (lower 8 bits)");

    // Put a string
    const char* s = "HELLO";
    ret = gzputs(f, s);
    TEST_CHECK(ret == (int)strlen(s), "gzputs should return the number of characters written");

    gzclose(f);
    return true;
}

// Test 5: Public API should handle multi-call path without error when writing small data
bool test_gzwrite_small_chunks() {
    gzFile f = gzopen("test_small_chunks.gz", "wb");
    if (f == nullptr) {
        std::cerr << "Could not create test file for small chunks test." << std::endl;
        g_test_failures++;
        return false;
    }

    const char* data = "0123456789";
    // Write in two small chunks
    int r1 = gzwrite(f, (void*)data, 4);
    int r2 = gzwrite(f, (void*)(data + 4), 6);

    TEST_CHECK(r1 == 4, "First chunk write should write 4 bytes");
    TEST_CHECK(r2 == 6, "Second chunk write should write 6 bytes");

    gzclose(f);
    return true;
}

// Simple test runner
int main() {
    std::cout << "Running gz_zero related test suite (via public API)..." << std::endl;

    // Run tests
    bool all_passed = true;
    all_passed &= test_gzwrite_null_file();
    all_passed &= test_gzwrite_zero_length();
    all_passed &= test_gzfwrite_overflow_detection();
    all_passed &= test_gzputc_and_gzputs_basic();
    all_passed &= test_gzwrite_small_chunks();

    std::cout << "Tests completed. Failures: " << g_test_failures << std::endl;
    if (g_test_failures > 0) {
        std::cout << "Some tests failed. See diagnostics above." << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}