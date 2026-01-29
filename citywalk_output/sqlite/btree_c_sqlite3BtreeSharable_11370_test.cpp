// Test suite for the focal method: sqlite3BtreeSharable
// This file provides a small, self-contained test harness (no GTest) to verify
// that sqlite3BtreeSharable correctly returns the sharable flag from a Btree.
// The tests reuse the real Btree type and the real function signature from the
// project (via btreeInt.h). We compile as C++11, linking against the project
// object files that implement sqlite3BtreeSharable.
//
// Key concepts covered per steps:
// - Step 1: Understand that sqlite3BtreeSharable simply returns p->sharable (an int).
//   Candidate Keywords: sharable, Btree, sqlite3BtreeSharable
// - Step 2: Use the class dependency (Btree) to construct test cases that cover
//   true/false-ish scenarios by manipulating p->sharable.
// - Step 3: Provide non-terminating assertions that allow multiple tests to run,
//   ensuring executable test cases without terminating on first failure.

#include <btreeInt.h>
#include <iostream>
#include <string>


// Import C headers within C linkage to ensure proper symbol resolution.
// This header defines the Btree structure and the sqlite3BtreeSharable function.
extern "C" {
}

// Ensure the C function is visible to C++ (linkage is C).
extern "C" int sqlite3BtreeSharable(Btree *p);

// Lightweight non-terminating test assertions
static int g_failures = 0;

#define EXPECT_EQ(expected, actual) do { \
    if ((actual) != (expected)) { \
        std::cerr << "EXPECT_EQ failed: expected " << (expected) \
                  << ", got " << (actual) \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(false)

#define EXPECT_TRUE(expr) do { \
    if (!(expr)) { \
        std::cerr << "EXPECT_TRUE failed: expression is false at " \
                  << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(false)

#define EXPECT_FALSE(expr) do { \
    if ((expr)) { \
        std::cerr << "EXPECT_FALSE failed: expression is true at " \
                  << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(false)

// Test 1: sharable = 0 should return 0
static bool test_sharable_zero() {
    Btree p = {};            // Initialize all fields to zero/empty
    p.sharable = 0;           // Candidate branch: false/disabled sharing
    int res = sqlite3BtreeSharable(&p);
    EXPECT_EQ(0, res);
    return g_failures == 0;
}

// Test 2: sharable = 1 should return 1
static bool test_sharable_one() {
    Btree p = {};
    p.sharable = 1;            // Candidate branch: true/sharing enabled
    int res = sqlite3BtreeSharable(&p);
    EXPECT_EQ(1, res);
    return g_failures == 0;
}

// Test 3: sharable = -5 should return -5 (to verify exact content transfer)
static bool test_sharable_negative() {
    Btree p = {};
    p.sharable = -5;            // Negative value to ensure exact memory content is returned
    int res = sqlite3BtreeSharable(&p);
    EXPECT_EQ(-5, res);
    return g_failures == 0;
}

// Entrypoint for the test suite
int main() {
    // Domain knowledge notes:
    // - We exercise the function with 0, 1, and -5 to cover typical and boundary values.
    // - Static/internal dependencies are not accessed directly; we rely on the
    //   public API surface declared in the project's headers.
    bool ok = true;

    // Run tests and accumulate results
    ok &= test_sharable_zero();
    ok &= test_sharable_one();
    ok &= test_sharable_negative();

    // Summary
    if (g_failures == 0) {
        std::cout << "All sqlite3BtreeSharable tests passed." << std::endl;
    } else {
        std::cerr << g_failures << " test(s) failed for sqlite3BtreeSharable." << std::endl;
    }

    return g_failures == 0 ? 0 : 1;
}