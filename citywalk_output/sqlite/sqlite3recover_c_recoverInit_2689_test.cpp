// High-quality C++11 unit test suite for the focal method: recoverInit
// This test suite targets sqlite3_recover *recoverInit(...) in sqlite3recover.c
// It uses no GTest and relies on a lightweight custom test harness.
// Notes:
// - Compile against SQLite3 (for sqlite3_malloc/sqlite3_free) and the sqlite3recover.h header.
// - Tests focus on correctness of recovered internal state after recoverInit is called.
// - The tests cover NULL and non-NULL zDb, NULL and non-NULL zUri, and propagation of callbacks/context.

#include <string.h>
#include <assert.h>
#include <sqlite3recover.h>
#include <sqlite3.h>
#include <cstring>
#include <iostream>
#include <cstdint>


// Import C interfaces
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;
#define TEST_EXPECT_TRUE(cond, msg) \
  do { \
    if(!(cond)) { \
      std::cerr << "[TEST_FAIL] " << msg << std::endl; \
      ++g_failures; \
    } \
  } while(0)

#define TEST_EXPECT_EQ_INT(a, b, msg) \
  do { \
    if((int)(a) != (int)(b)) { \
      std::cerr << "[TEST_FAIL] " << msg \
                << " (actual=" << (int)(a) << ", expected=" << (int)(b) << ")" \
                << std::endl; \
      ++g_failures; \
    } \
  } while(0)

#define TEST_EXPECT_EQ_PTR(a, b, msg) \
  do { \
    if((void*)(a) != (void*)(b)) { \
      std::cerr << "[TEST_FAIL] " << msg \
                << " (actual=" << a << ", expected=" << b << ")" \
                << std::endl; \
      ++g_failures; \
    } \
  } while(0)

#define TEST_EXPECT_STR(a, b, msg) \
  do { \
    if(((a) == nullptr) || strcmp((a), (b)) != 0) { \
      std::cerr << "[TEST_FAIL] " << msg \
                << " (actual=" << (a ? a : "NULL") << ", expected=" << (b) << ")" \
                << std::endl; \
      ++g_failures; \
    } \
  } while(0)


// A C linkage function to be used as xSql callback in tests
extern "C" int testSqlCallback(void* ctx, const char* sql) {
    // Simple stub used only to verify pointer propagation
    (void)ctx;
    (void)sql;
    return 0;
}


// Test suite for recoverInit
class RecoverInitTests {
public:
    static void RunAll() {
        test_ZDbNull_ZUriNonNull();
        test_ZDbNonNull_ZUriNull();
        test_ZDbNonNull_ZUriNonNull();
        test_CallbackPropagation();
        test_DefaultBRecoverRowid();
        summarize();
    }

private:
    // Test 1: zDb is NULL -> should default to "main" and copy zUri
    static void test_ZDbNull_ZUriNonNull() {
        sqlite3* db = nullptr;
        const char* zUri = "uri_example";
        sqlite3_recover* p = recoverInit(db, nullptr, zUri, nullptr, nullptr);

        TEST_EXPECT_TRUE(p != nullptr, "recoverInit should return non-null when zDb is NULL");
        if(p){
            TEST_EXPECT_STR(p->zDb, "main", "Default zDb should be 'main' when zDb is NULL");
            TEST_EXPECT_STR(p->zUri, zUri, "zUri should be copied when provided");
            TEST_EXPECT_EQ_PTR(p->dbIn, db, "dbIn should reflect the input db pointer");
            TEST_EXPECT_EQ_PTR(p->xSql, nullptr, "xSql should be preserved when NULL is passed");
            TEST_EXPECT_EQ_PTR(p->pSqlCtx, nullptr, "pSqlCtx should be preserved when NULL is passed");
            TEST_EXPECT_TRUE(p->bRecoverRowid == RECOVER_ROWID_DEFAULT,
                             "bRecoverRowid should be set to RECOVER_ROWID_DEFAULT");
            sqlite3_free(p);
        }
    }

    // Test 2: zDb non-NULL, zUri NULL -> zDb should be preserved, zUri should be empty string
    static void test_ZDbNonNull_ZUriNull() {
        sqlite3* db = nullptr;
        const char* zDb = "testdb";
        sqlite3_recover* p = recoverInit(db, zDb, nullptr, nullptr, nullptr);

        TEST_EXPECT_TRUE(p != nullptr, "recoverInit should not return NULL for normal zDb");
        if(p){
            TEST_EXPECT_STR(p->zDb, zDb, "zDb should be preserved when non-NULL");
            // Since zUri is NULL, recoverStrlen(NULL) should yield 0; zUri should be an empty string
            TEST_EXPECT_TRUE(p->zUri != nullptr && p->zUri[0] == '\0',
                             "zUri should be empty string when NULL zUri is provided");
            sqlite3_free(p);
        }
    }

    // Test 3: Both zDb and zUri non-NULL -> both should be copied correctly
    static void test_ZDbNonNull_ZUriNonNull() {
        sqlite3* db = nullptr;
        const char* zDb = "db_name";
        const char* zUri = "recover_uri";
        sqlite3_recover* p = recoverInit(db, zDb, zUri, nullptr, nullptr);

        TEST_EXPECT_TRUE(p != nullptr, "recoverInit should return non-null for valid inputs");
        if(p){
            TEST_EXPECT_STR(p->zDb, zDb, "zDb should be copied exactly as provided");
            TEST_EXPECT_STR(p->zUri, zUri, "zUri should be copied exactly as provided");
            sqlite3_free(p);
        }
    }

    // Test 4: xSql callback and pSqlCtx propagation
    static void test_CallbackPropagation() {
        sqlite3* db = nullptr;
        const char* zDb = "cb_db";
        const char* zUri = "cb_uri";

        sqlite3_recover* p = recoverInit(db, zDb, zUri, testSqlCallback, reinterpret_cast<void*>(0xDEADBEEF));

        TEST_EXPECT_TRUE(p != nullptr, "recoverInit should allocate for callback propagation");
        if(p){
            TEST_EXPECT_EQ_PTR(p->xSql, testSqlCallback, "xSql callback should be propagated");
            TEST_EXPECT_EQ_PTR(p->pSqlCtx, reinterpret_cast<void*>(0xDEADBEEF),
                               "pSqlCtx should be propagated with the correct value");
            sqlite3_free(p);
        }
    }

    // Test 5: bRecoverRowid default value remains set
    static void test_DefaultBRecoverRowid() {
        sqlite3* db = nullptr;
        sqlite3_recover* p = recoverInit(db, "dbX", "uriX", nullptr, nullptr);

        TEST_EXPECT_TRUE(p != nullptr, "recoverInit should not fail for default value test");
        if(p){
            TEST_EXPECT_TRUE(p->bRecoverRowid == RECOVER_ROWID_DEFAULT,
                             "bRecoverRowid should equal RECOVER_ROWID_DEFAULT");
            sqlite3_free(p);
        }
    }

    // Helper to print a final summary
    static void summarize() {
        std::cout << "[SUMMARY] Failures: " << g_failures << std::endl;
    }
};

// Entry point for tests
int main() {
    std::cout << "Starting sqlite3_recoverInit unit tests (C++11, no GTest)..." << std::endl;
    RecoverInitTests::RunAll();

    if(g_failures > 0){
        std::cerr << "Some tests FAILED. Failures: " << g_failures << std::endl;
        return 1;
    }else{
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}