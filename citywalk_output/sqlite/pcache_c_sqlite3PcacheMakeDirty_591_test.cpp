// Unit test suite for sqlite3PcacheMakeDirty (reproduced in-test for isolation)
// This test suite is designed to be compiled with C++11 without a testing framework.
// It mocks only the minimal required dependencies of the focal method and verifies
// both true/false branches of its predicates.
// Notes:
// - The tests use non-terminating assertions: they accumulate failures and report at the end.
// - The code reproduces the focal function semantics in this single translation unit.

#include <vector>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain-specific types and constants (minimal mock داد)
using u8 = unsigned char;
using Pgno = int;

// Forward declarations to mirror the real environment.
// These are implemented as simple mocks within this translation unit.
struct PCache; // opaque cache handle
struct PgHdr {
  int nRef;
  unsigned int flags;
  PCache *pCache;
  Pgno pgno;
};

// Bit flags used by sqlite3PcacheMakeDirty (subset sufficient for tests)
static const unsigned int PGHDR_CLEAN     = 1 << 0;
static const unsigned int PGHDR_DONT_WRITE= 1 << 1;
static const unsigned int PGHDR_DIRTY     = 1 << 2;

// Dirty list operation indicator (mock value)
static const u8 PCACHE_DIRTYLIST_ADD = 1;

// Global tracking (within this translation unit)
static std::vector<std::string> g_traceLog; // trace messages collected for assertions
static int g_dirtyListAddCount = 0;        // count of pcacheManageDirtyList calls with ADD

// Mocked dependency: sqlite3PcachePageSanity - always returns true in the test harness
static int sqlite3PcachePageSanity(PgHdr *p){
  (void)p; // unused in the mock beyond presence
  return 1;
}

// Mocked dependency: pcacheTrace - records trace output for later inspection
static void pcacheTrace(const char *fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  char buffer[1024];
  vsnprintf(buffer, sizeof(buffer), fmt, ap);
  va_end(ap);
  g_traceLog.emplace_back(buffer);
}

// Mocked dependency: pcacheManageDirtyList - increments when ADD is requested
static void pcacheManageDirtyList(PgHdr *pPage, u8 addRemove){
  (void)pPage; // pointer is not relevant for counting in this mock
  if(addRemove == PCACHE_DIRTYLIST_ADD){
    ++g_dirtyListAddCount;
  }
}

// The focal function under test (reproduced here for isolated testing)
void sqlite3PcacheMakeDirty(PgHdr *p){
  assert( p->nRef>0 );
  assert( sqlite3PcachePageSanity(p) );
  if( p->flags & (PGHDR_CLEAN|PGHDR_DONT_WRITE) ){    /*OPTIMIZATION-IF-FALSE*/
    p->flags &= ~PGHDR_DONT_WRITE;
    if( p->flags & PGHDR_CLEAN ){
      p->flags ^= (PGHDR_DIRTY|PGHDR_CLEAN);
      pcacheTrace(("%p.DIRTY %d\n",(void*)p->pCache,p->pgno));
      assert( (p->flags & (PGHDR_DIRTY|PGHDR_CLEAN))==PGHDR_DIRTY );
      pcacheManageDirtyList(p, PCACHE_DIRTYLIST_ADD);
      assert( sqlite3PcachePageSanity(p) );
    }
    assert( sqlite3PcachePageSanity(p) );
  }
}

// Helper: simple test harness (non-terminating assertions)
static int g_totalTests = 0;
static int g_failedTests = 0;

#define TEST_PASS(msg) do { std::cout << "[PASS] " << msg << "\n"; } while(0)
#define TEST_FAIL(msg) do { std::cerr << "[FAIL] " << msg << "\n"; ++g_failedTests; } while(0)
#define TEST_ASSERT(cond, msg) do { ++g_totalTests; if(!(cond)){ TEST_FAIL(msg); } else { TEST_PASS((std::string("CHECK: ") + msg).c_str()); } } while(0)

// Helpers to reset test state
static void reset_test_state(){
  g_traceLog.clear();
  g_dirtyListAddCount = 0;
}

// Test 1: No flags set; nRef>0 and sanity returns true. Expect no changes.
static void test_no_flags_no_op(){
  // Prepare PgHdr
  PCache cache;
  PgHdr p;
  p.nRef = 1;
  p.flags = 0; // neither CLEAN nor DONT_WRITE set
  p.pCache = &cache;
  p.pgno = 42;

  reset_test_state();
  sqlite3PcacheMakeDirty(&p);

  TEST_ASSERT(p.nRef > 0, "nRef must be > 0");
  TEST_ASSERT((p.flags & (PGHDR_CLEAN|PGHDR_DONT_WRITE)) == 0,
              "Flags should remain unchanged (no CLEAN/DONT_WRITE)");

  // No trace should be emitted, and dirty list should not be touched
  TEST_ASSERT(g_traceLog.empty(), "No trace expected when no flags set");
  TEST_ASSERT(g_dirtyListAddCount == 0, "Dirty list should not be modified");
}

// Test 2: Only DONT_WRITE set; expect DONT_WRITE cleared and no inner dirty handling.
static void test_only_dont_write_cleared_no_dirty_handling(){
  PCache cache;
  PgHdr p;
  p.nRef = 1;
  p.flags = PGHDR_DONT_WRITE; // only DONT_WRITE set
  p.pCache = &cache;
  p.pgno = 7;

  reset_test_state();
  sqlite3PcacheMakeDirty(&p);

  TEST_ASSERT((p.flags & (PGHDR_CLEAN|PGHDR_DONT_WRITE)) == 0,
              "DONT_WRITE should be cleared; no further actions");
  TEST_ASSERT(g_dirtyListAddCount == 0, "Dirty list should not be modified");
  TEST_ASSERT(g_traceLog.empty(), "No trace expected when only DONT_WRITE set");
}

// Test 3: Both DONT_WRITE and CLEAN set; expect DONT_WRITE cleared, CLEAN toggled off, DIRTY set,
// and a call to pcacheManageDirtyList.
static void test_clean_and_dont_write_triggers_dirty(){
  PCache cache;
  PgHdr p;
  p.nRef = 1;
  p.flags = PGHDR_DONT_WRITE | PGHDR_CLEAN;
  p.pCache = &cache;
  p.pgno = 99;

  reset_test_state();
  sqlite3PcacheMakeDirty(&p);

  // After operation: DONT_WRITE cleared, CLEAN cleared, DIRTY set
  TEST_ASSERT((p.flags & (PGHDR_CLEAN|PGHDR_DONT_WRITE|PGHDR_DIRTY)) == PGHDR_DIRTY,
              "After operation, only DIRTY should be set");

  TEST_ASSERT(g_dirtyListAddCount == 1, "pcacheManageDirtyList must be called with ADD exactly once");
  TEST_ASSERT(!g_traceLog.empty(), "Trace should be emitted for DIRTY transition");
  // Ensure the trace mentions DIRTY with the page number
  bool traceMentionsDIRTY = false;
  for(const auto &s : g_traceLog){
    if(s.find("DIRTY") != std::string::npos){
      traceMentionsDIRTY = true;
      break;
    }
  }
  TEST_ASSERT(traceMentionsDIRTY, "Trace should contain DIRTY entry");
}

// Test 4: CLEAN only (no DONT_WRITE); expect same as Test 3 but without prior DONT_WRITE flag.
// This validates true branch when only CLEAN is set.
static void test_clean_only_triggers_dirty(){
  PCache cache;
  PgHdr p;
  p.nRef = 1;
  p.flags = PGHDR_CLEAN;
  p.pCache = &cache;
  p.pgno = 1234;

  reset_test_state();
  sqlite3PcacheMakeDirty(&p);

  // Expect the same final state as Test 3
  TEST_ASSERT((p.flags & (PGHDR_CLEAN|PGHDR_DONT_WRITE|PGHDR_DIRTY)) == PGHDR_DIRTY,
              "After operation (CLEAN only), final state should be DIRTY only");

  TEST_ASSERT(g_dirtyListAddCount == 1, "Dirty list ADD should be invoked once");
  TEST_ASSERT(!g_traceLog.empty(), "Trace should be produced for DIRTY transition");
}

// Entry point: run all tests and report summary
int main(){
  std::cout << "Starting sqlite3PcacheMakeDirty unit tests (standalone C++11 harness)\n";

  test_no_flags_no_op();
  test_only_dont_write_cleared_no_dirty_handling();
  test_clean_and_dont_write_triggers_dirty();
  test_clean_only_triggers_dirty();

  int total = g_totalTests;
  int failed = g_failedTests;
  if(failed == 0){
    std::cout << "All tests passed (" << total << " checks).\n";
  }else{
    std::cerr << "Tests finished with " << failed << " failure(s) out of " << total << " checks.\n";
  }

  return failed ? 1 : 0;
}