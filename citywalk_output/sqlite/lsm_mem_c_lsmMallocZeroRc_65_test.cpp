/*
  Unit test suite for lsmMallocZeroRc (lsm_mem.c)
  - Uses C++11 (no Google Test)
  - Tests cover key branches of lsmMallocZeroRc:
      1) When *pRc == LSM_OK, a successful allocation via lsmMallocZero => pRet != 0, *pRc unchanged.
      2) When *pRc != LSM_OK, no allocation occurs and pRet == 0, *pRc unchanged.
      3) When *pRc == LSM_OK and lsmMallocZero returns 0 (OOM scenario) => pRet == 0 and *pRc becomes LSM_NOMEM_BKPT.
  - The tests rely on the actual lsmMallocZero and memory allocator semantics provided by the project.
  - Test harness uses lightweight non-terminating assertions to maximize coverage.
*/

#include <lsmInt.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <cstdint>


// Bring in the C headers with C linkage
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

// Helper macro for assertion with message, non-terminating.
#define ASSERT_TRUE(cond, msg) do { \
    ++g_total; \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << msg << " (line " << __LINE__ << ")" << std::endl; \
        ++g_failed; \
    } \
} while(false)

#define ASSERT_NOT_NULL(ptr, msg) do { \
    ++g_total; \
    if((ptr) == nullptr) { \
        std::cerr << "[FAIL] " << msg << " (ptr == nullptr, line " << __LINE__ << ")" << std::endl; \
        ++g_failed; \
    } \
} while(false)

#define ASSERT_EQUAL(a, b, msg) do { \
    ++g_total; \
    if((a) != (b)) { \
        std::cerr << "[FAIL] " << msg << " (expected: " << (b) << ", actual: " << (a) << ", line " << __LINE__ << ")" << std::endl; \
        ++g_failed; \
    } \
} while(false)

// Test 1: True branch of first if (successful allocation) -> pRet != 0, rc remains LSM_OK, memory zeroed
void test_lsmMallocZeroRc_success_allocation() {
    std::cout << "Test: lsmMallocZeroRc_success_allocation" << std::endl;

    lsm_env env = {};            // Initialize environment (assumed POD from project)
    int rc = LSM_OK;               // Start with OK state
    size_t N = 16;                   // Small allocation size to be robust across platforms

    void *p = lsmMallocZeroRc(&env, N, &rc);

    // Assert we got a non-null pointer and rc is unchanged
    ASSERT_NOT_NULL(p, "Expected non-null pointer when rc == LSM_OK and allocation succeeds");

    // If allocation succeeded, rc should remain LSM_OK
    ASSERT_EQUAL(rc, LSM_OK, "rc should remain LSM_OK after successful allocation");

    // Validate that allocated memory is zeroed (lsmMallocZero contract)
    if (p != nullptr) {
        unsigned char *bytes = static_cast<unsigned char*>(p);
        bool allZero = true;
        for (size_t i = 0; i < N; ++i) {
            if (bytes[i] != 0) { allZero = false; break; }
        }
        ASSERT_TRUE(allZero, "Allocated memory should be zero-initialized by lsmMallocZeroRc path");
        // Free the allocation to avoid leaks
        lsmFree(&env, p);
    } else {
        // If allocation failed (very unlikely for small N), RC would have changed, which would fail earlier asserts
        // Nothing additional to do here.
    }
}

// Test 2: False branch of first if (pRc != LSM_OK) -> no allocation, pRet == 0, rc unchanged
void test_lsmMallocZeroRc_rc_not_ok() {
    std::cout << "Test: lsmMallocZeroRc_rc_not_ok" << std::endl;

    lsm_env env = {};
    int rc = -12345; // Some non-OK value
    size_t N = 32;

    void *p = lsmMallocZeroRc(&env, N, &rc);

    // Expect no allocation
    ASSERT_TRUE(p == nullptr, "Expected nullptr when initial rc != LSM_OK");

    // rc should remain unchanged
    ASSERT_EQUAL(rc, -12345, "rc should remain unchanged when initial rc != LSM_OK");
}

// Test 3: Inner branch (pRet == 0) -> rc updated to LSM_NOMEM_BKPT
void test_lsmMallocZeroRc_oom_path() {
    std::cout << "Test: lsmMallocZeroRc_oom_path" << std::endl;

    lsm_env env = {};
    int rc = LSM_OK;
    // Large allocation request to intentionally trigger allocation failure on common platforms
    size_t N = static_cast<size_t>(1) << 28; // ~256MB, may fail on constrained environments

    void *p = lsmMallocZeroRc(&env, N, &rc);

    if (p == nullptr) {
        // If allocation failed, rc should have been updated to LSM_NOMEM_BKPT
        ASSERT_EQUAL(rc, LSM_NOMEM_BKPT, "rc should be LSM_NOMEM_BKPT after OOM from lsmMallocZeroRc");
    } else {
        // If allocation unexpectedly succeeded, treat as inconclusive for OOM path
        // Free allocated memory to avoid leaks
        lsmFree(&env, p);
        std::cout << "[INFO] OOM path not triggered on this platform; allocation succeeded for N=" << N << std::endl;
    }
}

// Summary printer
void print_summary() {
    std::cout << "Summary: total tests = " << g_total << ", failures = " << g_failed << std::endl;
}

// Main: run all tests
int main() {
    test_lsmMallocZeroRc_success_allocation();
    test_lsmMallocZeroRc_rc_not_ok();
    test_lsmMallocZeroRc_oom_path();

    print_summary();
    // Return non-zero if any test failed
    return (g_failed > 0) ? 1 : 0;
}