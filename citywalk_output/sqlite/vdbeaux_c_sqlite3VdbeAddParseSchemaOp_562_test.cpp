// C++11 unit test suite for sqlite3VdbeAddParseSchemaOp
// This test suite mocks the dependent VM/DB/Parse infrastructure in order to verify the focal function
// sqlite3VdbeAddParseSchemaOp correctly emits a ParseSchema operation, updates P5, iterates over BTrees,
// and performs the MayAbort action regardless of the number of databases in the Vdbe's Db.
// Note: This test is standalone and does not rely on GTest. It uses simple assertions and a tiny test harness.

#include <vector>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Domain-specific lightweight mocks and minimal types to exercise the focal function

typedef unsigned short u16;

// Minimal value for OP_ParseSchema (the actual value is irrelevant for the test as long as consistent)
#define OP_ParseSchema 42
#define P4_DYNAMIC 1

// Forward declarations to satisfy the focal function dependencies
struct Parse;
struct Db;
struct Vdbe;

// Lightweight structure definitions to simulate SQLite's internal types
struct Parse {
  // Intentionally empty; acts as a marker for sqlite3MayAbort usage
};

struct Db {
  int nDb; // number of databases attached to the Vdbe
};

struct Vdbe {
  Db* db;      // pointer to database array descriptor
  Parse* pParse; // associated parse context
};

// Logs to observe calls made by the mocks
struct AddOp4Log {
  Vdbe* p;
  int op;
  int p1, p2, p3;
  const char* zP4;
  int p4type;
};

struct ChangeP5Log {
  Vdbe* p;
  u16 p5;
};

struct UsesBtreeLog {
  Vdbe* p;
  int i;
};

struct MayAbortLog {
  Parse* p;
};

// Global logs used by the mocked dependencies
static std::vector<AddOp4Log> g_addOp4_logs;
static std::vector<ChangeP5Log> g_changeP5_logs;
static std::vector<UsesBtreeLog> g_usesBtree_logs;
static std::vector<MayAbortLog> g_mayAbort_logs;

// Mocked dependency implementations (stubs) to capture interactions

void sqlite3VdbeAddOp4(Vdbe* p, int op, int p1, int p2, int p3,
                       const char* zP4, int p4type) {
  g_addOp4_logs.push_back(AddOp4Log{p, op, p1, p2, p3, zP4, p4type});
}

void sqlite3VdbeChangeP5(Vdbe* p, u16 p5) {
  g_changeP5_logs.push_back(ChangeP5Log{p, p5});
}

void sqlite3VdbeUsesBtree(Vdbe* p, int i) {
  g_usesBtree_logs.push_back(UsesBtreeLog{p, i});
}

void sqlite3MayAbort(Parse* p) {
  g_mayAbort_logs.push_back(MayAbortLog{p});
}

// The focal function under test (redeclared here to operate on the mocked environment)
void sqlite3VdbeAddParseSchemaOp(Vdbe *p, int iDb, char *zWhere, u16 p5){
  int j;
  sqlite3VdbeAddOp4(p, OP_ParseSchema, iDb, 0, 0, zWhere, P4_DYNAMIC);
  sqlite3VdbeChangeP5(p, p5);
  for(j=0; j<p->db->nDb; j++) sqlite3VdbeUsesBtree(p, j);
  sqlite3MayAbort(p->pParse);
}

// Simple test harness helpers

static void reset_logs() {
  g_addOp4_logs.clear();
  g_changeP5_logs.clear();
  g_usesBtree_logs.clear();
  g_mayAbort_logs.clear();
}

// Test 1: Zero databases (nDb == 0)
// - Verifies that exactly one AddOp4 and one ChangeP5 occur
// - Verifies no UsesBtree calls are made
// - Verifies MayAbort is invoked
bool test_ParseSchemaOp_zeroDb() {
  reset_logs();

  Parse parse;
  Db db;
  db.nDb = 0;
  Vdbe v;
  v.db = &db;
  v.pParse = &parse;

  char zWhere[] = "WHERE_CLAUSE_ZERO";
  u16 p5 = 100; // arbitrary test value

  // Call the focal function
  sqlite3VdbeAddParseSchemaOp(&v, 0, zWhere, p5);

  // Assertions with explanatory comments

  // 1) Exactly one AddOp4 call with OP_ParseSchema and correct P4 details
  if (g_addOp4_logs.size() != 1) {
    std::cerr << "Test 1 FAILED: expected 1 AddOp4 call, got " << g_addOp4_logs.size() << "\n";
    return false;
  }
  const AddOp4Log& a0 = g_addOp4_logs[0];
  if (a0.p != &v || a0.op != OP_ParseSchema || a0.p1 !=  iDb || a0.p2 != 0 || a0.p3 != 0 ||
      a0.zP4 == nullptr || std::strcmp(a0.zP4, zWhere) != 0 || a0.p4type != P4_DYNAMIC) {
    std::cerr << "Test 1 FAILED: AddOp4 arguments mismatch.\n";
    return false;
  }

  // 2) Exactly one ChangeP5 with the same Vdbe and p5 value
  if (g_changeP5_logs.size() != 1) {
    std::cerr << "Test 1 FAILED: expected 1 ChangeP5 call, got " << g_changeP5_logs.size() << "\n";
    return false;
  }
  if (g_changeP5_logs[0].p != &v || g_changeP5_logs[0].p5 != p5) {
    std::cerr << "Test 1 FAILED: ChangeP5 arguments mismatch.\n";
    return false;
  }

  // 3) No UsesBtree calls (since nDb == 0)
  if (!g_usesBtree_logs.empty()) {
    std::cerr << "Test 1 FAILED: expected 0 UsesBtree calls, got " << g_usesBtree_logs.size() << "\n";
    return false;
  }

  // 4) MayAbort invoked with the correct Parse pointer
  if (g_mayAbort_logs.size() != 1 || g_mayAbort_logs[0].p != &parse) {
    std::cerr << "Test 1 FAILED: MayAbort not invoked correctly.\n";
    return false;
  }

  std::cout << "Test 1 PASSED: sqlite3VdbeAddParseSchemaOp handles zero databases correctly.\n";
  return true;
}

// Test 2: Two databases (nDb == 2)
// - Verifies that there are exactly two UsesBtree calls with indices 0 and 1
// - Other calls should mirror Test 1 behavior
bool test_ParseSchemaOp_twoDb() {
  reset_logs();

  Parse parse;
  Db db;
  db.nDb = 2;
  Vdbe v;
  v.db = &db;
  v.pParse = &parse;

  char zWhere[] = "WHERE_CLAUSE_TWO";
  u16 p5 = 200;

  sqlite3VdbeAddParseSchemaOp(&v, 1, zWhere, p5);

  // Assertions

  // 1) AddOp4 called once with expected args
  if (g_addOp4_logs.size() != 1) {
    std::cerr << "Test 2 FAILED: expected 1 AddOp4 call, got " << g_addOp4_logs.size() << "\n";
    return false;
  }
  const AddOp4Log& a0 = g_addOp4_logs[0];
  if (a0.p != &v || a0.op != OP_ParseSchema || a0.p1 != 1 || a0.p2 != 0 || a0.p3 != 0 ||
      a0.zP4 == nullptr || std::strcmp(a0.zP4, zWhere) != 0 || a0.p4type != P4_DYNAMIC) {
    std::cerr << "Test 2 FAILED: AddOp4 arguments mismatch.\n";
    return false;
  }

  // 2) ChangeP5 called once with p5
  if (g_changeP5_logs.size() != 1 || g_changeP5_logs[0].p != &v || g_changeP5_logs[0].p5 != p5) {
    std::cerr << "Test 2 FAILED: ChangeP5 mismatch.\n";
    return false;
  }

  // 3) Exactly two UsesBtree calls with indices 0 and 1 (order matters)
  if (g_usesBtree_logs.size() != 2) {
    std::cerr << "Test 2 FAILED: expected 2 UsesBtree calls, got " << g_usesBtree_logs.size() << "\n";
    return false;
  }
  if (g_usesBtree_logs[0].p != &v || g_usesBtree_logs[0].i != 0 ||
      g_usesBtree_logs[1].p != &v || g_usesBtree_logs[1].i != 1) {
    std::cerr << "Test 2 FAILED: UsesBtree call arguments mismatch.\n";
    return false;
  }

  // 4) MayAbort invoked with the correct Parse pointer
  if (g_mayAbort_logs.size() != 1 || g_mayAbort_logs[0].p != &parse) {
    std::cerr << "Test 2 FAILED: MayAbort not invoked correctly.\n";
    return false;
  }

  std::cout << "Test 2 PASSED: sqlite3VdbeAddParseSchemaOp handles two databases correctly.\n";
  return true;
}

// Minimal test runner
int main() {
  bool t1 = test_ParseSchemaOp_zeroDb();
  bool t2 = test_ParseSchemaOp_twoDb();

  if (t1 && t2) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cout << "SOME TESTS FAILED\n";
    return 1;
  }
}