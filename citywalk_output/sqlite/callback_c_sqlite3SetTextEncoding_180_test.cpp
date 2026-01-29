// Unit tests for sqlite3SetTextEncoding (callback.c) using a lightweight, GTest-free approach
// This test suite is written for C++11 and avoids GTest; it uses simple runtime assertions
// and prints PASS/FAIL messages. It mocks the relevant SQLite internals to exercise the
// behavior of sqlite3SetTextEncoding without requiring the full SQLite runtime.
//
// Test strategy:
// - Provide a minimal sqlite3-like struct with only the fields used by sqlite3SetTextEncoding.
// - Provide mock implementations for sqlite3FindCollSeq, sqlite3ExpirePreparedStatements, and
//   a definition for sqlite3StrBINARY to satisfy linking.
// - Call sqlite3SetTextEncoding with each supported encoding (UTF8, UTF16LE, UTF16BE) and verify:
//     * db->enc is updated
//     * db->pDfltColl points to the expected CollSeq instance
//     * sqlite3ExpirePreparedStatements is invoked exactly once
//     * sqlite3FindCollSeq is invoked with the correct enc and zName (sqlite3StrBINARY)

#include <cstdio>
#include <sqliteInt.h>
#include <cstdint>
#include <cstring>


// 1) Domain-specific constants (matching the focal method's expectations)
enum { SQLITE_UTF8 = 1, SQLITE_UTF16LE = 2, SQLITE_UTF16BE = 3 }; // encoding constants

// 2) Lightweight type/struct definitions to mirror the portion of sqlite3 used by sqlite3SetTextEncoding
typedef unsigned char u8;

typedef struct sqlite3 sqlite3;
struct sqlite3 {
    u8 enc;
    void *pDfltColl;
};

// 3) Minimal CollSeq placeholder (the test only checks pointer identity)
typedef struct CollSeq CollSeq;

// 4) Forward declarations of the function under test and the dependencies (to be resolved by linking with callback.c)
extern "C" void sqlite3SetTextEncoding(sqlite3 *db, u8 enc);

// 5) Mocks and test scaffolding (C linkage to match the actual SQLite C API)
extern "C" {
    // Global symbol used by sqlite3SetTextEncoding when passing the default collating sequence
    // We provide a symbol definition to satisfy linkage with callback.c (which uses sqlite3StrBINARY)
    const char sqlite3StrBINARY[];
}

// 6) Mock storage for sentinel values to verify correct interactions
static CollSeq collUtf8;
static CollSeq collUtf16LE;
static CollSeq collUtf16BE;

// Track last parameters passed to sqlite3FindCollSeq for verification
static u8 lastFindEnc;
static const char *lastZName;

// Track how many times sqlite3ExpirePreparedStatements was called
static int expireCalls;

// 7) Mock implementation of sqlite3FindCollSeq
// Returns a pointer to a per-encoding CollSeq instance to allow identity checks
extern "C" CollSeq* sqlite3FindCollSeq(sqlite3 *db, u8 enc, const char *zName, int create) {
    lastFindEnc = enc;
    lastZName = zName;

    // Simulate behavior: return a distinct CollSeq per encoding
    if (enc == SQLITE_UTF8) {
        return &collUtf8;
    } else if (enc == SQLITE_UTF16LE) {
        return &collUtf16LE;
    } else if (enc == SQLITE_UTF16BE) {
        return &collUtf16BE;
    } else {
        return nullptr;
    }
}

// 8) Mock implementation of sqlite3ExpirePreparedStatements
extern "C" void sqlite3ExpirePreparedStatements(sqlite3 *db, int N){
    (void)db; // unused in test
    (void)N;
    expireCalls++;
}

// 9) Provide actual storage for sqlite3StrBINARY symbol used by the code under test
const char sqlite3StrBINARY[] = "BINARY";

// 10) Lightweight assertion helper and test harness
static int failures = 0;

#define TEST_PASS(msg) do { printf("PASS: %s\n", msg); } while(0)
#define TEST_FAIL(msg) do { printf("FAIL: %s\n", msg); ++failures; } while(0)
#define ASSERT_EQ(actual, expected, msg) do { if ((actual) == (expected)) { TEST_PASS(msg); } else { printf("  Expected: %s, Got: %p\n", #expected, (void*)(actual)); TEST_FAIL(msg); } } while(0)
#define ASSERT_PTR_EQ(actual, expected, msg) do { if ((actual) == (void*)(expected)) { TEST_PASS(msg); } else { printf("  Expected pointer: %p, Got: %p\n", (void*)(expected), (void*)(actual)); TEST_FAIL(msg); } } while(0)

// 11) The actual test cases

// Test 1: UTF8 encoding sets db->enc, updates default coll sequence to collUtf8, and expires prepared statements
void test_utf8_encoding_sets_fields() {
    // Arrange
    sqlite3 db;
    db.enc = 0;
    db.pDfltColl = nullptr;
    expireCalls = 0;
    lastFindEnc = 0;
    lastZName = nullptr;

    // Act
    sqlite3SetTextEncoding(&db, SQLITE_UTF8);

    // Assert
    ASSERT_EQ(db.enc, SQLITE_UTF8, "db.enc should be set to SQLITE_UTF8");
    ASSERT_PTR_EQ(db.pDfltColl, &collUtf8, "db.pDfltColl should point to collUtf8 for UTF-8 encoding");
    if (expireCalls != 1) {
        printf("  expireCalls expected 1, got %d\n", expireCalls);
        TEST_FAIL("sqlite3ExpirePreparedStatements should be called exactly once for UTF-8");
    } else {
        TEST_PASS("sqlite3ExpirePreparedStatements called exactly once for UTF-8");
    }
    ASSERT_EQ(lastFindEnc, SQLITE_UTF8, "sqlite3FindCollSeq should be invoked with enc = SQLITE_UTF8");
    ASSERT_PTR_EQ(lastZName, sqlite3StrBINARY, "sqlite3FindCollSeq should be called with zName = sqlite3StrBINARY");
}

// Test 2: UTF16LE encoding sets db->enc, updates default coll sequence to collUtf16LE, and expires prepared statements
void test_utf16le_encoding_sets_fields() {
    // Arrange
    sqlite3 db;
    db.enc = 0;
    db.pDfltColl = nullptr;
    expireCalls = 0;
    lastFindEnc = 0;
    lastZName = nullptr;

    // Act
    sqlite3SetTextEncoding(&db, SQLITE_UTF16LE);

    // Assert
    ASSERT_EQ(db.enc, SQLITE_UTF16LE, "db.enc should be set to SQLITE_UTF16LE");
    ASSERT_PTR_EQ(db.pDfltColl, &collUtf16LE, "db.pDfltColl should point to collUtf16LE for UTF-16LE encoding");
    if (expireCalls != 1) {
        printf("  expireCalls expected 1, got %d\n", expireCalls);
        TEST_FAIL("sqlite3ExpirePreparedStatements should be called exactly once for UTF-16LE");
    } else {
        TEST_PASS("sqlite3ExpirePreparedStatements called exactly once for UTF-16LE");
    }
    ASSERT_EQ(lastFindEnc, SQLITE_UTF16LE, "sqlite3FindCollSeq should be invoked with enc = SQLITE_UTF16LE");
    ASSERT_PTR_EQ(lastZName, sqlite3StrBINARY, "sqlite3FindCollSeq should be called with zName = sqlite3StrBINARY (UTF-16LE)");
}

// Test 3: UTF16BE encoding sets db->enc, updates default coll sequence to collUtf16BE, and expires prepared statements
void test_utf16be_encoding_sets_fields() {
    // Arrange
    sqlite3 db;
    db.enc = 0;
    db.pDfltColl = nullptr;
    expireCalls = 0;
    lastFindEnc = 0;
    lastZName = nullptr;

    // Act
    sqlite3SetTextEncoding(&db, SQLITE_UTF16BE);

    // Assert
    ASSERT_EQ(db.enc, SQLITE_UTF16BE, "db.enc should be set to SQLITE_UTF16BE");
    ASSERT_PTR_EQ(db.pDfltColl, &collUtf16BE, "db.pDfltColl should point to collUtf16BE for UTF-16BE encoding");
    if (expireCalls != 1) {
        printf("  expireCalls expected 1, got %d\n", expireCalls);
        TEST_FAIL("sqlite3ExpirePreparedStatements should be called exactly once for UTF-16BE");
    } else {
        TEST_PASS("sqlite3ExpirePreparedStatements called exactly once for UTF-16BE");
    }
    ASSERT_EQ(lastFindEnc, SQLITE_UTF16BE, "sqlite3FindCollSeq should be invoked with enc = SQLITE_UTF16BE");
    ASSERT_PTR_EQ(lastZName, sqlite3StrBINARY, "sqlite3FindCollSeq should be called with zName = sqlite3StrBINARY (UTF-16BE)");
}

// 12) Main entry point to run tests
int main() {
    // Run all tests
    printf("Starting sqlite3SetTextEncoding unit tests...\n");

    test_utf8_encoding_sets_fields();
    test_utf16le_encoding_sets_fields();
    test_utf16be_encoding_sets_fields();

    // Summary
    if (failures == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("TOTAL FAILURES: %d\n", failures);
    }

    // Return non-zero if any test failed (helps integrate with some CI systems)
    return failures;
}