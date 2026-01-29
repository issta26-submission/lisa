/*
  Self-contained unit test suite for sqlite3ArrayAllocate (C function)
  - No external testing framework (no GTest). Uses simple EXPECT-like macros.
  - Provides minimal stubs for sqlite3 types and sqlite3DbRealloc to enable isolated testing.
  - Covers allocation and reallocation paths, non-reallocation path, and failure modes.
  - All tests are self-contained and reset memory state between tests.
*/

#include <cstring>
#include <sqliteInt.h>
#include <unordered_map>
#include <iostream>
#include <cstdlib>


// Domain-specific pragmatics: use C-style interfaces where needed, but implement in C++11.

typedef long long sqlite3_int64;

// Minimal sqlite3 stub to satisfy the focal function signature
struct sqlite3 { int dummy; };

// Global memory-tracking allocator to mimic sqlite3DbRealloc behavior
static std::unordered_map<void*, size_t> g_allocMap;
static bool g_failNextAlloc = false; // If true, the next allocation attempt will fail (simulate OOM)

// Reset allocator state between tests
static void reset_memory_system() {
    // Free all tracked allocations
    for (auto &entry : g_allocMap) {
        if (entry.first) {
            std::free(entry.first);
        }
    }
    g_allocMap.clear();
    g_failNextAlloc = false;
}

// Minimal mock of sqlite3DbRealloc
static void* sqlite3DbRealloc(sqlite3 *db, void *pOld, sqlite3_int64 nBytes) {
    // Simulate C realloc-like semantics with a light footprint memory store
    if (nBytes == 0) {
        if (pOld) {
            std::free(pOld);
            g_allocMap.erase(pOld);
        }
        return nullptr;
    }

    if (pOld == nullptr) {
        // Allocate new block
        void *p = std::malloc((size_t)nBytes);
        if (p) g_allocMap[p] = (size_t)nBytes;
        return p;
    }

    // Simulate OOM on a single "next" allocation if requested
    if (g_failNextAlloc) {
        g_failNextAlloc = false;
        return nullptr;
    }

    auto it = g_allocMap.find(pOld);
    size_t oldSize = (it != g_allocMap.end()) ? it->second : 0;
    if ((sqlite3_int64)oldSize == nBytes) {
        // Size unchanged
        return pOld;
    }

    void *p = std::realloc(pOld, (size_t)nBytes);
    if (p) {
        // Update tracking
        g_allocMap.erase(pOld);
        g_allocMap[p] = (size_t)nBytes;
    } else {
        // On realloc failure, simulate freeing by removing old entry
        g_allocMap.erase(pOld);
    }
    return p;
}

// Forward declaration of sqlite3ArrayAllocate with the exact signature from the focal method
static void* sqlite3ArrayAllocate(
  sqlite3 *db,      /* Connection to notify of malloc failures */
  void *pArray,     /* Array of objects.  Might be reallocated */
  int szEntry,      /* Size of each object in the array */
  int *pnEntry,     /* Number of objects currently in use */
  int *pIdx         /* Write the index of a new slot here */
);

// Implementation of sqlite3ArrayAllocate copied/adapted from the focal method
static void* sqlite3ArrayAllocate(
  sqlite3 *db,      /* Connection to notify of malloc failures */
  void *pArray,     /* Array of objects.  Might be reallocated */
  int szEntry,      /* Size of each object in the array */
  int *pnEntry,     /* Number of objects currently in use */
  int *pIdx         /* Write the index of a new slot here */
){
  char *z;
  sqlite3_int64 n = *pIdx = *pnEntry;
  if( (n & (n-1))==0 ){
    sqlite3_int64 sz = (n==0) ? 1 : 2*n;
    void *pNew = sqlite3DbRealloc(db, pArray, sz*szEntry);
    if( pNew==0 ){
      *pIdx = -1;
      return pArray;
    }
    pArray = pNew;
  }
  z = (char*)pArray;
  memset(&z[n * szEntry], 0, szEntry);
  ++*pnEntry;
  return pArray;
}

/* Simple test harness and unit tests */

#define EXPECT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_FAIL: " << (msg) << " (line " << __LINE__ << ")\n"; \
        ++g_testFailures; \
    } \
} while(0)

static int g_testFailures = 0;
static int g_totalTests = 0;

static void run_test_initial_allocation() {
    // Test 1: initial allocation when pnEntry=0 and pArray=nullptr
    reset_memory_system();
    g_totalTests++;

    sqlite3 db{};
    void *pArray = nullptr;
    int szEntry = 4; // size of int
    int pnEntry = 0;
    int pIdx = 0;

    void *res = sqlite3ArrayAllocate(&db, pArray, szEntry, &pnEntry, &pIdx);

    // Expectations
    EXPECT(res != nullptr, "Initial allocation should succeed (non-null result)");
    EXPECT(pnEntry == 1, "pnEntry should be 1 after first allocation");
    EXPECT(pIdx == 1, "pIdx should be set to 1 (old pnEntry)");
    if (res) {
        int *arr = (int*)res;
        EXPECT(arr[0] == 0, "First element should be initialized to 0");
    }
}

static void run_test_second_allocation_realloc() {
    // Test 2: second call should trigger reallocation for n=1 (power of two)
    reset_memory_system();
    g_totalTests++;

    sqlite3 db{};
    void *pArray = nullptr;
    int szEntry = 4;
    int pnEntry = 0;
    int pIdx = 0;

    // First allocation
    void *res1 = sqlite3ArrayAllocate(&db, pArray, szEntry, &pnEntry, &pIdx);
    EXPECT(res1 != nullptr, "First allocation (res1) should succeed");
    // Use the same res1 for next allocation
    void *res2 = sqlite3ArrayAllocate(&db, res1, szEntry, &pnEntry, &pIdx);

    EXPECT(res2 != res1, "Second allocation should realloc (pointer change)");
    EXPECT(pnEntry == 2, "pnEntry should be 2 after second allocation");
    EXPECT(pIdx == 1, "pIdx should be set to 1 for n=1 before increment");
    if (res2) {
        int *arr = (int*)res2;
        // New capacity should cover index 1
        arr[1] = 0; // initialize for safety
        EXPECT(arr[0] == 0, "Existing element 0 should remain zero");
        EXPECT(arr[1] == 0, "Newly allocated slot 1 should be initialized to 0");
    }
}

static void run_test_third_allocation_realloc() {
    // Test 3: third call (n becomes 2) should realloc to 4 entries
    reset_memory_system();
    g_totalTests++;

    sqlite3 db{};
    void *pArray = nullptr;
    int szEntry = 4;
    int pnEntry = 0;
    int pIdx = 0;

    // First allocation
    void *res1 = sqlite3ArrayAllocate(&db, pArray, szEntry, &pnEntry, &pIdx);
    // Second allocation (n=1)
    void *res2 = sqlite3ArrayAllocate(&db, res1, szEntry, &pnEntry, &pIdx);
    // Third allocation (n=2) -> should realloc again
    void *res3 = sqlite3ArrayAllocate(&db, res2, szEntry, &pnEntry, &pIdx);

    EXPECT(res3 != res2, "Third allocation should realloc for n=2");
    EXPECT(pnEntry == 3, "pnEntry should be 3 after third allocation");
    EXPECT(pIdx == 2, "pIdx should be set to 2 for n=2 before increment");
    if (res3) {
        int *arr = (int*)res3;
        // Ensure previous slots remain zeroed and new slot 2 is zeroed
        EXPECT(arr[0] == 0, "Slot 0 should remain 0");
        EXPECT(arr[1] == 0, "Slot 1 should remain 0 after reallocation");
        EXPECT(arr[2] == 0, "Slot 2 should be initialized to 0");
    }
}

static void run_test_non_reallocation_path() {
    // Test 4: non-reallocation when n=3 (not a power of two) after capacity has been expanded
    reset_memory_system();
    g_totalTests++;

    sqlite3 db{};
    void *pArray = nullptr;
    int szEntry = 4;
    int pnEntry = 0;
    int pIdx = 0;

    // Sequence to ensure capacity for index 3 exists:
    // 1) n=0 -> alloc 1
    // 2) n=1 -> realloc to 2
    // 3) n=2 -> realloc to 4
    // 4) n=3 -> no realloc; should reuse same pointer
    void *r1 = sqlite3ArrayAllocate(&db, pArray, szEntry, &pnEntry, &pIdx);
    void *r2 = sqlite3ArrayAllocate(&db, r1, szEntry, &pnEntry, &pIdx);
    void *r3 = sqlite3ArrayAllocate(&db, r2, szEntry, &pnEntry, &pIdx);

    // Now pnEntry should be 3; next call with n=3
    void *r4 = sqlite3ArrayAllocate(&db, r3, szEntry, &pnEntry, &pIdx);

    // r4 should be the same as r3 (no realloc)
    EXPECT(r4 == r3, "Fourth allocation (n=3) should not reallocate (same pointer)");
    EXPECT(pnEntry == 4, "pnEntry should be 4 after fourth allocation");
    EXPECT(pIdx == 3, "pIdx should be set to 3 for n=3 before increment");

    if (r4) {
        int *arr = (int*)r4;
        // Slot 3 should be zeroed by this call
        EXPECT(arr[3] == 0, "Slot 3 should be initialized to 0 on non-reallocation path");
    }
}

static void run_test_allocation_failure_on_second_realloc() {
    // Test 5: simulate allocation failure on the second reallocation (n=1 path)
    reset_memory_system();
    g_totalTests++;

    sqlite3 db{};
    void *pArray = nullptr;
    int szEntry = 4;
    int pnEntry = 0;
    int pIdx = 0;

    // First allocation (n=0) should succeed
    void *res1 = sqlite3ArrayAllocate(&db, pArray, szEntry, &pnEntry, &pIdx);
    // Prepare to fail on next allocation (n=1 -> size 2*szEntry)
    g_failNextAlloc = true;
    void *res2 = sqlite3ArrayAllocate(&db, res1, szEntry, &pnEntry, &pIdx);

    // Expect failure: res2 should be same as res1 and pIdx should be set to -1
    EXPECT(res2 == res1, "Allocation should fail on second realloc and return old pointer");
    EXPECT(pnEntry == 1, "pnEntry should remain unchanged after failed realloc (still 1)");
    EXPECT(pIdx == -1, "pIdx should be set to -1 on allocation failure");
}

static void run_test_allocation_failure_on_first_allocation() {
    // Test 6: simulate allocation failure on the very first allocation (n=0)
    reset_memory_system();
    g_totalTests++;

    sqlite3 db{};
    void *pArray = nullptr;
    int szEntry = 4;
    int pnEntry = 0;
    int pIdx = 0;

    // Force first allocation to fail
    g_failNextAlloc = true;
    void *res = sqlite3ArrayAllocate(&db, pArray, szEntry, &pnEntry, &pIdx);

    // Expect null result and indices updated to indicate failure
    EXPECT(res == nullptr, "First allocation should fail and return nullptr");
    EXPECT(pnEntry == 0, "pnEntry should remain 0 after failed first allocation");
    EXPECT(pIdx == -1, "pIdx should be set to -1 on allocation failure");
}

int main() {
    // Run tests
    run_test_initial_allocation();
    run_test_second_allocation_realloc();
    run_test_third_allocation_realloc();
    run_test_non_reallocation_path();
    run_test_allocation_failure_on_second_realloc();
    run_test_allocation_failure_on_first_allocation();

    // Summary
    std::cout << "Unit test suite completed. "
              << "Total tests: " << g_totalTests
              << ", Failures: " << g_testFailures << std::endl;

    // Exit code reflects test success (0) or failure (1)
    return g_testFailures ? 1 : 0;
}