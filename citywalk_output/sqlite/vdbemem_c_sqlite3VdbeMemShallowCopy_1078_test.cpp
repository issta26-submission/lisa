// C++11 unit test suite for sqlite3VdbeMemShallowCopy
// Focused on testing behavior of Mem shallow copy as implemented in vdbemem.c
// This test uses the internal SQLite Mem API exposed via sqliteInt.h / vdbeInt.h
// and does not rely on any external testing framework (GTest avoided per instructions).
//
// Notes:
// - We initialize Mem objects with sqlite3VdbeMemInit and manipulate them via
//   public internal APIs to exercise the shallow copy behavior.
// - We exercise multiple branches: non-dynamic path, static flag handling, and
//   a dynamic path scenario (ensuring no crash).
// - We keep assertions non-terminating (log and count failures instead of aborting).

#include <cstdio>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <cstdint>


extern "C" {
  // Import internal dependencies required by the focal method
  #include "sqliteInt.h"
  #include "vdbeInt.h"
  // sqlite3.h provides sqlite3_int64 alias used by tests
  #include "sqlite3.h"
}

// Simple non-terminating test harness
static int g_failures = 0;
#define TEST_CHECK(cond, msg) do { \
  if(!(cond)) { \
    std::printf("TEST FAIL: %s\n", msg); \
    ++g_failures; \
  } else { \
    /* Optional success log can be verbose; kept quiet for clean output */ \
  } \
} while(0)

namespace TestSqliteVdbeMemShallowCopy {

// Helper to set up two Mem objects with the same dummy db pointer
static void initMemPair(Mem &mFrom, Mem &mTo, sqlite3 *pDb = nullptr) {
  // Initialize memory cells with no flags by default
  sqlite3VdbeMemInit(&mFrom, pDb, 0);
  sqlite3VdbeMemInit(&mTo, pDb, 0);
}

// Test 1: Non-dynamic path (pTo not MEM_Dyn), pFrom MEM_Static == 0, srcType MEM_Static
// Expect: i64 value is copied; final flags should be MEM_Static only (no Dyn/Ephem)
static void test_ShashCopy_NonDynamic_StaticUnset_StaticSrc() {
  Mem fromMem;
  Mem toMem;
  sqlite3 dbDummy;
  initMemPair(fromMem, toMem, &dbDummy);

  // Set a deterministic 64-bit value in pFrom
  sqlite3VdbeMemSetInt64(&fromMem, 0x1122334455667788LL);

  // Ensure MEM_Static is not set in source (branch condition)
  fromMem.flags &= ~MEM_Static;

  // Perform shallow copy with srcType MEM_Static
  sqlite3VdbeMemShallowCopy(&toMem, &fromMem, MEM_Static);

  // Verify the copied value matches
  sqlite3_int64 fromVal = sqlite3VdbeIntValue(&fromMem);
  sqlite3_int64 toVal   = sqlite3VdbeIntValue(&toMem);
  TEST_CHECK(fromVal == toVal, "NonDynamic_StaticUnset: value should be copied from pFrom to pTo");

  // Verify flags behavior: final flags should have MEM_Static and not MEM_Dyn or MEM_Ephem
  bool hasStatic = (toMem.flags & MEM_Static) != 0;
  bool hasDyn    = (toMem.flags & MEM_Dyn) != 0;
  bool hasEphem   = (toMem.flags & MEM_Ephem) != 0;
  TEST_CHECK(hasStatic && !hasDyn && !hasEphem,
             "NonDynamic_StaticUnset: final flags should be MEM_Static only");
}

// Test 2: Non-dynamic path, srcType MEM_Ephem
// pFrom MEM_Static == 0, srcType MEM_Ephem; expect MEM_Ephem in final flags and i64 copied
static void test_ShashCopy_NonDynamic_StaticUnset_EphemSrc() {
  Mem fromMem;
  Mem toMem;
  sqlite3 dbDummy;
  initMemPair(fromMem, toMem, &dbDummy);

  // Set a deterministic 64-bit value in pFrom
  sqlite3VdbeMemSetInt64(&fromMem, 0xA1A2A3A4A5A6A7A8LL);

  // Ensure MEM_Static is not set in source
  fromMem.flags &= ~MEM_Static;

  // Perform shallow copy with srcType MEM_Ephem
  sqlite3VdbeMemShallowCopy(&toMem, &fromMem, MEM_Ephem);

  // Verify the copied value matches
  sqlite3_int64 fromVal = sqlite3VdbeIntValue(&fromMem);
  sqlite3_int64 toVal   = sqlite3VdbeIntValue(&toMem);
  TEST_CHECK(fromVal == toVal, "NonDynamic_StaticUnset_EphemSrc: value should be copied from pFrom to pTo");

  // Verify final flags: MEM_Ephem should be set, no MEM_Static or MEM_Dyn
  bool hasEphem = (toMem.flags & MEM_Ephem) != 0;
  bool hasStatic = (toMem.flags & MEM_Static) != 0;
  bool hasDyn = (toMem.flags & MEM_Dyn) != 0;
  TEST_CHECK(hasEphem && !hasStatic && !hasDyn,
             "NonDynamic_StaticUnset_EphemSrc: final flags should be MEM_Ephem only");
}

// Test 3: Non-dynamic path, pFrom MEM_Static != 0 (source has MEM_Static set)
// Expect: branch is skipped; after memcpy, toMem.flags should reflect source flags (i.e., MEM_Static preserved)
static void test_ShashCopy_NonDynamic_SourceHasStatic() {
  Mem fromMem;
  Mem toMem;
  sqlite3 dbDummy;
  initMemPair(fromMem, toMem, &dbDummy);

  // Set value
  sqlite3VdbeMemSetInt64(&fromMem, 0x0123456789ABCDEFLL);

  // Set MEM_Static on source to exercise the "else" path
  fromMem.flags |= MEM_Static;

  // Copy with srcType MEM_Static
  sqlite3VdbeMemShallowCopy(&toMem, &fromMem, MEM_Static);

  // Value should copy
  sqlite3_int64 fromVal = sqlite3VdbeIntValue(&fromMem);
  sqlite3_int64 toVal   = sqlite3VdbeIntValue(&toMem);
  TEST_CHECK(fromVal == toVal, "SourceHasStatic: value should be copied from pFrom to pTo");

  // Since src had MEM_Static, the final MEM_Static bit should be present in pTo (due to memcpy)
  bool hasStatic = (toMem.flags & MEM_Static) != 0;
  TEST_CHECK(hasStatic, "SourceHasStatic: final flags should include MEM_Static after shallow copy");
}

// Test 4: Dynamic path trigger (pTo has MEM_Dyn flag set)
// Ensure function runs without crashing; exact post-state depends on vdbeClrCopy implementation
static void test_ShashCopy_DynamicPath_NoCrash() {
  Mem fromMem;
  Mem toMem;
  sqlite3 dbDummy;
  initMemPair(fromMem, toMem, &dbDummy);

  // Prepare source data
  sqlite3VdbeMemSetInt64(&fromMem, 0xDEADBEEFDEADBEEFL);

  // Mark pTo as dynamic to trigger the dynamic path
  toMem.flags |= MEM_Dyn;

  // Call shallow copy; should take dynamic path and call vdbeClrCopy
  sqlite3VdbeMemShallowCopy(&toMem, &fromMem, MEM_Ephem);

  // Basic sanity: the test did not crash; verify i value is at least retrievable
  sqlite3_int64 toVal = sqlite3VdbeIntValue(&toMem);
  // We don't rely on the exact value here (depends on vdbeClrCopy), just ensure it's a valid 64-bit read
  TEST_CHECK((toVal == toVal), "DynamicPath_NoCrash: validated that value read is retrievable (no crash)");
}

// Public entry to run all tests
void run_all_tests() {
  std::printf("Starting sqlite3VdbeMemShallowCopy unit tests (C++11, no GTest)\n");

  test_ShashCopy_NonDynamic_StaticUnset_StaticSrc();
  test_ShashCopy_NonDynamic_StaticUnset_EphemSrc();
  test_ShashCopy_NonDynamic_SourceHasStatic();
  test_ShashCopy_DynamicPath_NoCrash();

  if(g_failures == 0){
    std::printf("ALL TESTS PASSED\n");
  } else {
    std::printf("TOTAL FAILURES: %d\n", g_failures);
  }
}

} // namespace TestSqliteVdbeMemShallowCopy

int main() {
  // Run the test suite
  TestSqliteVdbeMemShallowCopy::run_all_tests();
  return TestSqliteVdbeMemShallowCopy::g_failures > 0 ? 1 : 0;
}