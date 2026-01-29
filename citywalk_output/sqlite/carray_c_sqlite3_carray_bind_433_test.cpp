/*
Unit test suite for sqlite3_carray_bind (focal method) using a lightweight in-process harness.
- No GTest; a minimal main()-driven test framework is implemented.
- Tests are designed to be executable with C++11 and rely on the production code behavior.
- The test suite uses the internal constants (CARRAY_*) by including the focal class file (carray.c) to ensure macro values are in sync.
- Coverage includes true branches of some key predicates and safe-fail paths (e.g., TEXT, BLOB, INT32, and default/misc paths).
- The tests are designed to be non-terminating in assertions (i.e., the harness reports failures but continues execution where reasonable).
- All tests are self-contained in this single file and are invoked from main().
- Explanatory comments accompany each unit test describing which path/branch is exercised.

Note: This harness assumes that the environment provides SQLite3 headers and library and that carray.c is compilable in the same translation unit. If your build system uses a separate library for the extension, ensure the extension is compiled and linked appropriately.
*/

#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3ext.h>
#include <carray.c>
#include <sqlite3.h>
#include <cstring>
#include <iostream>
#include <cassert>


// Define to compile as C++11; ensure linkage with SQLite3 and internal carray.c
// The focal method and required macros (CARRAY_TEXT, CARRAY_BLOB, CARRAY_INT32, etc.)
// will be provided by including the focal class file (carray.c) in this translation unit.
extern "C" {
}

// Include the focal class file to expose the internal macros and implementation of sqlite3_carray_bind.
// This mirrors Step 1/Step 2 intent to align with the exact dependent components.
// If your build system handles extensions differently, adjust accordingly.

using std::cout;
using std::endl;

// Simple test harness helpers
static void test_report(const char* test_name, bool passed) {
    if (passed) {
        cout << "[PASS] " << test_name << "\n";
    } else {
        cout << "[FAIL] " << test_name << "\n";
    }
}

// Helper to create an in-memory SQLite database and a single parameter statement
static bool prepare_single_param_stmt(sqlite3** pDb, sqlite3_stmt** pStmt, const char* sql) {
    int rc = sqlite3_open(":memory:", pDb);
    if (rc != SQLITE_OK) {
        cout << "Failed to open in-memory DB: " << sqlite3_errmsg(*pDb) << "\n";
        return false;
    }
    rc = sqlite3_prepare_v2(*pDb, sql, -1, pStmt, NULL);
    if (rc != SQLITE_OK) {
        cout << "Failed to prepare statement: " << sqlite3_errmsg(*pDb) << "\n";
        sqlite3_close(*pDb);
        return false;
    }
    return true;
}

// Test 1: Bind a TEXT array using mFlags = CARRAY_TEXT and SQLITE_STATIC destructor
// This exercises the TEXT path under the TRANSIENT-unsafe branch (via range of strings).
static bool test_bind_text_array() {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    if (!prepare_single_param_stmt(&db, &stmt, "SELECT ?")) {
        return false;
    }

    // Build TEXT array: const char*[]
    const char* s0 = "alpha";
    const char* s1 = "beta";
    const char* aData[] = { s0, s1 };
    int nData = 2;
    int mFlags = CARRAY_TEXT; // Expect TEXT-path branch

    int rc = sqlite3_carray_bind(stmt, 1, (void*)aData, nData, mFlags, SQLITE_STATIC);
    bool ok = (rc == SQLITE_OK);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

// Test 2: Bind a BLOB array using mFlags = CARRAY_BLOB and SQLITE_TRANSIENT destructor
// This exercises the BLOB path, which allocates and copies blob content into internal buffers.
static bool test_bind_blob_array() {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    if (!prepare_single_param_stmt(&db, &stmt, "SELECT ?")) {
        return false;
    }

    // Build BLOB array using struct iovec
    struct iovec aData[2];
    unsigned char b0[] = {0x01, 0x02, 0x03};
    unsigned char b1[] = {0xAA, 0xBB, 0xCC, 0xDD};
    aData[0].iov_base = b0;
    aData[0].iov_len = sizeof(b0);
    aData[1].iov_base = b1;
    aData[1].iov_len = sizeof(b1);

    int nData = 2;
    int mFlags = CARRAY_BLOB; // Expect BLOB-path branch

    int rc = sqlite3_carray_bind(stmt, 1, (void*)aData, nData, mFlags, SQLITE_TRANSIENT);
    bool ok = (rc == SQLITE_OK);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

// Test 3: Bind an INT32 array using mFlags = CARRAY_INT32 and SQLITE_TRANSIENT destructor
// This exercises the numeric path; ensures non-pointer data can be bound and copied.
static bool test_bind_int32_array() {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    if (!prepare_single_param_stmt(&db, &stmt, "SELECT ?")) {
        return false;
    }

    int data[3] = { 10, 20, 30 };
    int nData = 3;
    int mFlags = CARRAY_INT32; // Expect 32-bit int path
    int rc = sqlite3_carray_bind(stmt, 1, (void*)data, nData, mFlags, SQLITE_TRANSIENT);
    bool ok = (rc == SQLITE_OK);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

// Test 4: Bind using default/misc flags (mFlags = 0) to ensure fallback path is exercised
// This checks that the function handles non-matching mFlags gracefully and returns SQLITE_OK.
static bool test_bind_default_flags() {
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;

    if (!prepare_single_param_stmt(&db, &stmt, "SELECT ?")) {
        return false;
    }

    // Use arbitrary int data; since mFlags=0, the code should follow the default copying path
    int data[2] = { 7, 11 };
    int nData = 2;
    int mFlags = 0; // Not a recognized CARRAY_* value; tests a default branch
    int rc = sqlite3_carray_bind(stmt, 1, (void*)data, nData, mFlags, SQLITE_TRANSIENT);
    bool ok = (rc == SQLITE_OK);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

// Entry point: Run all tests and report results
int main() {
    cout << "Starting sqlite3_carray_bind unit tests (C++11, no GTest)" << endl;

    int pass_count = 0;
    int fail_count = 0;

    if (test_bind_text_array()) {
        ++pass_count;
    } else {
        ++fail_count;
    }
    test_report("test_bind_text_array (TEXT path)", test_bind_text_array());

    if (test_bind_blob_array()) {
        ++pass_count;
    } else {
        ++fail_count;
    }
    test_report("test_bind_blob_array (BLOB path)", test_bind_blob_array());

    if (test_bind_int32_array()) {
        ++pass_count;
    } else {
        ++fail_count;
    }
    test_report("test_bind_int32_array (INT32 path)", test_bind_int32_array());

    if (test_bind_default_flags()) {
        ++pass_count;
    } else {
        ++fail_count;
    }
    test_report("test_bind_default_flags (fallback path)", test_bind_default_flags());

    cout << "Tests completed: " << pass_count << " passed, " << fail_count << " failed." << endl;

    // Return non-zero if any tests failed
    return fail_count > 0 ? 1 : 0;
}