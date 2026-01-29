// C++11 unit tests for sqlite3_db_dump (focal method) without using GTest.
// This test suite exercises two primary code paths of sqlite3_db_dump:
// 1) Dumping all tables in a schema (zTable == NULL)
// 2) Dumping a single specified table (zTable != NULL)
// The tests rely on the real sqlite3_db_dump function (from dbdump.c) and
// the sqlite3 C API. Output from sqlite3_db_dump is captured via a callback.

#include <string.h>
#include <ctype.h>
#include <vector>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <iostream>


// Forward declaration of the focal function as declared in the project.
// The function is implemented in dbdump.c; we assume it will be linked with the test.
extern "C" int sqlite3_db_dump(
    sqlite3 *db,
    const char *zSchema,
    const char *zTable,
    int (*xCallback)(const char*, void*),
    void *pArg
);

// Lightweight output collector used as the xCallback for sqlite3_db_dump.
struct OutputCollector {
    std::string out;
    // Static callback compatible with sqlite3_db_dump signature.
    static int Callback(const char* z, void* pArg) {
        OutputCollector* self = static_cast<OutputCollector*>(pArg);
        if (z) self->out.append(z);
        return 0;
    }
};

// Simple test harness helpers
static int failures = 0;

static void log_failure(const std::string& msg) {
    std::cerr << "TEST FAILURE: " << msg << std::endl;
    ++failures;
}

// Test 1: Dump all tables in an in-memory database.
// Goals:
// - Ensure basic path (zTable == NULL) executes and returns SQLITE_OK.
// - Ensure PRAGMA header and some CREATE TABLE statements appear in output.
static bool test_dump_all_tables_basic() {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        log_failure("Failed to open in-memory database for test_dump_all_tables_basic.");
        if (db) sqlite3_close(db);
        return false;
    }

    // Create a couple of tables to exercise schema_dump logic.
    const char* ddl =
        "CREATE TABLE t1 (id INTEGER PRIMARY KEY, name TEXT);"
        "CREATE TABLE t2 (x INTEGER, y TEXT);";
    rc = sqlite3_exec(db, ddl, 0, 0, 0);
    if (rc != SQLITE_OK) {
        log_failure("Failed to create test tables for test_dump_all_tables_basic.");
        sqlite3_close(db);
        return false;
    }

    OutputCollector collector;
    rc = sqlite3_db_dump(db, "main", 0, OutputCollector::Callback, &collector);
    sqlite3_close(db);

    bool ok = (rc == SQLITE_OK);
    // Basic content checks
    bool hasBegin = collector.out.find("BEGIN TRANSACTION") != std::string::npos;
    bool hasCreateTable = collector.out.find("CREATE TABLE") != std::string::npos;
    if (!ok) log_failure("sqlite3_db_dump did not return SQLITE_OK in test_dump_all_tables_basic.");
    if (!hasBegin) log_failure("OUTPUT missing BEGIN TRANSACTION in test_dump_all_tables_basic.");
    if (!hasCreateTable) log_failure("OUTPUT missing CREATE TABLE statements in test_dump_all_tables_basic.");

    return ok && hasBegin && hasCreateTable;
}

// Test 2: Dump a single specific table (zTable != NULL).
// Goals:
// - Ensure single-table path executes and returns SQLITE_OK.
// - Ensure the output includes the CREATE TABLE statement for the specified table.
static bool test_dump_single_table_specific() {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        log_failure("Failed to open in-memory database for test_dump_single_table_specific.");
        if (db) sqlite3_close(db);
        return false;
    }

    // Create two tables; then dump only one of them.
    const char* ddl =
        "CREATE TABLE t1 (id INTEGER PRIMARY KEY, name TEXT);"
        "CREATE TABLE t2 (a INTEGER);";
    rc = sqlite3_exec(db, ddl, 0, 0, 0);
    if (rc != SQLITE_OK) {
        log_failure("Failed to create test tables for test_dump_single_table_specific.");
        sqlite3_close(db);
        return false;
    }

    OutputCollector collector;
    rc = sqlite3_db_dump(db, "main", "t1", OutputCollector::Callback, &collector);
    sqlite3_close(db);

    bool ok = (rc == SQLITE_OK);
    bool hasSingleCreateTable = collector.out.find("CREATE TABLE") != std::string::npos;
    // Since we dump a single table 't1', we should see a CREATE TABLE statement for t1.
    bool containsT1 = collector.out.find("t1") != std::string::npos;

    if (!ok) log_failure("sqlite3_db_dump did not return SQLITE_OK in test_dump_single_table_specific.");
    if (!hasSingleCreateTable) log_failure("OUTPUT missing CREATE TABLE statements in test_dump_single_table_specific.");
    if (!containsT1) log_failure("OUTPUT missing table name 't1' in test_dump_single_table_specific.");

    return ok && hasSingleCreateTable && containsT1;
}

// Entry point for running tests
int main() {
    std::vector<std::string> results;
    bool t1 = test_dump_all_tables_basic();
    results.push_back(std::string("test_dump_all_tables_basic: ") + (t1 ? "PASS" : "FAIL"));

    bool t2 = test_dump_single_table_specific();
    results.push_back(std::string("test_dump_single_table_specific: ") + (t2 ? "PASS" : "FAIL"));

    // Print results
    std::cout << " sqlite3_db_dump test suite results:\n";
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "  " << i+1 << ". " << results[i] << "\n";
    }

    if (failures > 0) {
        std::cerr << "Total failures: " << failures << "\n";
        return 1;
    }
    return 0;
}