// Unit test suite for sqlite3_wasm_config_ii
// Focus: verify sqlite3_wasm_config_ii(int op, int arg1, int arg2) correctly forwards
//        its parameters to sqlite3_config(op, arg1, arg2) and returns the same result.
//
// This test is designed to be compiled together with sqlite3-wasm.c in a C/C++
// capable environment. It uses a lightweight, non-terminating test harness (no
// GTest) and relies on the real sqlite3_config implementation exposed by the
// system's SQLite library when available.
//
// Notes about the approach (mapped to steps in the task):
// Step 1 (Program Understanding): sqlite3_wasm_config_ii is a thin wrapper around
// the SQLite C API function sqlite3_config with 3 parameters. Core dependency: sqlite3_config.
// We exercise a representative set of op values (i.e., config options) to ensure
// the wrapper passes through values verbatim and yields consistent results.
// Step 2 (Unit Test Generation): Test with a mix of known SQLITE_CONFIG_* op constants
// and some extreme values to exercise potential branches in underlying sqlite3_config.
// Step 3 (Test Case Refinement): Use a lightweight, non-terminating assertion approach
// to maximize coverage while keeping tests executable without pulling in a heavy test
// framework. Static members are not applicable here (no class under test); tests
// focus on function behavior and type stability.

#include <vector>
#include <emscripten/wasmfs.h>
#include <assert.h>
#include <limits.h>
#include <string>
#include <cstdarg>
#include <iostream>


// Prototypes for the functions under test.
// sqlite3_config is a variadic C API; sqlite3_wasm_config_ii is the wrapper we test.
extern "C" int sqlite3_config(int op, ...);
extern "C" int sqlite3_wasm_config_ii(int op, int arg1, int arg2);

// Also declare the wrapper we are testing for linkage in this test TU.
extern "C" int sqlite3_wasm_config_ii(int op, int arg1, int arg2);

// Lightweight test harness: non-terminating assertions.
// Tracks total tests and failures; prints detailed failures but continues execution.
static int g_total = 0;
static int g_failed = 0;

#define CHECK_EQ(a,b, msg) do { \
  ++g_total; \
  if((a) != (b)) { \
    ++g_failed; \
    std::cerr << "TEST_FAIL: " << (msg) \
              << " | actual=" << (a) << " expected=" << (b) << "\n"; \
  } \
} while(0)

#define RUN_TEST(t) do { \
  std::cout << "RUN_TEST: " #t " ... "; \
  t(); \
  std::cout << "done\n"; \
} while(0)


// Step 1 Candidate Keywords (derived from focal method dependencies)
// - op, arg1, arg2: inputs to sqlite3_wasm_config_ii
// - sqlite3_config: underlying API being delegated to
// - SQLITE_CONFIG_*: potential op codes to exercise (from sqlite3.h)
//
// This seed informs test case selection:
// - A representative spread of SQLITE_CONFIG_* op codes
// - Boundary/extreme values for arg1/arg2
// - Negative op values to validate error handling paths if any
//
// Step 2: Generate test suite
// Test 1: Baseline compatibility - wrapper matches direct sqlite3_config for several known op codes
// Test 2: Boundary/extreme values - test with INT_MAX/INT_MIN and negative op
// Test 3: Repeated calls with varying args to ensure consistency across successive invocations
// Step 4: Provide explanatory comments for each test case

// Test 1: Baseline compatibility for several common SQLITE_CONFIG_* options.
// We compare sqlite3_wasm_config_ii(op, a1, a2) against sqlite3_config(op, a1, a2)
static void test_config_ii_matches_direct_config_basic() {
  struct Case {
    int op;
    int a1;
    int a2;
    const char* name;
  };

  // A curated list of commonly used config options.
  // Note: These constants are defined in sqlite3.h; if not available in the test env,
  // the test should be adapted to other available op codes or guarded accordingly.
  const Case cases[] = {
    { SQLITE_CONFIG_SINGLETHREAD, 0, 0, "SINGLETHREAD" },
    { SQLITE_CONFIG_MULTITHREAD, 0, 0, "MULTITHREAD" },
    { SQLITE_CONFIG_SERIALIZED, 0, 0, "SERIALIZED" },
    { SQLITE_CONFIG_MALLOC, 0, 0, "MALLOC" },
    { SQLITE_CONFIG_GETMALLOC, 0, 0, "GETMALLOC" },
    { SQLITE_CONFIG_PAGECACHE, 1024, 0, "PAGECACHE" },
    { SQLITE_CONFIG_LOOKASIDE, 128, 64, "LOOKASIDE" },
    { SQLITE_CONFIG_PMASZ, 2048, 256, "PMASZ" },
    { SQLITE_CONFIG_STMTJRNL_SPILL, 0, 0, "STMTJRNL_SPILL" },
    { SQLITE_CONFIG_SCRATCH, 512, 1024, "SCRATCH" }
  };

  const int n = sizeof(cases) / sizeof(cases[0]);
  for(int i = 0; i < n; ++i){
    const Case &c = cases[i];
    int r1 = sqlite3_config(c.op, c.a1, c.a2);
    int r2 = sqlite3_wasm_config_ii(c.op, c.a1, c.a2);
    CHECK_EQ(r1, r2, ("config_ii_matches_direct_config_basic: op=" + std::to_string(c.op) +
                       ", a1=" + std::to_string(c.a1) +
                       ", a2=" + std::to_string(c.a2) +
                       " [" + std::string(c.name) + "]").c_str());
  }
}

// Test 2: Extreme values and edge cases to exercise potential overflow/underflow paths.
// We also test a negative op to ensure wrapper forwards values faithfully.
static void test_config_ii_with_extreme_values() {
  struct Case {
    int op;
    int a1;
    int a2;
    const char* name;
  };

  const Case cases[] = {
    { -1, 1, 2, "NEG_OP" },
    { 9999, INT_MAX, INT_MIN, "INT_BOUNDS" },
    { SQLITE_CONFIG_PAGECACHE, 0, 0, "PAGECACHE_ZERO" },
    { SQLITE_CONFIG_MUTEX, 0, 0, "MUTEX" }
  };

  const int n = sizeof(cases) / sizeof(cases[0]);
  for(int i = 0; i < n; ++i){
    const Case &c = cases[i];
    int r1 = sqlite3_config(c.op, c.a1, c.a2);
    int r2 = sqlite3_wasm_config_ii(c.op, c.a1, c.a2);
    CHECK_EQ(r1, r2, ("extreme_values: op=" + std::to_string(c.op) +
                       ", a1=" + std::to_string(c.a1) +
                       ", a2=" + std::to_string(c.a2) +
                       " [" + std::string(c.name) + "]").c_str());
  }
}

// Test 3: Repeated invocations with mixed values to verify consistency across calls.
static void test_config_ii_consistency_over_multiple_calls() {
  const int ops[] = { SQLITE_CONFIG_SINGLETHREAD, SQLITE_CONFIG_MULTITHREAD, SQLITE_CONFIG_SERIALIZED,
                      SQLITE_CONFIG_MALLOC, SQLITE_CONFIG_GETMALLOC };
  const int n = sizeof(ops) / sizeof(ops[0]);
  for(int i = 0; i < 5; ++i){
    int op = ops[i % n];
    int a1 = i * 7;
    int a2 = i * -3;
    int r1 = sqlite3_config(op, a1, a2);
    int r2 = sqlite3_wasm_config_ii(op, a1, a2);
    CHECK_EQ(r1, r2, ("consistency: op=" + std::to_string(op) +
                       ", a1=" + std::to_string(a1) +
                       ", a2=" + std::to_string(a2)).c_str());
  }
}

// Runner and summary
static void run_all_tests() {
  test_config_ii_matches_direct_config_basic();
  test_config_ii_with_extreme_values();
  test_config_ii_consistency_over_multiple_calls();
}

int main() {
  // Run the focused test suite for sqlite3_wasm_config_ii
  run_all_tests();

  // Summary (non-terminating assertions)
  std::cout << "Tests run: " << g_total << "\n";
  if(g_failed){
    std::cout << "Total failures: " << g_failed << "\n";
  } else {
    std::cout << "All tests passed.\n";
  }

  // Return non-zero if any test failed
  return g_failed ? 1 : 0;
}