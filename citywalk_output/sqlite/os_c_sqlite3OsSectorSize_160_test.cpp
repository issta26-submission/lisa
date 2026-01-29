// Lightweight C++11 unit tests for sqlite3OsSectorSize (no GTest, no external frameworks)

#include <sqlite3.h>
#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Domain knowledge notes (as in Step 1-3):
// Focal method: sqlite3OsSectorSize(sqlite3_file* id)
// - Depends on id->pMethods->xSectorSize
// - If xSectorSize is non-null, returns xSectorSize(id); otherwise returns SQLITE_DEFAULT_SECTOR_SIZE
// Candidate Keywords: sqlite3_file, sqlite3_io_methods, xSectorSize, SQLITE_DEFAULT_SECTOR_SIZE

extern "C" {
}

// Simple test harness (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_EQ(expected, actual) do { \
    if ((expected) != (actual)) { \
        ++g_failures; \
        std::cerr << "EXPECT_EQ failed: expected " << (expected) \
                  << ", got " << (actual) << std::endl; \
    } \
} while(0)

static int g_last_called_with = 0;
static sqlite3_file* g_last_called_ptr = nullptr;

// Test 1: When xSectorSize is provided, sqlite3OsSectorSize should call it and return its value.
// This test also asserts that the function is invoked with the same sqlite3_file pointer.
static int s_test1_sector_size_custom(void)
{
    // Reset tracking
    g_last_called_ptr = nullptr;
    g_last_called_with = 0;

    // Provide a custom xSectorSize that records the input and returns a known value
    auto customSectorSize = [](sqlite3_file* id)->int {
        g_last_called_ptr = id;
        g_last_called_with = 1;
        return 0x7B; // 123 decimal
    };

    sqlite3_io_methods methods = {};
    methods.xSectorSize = customSectorSize;

    sqlite3_file file = {};
    // Point to our methods
    file.pMethods = &methods;

    // Call the function under test
    int result = sqlite3OsSectorSize(&file);

    // Verify both that the sector size function was called with the same id
    EXPECT_EQ(&file, g_last_called_ptr);
    EXPECT_EQ(1, g_last_called_with);
    // And that the return value is the value provided by the custom function
    EXPECT_EQ(0x7B, result);

    return 0;
}

// Test 2: When xSectorSize is null, sqlite3OsSectorSize should return SQLITE_DEFAULT_SECTOR_SIZE.
static int s_test2_sector_size_default(void)
{
    sqlite3_io_methods methods = {};
    methods.xSectorSize = nullptr;

    sqlite3_file file = {};
    file.pMethods = &methods;

    int result = sqlite3OsSectorSize(&file);

    EXPECT_EQ(SQLITE_DEFAULT_SECTOR_SIZE, result);
    return 0;
}

// Optional: a runner to execute all tests and report summary
static void run_all_tests(void)
{
    std::cout << "Running sqlite3OsSectorSize unit tests (C++11, no GTest)..." << std::endl;

    // Execute tests
    s_test1_sector_size_custom();
    s_test2_sector_size_default();

    // Report summary
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
    }
}

// Entry point
int main() {
    run_all_tests();
    // Return non-zero if there were failures to aid integration with CI systems
    return g_failures;
}