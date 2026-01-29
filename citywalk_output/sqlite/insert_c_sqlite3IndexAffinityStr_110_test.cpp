// Unit test suite for sqlite3IndexAffinityStr
// - Uses a light, custom test framework (no GTest) with non-terminating EXPECT-like assertions.
// - Targets the focal method sqlite3IndexAffinityStr(sqlite3 *db, Index *pIdx) as described.
// - Assumes the project provides the real SQLite sources (insert.c) and headers (sqliteInt.h, sqlite3.h).
// - Compile with: g++ -std=c++11 -I<path_to_sqlite_headers> test_sqlite3IndexAffinityStr.cpp insert.c -o test_sqlite3IndexAffinityStr
//   (Adjust include paths and linked files as appropriate for your build environment.)

#include <cstddef>
#include <cstdio>
#include <sqliteInt.h>
#include <cstring>


// Bring in the SQLite internal declarations required for the Index type and sqlite3 type.
// The tests rely on the same type definitions as the production code.
// Enclose C headers in extern "C" to be safe when compiling as C++.
extern "C" {
}

// Declare the focal function with C linkage for the test harness.
extern "C" const char *sqlite3IndexAffinityStr(sqlite3 *db, Index *pIdx);

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::fprintf(stderr, "EXPECT_TRUE failed: %s\n  at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    if(cond) { \
        std::fprintf(stderr, "EXPECT_FALSE failed: %s\n  at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ_PTR(a,b) do { \
    if((const void*)(a) != (const void*)(b)) { \
        std::fprintf(stderr, "EXPECT_EQ_PTR failed: %p != %p\n  at %s:%d\n", (void*)(a), (void*)(b), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_NOT_NULL(p) do { \
    if((p) == nullptr) { \
        std::fprintf(stderr, "EXPECT_NOT_NULL failed: pointer is null\n  at %s:%d\n", __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

// Test 1: True branch - when zColAff is non-null, sqlite3IndexAffinityStr should return that string pointer directly.
void test_sqlite3IndexAffinityStr_trueBranch() {
    // Prepare a dummy sqlite3 database object (opaque in this test context).
    sqlite3 db;
    std::memset(&db, 0, sizeof(db));

    // Prepare an Index with a non-null zColAff
    Index idx;
    const char *expected = "AFFINITY_STRING";
    idx.zColAff = expected;

    // Call the focal function
    const char *actual = sqlite3IndexAffinityStr(&db, &idx);

    // Verify: should return the exact pointer stored in zColAff
    EXPECT_EQ_PTR(actual, expected);
}

// Test 2: False branch - when zColAff is null, sqlite3IndexAffinityStr should fall back to computeIndexAffStr.
// We validate that a non-null result is returned and that repeated calls are consistent.
void test_sqlite3IndexAffinityStr_falseBranch() {
    // Prepare a dummy sqlite3 database object
    sqlite3 db;
    std::memset(&db, 0, sizeof(db));

    // Prepare an Index with zColAff = NULL to trigger the static computeIndexAffStr path
    Index idx;
    idx.zColAff = nullptr;

    // First call: should not crash and should return a non-null pointer (content is determined by computeIndexAffStr)
    const char *first = sqlite3IndexAffinityStr(&db, &idx);
    EXPECT_NOT_NULL(first);

    // Second call: ensure consistency for the same inputs
    const char *second = sqlite3IndexAffinityStr(&db, &idx);
    EXPECT_NOT_NULL(second);
    EXPECT_EQ_PTR(first, second);
}

// A tiny helper to run all tests and report summary
void run_all_tests() {
    test_sqlite3IndexAffinityStr_trueBranch();
    test_sqlite3IndexAffinityStr_falseBranch();
}

// Entry point
int main() {
    run_all_tests();

    if(g_failures == 0) {
        std::puts("ALL TESTS PASSED");
        return 0;
    } else {
        std::fprintf(stderr, "TOTAL FAILED TESTS: %d\n", g_failures);
        return 1;
    }
}