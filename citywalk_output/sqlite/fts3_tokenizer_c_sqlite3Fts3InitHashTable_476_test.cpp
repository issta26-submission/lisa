// Unit test harness for sqlite3Fts3InitHashTable
// Note: This test uses the real SQLite library and the internal FTS3 types.
// Compile with: g++ -std=c++11 -I<path_to_sqlite_headers> test_fts3_init_hash_table.cpp -lsqlite3
// and ensure fts3_tokenizer.c is part of the build so the symbol sqlite3Fts3InitHashTable is linked in.

#include <fts3Int.h>
#include <string.h>
#include <assert.h>
#include <sqlite3.h>
#include <iostream>


extern "C" int sqlite3Fts3InitHashTable(sqlite3 *db, Fts3Hash *pHash, const char *zName);

// Simple non-terminating test assertion macros
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[TEST FAIL] " << msg << std::endl; \
        ++g_failures; \
    } \
} while(0)

static void test_basic_init_hash_table()
{
    // Test that the normal code path registers tokenizer functions for a valid DB
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TEST_ASSERT(db != nullptr && rc == SQLITE_OK, "sqlite3_open should succeed for in-memory DB");
    if(db == nullptr || rc != SQLITE_OK) {
        // Cannot proceed
        return;
    }

    Fts3Hash hash;
    // First call should succeed and return SQLITE_OK
    rc = sqlite3Fts3InitHashTable(db, &hash, "test_tokenizer");
    TEST_ASSERT(rc == SQLITE_OK, "sqlite3Fts3InitHashTable should return SQLITE_OK on first registration");

    // Second call with the same db/hash/name should also succeed (overloads for arity)
    rc = sqlite3Fts3InitHashTable(db, &hash, "test_tokenizer");
    TEST_ASSERT(rc == SQLITE_OK, "sqlite3Fts3InitHashTable should return SQLITE_OK on second registration (overload)");
    
    sqlite3_close(db);
}

static void test_init_hash_table_null_hash_pointer()
{
    // Test that passing a NULL Fts3Hash pointer does not crash and returns SQLITE_OK
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TEST_ASSERT(db != nullptr && rc == SQLITE_OK, "sqlite3_open should succeed for in-memory DB (null-hash test)");
    if(db == nullptr || rc != SQLITE_OK) {
        return;
    }

    // Pass NULL as the tokenizer hash pointer; the code should still attempt registration
    rc = sqlite3Fts3InitHashTable(db, nullptr, "null_hash_tokenizer");
    // Depending on sqlite3 behavior, this should generally succeed for a valid DB and arity
    TEST_ASSERT(rc == SQLITE_OK, "sqlite3Fts3InitHashTable should handle NULL Fts3Hash pointer gracefully");

    sqlite3_close(db);
}

int main()
{
    std::cout << "Running sqlite3Fts3InitHashTable unit tests (no GTest)..." << std::endl;

    test_basic_init_hash_table();
    test_init_hash_table_null_hash_pointer();

    if(g_failures){
        std::cerr << "[OVERALL RESULT] " << g_failures << " test(s) failed." << std::endl;
        return 1;
    }else{
        std::cout << "[OVERALL RESULT] All tests passed." << std::endl;
        return 0;
    }
}