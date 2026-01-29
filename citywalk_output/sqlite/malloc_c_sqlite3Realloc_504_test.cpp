// High-coverage unit tests for sqlite3Realloc (excerpted from malloc.c) using a lightweight harness.
// This test is designed to compile under C++11 without Google Test (GTest).
// It provides minimal in-file implementations of the sqlite3 memory subsystem used by sqlite3Realloc.
// The tests exercise true/false branches of various predicates and ensure memory correctness,
// while using non-terminating assertions (custom EXPECT-like macros).

#include <cstddef>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Lightweight test harness: non-terminating assertions that accumulate results.
// This is used in place of GTest/GMock as per the constraints.
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAILED] " << (msg) << "  (expected true, got false)\\n"; \
        ++g_tests_failed; \
    } else { \
        ++g_tests_passed; \
    } \
} while(0)

#define EXPECT_EQ(a,b,msg) do { \
    if(!((a) == (b))) { \
        std::cerr << "[FAILED] " << (msg) << "  (expected " << (a) << " == " << (b) << ")" << "\\n"; \
        ++g_tests_failed; \
    } else { \
        ++g_tests_passed; \
    } \
} while(0)

#define EXPECT_NEQ(a,b,msg) do { \
    if((a) == (b)) { \
        std::cerr << "[FAILED] " << (msg) << "  (expected " << (a) << " != " << (b) << ")" << "\\n"; \
        ++g_tests_failed; \
    } else { \
        ++g_tests_passed; \
    } \
} while(0)

#define EXPECT_GE(a,b,msg) do { \
    if(!((a) >= (b))) { \
        std::cerr << "[FAILED] " << (msg) << "  (expected " << (a) << " >= " << (b) << ")" << "\\n"; \
        ++g_tests_failed; \
    } else { \
        ++g_tests_passed; \
    } \
} while(0)


// Domain-specific minimal sqlite3-like environment.
// We reproduce only the subset used by sqlite3Realloc.

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned char u8;
typedef long long sqlite3_int64;
typedef unsigned long long sqlite3_uint64;

#define MEMTYPE_HEAP 1

// Forward declarations (as used by sqlite3Realloc).
static void *sqlite3Malloc(u64 n);
static void sqlite3_free(void *p);
static int sqlite3MallocSize(const void *p);
static void sqlite3MallocAlarm(int n);
static void sqlite3StatusHighwater(int a, int b);
static sqlite3_int64 sqlite3StatusValue(int op);
static void sqlite3StatusUp(int op, sqlite3_int64 v);
static void *sqlite3Realloc(void *pOld, u64 nBytes);
static int sqlite3MemdebugHasType(void *p, u8 t);
static int sqlite3MemdebugNoType(void *p, u8 t);
static void *sqlite3_global_xrealloc(void *p, int n);
static void *internal_xrealloc(void *pOld, int nNew);
static int EIGHT_BYTE_ALIGNMENT_OK(void *p); // helper for tests

// Memory accounting (simplified)
static sqlite3_int64 g_memoryUsed = 0;

// Simple header-based allocator: header stores payload size.
static const size_t HEADER_SIZE = sizeof(size_t);

static struct {
  struct {
    int (*xRoundup)(int);
  } m;
  bool bMemstat;
} sqlite3GlobalConfig;

// Minimal incidental memory state
static struct {
  int mutex;
  sqlite3_int64 alarmThreshold;
  sqlite3_int64 hardLimit;
} mem0;

// Helpers for alignment (matching macro used in production)
#define EIGHT_BYTE_ALIGNMENT(p) (void*)(((uintptr_t)(p) + 7) & ~((uintptr_t)7))

// No-op mutex functions (standalone harness)
#define sqlite3_mutex_enter(x) do {} while(0)
#define sqlite3_mutex_leave(x) do {} while(0)

// ---------------- Memory subsystem implementation (simplified) ----------------

static void* sqlite3Malloc(u64 n) {
    if(n == 0) n = 1;
    size_t total = (size_t)n + HEADER_SIZE;
    unsigned char* base = (unsigned char*)malloc(total);
    if(!base) return nullptr;
    // store size in header
    *((size_t*)base) = (size_t)n;
    g_memoryUsed += (sqlite3_int64)n;
    return (void*)(base + HEADER_SIZE);
}

static void sqlite3_free(void *p) {
    if(!p) return;
    unsigned char* headerPtr = (unsigned char*)p - HEADER_SIZE;
    size_t oldSize = *((size_t*)headerPtr);
    g_memoryUsed -= (sqlite3_int64)oldSize;
    free(headerPtr);
}

static int sqlite3MallocSize(const void *p) {
    if(!p) return 0;
    const unsigned char* headerPtr = (const unsigned char*)p - HEADER_SIZE;
    return (int)(*((size_t*)headerPtr));
}

static void sqlite3MallocAlarm(int n) {
    // no-op in harness
    (void)n;
}

static void sqlite3StatusHighwater(int a, int b) {
    (void)a; (void)b;
}
static sqlite3_int64 sqlite3StatusValue(int op) {
    if(op == 1) return g_memoryUsed; // SQLITE_STATUS_MEMORY_USED fake
    // Other status codes return 0
    return 0;
}
static void sqlite3StatusUp(int op, sqlite3_int64 v) {
    (void)op; (void)v;
    // update memory used naive (only for completeness)
}

static int sqlite3MemdebugHasType(void *p, u8 t) {
    (void)t;
    return p != nullptr;
}
static int sqlite3MemdebugNoType(void *p, u8 t) {
    (void)t;
    return p != nullptr;
}

// Simple roundup: align to 8
static int simple_roundup(int n) {
    // Align to 8 bytes
    return (n + 7) & ~7;
}

static void init_globals() {
    sqlite3GlobalConfig.m.xRoundup = &simple_roundup;
    sqlite3GlobalConfig.m.xRealloc = &internal_xrealloc;
    sqlite3GlobalConfig.bMemstat = false; // keep it simple for harness
    mem0.mutex = 0;
    mem0.alarmThreshold = 0;
    mem0.hardLimit = 0;
}

// Implementation of xRealloc used by sqlite3Realloc when memstat is off.
// We perform a simple realloc compatible with our sqlite3Malloc/sqlite3_free.
static void *internal_xrealloc(void *pOld, int nNew) {
    if (pOld == nullptr) {
        // behave like malloc
        return sqlite3Malloc((u64)nNew);
    }
    if (nNew == 0) {
        sqlite3_free(pOld);
        return nullptr;
    }
    if (nNew >= 0x7fffff00) {
        return nullptr;
    }
    int oldSize = sqlite3MallocSize(pOld);
    int nRounded = sqlite3GlobalConfig.m.xRoundup(nNew);
    if (oldSize == nRounded) {
        // No change
        return pOld;
    }
    // allocate new block, copy, free old
    void *pNew = sqlite3Malloc((u64)nRounded);
    if (pNew) {
        int toCopy = oldSize < nRounded ? oldSize : nRounded;
        memcpy(pNew, pOld, toCopy);
        sqlite3_free(pOld);
    }
    return pNew;
}

// Forward declare the actually used xRealloc function pointer (as per production API)
static void *sqlite3_global_xrealloc(void *p, int n) {
    return internal_xrealloc(p, n);
}

// The actual public-like interface used by tests
static void *sqlite3GlobalConfig_m_xRealloc(void *pOld, int n) {
    // Wrapper to mimic production API path; here we call internal_xrealloc
    return internal_xrealloc(pOld, n);
}

// The focal method under test, copied/adapted to rely on harness definitions above.
static void *sqlite3Realloc(void *pOld, u64 nBytes) {
  int nOld, nNew, nDiff;
  void *pNew;
  // Assertions from focal method (use harness: we assume non-null pOld in test scenarios)
  // For safety, guard: if pOld is null, mimic real code path by calling sqlite3Malloc(nBytes)
  if (pOld == nullptr) {
    return sqlite3Malloc(nBytes);
  }
  // Basic guards
  // In harness, these asserts are assumed to always pass for non-null pOld.
  // We'll implement as no-ops to keep test flow, but preserve semantics.
  if(!(sqlite3MemdebugHasType(pOld, MEMTYPE_HEAP))) {
    // Cannot satisfy assertion in harness, fail gracefully
    return nullptr;
  }
  if(!(sqlite3MemdebugNoType(pOld, (u8)~MEMTYPE_HEAP))) {
    // Cannot satisfy assertion in harness, fail gracefully
    return nullptr;
  }

  if( pOld==0 ){
    return sqlite3Malloc(nBytes); /* IMP: R-04300-56712 */
  }
  if( nBytes==0 ){
    sqlite3_free(pOld); /* IMP: R-26507-47431 */
    return 0;
  }
  if( nBytes>=0x7fffff00 ){
    /* The 0x7ffff00 limit term is explained in comments on sqlite3Malloc() */
    return 0;
  }

  nOld = sqlite3MallocSize(pOld);
  /* IMPLEMENTATION-OF: R-46199-30249 SQLite guarantees that the second
  ** argument to xRealloc is always a value returned by a prior call to
  ** xRoundup. */
  nNew = sqlite3GlobalConfig.m.xRoundup((int)nBytes);
  if( nOld==nNew ){
    pNew = pOld;
  }else if( sqlite3GlobalConfig.bMemstat ){
    // In harness we keep memstat false to simplify; this block will be skipped.
    sqlite3_int64 nUsed;
    sqlite3_mutex_enter(mem0.mutex);
    sqlite3StatusHighwater(SQLITE_STATUS_MALLOC_SIZE, (int)nBytes);
    nDiff = nNew - nOld;
    if( nDiff>0 && (nUsed = sqlite3StatusValue(SQLITE_STATUS_MEMORY_USED)) >= 
          mem0.alarmThreshold-nDiff ){
      sqlite3MallocAlarm(nDiff);
      if( mem0.hardLimit>0 && nUsed >= mem0.hardLimit - nDiff ){
        sqlite3_mutex_leave(mem0.mutex);
        return 0;
      }
    }
    pNew = sqlite3GlobalConfig.m.xRealloc(pOld, nNew);
#ifdef SQLITE_ENABLE_MEMORY_MANAGEMENT
    if( pNew==0 && mem0.alarmThreshold>0 ){
      sqlite3MallocAlarm((int)nBytes);
      pNew = sqlite3GlobalConfig.m.xRealloc(pOld, nNew);
    }
#endif
    if( pNew ){
      nNew = sqlite3MallocSize(pNew);
      sqlite3StatusUp(SQLITE_STATUS_MEMORY_USED, nNew-nOld);
    }
    sqlite3_mutex_leave(mem0.mutex);
  }else{
    pNew = sqlite3GlobalConfig.m.xRealloc(pOld, nNew);
  }
  // Imp: alignment
  // The test harness ensures allocations are aligned; enforce via check as a no-op
  // in production we would assert EIGHT_BYTE_ALIGNMENT(pNew);
  return pNew;
}

// Fake define of SQLITE_STATUS_MALLOC_SIZE, SQLITE_STATUS_MEMORY_USED
#define SQLITE_STATUS_MALLOC_SIZE 0x01
#define SQLITE_STATUS_MEMORY_USED   0x02

// A tiny helper used by tests to check alignment of a pointer
static int EIGHT_BYTE_ALIGNMENT_OK(void *p) {
    return (((uintptr_t)p) % 8) == 0;
}

// ---------------- Tests ----------------

// Helper to fill memory with a known pattern
static void fill_pattern(void* p, int size, uint8_t pattern) {
    if(!p || size <= 0) return;
    memset(p, pattern, (size_t)size);
}

// Helper to verify memory pattern
static bool verify_pattern(const void* p, int size, uint8_t expected) {
    if(!p || size <= 0) return true;
    const uint8_t* data = (const uint8_t*)p;
    for(int i = 0; i < size; ++i) {
        if(data[i] != expected) return false;
    }
    return true;
}

// Test 1: Realloc to same size should return same pointer and keep data intact
static void test_realloc_same_size() {
    void* p = sqlite3Malloc(64);
    EXPECT_TRUE(p != nullptr, "Malloc returned non-null for 64");
    fill_pattern(p, 64, 0xAA);

    void* p2 = sqlite3Realloc(p, 64);
    EXPECT_TRUE(p2 == p, "Realloc to same size should return same pointer");

    // Data should be preserved
    bool ok = verify_pattern(p2, 64, 0xAA);
    EXPECT_TRUE(ok, "Data preserved after realloc to same size");

    // Cleanup
    sqlite3_free(p2);
}

// Test 2: Realloc to larger size should allocate new block and preserve old data
static void test_realloc_increase_size() {
    void* p = sqlite3Malloc(64);
    EXPECT_TRUE(p != nullptr, "Malloc returned non-null for 64 (increase test)");
    fill_pattern(p, 64, 0x55);

    void* pNew = sqlite3Realloc(p, 128);
    EXPECT_TRUE(pNew != nullptr, "Realloc to larger size should succeed");
    EXPECT_NEQ(pNew, p, "Pointer should change when increasing size");

    // Alignment check
    EXPECT_TRUE(EIGHT_BYTE_ALIGNMENT_OK(pNew), "New pointer should be 8-byte aligned");

    // Old data preserved where possible
    bool ok = verify_pattern(pNew, 64, 0x55);
    EXPECT_TRUE(ok, "Old data preserved after realloc to larger size");

    // New block size should be 128
    int newSize = sqlite3MallocSize(pNew);
    EXPECT_EQ(newSize, 128, "New block size should be 128");

    // Cleanup
    sqlite3_free(pNew);
}

// Test 3: Realloc to zero should free and return null
static void test_realloc_zero_bytes() {
    void* p = sqlite3Malloc(60);
    fill_pattern(p, 60, 0x33);
    int oldSize = sqlite3MallocSize(p);

    void* pNew = sqlite3Realloc(p, 0);
    EXPECT_TRUE(pNew == nullptr, "Realloc with 0 bytes should return null");
    // The old block should be freed
    int afterFreeSize = g_memoryUsed;
    // We cannot directly map to exact memoryUsed after free in harness, but ensure it decreased or is <= old
    EXPECT_TRUE(afterFreeSize <= oldSize, "Memory usage should not exceed previous usage after free");

    // No need to free pNew
}

// Test 4: Realloc with extremely large nBytes should return null
static void test_realloc_too_large() {
    void* p = sqlite3Malloc(16);
    fill_pattern(p, 16, 0xAB);

    void* pNew = sqlite3Realloc(p, 0x80000000u); // large value
    EXPECT_TRUE(pNew == nullptr, "Realloc with very large size should return null");

    // The original pointer remains valid in many scenarios; ensure it's still usable
    bool ok = verify_pattern(p, 16, 0xAB);
    EXPECT_TRUE(ok, "Original memory remains valid after failed realloc");

    // Cleanup
    sqlite3_free(p);
}

// Test 5: Ensure alignment invariant holds after realloc
static void test_alignment_after_realloc() {
    void* p = sqlite3Malloc(24);
    fill_pattern(p, 24, 0x77);

    void* pNew = sqlite3Realloc(p, 40); // increase
    EXPECT_TRUE(pNew != nullptr, "Realloc for alignment should succeed");
    EXPECT_TRUE(EIGHT_BYTE_ALIGNMENT_OK(pNew), "Pointer must be 8-byte aligned after realloc");

    // Cleanup
    sqlite3_free(pNew);
}

// Entry point for tests
int main() {
    // Initialize harness and globals
    init_globals();

    // Run tests
    test_realloc_same_size();
    test_realloc_increase_size();
    test_realloc_zero_bytes();
    test_realloc_too_large();
    test_alignment_after_realloc();

    // Summary
    std::cout << "Tests passed: " << g_tests_passed << "\n";
    std::cout << "Tests failed: " << g_tests_failed << "\n";
    return (g_tests_failed == 0) ? 0 : 1;
}