// Test suite for the focal method: sqlite3OsUnfetch
// This test harness is self-contained, uses no GTest, and relies on a lightweight
// custom test framework with non-terminating EXPECT_* assertions.
// The tests simulate the essential class dependencies (sqlite3_file and its pMethods)
// to verify that sqlite3OsUnfetch correctly delegates to the underlying xUnfetch method.
//
// Notes:
// - We implement minimal definitions of sqlite3_file and sqlite3_io_methods to mirror
//   the structure used by sqlite3OsUnfetch.
// - We provide multiple test cases to cover typical success/failure returns and
//   to verify that parameters (id, iOff, p) are forwarded correctly.
//
// Domain knowledge considerations:
// - Only standard library facilities are used; no external testing frameworks.
// - Tests exercise both content and address comparisons where appropriate.
// - Main() executes the test suite; assertions do not terminate on failure.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Minimal domain representations to mirror relevant parts of the SQLite internals.
using i64 = long long;

struct sqlite3_file;
struct sqlite3_io_methods {
  int (*xUnfetch)(sqlite3_file*, i64, void*);
};

struct sqlite3_file {
  sqlite3_io_methods *pMethods;
};

// The focal method under test, exact reproduction of the provided snippet:
// int sqlite3OsUnfetch(sqlite3_file *id, i64 iOff, void *p){
int sqlite3OsUnfetch(sqlite3_file *id, i64 iOff, void *p){
  return id->pMethods->xUnfetch(id, iOff, p);
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_EQ_VAL(a, b, msg)                                         \
  do {                                                                   \
    if ((a) != (b)) {                                                  \
      std::cerr << "[FAIL] " << (msg)                                        \
                << " : " << #a << " != " << #b                           \
                << " (" << (a) << " != " << (b) << ")" << std::endl;  \
      ++g_failures;                                                     \
    }                                                                    \
  } while (0)

#define EXPECT_EQ_PTR(a, b, msg)                                         \
  do {                                                                   \
    if (static_cast<const void*>(a) != static_cast<const void*>(b)) {   \
      std::cerr << "[FAIL] " << (msg)                                        \
                << " : " << #a << " != " << #b                           \
                << " (" << static_cast<const void*>(a)                 \
                << " != " << static_cast<const void*>(b) << ")" << std::endl; \
      ++g_failures;                                                     \
    }                                                                    \
  } while (0)


// --- Mock and Capturing State for Tests ---

static int g_last_ret = 0;          // Captured return value from mock xUnfetch
static sqlite3_file* g_last_id = nullptr;
static i64 g_last_iOff = 0;
static void* g_last_p = nullptr;

// Mock: capture all parameters and return a configurable value
static int mock_xUnfetch_capture(sqlite3_file* id, i64 off, void* p) {
  g_last_id = id;
  g_last_iOff = off;
  g_last_p = p;
  return g_last_ret;
}

// Mock: always return 0
static int mock_xUnfetch_zero(sqlite3_file*, i64, void*) {
  return 0;
}

// Mock: always return -7
static int mock_xUnfetch_neg7(sqlite3_file*, i64, void*) {
  return -7;
}

// --- Test Case Implementations ---

// 1) Assert that sqlite3OsUnfetch simply forwards to xUnfetch and returns the same value (0)
static void test_unfetch_forwards_zero() {
  // Arrange
  sqlite3_io_methods methods;
  methods.xUnfetch = mock_xUnfetch_zero;
  sqlite3_file f;
  f.pMethods = &methods;

  // Act
  int r = sqlite3OsUnfetch(&f, 123, (void*)0xDEADBEEF);

  // Assert
  EXPECT_EQ_VAL(r, 0, "sqlite3OsUnfetch should return value from xUnfetch (0)");
}

// 2) Assert that non-zero return values are propagated correctly
static void test_unfetch_forwards_nonzero() {
  // Arrange
  sqlite3_io_methods methods;
  methods.xUnfetch = mock_xUnfetch_neg7;
  sqlite3_file f;
  f.pMethods = &methods;

  // Act
  int r = sqlite3OsUnfetch(&f, 42, nullptr);

  // Assert
  EXPECT_EQ_VAL(r, -7, "sqlite3OsUnfetch should propagate non-zero return (-7)");
}

// 3) Comprehensive test: verify that parameters are forwarded as-is to xUnfetch
static void test_unfetch_forwarding_of_parameters() {
  // Prepare mock to capture parameters and return a known value
  g_last_id = nullptr;
  g_last_iOff = 0;
  g_last_p = nullptr;
  g_last_ret = 123; // predetermined return value

  sqlite3_io_methods methods;
  methods.xUnfetch = mock_xUnfetch_capture;
  sqlite3_file f;
  f.pMethods = &methods;

  void* payload = (void*)0x1234;

  // Act
  int r = sqlite3OsUnfetch(&f, 987, payload);

  // Assert
  EXPECT_EQ_VAL(r, 123, "Return value should equal mock_ret (123)");
  EXPECT_EQ_PTR(g_last_id, &f, "xUnfetch should receive the correct id pointer");
  EXPECT_EQ_VAL(g_last_iOff, 987, "xUnfetch should receive the correct iOff value");
  EXPECT_EQ_PTR(g_last_p, payload, "xUnfetch should receive the correct payload pointer");
}


// --- Test Runner ---

static void run_all_tests() {
  test_unfetch_forwards_zero();
  test_unfetch_forwards_nonzero();
  test_unfetch_forwarding_of_parameters();

  if (g_failures != 0) {
    std::cerr << g_failures << " test(s) FAILED." << std::endl;
  } else {
    std::cout << "All tests PASSED." << std::endl;
  }
}

// Entry point
int main() {
  run_all_tests();
  return g_failures ? 1 : 0;
}