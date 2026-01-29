// Minimal C++11 test harness for the focal method
// Focus: sqlite3VdbeUsesBtree(Vdbe *p, int i)
// Notes:
// - This harness assumes the project under test is compiled with the real SQLite sources.
// - We provide a lightweight, self-contained test driver (no GTest) that exercises the focal method
//   for several values of i, validating the behavior via direct inspection of masks and by leveraging
//   sqlite3BtreeSharable to determine the expected lockMask contribution.
// - We avoid terminating assertions; instead, we print PASS/FAIL for each test case.
// - This test uses a minimal Vdbe/Db layout that mirrors the fields accessed by sqlite3VdbeUsesBtree.
// - For the branch i == 1, the inner if condition should not set the lockMask regardless of sqlite3BtreeSharable.
// - For i != 1, the inner condition depends on sqlite3BtreeSharable(p->db->aDb[i].pBt). We derive the
//   expected result by calling sqlite3BtreeSharable ourselves (via its C linkage).

#include <cstdio>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Forward declarations to link against the project's implementation.
// We declare them with C linkage to match the library's expected ABI.
extern "C" {
  // Focal method under test
  void sqlite3VdbeUsesBtree(void *p, int i);

  // Btree sharability predicate used inside the focal method.
  // We rely on the library's implementation for determinism.
  int sqlite3BtreeSharable(void *pBt);
}

// Lightweight, test-specific type layout that mirrors the subset used by the focal function.
// IMPORTANT: This layout is crafted to be compatible with the focal function's usage (p->db->nDb,
// p->db->aDb[i].pBt, p->btreeMask, p->lockMask). It is not a full SQLite in-memory database model.
// The memory layout of the real sqlite3/vdbe structures is more complex; this simplified layout is used
// solely for test scaffolding in this focused context.
typedef unsigned int yDbMask; // mask type used by the original code

// Forward-declare a minimal Btree type (opaque to this test)
struct Btree { /* intentionally empty; details not required for test harness */ };

// Db entry holding a pointer to a Btree (matching p->db->aDb[i].pBt usage)
struct DbEntry {
  void *pBt; // pointer to Btree (opaque in this test)
};

// Minimal Db array descriptor with a fixed size (enough for our tests)
struct DbContainer {
  int nDb;
  DbEntry aDb[4];
};

// Minimal Vdbe layout sufficient for sqlite3VdbeUsesBtree
struct Vdbe {
  DbContainer *db; // pointer to the database object (sqlite3 in the real code)
  yDbMask btreeMask;
  yDbMask lockMask;
};

static void test_fail(const char* label, const char* detail) {
  std::cout << "[FAIL] " << label << (detail ? " - " : "") << (detail ? detail : "") << std::endl;
}
static void test_pass(const char* label, const char* detail) {
  std::cout << "[PASS] " << label << (detail ? " - " : "") << (detail ? detail : "") << std::endl;
}

// Helper to run a single test case for a given i value.
static void run_case(int i) {
  // Prepare a minimal database container with three entries (indexes 0..2)
  DbContainer db;
  db.nDb = 3;
  // Non-null pointers simulate pBt presence
  db.aDb[0].pBt = (void*)0x1;
  db.aDb[1].pBt = (void*)0x2;
  db.aDb[2].pBt = (void*)0x3;

  // Prepare Vdbe with zeroed masks and a reference to our db
  Vdbe v;
  v.db = &db;
  v.btreeMask = 0;
  v.lockMask = 0;

  // Call the focal method
  sqlite3VdbeUsesBtree(&v, i);

  // Expected outcomes:
  // - Bit i in btreeMask must be set after the call.
  // - If i == 1, the inner if is not taken, so lockMask bit i must remain 0.
  // - If i != 1, lockMask bit i should mirror sqlite3BtreeSharable(db.aDb[i].pBt).
  bool bitSetBtree = (v.btreeMask & (1u << i)) != 0;
  bool bitSetLock = (v.lockMask & (1u << i)) != 0;

  bool expectedLock;
  if (i == 1) {
    expectedLock = false;
  } else {
    // Use the library function to determine the expected result for the given pBt
    int sharable = sqlite3BtreeSharable(db.aDb[i].pBt);
    expectedLock = (sharable != 0);
  }

  // Validate
  bool ok = bitSetBtree && (bitSetLock == expectedLock);

  // Emit result
  if (ok) {
    test_pass("sqlite3VdbeUsesBtree: i = " + std::to_string(i),
              "btreeMask bit set and lockMask matches expected value");
  } else {
    // Build detailed diagnostic
    std::string detail = "Expected: btreeMask bit set=" +
                         std::string(bitSetBtree ? "1" : "0") +
                         ", lockMask bit=" +
                         std::string(bitSetLock ? "1" : "0") +
                         ", expectedLock=" + std::string(expectedLock ? "1" : "0");
    test_fail("sqlite3VdbeUsesBtree: i = " + std::to_string(i), detail.c_str());
  }
}

int main() {
  // Step-by-step test execution
  // We target coverage for the following branches:
  // - i in range {0, 1, 2} to exercise both i == 1 and i != 1 paths.
  // - The true branch of the outer asserts is assumed satisfied by test design (non-negative, within bounds).
  // - The inner branch is controlled by i != 1 and the result of sqlite3BtreeSharable.

  // Run three targeted test cases
  run_case(0); // Test i == 0 (inner branch may execute depending on sharable)
  run_case(1); // Test i == 1 (inner branch should NOT execute)
  run_case(2); // Test i == 2 (inner branch may execute depending on sharable)

  // Done
  return 0;
}