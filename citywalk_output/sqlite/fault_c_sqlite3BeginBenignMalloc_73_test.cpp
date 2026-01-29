// C++11 test suite for sqlite3BeginBenignMalloc (no GTest), with in-file
// minimal emulation of dependencies from the focal class.
// The tests exercise both branches of the predicate and the hooking mechanism.

#include <cstddef>
#include <iostream>
#include <sqliteInt.h>


// -------------------------
// Minimal runtime emulation of focal dependencies (fault.c subset)
// -------------------------

// Emulate the static/global hook container (as used by sqlite3BeginBenignMalloc)
static struct {
  void (*xBenignBegin)(void);
  void (*xBenignEnd)(void);
} wsdHooks = { nullptr, nullptr };

// Emulate the wsdHooksInit statement used in sqlite3BeginBenignMalloc
// In the real project this could perform initialization boilerplate.
// For testing, keep it a no-op to preserve hook pointers across calls.
static inline void wsdHooksInit_Impl() { /* no-op for test purposes */ }
#define wsdHooksInit wsdHooksInit_Impl()

// Emulate the public API that sets the-benign malloc hooks
void sqlite3BenignMallocHooks(
  void (*xBenignBegin)(void),
  void (*xBenignEnd)(void)
){
  wsdHooks.xBenignBegin = xBenignBegin;
  wsdHooks.xBenignEnd   = xBenignEnd;
}

// The focal method under test (reproduced in test environment)
void sqlite3BeginBenignMalloc(void){
  wsdHooksInit;
  if( wsdHooks.xBenignBegin ){
    wsdHooks.xBenignBegin();
  }
}

// The secondary component in the original file (present for completeness)
void sqlite3EndBenignMalloc(void){};

// -------------------------
// Lightweight test framework (non-terminating assertions)
// -------------------------

static int g_failed = 0;

// Expectation helper: logs failure but does not abort test execution
static bool expect(bool condition, const char* message){
  if(!condition){
    std::cerr << "EXPECTATION FAILED: " << (message ? message : "") << std::endl;
    ++g_failed;
  }
  return condition;
}

// -------------------------
// Test fixtures and test cases
// -------------------------

// Shared test state: a counter to verify that the benign-begin hook was invoked
static int g_beginCallCount = 0;

// Test hook: increments the counter to indicate invocation
static void test_begin_hook(void){
  ++g_beginCallCount;
}

// 1) Validate that when no hook is installed, sqlite3BeginBenignMalloc does not invoke anything
bool test_BeginMalloc_NoHookCalled(){
  // Reset shared state
  g_beginCallCount = 0;
  // Ensure no hook is installed
  sqlite3BenignMallocHooks(nullptr, nullptr);
  // Execute the focal function
  sqlite3BeginBenignMalloc();
  // Expect no invocation
  bool ok = (g_beginCallCount == 0);
  expect(ok, "Begin function should not be called when xBenignBegin is null");
  return ok;
}

// 2) Validate that when a hook is installed, sqlite3BeginBenignMalloc invokes it exactly once per call
bool test_BeginMalloc_HookCalled(){
  // Reset shared state
  g_beginCallCount = 0;
  // Install the hook
  sqlite3BenignMallocHooks(test_begin_hook, nullptr);
  // Execute the focal function
  sqlite3BeginBenignMalloc();
  // Expect the hook to have been called exactly once
  bool ok = (g_beginCallCount == 1);
  expect(ok, "Begin function should be called exactly once when xBenignBegin is installed");
  return ok;
}

// 3) Validate multiple invocations accumulate calls correctly
bool test_BeginMalloc_MultipleCalls(){
  // Reset shared state
  g_beginCallCount = 0;
  // Install the hook
  sqlite3BenignMallocHooks(test_begin_hook, nullptr);
  // Invoke twice
  sqlite3BeginBenignMalloc();
  sqlite3BeginBenignMalloc();
  // Expect two invocations
  bool ok = (g_beginCallCount == 2);
  expect(ok, "Begin function should be called on every invocation (expected 2 calls)");
  return ok;
}

// 4) Validate that sqlite3BenignMallocHooks wires the hook correctly and that
//    sqlite3BeginBenignMalloc uses the installed hook (via the hooks struct)
static int g_viaHooksCounter = 0;
static void begin_via_hooks_test(void){
  ++g_viaHooksCounter;
}
bool test_BeginMalloc_HookInstalledViaBenignMallocHooks(){
  // Reset
  g_viaHooksCounter = 0;
  // Install via BenignMallocHooks
  sqlite3BenignMallocHooks(begin_via_hooks_test, nullptr);
  // Trigger
  sqlite3BeginBenignMalloc();
  // Expect the hook to have been invoked exactly once
  bool ok = (g_viaHooksCounter == 1);
  expect(ok, "Hook installed via sqlite3BenignMallocHooks should be invoked by sqlite3BeginBenignMalloc");
  return ok;
}

// -------------------------
// Main driver
// -------------------------

int main(){
  std::cout << "Running tests for sqlite3BeginBenignMalloc (C++11 test harness)" << std::endl;

  int testCount = 0;
  int failedCount = 0;

  // Test 1
  ++testCount;
  if(!test_BeginMalloc_NoHookCalled()){
    ++failedCount;
  }

  // Test 2
  ++testCount;
  if(!test_BeginMalloc_HookCalled()){
    ++failedCount;
  }

  // Test 3
  ++testCount;
  if(!test_BeginMalloc_MultipleCalls()){
    ++failedCount;
  }

  // Test 4
  ++testCount;
  if(!test_BeginMalloc_HookInstalledViaBenignMallocHooks()){
    ++failedCount;
  }

  // Summary
  std::cout << "Test summary: " << testCount << " tests run, "
            << failedCount << " failures." << std::endl;

  // Non-terminating: return non-zero if any failures occurred
  return (failedCount == 0) ? 0 : 1;
}