// Test suite for sqlite3SelectDup in expr.c
// C++11 test harness (no GoogleTest). Uses manual EXPECT-like assertions.
// The test targets the behavior of sqlite3SelectDup(db, pDup, flags) including:
// - Deep-copy of the Select object chain
// - Correct handling of allocation failure (OOM) path
// - Optional window-function branch coverage
// Notes:
// - This harness provides minimal in-project stub implementations for dependencies
//   (ExprListDup, SrcListDup, ExprDup, WithDup, WindowListDup, etc.) so that
//   the focal method can be exercised in isolation from the full SQLite codebase.
// - The actual Select structure layout is approximated to cover fields accessed by
//   sqlite3SelectDup. The test focuses on pointer behavior and control flow rather than
//   full semantics of a real SQLite build.
// - The real sqlite3SelectDup is assumed to be linked from expr.c or a compatible object.

#include <vector>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Domain knowledge: static members of the focal file are to be treated as internal.
// We provide minimal stand-ins to exercise the function under test.

// Forward-declare the C-namespace focal API (as in the original SQLite code)
extern "C" {
  struct sqlite3;
  struct Select;
  struct ExprList;
  struct SrcList;
  struct Expr;
  struct With;

  // Focal function under test (signature taken from the provided snippet)
  Select* sqlite3SelectDup(sqlite3 *db, const Select *pDup, int flags);

  // Dependency stubs (names match what sqlite3SelectDup calls)
  struct ExprList* sqlite3ExprListDup(sqlite3*, const struct ExprList*, int);
  struct SrcList*  sqlite3SrcListDup(sqlite3*, const struct SrcList*, int);
  struct Expr*     sqlite3ExprDup(sqlite3*, const struct Expr*, int);
  struct With*      sqlite3WithDup(sqlite3*, const struct With*);
#ifndef SQLITE_OMIT_WINDOWFUNC
  // Window-related helpers
  void gatherSelectWindows(struct Select*);
  void* sqlite3WindowListDup(sqlite3*, const void*);
#endif
  // Utility used within the focal function for cleanup on OOM
  void sqlite3SelectDelete(sqlite3*, struct Select*);
  // Memory allocator used by sqlite3SelectDup (we override for test)
  void* sqlite3DbMallocRawNN(sqlite3*, size_t);
}

// Simple test assertion utilities (non-terminating)
static int g_test_failures = 0;
#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_test_failures; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if((a) != (b)) { \
    std::cerr << "EXPECT_EQ failed: " #a " == " #b " (" << (a) << " != " << (b) \
              << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_test_failures; \
  } \
} while(0)

#define EXPECT_PTR_NE(a,b) do { \
  if((void*)(a) == (void*)(b)) { \
    std::cerr << "EXPECT_PTR_NE failed: pointers equal (" << a << ") at " \
              << __FILE__ << ":" << __LINE__ << "\n"; \
    ++g_test_failures; \
  } \
} while(0)

#define LOG(msg) do { std::cout << "[LOG] " << msg << "\n"; } while(0)

// Approximate structure layout used by focal function.
// We only include fields accessed/used by sqlite3SelectDup in the provided snippet.
struct sqlite3 {
  int mallocFailed; // test switch to simulate OOM
};

// Minimal type placeholders for the dependent structures.
// The real SQLite types are far richer; here we only need opaque pointers to satisfy
// the API surface and allow pointer-based checks in tests.
struct ExprList { int dummy; };
struct SrcList  { int dummy; };
struct Expr     { int dummy; };
struct With     { int dummy; };

// The Select structure layout (approximate). Only members accessed by sqlite3SelectDup are included.
struct Select {
  Select *pPrior;
  Select *pNext;
  ExprList *pEList;
  SrcList  *pSrc;
  Expr     *pWhere;
  ExprList *pGroupBy;
  Expr     *pHaving;
  ExprList *pOrderBy;
  int op;
  Expr     *pLimit;
  int iLimit;
  int iOffset;
  unsigned int selFlags;
  int addrOpenEphm[2];
  int nSelectRow;
  With *pWith;
#ifndef SQLITE_OMIT_WINDOWFUNC
  void *pWin;
  void *pWinDefn;
#endif
  int selId;
};

// Ephemeral flag mask used by sqlite3SelectDup (from snippet SF_UsesEphemeral)
static const unsigned int SF_UsesEphemeral = 0x01;

// Globals to help verify behavior
static bool g_windows_gathered = false;
static Select* g_last_deleted = nullptr;
static Select* g_first_linked = nullptr;

// Implementations of dependency stubs (simple, shallow copies)
extern "C" {

Select* sqlite3SelectDup(sqlite3 *db, const Select *pDup, int flags);
ExprList* sqlite3ExprListDup(sqlite3*, const ExprList*, int);
SrcList*  sqlite3SrcListDup(sqlite3*, const SrcList*, int);
Expr*     sqlite3ExprDup(sqlite3*, const Expr*, int);
With*     sqlite3WithDup(sqlite3*, const With*);
#ifndef SQLITE_OMIT_WINDOWFUNC
void gatherSelectWindows(Select* p);
void* sqlite3WindowListDup(sqlite3*, const void*);
#endif
void sqlite3SelectDelete(sqlite3*, Select*);
void* sqlite3DbMallocRawNN(sqlite3*, size_t);
} // extern "C"

// Global allocation counter to simulate OOM after certain allocations
static int g_alloc_counter = 0;
static int g_alloc_fail_after = -1;

// Fallback implementations to satisfy linker when building with actual expr.c
extern "C" {

Select* sqlite3SelectDup(sqlite3 *db, const Select *pDup, int flags) {
  // The actual logic is in expr.c; here we proxy-run to the real function.
  // In this harness, a real expr.c would be linked; this stub exists to allow
  // linking in environments where we provide the minimal scaffolding above.
  // For the purposes of unit testing, we rely on the presence of the real
  // implementation during compilation/execution.
  // If the real implementation is linked, it will override this stub.
  return nullptr;
}

ExprList* sqlite3ExprListDup(sqlite3*, const ExprList*, int) { return new ExprList(); }
SrcList*  sqlite3SrcListDup(sqlite3*, const SrcList*, int) { return new SrcList(); }
Expr*     sqlite3ExprDup(sqlite3*, const Expr*, int) { return new Expr(); }
With*     sqlite3WithDup(sqlite3*, const With*) { return new With(); }

#ifndef SQLITE_OMIT_WINDOWFUNC
void gatherSelectWindows(Select* /*p*/) { g_windows_gathered = true; }
void* sqlite3WindowListDup(sqlite3*, const void*) { return (void*)0xDEADBEEF; }
#endif

void sqlite3SelectDelete(sqlite3* /*db*/, Select* p) {
  // Free the Select node and its linked list (pNext/pPrior are set up by test)
  // For safety, set a marker for validation
  if(p){
    g_last_deleted = p;
    delete p;
  }
}

void* sqlite3DbMallocRawNN(sqlite3* db, size_t n) {
  // Simple allocator with OOM control via db->mallocFailed and global counter
  if (db && db->mallocFailed) return 0;
  ++g_alloc_counter;
  void* p = malloc(n);
  if (!p) {
    if (db) db->mallocFailed = 1;
    return 0;
  }
  // Simulate an OOM condition after a fixed number of allocations (if requested)
  if (g_alloc_fail_after >= 0 && g_alloc_counter > g_alloc_fail_after) {
    // mimic an allocated object but mark as failed for subsequent usage
    if (db) db->mallocFailed = 1;
  }
  return p;
}
} // extern "C"

// Test helpers to construct a minimal Select chain and related structures
static Select* make_select_chain_one() {
  Select* p = new Select();
  p->pPrior = nullptr;
  p->pNext  = nullptr;
  p->pEList = new ExprList();
  p->pSrc   = new SrcList();
  p->pWhere = new Expr();
  p->pGroupBy = new ExprList();
  p->pHaving = new Expr();
  p->pOrderBy = new ExprList();
  p->op = 123;
  p->pLimit = new Expr();
  p->iLimit = 0;
  p->iOffset = 0;
  p->selFlags = 0;
  p->addrOpenEphm[0] = -2;
  p->addrOpenEphm[1] = -2;
  p->nSelectRow = 777;
  p->pWith = new With();
#ifndef SQLITE_OMIT_WINDOWFUNC
  p->pWin = (void*)1;
  p->pWinDefn = new int(0);
#endif
  p->selId = 999;
  return p;
}

static Select* make_select_chain_two() {
  Select* head = make_select_chain_one();
  Select* second = new Select();
  second->pPrior = nullptr;
  second->pNext  = nullptr;
  second->pEList = new ExprList();
  second->pSrc   = new SrcList();
  second->pWhere = new Expr();
  second->pGroupBy = new ExprList();
  second->pHaving = new Expr();
  second->pOrderBy = new ExprList();
  second->op = 456;
  second->pLimit = new Expr();
  second->iLimit = 0;
  second->iOffset = 0;
  second->selFlags = 0;
  second->addrOpenEphm[0] = -2;
  second->addrOpenEphm[1] = -2;
  second->nSelectRow = 888;
  second->pWith = new With();
#ifndef SQLITE_OMIT_WINDOWFUNC
  second->pWin = (void*)1;
  second->pWinDefn = new int(0);
#endif
  second->selId = 1001;
  // Link into a chain: first->pPrior = 0, first->pNext = second
  head->pNext = second;
  second->pPrior = head;
  return head;
}

// Cleanup helper for test results
static void delete_select_chain(Select* p) {
  while (p) {
    Select* cur = p;
    p = p->pNext;
    // Free members (best-effort minimal cleanup)
    delete cur->pEList;
    delete cur->pSrc;
    delete cur->pWhere;
    delete cur->pGroupBy;
    delete cur->pHaving;
    delete cur->pOrderBy;
    delete cur->pLimit;
#ifndef SQLITE_OMIT_WINDOWFUNC
    // pWinDefn is allocated; delete if present
    if (cur->pWinDefn) delete (int*)cur->pWinDefn;
#endif
    delete cur->pWith;
    delete cur;
  }
  // reset globals
  g_first_linked = nullptr;
  g_last_deleted = nullptr;
  g_windows_gathered = false;
}

// Test 1: Basic single-element duplication (no OOM, no window path) //
void test_basic_dup_single() {
  LOG("Test 1: Basic single-element duplication (no OOM)");
  // Prepare a single Select as pDup
  Select* pDup = make_select_chain_one();
  // Prepare a fake db
  sqlite3 db;
  db.mallocFailed = 0;
  // Ensure window function macro is active (we do not set SQLITE_OMIT_WINDOWFUNC)
  // Call focal function
  Select* pRet = sqlite3SelectDup(&db, pDup, 0);

  // Assertions
  EXPECT_TRUE(pRet != nullptr);
  if(pRet){
    EXPECT_EQ(pRet->op, pDup->op);
    EXPECT_PTR_NE(pRet, pDup);
    EXPECT_TRUE(pRet->pPrior == nullptr);
    EXPECT_TRUE(pRet->pNext == nullptr);
    // Pointers to substructures should be newly allocated (deep copy)
    EXPECT_TRUE(pRet->pEList != pDup->pEList);
    EXPECT_TRUE(pRet->pSrc != pDup->pSrc);
    EXPECT_TRUE(pRet->pWhere != pDup->pWhere);
    EXPECT_TRUE(pRet->pGroupBy != pDup->pGroupBy);
    EXPECT_TRUE(pRet->pHaving != pDup->pHaving);
    EXPECT_TRUE(pRet->pOrderBy != pDup->pOrderBy);
    EXPECT_TRUE(pRet->pLimit != pDup->pLimit);
#ifndef SQLITE_OMIT_WINDOWFUNC
    // pWin is forced to 0 in the dup; pWinDefn may be non-null
    // We only ensure no crash and non-null pWinDefn if present in original chain
    // (the exact value is not critical for this test)
#endif
    EXPECT_TRUE(pRet->selId == pDup->selId);
  }

  // Cleanup
  sqlite3SelectDelete(&db, pRet);
  delete_select_chain(pDup);
  LOG("Test 1 completed.");
}

// Test 2: Allocation failure (OOM) path on second element of a two-element chain //
void test_dup_oom_on_second() {
  LOG("Test 2: OOM path when duplicating a two-element chain (OOM on second)");
  // Build a two-element chain
  Select* pDup = make_select_chain_two(); // head->pNext != null
  // Prepare a fake db
  sqlite3 db;
  db.mallocFailed = 0;
  // Configure allocator to trigger OOM on the second allocation
  g_alloc_counter = 0;
  g_alloc_fail_after = 1; // fail after 1 successful allocation (i.e., on 2nd alloc)

  // Run
  Select* pRet = sqlite3SelectDup(&db, pDup, 0);

  // Assertions:
  // - First element should be linked in result
  // - Second element allocation may fail; on OOM, second is deleted and loop breaks
  EXPECT_TRUE(pRet != nullptr);
  if(pRet){
    // After OOM on second element, the first element should be the only one in result
    EXPECT_TRUE(pRet->pPrior == nullptr);
    // pRet should be the first element from the original chain
    // The first element's fields should have been copied
    EXPECT_TRUE(pRet->pNext == nullptr); // second not linked due to OOM
  }

  // Cleanup: ensure deletion of the first (and its copied substructures)
  sqlite3SelectDelete(&db, pRet);
  // second element would have been deleted inside the focal function upon OOM
  delete_select_chain(pDup); // free original chain as well
  g_alloc_fail_after = -1; // reset
  LOG("Test 2 completed.");
}

// Test 3: Window-function path coverage (gatherSelectWindows invoked) //
void test_dup_with_window_path() {
  LOG("Test 3: Window-function path triggered in duplication");
  // Build a single Select as pDup with pWin non-null to exercise window path
  Select* pDup = make_select_chain_one();
  pDup->pWin = (void*)1; // non-null to trigger window path
  // Prepare a fake db
  sqlite3 db;
  db.mallocFailed = 0;

  g_windows_gathered = false;
  Select* pRet = sqlite3SelectDup(&db, pDup, 0);

  EXPECT_TRUE(pRet != nullptr);
  if(pRet){
    // ensure gatherSelectWindows was invoked
    // The function sets a global flag
    EXPECT_TRUE(g_windows_gathered);
  }

  // Cleanup
  sqlite3SelectDelete(&db, pRet);
  delete_select_chain(pDup);
  LOG("Test 3 completed.");
}

// Entry point
int main() {
  std::cout << "sqlite3SelectDup unit tests (C++11 harness) started.\n";

  test_basic_dup_single();
  test_dup_oom_on_second();
  test_dup_with_window_path();

  std::cout << "Tests completed. Failures: " << g_test_failures << "\n";

  // Cleanup any global memory
  return g_test_failures ? 1 : 0;
}