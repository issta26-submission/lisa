/*
  Unit test suite for the focal method:
  int sqlite3VdbeMemExpandBlob(Mem *pMem)

  This test suite is designed for C++11 builds without GoogleTest.
  It uses a lightweight in-source test harness with non-terminating
  assertions (EXPECT_* macros) to maximize code execution and coverage.

  Approach and rationale:
  - The tests target the key logical branches evident from the focal method:
    a) Early return when nByte <= 0 and MEM_Blob is not set.
    b) Blob expansion path when MEM_Zero/MEM_Blob are set and a non-trivial
       nZero value is present.
    c) The post-conditions after a successful expansion (z is allocated, zeros filled,
       n increased, and MEM_Zero and MEM_Term cleared).

  - The tests rely on the actual library types and functions (Mem, MEM_Zero, MEM_Blob,
    SQLITE_OK, SQLITE_NOMEM_BKPT, etc.). They instantiate Mem directly and call
    sqlite3VdbeMemExpandBlob, validating behavior without using GTest or GMock.

  - Static dependencies within sqlite headers are leveraged as-is. The test sets
    pMem->db = nullptr to satisfy the internal assertion
    (pMem->db==0 || sqlite3_mutex_held(pMem->db->mutex)) and to avoid needing a real
    sqlite3 database handle.

  - Each test is annotated with comments describing what behavior it validates.

  Note: This file assumes the presence of SQLite internal headers (sqliteInt.h, vdbeInt.h)
  and linkage to the project’s implementation of sqlite3VdbeMemExpandBlob. It is intended
  to be added to the project alongside the original sources and compiled with the
  project (C++11) settings.
*/

#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Include internal SQLite structures and function declarations

using std::cerr;
using std::endl;

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_TRUE(cond) \
  do { \
    if(!(cond)) { \
      cerr << "[EXPECT_TRUE FAILED] " #cond " at line " << __LINE__ << endl; \
      ++g_failures; \
    } \
  } while(0)

#define EXPECT_FALSE(cond) \
  do { \
    if((cond)) { \
      cerr << "[EXPECT_FALSE FAILED] " #cond " at line " << __LINE__ << endl; \
      ++g_failures; \
    } \
  } while(0)

#define EXPECT_EQ(a, b) \
  do { \
    if(!((a) == (b))) { \
      cerr << "[EXPECT_EQ FAILED] " #a " != " #b " at line " << __LINE__ \
           << " -> " << (a) << " != " << (b) << endl; \
      ++g_failures; \
    } \
  } while(0)

#define EXPECT_NEQ(a, b) \
  do { \
    if(((a) == (b))) { \
      cerr << "[EXPECT_NEQ FAILED] " #a " == " #b " at line " << __LINE__ \
           << " -> " << (a) << " == " << (b) << endl; \
      ++g_failures; \
    } \
  } while(0)

// Helper to safely free allocated memory for Mem.z if allocated
static void freeMemZ(Mem *pMem) {
  if (pMem && pMem->z) {
    sqlite3_free(pMem->z);
    pMem->z = nullptr;
  }
}

// Helper: print a short summary after tests
static void printSummary() {
  if (g_failures == 0) {
    std::cout << "All tests passed.\n";
  } else {
    std::cout << g_failures << " test failure(s) detected.\n";
  }
}

/*
  Test 1: Early return when nByte <= 0 and MEM_Blob is not set.
  - Setup: MEM_Zero is set, MEM_Blob is not set, n = 0, u.nZero = 0.
  - Expectation: sqlite3VdbeMemExpandBlob returns SQLITE_OK and does nothing to z.
*/
static void test_expandBlob_noBlobEarlyReturn() {
  Mem m;
  memset(&m, 0, sizeof(Mem));

  m.flags = MEM_Zero;       // MEM_Zero is set
  m.n = 0;
  m.u.nZero = 0;              // nByte = n + nZero = 0
  // MEM_Blob not set
  m.z = nullptr;
  m.db = nullptr;

  int rc = sqlite3VdbeMemExpandBlob(&m);

  // Validate only as per function contract
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_EQ(m.n, 0);
  EXPECT_TRUE(m.z == nullptr);
  freeMemZ(&m);
}

/*
  Test 2: Expand blob path when MEM_Zero and MEM_Blob are set with nZero > 0.
  - Setup: MEM_Zero | MEM_Blob, n = 0, u.nZero = 4.
  - Expectation: z is allocated (at least 4 bytes), first 4 bytes are zero,
  and MEM_Zero flag is cleared after expansion.
*/
static void test_expandBlob_expandWithNZero() {
  Mem m;
  memset(&m, 0, sizeof(Mem));

  m.flags = MEM_Zero | MEM_Blob; // both flags set
  m.n = 0;
  m.u.nZero = 4;                 // nByte = 4
  m.z = nullptr;
  m.db = nullptr;                // allow allocator fallback to sqlite3Malloc

  int rc = sqlite3VdbeMemExpandBlob(&m);

  // Validate normal success path
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_EQ(m.n, 4);
  EXPECT_TRUE(m.z != nullptr);

  // The first 4 bytes should be zero due to memset in function
  unsigned char *p = reinterpret_cast<unsigned char*>(m.z);
  for (int i = 0; i < 4; ++i) {
    EXPECT_EQ(p[i], 0);
  }

  // MEM_Zero should be cleared after operation
  EXPECT_FALSE((m.flags & MEM_Zero) != 0);

  freeMemZ(&m);
}

/*
  Test 3: Expand blob path with non-zero n (partial content) to hit the memset
  of the zero region after copying existing data length.
  - Setup: MEM_Zero | MEM_Blob, n = 2, u.nZero = 3.
  - Expectation: z allocated to at least 5 bytes, bytes 2..4 zero-filled,
  n increased to 5, MEM_Zero cleared.
*/
static void test_expandBlob_expandWithNAndZeroFill() {
  Mem m;
  memset(&m, 0, sizeof(Mem));

  m.flags = MEM_Zero | MEM_Blob;
  m.n = 2;
  m.u.nZero = 3;                   // nByte = 5
  m.z = nullptr;
  m.db = nullptr;

  int rc = sqlite3VdbeMemExpandBlob(&m);

  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_EQ(m.n, 5);
  EXPECT_TRUE(m.z != nullptr);

  unsigned char *p = reinterpret_cast<unsigned char*>(m.z);
  // The region p[2], p[3], p[4] should be zeros after memset
  for (int i = 2; i < 5; ++i) {
    EXPECT_EQ(p[i], 0);
  }

  // The initial region p[0..1] is unspecified; we won't assert on it.

  // MEM_Zero must be cleared after operation
  EXPECT_FALSE((m.flags & MEM_Zero) != 0);

  freeMemZ(&m);
}

int main() {
  // Run tests
  test_expandBlob_noBlobEarlyReturn();
  test_expandBlob_expandWithNZero();
  test_expandBlob_expandWithNAndZeroFill();

  printSummary();
  return g_failures != 0 ? 1 : 0;
}