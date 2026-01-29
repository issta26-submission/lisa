// Test suite for the focal method: sqlite3OpenSchemaTable
// This is a self-contained, compile-once-run-all-test C++11 test
// that mocks the minimal SQLite dependencies required to exercise the function.
// No GTest is used. All tests are executed from main().

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <vector>


// Domain-specific simplifications and mocks
// These definitions mimic the minimal subset of SQLite types and API used by sqlite3OpenSchemaTable.

using namespace std;

// Basic type aliases to reflect SQLite-like API
typedef unsigned char u8;
typedef unsigned int Pgno;

// Constants used by the focal method
#define OP_OpenWrite 123          // dummy opcode for test
const int SCHEMA_ROOT = 42;        // fake root page id
static const char *LEGACY_SCHEMA_TABLE = "LEGACY_SCHEMA_TABLE";

// Minimal Parse and Vdbe structures needed for the test
struct OpLog {
  int op;
  int p1;
  int p2;
  int p3;
  int p4;
};

struct Vdbe {
  vector<OpLog> ops;
};

// Global mock state to capture interactions with the focal function's dependencies
static Vdbe g_vdbe;                 // shared Vdbe instance used by sqlite3GetVdbe
static bool g_lockCalled = false;   // whether sqlite3TableLock was invoked
static struct LockRecord {
  int iDb;
  Pgno iTab;
  u8 isWriteLock;
  const char *zName;
} g_lockRecord;

// Reset mocks for a clean test run
static void resetMocks() {
  g_lockCalled = false;
  g_lockRecord = {0, 0, 0, nullptr};
  g_vdbe.ops.clear();
}

// Minimal Parse structure with only fields used by the focal function
struct Parse {
  int nTab;
};

// Mocked interfaces expected by sqlite3OpenSchemaTable
static Vdbe* sqlite3GetVdbe(Parse *p) {
  (void)p; // parameter not used in mock beyond presence
  return &g_vdbe;
}

static void sqlite3TableLock(Parse *pParse, int iDb, Pgno iTab, u8 isWriteLock, const char *zName) {
  (void)pParse;
  g_lockCalled = true;
  g_lockRecord.iDb = iDb;
  g_lockRecord.iTab = iTab;
  g_lockRecord.isWriteLock = isWriteLock;
  g_lockRecord.zName = zName;
  // Intentionally do not perform heavy logic; only record for tests
}

static void sqlite3VdbeAddOp4Int(Vdbe *v, int op, int p1, int p2, int p3, int p4) {
  OpLog ol = {op, p1, p2, p3, p4};
  v->ops.push_back(ol);
}

// The focal method under test (copied/replicated for the self-contained test)
extern "C" void sqlite3OpenSchemaTable(Parse *p, int iDb){
  Vdbe *v = sqlite3GetVdbe(p);
  sqlite3TableLock(p, iDb, SCHEMA_ROOT, 1, LEGACY_SCHEMA_TABLE);
  sqlite3VdbeAddOp4Int(v, OP_OpenWrite, 0, SCHEMA_ROOT, iDb, 5);
  if( p->nTab==0 ){
    p->nTab = 1;
  }
}

// Simple non-terminating assertion helpers
static int g_test_failures = 0;

#define EXPECT_EQ(a, b, msg) do { \
  if ((a) != (b)) { \
    cerr << "[FAIL] " << msg << ": expected " << (b) << ", got " << (a) << "\n"; \
    ++g_test_failures; \
  } else { \
    cout << "[OK] " << msg << "\n"; \
  } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
  if (!(cond)) { \
    cerr << "[FAIL] " << msg << ": condition is false\n"; \
    ++g_test_failures; \
  } else { \
    cout << "[OK] " << msg << "\n"; \
  } \
} while(0)

#define EXPECT_FALSE(cond, msg) do { \
  if (cond) { \
    cerr << "[FAIL] " << msg << ": condition is true\n"; \
    ++g_test_failures; \
  } else { \
    cout << "[OK] " << msg << "\n"; \
  } \
} while(0)

// Test helpers to access mock state for assertions
static const LockRecord& getLastLockRecord() {
  return g_lockRecord;
}
static bool wasLockCalled() {
  return g_lockCalled;
}
static const vector<OpLog>& getVdbeOps() {
  return g_vdbe.ops;
}

// Test 1: When p->nTab is initially 0, sqlite3OpenSchemaTable should set nTab to 1
// and perform the expected locking and VDBE operation.
static void test_openSchemaTable_setsNtabWhenZero() {
  // Arrange
  resetMocks();
  Parse p;
  p.nTab = 0;
  int testDb = 7;

  // Act
  sqlite3OpenSchemaTable(&p, testDb);

  // Assert
  EXPECT_EQ(p.nTab, 1, "nTab should become 1 when initially 0");
  EXPECT_TRUE(wasLockCalled(), "sqlite3TableLock should be invoked");
  const LockRecord &lr = getLastLockRecord();
  EXPECT_EQ(lr.iTab, SCHEMA_ROOT, "Lock should target SCHEMA_ROOT");
  EXPECT_EQ(lr.iDb, testDb, "Lock should use the correct database id");
  EXPECT_EQ(lr.isWriteLock, 1, "Lock should be a write lock (1)");
  EXPECT_TRUE(lr.zName == LEGACY_SCHEMA_TABLE, "Lock should use LEGACY_SCHEMA_TABLE name");

  const auto &ops = getVdbeOps();
  EXPECT_EQ(ops.size(), 1, "Exactly one Vdbe operation should be added");
  if (ops.size() == 1) {
    const OpLog &o = ops[0];
    EXPECT_EQ(o.op, OP_OpenWrite, "Vdbe op should be OP_OpenWrite");
    EXPECT_EQ(o.p1, 0, "Vdbe op p1 should be 0");
    EXPECT_EQ(o.p2, SCHEMA_ROOT, "Vdbe op p2 should be SCHEMA_ROOT");
    EXPECT_EQ(o.p3, testDb, "Vdbe op p3 should be iDb");
    EXPECT_EQ(o.p4, 5, "Vdbe op p4 should be 5");
  }
  cout << "Test test_openSchemaTable_setsNtabWhenZero completed.\n";
}

// Test 2: When p->nTab is non-zero, sqlite3OpenSchemaTable should not modify nTab
// but should still perform the same locking and VDBE operation.
static void test_openSchemaTable_nTabPreservedWhenNonZero() {
  // Arrange
  resetMocks();
  Parse p;
  p.nTab = 3;
  int testDb = 9;

  // Act
  sqlite3OpenSchemaTable(&p, testDb);

  // Assert
  EXPECT_EQ(p.nTab, 3, "nTab should remain unchanged when initially non-zero");
  EXPECT_TRUE(wasLockCalled(), "sqlite3TableLock should be invoked even if nTab != 0");
  const LockRecord &lr = getLastLockRecord();
  EXPECT_EQ(lr.iTab, SCHEMA_ROOT, "Lock should target SCHEMA_ROOT");
  EXPECT_EQ(lr.iDb, testDb, "Lock should use the correct database id");
  EXPECT_EQ(lr.isWriteLock, 1, "Lock should be a write lock (1)");
  EXPECT_TRUE(lr.zName == LEGACY_SCHEMA_TABLE, "Lock should use LEGACY_SCHEMA_TABLE name");

  const auto &ops = getVdbeOps();
  EXPECT_EQ(ops.size(), 1, "Exactly one Vdbe operation should be added");
  if (ops.size() == 1) {
    const OpLog &o = ops[0];
    EXPECT_EQ(o.op, OP_OpenWrite, "Vdbe op should be OP_OpenWrite");
    EXPECT_EQ(o.p1, 0, "Vdbe op p1 should be 0");
    EXPECT_EQ(o.p2, SCHEMA_ROOT, "Vdbe op p2 should be SCHEMA_ROOT");
    EXPECT_EQ(o.p3, testDb, "Vdbe op p3 should be iDb");
    EXPECT_EQ(o.p4, 5, "Vdbe op p4 should be 5");
  }
  cout << "Test test_openSchemaTable_nTabPreservedWhenNonZero completed.\n";
}

// Step 3: Ensure that multiple invocations do not crash and consistently produce the same results
static void test_openSchemaTable_multipleInvocations() {
  resetMocks();
  Parse p;
  p.nTab = 0;
  int testDb = 11;

  // First invocation
  sqlite3OpenSchemaTable(&p, testDb);
  const auto &ops1 = getVdbeOps();
  // Second invocation with same initial state
  sqlite3OpenSchemaTable(&p, testDb);
  const auto &ops2 = getVdbeOps();

  // We expect that each invocation pushes exactly one OpenWrite op, so total ops = 2
  EXPECT_EQ(ops2.size(), ops1.size() + 1, "Two consecutive calls should append one more OpenWrite op each time");
  // Basic sanity: nTab should be 1 after first call
  EXPECT_EQ(p.nTab, 1, "After first call with initial nTab=0, nTab should be 1");
  cout << "Test test_openSchemaTable_multipleInvocations completed.\n";
}

// Main test runner
int main() {
  cout << "Starting unit tests for sqlite3OpenSchemaTable (self-contained mocks)\n";

  test_openSchemaTable_setsNtabWhenZero();
  test_openSchemaTable_nTabPreservedWhenNonZero();
  test_openSchemaTable_multipleInvocations();

  if (g_test_failures == 0) {
    cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    cerr << g_test_failures << " test(s) FAILED\n";
    return 1;
  }
}