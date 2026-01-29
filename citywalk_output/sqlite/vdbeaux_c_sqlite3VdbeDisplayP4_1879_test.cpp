// C++ test harness for sqlite3VdbeDisplayP4
// This test suite exercises the focal method using the actual internal
// SQLite structures. It avoids Google Test and uses a lightweight
//, non-terminating assertion macro style with explanatory comments.
// Compile with the SQLite source tree available (C headers + internal VDBe
// headers).

#include <cstdio>
#include <sqlite3.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <cstdlib>
#include <cstdint>


extern "C" {
}

// Lightweight test macro helpers
#define TEST_PASS(name)  do { printf("[PASS] %s\n", (name)); } while(0)
#define TEST_FAIL(name, msg) do { printf("[FAIL] %s: %s\n", (name), (msg)); return false; } while(0)
#define ASSERT_STR_EQ(expected, actual, testName) do { \
  if((actual) == NULL) { TEST_FAIL(testName, "Actual string is NULL"); } \
  else if(strcmp((expected), (actual)) != 0) { \
    char buf[512]; snprintf(buf, sizeof(buf), "Expected '%s', got '%s'", (expected), (actual)); \
    TEST_FAIL(testName, buf); \
  } else { TEST_PASS(testName); } \
} while(0)

#define ASSERT_STR_CONTAINS(substr, actual, testName) do { \
  if((actual) == NULL) { TEST_FAIL(testName, "Actual string is NULL"); } \
  else if(strstr((actual), (substr)) == NULL) { \
    char buf[512]; snprintf(buf, sizeof(buf), "Expected substring '%s' in '%s'", (substr), (actual)); \
    TEST_FAIL(testName, buf); \
  } else { TEST_PASS(testName); } \
} while(0)

static bool test_keyinfo_display() {
  // Test P4_KEYINFO path: k(n, [flags][collName]...) with a BINARY coll replaced by B
  // Expected: k(1,-N.B)
  // Setup:
  // - nKeyField = 1
  // - aSortFlags[0] has DESC and BIGNULL
  // - aColl[0] has zName = "BINARY" to trigger replacement to "B"
  sqlite3 *db = nullptr;
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    printf("[ERR] Unable to open in-memory DB for KEYINFO test\n");
    return false;
  }

  Op op;
  memset(&op, 0, sizeof(op));
  op.p4type = P4_KEYINFO;

  // KeyInfo setup
  KeyInfo ki;
  CollSeq coll;
  CollSeq *aColl[1] = { &coll };
  unsigned char aSortFlags[1] = { (unsigned char)(KEYINFO_ORDER_DESC | KEYINFO_ORDER_BIGNULL) };
  ki.nKeyField = 1;
  ki.aSortFlags = aSortFlags;
  ki.aColl = aColl;
  coll.zName = "BINARY";
  coll.enc = 0;
  ki.aSortFlags[0] = aSortFlags[0];
  ki.aColl[0] = &coll;
  op.p4.pKeyInfo = &ki;

  char *z = sqlite3VdbeDisplayP4(db, &op);
  if (z == NULL) {
    sqlite3_close(db);
    TEST_FAIL("KEYINFO", "sqlite3VdbeDisplayP4 returned NULL");
    return false;
  }

  // Expect "k(1,-N.B)"
  ASSERT_STR_EQ("k(1,-N.B)", z, "KEYINFO");
  sqlite3_free(z);
  sqlite3_close(db);
  TEST_PASS("KEYINFO final string exact match");
  return true;
}

static bool test_collseq_display() {
  // Test P4_COLLSEQ path: zName-enc
  // Expected: "UNICODE-16LE" (enc=2 -> encnames[2] == "16LE")
  sqlite3 *db = nullptr;
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    printf("[ERR] Unable to open in-memory DB for COLLSEQ test\n");
    return false;
  }

  Op op;
  memset(&op, 0, sizeof(op));
  op.p4type = P4_COLLSEQ;

  CollSeq coll;
  coll.zName = "UNICODE";
  coll.enc = 2; // 2 -> "16LE"

  op.p4.pColl = &coll;

  char *z = sqlite3VdbeDisplayP4(db, &op);
  if (z == NULL) {
    sqlite3_close(db);
    TEST_FAIL("COLLSEQ", "null result");
    return false;
  }

  // Expect "UNICODE-16LE"
  ASSERT_STR_EQ("UNICODE-16LE", z, "COLLSEQ");
  sqlite3_free(z);
  sqlite3_close(db);
  TEST_PASS("COLLSEQ exact string");
  return true;
}

static bool test_intarray_display() {
  // Test P4_INTARRAY path: [count, v1, v2, ...]
  // Setup: ai = {2, 10, 20}
  sqlite3 *db = nullptr;
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    printf("[ERR] Unable to open in-memory DB for INTARRAY test\n");
    return false;
  }

  Op op;
  memset(&op, 0, sizeof(op));
  op.p4type = P4_INTARRAY;

  static uint32_t ai[3] = {2, 10, 20};
  op.p4.ai = ai;

  char *z = sqlite3VdbeDisplayP4(db, &op);
  if (z == NULL) {
    sqlite3_close(db);
    TEST_FAIL("INTARRAY", "null result");
    return false;
  }

  // Expect "[10,20]"
  ASSERT_STR_EQ("[10,20]", z, "INTARRAY");
  sqlite3_free(z);
  sqlite3_close(db);
  TEST_PASS("INTARRAY exact string");
  return true;
}

static bool test_int32_and_int64_and_real_and_default() {
  sqlite3 *db = nullptr;
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    printf("[ERR] Unable to open in-memory DB for NUM tests\n");
    return false;
  }

  // Test P4_INT32
  {
    Op op;
    memset(&op, 0, sizeof(op));
    op.p4type = P4_INT32;
    op.p4.i = 42; // direct integer value
    char *z = sqlite3VdbeDisplayP4(db, &op);
    if (z == NULL) { sqlite3_close(db); TEST_FAIL("INT32", "null result"); return false; }
    ASSERT_STR_EQ("42", z, "INT32");
    sqlite3_free(z);
  }

  // Test P4_INT64
  {
    Op op;
    memset(&op, 0, sizeof(op));
    op.p4type = P4_INT64;
    static i64 val64 = 1234567890123LL;
    op.p4.pI64 = &val64;
    char *z = sqlite3VdbeDisplayP4(db, &op);
    if (z == NULL) { sqlite3_close(db); TEST_FAIL("INT64", "null result"); return false; }
    ASSERT_STR_EQ("1234567890123", z, "INT64");
    sqlite3_free(z);
  }

  // Test P4_REAL
  {
    Op op;
    memset(&op, 0, sizeof(op));
    op.p4type = P4_REAL;
    static double d = 3.14159265358979323846;
    op.p4.pReal = &d;
    char *z = sqlite3VdbeDisplayP4(db, &op);
    if (z == NULL) { sqlite3_close(db); TEST_FAIL("REAL", "null result"); return false; }
    // Should include 3.14159... in 16 significant digits. We check for prefix.
    ASSERT_STR_CONTAINS("3.14159", z, "REAL");
    sqlite3_free(z);
  }

  // Test P4_DEFAULT
  {
    Op op;
    memset(&op, 0, sizeof(op));
    op.p4type = 0; // default path
    op.p4.z = "default";
    char *z = sqlite3VdbeDisplayP4(db, &op);
    if (z == NULL) { sqlite3_close(db); TEST_FAIL("DEFAULT", "null result"); return false; }
    ASSERT_STR_EQ("default", z, "DEFAULT");
    sqlite3_free(z);
  }

  sqlite3_close(db);
  TEST_PASS("NUM tests (INT32, INT64, REAL, DEFAULT)");
  return true;
}

static bool test_p4_table_and_func_and_ctx() {
  // Test P4_TABLE, P4_FUNCDEF, P4_FUNCCTX paths
  sqlite3 *db = nullptr;
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    printf("[ERR] Unable to open in-memory DB for TABLE/FUNC tests\n");
    return false;
  }

  // P4_TABLE
  {
    Op op;
    memset(&op, 0, sizeof(op));
    op.p4type = P4_TABLE;
    static struct TableStub { const char *zName; } tbl;
    tbl.zName = "mytable";
    op.p4.pTab = &tbl; // use internal Table struct from vdbe; assume compatible
    char *z = sqlite3VdbeDisplayP4(db, &op);
    if (z == NULL) { sqlite3_close(db); TEST_FAIL("TABLE", "null result"); return false; }
    ASSERT_STR_EQ("mytable", z, "TABLE");
    sqlite3_free(z);
  }

  // P4_FUNCDEF
  {
    Op op;
    memset(&op, 0, sizeof(op));
    op.p4type = P4_FUNCDEF;
    static FuncDef f;
    f.zName = "myFunc";
    f.nArg = 2;
    op.p4.pFunc = &f;
    char *z = sqlite3VdbeDisplayP4(db, &op);
    if (z == NULL) { sqlite3_close(db); TEST_FAIL("FUNCDEF", "null result"); return false; }
    ASSERT_STR_EQ("myFunc(2)", z, "FUNCDEF");
    sqlite3_free(z);
  }

  // P4_FUNCCTX
  {
    Op op;
    memset(&op, 0, sizeof(op));
    op.p4type = P4_FUNCCTX;
    static FuncDef f;
    f.zName = "ctxFunc";
    f.nArg = 1;
    static struct { FuncDef *pFunc; } ctx;
    ctx.pFunc = &f;
    op.p4.pCtx = &ctx;
    char *z = sqlite3VdbeDisplayP4(db, &op);
    if (z == NULL) { sqlite3_close(db); TEST_FAIL("FUNCCTX", "null result"); return false; }
    ASSERT_STR_EQ("ctxFunc(1)", z, "FUNCCTX");
    sqlite3_free(z);
  }

  sqlite3_close(db);
  TEST_PASS("TABLE/FUNCDEF/FUNCCTX paths");
  return true;
}

static bool test_p4_default_with_z() {
  // Additional test: default path with z field and append
  sqlite3 *db = nullptr;
  if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
    printf("[ERR] Unable to open in-memory DB for DEFAULT-Z test\n");
    return false;
  }
  Op op;
  memset(&op, 0, sizeof(op));
  op.p4type = 0;
  op.p4.z = "default_string";
  char *z = sqlite3VdbeDisplayP4(db, &op);
  if (z == NULL) { sqlite3_close(db); TEST_FAIL("DEFAULT-Z", "null result"); return false; }
  ASSERT_STR_EQ("default_string", z, "DEFAULT-Z");
  sqlite3_free(z);
  sqlite3_close(db);
  TEST_PASS("DEFAULT with z string");
  return true;
}

int main() {
  int total = 0, passed = 0;

  printf("Starting sqlite3VdbeDisplayP4 test suite (C++ harness, no GTest)\n");

  if(test_keyinfo_display()) {
    passed++;
  }
  total++;

  if(test_collseq_display()) {
    passed++;
  }
  total++;

  if(test_intarray_display()) {
    passed++;
  }
  total++;

  if(test_int32_and_int64_and_real_and_default()) {
    passed++;
  }
  total++;

  if(test_p4_table_and_func_and_ctx()) {
    passed++;
  }
  total++;

  if(test_p4_default_with_z()) {
    passed++;
  }
  total++;

  printf("Tests run: %d, Passed: %d, Failed: %d\n", total, passed, total - passed);
  return (passed == total) ? 0 : 1;
}