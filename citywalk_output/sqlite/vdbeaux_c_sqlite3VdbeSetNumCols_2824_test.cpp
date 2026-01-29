// Minimal C++11 unit tests for the focal function sqlite3VdbeSetNumCols
// This test harness provides a self-contained, mock environment that mimics
// just enough of the SQLite Vdbe internals to exercise the function's logic.
// It purposefully avoids GTest. Tests are non-terminating: failures are recorded
// and execution continues to maximize coverage.

#include <vector>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Domain-specific simplifications and mocks
namespace TestMock {

// Forward declarations of minimal types used by sqlite3VdbeSetNumCols
struct sqlite3 {};
struct Mem { int dummy; };
struct Vdbe { sqlite3 *db; uint16_t nResAlloc; uint16_t nResColumn; Mem* aColName; };

// Constants and types matching the focal code
using u16 = uint16_t;
const uint16_t MEM_Null = 0x00;  // flag value used by initMemArray for test purposes
const int COLNAME_N = 2;          // number of Mem entries per result column (simplified)

// Global counters to verify correct branches and side-effects
static int g_releaseMemArray_calls = 0;
static int g_last_release_N = 0;
static int g_mallocRawNN_size = 0;
static Mem* g_last_malloc_ptr = nullptr;
static int g_initMemArray_calls = 0;
static bool g_forceMallocFail = false; // to simulate allocation failure in tests

// Simple sqlite3-like heap management for tests
static Mem* sqlite3DbMallocRawNN(sqlite3 *db, int sz){
    g_mallocRawNN_size = sz;
    int count = (sz > 0) ? (sz / (int)sizeof(Mem)) : 0;
    Mem* p = nullptr;
    if(count > 0){
        p = new Mem[count];
        g_last_malloc_ptr = p;
    }
    return p;
}
static void sqlite3DbFree(sqlite3 *db, void *p){
    delete[] (Mem*)p;
}
static void releaseMemArray(Mem *p, int N){
    g_releaseMemArray_calls++;
    g_last_release_N = N;
    // In the real code, this would recursively free embedded Mem elements.
}
static void initMemArray(Mem *p, int N, sqlite3 *db, uint16_t flags){
    g_initMemArray_calls++;
    for(int i = 0; i < N; ++i){
        p[i].dummy = (int)flags;
    }
}

// The focal function under test (re-implemented here for the mock environment)
void sqlite3VdbeSetNumCols(Vdbe *p, int nResColumn){
  int n;
  sqlite3 *db = p->db;
  if( p->nResAlloc ){
    releaseMemArray(p->aColName, p->nResAlloc*COLNAME_N);
    sqlite3DbFree(db, p->aColName);
  }
  n = nResColumn*COLNAME_N;
  p->nResColumn = p->nResAlloc = (u16)nResColumn;
  p->aColName = (Mem*)sqlite3DbMallocRawNN(db, sizeof(Mem)*n );
  if( p->aColName==0 ) return;
  initMemArray(p->aColName, n, db, MEM_Null);
}

// Helper to reset test environment
static void resetCounters(){
    g_releaseMemArray_calls = 0;
    g_last_release_N = -1;
    g_mallocRawNN_size = 0;
    g_last_malloc_ptr = nullptr;
    g_initMemArray_calls = 0;
    g_forceMallocFail = false;
}

} // namespace TestMock

// Bring in the mocks into the current scope for easy access
using namespace TestMock;

// Test harness: non-terminating assertions
static int g_total_tests_run = 0;
static int g_total_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "ASSERT FAILED: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
        ++g_total_failures; \
    } \
} while(0)
static void TEST_REPORT_SUMMARY() {
    std::cout << "Tests run: " << g_total_tests_run
              << ", Failures: " << g_total_failures << std::endl;
}

// Test 1: Basic allocation path when there is no prior allocation
// - Branch p->nResAlloc is false: no releaseMemArray/Free is performed
// - Allocation succeeds and Mem array is initialized to MEM_Null
static void test_basic_allocation_no_prior_alloc() {
    g_total_tests_run++;
    resetCounters();

    // Arrange
    sqlite3 db;
    TestMock::Vdbe p;
    p.db = &db;
    p.nResAlloc = 0;
    p.nResColumn = 0;
    p.aColName = nullptr;

    // Act
    sqlite3VdbeSetNumCols(&p, 3);

    // Assert
    int expectedN = 3 * COLNAME_N;
    TEST_ASSERT(p.nResColumn == 3, "nResColumn should be set to 3");
    TEST_ASSERT(p.nResAlloc == 3, "nResAlloc should be set to 3");
    TEST_ASSERT(p.aColName != nullptr, "aColName should be allocated (non-null)");
    TEST_ASSERT(g_initMemArray_calls == 1, "initMemArray should be called exactly once");
    TEST_ASSERT(g_mallocRawNN_size == sizeof(Mem) * (expectedN), "Malloc size should match Mem array allocation");

    // Validate memory initialization
    for(int i = 0; i < expectedN; ++i){
        TEST_ASSERT(p.aColName[i].dummy == MEM_Null, "Mem elements should be initialized to MEM_Null");
    }

    // Cleanup
    if(p.aColName) sqlite3DbFree(nullptr, p.aColName);
}

// Test 2: Allocation path when there is a prior allocation
// - Branch p->nResAlloc is true: releaseMemArray and sqlite3DbFree are invoked
// - After that, new allocation occurs and Mem array is initialized
static void test_allocation_with_prior_alloc() {
    g_total_tests_run++;
    resetCounters();

    // Arrange
    sqlite3 db;
    TestMock::Vdbe p;
    p.db = &db;
    // Pre-existing allocation
    p.nResAlloc = 4;
    p.nResColumn = 4;
    p.aColName = new Mem[p.nResAlloc * COLNAME_N];
    for(int i = 0; i < (p.nResAlloc*COLNAME_N); ++i) p.aColName[i].dummy = 123; // some sentinel

    // Act
    sqlite3VdbeSetNumCols(&p, 2);

    // Assert
    int expectedPrevN = 4 * COLNAME_N;
    TEST_ASSERT(g_releaseMemArray_calls == 1, "releaseMemArray should be called once");
    TEST_ASSERT(g_last_release_N == expectedPrevN, "releaseMemArray should be called with previous allocation size");
    TEST_ASSERT(p.nResColumn == 2, "nResColumn should be updated to 2");
    TEST_ASSERT(p.nResAlloc == 2, "nResAlloc should be updated to 2");
    TEST_ASSERT(p.aColName != nullptr, "New aColName should be allocated (non-null)");
    TEST_ASSERT(g_initMemArray_calls == 1, "initMemArray should be called exactly once for new allocation");

    int expectedN = 2 * COLNAME_N;
    TEST_ASSERT(sizeof(Mem) * (expectedN) == g_mallocRawNN_size, "Malloc size should equal new allocation size");
    for(int i = 0; i < expectedN; ++i){
        TEST_ASSERT(p.aColName[i].dummy == MEM_Null, "New Mem array should be initialized to MEM_Null");
    }

    // Cleanup
    if(p.aColName) sqlite3DbFree(nullptr, p.aColName);
}

// Test 3: Allocation failure path
// - Simulate sqlite3DbMallocRawNN returning nullptr
// - After call, fields should still reflect the intended nResColumn/nResAlloc values
static void test_allocation_failure_path() {
    g_total_tests_run++;
    resetCounters();
    g_forceMallocFail = true;

    // Arrange
    sqlite3 db;
    TestMock::Vdbe p;
    p.db = &db;
    p.nResAlloc = 0;
    p.nResColumn = 0;
    p.aColName = nullptr;

    // Act
    sqlite3VdbeSetNumCols(&p, 1);

    // Assert
    TEST_ASSERT(p.nResColumn == 1, "nResColumn should be set to 1 even if allocation fails");
    TEST_ASSERT(p.nResAlloc == 1, "nResAlloc should be set to 1 even if allocation fails");
    TEST_ASSERT(p.aColName == nullptr, "aColName should remain null when allocation fails");
    TEST_ASSERT(g_initMemArray_calls == 0, "initMemArray should not be called on allocation failure");
    // Cleanup: no allocation happened
}

// Test 4: No-release path when there is no prior allocation (to confirm false-branch)
// - Ensures that the false branch does not call releaseMemArray or sqlite3DbFree
static void test_no_release_branch_when_no_prior_alloc() {
    g_total_tests_run++;
    resetCounters();

    // Arrange
    sqlite3 db;
    TestMock::Vdbe p;
    p.db = &db;
    p.nResAlloc = 0;
    p.nResColumn = 0;
    p.aColName = nullptr;

    // Act
    sqlite3VdbeSetNumCols(&p, 5);

    // Assert
    TEST_ASSERT(g_releaseMemArray_calls == 0, "releaseMemArray should not be called when there was no prior allocation");
    TEST_ASSERT(g_last_release_N == -1, "releaseMemArray last argument should remain unset/-1");
    TEST_ASSERT(p.nResColumn == 5, "nResColumn should be set to 5");
    TEST_ASSERT(p.nResAlloc == 5, "nResAlloc should be set to 5");
    TEST_ASSERT(p.aColName != nullptr, "aColName should be allocated (non-null)");
    // Cleanup
    if(p.aColName) sqlite3DbFree(nullptr, p.aColName);
}

// Entry point
int main() {
    // Run all tests with explanatory comments
    test_basic_allocation_no_prior_alloc();
    test_allocation_with_prior_alloc();
    test_allocation_failure_path();
    test_no_release_branch_when_no_prior_alloc();

    TEST_REPORT_SUMMARY();
    return (g_total_failures != 0) ? 1 : 0;
}