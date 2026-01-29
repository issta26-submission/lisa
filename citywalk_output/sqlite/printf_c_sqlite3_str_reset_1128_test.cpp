// Unit test suite for the focal method sqlite3_str_reset using a minimal test harness.
// This code is self-contained and does not rely on GTest. It uses simple runtime
// assertions and a small harness to verify behavior of sqlite3_str_reset.

// Candidate Keywords (Step 1): StrAccum, isMalloced, sqlite3DbFree, SQLITE_PRINTF_MALLOCED, zText, nAlloc, nChar, db, printfFlags

#include <cstddef>
#include <iostream>
#include <sqliteInt.h>


// Focal class/dependency subset (Step 2): Minimal stubbed environment to exercise sqlite3_str_reset

// Lightweight replica of the StrAccum structure used by the focal method
struct StrAccum {
  void* db;           // Mock db handle
  char* zText;          // Text buffer
  int nAlloc;           // Allocated size
  int nChar;            // Current length
  unsigned int printfFlags; // Flags (including SQLITE_PRINTF_MALLOCED)
};

// Mocked flag that matches the original SQLITE_PRINTF_MALLOCED bit
static const unsigned int SQLITE_PRINTF_MALLOCED = 0x01;

// Track the results of sqlite3DbFree calls for verification (Step 3)
static void* lastFreedDb = nullptr;
static char* lastFreedZ = nullptr;
static int freeCallCount = 0;

// Minimal implementation of isMalloced used by sqlite3_str_reset (Step 2)
static inline int isMalloced(StrAccum *p){
  return (p->printfFlags & SQLITE_PRINTF_MALLOCED) != 0;
}

// Minimal mock of sqlite3DbFree to observe freeing behavior
static void sqlite3DbFree(void *db, char* z){
  lastFreedDb = db;
  lastFreedZ = z;
  ++freeCallCount;
}

// The focal method under test, reproduced here for isolated unit testing
void sqlite3_str_reset(StrAccum *p){
  if( isMalloced(p) ){
    sqlite3DbFree(p->db, p->zText);
    p->printfFlags &= ~SQLITE_PRINTF_MALLOCED;
  }
  p->nAlloc = 0;
  p->nChar = 0;
  p->zText = 0;
}

// Simple non-terminating test harness (Step 3)
static int failures = 0;
#define TEST_ASSERT(cond, msg) do { if(!(cond)) { \
  std::cerr << "TEST FAILURE: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
  ++failures; \
} } while(0)

namespace TestSuite {

// Test 1: When isMalloced(p) is true, sqlite3_str_reset should free the buffer,
// clear the malloced flag, and reset counters and pointer.
void test_reset_with_malloced_flag_clears_and_frees() {
  // Reset tracker
  lastFreedDb = nullptr;
  lastFreedZ = nullptr;
  freeCallCount = 0;

  StrAccum p;
  p.db = (void*)0xDEADBEEF;
  p.zText = (char*)0xBAADF00D;
  p.nAlloc = 42;
  p.nChar  = 17;
  p.printfFlags = SQLITE_PRINTF_MALLOCED; // mark as malloced

  char* originalZ = p.zText;

  sqlite3_str_reset(&p);

  TEST_ASSERT(freeCallCount == 1, "sqlite3DbFree should be called exactly once when malloced");
  TEST_ASSERT(lastFreedDb == p.db, "sqlite3DbFree should be called with the correct db handle");
  TEST_ASSERT(lastFreedZ == originalZ, "sqlite3DbFree should be called with the original zText pointer");
  TEST_ASSERT((p.printfFlags & SQLITE_PRINTF_MALLOCED) == 0, "SQLITE_PRINTF_MALLOCED flag should be cleared after reset");
  TEST_ASSERT(p.nAlloc == 0, "nAlloc should be reset to 0");
  TEST_ASSERT(p.nChar  == 0, "nChar should be reset to 0");
  TEST_ASSERT(p.zText  == nullptr, "zText should be reset to nullptr");
}

// Test 2: When isMalloced(p) is false, sqlite3_str_reset should NOT call sqlite3DbFree,
// but should still reset counters and pointer.
void test_reset_without_malloced_flag_no_free() {
  // Reset tracker
  lastFreedDb = nullptr;
  lastFreedZ = nullptr;
  freeCallCount = 0;

  StrAccum p;
  p.db = (void*)0xCAFEBABE;
  p.zText = (char*)0x0FEDCBA9;
  p.nAlloc = 7;
  p.nChar  = 3;
  p.printfFlags = 0; // not malloced

  char* originalZ = p.zText;

  sqlite3_str_reset(&p);

  TEST_ASSERT(freeCallCount == 0, "sqlite3DbFree should not be called when not malloced");
  TEST_ASSERT(lastFreedDb == nullptr, "No db should be freed when not malloced");
  TEST_ASSERT(lastFreedZ == nullptr, "No zText should be freed when not malloced");
  TEST_ASSERT((p.printfFlags & SQLITE_PRINTF_MALLOCED) == 0, "PRINTF_MALLOCED flag should remain unchanged when not malloced");
  TEST_ASSERT(p.nAlloc == 0, "nAlloc should be reset to 0");
  TEST_ASSERT(p.nChar  == 0, "nChar should be reset to 0");
  TEST_ASSERT(p.zText  == nullptr, "zText should be reset to nullptr");
  // Also ensure we did not accidentally touch the original zText pointer beyond setting it to nullptr
  (void)originalZ; // suppress unused warning in release builds
}

} // namespace TestSuite

// Entry point for the test executable
int main() {
  std::cout << "Running sqlite3_str_reset unit tests (C++11 minimal harness)..." << std::endl;

  TestSuite::test_reset_with_malloced_flag_clears_and_frees();
  TestSuite::test_reset_without_malloced_flag_no_free();

  if(failures == 0){
    std::cout << "All tests passed." << std::endl;
    return 0;
  }else{
    std::cerr << failures << " test(s) failed." << std::endl;
    return 1;
  }
}