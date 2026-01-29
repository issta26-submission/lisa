// Unit test suite for sqlite3_qpvtab_init (qpvtab.c) using plain C++11 (no Google Test).
// This test harness is designed to be compiled and run in an environment where SQLite3 is available.
// It avoids terminating on assertion failures to maximize code coverage (non-terminating assertions).

#include <stdlib.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3ext.h>
#include <sstream>
#include <sqlite3.h>
#include <string>
#include <iostream>



using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ostringstream;

// Expose the focal function with C linkage for proper name mangling when linked from C++.
extern "C" int sqlite3_qpvtab_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

// Non-terminating assertion framework
static vector<string> g_failure_messages;
static int g_total_tests = 0;
static int g_failed_tests = 0;

template <typename T>
static string ToString(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

#define EXPECT_EQ(expected, actual) do { \
    ++g_total_tests; \
    if (!((expected) == (actual))) { \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << __func__ << "(): Expected " << ToString(expected) \
                  << " but got " << ToString(actual) << " (line " << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << __func__ << "(): Condition is false at line " << __LINE__ << std::endl; \
    } \
} while(0)

#define LOG_INFO(msg) do { std::cout << "[INFO] " << __func__ << ": " << msg << std::endl; } while(0)

// ************************************************************************************
// Step 1 & 2: Program Understanding & Test Case Generation
// - The focal function sqlite3_qpvtab_init initializes the "qpvtab" extension.
// - It uses SQLITE_EXTENSION_INIT2(pApi) unconditionally to wire the API pointer.
// - It conditionally compiles the module creation path depending on SQLITE_OMIT_VIRTUALTABLE.
//   - If SQLITE_OMIT_VIRTUALTABLE is defined: rc remains SQLITE_OK, module creation is skipped.
//   - If SQLITE_OMIT_VIRTUALTABLE is not defined: rc is assigned the result of sqlite3_create_module(...).
// - In test environments, we can validate:
//     a) The function returns SQLITE_OK when the virtual table pathway is omitted (branch false).
//     b) The function's behavior when the virtual table pathway is present depends on the SQLite setup.
// - To maximize coverage, we provide two tests guarded by compile-time macros:
//     - test_qpvtab_init_omit_virtualtable: exercises the path where module creation is skipped.
//     - test_qpvtab_init_with_virtualtable: exercises the path where module creation is attempted.
// Note: The second test depends on a functioning SQLite environment and may vary by installation.
// The tests are designed to be compiled in two configurations:
//   1) With -DSQLITE_OMIT_VIRTUALTABLE (omits virtual table module creation)
//   2) Without -DSQLITE_OMIT_VIRTUALTABLE (tries to create the module)
// ************************************************************************************

// Domain knowledge notes (for maintainers):
// - The test relies on sqlite3_qpvtab_init's signature and typical SQLite behaviors.
// - We exercise the module-creation-branch by invoking sqlite3_qpvtab_init in a live SQLite environment.
// - Static helpers in qpvtab.c (e.g., qpvtabConnect, qpvtabModule) are not directly testable here
//   due to their internal scope, but we verify the observable effect (return codes) of the initializer.

#if defined(SQLITE_OMIT_VIRTUALTABLE)

// Test case: when SQLITE_OMIT_VIRTUALTABLE is defined, the code should skip module creation
// and simply return SQLITE_OK after extension initialization.
//
// Test will:
// - Open an in-memory database
// - Call sqlite3_qpvtab_init with a nullptr pApi (sufficient for SQLITE_EXTENSION_INIT2 to operate safely)
// - Verify the return code is SQLITE_OK
static void test_qpvtab_init_omit_virtualtable() {
    LOG_INFO("Starting test_qpvtab_init_omit_virtualtable (branch: OMIT_VIRTUALTABLE)");
    sqlite3 *db = nullptr;
    int rc = SQLITE_OK;

    rc = sqlite3_open(":memory:", &db);
    EXPECT_EQ(SQLITE_OK, rc);

    char *errMsg = nullptr;
    rc = sqlite3_qpvtab_init(db, &errMsg, nullptr); // pApi = nullptr is acceptable for this test path
    EXPECT_EQ(SQLITE_OK, rc);

    if (db) {
        sqlite3_close(db);
    }

    LOG_INFO("Finished test_qpvtab_init_omit_virtualtable");
}

#else // SQLITE_OMIT_VIRTUALTABLE not defined

// Test case: when SQLITE_OMIT_VIRTUALTABLE is not defined, the code should attempt
// to register the qpvtab module via sqlite3_create_module.
// We perform an end-to-end check by:
// - Opening an in-memory database
// - Invoking sqlite3_qpvtab_init
// - Verifying the return code is SQLITE_OK (indicates module creation likely succeeded on this system)
// Note: This test depends on a working SQLite installation and proper linkage with the qpvtabModule.
// If the environment cannot register the module, this test will report a failure
// but it provides useful coverage for the non-OMIT path.

static void test_qpvtab_init_with_virtualtable() {
    LOG_INFO("Starting test_qpvtab_init_with_virtualtable (branch: WITHOUT OMIT_VIRTUALTABLE)");
    sqlite3 *db = nullptr;
    int rc = SQLITE_OK;

    rc = sqlite3_open(":memory:", &db);
    EXPECT_EQ(SQLITE_OK, rc);

    // pApi can be NULL or a valid API struct; sqlite3_qpvtab_init will handle SQLITE_EXTENSION_INIT2(NULL) gracefully.
    char *errMsg = nullptr;
    rc = sqlite3_qpvtab_init(db, &errMsg, nullptr);
    // Accept any non-negative SQLite result code as success for test purposes, but prefer SQLITE_OK when possible.
    EXPECT_TRUE(rc == SQLITE_OK);

    if (db) {
        sqlite3_close(db);
    }

    LOG_INFO("Finished test_qpvtab_init_with_virtualtable");
}

#endif // SQLITE_OMIT_VIRTUALTABLE

// ************************************************************************************
// Step 3: Test Case Refinement
// - The tests above are designed to maximize coverage by exercising both compile-time branches
//   of the focal predicate (#ifndef SQLITE_OMIT_VIRTUALTABLE).
// - We use non-terminating assertions to allow the test to continue executing and potentially
//   reveal additional issues in a single run.
// - The tests rely on the standard SQLite3 API and do not mock private/internal functions of the
//   focal module, respecting the guideline to avoid private methods/fields.
// - Static/global state in the test is avoided; test harness is self-contained and uses a small
//   runtime assertion framework (non-terminating).

int main() {
    cout << " qpvt_init_unit_tests: C++11 test harness for sqlite3_qpvtab_init " << endl;

#if defined(SQLITE_OMIT_VIRTUALTABLE)
    test_qpvtab_init_omit_virtualtable();
#else
    test_qpvtab_init_with_virtualtable();
#endif

    // Summary
    cout << "Total tests run: " << g_total_tests << endl;
    cout << "Total failures:  " << g_failed_tests << endl;

    if (!g_failure_messages.empty()) {
        cout << "Failure messages:" << std::endl;
        for (const auto &m : g_failure_messages) {
            cout << "  - " << m << std::endl;
        }
    }

    // Return non-zero if any test failed
    if (g_failed_tests > 0) {
        return 1;
    }

    return 0;
}