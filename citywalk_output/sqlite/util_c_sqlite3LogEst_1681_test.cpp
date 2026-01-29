// Test suite for the focal method sqlite3LogEst (C function) using C++11, without GoogleTest.
// The tests are designed to be self-contained, non-terminating (non-fatal on failures), and
// provide explanatory comments for each test case.
// We rely on the project's sqliteInt.h for type definitions (LogEst, u64, etc.).
//
// How to build (example):
// g++ -std=c++11 -I. -c test_sqlite3LogEst.cpp
// g++ -std=c++11 -o test_sqlite3LogEst test_sqlite3LogEst.o util.o -lm
//
// Note: The test links against the existing implementation of sqlite3LogEst (provided in util.c)
// within the project. The macro GCC_VERSION is not forced here; the default (not defined) uses
// the non-GCC-specific path in sqlite3LogEst.

#include <math.h>
#include <string>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Include SQLite internal types (LogEst, u64, etc.)

// Declare the focal function with C linkage for correct linking.
// We rely on the actual implementation in the project; function name mangling is C.
extern "C" LogEst sqlite3LogEst(u64 x);

// Simple non-terminating test harness
static int g_total = 0;
static int g_fail = 0;

// Helper assertion: compares actual vs expected and logs result without terminating the program.
#define CHECK_EQ(note, actual, expected) do { \
    ++g_total; \
    if (static_cast<unsigned long long>(actual) != static_cast<unsigned long long>(expected)) { \
        std::cerr << "FAIL  [" << g_total << "] " << note \
                  << " | expected: " << static_cast<unsigned long long>(expected) \
                  << ", actual: " << static_cast<unsigned long long>(actual) << "\n"; \
        ++g_fail; \
    } else { \
        std::cout << "PASS  [" << g_total << "] " << note << "\n"; \
    } \
} while(0)

// Test 1: Values below 8 (including the boundary where exact behavior of inner branch matters).
// This exercises the inner conditional (x < 2) and the loop that progresses x up to < 8.
void test_below_eight_values() {
    // x = 0 -> x < 2, should return 0 directly.
    CHECK_EQ("sqlite3LogEst(0) == 0", sqlite3LogEst(0), 0);

    // x = 1 -> still x < 2, should return 0 directly.
    CHECK_EQ("sqlite3LogEst(1) == 0", sqlite3LogEst(1), 0);

    // x = 2 -> enters while loop, evaluates to 10 as computed by the algorithm.
    CHECK_EQ("sqlite3LogEst(2) == 10", sqlite3LogEst(2), 10);

    // x = 3 -> computed value 16 (see manual calculation in analysis).
    CHECK_EQ("sqlite3LogEst(3) == 16", sqlite3LogEst(3), 16);

    // x = 4 -> 20
    CHECK_EQ("sqlite3LogEst(4) == 20", sqlite3LogEst(4), 20);

    // x = 5 -> 23
    CHECK_EQ("sqlite3LogEst(5) == 23", sqlite3LogEst(5), 23);

    // x = 6 -> 26
    CHECK_EQ("sqlite3LogEst(6) == 26", sqlite3LogEst(6), 26);

    // x = 7 -> 28
    CHECK_EQ("sqlite3LogEst(7) == 28", sqlite3LogEst(7), 28);
}

// Test 2: Values from 8 to 15 to exercise the else-branch's first path when GCC_VERSION is not enabled.
// This uses the non-GCC path (the #else portion) where x is reduced via shifts and y accumulates accordingly.
void test_eight_to_fifteen_values() {
    CHECK_EQ("sqlite3LogEst(8)  == 30", sqlite3LogEst(8), 30);
    CHECK_EQ("sqlite3LogEst(9)  == 32", sqlite3LogEst(9), 32);
    CHECK_EQ("sqlite3LogEst(10) == 33", sqlite3LogEst(10), 33);
    CHECK_EQ("sqlite3LogEst(11) == 35", sqlite3LogEst(11), 35);
    CHECK_EQ("sqlite3LogEst(12) == 36", sqlite3LogEst(12), 36);
    CHECK_EQ("sqlite3LogEst(13) == 37", sqlite3LogEst(13), 37);
    CHECK_EQ("sqlite3LogEst(14) == 38", sqlite3LogEst(14), 38);
    CHECK_EQ("sqlite3LogEst(15) == 39", sqlite3LogEst(15), 39);
}

// Test 3: Values greater than 15 to exercise the second loop and a mix of macro paths.
// This includes 16, 256, 512, 1024 and 255 to cover various branches in the arithmetic.
void test_large_values() {
    // x = 16 -> enters second loop once (x > 15), yields 40
    CHECK_EQ("sqlite3LogEst(16) == 40", sqlite3LogEst(16), 40);

    // x = 255 -> close to 8<<...; expected precomputed value 79
    CHECK_EQ("sqlite3LogEst(255) == 79", sqlite3LogEst(255), 79);

    // x = 256 -> first loop triggers (x > 255), then continues with shifts
    CHECK_EQ("sqlite3LogEst(256) == 80", sqlite3LogEst(256), 80);

    // x = 512 -> expected 90
    CHECK_EQ("sqlite3LogEst(512) == 90", sqlite3LogEst(512), 90);

    // x = 1024 -> expected 100
    CHECK_EQ("sqlite3LogEst(1024) == 100", sqlite3LogEst(1024), 100);
}

// Entry point for the test suite
int main() {
    std::cout << "Starting tests for sqlite3LogEst...\n";

    test_below_eight_values();
    test_eight_to_fifteen_values();
    test_large_values();

    std::cout << "Tests completed. "
              << "Total: " << g_total << ", Failures: " << g_fail << "\n";

    // Return non-zero if any test failed.
    return (g_fail == 0) ? 0 : 1;
}