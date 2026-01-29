// Test suite for sqlite3OsWrite in os.c using a single translation unit approach.
// This file compiles with a C++11 compiler and does not rely on Google Test.
// It includes the os.c implementation directly and provides lightweight test harnesses.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <os.c>


// Define the DO_OS_MALLOC_TEST macro to a no-op for testing.
// This avoids altering os.c behavior unrelated to sqlite3OsWrite testing.
#define DO_OS_MALLOC_TEST(id) (void)0

// Include the focal implementation so that sqlite3OsWrite is defined within this TU.

// Lightweight test harness (non-terminating assertions)
static int g_tests_failed = 0;
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << #cond << " at line " << __LINE__ << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " << #a << " != " << #b << " (" << (a) << " != " << (b) << ")" \
                  << " at line " << __LINE__ << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

// Test 1: Basic behavior - sqlite3OsWrite should delegate to id->pMethods->xWrite
// and return whatever xWrite returns, passing through all arguments.
static void test_sqlite3OsWrite_basic_pass_through() {
    // Prepare a fake xWrite that records its arguments and returns a fixed value.
    static int g_call_count = 0;
    static sqlite3_file *g_last_id = nullptr;
    static const void *g_last_pBuf = nullptr;
    static int g_last_amt = 0;
    static i64 g_last_offset = 0;

    auto fake_xWrite = [](sqlite3_file *id, const void *pBuf, int amt, i64 offset) -> int {
        g_call_count++;
        g_last_id = id;
        g_last_pBuf = pBuf;
        g_last_amt = amt;
        g_last_offset = offset;
        return 123;  // deterministic return value to verify propagation
    };

    // Set up a sqlite3_io_methods with only xWrite implemented
    sqlite3_io_methods meths;
    std::memset(&meths, 0, sizeof(meths));
    meths.xWrite = fake_xWrite;

    // Create a sqlite3_file instance pointing to our mock vtable
    sqlite3_file id;
    std::memset(&id, 0, sizeof(id));
    id.pMethods = &meths;

    const char buffer[] = "hello";
    int result = sqlite3OsWrite(&id, buffer, (int)sizeof(buffer) - 1, (i64)42);

    // Verify the wrapper returns the exact value from xWrite and passes through parameters
    EXPECT_EQ(result, 123);
    EXPECT_EQ(g_call_count, 1);
    EXPECT_TRUE(g_last_id == &id);
    EXPECT_EQ(g_last_pBuf, buffer);
    EXPECT_EQ(g_last_amt, (int)sizeof(buffer) - 1);
    EXPECT_EQ(g_last_offset, (i64)42);
}

// Test 2: Return value propagation - sqlite3OsWrite should propagate the value returned by xWrite
static void test_sqlite3OsWrite_return_propagation() {
    static int g_call_count = 0;
    static sqlite3_file *g_last_id = nullptr;
    static const void *g_last_pBuf = nullptr;
    static int g_last_amt = 0;
    static i64 g_last_offset = 0;

    // Second fake that returns a different value
    auto fake_xWrite2 = [](sqlite3_file *id, const void *pBuf, int amt, i64 offset) -> int {
        g_call_count++;
        g_last_id = id;
        g_last_pBuf = pBuf;
        g_last_amt = amt;
        g_last_offset = offset;
        return -999;  // different return value to verify propagation
    };

    sqlite3_io_methods meths;
    std::memset(&meths, 0, sizeof(meths));
    meths.xWrite = fake_xWrite2;

    sqlite3_file id;
    std::memset(&id, 0, sizeof(id));
    id.pMethods = &meths;

    const char buffer[] = "world";
    int result = sqlite3OsWrite(&id, buffer, (int)sizeof(buffer) - 1, (i64)7);

    EXPECT_EQ(result, -999);
    EXPECT_EQ(g_call_count, 1);
    EXPECT_TRUE(g_last_id == &id);
    EXPECT_EQ(g_last_pBuf, buffer);
    EXPECT_EQ(g_last_amt, (int)sizeof(buffer) - 1);
    EXPECT_EQ(g_last_offset, (i64)7);
}

// Entry point
int main() {
    std::cout << "Running sqlite3OsWrite unit tests (single translation unit via os.c inclusion)" << std::endl;

    test_sqlite3OsWrite_basic_pass_through();
    test_sqlite3OsWrite_return_propagation();

    if (g_tests_failed == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_tests_failed << " test(s) failed." << std::endl;
        return g_tests_failed;
    }
}