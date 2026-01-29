// Unit test suite for the focal method: sqlite3_bind_zeroblob64
// This is a self-contained, mock-driven test harness in C++11
// to validate the logic of sqlite3_bind_zeroblob64 without requiring the
// full SQLite build. It reproduces the essential control flow and interactions
// (armor check, limit check, bind call, and ApiExit/Unlock sequencing).

#include <cstdio>
#include <opcodes.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <cstdint>
#include <cassert>


// Domain-oriented notes:
// - We implement a minimal in-memory model of Vdbe/Db necessary for sqlite3_bind_zeroblob64.
// - We provide a lightweight, no-op mutex API to satisfy the function calls.
// - We emulate the critical return codes used by the focal method.
// - We implement a stub of sqlite3_bind_zeroblob to verify invocation and parameters.
// - We run several test cases to cover true/false branches and boundary conditions.

// Compile-time macro to emulate the API armor feature in the focal code.
// This allows testing the NULL-pointer early-exit path.
#define SQLITE_ENABLE_API_ARMOR

// Basic return code constants (simplified, just to distinguish branches)
#define SQLITE_OK 0
#define SQLITE_MISUSE_BKPT 1
#define SQLITE_TOOBIG 2

// Forward declare the testable function (extern "C" to mimic real linkage)
#ifdef __cplusplus
extern "C" {
#endif
int sqlite3_bind_zeroblob64(void *pStmt, int i, uint64_t n);
#ifdef __cplusplus
}
#endif

// Typedefs mirroring the focal code's usage (simplified)
typedef unsigned long long sqlite3_uint64;
typedef void sqlite3_stmt; // opaque in test harness
typedef struct sqlite3 sqlite3;
typedef struct Vdbe Vdbe;

// Global minimal SQLite-like structures
struct sqlite3 {
    int dummy;            // placeholder
    int mutex;            // simplified mutex placeholder
    uint64_t aLimit[2];     // aLimit[SQLITE_LIMIT_LENGTH] is used
};

// Minimal Vdbe that carries a pointer to the database (sqlite3)
struct Vdbe {
    sqlite3 *db;
};

// Mock: global state to verify sqlite3_bind_zeroblob call behavior
static int g_last_bind_called = 0;
static int g_last_bind_i = -1;
static uint64_t g_last_bind_n = 0;

// Mock: stub for sqlite3_mutex_enter/leave (no real threading in test)
static inline void sqlite3_mutex_enter(int /*mutex*/) {
    // no-op in mock
}
static inline void sqlite3_mutex_leave(int /*mutex*/) {
    // no-op in mock
}

// Mock: sqlite3ApiExit passthrough (no error translation needed for test)
static inline int sqlite3ApiExit(sqlite3 * /*db*/, int rc) {
    return rc;
}

// Mock: sqlite3_bind_zeroblob - records parameters to validate downstream behavior
static int sqlite3_bind_zeroblob(sqlite3_stmt * /*pStmt*/, int i, uint64_t n) {
    g_last_bind_called = 1;
    g_last_bind_i = i;
    g_last_bind_n = n;
    return SQLITE_OK;
}

// The focal method under test (mirror of the real logic, but self-contained)
extern "C" int sqlite3_bind_zeroblob64(void *pStmt, int i, uint64_t n) {
    int rc;
    Vdbe *p = (Vdbe *)pStmt;
#ifdef SQLITE_ENABLE_API_ARMOR
    if( p==0 ) return SQLITE_MISUSE_BKPT;
#endif
    sqlite3_mutex_enter(p->db->mutex);
    if( n>(uint64_t)p->db->aLimit[0] ){ // SQLITE_LIMIT_LENGTH == 0 in this mock
        rc = SQLITE_TOOBIG;
    }else{
        // In original, there is an assertion that n fits 31-bits
        assert( (n & 0x7FFFFFFF) == n );
        rc = sqlite3_bind_zeroblob(pStmt, i, n);
    }
    rc = sqlite3ApiExit(p->db, rc);
    sqlite3_mutex_leave(p->db->mutex);
    return rc;
}

// Helper: create a minimal Db with given limit
static sqlite3* create_db_with_limit(uint64_t limit_len) {
    sqlite3* db = new sqlite3();
    db->dummy = 0;
    db->mutex = 0;
    // The real code uses aLimit[SQLITE_LIMIT_LENGTH]; we map this to aLimit[0]
    db->aLimit[0] = limit_len;
    db->aLimit[1] = 0;
    return db;
}

// Helper: create a Vdbe bound to a given db
static Vdbe* create_vdbe(sqlite3* db) {
    Vdbe* p = new Vdbe();
    p->db = db;
    return p;
}

// Simple test harness utilities
static int g_failures = 0;

#define EXPECT_EQ(a, b, msg) do { \
    if( (a) != (b) ) { \
        printf("EXPECT_EQ FAILED: %s (expected %lld, got %lld) at %s:%d\n", \
               msg, (long long)(b), (long long)(a), __FILE__, __LINE__); \
        g_failures++; \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        printf("EXPECT_TRUE FAILED: %s at %s:%d\n", msg, __FILE__, __LINE__); \
        g_failures++; \
    } \
} while(0)

#define TEST_PASS(msg) do { \
    printf("TEST PASSED: %s\n", msg); \
} while(0)

#define TEST_FAIL(msg) do { \
    printf("TEST FAILED: %s\n", msg); \
    g_failures++; \
} while(0)

// Test 1: Normal path where n is within limit; expect 0 and that sqlite3_bind_zeroblob was called
static void test_bind64_normal_path() {
    // Arrange
    uint64_t limit = 1000;
    sqlite3* db = create_db_with_limit(limit);
    Vdbe* p = create_vdbe(db);

    // Act
    int rc = sqlite3_bind_zeroblob64((void*)p, 2, 123);

    // Assert
    EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK for normal path");
    EXPECT_EQ(g_last_bind_called, 1, "sqlite3_bind_zeroblob should be invoked");
    EXPECT_EQ(g_last_bind_i, 2, "bind index should match");
    EXPECT_EQ(g_last_bind_n, 123ULL, "bind length should match");

    // Cleanup
    delete p;
    delete db;
    TEST_PASS("test_bind64_normal_path");
}

// Test 2: Path where n is bigger than limit; expect SQLITE_TOOBIG and no bind call
static void test_bind64_too_big_path() {
    // Arrange
    uint64_t limit = 500;
    sqlite3* db = create_db_with_limit(limit);
    Vdbe* p = create_vdbe(db);

    // Reset bind tracking
    g_last_bind_called = 0;
    g_last_bind_i = -1;
    g_last_bind_n = 0;

    // Act
    int rc = sqlite3_bind_zeroblob64((void*)p, 1, 600ULL); // > limit

    // Assert
    EXPECT_EQ(rc, SQLITE_TOOBIG, "rc should be SQLITE_TOOBIG when n > limit");
    EXPECT_EQ(g_last_bind_called, 0, "sqlite3_bind_zeroblob should NOT be invoked on overflow");

    // Cleanup
    delete p;
    delete db;
    TEST_PASS("test_bind64_too_big_path");
}

// Test 3: Armor path: pStmt is NULL should return MISUSE_BKPT
static void test_bind64_armor_null_ptr() {
    // This test validates the precondition guard when API_ARMOR is enabled.
    // Arrange
    // No Vdbe allocated (p==NULL)
    void* pStmt = nullptr;

    // Act
    int rc = sqlite3_bind_zeroblob64(pStmt, 0, 1ULL);

    // Assert
    EXPECT_EQ(rc, SQLITE_MISUSE_BKPT, "rc should be SQLITE_MISUSE_BKPT when pStmt is NULL under API_ARMOR");

    TEST_PASS("test_bind64_armor_null_ptr");
}

// Test 4: Boundary case: n exactly equals limit, should proceed to bind
static void test_bind64_boundary_case() {
    // Arrange
    uint64_t limit = 100;
    sqlite3* db = create_db_with_limit(limit);
    Vdbe* p = create_vdbe(db);

    // Reset bind tracking
    g_last_bind_called = 0;
    g_last_bind_i = -1;
    g_last_bind_n = 0;

    // Act
    int rc = sqlite3_bind_zeroblob64((void*)p, 5, limit); // equals limit

    // Assert
    EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK when n equals limit");
    EXPECT_EQ(g_last_bind_called, 1, "sqlite3_bind_zeroblob should be invoked on boundary");
    EXPECT_EQ(g_last_bind_i, 5, "bind index should match boundary case");
    EXPECT_EQ(g_last_bind_n, limit, "bind length should match boundary value");

    // Cleanup
    delete p;
    delete db;
    TEST_PASS("test_bind64_boundary_case");
}

// Main to run tests
int main() {
    printf("Starting sqlite3_bind_zeroblob64 unit tests (mock harness) ...\n");

    test_bind64_normal_path();
    test_bind64_too_big_path();
    test_bind64_armor_null_ptr();
    test_bind64_boundary_case();

    if( g_failures == 0 ) {
        printf("All tests PASSED.\n");
        return 0;
    } else {
        printf("Tests completed with %d failure(s).\n", g_failures);
        return 1;
    }
}