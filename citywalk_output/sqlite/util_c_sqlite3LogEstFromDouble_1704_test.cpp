// Lightweight C++11 test suite for the focal function: sqlite3LogEstFromDouble
// This test suite is implemented without GoogleTest. It uses a small, self-contained
// testing harness to verify the behavior of sqlite3LogEstFromDouble and its
// dependent function sqlite3LogEst as described in the provided snippet.
//
// Notes:
// - The test suite provides minimal stubs for dependencies (LogEst type and sqlite3LogEst).
// - Tests cover: x <= 1 branch, x within (1, 2e9] branch, x == 2e9 boundary,
//   and the bit-exponent-based branches for powers of two (e.g., 2^31 and 2^40).
// - Tests are designed to be executable in a C++11 environment and do not rely on GTest.

#include <math.h>
#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <cstdint>
#include <cassert>


// Basic type aliases to mirror the focal code's expectations
typedef unsigned long long u64;
typedef unsigned long long LogEst;

// Minimal dependency implementations to enable testing of sqlite3LogEstFromDouble

// Stub for sqlite3LogEst(u64 x); in the real project this computes a logarithmic estimate.
// For testing purposes, we can implement a straightforward pass-through.
LogEst sqlite3LogEst(u64 x){
    return (LogEst)x;
}

// The focal function under test (reproduced here to enable unit testing in isolation)
LogEst sqlite3LogEstFromDouble(double x){
  u64 a;
  LogEst e;
  assert( sizeof(x)==8 && sizeof(a)==8 );
  if( x<=1 ) return 0;
  if( x<=2000000000 ) return sqlite3LogEst((u64)x);
  memcpy(&a, &x, 8);
  e = (a>>52) - 1022;
  return e*10;
}

// -------------------------
// Test harness helpers
// -------------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

static void report_pass(const char* name){
    printf("PASS: %s\n", name);
}
static void report_fail(const char* name, const char* msg){
    printf("FAIL: %s - %s\n", name, msg);
    ++g_failed_tests;
}
static void run_test(bool ok, const char* name, const char* explain){
    ++g_total_tests;
    if(ok){
        report_pass(name);
    }else{
        report_fail(name, explain);
    }
}

// -------------------------
// Individual test cases
// -------------------------

// Test 1: x = 0.5 (less than or equal to 1) should return 0
bool test_x_less_or_equal_one_returns_zero(){
    // Domain: x <= 1 -> 0
    double x = 0.5;
    LogEst r = sqlite3LogEstFromDouble(x);
    bool ok = (r == 0);
    if(!ok){
        printf("  Expected 0, got %llu\n", (unsigned long long)r);
    }
    return ok;
}

// Test 2: x = -5.0 (negative values also go to the first branch)
bool test_negative_x_returns_zero(){
    // Domain: x <= 1 -> 0
    double x = -5.0;
    LogEst r = sqlite3LogEstFromDouble(x);
    bool ok = (r == 0);
    if(!ok){
        printf("  Expected 0, got %llu\n", (unsigned long long)r);
    }
    return ok;
}

// Test 3: x = 2.0 (within (1, 2e9] branch, uses sqlite3LogEst((u64)x))
bool test_x_two_point_zero_in_small_branch(){
    // Domain: x <= 2e9 -> sqlite3LogEst((u64)x). If sqlite3LogEst is a passthrough, result == 2
    double x = 2.0;
    LogEst r = sqlite3LogEstFromDouble(x);
    bool ok = (r == 2);
    if(!ok){
        printf("  Expected 2, got %llu\n", (unsigned long long)r);
    }
    return ok;
}

// Test 4: x = 2000000000.0 (boundary of the small-branch)
bool test_x_equals_two_billion_boundary(){
    double x = 2000000000.0;
    LogEst r = sqlite3LogEstFromDouble(x);
    bool ok = (r == 2000000000ULL);
    if(!ok){
        printf("  Expected 2000000000, got %llu\n", (unsigned long long)r);
    }
    return ok;
}

// Test 5: x = 2^31 (a large power of two, triggers bit-exponent path)
// Expected: e = ( (a>>52) - 1022 ) with a>>52 = 1023 + 31 = 1054, so e = 32, result = 320
bool test_x_power_of_two_32_branch(){
    double x = static_cast<double>(1ULL << 31); // 2^31
    LogEst r = sqlite3LogEstFromDouble(x);
    bool ok = (r == 320ULL);
    if(!ok){
       printf("  Expected 320, got %llu\n", (unsigned long long)r);
    }
    return ok;
}

// Test 6: x = 2^40 (another large power of two, ensures correctness of exponent path)
// Expected: e = (1023+40) - 1022 = 41, result = 410
bool test_x_power_of_two_40_branch(){
    double x = static_cast<double>(1ULL << 40); // 2^40
    LogEst r = sqlite3LogEstFromDouble(x);
    bool ok = (r == 410ULL);
    if(!ok){
        printf("  Expected 410, got %llu\n", (unsigned long long)r);
    }
    return ok;
}

// -------------------------
// Main test runner
// -------------------------

int main(){
    // Run all tests
    run_test(test_x_less_or_equal_one_returns_zero(), "test_x_less_or_equal_one_returns_zero",
             "For x <= 1, sqlite3LogEstFromDouble should return 0");
    run_test(test_negative_x_returns_zero(), "test_negative_x_returns_zero",
             "For negative x, sqlite3LogEstFromDouble should return 0");
    run_test(test_x_two_point_zero_in_small_branch(), "test_x_two_point_zero_in_small_branch",
             "For 1 < x <= 2e9, function should return sqlite3LogEst((u64)x)");
    run_test(test_x_equals_two_billion_boundary(), "test_x_equals_two_billion_boundary",
             "For x == 2e9, function should return 2000000000 using small branch");
    run_test(test_x_power_of_two_32_branch(), "test_x_power_of_two_32_branch",
             "For x = 2^31, exponent path should yield 320");
    run_test(test_x_power_of_two_40_branch(), "test_x_power_of_two_40_branch",
             "For x = 2^40, exponent path should yield 410");

    // Summary
    printf("Total tests: %d, Passed: %d, Failed: %d\n",
           g_total_tests, g_total_tests - g_failed_tests, g_failed_tests);

    return g_failed_tests;
}