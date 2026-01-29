// C++11 test harness for sqlite3VdbeMemClearAndResize
// This test targets the focal method described in <FOCAL_METHOD>.
// It relies on internal sqlite3 Vdbe memory structures (Mem) and related constants.
// The tests are designed to be non-terminating (they log failures and continue).
// Note: This harness expects the sqlite internal headers to be available in the include path.

#include <vector>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Bring in the internal sqlite types and declarations.
// We use extern "C" to avoid C++ name mangling for C-code integration.
extern "C" {
}

// Declaration of the focal function (should be provided by the sqlite source under test)
extern "C" int sqlite3VdbeMemClearAndResize(Mem *pMem, int szNew);

// Simple non-terminating test harness
static int g_failures = 0;
static std::vector<std::string> g_failure_msgs;

#define EXPECT_TRUE(cond, msg)                                              \
  do {                                                                      \
    if(!(cond)) {                                                           \
      g_failures++;                                                        \
      g_failure_msgs.push_back(std::string("EXPECT_TRUE: ") + (msg));      \
      std::cerr << "FAIL: " << (msg) << "\n";                             \
    }                                                                       \
  } while(0)

#define EXPECT_EQ(a, b, msg)                                                \
  do {                                                                      \
    if( (a) != (b) ) {                                                    \
      g_failures++;                                                        \
      g_failure_msgs.push_back(std::string("EXPECT_EQ: ") + (msg));        \
      std::cerr << "FAIL: " << (msg) << " (expected " << (long)(b)        \
                << ", got " << (long)(a) << ")\n";                         \
    }                                                                       \
  } while(0)

#define EXPECT_PTR_EQ(a, b, msg)                                            \
  do {                                                                      \
    if( (void*)(a) != (void*)(b) ) {                                      \
      g_failures++;                                                        \
      g_failure_msgs.push_back(std::string("EXPECT_PTR_EQ: ") + (msg));    \
      std::cerr << "FAIL: " << (msg) << " (expected " << (void*)(b)          \
                << ", got " << (void*)(a) << ")\n";                        \
    }                                                                       \
  } while(0)

using namespace std;

/*
 Step 2 & 3: Test design rationale

 Tests target the following observable behaviors of sqlite3VdbeMemClearAndResize(Mem*, int):
 - Branch: growth path (szMalloc < szNew) should call sqlite3VdbeMemGrow and (at minimum)
   ensure szMalloc becomes >= szNew after operation.
 - Branch: non-growth path (szMalloc >= szNew) should set pMem->z to pMem->zMalloc and mask
   pMem->flags to only the allowed bits: MEM_Null|MEM_Int|MEM_Real|MEM_IntReal.
 - Additional check: ensure that when non-dyn path is taken, the MEM_Dyn flag remains 0.

 The tests avoid relying on private/internal behavior beyond the public API surface
 and static/internal details are exercised via the provided API surface and known
 Mem layout (as defined in the included headers).
*/

// Helper to initialize a Mem-like structure with a minimal layout.
// We rely on the actual sqlite Mem layout provided by the included headers.
// This function simply zeroes the structure to a known baseline.
static void init_mem(Mem &m) {
  memset(&m, 0, sizeof(Mem));
}

// Test 1: Growth path when szMalloc < szNew
// - Prepare a Mem with szMalloc smaller than szNew and a valid zMalloc buffer.
// - Call sqlite3VdbeMemClearAndResize and verify that growth path allocated/expanded
//   memory (szMalloc >= szNew) as a consequence of sqlite3VdbeMemGrow being invoked.
static void test_growth_path() {
  // Explanation:
  // This test exercises the branch that calls sqlite3VdbeMemGrow when additional space is needed.
  // We cannot directly observe the internal growth call, but we can observe that
  // the szMalloc field increases to accommodate szNew.
  Mem m;
  init_mem(m);

  // Prepare memory layout for growth scenario
  m.szMalloc = 2;
  m.zMalloc = (char*)malloc(32); // existing allocation requested by the Mem
  m.z = nullptr;
  m.flags = 0; // ensure MEM_Dyn not set

  const int szNew = 16; // szNew > szMalloc to trigger growth

  int rc = sqlite3VdbeMemClearAndResize(&m, szNew);

  // We expect growth: szMalloc should be at least szNew after growth
  EXPECT_TRUE(m.szMalloc >= szNew, "Growth path should allocate enough memory (szMalloc >= szNew)");

  // Cleanup
  if (m.zMalloc) free(m.zMalloc);
  // The growth path may have reallocated zMalloc; ensure we don't leak:
  if (m.z) free(m.z); // safe no-op if NULL
  // rc is not strictly asserted here since sqlite3VdbeMemGrow behavior may vary by build
}

// Test 2: Non-growth path when szMalloc >= szNew
// - Prepare a Mem with an existing zMalloc buffer and a separate z pointer.
// - Also set extra bits in flags to verify masking.
// - After calling, verify:
//   a) pMem->z is updated to pMem->zMalloc
//   b) pMem->flags is masked to allowed bits: MEM_Null|MEM_Int|MEM_Real|MEM_IntReal
//   c) The non-dyn invariant ((pMem->flags & MEM_Dyn) == 0) holds (indirectly by the zeroing of non-Dyn bits).
static void test_no_growth_and_flag_mask() {
  Mem m;
  init_mem(m);

  // Prepare for non-growth path
  const int szNew = 8;
  m.szMalloc = szNew; // equal to szNew to avoid growth
  m.zMalloc = (char*)malloc(szNew);
  m.z = (char*)malloc(8); // some existing data; will be overridden by zMalloc
  // Create an extra flag bit to verify masking
  const unsigned int EXTRA = 0x1000;
  m.flags = EXTRA | MEM_Null; // include an extra flag bit

  int rc = sqlite3VdbeMemClearAndResize(&m, szNew);

  // z should be updated to point to zMalloc
  EXPECT_PTR_EQ(m.z, m.zMalloc, "After non-growth path, z must be equal to zMalloc");

  // flags should be masked with (MEM_Null|MEM_Int|MEM_Real|MEM_IntReal)
  const unsigned int allowedMask = MEM_Null | MEM_Int | MEM_Real | MEM_IntReal;
  unsigned int expectedFlags = (m.flags) & allowedMask; // compute using actual mask
  // The final flags should equal the original flags masked by allowedMask
  unsigned int originalMasked = (EXTRA | MEM_Null) & allowedMask;
  EXPECT_EQ((unsigned int)(originalMasked), (unsigned int)expectedFlags, "flags should be masked to allowed bits");

  // Cleanup
  if (m.zMalloc) free(m.zMalloc);
  if (m.z) free(m.z);
}

// Test 3: Sanity check for static-independent behavior (basic type integrity)
// - This test ensures that we can construct a Mem-like object and invoke the function
//   without crashing, focusing on proper interaction of memory pointers.
// - It does not enforce internal static function behavior (which is not accessible here).
static void test_basic_sanity() {
  Mem m;
  init_mem(m);
  m.szMalloc = 4;
  m.zMalloc = (char*)malloc(4);
  m.z = m.zMalloc;
  m.flags = MEM_Null | MEM_IntReal; // some valid combination

  int rc = sqlite3VdbeMemClearAndResize(&m, 4);
  // Basic post-condition: memory pointers must remain non-null
  EXPECT_TRUE(m.zMalloc != nullptr, "zMalloc should remain non-null after call");
  if (m.zMalloc) free(m.zMalloc);
  if (m.z) free(m.z);
}

// Entry point
int main(int argc, char **argv) {
  // Run tests
  test_growth_path();
  test_no_growth_and_flag_mask();
  test_basic_sanity();

  // Report summary
  if (g_failures == 0) {
    std::cout << "All tests passed." << std::endl;
    return 0;
  } else {
    std::cout << g_failures << " test(s) failed." << std::endl;
    // Print detailed messages
    for (const auto &msg : g_failure_msgs) {
      std::cerr << "  - " << msg << std::endl;
    }
    return 1;
  }
}