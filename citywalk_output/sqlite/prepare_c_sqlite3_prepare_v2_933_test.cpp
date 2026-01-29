// Test-suite for sqlite3_prepare_v2 (C API) using a C++11 test harness (no GTest).
// This suite targets the focal function as described, relying on a live in-memory SQLite database.
// It exercises valid paths, error paths, and tail/statement handling to maximize coverage.
// NOTE: This test assumes that the project is built with SQLite's sqlite3 library available
// (e.g., -lsqlite3) and that linking against the system's SQLite library is feasible.

#include <sqlite3.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>



// Lightweight test harness: reports per-test PASS/FAIL without terminating on failure.
static void print_test_result(const std::string &name, bool pass) {
    std::cout << "[TEST " << (pass ? "PASS" : "FAIL") << "] " << name << "\n";
}

// Test 1: Valid single-statement SQL should succeed and produce a non-null prepared statement.
// This validates the happy-path of sqlite3_prepare_v2 when provided with a simple CREATE statement.
bool testPrepareV2_ValidSingleStatement() {
    bool pass = true;
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;

    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        print_test_result("testPrepareV2_ValidSingleStatement - open in-memory db", false);
        return false;
    }

    // A simple valid SQL statement: should produce SQLITE_OK and a non-null prepared statement.
    int rc = sqlite3_prepare_v2(db, "CREATE TABLE t(a);", -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        pass = false;
    }
    if (stmt == nullptr) {
        pass = false;
    }
    if (tail == nullptr) {
        // tail may be non-null in practice; requiring non-null helps ensure the API contract
        // provides an end-pointer for the parsed portion.
        pass = pass && false;
    }

    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);

    print_test_result("testPrepareV2_ValidSingleStatement", pass);
    return pass;
}

// Test 2: Multiple statements in the input; ensure tail points to the start of the second statement.
// This checks handling of zSql containing more than one SQL statement and verifies tail content.
bool testPrepareV2_MultipleStatementsTail() {
    bool pass = true;
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;

    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        print_test_result("testPrepareV2_MultipleStatementsTail - open in-memory db", false);
        return false;
    }

    int rc = sqlite3_prepare_v2(db, "SELECT 1; SELECT 2;", -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        pass = false;
    }
    if (stmt == nullptr) {
        pass = false;
    }
    if (tail == nullptr) {
        pass = false;
    } else {
        // Verify that the tail contains the second statement, e.g., " SELECT 2;"
        std::string tailStr(tail);
        if (tailStr.find("SELECT 2") == std::string::npos) {
            pass = false;
        }
    }

    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);

    print_test_result("testPrepareV2_MultipleStatementsTail", pass);
    return pass;
}

// Test 3: Invalid SQL should return a non-OK code and not produce a usable statement.
// We expect the prepared statement to be NULL or invalid in this error scenario.
bool testPrepareV2_InvalidSQL() {
    bool pass = true;
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;

    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        print_test_result("testPrepareV2_InvalidSQL - open in-memory db", false);
        return false;
    }

    int rc = sqlite3_prepare_v2(db, "SELEC T 1;", -1, &stmt, &tail);
    // rc should indicate error since SQL is invalid.
    if (rc == SQLITE_OK) {
        // If unexpectedly OK, this is a failure for this test path.
        pass = false;
        if (stmt) sqlite3_finalize(stmt);
    } else {
        // On error, ideally no statement is produced.
        if (stmt != nullptr) {
            sqlite3_finalize(stmt);
            // Some implementations may still return a non-null stmt on certain errors;
            // treat this as a failure unless it's clearly NULL by contract.
            pass = false;
        }
        // tail may remain NULL or unchanged on error; we won't depend on it here.
    }

    if (db) sqlite3_close(db);

    print_test_result("testPrepareV2_InvalidSQL", pass);
    return pass;
}

// Test 4: Invalid nBytes value simulates partial input; ensure RC indicates error and no statement is produced.
// This tests the nBytes constraint branch to ensure we don't accidentally parse partial statements.
bool testPrepareV2_InvalidNBytes() {
    bool pass = true;
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;

    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        print_test_result("testPrepareV2_InvalidNBytes - open in-memory db", false);
        return false;
    }

    // Provide nBytes smaller than the actual string length
    int rc = sqlite3_prepare_v2(db, "SELECT 1;", 2, &stmt, &tail);
    if (rc == SQLITE_OK) {
        // Expecting failure due to insufficient bytes for a valid statement
        pass = false;
    }
    if (stmt != nullptr) {
        sqlite3_finalize(stmt);
        pass = false;
    }

    if (db) sqlite3_close(db);

    print_test_result("testPrepareV2_InvalidNBytes", pass);
    return pass;
}

// Test 5: Single statement with no tail (i.e., the entire zSql is consumed by the first statement).
// The tail should either be an empty string or point to the null terminator.
bool testPrepareV2_SingleStatementNoTail() {
    bool pass = true;
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;

    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        print_test_result("testPrepareV2_SingleStatementNoTail - open in-memory db", false);
        return false;
    }

    int rc = sqlite3_prepare_v2(db, "SELECT 1", -1, &stmt, &tail);
    if (rc != SQLITE_OK) {
        pass = false;
    }
    if (stmt == nullptr) {
        pass = false;
    }
    if (tail == nullptr) {
        pass = false;
    } else {
        // tail should be empty (no trailing content)
        if (std::strlen(tail) != 0) {
            pass = false;
        }
    }

    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);

    print_test_result("testPrepareV2_SingleStatementNoTail", pass);
    return pass;
}

// Entry point: run all tests and report overall result.
int main() {
    bool t1 = testPrepareV2_ValidSingleStatement();
    bool t2 = testPrepareV2_MultipleStatementsTail();
    bool t3 = testPrepareV2_InvalidSQL();
    bool t4 = testPrepareV2_InvalidNBytes();
    bool t5 = testPrepareV2_SingleStatementNoTail();

    int total = t1 + t2 + t3 + t4 + t5;
    if (total == 5) {
        std::cout << "[OVERALL PASS] All sqlite3_prepare_v2 unit tests passed.\n";
        return 0;
    } else {
        std::cout << "[OVERALL FAIL] " << (5 - total) << " test(s) failed.\n";
        return 1;
    }
}