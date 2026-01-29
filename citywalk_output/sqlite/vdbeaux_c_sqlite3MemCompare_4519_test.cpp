// FOCAL TEST SUITE for sqlite3MemCompare
//
// Step 2: Unit Test Generation for sqlite3MemCompare located in vdbeaux.c
// The tests rely on the project's internal SQLite structures (Mem, CollSeq, sqlite3).
// We compile this test as a C++11 unit test without using GTest. A simple
// non-terminating assertion framework is provided (EXPECT_ macros).
//
// Important: The tests assume the internal SQLite headers are available in the
// include path (e.g., sqliteInt.h, vdbeInt.h) and that sqlite3MemCompare is linked
// from the library. The tests create Mem objects by directly populating their
// public fields (flags, u.i, u.r, enc, db) to exercise numeric paths of sqlite3MemCompare.

#include <sqlite3.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>


// Candidate Keywords (Step 1) for test focus:
// - Mem, CollSeq, MEM_Null, MEM_Int, MEM_Real, MEM_IntReal, MEM_Str, MEM_Blob
// - pMem1, pMem2, pColl, u.i, u.r, enc, db
// - sqlite3MemCompare, sqlite3IntFloatCompare
// - NULL handling, numeric comparisons (int/int, int/real, real/real), branch coverage

// Forward declarations of internal SQLite types/functions.
// We assume internal headers are available in the build environment.
extern "C" {
  // Public API for sqlite3MemCompare (defined in vdbeaux.c)
  int sqlite3MemCompare(const struct Mem *pMem1, const struct Mem *pMem2, const struct CollSeq *pColl);

  // Collation and numeric helper (may be declared in sqliteInt.h)
  int sqlite3IntFloatCompare(sqlite3_int64 i, double r);
}

// Bring types into scope (from sqlite3.h)

// Attempt to pull internal definitions (Mem, CollSeq).
// These headers are commonly available in the SQLite source tree and are needed
// to construct Mem and CollSeq instances for testing.

// Simple non-terminating test harness.
// We log failures but do not abort on first error, enabling higher coverage.
static int gTests = 0;
static int gFailures = 0;

#define EXPECT_TRUE(cond, msg) \
  do { \
    ++gTests; \
    if(!(cond)) { \
      std::cerr << "TEST FAIL: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
      ++gFailures; \
    } \
  } while(0)

static void run_memcompare_numeric_tests() {
  // Initialize a dummy sqlite3 db object (mallocFailed is used by asserts in code)
  sqlite3 db;
  std::memset(&db, 0, sizeof(db));

  // Helper to initialize a Mem structure for test cases
  auto initMem = [&](Mem &m, int flags, sqlite3_int64 ival, double dval, unsigned char enc) {
    m.flags = flags;
    m.u.i = ival;
    m.u.r = dval;
    m.enc = enc;
    m.db = &db;
  };

  // Common encoding
  const unsigned char ENCODING = SQLITE_UTF8;

  // 1) NULL vs NULL -> expect 0
  {
    Mem m1, m2;
    initMem(m1, MEM_Null, 0, 0.0, ENCODING);
    initMem(m2, MEM_Null, 0, 0.0, ENCODING);
    int r = sqlite3MemCompare(&m1, &m2, nullptr);
    EXPECT_TRUE(r == 0, "NULL vs NULL should return 0");
  }

  // 2) NULL vs Int  -> first NULL is less
  {
    Mem m1, m2;
    initMem(m1, MEM_Null, 0, 0.0, ENCODING);
    initMem(m2, MEM_Int, 5, 0.0, ENCODING);
    int r = sqlite3MemCompare(&m1, &m2, nullptr);
    EXPECT_TRUE(r < 0, "NULL should be less than INT");
  }

  // 3) Int vs NULL  -> first non-NULL greater
  {
    Mem m1, m2;
    initMem(m1, MEM_Int, 7, 0.0, ENCODING);
    initMem(m2, MEM_Null, 0, 0.0, ENCODING);
    int r = sqlite3MemCompare(&m1, &m2, nullptr);
    EXPECT_TRUE(r > 0, "INT should be greater than NULL");
  }

  // 4) Int vs Int
  {
    Mem m1, m2;
    initMem(m1, MEM_Int, 3, 0.0, ENCODING);
    initMem(m2, MEM_Int, 7, 0.0, ENCODING);
    int r = sqlite3MemCompare(&m1, &m2, nullptr);
    EXPECT_TRUE(r < 0, "INT 3 < INT 7");
    m2.u.i = 3;
    r = sqlite3MemCompare(&m1, &m2, nullptr);
    EXPECT_TRUE(r == 0, "INT 3 == INT 3");
    m2.u.i = 1;
    r = sqlite3MemCompare(&m1, &m2, nullptr);
    EXPECT_TRUE(r > 0, "INT 3 > INT 1");
  }

  // 5) Int vs Real
  {
    Mem m1, m2;
    initMem(m1, MEM_Int, 3, 0.0, ENCODING);
    initMem(m2, MEM_Real, 0, 3.0, ENCODING);
    int r = sqlite3MemCompare(&m1, &m2, nullptr);
    // Expect 3 vs 3.0 -> 0
    EXPECT_TRUE(r == 0, "INT(3) vs REAL(3.0) should be equal");
  }

  // 6) Real vs Int
  {
    Mem m1, m2;
    initMem(m1, MEM_Real, 0, 4.2, ENCODING);
    initMem(m2, MEM_Int, 4, 0.0, ENCODING);
    int r = sqlite3MemCompare(&m1, &m2, nullptr);
    // Expected: -sqlite3IntFloatCompare(4, 4.2)
    int expected = -sqlite3IntFloatCompare((sqlite3_int64)m2.u.i, m1.u.r);
    EXPECT_TRUE(r == expected, "REAL(4.2) vs INT(4) should follow Int/Real comparison");
  }

  // 7) Real vs Real
  {
    Mem m1, m2;
    initMem(m1, MEM_Real, 0, 2.5, ENCODING);
    initMem(m2, MEM_Real, 0, 3.0, ENCODING);
    int r = sqlite3MemCompare(&m1, &m2, nullptr);
    EXPECT_TRUE(r < 0, "REAL 2.5 < REAL 3.0");
    m2.u.r = 2.5;
    r = sqlite3MemCompare(&m1, &m2, nullptr);
    EXPECT_TRUE(r == 0, "REAL 2.5 == REAL 2.5");
    m2.u.r = 1.0;
    r = sqlite3MemCompare(&m1, &m2, nullptr);
    EXPECT_TRUE(r > 0, "REAL 2.5 > REAL 1.0");
  }

  // 8) IntReal path: ensure we can exercise branch where (f1 & f2 & MEM_IntReal) != 0
  // We simulate by giving MEM_IntReal on one side and MEM_Int on the other side.
  {
    Mem m1, m2;
    initMem(m1, MEM_IntReal, 5, 0.0, ENCODING);
    initMem(m2, MEM_Int, 5, 0.0, ENCODING); // (f1 & f2 & MEM_Int) != 0 and (f2&MEM_Real) == 0
    int r = sqlite3MemCompare(&m1, &m2, nullptr);
    // Since both ints equal, but path uses MEM_Int & MEM_IntReal; if i's equal, returns 0
    EXPECT_TRUE(r == 0, "INTREAL(5) vs INT(5) should compare equal (branch MEM_IntReal)");
  }
}

// Entry point
int main() {
  // Run tests
  run_memcompare_numeric_tests();

  // Summary
  if( gFailures == 0 ){
    std::cout << "All sqlite3MemCompare tests passed. Total tests: " << gTests << std::endl;
    return 0;
  }else{
    std::cerr << gFailures << " test(s) failed out of " << gTests << "." << std::endl;
    return 1;
  }
}