// Test suite for sqlite3HaltConstraint (no Google Test, pure C++11)
#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <cassert>


// Domain-specific test scaffolding to mock dependent C code used by sqlite3HaltConstraint.
// This test assumes the production function is linked from the build.c file and uses the
// following simple C-callable hooks. We provide lightweight stubs to observe behavior.

extern "C" {

// Forward declarations matching the production types used by sqlite3HaltConstraint.
typedef struct Vdbe Vdbe;
typedef struct Parse Parse;
typedef signed char i8;
typedef unsigned char u8;

// Minimal production-like constants (values chosen for test determinism).
#define SQLITE_CONSTRAINT 19
#define OE_Abort 1
#define P4_STATIC 0
#define P4_TRANSIENT 1
#define OP_Halt 0x0A  // arbitrary opcode value for testing

// Production function under test (assumed to be linked from build.c)
void sqlite3HaltConstraint(
  Parse *pParse,
  int errCode,
  int onError,
  char *p4,
  i8 p4type,
  u8 p5Errmsg
);

//
// Stubs to observe behavior of HAltConstraint
//

// A minimal Vdbe instance (the actual contents are irrelevant for these tests)
static Vdbe g_vdbe_stub;

// Global flags and counters to observe calls
static bool g_mayAbort_called = false;
static int g_addOp4_called = 0;
static int g_addOp4_p1 = 0;
static int g_addOp4_p2 = 0;
static int g_addOp4_p3 = 0;
static const char* g_addOp4_p4 = nullptr;
static i8 g_addOp4_p4type = 0;
static u8 g_changeP5_value = 0;
static Vdbe* g_last_Vdbe_used = nullptr;

// Lightweight Parse type used by the test. The actual layout in the real project may differ,
// but for our unit tests we only need fields accessed by the code under test.
struct Vdbe {
  // Intentionally empty for mock purposes
  int dummy;
};

struct Parse {
  Vdbe *pVdbe;
  int nested;
};

// Production-like helpers (stubs)
Vdbe* sqlite3GetVdbe(Parse* pParse) {
  return pParse ? pParse->pVdbe : nullptr;
}

void sqlite3MayAbort(Parse* /*pParse*/) {
  g_mayAbort_called = true;
}

void sqlite3VdbeAddOp4(Vdbe* v, int op, int p1, int p2, int p3, const char* p4, i8 p4type) {
  g_addOp4_called++;
  g_last_Vdbe_used = v;
  g_addOp4_p1 = p1;
  g_addOp4_p2 = p2;
  g_addOp4_p3 = p3;
  g_addOp4_p4 = p4;
  g_addOp4_p4type = p4type;
}

void sqlite3VdbeChangeP5(Vdbe* /*v*/, u8 p5) {
  g_changeP5_value = p5;
}

// We provide a dummy main to satisfy the linker in some environments, but actual tests use main below.
} // extern "C"

//
// Helper function to reset all mocks before each test
//
static void resetMocks() {
  g_mayAbort_called = false;
  g_addOp4_called = 0;
  g_addOp4_p1 = g_addOp4_p2 = g_addOp4_p3 = 0;
  g_addOp4_p4 = nullptr;
  g_addOp4_p4type = 0;
  g_changeP5_value = 0;
  g_last_Vdbe_used = nullptr;
}

// Test 1: Basic path: onError != OE_Abort, pParse->nested = false
static void test_haltConstraint_basic_noAbort() {
  resetMocks();
  Parse p;
  p.pVdbe = &g_vdbe_stub;
  p.nested = 0;

  const char* msg = "Error occurred";
  sqlite3HaltConstraint(&p, SQLITE_CONSTRAINT, 2, const_cast<char*>(msg), P4_STATIC, 0);

  // Verify that a Halt op was added with expected parameters
  if (g_addOp4_called != 1) {
    std::cerr << "[FAIL] Test 1: sqlite3VdbeAddOp4 should be called exactly once.\n";
    std::exit(1);
  }
  if (g_addOp4_p1 != SQLITE_CONSTRAINT || g_addOp4_p2 != 2 || g_addOp4_p3 != 0) {
    std::cerr << "[FAIL] Test 1: Incorrect op parameters passed to sqlite3VdbeAddOp4.\n";
    std::exit(1);
  }
  if (g_addOp4_p4 == nullptr || std::strcmp(g_addOp4_p4, msg) != 0) {
    std::cerr << "[FAIL] Test 1: p4 argument not propagated correctly.\n";
    std::exit(1);
  }
  if (g_addOp4_p4type != P4_STATIC) {
    std::cerr << "[FAIL] Test 1: p4type not propagated correctly.\n";
    std::exit(1);
  }
  if (g_changeP5_value != 0) {
    std::cerr << "[FAIL] Test 1: ChangeP5 should be called with p5Errmsg 0.\n";
    std::exit(1);
  }
  if (g_mayAbort_called) {
    std::cerr << "[FAIL] Test 1: sqlite3MayAbort should not be called when onError != OE_Abort.\n";
    std::exit(1);
  }

  std::cout << "[PASS] Test 1: Basic non-Abort path covered.\n";
}

// Test 2: Path where onError == OE_Abort triggers MayAbort
static void test_haltConstraint_abort_path() {
  resetMocks();
  Parse p;
  p.pVdbe = &g_vdbe_stub;
  p.nested = 0;

  sqlite3HaltConstraint(&p, SQLITE_CONSTRAINT, OE_Abort, const_cast<char*>("Abort"), P4_STATIC, 0);

  // Verify MayAbort was called
  if (!g_mayAbort_called) {
    std::cerr << "[FAIL] Test 2: sqlite3MayAbort should be called when onError == OE_Abort.\n";
    std::exit(1);
  }
  // Ensure op is still added
  if (g_addOp4_called != 1) {
    std::cerr << "[FAIL] Test 2: sqlite3VdbeAddOp4 should be called even on Abort path.\n";
    std::exit(1);
  }
  std::cout << "[PASS] Test 2: Abort path correctly triggers MayAbort and Op4 addition.\n";
}

// Test 3: Nested mode allows non-CONSTRAINT errCode (ensures assertion is bypassed)
static void test_haltConstraint_nested_allows_other_err() {
  resetMocks();
  Parse p;
  p.pVdbe = &g_vdbe_stub;
  p.nested = 1; // nested allows non-CONSTRAINT due to assertion
  int irregularErr = 0x63; // 99, not SQLITE_CONSTRAINT

  sqlite3HaltConstraint(&p, irregularErr, 3, const_cast<char*>("Nested"), P4_TRANSIENT, 5);

  // Expect Op4 with provided values
  if (g_addOp4_called != 1) {
    std::cerr << "[FAIL] Test 3: sqlite3VdbeAddOp4 should be called once in nested path.\n";
    std::exit(1);
  }
  if (g_addOp4_p1 != irregularErr || g_addOp4_p2 != 3 || g_addOp4_p3 != 0) {
    std::cerr << "[FAIL] Test 3: Incorrect op parameters in nested path.\n";
    std::exit(1);
  }
  if (g_addOp4_p4 == nullptr || std::strcmp(g_addOp4_p4, "Nested") != 0) {
    std::cerr << "[FAIL] Test 3: p4 argument not propagated correctly in nested path.\n";
    std::exit(1);
  }
  if (g_addOp4_p4type != P4_TRANSIENT) {
    std::cerr << "[FAIL] Test 3: p4type not propagated correctly in nested path.\n";
    std::exit(1);
  }
  if (g_changeP5_value != 5) {
    std::cerr << "[FAIL] Test 3: ChangeP5 should reflect provided p5Errmsg (5).\n";
    std::exit(1);
  }

  std::cout << "[PASS] Test 3: Nested true allows non-CONSTRAINT and passes correct data.\n";
}

// Test 4: Ensure P4_TRANSIENT path is handled correctly (non-Abort)
static void test_haltConstraint_p4Transient() {
  resetMocks();
  Parse p;
  p.pVdbe = &g_vdbe_stub;
  p.nested = 0;

  sqlite3HaltConstraint(&p, SQLITE_CONSTRAINT, 0, const_cast<char*>("Transient"), P4_TRANSIENT, 42);

  if (g_addOp4_called != 1) {
    std::cerr << "[FAIL] Test 4: sqlite3VdbeAddOp4 should be called for P4_TRANSIENT case.\n";
    std::exit(1);
  }
  if (g_addOp4_p4type != P4_TRANSIENT) {
    std::cerr << "[FAIL] Test 4: p4type should be P4_TRANSIENT in this test.\n";
    std::exit(1);
  }
  if (g_changeP5_value != 42) {
    std::cerr << "[FAIL] Test 4: ChangeP5 should be called with p5Errmsg 42.\n";
    std::exit(1);
  }

  std::cout << "[PASS] Test 4: P4_TRANSIENT path handled correctly.\n";
}

// Main: run all tests
int main() {
  // Informative banner
  std::cout << "Running sqlite3HaltConstraint unit tests (without GTest) in C++11...\n";

  test_haltConstraint_basic_noAbort();
  test_haltConstraint_abort_path();
  test_haltConstraint_nested_allows_other_err();
  test_haltConstraint_p4Transient();

  std::cout << "All tests completed.\n";
  return 0;
}