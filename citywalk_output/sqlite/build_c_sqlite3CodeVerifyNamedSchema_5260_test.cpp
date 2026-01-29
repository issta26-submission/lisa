/*
   Unit test suite for the focal method equivalent to sqlite3CodeVerifyNamedSchema
   adapted for C++11 without Google Test.

   Notes:
   - This test deliberately re-implements a small, self-contained subset of the
     environment needed to exercise the logic of sqlite3CodeVerifyNamedSchema.
   - It uses a minimal, test-only version of the helper function sqlite3CodeVerifySchema
     to record which iDb indices would be verified.
   - The tests are designed to cover true/false branches of the condition in:
       if (pDb->pBt && (!zDb || 0 == sqlite3StrICmp(zDb, pDb->zDbSName)))
     i.e., both sides of the AND and the two parts of the OR inside.
   - Running this file directly will execute all tests and print a simple pass/fail summary.

   Limitations:
   - This is a focused, stand-alone test harness. It does not depend on external
     SQLite binaries or GTest. It mirrors the control flow of the focal function
     for the purpose of unit testing its logic in isolation.
*/

#include <vector>
#include <cctype>
#include <string>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// ---------------------------
// Minimal environment stubs
// ---------------------------

struct Db {
    void* pBt;            // non-null means "has B-tree" in original code
    const char* zDbSName; // database schema name
};

struct sqlite3 {
    int nDb;
    Db* aDb;
};

struct Parse {
    sqlite3* db;
};

// Global vector to record which iDb indices were "verified" during tests
static std::vector<int> g_verifiedDbIndices;

// Case-insensitive string compare (compatible with sqlite3StrICmp contract for non-null inputs)
static int sqlite3StrICmp(const char* zA, const char* zB) {
    if (zA == zB) return 0;
    if (zA == nullptr) return -1;
    if (zB == nullptr) return 1;
    while (*zA && *zB) {
        unsigned char ca = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*zA)));
        unsigned char cb = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*zB)));
        if (ca != cb) return (int)ca - (int)cb;
        ++zA; ++zB;
    }
    if (*zA == *zB) return 0;
    return *zA ? 1 : -1;
}

// Mock version of sqlite3CodeVerifySchema used by the focal method
static void sqlite3CodeVerifySchema(Parse* /*pParse*/, int iDb) {
    g_verifiedDbIndices.push_back(iDb);
}

// The test-specific replica of the focal function logic
static void test_sqlite3CodeVerifyNamedSchema(Parse* pParse, const char* zDb) {
    sqlite3* db = pParse->db;
    int i;
    for (i = 0; i < db->nDb; i++) {
        Db* pDb = &db->aDb[i];
        if (pDb->pBt && (!zDb || 0 == sqlite3StrICmp(zDb, pDb->zDbSName))) {
            sqlite3CodeVerifySchema(pParse, i);
        }
    }
}

// ---------------------------------
// Helper: Simple test framework
// ---------------------------------

struct TestCase {
    std::string name;
    bool (*fn)();
};

// Helper macros for assertions
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { std::cerr << "ASSERT TRUE FAILED: " << (msg) << " (line " << __LINE__ << ")\n"; return false; } \
} while(false)

#define ASSERT_EQ(a, b, msg) do { \
    if ((a) != (b)) { std::cerr << "ASSERT EQUAL FAILED: " << (msg) << " Expected " << (a) << " but got " << (b) << " (line " << __LINE__ << ")\n"; return false; } \
} while(false)

static void clear_records() { g_verifiedDbIndices.clear(); }

// ---------------------------
// Test cases
// ---------------------------

// Test 1: zDb is NULL. Expect verification for every Db with non-null pBt.
static bool test_case_null_zdb_verifies_all_active() {
    clear_records();

    // Build database with 4 entries; all with non-null pBt except one to verify filtering
    const int N = 4;
    Db dbEntries[N] = {
        { (void*)1, "dbOne" },  // active
        { (void*)1, "dbTwo"  }, // active
        { NULL,      "dbThree" }, // inactive (pBt == NULL)
        { (void*)1, "dbFour" }  // active
    };

    sqlite3 db;
    db.nDb = N;
    db.aDb = dbEntries;

    Parse parseObj;
    parseObj.db = &db;

    test_sqlite3CodeVerifyNamedSchema(&parseObj, nullptr); // zDb is NULL

    // Expected: indices 0,1,3 should be verified (skip index 2 due to pBt == NULL)
    ASSERT_EQ((int)g_verifiedDbIndices.size(), 3, "Case1: number of verified indices should be 3");
    // Check exact indices
    ASSERT_TRUE((g_verifiedDbIndices[0] == 0) && (g_verifiedDbIndices[1] == 1) && (g_verifiedDbIndices[2] == 3),
                "Case1: expected verification of indices 0,1,3 in order");

    return true;
}

// Test 2: zDb matches one (or more) databases (case-insensitive). Only matched and active ones verified.
static bool test_case_zdb_matches_specific() {
    clear_records();

    // Build database with 3 entries
    Db dbEntries[3] = {
        { (void*)1, "alpha" },  // matches
        { (void*)1, "BETA" },   // matches if queried with "beta" (case-insensitive)
        { (void*)1, "gamma" }   // does not match
    };

    sqlite3 db;
    db.nDb = 3;
    db.aDb = dbEntries;

    Parse parseObj;
    parseObj.db = &db;

    // zDb equals to "beta" case-insensitively; should verify only index 1
    test_sqlite3CodeVerifyNamedSchema(&parseObj, "beta");

    ASSERT_EQ((int)g_verifiedDbIndices.size(), 1, "Case2: should verify exactly 1 indexed DB");
    ASSERT_EQ(g_verifiedDbIndices[0], 1, "Case2: expected index 1 to be verified");

    return true;
}

// Test 3: zDb does not match any; expect zero verifications.
static bool test_case_zdb_no_match() {
    clear_records();

    Db dbEntries[2] = {
        { (void*)1, "dbA" },
        { (void*)1, "dbB" }
    };

    sqlite3 db;
    db.nDb = 2;
    db.aDb = dbEntries;

    Parse parseObj;
    parseObj.db = &db;

    test_sqlite3CodeVerifyNamedSchema(&parseObj, "nonexistent");

    ASSERT_EQ((int)g_verifiedDbIndices.size(), 0, "Case3: no databases should be verified when no match");
    return true;
}

// Test 4: pBt == NULL entries should be ignored even if zDb matches.
static bool test_case_ignore_inactive_with_match() {
    clear_records();

    Db dbEntries[3] = {
        { (void*)1, "alpha" }, // match
        { NULL,       "alpha" }, // inactive
        { (void*)1, "beta" }  // non-match for "gamma" though
    };

    sqlite3 db;
    db.nDb = 3;
    db.aDb = dbEntries;

    Parse parseObj;
    parseObj.db = &db;

    // zDb matches "alpha"; only index 0 should be verified
    test_sqlite3CodeVerifyNamedSchema(&parseObj, "ALPHA");

    ASSERT_EQ((int)g_verifiedDbIndices.size(), 1, "Case4: only active matching DB should be verified");
    ASSERT_EQ(g_verifiedDbIndices[0], 0, "Case4: expected index 0 to be verified");

    return true;
}

// ---------------------------
// Main runner
// ---------------------------

int main() {
    std::vector<TestCase> tests = {
        {"test_case_null_zdb_verifies_all_active", test_case_null_zdb_verifies_all_active},
        {"test_case_zdb_matches_specific", test_case_zdb_matches_specific},
        {"test_case_zdb_no_match", test_case_z_no_match},
        {"test_case_ignore_inactive_with_match", test_case_ignore_inactive_with_match}
    };

    // Run tests
    int failed = 0;
    for (const auto& t : tests) {
        bool ok = t.fn();
        std::cout << "[" << (ok ? "PASSED" : "FAILED") << "] " << t.name << "\n";
        if (!ok) ++failed;
    }

    if (failed == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << failed << " TEST(S) FAILED\n";
        return 1;
    }
}