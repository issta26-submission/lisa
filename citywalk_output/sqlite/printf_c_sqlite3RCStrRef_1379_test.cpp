/*
Unit Test Suite for the focal method:
  char *sqlite3RCStrRef(char *z)

Purpose:
- Verify that sqlite3RCStrRef increments the nRCRef counter of the RCStr
  object that precedes the given data pointer in memory.
- Ensure the function returns the original pointer unchanged.
- Use a minimal, self-contained RCStr layout to mimic the expected behavior.

Notes on test design (mapped to the provided guidance):
- Candidate Keywords and dependencies simulated in tests:
  RCStr, nRCRef, sqlite3RCStrRef
- We avoid external frameworks (no GTest) and implement a tiny test harness with non-terminating assertions.
- Tests cover basic increment, multiple invocations, and independence across multiple RCStr pairs.
- Static/file-scope aspects are not exercised here (we define RCStr locally in test).
- All tests run under C++11 compatibility.

Code below:
*/
#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <cassert>


// Minimal reproduction of the RCStr structure used by sqlite3RCStrRef
// This mirrors the expected memory layout where the RCStr header sits
// immediately before the data portion pointed to by z.
struct RCStr {
    uint64_t nRCRef;
};

// Focal method under test (reproduced for self-contained testing)
char *sqlite3RCStrRef(char *z) {
    RCStr *p = (RCStr*)z;
    assert(p != 0);
    p--;
    p->nRCRef++;
    return z;
}

// Simple non-terminating test harness macros
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAILED] " << (msg) << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        std::cerr << "[FAILED] " << (msg) \
                  << " (lhs=" << (a) << ", rhs=" << (b) << ")\n"; \
        ++g_failures; \
    } \
} while(0)


// Test 1: Basic increment of header's nRCRef via sqlite3RCStrRef
// - Layout: mem[0] is the header (RCStr), mem[1] is the data block.
// - z points to mem[1], so p-- yields mem[0] and increments mem[0].nRCRef.
void test_sqlite3RCStrRef_basic_increment() {
    // Candidate Keywords: RCStr, nRCRef, sqlite3RCStrRef, memory layout, pointer arithmetic
    RCStr mem[2];
    mem[0].nRCRef = 5;     // initial header reference count
    // mem[1] acts as the data region; z will point here
    char *z = reinterpret_cast<char*>(&mem[1]);
    char *ret = sqlite3RCStrRef(z);
    EXPECT_TRUE(ret == z, "Return value should be the same pointer z");
    EXPECT_EQ(mem[0].nRCRef, 6, "Header.nRCRef should be incremented by 1");
}

// Test 2: Multiple invocations should accumulate nRCRef on the same header
void test_sqlite3RCStrRef_multiple_calls() {
    RCStr mem[2];
    mem[0].nRCRef = 7;
    char *z = reinterpret_cast<char*>(&mem[1]);
    sqlite3RCStrRef(z); // first increment
    sqlite3RCStrRef(z); // second increment
    EXPECT_EQ(mem[0].nRCRef, 9, "Header.nRCRef should reflect two increments (7 -> 9)");
}

// Test 3: Independent RCStr pairs should maintain separate nRCRef counters
void test_sqlite3RCStrRef_multiple_pairs_independent() {
    RCStr pair1[2];
    RCStr pair2[2];
    pair1[0].nRCRef = 2;
    pair2[0].nRCRef = 10;

    char *z1 = reinterpret_cast<char*>(&pair1[1]);
    char *z2 = reinterpret_cast<char*>(&pair2[1]);

    sqlite3RCStrRef(z1); // affects pair1
    sqlite3RCStrRef(z2); // affects pair2

    EXPECT_EQ(pair1[0].nRCRef, 3, "Pair1 header should increment to 3");
    EXPECT_EQ(pair2[0].nRCRef, 11, "Pair2 header should increment to 11");
}

// Main: run all tests and report summary
int main() {
    // Run tests
    test_sqlite3RCStrRef_basic_increment();
    test_sqlite3RCStrRef_multiple_calls();
    test_sqlite3RCStrRef_multiple_pairs_independent();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED\n";
        return g_failures;
    }
}