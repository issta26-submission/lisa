#include <cstdlib>
#include <lsmInt.h>
#include <cstdio>
#include <cstddef>


// -------------------- FOCAL METHOD AND DEPENDENCIES (Test Harbor) --------------------
// Minimalistic reimplementation of the focal method and its dependencies to enable
// isolated unit testing without external frameworks.

// FOCAL_ENV equivalent
struct lsm_env {};

// Forward declarations to mimic real project dependencies
void *lsm_realloc(lsm_env *pEnv, void *p, size_t N);
void lsm_free(lsm_env *pEnv, void *p);

// The focal method under test (re-implemented here for a self-contained test)
void *lsmReallocOrFree(lsm_env *pEnv, void *p, size_t N){
  void *pNew;
  pNew = lsm_realloc(pEnv, p, N);
  if( !pNew ) lsm_free(pEnv, p);
  return pNew;
}

// -------------------- Test Hooks and Behavior Injection --------------------
// Global state to control lsm_realloc behavior and to observe lsm_free calls.
// This keeps tests self-contained without modifying real code paths.
static bool g_lsm_realloc_will_fail = false;
static bool g_lsm_free_called = false;
static void *g_lsm_free_arg = nullptr;

// Helper to reset test state between test cases
static void reset_test_state() {
    g_lsm_realloc_will_fail = false;
    g_lsm_free_called = false;
    g_lsm_free_arg = nullptr;
}

// Minimalistic behavior injection for lsm_realloc
void *lsm_realloc(lsm_env *pEnv, void *p, size_t N){
    (void)pEnv; (void)p; // unused in this mock
    if (g_lsm_realloc_will_fail) {
        return NULL;
    }
    if (N == 0) {
        // For simplicity, treat 0-sized realloc as failure in tests
        return NULL;
    }
    // Simulate a successful reallocation by allocating new memory
    return malloc(N);
}

// Non-destructive lsm_free to avoid invalid frees in tests.
// It only records that a free would have been invoked and with which pointer.
// Freeing the pointer is intentionally omitted to prevent undefined behavior
// when the pointer did not originate from our allocator in the test harness.
void lsm_free(lsm_env *pEnv, void *p){
    (void)pEnv; (void)p; // keep signature intact
    g_lsm_free_called = true;
    g_lsm_free_arg = p;
    // Do not call free(p) here to avoid freeing non-owned addresses in tests.
}

// -------------------- Lightweight Test Framework (no GTest) --------------------
// Expectations are non-terminating: they only record failures and continue.
// This maximizes code coverage by allowing all tests to run.
static int g_numExpect = 0;
static int g_numFail = 0;

#define EXPECT(cond) do { \
    ++g_numExpect; \
    if(!(cond)) { \
        ++g_numFail; \
        printf("  [EXPECT FAILED] %s:%d: %s\n", __FILE__, __LINE__, #cond); \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_numExpect; \
    if((void*)(a) != (void*)(b)) { \
        ++g_numFail; \
        printf("  [EXPECT_EQ FAILED] %s:%d: %p != %p (\"%s\" vs \"%s\")\n", __FILE__, __LINE__, (void*)(a), (void*)(b), #a, #b); \
    } \
} while(0)

static void reset_expectations() { g_numExpect = 0; g_numFail = 0; }

// -------------------- Test Cases for lsmReallocOrFree --------------------
// Test 1: Reallocation succeeds; existing pointer should NOT be freed.
// Expectation: result != NULL and lsm_free was NOT called with the original pointer.
static bool test_realloc_succeeds_no_free() {
    reset_test_state();
    reset_expectations();

    g_lsm_realloc_will_fail = false;
    void *orig = reinterpret_cast<void*>(0x1000);
    void *res = lsmReallocOrFree(nullptr, orig, 16);

    EXPECT(res != nullptr);
    EXPECT(!g_lsm_free_called);
    if (res) free(res); // clean up allocated block used by test

    return (g_numFail == 0);
}

// Test 2: Reallocation fails; original pointer should be freed (observed via call flag).
// Expectation: result == NULL and lsm_free was called with the original pointer.
static bool test_realloc_fails_calls_free() {
    reset_test_state();
    reset_expectations();

    g_lsm_realloc_will_fail = true;
    void *orig = reinterpret_cast<void*>(0x2000);
    void *res = lsmReallocOrFree(nullptr, orig, 16);

    EXPECT(res == nullptr);
    EXPECT(g_lsm_free_called);
    EXPECT_EQ(g_lsm_free_arg, orig);

    // No memory to free here due to test harness; no cleanup needed.
    return (g_numFail == 0);
}

// Test 3: Reallocation succeeds when input pointer is NULL (acts like malloc).
// Expectation: result != NULL and lsm_free was not called.
static bool test_realloc_succeeds_with_null_input() {
    reset_test_state();
    reset_expectations();

    g_lsm_realloc_will_fail = false;
    void *res = lsmReallocOrFree(nullptr, nullptr, 32);

    EXPECT(res != nullptr);
    EXPECT(!g_lsm_free_called);
    if (res) free(res); // cleanup

    return (g_numFail == 0);
}

// Test 4: Reallocation fails when input pointer is NULL; free should be attempted with NULL.
// Expectation: result == NULL and lsm_free was called with NULL.
static bool test_realloc_fails_with_null_input() {
    reset_test_state();
    reset_expectations();

    g_lsm_realloc_will_fail = true;
    void *res = lsmReallocOrFree(nullptr, nullptr, 32);

    EXPECT(res == nullptr);
    EXPECT(g_lsm_free_called);
    EXPECT_EQ(g_lsm_free_arg, nullptr);

    return (g_numFail == 0);
}

// -------------------- Main: Run Tests --------------------
int main() {
    bool t1 = test_realloc_succeeds_no_free();
    printf("Test 1 (realloc success -> no free) %s\n", t1 ? "PASSED" : "FAILED");

    bool t2 = test_realloc_fails_calls_free();
    printf("Test 2 (realloc fail -> free original) %s\n", t2 ? "PASSED" : "FAILED");

    bool t3 = test_realloc_succeeds_with_null_input();
    printf("Test 3 (realloc with null input success) %s\n", t3 ? "PASSED" : "FAILED");

    bool t4 = test_realloc_fails_with_null_input();
    printf("Test 4 (realloc with null input fail) %s\n", t4 ? "PASSED" : "FAILED");

    // Overall result
    int overall = (t1 && t2 && t3 && t4) ? 0 : 1;
    if (overall == 0) {
        printf("OVERALL: PASSED all tests.\n");
    } else {
        printf("OVERALL: FAILED one or more tests.\n");
    }
    return overall;
}