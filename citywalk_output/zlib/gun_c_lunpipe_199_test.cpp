/*
 * lunpipe_unit_tests.cpp
 *
 * Lightweight C++11 unit tests for the focal function:
 *   lunpipe(unsigned have, z_const unsigned char *next, struct ind *indp,
 *           int outfile, z_stream *strm)
 *
 * Notes:
 * - This test harness assumes the presence of the original gun.c implementation
 *   linked into the test binary. It relies on the function prototype used in
 *   the provided snippet:
 *       extern "C" int lunpipe(unsigned have, z_const unsigned char *next,
 *                              struct ind *indp, int outfile, z_stream *strm);
 * - A minimal placeholder struct ind is declared to satisfy the function signature.
 * - The tests exercise early branches of lunpipe that do not require a full
 *   LZW stream to be decoded (as providing a full valid compressed stream would be
 *   impractical here). Specifically:
 *     1) Unknown LZW flags (flags & 0x60) => Z_DATA_ERROR
 *     2) LZW bits out of range (max < 9 or max > 16) => Z_DATA_ERROR
 *     3) No additional input after header => Z_OK (returns early)
 * - The tests do not rely on GoogleTest; they use simple asserts and print PASS/FAIL.
 *
 * This file is intended to be compiled along with the project (C++11) and linked
 * with gun.c (or the corresponding translation unit that provides lunpipe).
 */

#include <cstring>
#include <cstdio>
#include <string.h>
#include <errno.h>
#include <cassert>
#include <sys/stat.h>
#include <utime.h>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <zlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cstdlib>


// Forward declaration of the focal function from the original codebase.
// The exact signature must match the one used by gun.c.
extern "C" int lunpipe(unsigned have,
                      z_const unsigned char *next,
                      struct ind *indp,
                      int outfile,
                      z_stream *strm);

// Minimal placeholder for the 'ind' type used by lunpipe.
// The real project may have a richer definition. For testing the control
// flow we only need a distinct type.
struct ind {};

// Lightweight assertion helper to avoid depending on a test framework.
#define ASSERT_TRUE(cond, msg)                                     \
    do { if (!(cond)) {                                             \
            std::cerr << "ASSERT FAILED: " << (msg) << std::endl;  \
            return 1;                                              \
        }                                                            \
    } while (0)

#define ASSERT_EQ(lhs, rhs, msg)                                    \
    do { if (!((lhs) == (rhs))) {                                     \
            std::cerr << "ASSERT FAILED: " << (msg) << " -- "        \
                      << "expected: " << (rhs) << ", got: " << (lhs)  \
                      << std::endl;                                  \
            return 1;                                                \
        }                                                            \
    } while (0)

// Helper to run a single test case.
// data: pointer to input bytes that lunpipe will consume via NEXT()
// len: length of input data
// expected_rc: expected return code of lunpipe
// expected_msg_substr: optional substring that should appear in strm->msg on error
static int run_case(const char* case_name,
                    const unsigned char* data, size_t len,
                    int expected_rc,
                    const char* expected_msg_substr = nullptr) {
    // Prepare input
    unsigned have = static_cast<unsigned>(len);
    struct ind indp;
    // z_stream placeholder (subset needed by lunpipe)
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    // Call the focal function
    int rc = lunpipe(have, data, &indp, /*outfile*/ 0, &strm);

    // Validate return code
    if (rc != expected_rc) {
        std::cerr << "[CASE] " << case_name << " - FAIL: "
                  << "expected rc=" << expected_rc << ", got rc=" << rc << "\n";
        return 1;
    }

    // If an error was expected, optionally validate the message text
    if (expected_msg_substr != nullptr) {
        // The code path under test may have set strm.msg.
        // Some implementations may not set it; guard accordingly.
        const char* msg = strm.msg;
        if (msg == nullptr) {
            std::cerr << "[CASE] " << case_name << " - FAIL: "
                      << "expected msg substring '" << expected_msg_substr
                      << "', but no message was set.\n";
            return 1;
        }
        std::string s = msg;
        if (s.find(expected_msg_substr) == std::string::npos) {
            std::cerr << "[CASE] " << case_name << " - FAIL: "
                      << "expected msg substring '" << expected_msg_substr
                      << "', got: '" << s << "'\n";
            return 1;
        }
    }

    std::cout << "[CASE] " << case_name << " - PASS\n";
    return 0;
}

int main() {
    int failures = 0;

    // Case 1: Unknown lzw flags set (flags & 0x60 != 0) should yield Z_DATA_ERROR
    // Data to feed: a single byte where the high-level NEXT() returns 0x60.
    // The test relies on the internal NEXT() mechanism to set 'last' accordingly.
    {
        const unsigned char data1[] = { 0x60 }; // flags with 0x60 mask
        const char* case_name = "lunpipe_invalid_flags_should_return_Z_DATA_ERROR";
        int rc = run_case(case_name, data1, sizeof(data1),
                          Z_DATA_ERROR,
                          "unknown lzw flags set");
        failures += rc;
    }

    // Case 2: LZW bits out of range (max < 9 or max > 16) should yield Z_DATA_ERROR
    // Use a single byte with low 5 bits representing max. For example 0x08 -> max==8
    {
        const unsigned char data2[] = { 0x08 }; // max = 8 (out of range)
        const char* case_name = "lunpipe_bits_out_of_range_should_return_Z_DATA_ERROR";
        int rc = run_case(case_name, data2, sizeof(data2),
                          Z_DATA_ERROR,
                          "lzw bits out of range");
        failures += rc;
    }

    // Case 3: No additional input after header leads to Z_OK (early exit)
    // Provide only the first header byte; NEXT() will be exhausted on the second call,
    // causing lunpipe to return Z_OK as per the code path.
    {
        const unsigned char data3[] = { 0x00 }; // flags with max=0; end set; no extra data
        const char* case_name = "lunpipe_no_more_input_should_return_Z_OK";
        int rc = run_case(case_name, data3, sizeof(data3),
                          Z_OK);
        failures += rc;
    }

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << failures << " test(s) FAILED\n";
        return 2;
    }
}