// Unit test suite for the focal method: sqlite3VdbeGetLastOp
// This test is designed to be self-contained (no GTest) and uses simple
// non-terminating EXPECT_* style assertions to exercise the wrapper logic.
//
// FOCAL_METHOD under test (reproduced for reference):
// VdbeOp *sqlite3VdbeGetLastOp(Vdbe *p){
//   return sqlite3VdbeGetOp(p, p->nOp - 1);
// }

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>


// Domain-specific: simple, self-contained types to mock the real SQLite structures.
// We implement only what's necessary to test the wrapper logic in isolation.

struct VdbeOp {
  int dummy; // placeholder
};

struct Vdbe {
  int nOp; // number of operations in the Vdbe's op array
};

// Global pool of mock VdbeOp objects to simulate the real op storage.
static VdbeOp opPool[8];

// Track the last call made to sqlite3VdbeGetOp so tests can verify
// that the wrapper passes the correct address.
static Vdbe* g_lastVdbeCalled = nullptr;
static int g_lastOpAddrCalled = 0;

// Mock implementation of sqlite3VdbeGetOp.
// In a real test, this would be linked against the library, but here we provide
// a minimal replacement to validate the wrapper's behavior in isolation.
extern "C" VdbeOp* sqlite3VdbeGetOp(Vdbe* p, int addr) {
  g_lastVdbeCalled = p;
  g_lastOpAddrCalled = addr;
  // Return a pointer from our pool if the address is valid; otherwise return a
  // deterministic default (pool index 0) to keep tests stable.
  if (p != nullptr && addr >= 0 && addr < p->nOp) {
    return &opPool[addr];
  }
  return &opPool[0];
}

// The focal wrapper (mirrors the real code in vdbeaux.c)
extern "C" VdbeOp* sqlite3VdbeGetLastOp(Vdbe* p) {
  return sqlite3VdbeGetOp(p, p->nOp - 1);
}

// Lightweight test framework (non-terminating assertions)
static int g_testCount = 0;
static int g_failCount = 0;

#define EXPECT_EQ(a, b) do { \
  ++g_testCount; \
  if (!((a) == (b))) { \
    std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")" \
              << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_failCount; \
  } \
} while (0)

#define EXPECT_PTR_EQ(a, b) do { \
  ++g_testCount; \
  if ((void*)(a) != (void*)(b)) { \
    std::cerr << "EXPECT_PTR_EQ failed: " #a " (" << (void*)a << ") != " #b " (" << (void*)b << ")" \
              << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_failCount; \
  } \
} while (0)

#define EXPECT_TRUE(cond) do { \
  ++g_testCount; \
  if (!(cond)) { \
    std::cerr << "EXPECT_TRUE failed: condition is false at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_failCount; \
  } \
} while (0)

static void test_LastOp_NormalCase() {
  // Prepare a Vdbe with nOp = 3; expect last op address to be 2
  Vdbe p;
  p.nOp = 3;

  // Reset tracking state
  g_lastVdbeCalled = nullptr;
  g_lastOpAddrCalled = 0;

  // Call the wrapper under test
  VdbeOp* last = sqlite3VdbeGetLastOp(&p);

  // Expected: last points to opPool[2]
  EXPECT_PTR_EQ(last, &opPool[2]);
  // Underlying GetOp should have been invoked with addr = 2
  EXPECT_TRUE(g_lastVdbeCalled == &p);
  EXPECT_EQ(g_lastOpAddrCalled, 2);
}

static void test_LastOp_ZeroNopCase() {
  // Prepare a Vdbe with nOp = 0; expect last op address to be -1 (since p->nOp - 1)
  Vdbe p;
  p.nOp = 0;

  g_lastVdbeCalled = nullptr;
  g_lastOpAddrCalled = 0;

  VdbeOp* last = sqlite3VdbeGetLastOp(&p);

  // When nOp is 0, the address passed should be -1, and we still return opPool[0]
  EXPECT_PTR_EQ(last, &opPool[0]);
  EXPECT_TRUE(g_lastVdbeCalled == &p);
  EXPECT_EQ(g_lastOpAddrCalled, -1);
}

static void test_LastOp_VariousCases() {
  // Additional coverage: ensure for nOp values 1..3, the correct addresses are used
  for (int n = 1; n <= 3; ++n) {
    Vdbe p;
    p.nOp = n;
    g_lastVdbeCalled = nullptr;
    g_lastOpAddrCalled = 0;

    VdbeOp* last = sqlite3VdbeGetLastOp(&p);
    VdbeOp* expectedPtr = &opPool[n - 1];

    EXPECT_PTR_EQ(last, expectedPtr);
    EXPECT_TRUE(g_lastVdbeCalled == &p);
    EXPECT_EQ(g_lastOpAddrCalled, n - 1);
  }
}

static void run_all_tests() {
  test_LastOp_NormalCase();
  test_LastOp_ZeroNopCase();
  test_LastOp_VariousCases();
}

int main() {
  run_all_tests();

  std::cout << "Test results: " << g_testCount
            << " assertions, " << g_failCount << " failures.\n";

  // Return non-zero if any test failed
  return g_failCount;
}