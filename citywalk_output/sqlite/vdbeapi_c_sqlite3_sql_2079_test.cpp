// C++11 test suite for sqlite3_sql focal method
// This test targets the function:
//   const char *sqlite3_sql(sqlite3_stmt *pStmt)
// It relies on the project's Vdbe structure (zSql member) as defined in the
// included sqlite vdbe headers. The tests avoid GoogleTest and use a small
// in-house test harness with non-terminating assertions.

#include <opcodes.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Forward-declare the external C function under test.
// We assume the project provides the real implementation when compiling/linking.
extern "C" const char *sqlite3_sql(void *pStmt); // sqlite3_stmt is typically an opaque struct

// The actual project defines the types; we mirror the usage by casting a Vdbe
// instance to sqlite3_stmt* in tests. We include the project headers to get
// the real definitions when available.
extern "C" {
  // The real header in the project should expose:
  // typedef struct sqlite3_stmt sqlite3_stmt;
  // struct Vdbe { char *zSql; ... };
  // For testing in environments where headers are available, include them here.
  // If not available, the test relies on the fact that sqlite3_sql upcasts from Vdbe*.
}

// If the project provides the actual headers, include them.
// This is a best-effort include; it should be adapted to your build environment.
// #include "sqliteInt.h"
// #include "vdbeInt.h"
// #include "opcodes.h"

#ifdef __cplusplus
extern "C" {
#endif

// Provide a lightweight shim to the exact signature used by sqlite3_sql in the project.
// If the real signature in your build is 'const char *sqlite3_sql(sqlite3_stmt *pStmt)'
// you can adjust the typedef accordingly (sqlite3_stmt is typically an opaque struct).
// We declare a size-safe alias to allow casting from a Vdbe* in tests.
typedef struct sqlite3_stmt sqlite3_stmt;

#ifdef __cplusplus
}
#endif

// Simple in-house test harness (non-terminating assertions)
static int g_failures = 0;

static void expect_true(bool cond, const char* msg)
{
    if (!cond) {
        std::cerr << "EXPECT_FAIL: " << msg << std::endl;
        ++g_failures;
    }
}

static void test_sql_with_valid_vdbe_and_sql()
{
    // Focal scenario: pStmt points to a valid Vdbe with non-null zSql.
    // Arrange a minimal Vdbe with zSql member accessible.
    struct Vdbe {
        char *zSql;
        // ... other members are not needed for this test
    };

    // Prepare a C string buffer to survive the test's duration.
    char sqlbuf[] = "SELECT 1";
    Vdbe v;
    v.zSql = sqlbuf;

    // Cast Vdbe* to sqlite3_stmt* as the production API does.
    sqlite3_stmt *pStmt = (sqlite3_stmt *)&v;

    // Act
    const char *res = sqlite3_sql((void*)pStmt); // cast to void* to bypass potential prototype mismatch

    // Assert: should return the exact pointer to zSql
    expect_true(res == v.zSql, "sqlite3_sql should return the same zSql pointer for non-null pStmt with valid zSql.");
}

static void test_sql_with_valid_vdbe_and_null_sql()
{
    // Focal scenario: pStmt is non-null, but zSql is nullptr.
    struct Vdbe {
        char *zSql;
    };

    Vdbe v;
    v.zSql = nullptr;

    sqlite3_stmt *pStmt = (sqlite3_stmt *)&v;

    const char *res = sqlite3_sql((void*)pStmt);

    // Assert: should return null when zSql is null
    expect_true(res == nullptr, "sqlite3_sql should return nullptr when zSql is null.");
}

static void test_sql_with_null_pStmt()
{
    // Focal scenario: pStmt itself is null.
    const char *res = sqlite3_sql(nullptr);

    // Assert: should return nullptr for null input
    expect_true(res == nullptr, "sqlite3_sql should return nullptr when pStmt is NULL.");
}

static void test_sql_consistency_across_multiple_vdbes()
{
    // Additional sanity: ensure two distinct Vdbe instances yield their own zSql
    struct Vdbe {
        char *zSql;
    };

    char sql1[] = "SELECT 42";
    char sql2[] = "SELECT 7";

    Vdbe v1;
    v1.zSql = sql1;
    Vdbe v2;
    v2.zSql = sql2;

    sqlite3_stmt *pStmt1 = (sqlite3_stmt *)&v1;
    sqlite3_stmt *pStmt2 = (sqlite3_stmt *)&v2;

    const char *r1 = sqlite3_sql((void*)pStmt1);
    const char *r2 = sqlite3_sql((void*)pStmt2);

    // Assertions
    expect_true(r1 == v1.zSql, "sqlite3_sql should return correct zSql for first Vdbe.");
    expect_true(r2 == v2.zSql, "sqlite3_sql should return correct zSql for second Vdbe.");
    // Also ensure pointers are distinct unless strings are identical (they are distinct here)
    expect_true(r1 != r2, "zSql pointers for two different Vdbe instances should be distinct when their buffers are distinct.");
}

static void run_all_tests()
{
    test_sql_with_valid_vdbe_and_sql();
    test_sql_with_valid_vdbe_and_null_sql();
    test_sql_with_null_pStmt();
    test_sql_consistency_across_multiple_vdbes();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED: sqlite3_sql focal method" << std::endl;
    } else {
        std::cout << g_failures << " TEST(S) FAILED for sqlite3_sql focal method" << std::endl;
    }
}

int main()
{
    run_all_tests();
    return g_failures;
}