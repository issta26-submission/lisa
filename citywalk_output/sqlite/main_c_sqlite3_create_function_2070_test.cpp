// Test suite for sqlite3_create_function (C API wrapper in main.c)
// This test suite is written for C++11 (no Google Test). It uses the real
// SQLite C API (sqlite3.h) to exercise the focal function by registering
// user-defined functions and executing simple SQL against an in-memory DB.
// The tests are non-terminating: they log failures but continue execution.

#include <functional>
#include <vector>
#include <sqlite3.h>
#include <string>
#include <sqliteInt.h>
#include <iostream>


// Include the SQLite API header. The project under test should link against
// the SQLite library. If you are running these tests in a real environment,
// ensure -lsqlite3 (or equivalent) is provided during linking.

// Domain Knowledge: Candidate Keywords representing core dependencies
// - sqlite3_open / sqlite3_open_v2 / sqlite3_close
// - sqlite3_create_function (the focal wrapper under test)
// - sqlite3_prepare_v2, sqlite3_step, sqlite3_finalize
// - sqlite3_value, sqlite3_context, sqlite3_result_text
// - SQLITE_OK, SQLITE_UTF8, SQLITE_STATIC
// - In-memory databases (":memory:")
// - Function pointers for xSFunc, xStep, xFinal (scalar vs aggregate)
// These keywords guide the test cases to exercise the wrapper's forwarding
// behavior, argument handling, and typical success/failure scenarios.

// Simple non-terminating test harness (no GTest)
static int g_total = 0;
static int g_failed = 0;

#define LOG(msg) std::cerr << msg << std::endl
#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_total; \
        if (!(cond)) { \
            LOG("EXPECT_TRUE FAILED: " << (msg)); \
            ++g_failed; \
        } \
    } while (0)

#define EXPECT_EQ(val, exp, msg) \
    do { \
        ++g_total; \
        if (!((val) == (exp))) { \
            LOG("EXPECT_EQ FAILED: " << (msg) \
                << " | Expected: " << (exp) \
                << " Got: " << (val)); \
            ++g_failed; \
        } \
    } while (0)

static void testDomainComment() {
    // Placeholder to ensure the test file references domain keywords
    // (No runtime effect; demonstrates Step 1 candidate keywords alignment)
    // This comment block asserts the test intent matches dependencies.
}

// Forward declaration for the test function used by sqlite3_create_function
static void testFunc(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    // Return a simple text value "ok" regardless of input.
    sqlite3_result_text(ctx, "ok", -1, SQLITE_STATIC);
}

// Helper: run a scalar function test by executing SELECT mytest(1);
static bool run_query_and_check_result(sqlite3* db, const std::string& sql, const std::string& expected) {
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        LOG("Failed to prepare statement: " << sqlite3_errmsg(db));
        return false;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        LOG("Expected a row result, got rc=" << rc);
        sqlite3_finalize(stmt);
        return false;
    }

    const unsigned char* text = sqlite3_column_text(stmt, 0);
    std::string got = text ? reinterpret_cast<const char*>(text) : "";
    sqlite3_finalize(stmt);

    return got == expected;
}

// Test 1: Register a scalar function with nArg = 1 and invoke via SELECT
static void test_sqlite3_create_function_scalar_one_arg() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr && rc == SQLITE_OK, "open in-memory DB");

    // Register a scalar function named "mytest" with 1 argument
    rc = sqlite3_create_function(db, "mytest", 1, SQLITE_UTF8, nullptr, testFunc, nullptr, nullptr);
    EXPECT_EQ(rc, SQLITE_OK, "sqlite3_create_function with nArg=1 should succeed");

    // Prepare and run: SELECT mytest(1);
    bool ok = run_query_and_check_result(db, "select mytest(1);", "ok");
    EXPECT_TRUE(ok, "Function mytest should return 'ok' for a single argument");

    sqlite3_close(db);
}

// Test 2: Register a scalar function with nArg = 0 and invoke via SELECT
static void test_sqlite3_create_function_scalar_zero_args() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr && rc == SQLITE_OK, "open in-memory DB for zero-arg test");

    // Register a scalar function named "myzero" with 0 arguments
    rc = sqlite3_create_function(db, "myzero", 0, SQLITE_UTF8, nullptr, testFunc, nullptr, nullptr);
    EXPECT_EQ(rc, SQLITE_OK, "sqlite3_create_function with nArg=0 should succeed");

    // Prepare and run: SELECT myzero();
    bool ok = run_query_and_check_result(db, "select myzero();", "ok");
    EXPECT_TRUE(ok, "Function myzero should return 'ok' when called with no args");

    sqlite3_close(db);
}

// Test 3: Register with nArg = -1 (any number of args) and invoke with multiple args
static void test_sqlite3_create_function_any_args() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr && rc == SQLITE_OK, "open in-memory DB for variadic arg test");

    // Register with -1 (any number of args)
    rc = sqlite3_create_function(db, "myvar", -1, SQLITE_UTF8, nullptr, testFunc, nullptr, nullptr);
    EXPECT_EQ(rc, SQLITE_OK, "sqlite3_create_function with nArg=-1 should succeed");

    // Invoke with two arguments
    bool ok = run_query_and_check_result(db, "select myvar(1,2);", "ok");
    EXPECT_TRUE(ok, "Function myvar should return 'ok' for multiple args (nArg=-1)");

    sqlite3_close(db);
}

// Test 4: Error path - NULL db handle should fail
static void test_sqlite3_create_function_null_db_fails() {
    // Define a function pointer to testFunc; arguments as NULLs
    int rc = sqlite3_create_function(nullptr, "bad", 1, SQLITE_UTF8, nullptr, testFunc, nullptr, nullptr);
    EXPECT_TRUE(rc != SQLITE_OK, "sqlite3_create_function with NULL db should fail");
}

// Test 5: Error path - NULL xSFunc should fail to register
static void test_sqlite3_create_function_null_func_ptr_fails() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr && rc == SQLITE_OK, "open in-memory DB for NULL xSFunc test");

    // Pass NULL for xSFunc; should fail
    rc = sqlite3_create_function(db, "badfunc2", 1, SQLITE_UTF8, nullptr, nullptr, nullptr, nullptr);
    EXPECT_TRUE(rc != SQLITE_OK, "sqlite3_create_function with NULL xSFunc should fail");

    sqlite3_close(db);
}

// Main entry: run all tests
int main() {
    testDomainComment(); // Step 1: acknowledge candidates

    // Run individual tests
    test_sqlite3_create_function_scalar_one_arg();
    test_sqlite3_create_function_scalar_zero_args();
    test_sqlite3_create_function_any_args();
    test_sqlite3_create_function_null_db_fails();
    test_sqlite3_create_function_null_func_ptr_fails();

    // Summary
    std::cout << "==================== Test Summary ====================" << std::endl;
    std::cout << "Total checks attempted: " << g_total << std::endl;
    std::cout << "Total failures: " << g_failed << std::endl;
    if (g_failed == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests FAILED." << std::endl;
        return 1;
    }
}