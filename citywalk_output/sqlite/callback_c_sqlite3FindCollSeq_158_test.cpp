// Unit test suite for sqlite3FindCollSeq
// This test is designed to be run without GoogleTest.
// It relies on the focal implementation embedded (callback.c) and uses a lightweight, non-terminating assertion approach.
// The tests cover the key decision branches in sqlite3FindCollSeq according to Step 2/Step 3 guidelines.

#include <callback.c>
#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Bring in the focal implementation so that sqlite3FindCollSeq and related types exist.
// We assume the environment places the internal SQLite types in a compatible way when compiling the test.
// The test uses C linkage for the FOCAL function.
extern "C" {
}

// Also declare the public prototype for sqlite3FindCollSeq with C linkage.
// We rely on the actual callback.c to provide the concrete implementation.
extern "C" CollSeq *sqlite3FindCollSeq(
  sqlite3 *db,          /* Database connection to search */
  u8 enc,               /* Desired text encoding */
  const char *zName,    /* Name of the collating sequence.  Might be NULL */
  int create            /* True to create CollSeq if doesn't already exist */
);

// Lightweight non-terminating test harness
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << " (" << #cond << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        std::cerr << "[FAIL] " << (msg) << " Expected: " << (void*)(a) << " == " << (void*)(b) << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_NULL(p, msg) do { \
    if((p) != nullptr) { \
        std::cerr << "[FAIL] " << (msg) << " Expected NULL, got " << (void*)(p) << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_NOT_NULL(p, msg) do { \
    if((p) == nullptr) { \
        std::cerr << "[FAIL] " << (msg) << " Expected non-NULL" << std::endl; \
        ++g_failures; \
    } \
} while(0)


// Helper to create a minimal sqlite3 object footprint for testing.
// Since sqlite3FindCollSeq only accesses db->pDfltColl, we only need that field to be valid.
static sqlite3* make_test_db_with_default_coll(CollSeq* defaultColl) {
    sqlite3* db = (sqlite3*)std::malloc(sizeof(sqlite3));
    if(!db) return nullptr;
    std::memset(db, 0, sizeof(sqlite3));
    db->pDfltColl = defaultColl;
    return db;
}

// Test 1: zName == NULL should return the default collision sequence (db->pDfltColl)
static void test_sqlite3FindCollSeq_null_name_returns_default() {
    // Arrange
    CollSeq* dummyDefault = (CollSeq*)std::malloc(sizeof(CollSeq));
    std::memset(dummyDefault, 0, sizeof(CollSeq));

    sqlite3* db = make_test_db_with_default_coll(dummyDefault);
    EXPECT_NOT_NULL(db, "Test 1: db allocation failed");

    // Act
    CollSeq* result = sqlite3FindCollSeq(db, SQLITE_UTF8, NULL, 0);

    // Assert
    EXPECT_EQ(result, dummyDefault, "sqlite3FindCollSeq with NULL zName should return default coll sequence pointer");
    // Cleanup
    std::free(dummyDefault);
    std::free(db);
}

// Test 2: zName non-NULL but unknown coll name should return NULL
static void test_sqlite3FindCollSeq_unknown_name_returns_null() {
    // Arrange
    CollSeq* dummyDefault = (CollSeq*)std::malloc(sizeof(CollSeq));
    std::memset(dummyDefault, 0, sizeof(CollSeq));

    sqlite3* db = make_test_db_with_default_coll(dummyDefault);
    EXPECT_NOT_NULL(db, "Test 2: db allocation failed");

    // Use a clearly unknown coll name
    const char* unknownName = "ThisNameShouldNotExist12345";

    // Act
    CollSeq* result = sqlite3FindCollSeq(db, SQLITE_UTF8, unknownName, 0);

    // Assert
    EXPECT_NULL(result, "sqlite3FindCollSeq with unknown zName should return NULL");
    // Cleanup
    std::free(dummyDefault);
    std::free(db);
}

// Test 3: zName non-NULL and known coll name should return a non-NULL pointer
// This exercises the path that calls findCollSeqEntry and offsets by enc-1.
// It also asserts that we do not regress to NULL for a known name.
static void test_sqlite3FindCollSeq_known_name_returns_non_null() {
    // Arrange
    CollSeq* dummyDefault = (CollSeq*)std::malloc(sizeof(CollSeq));
    std::memset(dummyDefault, 0, sizeof(CollSeq));

    sqlite3* db = make_test_db_with_default_coll(dummyDefault);
    EXPECT_NOT_NULL(db, "Test 3: db allocation failed");

    // Choose a name that is expected to exist in SQLite's built-ins.
    const char* knownName = "NOCASE";

    // Act
    CollSeq* result = sqlite3FindCollSeq(db, SQLITE_UTF8, knownName, 0);

    // Assert
    EXPECT_NOT_NULL(result, "sqlite3FindCollSeq with a known zName should not return NULL");
    // Most likely the returned pointer is not equal to the default pointer,
    // due to the pointer arithmetic (enc-1). This is a best-effort check for a non-default pointer.
    EXPECT_TRUE(result != dummyDefault, "Result should not be the same as the default coll sequence for a known name");
    // Cleanup
    std::free(dummyDefault);
    std::free(db);
}


int main() {
    std::cout << "Starting sqlite3FindCollSeq unit tests (non-GTest)..." << std::endl;

    test_sqlite3FindCollSeq_null_name_returns_default();
    test_sqlite3FindCollSeq_unknown_name_returns_null();
    test_sqlite3FindCollSeq_known_name_returns_non_null();

    if(g_failures == 0) {
        std::cout << "All tests passed (non-terminating assertions)." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}