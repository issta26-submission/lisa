// Test suite for sqlite3WhereUsesDeferredSeek in where.c
// - This test suite is self-contained with a lightweight harness (no GTest/GMock).
// - It focuses on validating that sqlite3WhereUsesDeferredSeek correctly returns
//   the value of pWInfo->bDeferredSeek for a valid WhereInfo object.
// - Static/internal helpers in the focal file are not exercised directly (as requested).
// - All tests are non-terminating on failure; they report diagnostics and continue.

#include <iostream>
#include <sqliteInt.h>
#include <whereInt.h>
#include <string>


// Include project headers to obtain the real WhereInfo type and the function under test.
// This assumes the project provides these headers in the include path.

// Lightweight test harness (non-terminating assertions)
static int g_test_failures = 0;

#define TEST_ASSERT_EQ(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        std::cerr << "TEST FAILED: " << std::string(msg) \
                  << " | expected: " << (expected) \
                  << ", actual: " << (actual) \
                  << " | In " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)

#define TEST_ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "TEST FAILED: " << std::string(msg) \
                  << " | Condition is false in " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)

// Test 1: True branch scenario - bDeferredSeek is 0, function should return 0
void test_sqlite3WhereUsesDeferredSeek_zero() {
    // Arrange: Create a WhereInfo instance with bDeferredSeek = 0
    WhereInfo w;
    w.bDeferredSeek = 0;

    // Act: Call the focal function
    int result = sqlite3WhereUsesDeferredSeek(&w);

    // Assert: Result should be exactly 0
    TEST_ASSERT_EQ(0, result, "sqlite3WhereUsesDeferredSeek should return 0 when bDeferredSeek is 0");
}

// Test 2: True branch scenario - bDeferredSeek is non-zero (1) and should yield 1
void test_sqlite3WhereUsesDeferredSeek_one() {
    // Arrange: Create a WhereInfo instance with bDeferredSeek = 1
    WhereInfo w;
    w.bDeferredSeek = 1;

    // Act: Call the focal function
    int result = sqlite3WhereUsesDeferredSeek(&w);

    // Assert: Result should be exactly 1
    TEST_ASSERT_EQ(1, result, "sqlite3WhereUsesDeferredSeek should return 1 when bDeferredSeek is 1");
}

// Test 3: Consistency check - changing the value should reflect in the return value
void test_sqlite3WhereUsesDeferredSeek_consistency() {
    WhereInfo w;
    // Start with 1
    w.bDeferredSeek = 1;
    int r1 = sqlite3WhereUsesDeferredSeek(&w);
    TEST_ASSERT_EQ(1, r1, "First read should reflect bDeferredSeek = 1");

    // Change to 0 and read again
    w.bDeferredSeek = 0;
    int r2 = sqlite3WhereUsesDeferredSeek(&w);
    TEST_ASSERT_EQ(0, r2, "Second read should reflect bDeferredSeek = 0 after change");
}

// Entry point for the test suite
int main() {
    std::cout << "Starting tests for sqlite3WhereUsesDeferredSeek...\n";

    // Execute tests
    test_sqlite3WhereUsesDeferredSeek_zero();
    test_sqlite3WhereUsesDeferredSeek_one();
    test_sqlite3WhereUsesDeferredSeek_consistency();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "All tests passed for sqlite3WhereUsesDeferredSeek.\n";
        return 0;
    } else {
        std::cout << g_test_failures << " test(s) FAILED for sqlite3WhereUsesDeferredSeek.\n";
        return 1;
    }
}