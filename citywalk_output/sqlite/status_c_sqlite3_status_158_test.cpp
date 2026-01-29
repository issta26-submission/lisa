// Lightweight C++11 unit tests for sqlite3_status (status.c) without GoogleTest.
// This test suite is designed as a small, self-contained harness with simple
// expectations and a minimal test runner. It aims to cover the primary behavior
// of sqlite3_status by driving the underlying sqlite3_status64 via the real
// implementation provided in status.c (assumed to be present in the build).

// Explanations:
// - We do not rely on GTest. Instead we implement small EXPECT_* macros that
//   perform non-fatal checks (log failures but continue executing other tests).
// - We validate:
//    1) When sqlite3_status64 reports success (rc == 0), the wrapper sqlite3_status
//       copies iCur/iHwtr into the provided int* outputs (with casting to int).
//    2) When sqlite3_status64 reports an error (rc != 0), the outputs remain unchanged.
// - We do not test the SQLITE_ENABLE_API_ARMOR branch here (null-pointer guards),
//   to keep tests stable across environments where that macro may not be defined.
//   The tests focus on the core data-path behavior (rc == 0 vs rc != 0).
// - To keep things self-contained, tests are executed from main() with explicit
//   function calls and printed results.

// Note: This test assumes status.c is compiled and linked into the same binary.
// The tests directly exercise sqlite3_status so office-specific internal lookaside
// and DB structures are not required for these scenarios.

#include <vector>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Define the minimal sqlite3_int64 type used by the focal method.
// In the real project, sqlite3_int64 is defined in sqliteInt.h.
// Here we provide a compatible alias for test compilation.
typedef long long sqlite3_int64;

// Forward declaration of the function under test (as declared in status.c).
// The real implementation has the signature:
// int sqlite3_status(int op, int *pCurrent, int *pHighwater, int resetFlag);
extern "C" int sqlite3_status(int op, int *pCurrent, int *pHighwater, int resetFlag);

// Simple non-terminating test assertion helpers.
#define EXPECT_TRUE(cond) do { if(!(cond)) { logFailure(__FILE__, __LINE__, #cond, "EXPECTED_TRUE"); } else { logPass(__FILE__, __LINE__, #cond); } } while(0)
#define EXPECT_FALSE(cond) do { if((cond)) { logFailure(__FILE__, __LINE__, #cond, "EXPECTED_FALSE"); } else { logPass(__FILE__, __LINE__, #cond); } } while(0)
#define EXPECT_EQ(a,b) do { if((a)!=(b)) { logFailure(__FILE__, __LINE__, (std::string("EXPECTED_EQ: ") + #a).c_str(), (std::to_string(a) + " != " + std::to_string(b)).c_str()); } else { logPass(__FILE__, __LINE__, (std::string("EXPECTED_EQ: ") + #a).c_str()); } } while(0)
#define EXPECT_NE(a,b) do { if((a)==(b)) { logFailure(__FILE__, __LINE__, (std::string("EXPECTED_NE: ") + #a).c_str(), (std::to_string(a) + " == " + std::to_string(b)).c_str()); } else { logPass(__FILE__, __LINE__, (std::string("EXPECTED_NE: ") + #a).c_str()); } } while(0)

static int g_totalTests = 0;
static int g_failedTests = 0;

// Logging helpers
static void logPass(const char* file, int line, const char* message) {
  std::cout << "[PASS] " << file << ":" << line << " - " << message << "\n";
}
static void logFailure(const char* file, int line, const char* condition, const char* detail) {
  std::cout << "[FAIL] " << file << ":" << line << " - " << condition
            << " -> " << detail << "\n";
  ++g_failedTests;
}
static void logSummary() {
  int total = g_totalTests;
  int failed = g_failedTests;
  int passed = total - failed;
  std::cout << "\nTest Summary: " << passed << " passed, "
            << failed << " failed, out of " << total << " tests.\n";
}

// Test 1: rc == 0 path copies iCur/iHwtr into *pCurrent/*pHighwater with int casting
static bool test_sqlite3_status_rc0_copies_values() {
  ++g_totalTests;
  // The sqlite3_status wrapper will call sqlite3_status64 and, on rc == 0, assign:
  // *pCurrent = (int)iCur; *pHighwater = (int)iHwtr;
  int current = -1;
  int highwater = -1;
  int rc = sqlite3_status(42, &current, &highwater, 0);

  // We expect rc == 0 and the outputs to reflect values provided by sqlite3_status64.
  // If the underlying sqlite3_status64 logic changes, adjust expected values accordingly.
  EXPECT_TRUE(rc == 0);
  EXPECT_EQ(current, 123);     // Expected to be the cast of iCur (set by sqlite3_status64)
  EXPECT_EQ(highwater, 456);   // Expected to be the cast of iHwtr (set by sqlite3_status64)
  return (rc == 0 && current == 123 && highwater == 456);
}

// Test 2: rc != 0 path leaves outputs unchanged
static bool test_sqlite3_status_rc_nonzero_no_change() {
  ++g_totalTests;
  int current = 7;
  int highwater = 9;
  int rc = sqlite3_status(99, &current, &highwater, 0);

  // Expect non-zero rc and that outputs are unchanged from initial values
  EXPECT_TRUE(rc != 0);
  EXPECT_EQ(current, 7);
  EXPECT_EQ(highwater, 9);
  return (rc != 0 && current == 7 && highwater == 9);
}

// Test 3: Additional check - ensure behavior remains stable for another non-zero rc
static bool test_sqlite3_status_another_rc_nonzero() {
  ++g_totalTests;
  int current = -123;
  int highwater = -456;
  int rc = sqlite3_status(1234, &current, &highwater, 0);

  // Similar to Test 2, but using a different op value to ensure consistent non-zero path
  EXPECT_TRUE(rc != 0);
  EXPECT_EQ(current, -123);
  EXPECT_EQ(highwater, -456);
  return (rc != 0 && current == -123 && highwater == -456);
}

// Entry point
int main() {
  // Run tests
  bool t1 = test_sqlite3_status_rc0_copies_values();
  bool t2 = test_sqlite3_status_rc_nonzero_no_change();
  bool t3 = test_sqlite3_status_another_rc_nonzero();

  // Aggregate results
  if (t1) logPass(__FILE__, __LINE__, "test_sqlite3_status_rc0_copies_values passed");
  if (t2) logPass(__FILE__, __LINE__, "test_sqlite3_status_rc_nonzero_no_change passed");
  if (t3) logPass(__FILE__, __LINE__, "test_sqlite3_status_another_rc_nonzero passed");

  // Count and print final summary
  logSummary();

  // Return non-zero if any test failed
  return g_failedTests == 0 ? 0 : 1;
}