// Test suite for sqlite3SubInt64 (as found in util.c) using C++11 (no GTest).
// This harness focuses on exercising the #else branch of sqlite3SubInt64
// (i.e., when GCC_VERSION < 5004000 or __INTEL_COMPILER is defined).
// It provides a minimal implementation of dependent pieces (sqlite3AddInt64, etc.)
// sufficient to verify the behavior of sqlite3SubInt64 with various inputs.
//
// Notes:
// - We purposely define a small, self-contained environment to avoid requiring
//   the full SQLite build system. This includes a light-weight
//   sqlite3AddInt64 and a replica of sqlite3SubInt64 from the focal method.
// - We use non-terminating test checks: tests print pass/fail rather than abort.
// - We include explanatory comments above each test case to document intent and coverage.

#include <math.h>
#include <climits>
#include <string>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Domain-specific aliases and constants
using i64 = long long;
const i64 SMALLEST_INT64 = LLONG_MIN;

// Lightweight test macro (no-op in case test framework macros are unavailable)
#ifndef testcase
#define testcase(x) do { (void)(x); } while(0)
#endif

// Forward declaration of the dependent helper used by sqlite3SubInt64.
// In the real SQLite sources, this is an internal helper that adds two 64-bit
// integers with overflow detection.
int sqlite3AddInt64(i64 *pA, i64 iB);

// Minimal implementation of sqlite3SubInt64 mirroring the focal method.
// We'll force the #else branch by ensuring GCC_VERSION < 5004000 (via defining 0).
#define GCC_VERSION 0
int sqlite3SubInt64(i64 *pA, i64 iB){
#if GCC_VERSION>=5004000 && !defined(__INTEL_COMPILER)
  // In real environments, this would use the compiler builtin overflow check.
  // We do not exercise this path in this test harness.
  return __builtin_sub_overflow(*pA, iB, pA);
#else
  testcase( iB==SMALLEST_INT64+1 );
  if( iB==SMALLEST_INT64 ){
    testcase( (*pA)==(-1) ); testcase( (*pA)==0 );
    if( (*pA)>=0 ) return 1;
    *pA -= iB;
    return 0;
  }else{
    return sqlite3AddInt64(pA, -iB);
  }
#endif
}

// Minimal, self-contained version of sqlite3AddInt64.
// Returns 0 on success (no overflow) and stores the result in *pA.
// Returns 1 on overflow and leaves *pA unchanged.
int sqlite3AddInt64(i64 *pA, i64 iB){
  i64 a = *pA;
  // Overflow detection: if adding iB to a would exceed 64-bit bounds.
  if( (iB > 0 && a > LLONG_MAX - iB) || (iB < 0 && a < LLONG_MIN - iB) ){
    return 1;
  }
  *pA = a + iB;
  return 0;
}

// Lightweight test harness
static int g_total_tests  = 0;
static int g_failed_tests = 0;

void report(const std::string &name, bool ok){
    ++g_total_tests;
    if(ok){
        std::cout << "[PASS] " << name << std::endl;
    }else{
        ++g_failed_tests;
        std::cout << "[FAIL] " << name << std::endl;
    }
}

int main(){
    // Test 1: iB == SMALLEST_INT64, pA = -1
    // Expect: return 0 and *pA becomes SMALLEST_INT64 negated, i.e., LLONG_MAX.
    {
        i64 a = -1;
        int r = sqlite3SubInt64(&a, SMALLEST_INT64);
        bool ok = (r == 0) && (a == LLONG_MAX);
        report("sqlite3SubInt64: iB == SMALLEST_INT64; pA = -1 -> pA becomes LLONG_MAX; return 0", ok);
    }

    // Test 2: iB != SMALLEST_INT64, normal add path
    // iB = 5, pA = 2 -> expect pA = -3, return 0
    {
        i64 a = 2;
        int r = sqlite3SubInt64(&a, 5);
        bool ok = (r == 0) && (a == -3);
        report("sqlite3SubInt64: else path; pA=2, iB=5 -> pA=-3, return 0", ok);
    }

    // Test 3: Overflow in add path (iB != SMALLEST_INT64)
    // pA = LLONG_MAX, iB = -1; -iB = 1; LLONG_MAX + 1 would overflow -> expect return 1 and pA unchanged
    {
        i64 a = LLONG_MAX;
        int r = sqlite3SubInt64(&a, -1);
        bool ok = (r == 1) && (a == LLONG_MAX);
        report("sqlite3SubInt64: else path; pA=LLONG_MAX, iB=-1 -> overflow; return 1; pA unchanged", ok);
    }

    // Test 4: iB == SMALLEST_INT64, pA == 0
    // Expect: return 1 and *pA remains 0
    {
        i64 a = 0;
        int r = sqlite3SubInt64(&a, SMALLEST_INT64);
        bool ok = (r == 1) && (a == 0);
        report("sqlite3SubInt64: iB==SMALLEST_INT64; pA=0 -> return 1; pA unchanged", ok);
    }

    // Test 5: iB == SMALLEST_INT64, pA negative
    // pA = -2; after operation: *pA becomes LLONG_MAX - 1; return 0
    {
        i64 a = -2;
        int r = sqlite3SubInt64(&a, SMALLEST_INT64);
        bool ok = (r == 0) && (a == LLONG_MAX - 1);
        report("sqlite3SubInt64: iB==SMALLEST_INT64; pA=-2 -> pA=LLONG_MAX-1; return 0", ok);
    }

    // Test 6: iB == SMALLEST_INT64 + 1; pA = LLONG_MAX
    // iB is slightly larger than SMALLEST_INT64; -iB overflows in practice;
    // In our test environment, this should cause overflow in sqlite3AddInt64 path -> return 1, pA unchanged
    {
        i64 iB = SMALLEST_INT64 + 1;
        i64 a = LLONG_MAX;
        int r = sqlite3SubInt64(&a, iB);
        bool ok = (r == 1) && (a == LLONG_MAX);
        report("sqlite3SubInt64: iB==SMALLEST_INT64+1; pA=LLONG_MAX -> overflow; return 1; pA unchanged", ok);
    }

    // Test 7: iB = 0; pA = 0
    // Sanity check: no change, no overflow
    {
        i64 a = 0;
        int r = sqlite3SubInt64(&a, 0);
        bool ok = (r == 0) && (a == 0);
        report("sqlite3SubInt64: iB=0; pA=0 -> no change; return 0", ok);
    }

    // Summary
    std::cout << "Total tests: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;
    return g_failed_tests ? 1 : 0;
}