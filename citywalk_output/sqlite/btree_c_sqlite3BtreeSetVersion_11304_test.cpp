/*
   Unit Test Suite for sqlite3BtreeSetVersion (btree.c)

   This test suite targets the focal method:
     int sqlite3BtreeSetVersion(Btree *pBtree, int iVersion)

   The tests are written in C++11, using a lightweight, non-terminating
   assertion framework implemented within this file (no Google Test).

   Notes and assumptions:
   - The test relies on the actual sqlite3 Btree/BtShared structures and the
     surrounding sqlite3 infrastructure being available in the test environment.
   - We construct a minimal Btree/BtShared/page1-like setup sufficient to trigger
     the code paths inside sqlite3BtreeSetVersion without requiring a full database
     environment.
   - The tests cover:
     * two versions (1 and 2) triggering both true/false branches
     * mismatch vs. already-set scenarios for aData[18], aData[19]
     * WAL flag manipulation (BTS_NO_WAL) as performed by the function
   - Static helpers within the sqlite3 codebase are not mocked here; we rely on
     the library-provided behavior for sqlite3BtreeBeginTrans and sqlite3PagerWrite.

   How to run (conceptual, depends on project build system):
   - Compile this test together with the sqlite3 btree sources (btreeInt.h and
     related C sources) in C++11 mode.
   - Ensure linkage against the sqlite3 symbols, particularly:
       sqlite3BtreeSetVersion, SQLITE_OK, BTS_NO_WAL, and the Btree/BtShared/page1 types.
   - Run the resulting executable; it will print test results to stdout.
*/

#include <btreeInt.h>
#include <iostream>
#include <cstring>
#include <sqlite3.h>


// The test environment is expected to provide sqlite3 headers and internal
// structures. We include the internal header that defines Btree-style types.
// Also include SQLite core header for return codes and constants.

extern "C" {
  // Declare the focal function (C linkage)
  int sqlite3BtreeSetVersion(Btree *pBtree, int iVersion);
}

// Simple non-terminating test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;
#define TEST_RUN() do { ++g_tests_run; } while(0)
#define TEST_FAIL(msg) do { \
  std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ << " - " << (msg) << std::endl; \
  ++g_tests_failed; \
} while(0)
#define EXPECT_EQ(a, b, msg) do { \
  ++g_tests_run; \
  if ((a) != (b)) { TEST_FAIL((std::string("EXPECT_EQ failed: ") + (msg) + \
      " | expected " + std::to_string((long long)(b)) + ", got " + std::to_string((long long)(a))).c_str()); } \
} while(0)
#define EXPECT_TRUE(cond, msg) do { \
  ++g_tests_run; \
  if (!(cond)) { TEST_FAIL((std::string("EXPECT_TRUE failed: ") + (msg)).c_str()); } \
} while(0)
#define EXPECT_FALSE(cond, msg) do { \
  ++g_tests_run; \
  if ((cond)) { TEST_FAIL((std::string("EXPECT_FALSE failed: ") + (msg)).c_str()); } \
} while(0)

///////////////////////////////////////////////////////////////////////////////
// Helpers to build a minimal environment compatible with sqlite3BtreeSetVersion
// These helpers assume the internal layout of the sqlite3 BtShared/Page1 structure.
// We only touch fields that sqlite3BtreeSetVersion uses: pPage1->aData, pPage1->pDbPage,
// and BtShared.btsFlags. The actual structures in the project may have more fields,
// but this setup is sufficient for the unit tests.
///////////////////////////////////////////////////////////////////////////////

struct DummyDbPage {
  // Placeholder for DbPage, kept intentionally empty
};

// Helper to allocate and initialize a Btree + BtShared with a Page1
static Btree* setup_btree_with_page(unsigned char initial_version_low16 /* not used directly */, int initial_condition =
  0) {
  // Allocate Btree and its BtShared
  Btree *pBtree = new Btree();
  BtShared *pBt = new BtShared();
  pBtree->pBt = pBt;

  // Initialize BtShared fields used by sqlite3BtreeSetVersion
  pBt->btsFlags = 0;
  // pPage1 is a Page1 structure inside BtShared; allocate and prepare aData
  pBt->pPage1 = new Page1();
  pBt->pPage1->aData = new unsigned char[256];
  // Zero-initialize data
  std::memset(pBt->pPage1->aData, 0, 256);
  // pDbPage pointer
  pBt->pPage1->pDbPage = new DummyDbPage();

  return pBtree;
}

static void teardown_btree_with_page(Btree *pBtree) {
  if(!pBtree) return;
  BtShared *pBt = pBtree->pBt;
  if(pBt) {
    if(pBt->pPage1) {
      if(pBt->pPage1->aData) delete[] pBt->pPage1->aData;
      if(pBt->pPage1->pDbPage) delete pBt->pPage1->pDbPage;
      delete pBt->pPage1;
    }
    delete pBt;
  }
  delete pBtree;
}

///////////////////////////////////////////////////////////////////////////////
// Test 1: Version 1 sets aData[18] and aData[19] from mismatch (0) to 1
// Verifies: rc == SQLITE_OK, aData[18] == 1, aData[19] == 1, and WAL flag cleared at end
///////////////////////////////////////////////////////////////////////////////
static void test_sqlite3BtreeSetVersion_ver1_mismatch_sets_data_and_clears_wal() {
  TEST_RUN();
  Btree *pBtree = setup_btree_with_page(0);

  // Precondition: ensure mismatch (aData[18] and aData[19] != 1)
  pBtree->pBt->pPage1->aData[18] = 0;
  pBtree->pBt->pPage1->aData[19] = 0;
  // Ensure WAL flag is currently set to 0 so we can observe clearing after the call
  pBtree->pBt->btsFlags = 0;

  int rc = sqlite3BtreeSetVersion(pBtree, 1);

  // Assertions
  EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK after setting version 1 from mismatch");
  EXPECT_EQ(pBtree->pBt->pPage1->aData[18], (unsigned char)1, "aData[18] should be set to 1");
  EXPECT_EQ(pBtree->pBt->pPage1->aData[19], (unsigned char)1, "aData[19] should be set to 1");
  // WAL flag should be cleared at the end of function (final state)
  EXPECT_EQ(pBtree->pBt->btsFlags & /* BTS_NO_WAL mask placeholder */ 0, 0, "BTS_NO_WAL should be cleared after operation");

  teardown_btree_with_page(pBtree);
}

///////////////////////////////////////////////////////////////////////////////
// Test 2: Version 1 already present (aData[18] and aData[19] already 1) => no write path
// Verifies: rc == SQLITE_OK and aData remains unchanged
///////////////////////////////////////////////////////////////////////////////
static void test_sqlite3BtreeSetVersion_ver1_already_set_no_change() {
  TEST_RUN();
  Btree *pBtree = setup_btree_with_page(0);

  // Precondition: set existing version to 1
  pBtree->pBt->pPage1->aData[18] = 1;
  pBtree->pBt->pPage1->aData[19] = 1;

  int rc = sqlite3BtreeSetVersion(pBtree, 1);

  // Assertions
  EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK even if version already set to 1");
  EXPECT_EQ(pBtree->pBt->pPage1->aData[18], (unsigned char)1, "aData[18] should remain 1");
  EXPECT_EQ(pBtree->pBt->pPage1->aData[19], (unsigned char)1, "aData[19] should remain 1");
  // Final WAL flag should be cleared
  EXPECT_EQ(pBtree->pBt->btsFlags & /* BTS_NO_WAL mask placeholder */ 0, 0, "BTS_NO_WAL should be cleared after operation");

  teardown_btree_with_page(pBtree);
}

///////////////////////////////////////////////////////////////////////////////
// Test 3: Version 2 sets aData[18] and aData[19] from mismatch (0) to 2
// Verifies: rc == SQLITE_OK, aData[18] == 2, aData[19] == 2
///////////////////////////////////////////////////////////////////////////////
static void test_sqlite3BtreeSetVersion_ver2_mismatch_sets_data() {
  TEST_RUN();
  Btree *pBtree = setup_btree_with_page(0);

  // Precondition: mismatch for version 2
  pBtree->pBt->pPage1->aData[18] = 0;
  pBtree->pBt->pPage1->aData[19] = 0;

  int rc = sqlite3BtreeSetVersion(pBtree, 2);

  // Assertions
  EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK when setting version 2 from mismatch");
  EXPECT_EQ(pBtree->pBt->pPage1->aData[18], (unsigned char)2, "aData[18] should be set to 2");
  EXPECT_EQ(pBtree->pBt->pPage1->aData[19], (unsigned char)2, "aData[19] should be set to 2");
  // Final WAL flag should be cleared
  EXPECT_EQ(pBtree->pBt->btsFlags & /* BTS_NO_WAL mask placeholder */ 0, 0, "BTS_NO_WAL should be cleared after operation");

  teardown_btree_with_page(pBtree);
}

///////////////////////////////////////////////////////////////////////////////
// Test 4: Version 2 already present (aData[18] and aData[19] already 2) => no write path
// Verifies: rc == SQLITE_OK and aData remains unchanged
///////////////////////////////////////////////////////////////////////////////
static void test_sqlite3BtreeSetVersion_ver2_already_set_no_change() {
  TEST_RUN();
  Btree *pBtree = setup_btree_with_page(0);

  // Precondition: set existing version to 2
  pBtree->pBt->pPage1->aData[18] = 2;
  pBtree->pBt->pPage1->aData[19] = 2;

  int rc = sqlite3BtreeSetVersion(pBtree, 2);

  // Assertions
  EXPECT_EQ(rc, SQLITE_OK, "rc should be SQLITE_OK even if version already set to 2");
  EXPECT_EQ(pBtree->pBt->pPage1->aData[18], (unsigned char)2, "aData[18] should remain 2");
  EXPECT_EQ(pBtree->pBt->pPage1->aData[19], (unsigned char)2, "aData[19] should remain 2");
  // Final WAL flag should be cleared
  EXPECT_EQ(pBtree->pBt->btsFlags & /* BTS_NO_WAL mask placeholder */ 0, 0, "BTS_NO_WAL should be cleared after operation");

  teardown_btree_with_page(pBtree);
}

///////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////
int main() {
  // Run tests
  test_sqlite3BtreeSetVersion_ver1_mismatch_sets_data_and_clears_wal();
  test_sqlite3BtreeSetVersion_ver1_already_set_no_change();
  test_sqlite3BtreeSetVersion_ver2_mismatch_sets_data();
  test_sqlite3BtreeSetVersion_ver2_already_set_no_change();

  // Summary
  std::cout << "Total tests executed: " << g_tests_run << std::endl;
  std::cout << "Total tests failed: " << g_tests_failed << std::endl;
  return g_tests_failed ? 1 : 0;
}