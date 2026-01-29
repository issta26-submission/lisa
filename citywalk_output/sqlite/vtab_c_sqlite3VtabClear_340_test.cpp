// Lightweight C++11 unit tests for sqlite3VtabClear (re-implemented locally for testability).
// This test suite is designed to run without GoogleTest. It uses a tiny assertion helper
// to log non-terminating test results and a minimal mock of the needed SQLite-like APIs.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <vector>


//-------------------------------------------------------------------------------------
// Mocked/Minimal Dependency Primitives to support sqlite3VtabClear under test
//-------------------------------------------------------------------------------------

// Forward declarations to resemble the real signatures used by sqlite3VtabClear
struct sqlite3;
struct Table;
struct VTable;

// Simple VTable placeholder (actual contents are irrelevant for the test)
struct VTable {};

// Global counters to observe internal calls
static int g_vtabDisconnectAll_calls = 0;
static int g_sqlite3DbFree_calls = 0;
static std::vector<void*> g_freed_ptrs; // track freed pointers for verification

// Minimal sqlite3-like db object
struct sqlite3 {
  int pnBytesFreed;
};

// Table and nested vtab structure to satisfy sqlite3VtabClear usage
struct Table {
  struct {
    char **azArg; // array of argument strings
    int nArg;     // number of arguments
  } u;
};

// Utility: IsVirtual stubbed to always return true for testing
static bool IsVirtual(Table* /*p*/) {
  return true;
}

// Stubbed vtabDisconnectAll to observe its invocation without implementing full logic
static VTable* vtabDisconnectAll(sqlite3 * /*db*/, Table * /*p*/) {
  ++g_vtabDisconnectAll_calls;
  return nullptr;
}

// Memory free tracker to emulate sqlite3DbFree
static void sqlite3DbFree(sqlite3 * /*db*/, void *p) {
  if(p != nullptr){
    // Record the freed pointer and free as a C-style array to mimic sqlite's allocator usage
    g_sqlite3DbFree_calls++;
    g_freed_ptrs.push_back(p);
    delete[] reinterpret_cast<char*>(p);
  }
}

// The focal method under test, implemented identically to the provided snippet.
// It relies on the mocks above for dependencies.
void sqlite3VtabClear(sqlite3 *db, Table *p){
  // Mirror the exact behavior with the mocks
  assert( IsVirtual(p) );
  assert( db!=0 );
  if( db->pnBytesFreed==0 ) vtabDisconnectAll(0, p);
  if( p->u.vtab.azArg ){
    int i;
    for(i=0; i<p->u.vtab.nArg; i++){
      if( i!=1 ) sqlite3DbFree(db, p->u.vtab.azArg[i]);
    }
    sqlite3DbFree(db, p->u.vtab.azArg);
  }
}

//-------------------------------------------------------------------------------------
// Lightweight test framework scaffolding (non-terminating style)
//-------------------------------------------------------------------------------------

static int g_tests_run = 0;
static int g_tests_failed = 0;

#define LOG_FATAL(msg) do { std::cerr << "FATAL: " << msg << std::endl; } while(0)
#define EXPECT_TRUE(cond) \
  do { \
    ++g_tests_run; \
    if(!(cond)) { \
      std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                << " — condition: " #cond << std::endl; \
      ++g_tests_failed; \
    } \
  } while(0)
#define EXPECT_EQ(a, b) \
  do { \
    ++g_tests_run; \
    if(!((a) == (b))) { \
      std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                << " — " #a " != " #b " (" << (a) << " vs " << (b) << ")" << std::endl; \
      ++g_tests_failed; \
    } \
  } while(0)
#define EXPECT_PTR_EQ(a, b) \
  do { \
    ++g_tests_run; \
    if((void*)(a) != (void*)(b)) { \
      std::cerr << "EXPECT_PTR_EQ failed at " << __FILE__ << ":" << __LINE__ \
                << " — pointers differ: " << (void*)(a) << " != " << (void*)(b) << std::endl; \
      ++g_tests_failed; \
    } \
  } while(0)

// Helper to reset global counters before each test
static void resetGlobals() {
  g_vtabDisconnectAll_calls = 0;
  g_sqlite3DbFree_calls = 0;
  g_freed_ptrs.clear();
}

// Helper to access freed pointers vector (read-only view)
static const std::vector<void*>& freedPointers() { return g_freed_ptrs; }

//-------------------------------------------------------------------------------------
// Test Cases
// Each test sets up a minimal sqlite3 and Table, then calls sqlite3VtabClear
// and asserts on expected side effects (branch coverage, memory frees, etc.)
//-------------------------------------------------------------------------------------

// Test Case 1: azArg == nullptr and db->pnBytesFreed == 0
// Expected: vtabDisconnectAll called exactly once, no sqlite3DbFree invocations.
static void testCase1_AzArgNull_PnBytesFreedZero() {
  resetGlobals();

  sqlite3 db;
  db.pnBytesFreed = 0;

  Table p;
  p.u.vtab.azArg = nullptr;
  p.u.vtab.nArg = 0;

  sqlite3VtabClear(&db, &p);

  EXPECT_EQ(g_vtabDisconnectAll_calls, 1);
  EXPECT_EQ(g_sqlite3DbFree_calls, 0);
  EXPECT_EQ(g_freed_ptrs.size(), 0);
}

// Test Case 2: azArg == nullptr but db->pnBytesFreed != 0
// Expected: vtabDisconnectAll not called.
static void testCase2_AzArgNull_PnBytesFreedNonZero() {
  resetGlobals();

  sqlite3 db;
  db.pnBytesFreed = 12345;

  Table p;
  p.u.vtab.azArg = nullptr;
  p.u.vtab.nArg = 0;

  sqlite3VtabClear(&db, &p);

  EXPECT_EQ(g_vtabDisconnectAll_calls, 0);
  EXPECT_EQ(g_sqlite3DbFree_calls, 0);
  EXPECT_EQ(g_freed_ptrs.size(), 0);
}

// Test Case 3: azArg non-null with 3 arguments; ensure i != 1 frees arg[0] and arg[2], but not arg[1]
// Also exercise the first branch (pnBytesFreed == 0)
static void testCase3_AzArg3Args_FreesSelectedAndAzArg() {
  resetGlobals();

  sqlite3 db;
  db.pnBytesFreed = 0;

  // Allocate argument strings
  char* s0 = new char[6]; std::strcpy(s0, "arg0");
  char* s1 = new char[6]; std::strcpy(s1, "arg1"); // this one should NOT be freed by sqlite3VtabClear
  char* s2 = new char[6]; std::strcpy(s2, "arg2");

  // Allocate azArg array
  char** arg = new char*[3];
  arg[0] = s0;
  arg[1] = s1;
  arg[2] = s2;

  Table p;
  p.u.vtab.azArg = arg;
  p.u.vtab.nArg = 3;

  sqlite3VtabClear(&db, &p);

  // vtabDisconnectAll should be called due to pnBytesFreed == 0
  EXPECT_EQ(g_vtabDisconnectAll_calls, 1);
  // arg[0], arg[2], and azArg array should be freed; arg[1] must NOT be freed
  EXPECT_EQ(g_sqlite3DbFree_calls, 3);
  // Check that freed pointers include s0, s2, and the azArg array pointer
  const std::vector<void*>& freed = freedPointers();
  bool found_s0 = false, found_s2 = false, found_azArg = false;
  for (void* pfp : freed) {
    if (pfp == static_cast<void*>(s0)) found_s0 = true;
    if (pfp == static_cast<void*>(s2)) found_s2 = true;
    if (pfp == static_cast<void*>(arg)) found_azArg = true;
  }
  EXPECT_TRUE(found_s0);
  EXPECT_TRUE(found_s2);
  EXPECT_TRUE(found_azArg);
  // arg[1] memory should remain allocated (not freed here)
  // We'll clean up arg[1] manually to avoid leaks after test ends
  delete[] s1;
}

// Test Case 4: azArg non-null with 1 argument (edge case for loop condition i != 1 discriminant)
// Expected: arg[0] freed, azArg freed; vtabDisconnectAll invoked since pnBytesFreed == 0
static void testCase4_AzArgSingleArg_FreesSingleThenAzArg() {
  resetGlobals();

  sqlite3 db;
  db.pnBytesFreed = 0;

  char* s0 = new char[5]; std::strcpy(s0, "arg0");
  char** arg = new char*[1];
  arg[0] = s0;

  Table p;
  p.u.vtab.azArg = arg;
  p.u.vtab.nArg = 1;

  sqlite3VtabClear(&db, &p);

  // Should call vtabDisconnectAll and free arg[0] and azArg
  EXPECT_EQ(g_vtabDisconnectAll_calls, 1);
  EXPECT_EQ(g_sqlite3DbFree_calls, 2);
  const std::vector<void*>& freed = freedPointers();
  bool found_s0 = false, found_azArg = false;
  for (void* pfp : freed) {
    if (pfp == static_cast<void*>(s0)) found_s0 = true;
    if (pfp == static_cast<void*>(arg)) found_azArg = true;
  }
  EXPECT_TRUE(found_s0);
  EXPECT_TRUE(found_azArg);

  // Clean up the leftover string allocated for s0? It should have been freed, but in case it wasn't
  // we free it here to avoid leaks if the implementation did not free due to a hypothetical path.
  // Note: If the implementation freed s0, this would be a double-free; in tests we rely on the
  // contract that it frees as per code path.
  // We skip additional manual delete to avoid double-free risk in case of future changes.
}

//-------------------------------------------------------------------------------------
// Main
//-------------------------------------------------------------------------------------

int main() {
  std::cout << "Running sqlite3VtabClear unit tests (mocked environment)\n";

  testCase1_AzArgNull_PnBytesFreedZero();
  testCase2_AzArgNull_PnBytesFreedNonZero();
  testCase3_AzArg3Args_FreesSelectedAndAzArg();
  testCase4_AzArgSingleArg_FreesSingleThenAzArg();

  std::cout << "Tests executed: " << g_tests_run << ", Failures: " << g_tests_failed << "\n";
  if(g_tests_failed == 0) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cout << "SOME TESTS FAILED\n";
    return 1;
  }
}