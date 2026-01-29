// C++11 unit tests for sqlite3BitvecTest (no GTest). 
// This test suite exercises the focal method by using the project
// bitvec.c/.h dependencies (Bitvec type and related APIs).
// The tests rely on the real implementations provided by the project.

#include <cstdlib>
#include <cstdio>
#include <sqliteInt.h>
#include <cstdint>


// Import project dependencies.
// The actual project provides a C API for Bitvec and related functions.
// Ensure the header guards in the real project wrap extern "C" when compiled under C++.

// Lightweight test harness: non-terminating assertions with logging.
static int g_testFailures = 0;
static int g_totalTests = 0;

// Basic assertion helpers that do not abort test execution.
#define EXPECT_TRUE(cond) do { \
    ++g_totalTests; \
    if(!(cond)){ \
        fprintf(stderr, "EXPECT_TRUE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_testFailures; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    ++g_totalTests; \
    if((cond)){ \
        fprintf(stderr, "EXPECT_FALSE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_testFailures; \
    } \
} while(0)

#define EXPECT_NOTNULL(ptr) do { \
    ++g_totalTests; \
    if((ptr) == nullptr){ \
        fprintf(stderr, "EXPECT_NOTNULL failed: %s is NULL at %s:%d\n", #ptr, __FILE__, __LINE__); \
        ++g_testFailures; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    ++g_totalTests; \
    if(((a) != (b))){ \
        fprintf(stderr, "EXPECT_EQ failed: %ld != %ld at %s:%d\n", (long)(a), (long)(b), __FILE__, __LINE__); \
        ++g_testFailures; \
    } \
} while(0)


// Test 1: Fallback path when p is null. The focal method should return false
// without invoking sqlite3BitvecTestNotNull (true short-circuit of the and operand).
// This validates the false branch of the first predicate: p != 0.
static void test_sqlite3BitvecTest_nullPointer() {
    // Explanatory: p == 0 should short-circuit and return 0.
    // We pass an arbitrary index; value is irrelevant in this path.
    int result = sqlite3BitvecTest(nullptr, 0);
    // Expect false (0) due to null pointer.
    EXPECT_FALSE(result);
    // Comment: This confirms the left-hand branch (p != 0) short-circuits correctly.
}

// Test 2: Non-null Bitvec where the inner predicate returns true.
// This exercises the true branch of the second predicate (sqlite3BitvecTestNotNull(p,i)).
// We first create a Bitvec, set a bit, and confirm sqlite3BitvecTest reports true.
static void test_sqlite3BitvecTest_nonNullBitvecSetBit() {
    // Create a bitvec with a reasonable size. The project provides sqlite3BitvecCreate.
    Bitvec *p = sqlite3BitvecCreate(16);
    EXPECT_NOTNULL(p);

    // Initially, without setting bit 5, sqlite3BitvecTest(p,5) should be false.
    EXPECT_FALSE(sqlite3BitvecTest(p, 5));

    // Set bit 5 and verify the focal method reports true.
    sqlite3BitvecSet(p, 5);
    int afterSet = sqlite3BitvecTest(p, 5);
    EXPECT_TRUE(afterSet);

    // Cleanup
    sqlite3BitvecDestroy(p);
}

// Test 3: Non-null Bitvec where the inner predicate returns false (bit not set).
// This ensures the non-null path still leads to a false return when the bit is not set.
// We reuse a fresh Bitvec and a different bit index to validate independence.
static void test_sqlite3BitvecTest_nonNullBitvecBitNotSet() {
    Bitvec *p = sqlite3BitvecCreate(16);
    EXPECT_NOTNULL(p);

    // Do not set bit 3; sqlite3BitvecTest should return false.
    int res = sqlite3BitvecTest(p, 3);
    EXPECT_FALSE(res);

    // Additionally, verify that setting then clearing yields expected transitions.
    sqlite3BitvecSet(p, 3);
    EXPECT_TRUE(sqlite3BitvecTest(p, 3));

    // Clear and verify returns to false
    sqlite3BitvecClear(p, 3, nullptr);
    EXPECT_FALSE(sqlite3BitvecTest(p, 3));

    sqlite3BitvecDestroy(p);
}

// Optional: A small helper to run all tests and report summary.
static void run_all_tests() {
    test_sqlite3BitvecTest_nullPointer();
    test_sqlite3BitvecTest_nonNullBitvecSetBit();
    test_sqlite3BitvecTest_nonNullBitvecBitNotSet();
}

int main() {
    // Run the test suite.
    run_all_tests();

    // Summary
    printf("sqlite3BitvecTest test suite finished. Ran %d tests with %d failures.\n",
           g_totalTests, g_testFailures);
    // Return non-zero if any test failed to signal issues without aborting on first failure.
    return g_testFailures != 0 ? 1 : 0;
}