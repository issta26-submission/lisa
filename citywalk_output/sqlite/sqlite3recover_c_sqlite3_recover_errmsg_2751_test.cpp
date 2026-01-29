// Test suite for sqlite3_recover_errmsg in sqlite3recover.c
// This test is designed for C++11 and does not rely on any external test framework.
// It uses a lightweight in-file test harness with non-terminating assertions to maximize coverage.

#include <string.h>
#include <assert.h>
#include <sqlite3recover.h>
#include <cstring>
#include <iostream>


// Bring in the C API with C linkage
extern "C" {
}

// Lightweight test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Non-terminating expectation macro: logs failures but continues execution
#define TEST_EXPECT(cond, msg) do {                    \
    ++g_tests_run;                                      \
    if(!(cond)) {                                       \
        ++g_tests_failed;                               \
        std::cerr << "Test failed: " << (msg)          \
                  << " [@" << __FILE__ << ":" << __LINE__ << "]" \
                  << std::endl;                         \
    }                                                   \
} while(0)

// Helper to print a summary at the end
static void printSummary() {
    std::cout << "Tests run: " << g_tests_run
              << ", Failed: " << g_tests_failed
              << std::endl;
}

/*
 * Test 1: Non-null p with errCode != SQLITE_NOMEM
 * Verify that the function returns the exact zErrMsg pointer
 * and that the content matches the zErrMsg string.
 */
static void test_sqlite3_recover_errmsg_nonnull_nonmem() {
    sqlite3_recover rec;
    rec.errCode = 0;                 // Some code other than SQLITE_NOMEM
    rec.zErrMsg = "custom error";

    const char *res = sqlite3_recover_errmsg(&rec);

    // Address-level check: returned pointer should be the same as p->zErrMsg
    TEST_EXPECT(res == rec.zErrMsg, "Returned pointer should equal p->zErrMsg when errCode != SQLITE_NOMEM");

    // Content-level check: content should match the zErrMsg string
    if (res != nullptr) {
        TEST_EXPECT(std::strcmp(res, "custom error") == 0,
                    "Returned message content should be 'custom error'");
    } else {
        TEST_EXPECT(false, "Returned null pointer unexpectedly for non-null p");
    }
}

/*
 * Test 2: Null pointer p
 * Verify that the function returns "out of memory".
 * This tests the null-guard path.
 */
static void test_sqlite3_recover_errmsg_nullptr() {
    const char *res = sqlite3_recover_errmsg(nullptr);

    // The function should not return null and should yield "out of memory"
    TEST_EXPECT(res != nullptr, "Returned pointer should not be null when p is null");
    if (res != nullptr) {
        TEST_EXPECT(std::strcmp(res, "out of memory") == 0,
                    "Returned message should be 'out of memory' for null p");
    }
}

/*
 * Test 3: Non-null p with errCode == SQLITE_NOMEM
 * Verify that the function returns "out of memory" even if zErrMsg is set.
 */
static void test_sqlite3_recover_errmsg_memerror() {
    sqlite3_recover rec;
    rec.errCode = SQLITE_NOMEM;
    rec.zErrMsg = "should not matter";

    const char *res = sqlite3_recover_errmsg(&rec);

    // Content check: should be the standard memory error message
    TEST_EXPECT(res != nullptr && std::strcmp(res, "out of memory") == 0,
                "Returned should be 'out of memory' when errCode == SQLITE_NOMEM");
}

/*
 * Test 4: Non-null p with errCode != SQLITE_NOMEM but zErrMsg is null
 * Verify that the function returns a null pointer in this edge case (content is undefined).
 */
static void test_sqlite3_recover_errmsg_zerrmsg_null_when_ok() {
    sqlite3_recover rec;
    rec.errCode = 0;       // Not SQLITE_NOMEM
    rec.zErrMsg = nullptr; // zErrMsg is null

    const char *res = sqlite3_recover_errmsg(&rec);

    // Expect a null return when zErrMsg is null for a non-NOMEM condition
    TEST_EXPECT(res == nullptr, "Expected nullptr when zErrMsg is null and errCode != SQLITE_NOMEM");
}

int main() {
    // Run all tests
    test_sqlite3_recover_errmsg_nonnull_nonmem();
    test_sqlite3_recover_errmsg_nullptr();
    test_sqlite3_recover_errmsg_memerror();
    test_sqlite3_recover_errmsg_zerrmsg_null_when_ok();

    // Print summary
    printSummary();

    // Return non-zero if any test failed
    return g_tests_failed ? 1 : 0;
}