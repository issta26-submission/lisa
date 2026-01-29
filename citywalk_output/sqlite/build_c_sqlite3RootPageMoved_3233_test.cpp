// Unit tests for sqlite3RootPageMoved (C function) using a lightweight C++11 test harness.
// The tests provide minimal in-process mocks for the function's internal dependencies
// (Hash data structures and accessors) to exercise the core logic without requiring the
// full SQLite build environment. This harness deliberately uses non-terminating checks
// (custom CHECK macros) so that multiple tests can run in a single run and report
// successes/failures without terminating the process on first failure.
//
// Important note: This test relies on the assumption that the internal layout of the
// lightweight fake structures used in the test aligns with the expectations of
// sqlite3RootPageMoved for the fields it touches (specifically tnum on Table and Index).
// We provide the minimal layout (tnum as the first member) to maximize compatibility
// with the function's simple member access. If the real project uses a different layout,
// this test might require adaptation to match the actual in-memory representations.

#include <iostream>
#include <sqliteInt.h>


// Domain/domain-knowledge-oriented notes (embedded as comments for maintainability):
// - We'll mock the following concepts used by sqlite3RootPageMoved:
//   - sqlite3 struct with a single aDb pointer to Db
//   - Db struct with a Schema pointer (pSchema)
//   - Schema struct with two Hash members: tblHash and idxHash
//   - Hash and HashElem linked-list implementation to simulate sqliteHashFirst/Next/Data
//   - Table and Index structs with a tnum member (Pgno alias) used by the function
// - We'll mock dependencies: sqlite3SchemaMutexHeld, sqliteHashFirst, sqliteHashNext, sqliteHashData
// - We'll declare sqlite3RootPageMoved with C linkage and provide a small test driver in main()

typedef unsigned int Pgno;

// Lightweight in-test data structures (matching the needs of sqlite3RootPageMoved)
struct HashElem {
  void* pData;
  HashElem* pNext;
};

struct Hash {
  HashElem* pHead;
};

struct Table {
  Pgno tnum; // Core field accessed by the function
  // Other fields are intentionally omitted for test simplicity
};

struct Index {
  Pgno tnum; // Core field accessed by the function
  // Other fields are intentionally omitted for test simplicity
};

struct Schema {
  Hash tblHash; // holds Table objects
  Hash idxHash; // holds Index objects
};

struct Db {
  Schema* pSchema;
};

struct sqlite3 {
  Db* aDb;
};

// Function prototype for the focal method (C linkage)
extern "C" void sqlite3RootPageMoved(sqlite3* db, int iDb, Pgno iFrom, Pgno iTo);

// Mocks for the dependencies used inside sqlite3RootPageMoved

extern "C" int sqlite3SchemaMutexHeld(sqlite3*db, int iDb, int unused) {
  // For testing we always pretend the mutex is held
  return 1;
}

// Hash accessors used by sqlite3RootPageMoved
extern "C" HashElem* sqliteHashFirst(Hash* pHash) {
  return (pHash ? pHash->pHead : nullptr);
}
extern "C" HashElem* sqliteHashNext(HashElem* pElem) {
  return (pElem ? pElem->pNext : nullptr);
}
extern "C" void* sqliteHashData(HashElem* pElem) {
  return (pElem ? pElem->pData : nullptr);
}

// Candidate helper macro for non-terminating checks
#define TEST_CHECK(cond, msg)                                            \
  do { if(!(cond)) {                                                  \
      std::cerr << "Test failure: " << (msg) << "\n";                 \
      ++g_failCount;                                               \
    } } while(0)

static int g_failCount = 0;

// Helper to construct a simple linked hash for Tables
static void linkTblHash(HashElem& a, Table& t1, HashElem& b, Table& t2) {
  a.pData = &t1; a.pNext = &b;
  b.pData = &t2; b.pNext = nullptr;
}

// Helper to construct a simple linked hash for Indices
static void linkIdxHash(HashElem& a, Index& i1, HashElem& b, Index& i2) {
  a.pData = &i1; a.pNext = &b;
  b.pData = &i2; b.pNext = nullptr;
}

// Basic test: move a single Table and a single Index from iFrom to iTo
void test_basic_move_both_present() {
  // Arrange
  const Pgno iFrom = 3;
  const Pgno iTo = 7;
  Table tabFrom; tabFrom.tnum = iFrom;
  Table tabOther; tabOther.tnum = 999;
  Index idxFrom; idxFrom.tnum = iFrom;
  Index idxOther; idxOther.tnum = 888;

  // Build hash chains
  HashElem tblElem1, tblElem2;
  linkTblHash(tblElem1, tabFrom, tblElem2, tabOther);
  Hash tblHash; tblHash.pHead = &tblElem1;

  HashElem idxElem1, idxElem2;
  linkIdxHash(idxElem1, idxFrom, idxElem2, idxOther);
  Hash idxHash; idxHash.pHead = &idxElem1;

  Schema schema;
  schema.tblHash = tblHash;
  schema.idxHash = idxHash;

  Db db0;
  db0.pSchema = &schema;

  sqlite3 db;
  db.aDb = &db0;

  // Act
  sqlite3RootPageMoved(&db, 0, iFrom, iTo);

  // Assert
  bool tblMoved = (tabFrom.tnum == iTo) && (tabOther.tnum == 999);
  bool idxMoved = (idxFrom.tnum == iTo) && (idxOther.tnum == 888);
  TEST_CHECK(tblMoved, "Table tnum should be moved from iFrom to iTo in tblHash");
  TEST_CHECK(idxMoved, "Index tnum should be moved from iFrom to iTo in idxHash");

  if(tblMoved && idxMoved) {
    std::cout << "PASS: test_basic_move_both_present\n";
  } else {
    std::cout << "FAIL: test_basic_move_both_present\n";
  }
}

// Test: move only in Index when Table has no matching tnum
void test_move_only_in_index() {
  // Arrange
  const Pgno iFrom = 5;
  const Pgno iTo = 9;
  Table tabFrom; tabFrom.tnum = 4; // not equal to iFrom
  Table tabOther; tabOther.tnum = 12;
  Index idxFrom; idxFrom.tnum = iFrom; // matches
  Index idxOther; idxOther.tnum = 77;

  HashElem tblElem1, tblElem2;
  linkTblHash(tblElem1, tabFrom, tblElem2, tabOther);
  Hash tblHash; tblHash.pHead = &tblElem1;

  HashElem idxElem1, idxElem2;
  linkIdxHash(idxElem1, idxFrom, idxElem2, idxOther);
  Hash idxHash; idxHash.pHead = &idxElem1;

  Schema schema;
  schema.tblHash = tblHash;
  schema.idxHash = idxHash;

  Db db0;
  db0.pSchema = &schema;

  sqlite3 db;
  db.aDb = &db0;

  // Act
  sqlite3RootPageMoved(&db, 0, iFrom, iTo);

  // Assert
  bool tableUnchanged = (tabFrom.tnum == 4) && (tabOther.tnum == 12);
  bool indexMoved = (idxFrom.tnum == iTo) && (idxOther.tnum == 77);
  TEST_CHECK(tableUnchanged, "Table should remain unchanged when iFrom not present in tblHash");
  TEST_CHECK(indexMoved, "Index tnum should be moved from iFrom to iTo in idxHash");
  if(tableUnchanged && indexMoved) {
    std::cout << "PASS: test_move_only_in_index\n";
  } else {
    std::cout << "FAIL: test_move_only_in_index\n";
  }
}

// Test: no matches anywhere (neither Table nor Index)
void test_no_matches() {
  // Arrange
  const Pgno iFrom = 11;
  const Pgno iTo = 22;
  Table tabFrom; tabFrom.tnum = 1;
  Table tabOther; tabOther.tnum = 2;
  Index idxFrom; idxFrom.tnum = 3;
  Index idxOther; idxOther.tnum = 4;

  HashElem tblElem1, tblElem2;
  linkTblHash(tblElem1, tabFrom, tblElem2, tabOther);
  Hash tblHash; tblHash.pHead = &tblElem1;

  HashElem idxElem1, idxElem2;
  linkIdxHash(idxElem1, idxFrom, idxElem2, idxOther);
  Hash idxHash; idxHash.pHead = &idxElem1;

  Schema schema;
  schema.tblHash = tblHash;
  schema.idxHash = idxHash;

  Db db0;
  db0.pSchema = &schema;

  sqlite3 db;
  db.aDb = &db0;

  // Act
  sqlite3RootPageMoved(&db, 0, iFrom, iTo);

  // Assert
  bool tblUnchanged = (tabFrom.tnum == 1) && (tabOther.tnum == 2);
  bool idxUnchanged = (idxFrom.tnum == 3) && (idxOther.tnum == 4);
  TEST_CHECK(tblUnchanged, "No Table tnum should be moved when iFrom not present");
  TEST_CHECK(idxUnchanged, "No Index tnum should be moved when iFrom not present");
  if(tblUnchanged && idxUnchanged) {
    std::cout << "PASS: test_no_matches\n";
  } else {
    std::cout << "FAIL: test_no_matches\n";
  }
}

// Main runner
int main() {
  std::cout << "Starting sqlite3RootPageMoved unit tests (lightweight harness)..." << std::endl;

  test_basic_move_both_present();
  test_move_only_in_index();
  test_no_matches();

  if(g_failCount == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cout << g_failCount << " TEST(S) FAILED" << std::endl;
    return 1;
  }
}