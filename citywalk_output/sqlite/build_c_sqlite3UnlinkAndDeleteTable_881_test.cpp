// C++11 test harness for SQLite's sqlite3UnlinkAndDeleteTable
// Approach:
// - Provide minimal C-style stubs for the dependencies used by sqlite3UnlinkAndDeleteTable
// - Include the focal C source (build.c) in this translation unit to let it link against our stubs
// - Run two test cases (normal path with non-empty name and zero-length name) and verify the expected side effects
// - Use non-terminating expectations (simple boolean checks with console output) as required (no GTest)

/*
Notes:
- We define a few internal stubs and a very small in-memory representation of the structures used by sqlite3UnlinkAndDeleteTable.
- We override the test macro 'testcase' to a no-op so that the focal code compiles in this environment.
- This test focuses on the positive path (assertions are assumed to hold under these tests); we verify that:
  1) sqlite3DeleteTable is invoked via our hook (via g_lastDeletedTable)
  2) The schema-change flag is set on the db (DBFLAG_SchemaChange)
  3) The inserted table name matches what's passed to the hash insert
- Branch coverage for asserts can be limited in this environment due to asserts aborting on failure; we cover the primary execution path and zero-length name path (to exercise the testcase macro usage).
*/

#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <build.c>


// Step 1: Define testcase macro to avoid external test framework dependencies
#define testcase(x) do { (void)0; } while(0)

// Step 2: Provide C-style type declarations and lightweight implementations
// These definitions are crafted to satisfy the focal function's usage within build.c

// Forward declarations to mimic SQLite internal types used by sqlite3UnlinkAndDeleteTable
typedef struct Table Table;
typedef struct Hash Hash;
typedef struct Schema Schema;
typedef struct Db Db;
typedef struct sqlite3 sqlite3;

// Lightweight representations
struct Table {
    char *zName;
};

struct Hash {
    int _dummy; // not used; placeholder
};

struct Schema {
    Hash tblHash;
};

struct Db {
    Schema *pSchema;
};

// Public SQLite handle (simplified)
struct sqlite3 {
    int nDb;
    Db *aDb;
    int mDbFlags;
};

// A few internal utilities/macros that the focal function uses
#ifndef DBFLAG_SchemaChange
#define DBFLAG_SchemaChange 0x01
#endif

// Global hooks/state used by stubs to verify behavior
static Table* g_lastDeletedTable = nullptr;
static int g_schemaMutexHeldFlag = 1; // simulate that the schema mutex is held (true by default)

// Prototypes for the functions that sqlite3UnlinkAndDeleteTable will call
extern "C" Table* sqlite3HashInsert(Hash *pHash, const char *zKey, int dummy);
extern "C" void sqlite3DeleteTable(sqlite3 *db, Table *pTable);
extern "C" int sqlite3SchemaMutexHeld(sqlite3 *db, int iDb, int dummy);

// Step 3: Implement stubs with C linkage to be discovered by the linker during build.c inclusion

extern "C" {

Table* sqlite3HashInsert(Hash *pHash, const char *zKey, int dummy) {
    // Simple hash-insert stub: allocate a Table with the given key as name
    Table* t = new Table();
    if (zKey) {
        size_t len = std::strlen(zKey) + 1;
        t->zName = (char*)std::malloc(len);
        std::memcpy(t->zName, zKey, len);
    } else {
        t->zName = nullptr;
    }
    return t;
}

void sqlite3DeleteTable(sqlite3 *db, Table *pTable) {
    // Record the table that would have been deleted
    (void)db; // unused in test stub
    g_lastDeletedTable = pTable;
}

int sqlite3SchemaMutexHeld(sqlite3 *db, int iDb, int dummy) {
    (void)db; (void)iDb; (void(dummy);
    return g_schemaMutexHeldFlag;
}

} // extern "C"

// Step 4: Include the focal source file (build.c)
// The build.c is expected to be in the same path as this test or accessible via include path
extern "C" {
}

// Step 5: Simple test harness (non-GTest) with verbose output
static int total_tests = 0;
static int tests_passed = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        total_tests++; \
        if (cond) { \
            ++tests_passed; \
            std::cout << "[PASS] " << msg << std::endl; \
        } else { \
            std::cout << "[FAIL] " << msg << std::endl; \
        } \
    } while(0)

// Helper to clean up allocated memory for test case
static void cleanup_test_resources(sqlite3 *db) {
    if (!db) return;
    if (db->aDb) {
        for (int i = 0; i < db->nDb; ++i) {
            if (db->aDb[i].pSchema) {
                // tblHash is a member; no dynamic allocation in our stub
                free(db->aDb[i].pSchema);
            }
        }
        free(db->aDb);
    }
    free(db);
}

// Test 1: Normal path with a non-empty table name
static void test_unlink_and_delete_table_normal_path() {
    // Arrange
    sqlite3 *db = (sqlite3*)std::malloc(sizeof(sqlite3));
    db->nDb = 1;
    db->mDbFlags = 0;
    db->aDb = (Db*)std::malloc(sizeof(Db) * db->nDb);
    Db* pDb0 = &db->aDb[0];
    pDb0->pSchema = (Schema*)std::malloc(sizeof(Schema));
    // Initialize hash struct (contents are not used by our stub)
    pDb0->pSchema->tblHash = Hash();

    g_lastDeletedTable = nullptr;
    g_schemaMutexHeldFlag = 1; // ensure asserts pass

    const char *zTabName = "TestTable";

    // Act
    sqlite3UnlinkAndDeleteTable(db, 0, zTabName);

    // Assert
    bool tableDeleted = (g_lastDeletedTable != nullptr);
    bool tableNameMatches = tableDeleted && g_lastDeletedTable->zName != nullptr &&
                          std::strcmp(g_lastDeletedTable->zName, zTabName) == 0;
    bool schemaFlagSet = (db->mDbFlags & DBFLAG_SchemaChange) != 0;

    // Cleanup
    if (g_lastDeletedTable) {
        std::free(g_lastDeletedTable->zName);
        delete g_lastDeletedTable;
        g_lastDeletedTable = nullptr;
    }
    cleanup_test_resources(db);

    // Record results
    EXPECT_TRUE(tableDeleted && tableNameMatches && schemaFlagSet,
                "Normal path: DeleteTable invoked, name matches, and DBFLAG_SchemaChange set");
}

// Test 2: Path with a zero-length table name
static void test_unlink_and_delete_table_zero_length_name() {
    // Arrange
    sqlite3 *db = (sqlite3*)std::malloc(sizeof(sqlite3));
    db->nDb = 1;
    db->mDbFlags = 0;
    db->aDb = (Db*)std::malloc(sizeof(Db) * db->nDb);
    Db* pDb0 = &db->aDb[0];
    pDb0->pSchema = (Schema*)std::malloc(sizeof(Schema));
    pDb0->pSchema->tblHash = Hash();

    g_lastDeletedTable = nullptr;
    g_schemaMutexHeldFlag = 1; // ensure asserts pass

    const char *zTabName = ""; // zero-length

    // Act
    sqlite3UnlinkAndDeleteTable(db, 0, zTabName);

    // Assert
    bool tableDeleted = (g_lastDeletedTable != nullptr);
    bool tableNameMatches = tableDeleted && g_lastDeletedTable->zName != nullptr &&
                          std::strcmp(g_lastDeletedTable->zName, zTabName) == 0;
    bool schemaFlagSet = (db->mDbFlags & DBFLAG_SchemaChange) != 0;

    // Cleanup
    if (g_lastDeletedTable) {
        std::free(g_lastDeletedTable->zName);
        delete g_lastDeletedTable;
        g_lastDeletedTable = nullptr;
    }
    cleanup_test_resources(db);

    // Record results
    EXPECT_TRUE(tableDeleted && tableNameMatches && schemaFlagSet,
                "Zero-length name path: DeleteTable invoked, name matches (empty string), and DBFLAG_SchemaChange set");
}

// Step 6: Main function to run tests
int main() {
    std::cout << "Starting sqlite3UnlinkAndDeleteTable test suite (C++11 harness)..." << std::endl;

    test_unlink_and_delete_table_normal_path();
    test_unlink_and_delete_table_zero_length_name();

    std::cout << "Tests completed. Passed: " << tests_passed << " / " << total_tests << std::endl;

    // Return non-zero if any test failed
    return (total_tests == tests_passed) ? 0 : 1;
}