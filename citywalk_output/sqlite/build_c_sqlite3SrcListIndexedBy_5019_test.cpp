// Lightweight C++11 test harness for sqlite3SrcListIndexedBy (C function)
// This test suite is designed to compile with the given focal method
// located in build.c and its dependencies, using only the C/C++ standard
// library. We provide minimal type definitions that match the usage in the
// focal method so the function can be exercised in a controlled manner.
// Note: This test uses a small, self-contained stub for sqlite3NameFromToken
// to avoid depending on the full SQLite build. The test ensures coverage of
// true/false branches and validates side effects on SrcList/SrcItem.

// Candidate Keywords (Step 1): Parse, SrcList, SrcItem, Token, pIndexedBy, fg.notIndexed,
// fg.isIndexedBy, fg.isTabFunc, u1.zIndexedBy, sqlite3NameFromToken, db, pParse.

#include <cstdio>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Minimal type declarations to mirror the usage inside sqlite3SrcListIndexedBy.
// These definitions are crafted to align with the focal method's expectations.

typedef struct sqlite3 sqlite3; // Opaque sqlite3 structure (stub)
typedef struct Parse {
  sqlite3 *db;
} Parse;

typedef struct Token {
  int n;         // token length
  const char *z; // token text (may be NULL for NOT INDEXED case)
} Token;

typedef struct SrcItemFg {
  int notIndexed;
  int isIndexedBy;
  int isTabFunc;
  int isCte;
} SrcItemFg;

typedef struct SrcItem {
  SrcItemFg fg;
  struct {
    char *zIndexedBy;
  } u1;
} SrcItem;

typedef struct SrcList {
  int nSrc;
  SrcItem *a; // array of SrcItem
} SrcList;

// Forward declaration of the focal function (to be linked with build.c)
extern "C" void sqlite3SrcListIndexedBy(Parse *pParse, SrcList *p, Token *pIndexedBy);

// Forward declaration of sqlite3NameFromToken used by the focal method.
// We provide a small test stub here to produce deterministic outputs.
extern "C" char *sqlite3NameFromToken(sqlite3 *db, const Token *pName);

// ===== Test Harness =====

static int gFailures = 0;

// Helper: compare strings safely
static bool strEq(const char* a, const char* b) {
  if (a == nullptr && b == nullptr) return true;
  if (a == nullptr || b == nullptr) return false;
  return std::strcmp(a, b) == 0;
}

// Stub for sqlite3NameFromToken to support test scenarios.
// Returns a freshly allocated C-string based on the token's content.
extern "C" char *sqlite3NameFromToken(sqlite3 *db, const Token *pName) {
  (void)db; // unused in test stub
  if (pName && pName->z) {
    return std::strdup(pName->z);
  }
  // If z is NULL, return a deterministic placeholder
  return std::strdup("IndexedBy");
}

// Helper: create a simple Parse object with a dummy sqlite3 db pointer
static Parse makeParse(sqlite3* db) {
  Parse p;
  p.db = db;
  return p;
}

// Helper: create a SrcList with a single SrcItem initialized for testing
static SrcList makeSrcListOneItem() {
  SrcList list;
  list.nSrc = 1;
  list.a = new SrcItem[1];
  // Initialize the single SrcItem to match the preconditions of the focal method
  list.a[0].fg.notIndexed = 0;
  list.a[0].fg.isIndexedBy = 0;
  list.a[0].fg.isTabFunc = 0;
  list.a[0].fg.isCte = 0;
  list.a[0].u1.zIndexedBy = nullptr;
  return list;
}

// Helper: free SrcList resources
static void freeSrcList(SrcList &list) {
  if (list.a) {
    // If zIndexedBy was allocated, free it
    if (list.a[0].u1.zIndexedBy) {
      std::free(list.a[0].u1.zIndexedBy);
      list.a[0].u1.zIndexedBy = nullptr;
    }
    delete[] list.a;
    list.a = nullptr;
  }
  list.nSrc = 0;
}

// Test 1: NOT INDEXED path when pIndexedBy->n == 1 and pIndexedBy->z == NULL
// Expect: last SrcItem's fg.notIndexed becomes 1; isIndexedBy remains 0; zIndexedBy untouched.
static void test_notIndexedPath() {
  // Setup
  SrcList list = makeSrcListOneItem();
  Token idxBy;
  idxBy.n = 1;
  idxBy.z = nullptr; // NULL triggers NOT INDEXED branch
  Parse pParse = makeParse(nullptr);

  // Call
  sqlite3SrcListIndexedBy(&pParse, &list, &idxBy);

  // Verify
  SrcItem &pItem = list.a[list.nSrc - 1];
  bool pass = (pItem.fg.notIndexed == 1) &&
              (pItem.fg.isIndexedBy == 0) &&
              (pItem.fg.isTabFunc == 0) &&
              (pItem.u1.zIndexedBy == nullptr);

  std::cout << "test_notIndexedPath: " << (pass ? "PASS" : "FAIL") << "\n";
  if (!pass) {
    gFailures++;
  }

  freeSrcList(list);
}

// Test 2: IndexedBy path when pIndexedBy->n == 1 and pIndexedBy->z != NULL
// Expect: last SrcItem's fg.isIndexedBy becomes 1; zIndexedBy points to token name.
static void test_indexedByPath() {
  // Setup
  SrcList list = makeSrcListOneItem();
  Token idxBy;
  idxBy.n = 1;
  idxBy.z = "idx_col"; // non-NULL triggers else path
  Parse pParse = makeParse(nullptr);

  // Call
  sqlite3SrcListIndexedBy(&pParse, &list, &idxBy);

  // Verify
  SrcItem &pItem = list.a[list.nSrc - 1];
  bool pass = (pItem.fg.isIndexedBy == 1) &&
              (pItem.fg.notIndexed == 0) && // should remain notIndexed false
              (pItem.fg.isTabFunc == 0) &&
              (pItem.u1.zIndexedBy != nullptr) &&
              (std::strcmp(pItem.u1.zIndexedBy, "idx_col") == 0);

  std::cout << "test_indexedByPath: " << (pass ? "PASS" : "FAIL") << "\n";
  if (!pass) {
    gFailures++;
  }

  // Cleanup allocated zIndexedBy
  if (pItem.u1.zIndexedBy) {
    std::free(pItem.u1.zIndexedBy);
    pItem.u1.zIndexedBy = nullptr;
  }

  freeSrcList(list);
}

// Test 3: No-op when p is NULL
// Expect: function returns without modifying any memory or triggering asserts
static void test_noOpWhenPNull() {
  // Setup
  SrcList list = makeSrcListOneItem();
  Token idxBy;
  idxBy.n = 2;          // any positive value
  idxBy.z = "any";      // z non-NULL
  Parse pParse = makeParse(nullptr);

  // Call with p == NULL
  sqlite3SrcListIndexedBy(&pParse, nullptr, &idxBy);

  // Verify: list should be untouched (no access occurs)
  // Since p is NULL, there is nothing to modify; ensure item remains in initial state
  SrcItem &pItem = list.a[list.nSrc - 1];
  bool pass = (pItem.fg.notIndexed == 0) &&
              (pItem.fg.isIndexedBy == 0) &&
              (pItem.fg.isTabFunc == 0) &&
              (pItem.u1.zIndexedBy == nullptr);

  std::cout << "test_noOpWhenPNull: " << (pass ? "PASS" : "FAIL") << "\n";
  if (!pass) {
    gFailures++;
  }

  freeSrcList(list);
}

// Entry point: run all tests and summarize results
int main() {
  // Run tests
  test_notIndexedPath();
  test_indexedByPath();
  test_noOpWhenPNull();

  // Summary
  if (gFailures == 0) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cout << gFailures << " TEST(S) FAILED\n";
    return 1;
  }
}