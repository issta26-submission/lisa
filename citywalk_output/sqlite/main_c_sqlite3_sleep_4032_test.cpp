// Lightweight C++11 test harness for sqlite3_sleep (Step 2 & 3 focused)
// This test mocks the external dependencies (sqlite3_vfs_find and sqlite3OsSleep)
// to deterministically exercise the control flow of sqlite3_sleep without depending
// on an actual SQLite environment. It implements three test cases to cover true/false
// branches of the key predicate and clamping behavior.
// Note: This is a self-contained unit test harness and does not rely on GTest.
// It uses simple non-terminating assertions (logging failures) and a main() entry
// to run the tests as instructed (Step 10).

#include <iostream>
#include <cstdio>
#include <sqliteInt.h>
#include <cstdint>


// Forward declaration of the minimal sqlite3_vfs type used by the focal function.
// We only need an incomplete type to allow pointer usage in sqlite3_sleep.
typedef struct sqlite3_vfs sqlite3_vfs;

// Global flags and state used by our mocks to drive test scenarios.
static bool g_vfs_should_be_null = false;      // When true, sqlite3_vfs_find returns null (to test the early return branch)
static int g_last_microseconds_requested = -1; // Capture the argument passed to sqlite3OsSleep
static int g_os_sleep_return_value = 2500;     // Mocked OsSleep return value (in microseconds, to be divided by 1000)

// Mocked extern "C" function: sqlite3_vfs_find
// This simulates the behavior of SQLite's VFS finder for our tests.
extern "C" sqlite3_vfs* sqlite3_vfs_find(const char* zVfs) {
    (void)zVfs; // unused parameter in tests
    if (g_vfs_should_be_null) {
        return nullptr;
    }
    // Return a non-null dummy VFS pointer to exercise the non-null path.
    return reinterpret_cast<sqlite3_vfs*>(reinterpret_cast<uintptr_t>(0x1));
}

// Mocked extern "C" function: sqlite3OsSleep
// This simulates the underlying OS sleep API. It records the input and returns
// a value depending on the input to exercise the division logic in sqlite3_sleep.
extern "C" int sqlite3OsSleep(sqlite3_vfs* /*pVfs*/, int microseconds) {
    g_last_microseconds_requested = microseconds;
    if (microseconds == 0) {
        // Simulate immediate return when asked to sleep 0 microseconds.
        return 0;
    }
    // Return a fixed value to test the division by 1000 in sqlite3_sleep.
    return g_os_sleep_return_value;
}

// The focal method under test (redeclared here for a self-contained unit test).
// We mirror the original logic exactly as provided.
int sqlite3_sleep(int ms) {
    sqlite3_vfs *pVfs;
    int rc;
    pVfs = sqlite3_vfs_find(0);
    if( pVfs==0 ) return 0;
    /* This function works in milliseconds, but the underlying OsSleep()
    ** API uses microseconds. Hence the 1000's.
    */
    rc = (sqlite3OsSleep(pVfs, ms<0 ? 0 : 1000*ms)/1000);
    return rc;
}

// Simple non-terminating assertion helpers to maximize code execution.
// They log failures but do not abort the test suite.
static int g_passed = 0;
static int g_failed = 0;

#define CHECK_EQ(expected, actual, test_name) do { \
    if (static_cast<long long>(expected) != static_cast<long long>(actual)) { \
        std::cerr << "[FAIL] " << test_name << ": expected " \
                  << static_cast<long long>(expected) \
                  << ", got " << static_cast<long long>(actual) \
                  << std::endl; \
        ++g_failed; \
    } else { \
        ++g_passed; \
    } \
} while(0)

#define TEST_LOG_START(name) std::cout << "Running test: " << name << std::endl;
#define TEST_LOG_END() std::cout << "Test finished. " \
                                << "Passed: " << g_passed \
                                << ", Failed: " << g_failed << std::endl;

// Namespace to group tests (as per best practices and to respect static scope)
namespace sqlite3_sleep_tests {

    // Test 1: When sqlite3_vfs_find returns null, sqlite3_sleep should return 0.
    // This covers the true branch of the "pVfs==0" predicate.
    void test_vfs_find_returns_null() {
        TEST_LOG_START("test_vfs_find_returns_null");
        // Arrange
        g_vfs_should_be_null = true;
        // Act
        int result = sqlite3_sleep(10); // any positive ms
        // Assert
        CHECK_EQ(0, result, "sqlite3_sleep should return 0 when VFS is not found");
        // Cleanup
        g_vfs_should_be_null = false;
        TEST_LOG_END();
    }

    // Test 2: When VFS is found and OsSleep returns a non-zero value, the function
    // should return OsSleep(...) / 1000. We simulate ms = 3 to exercise the
    // 1000*ms path and ensure proper division behavior.
    void test_vfs_found_positive_ms_returns_expected() {
        TEST_LOG_START("test_vfs_found_positive_ms_returns_expected");
        // Arrange
        g_vfs_should_be_null = false;           // VFS found
        g_last_microseconds_requested = -1;
        g_os_sleep_return_value = 2500;           // OsSleep returns 2500 microseconds
        // Act
        int result = sqlite3_sleep(3);            // 3 ms -> 3000 microseconds passed to OsSleep
        // Assert
        // 2500 / 1000 -> 2 (integer division)
        CHECK_EQ(2, result, "sqlite3_sleep should return OsSleep()/1000 for ms=3 with non-null VFS");
        // Sanity check the mocked OsSleep received the expected microseconds
        CHECK_EQ(3000, g_last_microseconds_requested, "OsSleep should receive 3000 microseconds for ms=3");
        TEST_LOG_END();
    }

    // Test 3: When VFS is found and ms is negative, the function clamps to 0 microseconds.
    // We expect OsSleep to be called with 0, which should yield a 0 return value.
    void test_vfs_found_negative_ms_clamps_to_zero() {
        TEST_LOG_START("test_vfs_found_negative_ms_clamps_to_zero");
        // Arrange
        g_vfs_should_be_null = false;            // VFS found
        g_last_microseconds_requested = -1;
        g_os_sleep_return_value = 2500;            // Irrelevant for 0 microseconds, will return 0
        // Act
        int result = sqlite3_sleep(-5);            // ms < 0 -> 0 microseconds passed
        // Assert
        CHECK_EQ(0, result, "sqlite3_sleep should return 0 when ms is negative (clamped to 0)");
        // Verify OsSleep was invoked with 0 microseconds
        CHECK_EQ(0, g_last_microseconds_requested, "OsSleep should be called with 0 microseconds when ms<0");
        TEST_LOG_END();
    }

    // Helper to run all tests
    void run_all_tests() {
        test_vfs_find_returns_null();
        test_vfs_found_positive_ms_returns_expected();
        test_vfs_found_negative_ms_clamps_to_zero();
        // Summary
        std::cout << "Test Summary: "
                  << "Passed=" << g_passed
                  << ", Failed=" << g_failed
                  << std::endl;
    }
} // namespace sqlite3_sleep_tests

// Entry point: run tests as per domain guidance (Step 10)
int main() {
    sqlite3_sleep_tests::run_all_tests();
    // Return non-zero if any test failed to aid quick integration in CI if desired
    return (sqlite3_sleep_tests::g_failed == 0) ? 0 : 1;
}