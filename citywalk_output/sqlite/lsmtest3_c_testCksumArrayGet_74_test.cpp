/*
Unit test suite for the focal method:
  char *testCksumArrayGet(CksumDb *p, int nRow)

Context and approach:
- The focal method relies on the CksumDb structure fields:
  - int nFirst, nLast, nStep;
  - char **azCksum;
  - It returns azCksum[i] where i = (nRow - nFirst) / nStep, after validating
    that nRow lies in [nFirst, nLast] and aligns with the stepping.

- The provided lsmtest3.c file contains a testCksumArrayGet implementation that uses asserts.
  To cover true/false predicates safely, compile this test with -DNDEBUG to disable asserts
  (so that false predicates do not abort the test run). The tests below focus on the
  calculation path and correct pointer retrieval.

- This test suite is implemented in C++11 without any GTest, using a tiny custom test
  harness that provides non-terminating EXPECT-like checks and prints results.

- We directly construct a CksumDb-like object in tests (without relying on testCksumArrayNew),
  filling nFirst, nLast, nStep and azCksum to validate testCksumArrayGet behavior.

Notes on compilation:
- Compile with -DNDEBUG to disable asserts in the focal method (as per DOMAIN_KNOWLEDGE
  guideline about non-terminating assertions and to enable coverage of the branches).

Example compile line (gcc/clang):
  g++ -std=c++11 -DNDEBUG -I. -c test_suite.cpp
  g++ -std=c++11 -DNDEBUG -I. test_suite.cpp lsmtest3.c -o test_suite_run
  ./test_suite_run

The test suite includes:
- Normal path test: nFirst=10, nLast=30, nStep=5; verify nRow=10 -> azCksum[0], nRow=25 -> azCksum[3].
- Boundary path test: nFirst=0, nLast=20, nStep=2; verify nRow=20 -> azCksum[10] (last element).

Explanatory comments accompany each test function.
*/

#include <lsmtest.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdlib>


extern "C" {
}

// ----------------------------------------------------------------------------------
// Lightweight helper to duplicate strings into malloc'ed memory (to own azCksum entries).
static char* cksum_dup(const char* s) {
    if (s == nullptr) return nullptr;
    size_t len = std::strlen(s);
    char* t = (char*)std::malloc(len + 1);
    if (t) std::memcpy(t, s, len + 1);
    return t;
}

// Global test failure counter
static int g_failCount = 0;

// Minimal EXPECT-like macros (non-terminating)
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        ++g_failCount; \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) << " in " << __func__ << "\n"; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
    if((void*)(a) != (void*)(b)) { \
        ++g_failCount; \
        std::cerr << "EXPECT_PTR_EQ FAILED: " << (msg) \
                  << " (got " << (void*)(a) << ", expected " << (void*)(b) << ") in " \
                  << __func__ << "\n"; \
    } \
} while(0)

// ----------------------------------------------------------------------------------
// Test 1: Normal path - verify correct retrieval for two valid rows
void testCksumArrayGet_NormalCase() {
    // Given a CksumDb-like object with range 10..30 and step 5
    CksumDb p;
    p.nFirst = 10;
    p.nLast  = 30;
    p.nStep  = 5;

    // Prepare azCksum with 5 entries: indices 0..4
    const int N = 5;
    char** arr = (char**)std::malloc(N * sizeof(char*));
    arr[0] = cksum_dup("ck0");
    arr[1] = cksum_dup("ck1");
    arr[2] = cksum_dup("ck2");
    arr[3] = cksum_dup("ck3");
    arr[4] = cksum_dup("ck4");
    p.azCksum = arr;

    // When: nRow exactly at first and a later valid step
    char* r0 = testCksumArrayGet(&p, 10); // i = (10-10)/5 = 0
    EXPECT_PTR_EQ(r0, arr[0], "nRow=10 should map to azCksum[0]");

    char* r3 = testCksumArrayGet(&p, 25); // i = (25-10)/5 = 3
    EXPECT_PTR_EQ(r3, arr[3], "nRow=25 should map to azCksum[3]");

    // Cleanup
    for(int i = 0; i < N; ++i) {
        std::free(arr[i]);
    }
    std::free(arr);
}

// ----------------------------------------------------------------------------------
// Test 2: Boundary path - verify retrieval at the last valid row
void testCksumArrayGet_Boundary() {
    // Given a CksumDb-like object with range 0..20 and step 2
    CksumDb p;
    p.nFirst = 0;
    p.nLast  = 20;
    p.nStep  = 2;

    // Number of elements: (20-0)/2 + 1 = 11
    const int N = (p.nLast - p.nFirst) / p.nStep + 1;
    char** arr = (char**)std::malloc(N * sizeof(char*));
    for(int i = 0; i < N; ++i) {
        // Fill with identifiable values
        char buf[8];
        std::snprintf(buf, sizeof(buf), "v%d", i);
        arr[i] = cksum_dup(buf);
    }
    p.azCksum = arr;

    // When: nRow equals the last value
    char* rLast = testCksumArrayGet(&p, p.nLast);
    EXPECT_PTR_EQ(rLast, arr[N - 1], "nRow=nLast should map to azCksum[lastIndex]");

    // Cleanup
    for(int i = 0; i < N; ++i) {
        std::free(arr[i]);
    }
    std::free(arr);
}

// ----------------------------------------------------------------------------------
// Main: Run tests and report results. Compile with -DNDEBUG to disable asserts in the focal
// method so that branches corresponding to false predicates can be exercised.
// ----------------------------------------------------------------------------------
int main() {
    std::cout << "Starting testCksumArrayGet test suite (C++11, no GTest)\n";

    testCksumArrayGet_NormalCase();
    testCksumArrayGet_Boundary();

    if (g_failCount > 0) {
        std::cerr << g_failCount << " test(s) FAILED\n";
        return 1;
    } else {
        std::cout << "All tests PASSED\n";
        return 0;
    }
}