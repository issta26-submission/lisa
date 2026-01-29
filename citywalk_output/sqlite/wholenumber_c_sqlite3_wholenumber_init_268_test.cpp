// Unit test suite for the focal method: sqlite3_wholenumber_init
// This test suite is written in C++11 and relies on the system's SQLite3 library.
// It exercises the initialization path of the wholenumber virtual table extension by
// loading the module into an in-memory database and verifying that the module is registered.
// The test does not use GTest; it uses a simple custom harness and plain asserts with
// explanatory comments for each test case.

// Step 1 (Conceptual): Candidate Keywords extracted from the focal method and its dependencies
// - sqlite3: core database handle (db).
// - sqlite3_api_routines: API table pointer passed to extensions (pApi).
// - sqlite3_extension_init: macro family (SQLITE_EXTENSION_INIT1/SQLITE_EXTENSION_INIT2).
// - sqlite3_create_module: function used to register the module with the database.
// - wholenumberModule: the module instance registered for name "wholenumber".
// - SQLITE_OK, SQLITE_OMIT_VIRTUALTABLE: control flow for virtual-table related code paths.
// - PRAGMA module_list: SQLite pragma to verify loaded modules.
// - in-memory database: sqlite3_open(":memory:", &db).
// These keywords map to the dependencies and runtime behavior we test.

// Step 2: Test suite (no GTest). Compile with: g++ -std=c++11 -lsqlite3 <test.cpp> wholenumber.c
// Note: We rely on the system's sqlite3 library for module registration and PRAGMA inspection.

#include <string.h>
#include <assert.h>
#include <sqlite3ext.h>
#include <cerrno>
#include <sqlite3.h>
#include <cstring>
#include <iostream>


// Forward declaration of the focal function (defined in wholenumber.c)
extern "C" int sqlite3_wholenumber_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

// The SQLite extension defines a module named "wholenumber" and uses a static module object
// inside wholenumber.c. We assume wholenumber.c is compiled/linked together with this test
// so that sqlite3_wholenumber_init will attempt to create the module via sqlite3_create_module.

// Simple test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Test 1: Ensure that sqlite3_wholenumber_init loads the module and registers it
// The test creates an in-memory database, calls the init function, and then queries
// the loaded modules via PRAGMA module_list to verify that "wholenumber" is present.
bool test_wholenumber_init_loads_module() {
    std::cout << "[Test] test_wholenumber_init_loads_module: start\n";

    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        std::cerr << "Failed to open in-memory database: " << sqlite3_errmsg(db) << "\n";
        if (db) sqlite3_close(db);
        return false;
    }

    // Provide a zero-initialized dummy API table. The focal function uses SQLITE_EXTENSION_INIT2(pApi)
    // but for this test, the API surface is not needed for the module registration to succeed.
    sqlite3_api_routines api_dummy;
    std::memset(&api_dummy, 0, sizeof(api_dummy));

    char* pErrMsg = nullptr;
    rc = sqlite3_wholenumber_init(db, &pErrMsg, &api_dummy);
    if (rc != SQLITE_OK) {
        std::cerr << "sqlite3_wholenumber_init failed with rc=" << rc
                  << " and error msg: " << (pErrMsg ? pErrMsg : "nullptr") << "\n";
        sqlite3_close(db);
        return false;
    }

    // Verify that the module "wholenumber" appears in the module list.
    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "PRAGMA module_list;", -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare PRAGMA module_list: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }

    bool found = false;
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // The first column of PRAGMA module_list is typically the module name.
        const unsigned char* name = sqlite3_column_text(stmt, 0);
        if (name != nullptr) {
            const char* name_cstr = reinterpret_cast<const char*>(name);
            // Look for the wholenumber module by name
            if (std::strcmp(name_cstr, "wholenumber") == 0) {
                found = true;
                break;
            }
        }
    }
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        std::cerr << "Error iterating PRAGMA module_list: " << sqlite3_errmsg(db) << "\n";
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }
    sqlite3_finalize(stmt);

    if (!found) {
        std::cerr << "Module 'wholenumber' not found in module_list after init.\n";
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    std::cout << "[Test] test_wholenumber_init_loads_module: pass\n";
    return true;
}

// Test 2: Ensure that sqlite3_wholenumber_init works when a null pApi is provided.
// This validates that the function handles a null API pointer gracefully (no crash, returns SQLITE_OK).
bool test_wholenumber_init_with_null_api() {
    std::cout << "[Test] test_wholenumber_init_with_null_api: start\n";

    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        std::cerr << "Failed to open in-memory database: " << sqlite3_errmsg(db) << "\n";
        if (db) sqlite3_close(db);
        return false;
    }

    // Do not provide a valid API table (null pointer for pApi) to exercise the null path.
    char* pErrMsg = nullptr;
    rc = sqlite3_wholenumber_init(db, &pErrMsg, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "sqlite3_wholenumber_init with null pApi failed with rc=" << rc
                  << " and error msg: " << (pErrMsg ? pErrMsg : "nullptr") << "\n";
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    std::cout << "[Test] test_wholenumber_init_with_null_api: pass\n";
    return true;
}

// Step 3: Test case refinement and execution entrypoint
// We provide a minimal harness that executes the tests and reports summary.
// The tests rely only on the standard library and sqlite3; they do not mock private
// methods or rely on private state, aligning with the constraints provided.

int main() {
    std::cout << "Starting sqlite3_wholenumber_init unit tests (C++11)...\n";

    bool t1 = test_wholenumber_init_loads_module();
    bool t2 = test_wholenumber_init_with_null_api();

    int total = 0, passed = 0, failed = 0;
    // Manual accounting based on test results
    total = 2;
    passed = (t1 ? 1 : 0) + (t2 ? 1 : 0);
    failed = total - passed;

    std::cout << "Tests completed. Total: " << total
              << ", Passed: " << passed
              << ", Failed: " << failed << "\n";

    if (failed > 0) {
        std::cerr << "Some tests failed. See above messages for details.\n";
        return 1;
    }
    return 0;
}