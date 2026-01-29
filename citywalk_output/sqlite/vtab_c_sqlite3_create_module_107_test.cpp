// sqlite3_create_module_unit_tests.cpp
// A small, self-contained C++11 test suite for the focal function
// sqlite3_create_module (as used in vtab.c of SQLite).
// Note: This test uses the real SQLite headers and runtime. It is designed
// to be compiled in a project that already builds SQLite components and links
// against the SQLite library. No GTest is used; tests are run from main().
// The tests focus on true branch coverage for the API-armor precondition and
// a minimal positive registration scenario.

#include <vector>
#include <sqlite3.h>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// We declare C linkage for helper module callbacks to avoid C++ name mangling issues.
extern "C" {

// Minimal xCreate callback: always succeed for test purposes
int test_module_xCreate(sqlite3* db, void* pAux, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVtab; (void)pzErr;
    return SQLITE_OK;
}

// Minimal xConnect callback: always succeed for test purposes
int test_module_xConnect(sqlite3* db, void* pAux, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVtab; (void)pzErr;
    return SQLITE_OK;
}
} // extern "C"

// Helper to initialize a sqlite3_module with minimal valid callbacks
static void initMinimalTestModule(sqlite3_module &mod) {
    mod.iVersion = 1;
    mod.xCreate = test_module_xCreate;
    mod.xConnect = test_module_xConnect;

    // The rest of the function pointers are set to NULL for a minimal, non-activating module.
    // Real-world modules would provide meaningful implementations for these.
    mod.xBestIndex = nullptr;
    mod.xDisconnect = nullptr;
    mod.xDestroy = nullptr;
    mod.xOpen = nullptr;
    mod.xClose = nullptr;
    mod.xFilter = nullptr;
    mod.xNext = nullptr;
    mod.xEof = nullptr;
    mod.xColumn = nullptr;
    mod.xRowid = nullptr;
    mod.xUpdate = nullptr;
    mod.xBegin = nullptr;
    mod.xSync = nullptr;
    mod.xCommit = nullptr;
    mod.xRollback = nullptr;
    mod.xFindFunction = nullptr;
    mod.xRename = nullptr;
}

// Test 1: Ensure that with a valid in-memory database and a NULL module name, the API-armored
// path returns SQLITE_MISUSE_BKPT. This exercises the short-circuit path in sqlite3_create_module.
bool test_sqlite3_create_module_null_name_returns_misuse() {
    sqlite3* db = nullptr;
    int rc = SQLITE_OK;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK || db == nullptr) {
        std::cerr << "Test 1: Failed to open in-memory database.\n";
        if (db) sqlite3_close(db);
        return false;
    }

    sqlite3_module mod;
    initMinimalTestModule(mod);

    // Call with NULL zName to trigger MISUSE_BKPT when API armor is enabled.
    rc = sqlite3_create_module(db, nullptr, &mod, nullptr);

    sqlite3_close(db);

    bool passed = (rc == SQLITE_MISUSE_BKPT);
    if (!passed) {
        std::cerr << "Test 1: Expected SQLITE_MISUSE_BKPT, got " << rc << "\n";
    }
    return passed;
}

// Test 2: Ensure that a valid in-memory database can register a simple module.
// This exercises the successful path where sqlite3_create_module delegates to createModule.
// We don't inspect internal state, only that the call returns SQLITE_OK (indicative of success).
bool test_sqlite3_create_module_valid_registration() {
    sqlite3* db = nullptr;
    int rc = SQLITE_OK;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK || db == nullptr) {
        std::cerr << "Test 2: Failed to open in-memory database.\n";
        if (db) sqlite3_close(db);
        return false;
    }

    sqlite3_module mod;
    initMinimalTestModule(mod);

    rc = sqlite3_create_module(db, "testmodule", &mod, nullptr);

    sqlite3_close(db);

    bool passed = (rc == SQLITE_OK);
    if (!passed) {
        std::cerr << "Test 2: Expected SQLITE_OK for valid module registration, got " << rc << "\n";
    }
    return passed;
}

// Simple test harness to run defined tests and report results.
int main() {
    using namespace std;

    struct TestCase {
        string name;
        bool (*func)();
    };

    vector<TestCase> tests = {
        {"test_sqlite3_create_module_null_name_returns_misuse", test_sqlite3_create_module_null_name_returns_misuse},
        {"test_sqlite3_create_module_valid_registration", test_sqlite3_create_module_valid_registration}
    };

    int passed = 0;
    int total = (int)tests.size();

    cout << "sqlite3_create_module unit test suite\n";
    cout << "-----------------------------------\n";

    for (const auto& t : tests) {
        bool ok = false;
        try {
            ok = t.func();
        } catch (const std::exception& e) {
            cout << "[FAILED] " << t.name << " - Exception: " << e.what() << "\n";
            ok = false;
        } catch (...) {
            cout << "[FAILED] " << t.name << " - Unknown exception\n";
            ok = false;
        }
        cout << (ok ? "[PASSED] " : "[FAILED] ") << t.name << "\n";
        if (ok) ++passed;
    }

    cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}