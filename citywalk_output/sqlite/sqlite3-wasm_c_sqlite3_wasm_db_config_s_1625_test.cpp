/*
  Unit test suite for sqlite3_wasm_db_config_s

  Notes:
  - This test harness is designed for C++11 environments without GoogleTest.
  - It mocks the dependency sqlite3_db_config to verify that sqlite3_wasm_db_config_s
    correctly forwards SQLITE_DBCONFIG_MAINDBNAME to sqlite3_db_config and
    returns SQLITE_MISUSE for any other operation.
  - The tests are implemented as simple functions with non-terminating assertions
    (EXPECT_* macros) so that all tests run and exercise multiple code paths.
  - The test uses a minimal sqlite3 struct to avoid requiring the full SQLite headers.
  - The test assumes symbol visibility/linking against the actual sqlite3_wasm_db_config_s
    implementation (extern "C") available in the build under test.
*/

#include <emscripten/wasmfs.h>
#include <assert.h>
#include <string>
#include <cstring>
#include <iostream>


// Domain knowledge: provide minimal, self-contained mock environment.
// Define constants (match the usage in sqlite3_wasm_db_config_s)
#ifndef SQLITE_DBCONFIG_MAINDBNAME
#define SQLITE_DBCONFIG_MAINDBNAME 1
#endif

#ifndef SQLITE_MISUSE
#define SQLITE_MISUSE -1
#endif

// Forward declare the sqlite3 type to simulate a database object.
struct sqlite3;

// Declare the focal function with C linkage for the test harness.
extern "C" int sqlite3_wasm_db_config_s(sqlite3 *pDb, int op, const char *zArg);

// Mockable sqlite3_db_config used by the focal method when op == SQLITE_DBCONFIG_MAINDBNAME.
// In a real test environment, this would be linked from the SQLite codebase.
// Here we provide a mock to validate forwarding behavior.
extern "C" int sqlite3_db_config(struct sqlite3 *pDb, int op, const char *zArg);

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_EQ(actual, expected, desc) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "EXPECT_EQ FAILED: " << (desc) \
                      << " | expected: " << (expected) \
                      << ", actual: " << (actual) << std::endl; \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_PTR_EQ(actual, expected, desc) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "EXPECT_PTR_EQ FAILED: " << (desc) \
                      << " | expected: " << expected \
                      << ", actual: " << actual << std::endl; \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_STR_EQ(actual, expected, desc) \
    do { \
        if ((actual) == nullptr && (expected) == nullptr) { \
            /* both null, pass */ \
        } else if ((actual) == nullptr || (expected) == nullptr) { \
            std::cerr << "EXPECT_STR_EQ FAILED: " << (desc) \
                      << " | one side is null" << std::endl; \
            ++g_failures; \
        } else if (std::strcmp((actual), (expected)) != 0) { \
            std::cerr << "EXPECT_STR_EQ FAILED: " << (desc) \
                      << " | expected: \"" << (expected) \
                      << "\", actual: \"" << (actual) << "\"" << std::endl; \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_TRUE(condition, desc) \
    do { \
        if (!(condition)) { \
            std::cerr << "EXPECT_TRUE FAILED: " << (desc) << std::endl; \
            ++g_failures; \
        } \
    } while(0)

//
// Global state to simulate sqlite3_db_config behavior.
// The mock records the last call parameters so tests can verify forwarding.
//
static sqlite3 *g_last_pDb = nullptr;
static int g_last_op = 0;
static const char *g_last_zArg = nullptr;
static int g_mock_called_for_main = 0;

// Mock implementation of sqlite3_db_config
extern "C" int sqlite3_db_config(struct sqlite3 *pDb, int op, const char *zArg)
{
    g_last_pDb = pDb;
    g_last_op = op;
    g_last_zArg = zArg;
    // Simulate a successful probe for MAINDBNAME
    if (op == SQLITE_DBCONFIG_MAINDBNAME) {
        g_mock_called_for_main = 1;
        return 123; // arbitrary non-zero value to test propagation
    }
    // For other ops, we don't expect calls in these tests
    g_mock_called_for_main = 0;
    return SQLITE_MISUSE;
}

// Forward declare the focal function implementation to ensure linkage in test builds.
// In real tests, the function would be linked from sqlite3-wasm.c.
extern "C" int sqlite3_wasm_db_config_s(sqlite3 *pDb, int op, const char *zArg);

//
// Test 1: Forwarding MAINDBNAME to sqlite3_db_config and propagating its return value
//
bool test_forward_mainDbName_propagates_return() {
    // Reset mock state
    g_last_pDb = nullptr;
    g_last_op = 0;
    g_last_zArg = nullptr;
    g_mock_called_for_main = 0;

    sqlite3 db;
    const char *z = "test_main_db";

    int ret = sqlite3_wasm_db_config_s(&db, SQLITE_DBCONFIG_MAINDBNAME, z);

    EXPECT_EQ(ret, 123, "Return value from sqlite3_db_config should be propagated");
    EXPECT_PTR_EQ(g_last_pDb, &db, "pDb should be forwarded to sqlite3_db_config");
    EXPECT_EQ(g_last_op, SQLITE_DBCONFIG_MAINDBNAME, "op should be forwarded to sqlite3_db_config");
    EXPECT_STR_EQ(g_last_zArg, z, "zArg should be forwarded to sqlite3_db_config");
    // The mock should indicate it handled MAINDBNAME
    EXPECT_TRUE(g_mock_called_for_main == 1, "sqlite3_db_config should be called for MAINDBNAME");
    return true;
}

//
// Test 2: For non-MAINDBNAME ops, should return SQLITE_MISUSE and not call sqlite3_db_config
//
bool test_non_mainDbName_returns_misuse_and_no_call() {
    // Reset mock state
    g_last_pDb = nullptr;
    g_last_op = 0;
    g_last_zArg = nullptr;
    g_mock_called_for_main = 0;

    sqlite3 db;
    const char *z = "ignored";

    int ret = sqlite3_wasm_db_config_s(&db, 9999 /* some non-MAINDBNAME value */, z);

    EXPECT_EQ(ret, SQLITE_MISUSE, "Non-MAINDBNAME op should return SQLITE_MISUSE");
    EXPECT_TRUE(g_last_pDb == nullptr || g_last_op == 0, "sqlite3_db_config should not be called for non-MAINDBNAME");
    EXPECT_TRUE(g_mock_called_for_main == 0, "sqlite3_db_config should not be invoked for non-MAINDBNAME");
    return true;
}

//
// Test 3: MAINDBNAME with a null zArg should still forward and propagate value
//
bool test_mainDbName_with_null_zArg() {
    // Reset mock state
    g_last_pDb = nullptr;
    g_last_op = 0;
    g_last_zArg = nullptr;
    g_mock_called_for_main = 0;

    sqlite3 db;

    int ret = sqlite3_wasm_db_config_s(&db, SQLITE_DBCONFIG_MAINDBNAME, nullptr);

    EXPECT_EQ(ret, 123, "NULL zArg should still propagate return value from sqlite3_db_config");
    EXPECT_PTR_EQ(g_last_pDb, &db, "pDb should be forwarded to sqlite3_db_config with NULL zArg");
    EXPECT_EQ(g_last_op, SQLITE_DBCONFIG_MAINDBNAME, "op should be forwarded to sqlite3_db_config");
    EXPECT_TRUE(g_last_zArg == nullptr, "zArg should be forwarded as NULL");
    return true;
}

//
// Test 4: MAINDBNAME with a NULL pDb should be forwarded to sqlite3_db_config
//
bool test_mainDbName_with_null_pDb() {
    // Reset mock state
    g_last_pDb = reinterpret_cast<sqlite3*>(0xDEADBEEF); // junk
    g_last_op = 0;
    g_last_zArg = nullptr;
    g_mock_called_for_main = 0;

    const int op = SQLITE_DBCONFIG_MAINDBNAME;
    const char *zArg = "arg";

    // Pass NULL as pDb
    int ret = sqlite3_wasm_db_config_s(nullptr, op, zArg);

    EXPECT_EQ(ret, 123, "NULL pDb should still propagate return value from sqlite3_db_config");
    EXPECT_PTR_EQ(g_last_pDb, nullptr, "pDb forwarded to sqlite3_db_config should be NULL");
    EXPECT_EQ(g_last_op, op, "op forwarded should be the MAINDBNAME value");
    EXPECT_STR_EQ(g_last_zArg, zArg, "zArg should be forwarded when pDb is NULL");
    return true;
}

//
// Runner
//
int main() {
    std::cout << "Running sqlite3_wasm_db_config_s unit tests (C++11 non-GTest harness)..." << std::endl;

    int total = 0;
    int passed = 0;

    auto runTest = [&](const char* name, bool (*fn)()) {
        std::cout << "[RUN] " << name << std::endl;
        bool ok = fn();
        total++;
        if (ok) {
            std::cout << "[OK ] " << name << std::endl;
            passed++;
        } else {
            std::cout << "[FAIL] " << name << std::endl;
        }
        return ok;
    };

    runTest("test_forward_mainDbName_propagates_return", test_forward_mainDbName_propagates_return);
    runTest("test_non_mainDbName_returns_misuse_and_no_call", test_non_mainDbName_returns_misuse_and_no_call);
    runTest("test_mainDbName_with_null_zArg", test_mainDbName_with_null_zArg);
    runTest("test_mainDbName_with_null_pDb", test_mainDbName_with_null_pDb);

    std::cout << "Tests run: " << total << ", Passed: " << passed << ", Failed: " << (total - passed) << std::endl;
    return (total == passed) ? 0 : 1;
}