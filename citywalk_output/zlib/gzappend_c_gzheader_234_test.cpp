// C++11 test harness for the focal function gzheader in gzappend.c
// This test suite is designed to be compiled as a single translation unit
// by including the C source file gzappend.c into this test file.
// It provides a minimal mock of the file structure expected by gzheader
// and exercises several branches of the header parsing logic without
// triggering abnormal termination (bye()).

#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <gzappend.c>
#include <cstddef>
#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>


// Forward declaration of the focal function to enable calling it from tests.
// We declare a static (internal) function prototype matching the real
// signature used in gzappend.c. The actual static function in gzappend.c
// will be compatible with this prototype when this test file is compiled
// in the same translation unit.
typedef struct file {
    const char *name;
    unsigned char *data;
    size_t size;
    size_t pos;
} file;

static void gzheader(file *in);

// Include the focal C source file so that static functions become
// accessible within this translation unit (as per test harness design).
// Note: This approach relies on the test being compiled as C++ and the
// included C file using compatible types/structs (see 'file' above).
extern "C" {
}

// Utility: a simple, lightweight assertion macro that returns false
// from the test if the condition fails. We avoid terminating the
// entire test suite to maximize code execution.
#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Assertion failed: " << (msg) << " (in " << __FILE__ << ":" << __LINE__ << ")\n"; \
        return false; \
    } \
} while (0)

// Helper to invoke gzheader on a crafted buffer.
// Returns true if gzheader returns normally, false if a crash or undefined behavior occurs.
// We cannot catch all runtime errors robustly in such a harness, but normal return is expected.
static bool run_gzheader_with_buffer(const unsigned char *buf, size_t len, const char *name) {
    // Prepare a modifiable buffer for the input file
    unsigned char *data = new unsigned char[len];
    std::memcpy(data, buf, len);

    file in;
    in.name = name;
    in.data = data;
    in.size = len;
    in.pos = 0;

    // Call the focal function
    // If gzheader ever calls bye() (which may terminate the program),
    // this test cannot recover gracefully. We ensure test data avoids
    // bye() conditions.
    gzheader(&in);

    delete[] data;
    return true;
}

// Test 1: Valid gzip header with no optional flags (flags = 0)
// Expected: gzheader completes without calling bye() and without crashes.
static bool test_gzheader_flags_zero() {
    // Structure:
    // [1] 0x1F, [2] 0x8B (gzip magic)
    // [3] compression method: 0x08 (deflate)
    // [4] flags: 0x00
    // [5..10] 6 bytes of leftover header data (skipped)
    unsigned char data[] = {
        0x1F, 0x8B, 0x08, 0x00, // signature and method
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // flags-agnostic bytes to skip
    };
    // Length is exactly 10 bytes
    return run_gzheader_with_buffer(data, sizeof(data), "test_gzheader_flags_zero");
}

// Test 2: Flags include 4 (FEXTRA) with zero extra data length
// Expected: header parsed, skip length of 0 used, no bye().
// After this branch, function returns.
static bool test_gzheader_flags_four_with_zero_extra() {
    // [1-4] as above
    // [5..10] 6 bytes of header data (skipped)
    // [11-12] n = 0 (low, high)
    unsigned char data[] = {
        0x1F, 0x8B, 0x08, 0x04, // signature, method, flags (FEXTRA)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 6 bytes skip
        0x00, 0x00  // n = 0 + (0<<8)
    };
    return run_gzheader_with_buffer(data, sizeof(data), "test_gzheader_flags_four_with_zero_extra");
}

// Test 3: Flags include 8 (FNAME) with immediate zero terminator for the name field
// Expected: after 6-byte skip, read one byte which is 0, exit loop safely.
static bool test_gzheader_flags_eight_with_zero_fname() {
    unsigned char data[] = {
        0x1F, 0x8B, 0x08, 0x08, // signature, method, flags (FNAME)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 6 bytes skip
        0x00 // terminator for fname field loop
    };
    return run_gzheader_with_buffer(data, sizeof(data), "test_gzheader_flags_eight_with_zero_fname");
}

// Test 4: Flags include 16 (FCOMMENT) with immediate zero terminator
// Expected: after 6-byte skip, read until 0, but immediate 0 exits loop.
static bool test_gzheader_flags_sixteen_with_zero_comment() {
    unsigned char data[] = {
        0x1F, 0x8B, 0x08, 0x10, // signature, method, flags (FCOMMENT)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 6 bytes skip
        0x00 // terminator for comment field loop
    };
    return run_gzheader_with_buffer(data, sizeof(data), "test_gzheader_flags_sixteen_with_zero_comment");
}

// Test 5: Flags include 2 (FHCRC) with two-byte skip
// Expected: skip 2 bytes after the 6-byte header.
static bool test_gzheader_flags_two_with_crc_skip() {
    unsigned char data[] = {
        0x1F, 0x8B, 0x08, 0x02, // signature, method, flags (FHCRC)
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // 6 bytes skip
        0x00, 0x00 // skip(2)
    };
    return run_gzheader_with_buffer(data, sizeof(data), "test_gzheader_flags_two_with_crc_skip");
}

// Main test runner
int main() {
    bool all_passed = true;

    std::cout << "Running gzheader unit tests (no GTest)..." << std::endl;

    all_passed &= test_gzheader_flags_zero();
    std::cout << "Test 1 (flags=0) " << (all_passed ? "PASSED" : "FAILED") << std::endl;

    all_passed &= test_gzheader_flags_four_with_zero_extra();
    std::cout << "Test 2 (flags=4, extra length=0) " << (all_passed ? "PASSED" : "FAILED") << std::endl;

    all_passed &= test_gzheader_flags_eight_with_zero_fname();
    std::cout << "Test 3 (flags=8, fname=empty) " << (all_passed ? "PASSED" : "FAILED") << std::endl;

    all_passed &= test_gzheader_flags_sixteen_with_zero_comment();
    std::cout << "Test 4 (flags=16, comment=empty) " << (all_passed ? "PASSED" : "FAILED") << std::endl;

    all_passed &= test_gzheader_flags_two_with_crc_skip();
    std::cout << "Test 5 (flags=2, crc=skip 2) " << (all_passed ? "PASSED" : "FAILED") << std::endl;

    if (all_passed) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests failed." << std::endl;
        return 1;
    }
}