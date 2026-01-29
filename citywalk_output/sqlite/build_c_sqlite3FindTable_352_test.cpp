/*
   Unit tests for sqlite3FindTable (focal method) using a self-contained, mocked
   SQLite-like environment. The goal is to exercise true/false branches of
   the predicate logic in sqlite3FindTable as described in the prompt, while
   remaining C++11 compliant and not using GTest.

   Notes on approach and candidate keywords (Step 1):
   - Core components: sqlite3 db container, databases (aDb), zDbSName, zName,
     zDatabase, schemas (pSchema) with tblHash (HashFind), LEGACY/PREFERRED
     schema tables, sqlite_ prefixed names.
   - Branch coverage targets:
     - zDatabase != 0 (non-null) vs zDatabase == 0
     - Valid i index and main/temp/attached schema resolution
     - sqlite_ prefixed table handling (sqlite_main, sqlite_temp, sqlite_legacy)
     - Legacy/prefixed table resolution via LEGACY_SCHEMA_TABLE,
       LEGACY_TEMP_SCHEMA_TABLE
   - Static helpers: sqlite3StrICmp, sqlite3StrNICmp, sqlite3HashFind, etc.
   - Domain knowledge constraints applied: create minimal stubs for dependencies,
     use simple in-memory mappings, and avoid depending on the real SQLite runtime.
   - Static members: accessed only via the global test harness; this file provides
     a self-contained set of helpers; no private/hidden members of external code
     are relied upon.

   This test harness is designed to be compiled together with the focal method
   in a single C++11 translation unit (no GTest needed). The sqlite3FindTable
   implementation here mirrors the logic in the provided snippet and uses a
   controlled, in-memory data model to drive deterministic outcomes.
*/

#include <vector>
#include <iomanip>
#include <string>
#include <cctype>
#include <sqliteInt.h>
#include <unordered_map>
#include <iostream>
#include <cassert>


// Domain knowledge: test names and macros
#define TEST_OK(cond, msg) testAssert((cond), (msg), __FILE__, __LINE__)
static int g_test_failures = 0;

static void testAssert(bool condition, const char* message, const char* file, int line) {
    if (!condition) {
        std::cerr << "[TEST FAILED] " << file << ":" << line << " - " << message << std::endl;
        ++g_test_failures;
    }
}

// Candidate Keywords derived from the focal method
// - db, nDb, aDb, zDbSName, pSchema, tblHash, zName, zDatabase
// - sqlite3HashFind, sqlite3StrICmp, sqlite3StrNICmp, LEGACY_SCHEMA_TABLE,
//   LEGACY_TEMP_SCHEMA_TABLE, PREFERRED_SCHEMA_TABLE, PREFERRED_TEMP_SCHEMA_TABLE

// --- Minimal in-memory scaffolding to mock sqlite3FindTable's environment ---

typedef unsigned char u8;
typedef unsigned int Pgno;
typedef int i32;
struct Table { int dummy; }; // opaque for test purposes

// Hash map wrapper used by pSchema->tblHash
struct HashTableImpl {
    std::unordered_map<std::string, Table*> tbl;
};

struct Hash {
    HashTableImpl* p;
};

struct Schema {
    Hash tblHash; // maps table name -> Table*
};

struct Db {
    const char* zDbSName; // e.g., "main", "temp", "attached"
    Schema* pSchema;
};

struct Auth {
    int authLevel;
};

struct sqlite3 {
    int nDb;
    Db* aDb;      // array of nDb entries
    Auth auth;
};

// Constants used by sqlite3FindTable for sqlite_ prefixed special tables
static const char LEGACY_SCHEMA_TABLE[]     = "sqlite_legacy";
static const char LEGACY_TEMP_SCHEMA_TABLE[] = "sqlite_legacy_temp";
static const char PREFERRED_SCHEMA_TABLE[]   = "sqlite_main";
static const char PREFERRED_TEMP_SCHEMA_TABLE[] = "sqlite_temp";

// Forward declarations for helpers expected by sqlite3FindTable
static Table* sqlite3HashFind(Hash* h, const char* zName);
static int sqlite3StrICmp(const char* zIn1, const char* zIn2);
static int sqlite3StrNICmp(const char* zIn1, const char* zIn2, int n);
static int sqlite3BtreeHoldsAllMutexes(sqlite3*); // always true in tests
static int sqlite3SchemaMutexHeld(sqlite3*, int iDb, int isWrite);
static int sqlite3UserAuthTable(const char* zName); // never used in tests (macros off)
static int sqlite3Strlen(const char* s);
static int isSameString(const char* a, const char* b);

// Mocked/Stubbed helpers
static Table* sqlite3HashFind(Hash* h, const char* zName) {
    if (!h || !h->p) return nullptr;
    auto it = h->p->tbl.find(zName);
    if (it == h->p->tbl.end()) return nullptr;
    return it->second;
}

// Case-insensitive string compare (like sqlite3StrICmp)
static int sqlite3StrICmp(const char* zIn1, const char* zIn2) {
    if (zIn1 == zIn2) return 0;
    while (*zIn1 && *zIn2) {
        unsigned char c1 = static_cast<unsigned char>(std::tolower(*zIn1));
        unsigned char c2 = static_cast<unsigned char>(std::tolower(*zIn2));
        if (c1 != c2) return (int)c1 - (int)c2;
        ++zIn1; ++zIn2;
    }
    return (int)(unsigned char)*zIn1 - (int)(unsigned char)*zIn2;
}

// Case-insensitive n-prefix compare
static int sqlite3StrNICmp(const char* zIn1, const char* zIn2, int n) {
    int i = 0;
    while (i < n && zIn1[i] && zIn2[i]) {
        unsigned char c1 = static_cast<unsigned char>(std::tolower((unsigned char)zIn1[i]));
        unsigned char c2 = static_cast<unsigned char>(std::tolower((unsigned char)zIn2[i]));
        if (c1 != c2) return (int)c1 - (int)c2;
        ++i;
    }
    if (i == n) return 0;
    return (int)(unsigned char)zIn1[i] - (int)(unsigned char)zIn2[i];
}

// Helpers to satisfy the non-test parts
static int sqlite3BtreeHoldsAllMutexes(sqlite3* db) { (void)db; return 1; } // always hold in test
static int sqlite3SchemaMutexHeld(sqlite3* db, int iDb, int isWrite) {
    (void)db; (void)isWrite; (void)iDb;
    return 1; // always allowed in test environment
}
static int sqlite3UserAuthTable(const char* zName) { (void)zName; return 0; } // not used in tests

// A tiny helper to create a new Table instance
static Table* newTable() { return new Table(); }

// Non-terminating assertions (we'll collect failures and print at end)
static void runAllTests();

// --- sqlite3FindTable implementation (self-contained for this test) ---
Table* sqlite3FindTable(sqlite3* db, const char* zName, const char* zDatabase) {
    Table *p = 0;
    int i;
    // All mutexes are required for schema access in real code; ensure
    // that a non-null database is provided or mutex-holding is simulated.
    // Our tests ensure that condition holds.
    if (zDatabase != nullptr) {
        for (i = 0; i < db->nDb; i++) {
            if (sqlite3StrICmp(zDatabase, db->aDb[i].zDbSName) == 0) break;
        }
        if (i >= db->nDb) {
            // No match against official names. Legacy fallback to main for "main".
            if (sqlite3StrICmp(zDatabase, "main") == 0) {
                i = 0;
            } else {
                return 0;
            }
        }
        p = sqlite3HashFind(&db->aDb[i].pSchema->tblHash, zName);
        if (p == 0 && sqlite3StrNICmp(zName, "sqlite_", 7) == 0) {
            if (i == 1) {
                if (sqlite3StrICmp(zName + 7, &PREFERRED_TEMP_SCHEMA_TABLE[7]) == 0
                    || sqlite3StrICmp(zName + 7, &PREFERRED_SCHEMA_TABLE[7]) == 0
                    || sqlite3StrICmp(zName + 7, &LEGACY_SCHEMA_TABLE[7]) == 0) {
                    p = sqlite3HashFind(&db->aDb[1].pSchema->tblHash, LEGACY_TEMP_SCHEMA_TABLE);
                }
            } else {
                if (sqlite3StrICmp(zName + 7, &PREFERRED_SCHEMA_TABLE[7]) == 0) {
                    p = sqlite3HashFind(&db->aDb[i].pSchema->tblHash, LEGACY_SCHEMA_TABLE);
                }
            }
        }
    } else {
        // No database specified: search TEMP first, then MAIN, then attachments.
        p = sqlite3HashFind(&db->aDb[1].pSchema->tblHash, zName);
        if (p) return p;
        p = sqlite3HashFind(&db->aDb[0].pSchema->tblHash, zName);
        if (p) return p;
        for (i = 2; i < db->nDb; i++) {
            // In real code: sqlite3SchemaMutexHeld(db, i, 0)
            assert(sqlite3SchemaMutexHeld(db, i, 0));
            p = sqlite3HashFind(&db->aDb[i].pSchema->tblHash, zName);
            if (p) break;
        }
        if (p == 0 && sqlite3StrNICmp(zName, "sqlite_", 7) == 0) {
            if (sqlite3StrICmp(zName + 7, &PREFERRED_SCHEMA_TABLE[7]) == 0) {
                p = sqlite3HashFind(&db->aDb[0].pSchema->tblHash, LEGACY_SCHEMA_TABLE);
            } else if (sqlite3StrICmp(zName + 7, &PREFERRED_TEMP_SCHEMA_TABLE[7]) == 0) {
                p = sqlite3HashFind(&db->aDb[1].pSchema->tblHash, LEGACY_TEMP_SCHEMA_TABLE);
            }
        }
    }
    return p;
}

// --- Test scaffolding (scenes) ---

static void test_case_1_basic_database_lookup() {
    // Objective: zDatabase != NULL and matches aDb[1] (temp) map direct lookup
    // Expect to retrieve a Table* that we place in temp's hash with key equal to zName.

    // Build a fake DB with 2 databases: main and temp
    sqlite3 db;
    Table* t_temp_lookup = newTable();

    // Allocate aDb array with 2 entries
    Db aDb0, aDb1;
    Schema s0, s1;
    Hash h0{ nullptr }, h1{ nullptr };
    h0.p = new HashTableImpl(); // allocate actual map holder
    h1.p = new HashTableImpl();

    // Connect to db struct
    db.nDb = 2;
    db.aDb = new Db[2];
    // MAIN
    aDb0.zDbSName = "main";
    aDb0.pSchema = &s0;
    s0.tblHash.p = reinterpret_cast<HashTableImpl*>(new HashTableImpl());
    // Put a direct table for test in main for later negative branch test
    // (not used in this particular test case)

    // TEMP
    aDb1.zDbSName = "temp";
    aDb1.pSchema = &s1;
    s1.tblHash.p = reinterpret_cast<HashTableImpl*>(new HashTableImpl());

    // Install into arrays
    db.aDb[0] = aDb0;
    db.aDb[1] = aDb1;

    // Add lookup table in temp for zName = "tbl_in_temp"
    Table* tblInTemp = t_temp_lookup;
    static_cast<HashTableImpl*>(s1.tblHash.p)->tbl.emplace("tbl_in_temp", tblInTemp);

    // Also provide a NULL for other entries as needed
    // Call the function
    Table* res = sqlite3FindTable(&db, "tbl_in_temp", "temp");

    TEST_OK(res == tblInTemp, "Case 1: should find tbl_in_temp in temp database");
}

static void test_case_2_prefixed_main_legacy_lookup() {
    // Objective: zName = "sqlite_main" and main's hash contains LEGACY_SCHEMA_TABLE
    // Expect to retrieve the legacy table from main's schema.

    sqlite3 db;
    Table* tblLegacyInMain = newTable();

    // Build DB with two databases: main, temp
    Db aDb0, aDb1;
    Schema s0, s1;
    Hash *h0 = new Hash{ nullptr };
    Hash *h1 = new Hash{ nullptr };
    h0->p = new HashTableImpl();
    h1->p = new HashTableImpl();

    db.nDb = 2;
    db.aDb = new Db[2];
    aDb0.zDbSName = "main";
    aDb0.pSchema = &s0;
    s0.tblHash.p = reinterpret_cast<HashTableImpl*>(h0->p);
    // Put legacy table in main with key LEGACY_SCHEMA_TABLE
    static_cast<HashTableImpl*>(s0.tblHash.p)->tbl.emplace(LEGACY_SCHEMA_TABLE, tblLegacyInMain);

    aDb1.zDbSName = "temp";
    aDb1.pSchema = &s1;
    s1.tblHash.p = reinterpret_cast<HashTableImpl*>(h1->p);

    db.aDb[0] = aDb0;
    db.aDb[1] = aDb1;

    Table* res = sqlite3FindTable(&db, "sqlite_main", "main");
    TEST_OK(res == tblLegacyInMain, "Case 2: sqlite_main should map to LEGACY_SCHEMA_TABLE in main");
}

static void test_case_3_prefixed_temp_lookup() {
    // Objective: i==1 path for zName "sqlite_temp" should pull LEGACY_TEMP_SCHEMA_TABLE
    sqlite3 db;
    Table* tblLegacyInTemp = newTable();

    Db aDb0, aDb1;
    Schema s0, s1;
    Hash* h0 = new Hash{ nullptr };
    Hash* h1 = new Hash{ nullptr };

    db.nDb = 2;
    db.aDb = new Db[2];
    aDb0.zDbSName = "main";
    aDb0.pSchema = &s0;
    s0.tblHash.p = reinterpret_cast<HashTableImpl*>(new HashTableImpl());

    aDb1.zDbSName = "temp";
    aDb1.pSchema = &s1;
    s1.tblHash.p = reinterpret_cast<HashTableImpl*>(new HashTableImpl());

    db.aDb[0] = aDb0;
    db.aDb[1] = aDb1;

    // Put a legacy temp table in temp's hash under LEGACY_TEMP_SCHEMA_TABLE
    static_cast<HashTableImpl*>(s1.tblHash.p)->tbl.emplace(LEGACY_TEMP_SCHEMA_TABLE, tblLegacyInTemp);

    Table* res = sqlite3FindTable(&db, "sqlite_temp", "temp");
    TEST_OK(res == tblLegacyInTemp, "Case 3: sqlite_temp should map to LEGACY_TEMP_SCHEMA_TABLE in temp");
}

static void test_case_4_attached_lookup() {
    // Objective: zDatabase == NULL and an attached database contains the table
    sqlite3 db;
    Table* tblInAttached = newTable();

    // MAIN and TEMP with empty schemas
    Db aDb0, aDb1, aDb2;
    Schema s0, s1, s2;

    db.nDb = 3;
    db.aDb = new Db[3];
    aDb0.zDbSName = "main";
    aDb0.pSchema = &s0;
    s0.tblHash.p = reinterpret_cast<HashTableImpl*>(new HashTableImpl());

    aDb1.zDbSName = "temp";
    aDb1.pSchema = &s1;
    s1.tblHash.p = reinterpret_cast<HashTableImpl*>(new HashTableImpl());

    aDb2.zDbSName = "attached";
    aDb2.pSchema = &s2;
    s2.tblHash.p = reinterpret_cast<HashTableImpl*>(new HashTableImpl());

    // Attach a table in the attached database
    static_cast<HashTableImpl*>(s2.tblHash.p)->tbl.emplace("attached_table", tblInAttached);

    db.aDb[0] = aDb0;
    db.aDb[1] = aDb1;
    db.aDb[2] = aDb2;

    // Query from NULL database
    Table* res = sqlite3FindTable(&db, "attached_table", nullptr);
    TEST_OK(res == tblInAttached, "Case 4: lookup in attached database when zDatabase is NULL");
}

static void test_case_5_nonexistent_lookup() {
    // Objective: Non-existent table should yield NULL
    sqlite3 db;
    Db aDb0;
    Schema s0;
    Hash h0{ nullptr };

    db.nDb = 1;
    db.aDb = new Db[1];
    aDb0.zDbSName = "main";
    aDb0.pSchema = &s0;
    s0.tblHash.p = reinterpret_cast<HashTableImpl*>(new HashTableImpl());

    db.aDb[0] = aDb0;

    Table* res = sqlite3FindTable(&db, "no_such_table", nullptr);
    TEST_OK(res == nullptr, "Case 5: non-existent table should return NULL");
}

// Entrypoint for tests
static void runAllTests() {
    test_case_1_basic_database_lookup();
    test_case_2_prefixed_main_legacy_lookup();
    test_case_3_prefixed_temp_lookup();
    test_case_4_attached_lookup();
    test_case_5_nonexistent_lookup();

    std::cout << std::setw(20) << std::left << "Test Summary"
              << " | Failures: " << g_test_failures << std::endl;
}

// Main function to drive tests (no external test framework)
int main() {
    runAllTests();
    if (g_test_failures > 0) {
        std::cerr << "Some tests failed. Total failures: " << g_test_failures << "\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}