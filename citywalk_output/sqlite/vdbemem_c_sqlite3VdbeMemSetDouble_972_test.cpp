// Lightweight C++11 test harness for sqlite3VdbeMemSetDouble (vdbemem.c focal method)
//
// This test suite emulates the minimal necessary SQLite-VM Mem structure and
// dependencies to exercise the behavior of sqlite3VdbeMemSetDouble.
// It does not rely on the real SQLite source tree or GTest; instead, it provides
// small, self-contained shims and simple non-terminating assertions to maximize
// coverage across true/false branches of the target logic.
//
// Step 1 - Program Understanding (Candidate Keywords)
// - Mem: the memory cell structure used by the VDBE
// - MEM_Real: flag indicating a real-number value stored in Mem
// - MEM_Null/MEM_Null state: value represents a NULL
// - sqlite3VdbeMemSetNull: clears a Mem to NULL state
// - sqlite3IsNaN: predicate to detect NaN values
// - sqlite3VdbeMemSetDouble: the target function under test
// - pMem->u.r: the real-number field within Mem
// - NaN handling branch: when val is NaN, do not set real value
// These tests cover both the NaN and non-NaN branches, including edge cases like 0.0 and -0.0.

#include <cmath>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <limits>
#include <iostream>
#include <cfloat>


// Candidate Keywords mapping (for reference in code comments):
// - Mem, MEM_Real, MEM_Null
// - sqlite3VdbeMemSetNull, sqlite3IsNaN
// - sqlite3VdbeMemSetDouble
// - pMem->u.r (real value), pMem->flags (state)

/////////////////////////////////////////////////////////////////
// Minimal stand-in types and dependencies to exercise the focal
// method without requiring the full SQLite codebase.
/////////////////////////////////////////////////////////////////

// Representation of the Mem structure with only the fields needed by the test.
typedef struct Mem {
  struct { double r; } u; // real-number storage
  int flags;              // state flags (MEM_Real, MEM_Null, etc.)
} Mem;

// Flags (simplified subset)
static const int MEM_Null = 0;
static const int MEM_Real = 1;

// Dependency: sqlite3VdbeMemSetNull(pMem)
static void sqlite3VdbeMemSetNull(Mem *pMem) {
  // In the real code, this would clear any existing data and set NULL state.
  pMem->flags = MEM_Null;
  pMem->u.r = 0.0;
}

// Dependency: sqlite3IsNaN(val)
static int sqlite3IsNaN(double x) {
  // Use the standard C++ isnan to mirror the intended behavior
  return std::isnan(x) ? 1 : 0;
}

// Step 2: Implement the focal method (reproduced locally for test purposes)
static void sqlite3VdbeMemSetDouble(Mem *pMem, double val){
  sqlite3VdbeMemSetNull(pMem);
  if( !sqlite3IsNaN(val) ){
    pMem->u.r = val;
    pMem->flags = MEM_Real;
  }
}

/////////////////////////////////////////////////////////////////
// Test framework (non-terminating assertions)
/////////////////////////////////////////////////////////////////

static int g_failures = 0;

static void test_log_failure(const char* test_name, const char* reason) {
  std::cerr << "[TEST FAILED] " << test_name << " - " << reason << std::endl;
  ++g_failures;
}

#define TEST_ASSERT(cond, reason) do { \
  if(!(cond)) { \
    test_log_failure(__FUNCTION__, reason); \
  } \
} while(0)

/////////////////////////////////////////////////////////////////
// Unit Tests
/////////////////////////////////////////////////////////////////

// Test 1: Non-NaN value should be stored as Mem_Real with exact value.
// Candidate Keywords: Mem, MEM_Real, sqlite3VdbeMemSetNull, sqlite3IsNaN, pMem->u.r
static void test_set_double_normal_value() {
  Mem m;
  m.u.r = 0.0;
  m.flags = MEM_Null;

  double val = 3.14159265358979323846;
  sqlite3VdbeMemSetDouble(&m, val);

  TEST_ASSERT(m.flags == MEM_Real, "Normal value should set MEM_Real flag");
  TEST_ASSERT(m.u.r == val, "Stored real value should match input");
}

// Test 2: NaN value should clear to NULL (do not set real value).
// Candidate Keywords: Mem, MEM_Null, sqlite3VdbeMemSetNull, sqlite3IsNaN
static void test_set_double_nan_value() {
  Mem m;
  m.u.r = 123.456;
  m.flags = MEM_Real;

  double val = NAN;
  sqlite3VdbeMemSetDouble(&m, val);

  TEST_ASSERT(m.flags == MEM_Null, "NaN should result in MEM_Null state");
  TEST_ASSERT(m.u.r == 0.0, "After NaN, real value should be reset to 0.0");
}

// Test 3: Zero values (0.0 and -0.0) should be stored as real numbers with proper value.
// Candidate Keywords: Mem, MEM_Real, sqlite3VdbeMemSetNull, pMem->u.r
static void test_set_double_zero_and_negative_zero() {
  Mem m;

  // Test 0.0
  m.u.r = -1.0;
  m.flags = MEM_Null;
  sqlite3VdbeMemSetDouble(&m, 0.0);
  TEST_ASSERT(m.flags == MEM_Real, "0.0 should be stored as MEM_Real");
  TEST_ASSERT(m.u.r == 0.0, "Stored value should be 0.0");

  // Test -0.0
  m.u.r = 123.0;
  m.flags = MEM_Null;
  sqlite3VdbeMemSetDouble(&m, -0.0);
  TEST_ASSERT(m.flags == MEM_Real, "-0.0 should be stored as MEM_Real");
  // -0.0 has a sign bit; verify sign preservation
  TEST_ASSERT(std::signbit(m.u.r), "Stored value should be -0.0 (signbit set)");
}

// Test 4: Large magnitude value to ensure no overflow in the simple path.
// Candidate Keywords: Mem, MEM_Real, sqlite3VdbeMemSetNull, pMem->u.r
static void test_set_double_large_value() {
  Mem m;
  m.u.r = 0.0;
  m.flags = MEM_Null;

  double val = std::numeric_limits<double>::max();
  sqlite3VdbeMemSetDouble(&m, val);

  TEST_ASSERT(m.flags == MEM_Real, "Large value should be stored as MEM_Real");
  TEST_ASSERT(m.u.r == val, "Stored large value should match input");
}

/////////////////////////////////////////////////////////////////
// Entry point
/////////////////////////////////////////////////////////////////

static void run_all_tests() {
  // Candidate Keywords snapshot:
  // - Mem, MEM_Real, MEM_Null
  // - sqlite3VdbeMemSetNull, sqlite3IsNaN
  // - sqlite3VdbeMemSetDouble
  // - pMem->u.r, pMem->flags
  std::cout << "Starting sqlite3VdbeMemSetDouble test suite (simulated minimal environment)..." << std::endl;

  test_set_double_normal_value();
  test_set_double_nan_value();
  test_set_double_zero_and_negative_zero();
  test_set_double_large_value();

  if (g_failures == 0) {
    std::cout << "All tests passed." << std::endl;
  } else {
    std::cout << "Total failures: " << g_failures << std::endl;
  }
}

int main() {
  run_all_tests();
  return g_failures != 0 ? 1 : 0;
}