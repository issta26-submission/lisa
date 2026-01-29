// Test suite for the focal method: sqlite3VdbeSwap
// This test targets the internal SQLite Vdbe structure layout and its swap behavior.
// It uses internal SQLite headers to obtain the exact Vdbe type and field layout.
// The tests are written in C++11 but call C-style SQLite functions.
// Note: Compile this file together with the SQLite sources (headers and implementation).
//       Ensure SQLITE_ENABLE_NORMALIZE is defined for tests that cover the normalization swap path.

#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


extern "C" {
  // Include internal SQLite headers to obtain the exact Vdbe layout and macro definitions.
  // These headers are internal to SQLite and expose the Vdbe struct and SQLITE_STMTSTATUS_REPREPARE index.
  #include "sqliteInt.h"   // core internal definitions
  #include "vdbeInt.h"     // Vdbe struct and related internals
}

// Declare the target function from vdbeaux.c with C linkage
extern "C" void sqlite3VdbeSwap(Vdbe *pA, Vdbe *pB);

// Lightweight test harness
static int g_pass = 0;
static int g_fail = 0;

#define LOG_FAIL(msg) do { std::cerr << "[FAIL] " << (msg) << std::endl; g_fail++; } while(0)
#define LOG_PASS(msg) do { std::cout << "[PASS] " << (msg) << std::endl; g_pass++; } while(0)

// Helper assertion that does not terminate the program (non-terminating)
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { LOG_FAIL(msg); return false; } } while(0)
#define ASSERT_STR_EQ(a,b,msg) do { if(std::strcmp((a),(b))!=0) { LOG_FAIL(msg); return false; } } while(0)
#define ASSERT_PTR_EQ(a,b,msg) do { if((void*)(a)!=(void*)(b)) { LOG_FAIL(msg); return false; } } while(0)
#define ASSERT_INT_EQ(a,b,msg) do { if((a)!=(b)) { LOG_FAIL(msg); return false; } } while(0)

// Test 1: Basic swap behavior with normalization feature disabled (or enabled depending on compile)
static bool test_sqlite3VdbeSwap_basic_no_norm() {
  // Arrange
  sqlite3 db;
  std::memset(&db, 0, sizeof(db)); // zero initialize dummy db object

  Vdbe A, B;
  std::memset(&A, 0, sizeof(A));
  std::memset(&B, 0, sizeof(B));

  A.db = &db;
  B.db = &db;

  // Distinct sentinel pointers to verify swap of pointers
  A.pVNext = (Vdbe*)0x11111111;
  B.pVNext = (Vdbe*)0x22222222;

  Vdbe *pPrevA = (Vdbe*)0x33333333;
  Vdbe *pPrevB = (Vdbe*)0x44444444;
  Vdbe **ppA = &pPrevA;
  Vdbe **ppB = &pPrevB;
  A.ppVPrev = ppA;
  B.ppVPrev = ppB;

  A.zSql = std::strdup("SELECT 1");
  B.zSql = std::strdup("SELECT 2");

  // Only allocate normalization strings if the macro is defined (to cover both branches in separate tests)
  #ifdef SQLITE_ENABLE_NORMALIZE
  A.zNormSql = std::strdup("normA");
  B.zNormSql = std::strdup("normB");
  #endif

  A.expmask = 0xAA;
  B.expmask = 0xBB;

  A.prepFlags = 0x11;
  B.prepFlags = 0x22;

  // Initialize aCounter with distinct values
  size_t n = sizeof(A.aCounter) / sizeof(A.aCounter[0]);
  for(size_t i=0; i<n; ++i) {
    A.aCounter[i] = (int)(i + 1);       // 1, 2, 3, ...
    B.aCounter[i] = (int)(i + 100);     // 100,101,102,...
  }

  // Act
  sqlite3VdbeSwap(&A, &B);

  // Assert
  // pVNext swapped
  ASSERT_PTR_EQ(A.pVNext, (Vdbe*)0x22222222, "pA->pVNext should be original pB->pVNext after swap");
  ASSERT_PTR_EQ(B.pVNext, (Vdbe*)0x11111111, "pB->pVNext should be original pA->pVNext after swap");

  // ppVPrev swapped
  ASSERT_PTR_EQ(A.ppVPrev, &pPrevB, "pA->ppVPrev should be original pB->ppVPrev after swap");
  ASSERT_PTR_EQ(B.ppVPrev, &pPrevA, "pB->ppVPrev should be original pA->ppVPrev after swap");

  // zSql swapped
  ASSERT_STR_EQ(A.zSql, "SELECT 2", "pA->zSql should be original pB->zSql after swap");
  ASSERT_STR_EQ(B.zSql, "SELECT 1", "pB->zSql should be original pA->zSql after swap");

  // zNormSql swapped (only if normalization feature is enabled)
  #ifdef SQLITE_ENABLE_NORMALIZE
  ASSERT_STR_EQ(A.zNormSql, "normB", "pA->zNormSql should be original pB->zNormSql after swap");
  ASSERT_STR_EQ(B.zNormSql, "normA", "pB->zNormSql should be original pA->zNormSql after swap");
  #endif

  // expmask and prepFlags after swap
  // After full swap, both pA and pB end up with expmask and prepFlags equal to the value that occurred in B before swap
  ASSERT_INT_EQ(A.expmask, 0xBB, "pA->expmask should become original pB->expmask after swap");
  ASSERT_INT_EQ(B.expmask, 0xBB, "pB->expmask should become original pB->expmask after swap");
  ASSERT_INT_EQ(A.prepFlags, 0x22, "pA->prepFlags should become original pB->prepFlags after swap");
  ASSERT_INT_EQ(B.prepFlags, 0xBB, "pB->prepFlags should become original pB->prepFlags after swap");

  // aCounter should have been swapped by memcpy and then B's counter increment applied
  // After swap (A<->B), A.aCounter should reflect B's original values (100..)
  for(size_t i=0; i<n; ++i) {
    int expectedA = (int)(i + 100);
    if (A.aCounter[i] != expectedA) {
      LOG_FAIL("A.aCounter[" + std::to_string(i) + "] != expected (swap of aCounter)");
      return false;
    }
  }
  // B's aCounter should be A's original values (1..), then increment at index SQLITE_STMTSTATUS_REPREPARE
  int idx = SQLITE_STMTSTATUS_REPREPARE;
  int expectedB = (int)(idx + 1); // original A's value at idx is idx+1, then increment
  if (B.aCounter[idx] != expectedB) {
    LOG_FAIL("B.aCounter[SQLITE_STMTSTATUS_REPREPARE] not incremented correctly after swap");
    return false;
  }

  // Cleanup allocated strings
  free(A.zSql); free(B.zSql);
  #ifdef SQLITE_ENABLE_NORMALIZE
  free(A.zNormSql); free(B.zNormSql);
  #endif

  LOG_PASS("test_sqlite3VdbeSwap_basic_no_norm passed");
  return true;
}

// Test 2: Swap behavior with normalization strings present (covering #ifdef SQLITE_ENABLE_NORMALIZE branch)
static bool test_sqlite3VdbeSwap_basic_with_norm() {
  // This test assumes SQLITE_ENABLE_NORMALIZE is defined at compile time
  #ifdef SQLITE_ENABLE_NORMALIZE
  sqlite3 db;
  std::memset(&db, 0, sizeof(db)); // dummy db

  Vdbe A, B;
  std::memset(&A, 0, sizeof(A));
  std::memset(&B, 0, sizeof(B));

  A.db = &db; B.db = &db;

  A.pVNext = (Vdbe*)0xA1;
  B.pVNext = (Vdbe*)0xB2;

  Vdbe *pPrevA = (Vdbe*)0xC3;
  Vdbe *pPrevB = (Vdbe*)0xD4;
  Vdbe **ppA = &pPrevA;
  Vdbe **ppB = &pPrevB;
  A.ppVPrev = ppA;
  B.ppVPrev = ppB;

  A.zSql = std::strdup("SELECT 3");
  B.zSql = std::strdup("SELECT 4");

  A.zNormSql = std::strdup("normA");
  B.zNormSql = std::strdup("normB");

  A.expmask = 0x11;
  B.expmask = 0x22;

  A.prepFlags = 0x33;
  B.prepFlags = 0x44;

  size_t n = sizeof(A.aCounter) / sizeof(A.aCounter[0]);
  for(size_t i=0; i<n; ++i) {
    A.aCounter[i] = (int)(i + 1);
    B.aCounter[i] = (int)(i + 200);
  }

  sqlite3VdbeSwap(&A, &B);

  // Assertions
  ASSERT_PTR_EQ(A.pVNext, (Vdbe*)0xB2, "With normalization: A.pVNext should be original B.pVNext after swap");
  ASSERT_PTR_EQ(B.pVNext, (Vdbe*)0xA1, "With normalization: B.pVNext should be original A.pVNext after swap");

  ASSERT_PTR_EQ(A.ppVPrev, &pPrevB, "With normalization: A.ppVPrev should be original B.ppVPrev after swap");
  ASSERT_PTR_EQ(B.ppVPrev, &pPrevA, "With normalization: B.ppVPrev should be original A.ppVPrev after swap");

  ASSERT_STR_EQ(A.zSql, "SELECT 4", "With normalization: A.zSql swapped correctly");
  ASSERT_STR_EQ(B.zSql, "SELECT 3", "With normalization: B.zSql swapped correctly");

  ASSERT_STR_EQ(A.zNormSql, "normB", "With normalization: A.zNormSql swapped correctly");
  ASSERT_STR_EQ(B.zNormSql, "normA", "With normalization: B.zNormSql swapped correctly");

  // expmask and prepFlags post-swap (as explained in test 1)
  ASSERT_INT_EQ(A.expmask, 0x22, "With normalization: A.expmask should be original B.expmask after swap");
  ASSERT_INT_EQ(B.expmask, 0x22, "With normalization: B.expmask should be original B.expmask after swap");
  ASSERT_INT_EQ(A.prepFlags, 0x44, "With normalization: A.prepFlags should be original B.prepFlags after swap");
  ASSERT_INT_EQ(B.prepFlags, 0x22, "With normalization: B.prepFlags should be original B.prepFlags after swap");

  // aCounter should reflect B's initial values swapped into A, and B's should equal A's then incremented at REPREPARE
  int idx = SQLITE_STMTSTATUS_REPREPARE;
  for(size_t i=0; i<n; ++i) {
    int expectedA = (int)(i + 200); // from B initial
    if (A.aCounter[i] != expectedA) {
      LOG_FAIL("With normalization: A.aCounter mismatch after swap");
      return false;
    }
  }
  int expectedB = (int)(idx + 200) + 1; // B gets A's value at idx (idx+1) from A before swap, then +1
  if (B.aCounter[idx] != expectedB) {
    LOG_FAIL("With normalization: B.aCounter[SQLITE_STMTSTATUS_REPREPARE] incorrect after swap");
    return false;
  }

  free(A.zSql); free(B.zSql);
  free(A.zNormSql); free(B.zNormSql);

  LOG_PASS("test_sqlite3VdbeSwap_basic_with_norm passed");
  return true;
  #else
  // If SQLITE_ENABLE_NORMALIZE is not defined, this test is not applicable.
  (void)0; // silence unused when macro is not defined
  LOG_PASS("test_sqlite3VdbeSwap_basic_with_norm skipped (SQLITE_ENABLE_NORMALIZE not defined)");
  return true;
  #endif
}

int main() {
  bool all_ok = true;

  // Run test without relying on normalization branch
  if(!test_sqlite3VdbeSwap_basic_no_norm()) {
    all_ok = false;
  }

  // Run test that covers the normalization branch (only if enabled)
  if(!test_sqlite3VdbeSwap_basic_with_norm()) {
    all_ok = false;
  }

  std::cout << "Test summary: " << g_pass << " passed, " << g_fail << " failed." << std::endl;
  return all_ok ? 0 : 1;
}