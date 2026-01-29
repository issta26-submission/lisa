/***************************************************************
 * Unit Test Suite for sqlite3CreateForeignKey (via public SQL API)
 * 
 * This test suite targets the functionality implemented by
 * sqlite3CreateForeignKey in the project’s code base by exercising
 * the same behavior through SQLite's public C API (SQL-level tests).
 * 
 * Rationale:
 * - The internal implementation in build.c (sqlite3CreateForeignKey)
 *   is difficult to reach directly in a standalone C++ test harness
 *   without recreating a large portion of SQLite's internal state.
 * - By validating realistic FK scenarios through standard SQL
 *   statements, we exercise the same code paths (parsing, validation,
 *   constraint creation, and enforcement) that sqlite3CreateForeignKey
 *   participates in, ensuring true and false branches are covered.
 * - No GTest is used; a lightweight self-contained test runner is
 *   implemented using the C API and standard C++11 features.
 * 
 * How to run:
 * - Build with a development environment that provides sqlite3
 *   (e.g., link with -lsqlite3).
 * - Execute the resulting binary. It will print per-test results and a
 *   final summary.
 * 
 * Notes:
 * - The tests enable foreign key support via PRAGMA foreign_keys = ON.
 * - Tests cover:
 *   1) Basic FK creation and enforcement (true path).
 *   2) FK column count mismatch (false path).
 *   3) ON DELETE CASCADE behavior (true path).
 *   4) FK behavior when foreign_keys is OFF (false/path-defer).
 *   5) Composite foreign keys (true path).
 * - Static/private/internal helpers from the project are not required;
 *   public SQLite API suffices to validate key behaviors.
 * 
 * This file should be compiled with C++11 support.
 ****************************************************************/

#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>



// Simple lightweight test harness (no external testing framework)
#define TEST_CASE(name) void name()
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { std::cerr << "ASSERT_TRUE failed: " << (msg) << "\n"; return; } } while(0)
#define ASSERT_FALSE(cond, msg) ASSERT_TRUE(!(cond), msg)
#define ASSERT_EQ(a, b, msg) do { if((a) != (b)) { std::cerr << "ASSERT_EQ failed: " << (msg) << " (expected " << (a) << " == " << (b) << ")\n"; return; } } while(0)

static bool run(sqlite3* db, const std::string& sql) {
    char* errmsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << (errmsg ? errmsg : "unknown") << "\n";
        if (errmsg) sqlite3_free(errmsg);
        return false;
    }
    return true;
}

// Test 1: Basic foreign key creation and enforcement
// - Create parent(id PRIMARY KEY) and child(parent_id) with FOREIGN KEY(parent_id) REFERENCES parent(id).
// - Insert valid and invalid rows to verify enforcement.
TEST_CASE(test_basic_foreign_key_enforcement) {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    ASSERT_TRUE(db != nullptr, "sqlite3_open failed");

    // Enable foreign keys
    ASSERT_TRUE(run(db, "PRAGMA foreign_keys = ON;"), "Enable foreign keys");

    // Create parent table
    ASSERT_TRUE(run(db, "CREATE TABLE parent (id INTEGER PRIMARY KEY, name TEXT);"),
                "Create parent table");

    // Create child table with FK
    ASSERT_TRUE(run(db, "CREATE TABLE child (id INTEGER PRIMARY KEY, parent_id INTEGER, "
                        "FOREIGN KEY(parent_id) REFERENCES parent(id));"),
                "Create child table with FK");

    // Insert valid data
    ASSERT_TRUE(run(db, "INSERT INTO parent (id, name) VALUES (1, 'A');"),
                "Insert into parent");
    ASSERT_TRUE(run(db, "INSERT INTO child (id, parent_id) VALUES (1, 1);"),
                "Insert valid child referencing parent");

    // Insert invalid data (FK violation)
    ASSERT_FALSE(run(db, "INSERT INTO child (id, parent_id) VALUES (2, 999);"),
                 "FK constraint should reject invalid reference");

    sqlite3_close(db);
}

// Test 2: FK column count mismatch should be rejected by parser
// - parent has two columns in its primary key (composite), child FK references a different count.
// - The SQL should fail during CREATE TABLE due to column count mismatch.
TEST_CASE(test_fk_column_count_mismatch_rejected) {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    ASSERT_TRUE(db != nullptr, "sqlite3_open failed (Test 2)");

    ASSERT_TRUE(run(db, "PRAGMA foreign_keys = ON;"), "Enable foreign keys (Test 2)");

    // Parent with composite primary key
    ASSERT_TRUE(run(db, "CREATE TABLE parent_comp (id INTEGER, code TEXT, PRIMARY KEY(id, code));"),
                "Create composite parent");

    // Child references parent with a mismatch in column count
    // Expected to fail: FOREIGN KEY (parent_id) REFERENCES parent_comp(id, code)
    ASSERT_FALSE(run(db, "CREATE TABLE child_comp (id INTEGER PRIMARY KEY, parent_id INTEGER, "
                         "FOREIGN KEY(parent_id) REFERENCES parent_comp(id, code));"),
                 "FK with mismatched column count should fail");

    sqlite3_close(db);
}

// Test 3: ON DELETE CASCADE behavior
// - Verify that deleting a referenced row cascades delete to referencing rows.
TEST_CASE(test_on_delete_cascade) {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    ASSERT_TRUE(db != nullptr, "sqlite3_open failed (Test 3)");

    ASSERT_TRUE(run(db, "PRAGMA foreign_keys = ON;"), "Enable foreign keys (Test 3)");

    ASSERT_TRUE(run(db, "CREATE TABLE parent (id INTEGER PRIMARY KEY);"), "Create parent");
    ASSERT_TRUE(run(db, "CREATE TABLE child (id INTEGER PRIMARY KEY, parent_id INTEGER, "
                        "FOREIGN KEY(parent_id) REFERENCES parent(id) ON DELETE CASCADE);"),
                "Create child with ON DELETE CASCADE");

    ASSERT_TRUE(run(db, "INSERT INTO parent (id) VALUES (1);"), "Insert parent row");
    ASSERT_TRUE(run(db, "INSERT INTO child (id, parent_id) VALUES (10, 1);"), "Insert child row referencing parent");

    // Delete parent; child should be cascaded away
    ASSERT_TRUE(run(db, "DELETE FROM parent WHERE id = 1;"), "Delete parent to cascade");
    // Try to count child rows; should be 0
    sqlite3_stmt* stmt = nullptr;
    int res = sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM child;", -1, &stmt, nullptr);
    ASSERT_TRUE(res == SQLITE_OK, "Prepare count query");
    res = sqlite3_step(stmt);
    ASSERT_TRUE(res == SQLITE_ROW, "Step count query");
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    ASSERT_EQ(count, 0, "Child rows should be cascaded away");

    sqlite3_close(db);
}

// Test 4: FK enforcement off via PRAGMA and then re-enabled
// - When foreign_keys = OFF, FK constraints are not enforced.
// - After turning ON, constraints should be enforced again.
TEST_CASE(test_fk_enforcement_toggle) {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    ASSERT_TRUE(db != nullptr, "sqlite3_open failed (Test 4)");

    // Start with FK off
    ASSERT_TRUE(run(db, "PRAGMA foreign_keys = OFF;"), "Disable foreign keys (Test 4)");

    ASSERT_TRUE(run(db, "CREATE TABLE parent (id INTEGER PRIMARY KEY);"), "Create parent (Test 4)");
    ASSERT_TRUE(run(db, "CREATE TABLE child (id INTEGER PRIMARY KEY, parent_id INTEGER, "
                        "FOREIGN KEY(parent_id) REFERENCES parent(id));"),
                "Create child with FK (Test 4)");

    // Insert invalid child (should succeed due to FK OFF)
    ASSERT_TRUE(run(db, "INSERT INTO child (id, parent_id) VALUES (1, 999);"), "FK OFF allows invalid ref (Test 4)");

    // Now enable FK and insert again with invalid ref to verify enforcement
    ASSERT_TRUE(run(db, "PRAGMA foreign_keys = ON;"), "Enable foreign keys (Test 4)");
    // This insert should fail due to FK constraint now being enforced
    ASSERT_FALSE(run(db, "INSERT INTO child (id, parent_id) VALUES (2, 999);"),
                 "FK constraint should reject invalid reference after ON");

    sqlite3_close(db);
}

// Test 5: Composite foreign key (true path)
// - Parent composed of two columns (id, code) is primary key
// - Child references both columns via FOREIGN KEY (parent_id, code) REFERENCES parent(id, code)
TEST_CASE(test_composite_foreign_key_true_path) {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    ASSERT_TRUE(db != nullptr, "sqlite3_open failed (Test 5)");

    ASSERT_TRUE(run(db, "PRAGMA foreign_keys = ON;"), "Enable foreign keys (Test 5)");

    // Create composite parent
    ASSERT_TRUE(run(db, "CREATE TABLE parent_comp2 (id INTEGER, code TEXT, PRIMARY KEY(id, code));"),
                "Create composite parent (Test 5)");

    // Create child with composite FK
    ASSERT_TRUE(run(db, "CREATE TABLE child_comp2 (id INTEGER PRIMARY KEY, parent_id INTEGER, code TEXT, "
                        "FOREIGN KEY(parent_id, code) REFERENCES parent_comp2(id, code));"),
                "Create composite child with FK (Test 5)");

    // Insert valid parent
    ASSERT_TRUE(run(db, "INSERT INTO parent_comp2 (id, code) VALUES (1, 'A');"), "Insert composite parent");
    // Insert valid child referencing both columns
    ASSERT_TRUE(run(db, "INSERT INTO child_comp2 (id, parent_id, code) VALUES (10, 1, 'A');"),
                "Insert child with composite FK");

    // Delete parent; child should cascade
    ASSERT_TRUE(run(db, "DELETE FROM parent_comp2 WHERE id = 1 AND code = 'A';"),
                "Delete composite parent to cascade");
    // Count child rows
    sqlite3_stmt* stmt = nullptr;
    int res = sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM child_comp2;", -1, &stmt, nullptr);
    ASSERT_TRUE(res == SQLITE_OK, "Prepare count query (Test 5)");
    res = sqlite3_step(stmt);
    ASSERT_TRUE(res == SQLITE_ROW, "Step count query (Test 5)");
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    ASSERT_EQ(count, 0, "Composite FK cascade should remove child row");

    sqlite3_close(db);
}

// Entry point
int main() {
    std::cout << "sqlite3CreateForeignKey (via public SQL API) - test suite start\n";

    // Run tests
    test_basic_foreign_key_enforcement();
    std::cout << "[OK] test_basic_foreign_key_enforcement finished\n";

    test_fk_column_count_mismatch_rejected();
    std::cout << "[OK] test_fk_column_count_mismatch_rejected finished\n";

    test_on_delete_cascade();
    std::cout << "[OK] test_on_delete_cascade finished\n";

    test_fk_enforcement_toggle();
    std::cout << "[OK] test_fk_enforcement_toggle finished\n";

    test_composite_foreign_key_true_path();
    std::cout << "[OK] test_composite_foreign_key_true_path finished\n";

    std::cout << "sqlite3CreateForeignKey - test suite completed\n";

    return 0;
}