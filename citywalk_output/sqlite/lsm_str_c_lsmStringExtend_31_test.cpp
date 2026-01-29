// A self-contained C++11 test suite for the focal function lsmStringExtend.
// The tests are designed to exercise the function's branches and
// its dependent behavior without relying on an external GTest framework.
// The test harness uses a small in-file mock of the minimal C library
// components that lsmStringExtend relies on.

#include <lsmInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cassert>


// ---------------------- Mocked/Central Types and Helpers ----------------------

// Forward declaration for the environment type (as in the original API)
struct lsm_env;

// Minimal LsmString structure as used by lsmStringExtend
struct LsmString {
    int n;       // current used length
    int nAlloc;  // allocated size
    char* z;     // string buffer
    lsm_env* pEnv; // environment pointer (unused in tests, null-safe)
};

// Result/status codes (matching the original API)
static const int LSM_OK = 0;
static const int LSM_NOMEM = 1;
static const int LSM_NOMEM_BKPT = 2;

// Global knobs to drive test-specific behavior of the mock allocator
static int g_force_realloc_fail = 0; // if >0, force lsmRealloc to fail for n > this value
static int g_free_calls = 0;         // tracks number of lsmFree invocations

// Simple environments (empty for tests)
struct lsm_env {};

// Mocked allocator/free wrappers mirroring expected API
char* lsmRealloc(lsm_env* /*pEnv*/, char* p, int n){
    if( g_force_realloc_fail > 0 && n > g_force_realloc_fail ){
        return nullptr; // simulate out-of-memory condition for testing
    }
    // realloc handles null pointers as malloc
    return (char*)realloc(p, n);
}
void lsmFree(lsm_env* /*pEnv*/, char* p){
    if(p){
        free(p);
        ++g_free_calls;
    }
}

// ---------------------- Focal Method Under Test (as provided) ----------------------

int lsmStringExtend(LsmString *pStr, int nNew){
    assert( nNew>0 );
    if( pStr->n<0 ) return LSM_NOMEM;
    if( pStr->n + nNew >= pStr->nAlloc ){
        int nAlloc = pStr->n + nNew + 100;
        char *zNew = lsmRealloc(pStr->pEnv, pStr->z, nAlloc);
        if( zNew==0 ){
            lsmFree(pStr->pEnv, pStr->z);
            nAlloc = 0;
            pStr->n = -1;
        }
        pStr->nAlloc = nAlloc;
        pStr->z = zNew;
    }
    return (pStr->z ? LSM_OK : LSM_NOMEM_BKPT);
}

// ---------------------- Lightweight Test Harness ----------------------

// Simple assertion helpers that do not abort on failure, to maximize coverage
static int g_pass = 0;
static int g_fail = 0;

#define CHECK_EQ(a, b, msg) \
    do { \
        if( (a) != (b) ){ \
            std::cerr << "FAIL: " msg " | Expected " << (b) << ", got " << (a) \
                      << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            ++g_fail; \
        } else { \
            ++g_pass; \
        } \
    } while(0)

#define CHECK_PTR_EQ(a, b, msg) \
    do { \
        if( (void*)(a) != (void*)(b) ){ \
            std::cerr << "FAIL: " msg " | Pointer mismatch. Expected " \
                      << b << ", got " << a << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            ++g_fail; \
        } else { \
            ++g_pass; \
        } \
    } while(0)

#define CHECK_TRUE(cond, msg) \
    do { \
        if(!(cond)){ \
            std::cerr << "FAIL: " msg " | Condition is false (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            ++g_fail; \
        } else { \
            ++g_pass; \
        } \
    } while(0)

static void reset_globals(){
    g_free_calls = 0;
    g_force_realloc_fail = 0;
    g_pass = 0;
    g_fail = 0;
}

// Test 1: No reallocation occurs when there is enough allocated space; ensure state unchanged
void test_no_realloc_no_change(){
    // Setup
    LsmString s;
    s.n = 5;
    s.nAlloc = 100;
    s.z = (char*)malloc(20);
    s.pEnv = nullptr;

    void* old_z = s.z;

    // Exercise
    int ret = lsmStringExtend(&s, 10); // 5 + 10 = 15 < 100, no realloc

    // Verify
    CHECK_EQ(ret, LSM_OK, "No realloc path should return LSM_OK");
    CHECK_EQ(s.n, 5, "n should remain unchanged");
    CHECK_EQ(s.nAlloc, 100, "nAlloc should remain unchanged");
    CHECK_PTR_EQ(s.z, old_z, "z pointer should remain unchanged when no realloc");

    // Cleanup
    if(s.z) free(s.z);
}

// Test 2: Reallocation occurs and succeeds; new buffer allocated and pointers updated
void test_realloc_succeeds(){
    LsmString s;
    s.n = 5;
    s.nAlloc = 20;     // force reallocation path: 5+20 >= 20
    s.z = (char*)malloc(10);
    s.pEnv = nullptr;

    char* old_z = s.z;

    int ret = lsmStringExtend(&s, 20); // triggers realloc to 125 bytes

    CHECK_EQ(ret, LSM_OK, "Reallocation should succeed and return LSM_OK");
    CHECK_TRUE(s.z != nullptr, "z should be non-null after successful realloc");
    CHECK_TRUE(s.z != old_z, "z should point to a new allocation after realloc");
    CHECK_EQ(s.nAlloc, 125, "nAlloc should be updated to n + nNew + 100 (125)");
    // old_z memory is managed by realloc and may be moved; do not free old_z separately
    if(s.z) {
        free(s.z);
    }
}

// Test 3: Reallocation fails (OOM path); old buffer freed, function returns NOMEM_BKPT
void test_realloc_fails_oom(){
    // Drive realloc to fail for large allocations
    g_free_calls = 0;
    g_force_realloc_fail = 124; // any n > 124 will fail

    LsmString s;
    s.n = 5;
    s.nAlloc = 20;
    s.z = (char*)malloc(10);
    s.pEnv = nullptr;

    int ret = lsmStringExtend(&s, 20); // would allocate 125, but we force fail

    CHECK_EQ(ret, LSM_NOMEM_BKPT, "OOM path should return LSM_NOMEM_BKPT");
    CHECK_EQ(s.n, -1, "On OOM, n should be set to -1");
    CHECK_EQ(s.nAlloc, 0, "On OOM, nAlloc should be reset to 0");
    CHECK_EQ(s.z, nullptr, "On OOM, z should be nullified");
    CHECK_EQ(g_free_calls, 1, "Old buffer should be freed upon OOM");

    // Cleanup: ensure globals reset
    g_force_realloc_fail = 0;
}

// Test 4: Early exit when n < 0 (precondition failure)
void test_negative_n_exits_early(){
    LsmString s;
    s.n = -2;
    s.nAlloc = 0;
    s.z = nullptr;
    s.pEnv = nullptr;

    int ret = lsmStringExtend(&s, 5);

    CHECK_EQ(ret, LSM_NOMEM, "n < 0 should return LSM_NOMEM");
    CHECK_EQ(s.n, -2, "n should remain unchanged on early exit");
    CHECK_EQ(s.z, nullptr, "z should remain unchanged on early exit");
}

// Test 5: Null z with insufficient space should return NOMEM_BKPT and not allocate
void test_null_z_no_allocation_possible(){
    LsmString s;
    s.n = 0;
    s.nAlloc = 100;
    s.z = nullptr;
    s.pEnv = nullptr;

    int ret = lsmStringExtend(&s, 1);

    CHECK_EQ(ret, LSM_NOMEM_BKPT, "Null z with insufficient space should return NOMEM_BKPT");
    CHECK_EQ(s.z, nullptr, "z should remain null");
    CHECK_EQ(s.n, 0, "n should remain unchanged");
}

// ---------------------- Main Runner ----------------------

int main(){
    reset_globals();

    test_no_realloc_no_change();
    test_realloc_succeeds();
    test_realloc_fails_oom();
    test_negative_n_exits_early();
    test_null_z_no_allocation_possible();

    std::cout << "Tests Passed: " << g_pass << "\n";
    std::cout << "Tests Failed: " << g_fail << "\n";

    // Return non-zero if any test failed
    return (g_fail == 0) ? 0 : 1;
}