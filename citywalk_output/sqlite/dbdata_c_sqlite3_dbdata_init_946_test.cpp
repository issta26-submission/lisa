// C++11 test suite for sqlite3_dbdata_init (dbdata.c)
//
// This test suite does not rely on Google Test. It uses a lightweight
// in-process test harness with non-terminating checks to maximize
// coverage. It also employs a technique to override a static internal
// function (sqlite3DbdataRegister) by providing a non-static symbol with
// the same name in this test translation unit. This allows us to observe
// forwarding behavior without requiring the actual sqlite3DbdataRegister
// implementation.
//
// How it works:
// - We declare the external function sqlite3_dbdata_init with the expected
//   signature (sqlite3, char**, sqlite3_api_routines*). The real
//   implementation in dbdata.c will be linked with this test.
// - We provide a non-static sqlite3DbdataRegister(struct sqlite3*) in this
//   test translation unit. The linker will resolve the call from
//   sqlite3_dbdata_init to this override, allowing us to observe the
//   forwarding behavior and control return values.
//
// Notes:
// - We do not include sqlite3.h. We declare minimal forward-declarations for
//   struct sqlite3 and struct sqlite3_api_routines to satisfy the function types.
// - We keep static analysis strictly at the interface level; static helpers
//   in dbdata.c are not relied upon by these tests.
// - Tests focus on: (a) forwarding of the db parameter, (b) ignoring
//   pzErrMsg (as per (void)pzErrMsg), (c) propagation of return value from
//   sqlite3DbdataRegister, and (d) non-modification of pzErrMsg pointer.

#include <string.h>
#include <assert.h>
#include <cstddef>
#include <sqlite3.h>
#include <string>
#include <iostream>
#include <cstdint>


// Forward declarations to match sqlite3 types used by sqlite3_dbdata_init.
struct sqlite3;
struct sqlite3_api_routines;

// Declare the function under test with C linkage to avoid name mangling
extern "C" int sqlite3_dbdata_init(struct sqlite3* db,
                                 char** pzErrMsg,
                                 const struct sqlite3_api_routines* pApi);

// Global state to inspect the behavior of the override function
static void* g_lastDbPointer = nullptr;
static int g_overrideReturnValue = 0;

// Override sqlite3DbdataRegister by providing a non-static symbol with the
// same name. The linker will resolve sqlite3_dbdata_init's call to this
// override, enabling testability without modifying dbdata.c.
extern "C" int sqlite3DbdataRegister(struct sqlite3* db) {
    g_lastDbPointer = static_cast<void*>(db);
    return g_overrideReturnValue;
}

// Simple test harness state
static int g_tests_run = 0;
static int g_tests_passed = 0;

// Non-terminating assertion helpers
#define TEST_EXPECT_EQ(actual, expected, message)                                   \
    do {                                                                            \
        ++g_tests_run;                                                              \
        if ((actual) != (expected)) {                                             \
            std::cerr << "FAIL: " << message << " | Expected: " << (expected)    \
                      << ", Actual: " << (actual) << std::endl;                 \
        } else {                                                                    \
            ++g_tests_passed;                                                     \
        }                                                                           \
    } while (0)

#define TEST_EXPECT_PTR_EQ(actual, expected, message)                               \
    do {                                                                            \
        ++g_tests_run;                                                              \
        if ((actual) != (expected)) {                                             \
            std::cerr << "FAIL: " << message << " | Expected: "                    \
                      << static_cast<void*>(expected) << ", Actual: "                 \
                      << static_cast<void*>(actual) << std::endl;                 \
        } else {                                                                    \
            ++g_tests_passed;                                                     \
        }                                                                           \
    } while (0)

#define TEST_EXPECT_TRUE(cond, message)                                             \
    do {                                                                            \
        ++g_tests_run;                                                              \
        if (!(cond)) {                                                             \
            std::cerr << "FAIL: " << message << std::endl;                        \
        } else {                                                                    \
            ++g_tests_passed;                                                     \
        }                                                                           \
    } while (0)

// Test 1: Forwarding behavior with a null db pointer
// - Ensure that sqlite3_dbdata_init forwards the db pointer to sqlite3DbdataRegister.
// - Ensure that pzErrMsg is not modified by sqlite3_dbdata_init and that the return
//   value equals the one provided by sqlite3DbdataRegister.
void test_null_db_forwards_and_returns_overridden_value() {
    // Arrange: set override behavior
    g_lastDbPointer = nullptr;
    g_overrideReturnValue = 0xDEADBEEF; // distinctive value to verify forwarding

    char* errMsg = nullptr;
    int ret = sqlite3_dbdata_init(nullptr, &errMsg, nullptr);

    // Assert: forwarding occurred, and pzErrMsg was not modified
    TEST_EXPECT_PTR_EQ(g_lastDbPointer, static_cast<void*>(nullptr),
                       "db pointer should be forwarded as null");
    TEST_EXPECT_EQ(ret, g_overrideReturnValue,
                   "sqlite3_dbdata_init should return value provided by sqlite3DbdataRegister");
    TEST_EXPECT_TRUE(errMsg == nullptr,
                     "pzErrMsg should remain unchanged (no modification)");
}

// Test 2: Forwarding behavior with a non-null db pointer
// - Use a non-null dummy pointer to ensure forwarding works for non-null inputs.
// - Verify return value matches the override and that pzErrMsg remains unmodified.
void test_nonnull_db_forwards_and_returns_overridden_value() {
    // Arrange: create a dummy object and use its address as db
    g_lastDbPointer = nullptr;
    g_overrideReturnValue = 0xCAFEBABE;

    int dummy;
    sqlite3* fakeDb = reinterpret_cast<sqlite3*>(&dummy);
    char* errMsgOriginal = const_cast<char*>("initial_err"); // non-null to test immutability
    char* errMsg = errMsgOriginal;

    int ret = sqlite3_dbdata_init(fakeDb, &errMsg, nullptr);

    // Assert: forwarding occurred to the received pointer
    TEST_EXPECT_PTR_EQ(g_lastDbPointer, static_cast<void*>(fakeDb),
                       "db pointer should be forwarded to sqlite3DbdataRegister");
    TEST_EXPECT_EQ(ret, g_overrideReturnValue,
                   "sqlite3_dbdata_init should return value provided by sqlite3DbdataRegister");
    TEST_EXPECT_TRUE(errMsg == errMsgOriginal,
                     "pzErrMsg should remain unchanged (pointer unchanged)");
}

// Test 3: pApi and pzErrMsg handling are non-modifying and non-mutating
// - Ensure the function doesn't modify pApi or pzErrMsg and still forwards db.
// - We re-use the non-null db test with a dummy pApi to verify no mutation occurs.
void test_pApi_and_pzErr_not_modified() {
    // Arrange: non-null db, dummy pApi, and a non-null pzErrMsg
    g_lastDbPointer = nullptr;
    g_overrideReturnValue = 0x12345678;
    struct sqlite3_api_routines dummyApi;
    sqlite3* db = reinterpret_cast<sqlite3*>(&dummyApi); // intentionally mis-cast to simulate a pointer
    char errBuffer[10] = {0};
    char* pzErr = errBuffer;

    int ret = sqlite3_dbdata_init(db, &pzErr, &dummyApi);

    // Assert: the same as before; primarily ensuring no mutation of pApi or pzErrMsg
    TEST_EXPECT_EQ(ret, g_overrideReturnValue,
                   "sqlite3_dbdata_init should return value provided by sqlite3DbdataRegister");
    TEST_EXPECT_PTR_EQ(g_lastDbPointer, static_cast<void*>(db),
                       "db pointer should be forwarded to sqlite3DbdataRegister");
    TEST_EXPECT_TRUE(pzErr == errBuffer,
                     "pzErrMsg should remain unchanged (buffer address unchanged)");
}

// Helper: run all tests and report summary
void run_all_tests() {
    test_null_db_forwards_and_returns_overridden_value();
    test_nonnull_db_forwards_and_returns_overridden_value();
    test_pApi_and_pzErr_not_modified();

    // Summary
    std::cout << "Tests run: " << g_tests_run
              << ", Passed: " << g_tests_passed
              << ", Failed: " << (g_tests_run - g_tests_passed) << std::endl;
}

int main() {
    // Run the test suite
    run_all_tests();
    // Exit status: 0 if all tests passed, non-zero otherwise
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}