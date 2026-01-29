// Unit test suite for sqlite3RenameTokenMap (alter.c) using C++11, without GTest.
// This test suite is designed to be linked with a minimal mock of the SQLite-like environment
// to exercise the focal function's behavior under different scenarios.
// Note: We override sqlite3DbMallocZero with a test-provided implementation to simulate
// allocation success/failure and to control mallocFailed semantics.

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>
#include <cstring>


// Forward declare the function under test with C linkage for compatibility with alter.c
extern "C" const void *sqlite3RenameTokenMap(struct Parse *pParse, const void *pPtr, const struct Token *pToken);

// Lightweight mock of the SQLite structures needed by the focal function.
// We keep the layout minimal but compatible with how alter.c will access fields.

struct sqlite3 {
  int mallocFailed; // Simulated malloc failure flag used by the test boilerplate
};

struct Token {
  int a; // simple payload fields to verify copy semantics
  int b;
};

struct RenameToken {
  void *p;
  Token t;
  RenameToken *pNext;
};

// Minimal Parse struct matching the fields accessed by sqlite3RenameTokenMap.
// The real SQLite Parse has many more fields; we only include what is needed by the test.
struct Parse {
  sqlite3 *db;
  int eParseMode; // value compared against PARSE_MODE_UNMAP inside sqlite3RenameTokenMap
  void *pRename;  // head of the RenameToken list
};

// Provide a test-controlled override of sqlite3DbMallocZero used by alter.c.
// This allows us to simulate both successful allocations and allocation failures.
extern "C" void *sqlite3DbMallocZero(struct sqlite3 *db, size_t n);

static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "Test failure: " << msg << std::endl; \
    ++g_failures; \
  } \
} while(0)

// Test 1: Allocation path should create a new RenameToken, push it onto the head, and copy values.
static void test_allocation_creates_token() {
  // Setup environment
  sqlite3 db; db.mallocFailed = 0;
  Parse p; p.db = &db; p.eParseMode = 0; p.pRename = nullptr;

  int payload = 0xDEADBEEF;
  Token tok; tok.a = 11; tok.b = 22;
  void *pPtr = &payload;

  const void *ret = sqlite3RenameTokenMap(&p, pPtr, &tok);

  TEST_ASSERT(ret == pPtr, "Returned pointer should be the same as input pointer");

  RenameToken *head = reinterpret_cast<RenameToken*>(p.pRename);
  TEST_ASSERT(head != nullptr, "pRename should be non-null after allocation");
  TEST_ASSERT(head->p == pPtr, "RenameToken.p should point to original pPtr");
  TEST_ASSERT(head->t.a == tok.a && head->t.b == tok.b,
              "RenameToken.t should copy Token contents from input");
  TEST_ASSERT(head->pNext == nullptr, "New head's pNext should be null (no existing list)");
}

// Test 2: Multiple allocations should form a chain where new head points to previous head.
static void test_chain_linking() {
  // First insertion
  sqlite3 db; db.mallocFailed = 0;
  Parse p; p.db = &db; p.eParseMode = 0; p.pRename = nullptr;
  int v1 = 0x11111111;
  Token t1; t1.a = 1; t1.b = 10;
  void *p1 = &v1;
  sqlite3RenameTokenMap(&p, p1, &t1);
  // Second insertion
  int v2 = 0x22222222;
  Token t2; t2.a = 2; t2.b = 20;
  void *p2 = &v2;

  const void *ret2 = sqlite3RenameTokenMap(&p, p2, &t2);

  TEST_ASSERT(ret2 == p2, "Second return should echo the input pointer");

  RenameToken *head = reinterpret_cast<RenameToken*>(p.pRename);
  TEST_ASSERT(head != nullptr, "New head should be created on second insertion");
  TEST_ASSERT(head->p == p2, "Head should point to second pPtr");

  RenameToken *oldHead = head->pNext;
  TEST_ASSERT(oldHead != nullptr, "Old head should become next in chain");
  TEST_ASSERT(oldHead->p == p1, "Old head should point to first pPtr");

  TEST_ASSERT(oldHead->t.a == t1.a && oldHead->t.b == t1.b,
              "Old head should preserve first Token data");
}

// Test 3: When pPtr is null and malloc fails, function should return null and not crash.
static void test_null_ptr_with_malloc_failed() {
  sqlite3 db; db.mallocFailed = 1; // simulate malloc failure
  Parse p; p.db = &db; p.eParseMode = 0; p.pRename = nullptr;

  Token tok; tok.a = 99; tok.b = 1;
  void *pPtr = nullptr;

  const void *ret = sqlite3RenameTokenMap(&p, pPtr, &tok);

  TEST_ASSERT(ret == nullptr, "Return should be null when pPtr is null even if malloc fails");
  TEST_ASSERT(p.pRename == nullptr, "No RenameToken should be created when pPtr is null");
}

// Test 4: When malloc fails (even with non-null pPtr), the function should return pPtr
// and not modify the Rename list.
static void test_allocation_failure_does_not_modify_list() {
  sqlite3 db; db.mallocFailed = 1;
  Parse p; p.db = &db; p.eParseMode = 0; p.pRename = nullptr;

  int payload = 0xABCDEF;
  void *pPtr = &payload;
  Token tok; tok.a = 5; tok.b = 7;

  const void *ret = sqlite3RenameTokenMap(&p, pPtr, &tok);

  TEST_ASSERT(ret == pPtr, "Return value should be the original pointer even if malloc fails");
  TEST_ASSERT(p.pRename == nullptr, "No token should be added when allocation fails");
}

// Test harness: provide a mock for sqlite3DbMallocZero used by alter.c
extern "C" void *sqlite3DbMallocZero(struct sqlite3 *db, size_t n) {
  // If mallocFailed is set, simulate allocation failure
  if (db && db->mallocFailed) return NULL;
  // Otherwise allocate and zero initialize
  void *ptr = std::malloc(n);
  if (ptr && n > 0) std::memset(ptr, 0, n);
  return ptr;
}

// Entry point
int main() {
  test_allocation_creates_token();
  test_chain_linking();
  test_null_ptr_with_malloc_failed();
  test_allocation_failure_does_not_modify_list();

  if (g_failures == 0) {
    std::cout << "All tests passed.\n";
    return 0;
  } else {
    std::cerr << g_failures << " test(s) failed.\n";
    return 1;
  }
}