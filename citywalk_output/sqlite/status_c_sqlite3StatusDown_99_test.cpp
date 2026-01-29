// A self-contained C++11 unit test harness for the focal function: sqlite3StatusDown
// This test suite mocks the required sqlite3 environment to exercise the function
// without depending on the actual SQLite codebase or GTest. It uses simple main()
// driven tests with explicit comments describing each case.

#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// Domain-specific scaffolding to emulate the minimal environment the focal method
// depends on. The goal is to enable testing while keeping dependencies lean and
// deterministic for unit tests.

// 1) Basic type and utilities
typedef long long sqlite3_int64;
typedef unsigned int u32;
#define ArraySize(x) (sizeof(x)/sizeof((x)[0]))

// 2) Static-like state used by sqlite3StatusDown
struct WsdStat {
  int nowValue[16];
};
static WsdStat wsdStat = {};

// 3) Dependency stubs and their test-time behavior
static int statMutex[4];          // dependency: array used to decide which mutex path to take
static bool mock_mutex_held = true; // controls sqlite3_mutex_held() return value
static bool usedPcacheMutex = false; // tracks which mutex helper was used (Pcache1Mutex vs MallocMutex)

// Forward declarations of dependency functions used by sqlite3StatusDown
void* sqlite3Pcache1Mutex();
void* sqlite3MallocMutex();
bool sqlite3_mutex_held(void*);

// ArraySize-enabled wsdStat initialization macro (emulates the original macro)
#define wsdStatInit do { \
  for (size_t i = 0; i < ArraySize(wsdStat.nowValue); ++i) wsdStat.nowValue[i] = 0; \
} while(0)

// 4) Minimal mutex helpers to emulate different locking strategies
void* sqlite3Pcache1Mutex() {
  usedPcacheMutex = true;
  // dummy non-null pointer to simulate a real mutex handle
  return (void*)0x1;
}
void* sqlite3MallocMutex() {
  usedPcacheMutex = false;
  // dummy non-null pointer to simulate a different mutex handle
  return (void*)0x2;
}
bool sqlite3_mutex_held(void* m) {
  (void)m; // unused parameter in this mock
  return mock_mutex_held;
}

// 5) The focal method under test (reproduced locally for this harness)
void sqlite3StatusDown(int op, int N){
  wsdStatInit;
  assert( N>=0 );
  assert( op>=0 && op<ArraySize(statMutex) );
  assert( sqlite3_mutex_held(statMutex[op] ? sqlite3Pcache1Mutex()
                                           : sqlite3MallocMutex()) );
  assert( op>=0 && op<ArraySize(wsdStat.nowValue) );
  wsdStat.nowValue[op] -= N;
}

// 6) Helper to reset the mocked environment between tests
void reset_environment() {
  // Reset mutex array and related state
  for (size_t i = 0; i < ArraySize(statMutex); ++i) statMutex[i] = 0;
  // Reset runtime stat values
  for (size_t i = 0; i < ArraySize(wsdStat.nowValue); ++i) wsdStat.nowValue[i] = 0;
  mock_mutex_held = true;
  usedPcacheMutex = false;
}

// 7) Individual test cases

// Test 1: Basic decrement path with op in range and positive N.
// Verifies the value decreases by N and that the branch using sqlite3Pcache1Mutex()
bool test_basic_decrement() {
  reset_environment();
  int op = 2;
  int N = 5;
  wsdStat.nowValue[op] = 20;
  statMutex[op] = 1;           // non-zero -> use sqlite3Pcache1Mutex()
  mock_mutex_held = true;      // ensure the assertion on mutex ownership passes

  sqlite3StatusDown(op, N);

  // Expected: nowValue[op] should be 15
  if (wsdStat.nowValue[op] != 15) {
    std::cerr << "test_basic_decrement: expected nowValue[" << op << "] == 15, got "
              << wsdStat.nowValue[op] << "\n";
    return false;
  }
  // Should have used Pcache mutex path
  if (!usedPcacheMutex) {
    std::cerr << "test_basic_decrement: expected Pcache mutex path to be used.\n";
    return false;
  }
  return true;
}

// Test 2: N is zero; ensuring no change to the value and that the function still
// takes the mutex path correctly.
bool test_zero_N() {
  reset_environment();
  int op = 1;
  int N = 0;
  wsdStat.nowValue[op] = 7;
  statMutex[op] = 1;           // non-zero path
  mock_mutex_held = true;
  usedPcacheMutex = false;

  sqlite3StatusDown(op, N);

  // Expected: nowValue[op] remains 7
  if (wsdStat.nowValue[op] != 7) {
    std::cerr << "test_zero_N: expected nowValue[" << op << "] == 7, got "
              << wsdStat.nowValue[op] << "\n";
    return false;
  }
  // Path should still be Pcache mutex
  if (!usedPcacheMutex) {
    std::cerr << "test_zero_N: expected Pcache mutex path to be used.\n";
    return false;
  }
  return true;
}

// Test 3: Mutex path branching (two sub-cases to cover both branches):
//  - Case A: statMutex[op] non-zero -> uses sqlite3Pcache1Mutex()
//  - Case B: statMutex[op] zero -> uses sqlite3MallocMutex()
bool test_mutex_branch_variants() {
  // Case A: non-zero path
  reset_environment();
  int opA = 0;
  int N_A = 4;
  wsdStat.nowValue[opA] = 10;
  statMutex[opA] = 1;            // non-zero -> Pcache path
  mock_mutex_held = true;
  usedPcacheMutex = false;

  sqlite3StatusDown(opA, N_A);

  if (wsdStat.nowValue[opA] != 6) {
    std::cerr << "test_mutex_branch_variants (Case A): expected nowValue["
              << opA << "] == 6, got " << wsdStat.nowValue[opA] << "\n";
    return false;
  }
  if (!usedPcacheMutex) {
    std::cerr << "test_mutex_branch_variants (Case A): expected Pcache path usage.\n";
    return false;
  }

  // Case B: zero path
  reset_environment();
  int opB = 1;
  int N_B = 2;
  wsdStat.nowValue[opB] = 8;
  statMutex[opB] = 0;            // zero -> MallocMutex path
  mock_mutex_held = true;
  usedPcacheMutex = false;

  sqlite3StatusDown(opB, N_B);

  if (wsdStat.nowValue[opB] != 6) {
    std::cerr << "test_mutex_branch_variants (Case B): expected nowValue["
              << opB << "] == 6, got " << wsdStat.nowValue[opB] << "\n";
    return false;
  }
  if (usedPcacheMutex) {
    std::cerr << "test_mutex_branch_variants (Case B): expected Malloc mutex path usage.\n";
    return false;
  }

  return true;
}

// 8) Simple test runner
int main() {
  int failures = 0;

  std::cout << "Running sqlite3StatusDown unit tests (standalone harness, no GTest)\n";

  if (test_basic_decrement()) {
    std::cout << "[PASS] test_basic_decrement\n";
  } else {
    std::cout << "[FAIL] test_basic_decrement\n";
    ++failures;
  }

  if (test_zero_N()) {
    std::cout << "[PASS] test_zero_N\n";
  } else {
    std::cout << "[FAIL] test_zero_N\n";
    ++failures;
  }

  if (test_mutex_branch_variants()) {
    std::cout << "[PASS] test_mutex_branch_variants\n";
  } else {
    std::cout << "[FAIL] test_mutex_branch_variants\n";
    ++failures;
  }

  if (failures == 0) {
    std::cout << "All tests PASSED.\n";
  } else {
    std::cout << failures << " test(s) FAILED.\n";
  }

  return failures;
}