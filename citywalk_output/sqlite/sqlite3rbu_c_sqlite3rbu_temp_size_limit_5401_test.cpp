// C++11 test suite for sqlite3rbu_temp_size_limit
// This test suite is designed to be run without any external test framework (e.g., GTest).
// It uses a lightweight, non-terminating assertion mechanism to maximize coverage.

#include <string.h>
#include <windows.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <stdio.h>
#include <sqlite3rbu.h>
#include <cstdint>


// Forward declare C symbols to ensure correct linkage when compiled with a C++ compiler
extern "C" {
  #include "sqlite3.h"
  #include "sqlite3rbu.h"
}

// Lightweight test harness
static int g_fail_count = 0;

// Non-terminating assertions to maximize code coverage
#define EXPECT_EQ(actual, expected, msg) do {                               \
    auto _a = (actual);                                                       \
    auto _e = (expected);                                                     \
    if (!(_a == _e)) {                                                       \
        fprintf(stderr, "[TEST_FAIL] %s: expected %lld, got %lld\n", (msg),  \
                (long long)_e, (long long)_a);                               \
        ++g_fail_count;                                                       \
    }                                                                         \
} while(false)

#define EXPECT_TRUE(cond, msg) do {                                           \
    if (!(cond)) {                                                            \
        fprintf(stderr, "[TEST_FAIL] %s: condition failed\n", (msg));          \
        ++g_fail_count;                                                       \
    }                                                                         \
} while(false)

// Test 1: When n >= 0, the function should set szTempLimit to n and return that value.
static void test_sqlite3rbu_temp_size_limit_positive()
{
    // Prepare a RBU object and set an initial limit
    sqlite3rbu rbu;
    rbu.szTempLimit = 5;

    // Call the function with a non-negative value
    sqlite3_int64 ret = sqlite3rbu_temp_size_limit(&rbu, 10);

    // Verify that the limit was updated and returned value is the new limit
    EXPECT_EQ(ret, 10, "Positive input should set and return new temp size limit");
    EXPECT_EQ(rbu.szTempLimit, 10, "szTempLimit should be updated to non-negative input");
}

// Test 2: When n < 0, the function should NOT modify szTempLimit and should return the current value.
static void test_sqlite3rbu_temp_size_limit_negative_no_change()
{
    sqlite3rbu rbu;
    rbu.szTempLimit = 20;

    sqlite3_int64 ret = sqlite3rbu_temp_size_limit(&rbu, -1);

    // Verify that the limit remains unchanged and the return value equals the current limit
    EXPECT_EQ(ret, 20, "Negative input should return current temp size limit without modification");
    EXPECT_EQ(rbu.szTempLimit, 20, "szTempLimit should remain unchanged when input is negative");
}

// Test 3: Boundary case: n == 0 should set the limit to 0 and return 0.
static void test_sqlite3rbu_temp_size_limit_zero_boundary()
{
    sqlite3rbu rbu;
    rbu.szTempLimit = 7;  // initial non-zero to verify clearance to zero

    sqlite3_int64 ret = sqlite3rbu_temp_size_limit(&rbu, 0);

    EXPECT_EQ(ret, 0, "Zero input should set and return 0 as temp size limit");
    EXPECT_EQ(rbu.szTempLimit, 0, "szTempLimit should be updated to 0 when input is 0");
}

// Run all tests and report summary
static void run_all_tests()
{
    test_sqlite3rbu_temp_size_limit_positive();
    test_sqlite3rbu_temp_size_limit_negative_no_change();
    test_sqlite3rbu_temp_size_limit_zero_boundary();
}

int main()
{
    // Execute tests
    run_all_tests();

    // Report results
    if (g_fail_count == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        fprintf(stderr, "%d TEST(S) FAILED\n", g_fail_count);
        return 1;
    }
}