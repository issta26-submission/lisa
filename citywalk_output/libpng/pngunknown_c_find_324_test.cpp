// Lightweight C++11 test suite for the focal method "find" implemented in a
// self-contained way to match the behavior described by the provided code.
// This avoids GTest and uses a small custom test harness with non-terminating
// assertions to maximize coverage.

// The focal behavior being tested:
// int find(const char *name) {
//    int i = NINFO;
//    while (--i >= 0) {
//       if (memcmp(chunk_info[i].name, name, 4) == 0)
//          break;
//    }
//    return i;
// }

// Key dependencies (mocked for testing):
// - NINFO: number of chunk entries
// - chunk_info[].name: 4-byte name to compare via memcmp

#include <setjmp.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>


// Mocked lightweight environment to emulate the original dependencies
int NINFO;

struct chunk_info_t {
    unsigned char name[4];
};

chunk_info_t chunk_info[16];

// Reproduce the focal function under test with C linkage semantics suitable for C++ tests
int find(const char *name) {
    int i = NINFO;
    while (--i >= 0) {
        if (memcmp(chunk_info[i].name, name, 4) == 0)
            break;
    }
    return i;
}

// Helper to set a chunk's 4-byte name (e.g., "TEST" -> {'T','E','S','T'})
static inline void set_name(int idx, const char *s) {
    memcpy(chunk_info[idx].name, s, 4);
}

// Simple non-terminating assertion framework
static int total_tests = 0;
static int failed_tests = 0;

#define EXPECT_EQ_INT(actual, expected, desc)                          \
do {                                                                    \
    ++total_tests;                                                      \
    if ((actual) != (expected)) {                                     \
        ++failed_tests;                                                 \
        std::cerr << "FAIL: " << desc                                   \
                  << " | expected " << (expected)                         \
                  << ", got " << (actual) << std::endl;                 \
    } else {                                                            \
        std::cout << "PASS: " << desc << std::endl;                    \
    }                                                                   \
} while (0)

// Entry point: run a focused set of unit tests for the find function
int main() {
    // -- Test 1: Match at the last (highest) index (i == NINFO-1) --
    // Setup: NINFO indicates 4 entries with a match at index 3 ("TEST")
    NINFO = 4;
    set_name(0, "ABCD");
    set_name(1, "ABCE");
    set_name(2, "WXYZ");
    set_name(3, "TEST"); // last index
    int r = find("TEST"); // should return 3
    EXPECT_EQ_INT(r, 3, "find() returns index of matching entry at the end (3)");

    // -- Test 2: Match in the middle (i == 2) --
    // Using the same data, query "WXYZ" which is at index 2
    r = find("WXYZ"); // should return 2
    EXPECT_EQ_INT(r, 2, "find() returns index of matching entry in the middle (2)");

    // -- Test 3: No match should return -1 --
    r = find("XXXX"); // not present
    EXPECT_EQ_INT(r, -1, "find() returns -1 when no matching entry exists");

    // -- Test 4: Edge case with NINFO = 0 --
    // With zero entries, the loop does not execute; the function should return 0
    NINFO = 0;
    r = find("TEST");
    EXPECT_EQ_INT(r, 0, "find() with NINFO=0 returns 0 (no iteration occurs)");

    // Summary of results
    std::cout << "Total tests: " << total_tests
              << ", Passed: " << (total_tests - failed_tests)
              << ", Failed: " << failed_tests << std::endl;

    // Return non-zero if any test failed to aid integration into CI pipelines
    return failed_tests ? 1 : 0;
}