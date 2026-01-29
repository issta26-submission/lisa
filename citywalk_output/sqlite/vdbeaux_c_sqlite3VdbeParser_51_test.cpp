// Lightweight C++11 unit test suite for the focal method:
// Parse *sqlite3VdbeParser(Vdbe *p){ return p->pParse; }
//
// Notes based on the provided instructions:
// - No GTest is used. A tiny, non-terminating test harness is implemented.
// - The test focuses on the dependent components: Vdbe and Parse structures.
// - True/false branches are covered (pParse non-null, pParse null).
// - Static file-scope functions are not invoked or mocked; only the target
//   function and its direct dependencies are exercised here.
// - All tests run from main() with explanatory comments per test.

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>


// Domain-independent minimal stubs to model dependencies from the focal file.
// This mirrors only what is necessary to exercise sqlite3VdbeParser.

struct Parse {
  int dummy; // dummy field to simulate a real Parse structure
};

// The focal class dependency: Vdbe contains a pointer to a Parse instance.
struct Vdbe {
  Parse *pParse;
};

// The focal method under test (recreated locally for the test harness).
// In the real project, this function would be defined in vdbeaux.c.
// We implement it here to enable isolated testing without the full SQLite build.
Parse *sqlite3VdbeParser(Vdbe *p) {
  return p->pParse;
}

// ------------------------
// Lightweight test harness
// ------------------------

static int g_testCount = 0;
static int g_failCount = 0;

// Non-terminating assertion helpers (no aborts, just log failures).
template <typename T, typename U>
static void expect_ptr_eq(const T* a, const U* b, const char* testName, const char* details) {
  g_testCount++;
  if (a != b) {
    g_failCount++;
    std::cerr << "[FAIL] " << testName << ": " << details
              << " (expected " << static_cast<const void*>(b)
              << ", got " << static_cast<const void*>(a) << ")\n";
  } else {
    std::cout << "[PASS] " << testName << "\n";
  }
}

static void expect_true(bool cond, const char* testName, const char* details) {
  g_testCount++;
  if (!cond) {
    g_failCount++;
    std::cerr << "[FAIL] " << testName << ": " << details << "\n";
  } else {
    std::cout << "[PASS] " << testName << "\n";
  }
}

// ------------------------
// Unit Tests
// ------------------------

// Test 1: Normal case - Ensure sqlite3VdbeParser returns the pParse pointer
// associated with a non-null Vdbe object. This validates the "true" branch.
static void test_parser_returns_pParse_pointer() {
  // Arrange: create a Vdbe with a non-null Parse target
  Vdbe v;
  Parse p;
  p.dummy = 123;
  v.pParse = &p;

  // Act: call the focal function
  Parse *res = sqlite3VdbeParser(&v);

  // Assert: the returned pointer must be exactly the same as v.pParse
  expect_ptr_eq(res, &p, "test_parser_returns_pParse_pointer",
                "sqlite3VdbeParser should return the pParse pointer when p is non-null");
}

// Test 2: Null pParse - Ensure function returns null without dereferencing
// This exercises the "false" branch where pParse is nullptr.
static void test_parser_returns_null_when_pParse_is_null() {
  // Arrange: Vdbe with a null pParse
  Vdbe v;
  v.pParse = nullptr;

  // Act
  Parse *res = sqlite3VdbeParser(&v);

  // Assert: We expect nullptr to be returned (no dereference beyond p)
  expect_ptr_eq(res, (Parse*)nullptr, "test_parser_returns_null_when_pParse_is_null",
                "sqlite3VdbeParser should return nullptr when pParse is null");
}

// Test 3: Multiple Vdbe instances - Ensure distinct Vdbe objects map to their
// respective pParse pointers (no cross-contamination).
static void test_parser_handles_multiple_instances() {
  // Arrange: create two distinct Vdbe objects each with its own Parse target
  Vdbe v1, v2;
  Parse p1, p2;
  p1.dummy = 11;
  p2.dummy = 22;
  v1.pParse = &p1;
  v2.pParse = &p2;

  // Act
  Parse *r1 = sqlite3VdbeParser(&v1);
  Parse *r2 = sqlite3VdbeParser(&v2);

  // Assert
  expect_ptr_eq(r1, &p1, "test_parser_handles_multiple_instances - v1",
                "v1 should map to its own pParse");
  expect_ptr_eq(r2, &p2, "test_parser_handles_multiple_instances - v2",
                "v2 should map to its own pParse");
  // Sanity: ensure no cross-reference
  expect_ptr_eq(r1, &p1, "test_parser_handles_multiple_instances - no cross-link",
                "r1 should still point to p1");
  expect_ptr_eq(r2, &p2, "test_parser_handles_multiple_instances - no cross-link 2",
                "r2 should still point to p2");
}

// Test 4: Side-effect check - Ensure calling sqlite3VdbeParser does not modify pParse
// Pointer equality is preserved before and after the call.
static void test_parser_no_side_effects_on_pParse() {
  // Arrange
  Vdbe v;
  Parse p;
  v.pParse = &p;
  Parse *before = v.pParse;

  // Act
  Parse *after = sqlite3VdbeParser(&v);

  // Assert
  expect_ptr_eq(before, after, "test_parser_no_side_effects_on_pParse",
                "pParse pointer should be unchanged by sqlite3VdbeParser");
  // Additionally verify function did not mutate v.pParse
  expect_ptr_eq(v.pParse, before, "test_parser_no_side_effects_on_pParse - vpParse_mutation",
                "Vdbe.pParse should remain unchanged after call");
}

// ------------------------
// Main: Run all tests and report summary
// ------------------------

int main() {
  std::cout << "Starting tests for sqlite3VdbeParser(...)\n";

  test_parser_returns_pParse_pointer();
  test_parser_returns_null_when_pParse_is_null();
  test_parser_handles_multiple_instances();
  test_parser_no_side_effects_on_pParse();

  std::cout << "Tests completed. Total: " << g_testCount
            << ", Failures: " << g_failCount << "\n";

  // Return non-zero if any test failed
  return (g_failCount == 0) ? 0 : 1;
}