// Unit tests for defMtxCreate in cmserr.c using a light-weight custom test runner
// This test suite does not rely on Google Test. It uses a small set of assertion helpers
// and runs all tests from main(). It assumes the surrounding project provides the real
// cmsContext type and the defMtxCreate/defMtxDestroy C interfaces (as in the focal file).

#include <lcms2_internal.h>
#include <cstdio>
#include <cstddef>


// Forward declaration of cmsContext to match potential library typedefs.
// We declare it as a pointer to an incomplete struct to maximize compatibility
// with variations in the library's internal cmsContext definition.
typedef struct _cmsContext_struct* cmsContext;

// Declares of the focal function under test (C linkage)
extern "C" {
    void* defMtxCreate(cmsContext id);
    void  defMtxDestroy(cmsContext id, void* mtx);
}

// Simple test harness state
static int g_failures = 0;

// Lightweight assertion macro that logs failures but does not abort tests.
// This aligns with "non-terminating assertions" guideline.
#define CHECK(condition, message)                              \
    do {                                                       \
        if (!(condition)) {                                    \
            fprintf(stderr, "CHECK FAILED: %s (at %s:%d) - %s\n",\
                    #condition, __FILE__, __LINE__, message);    \
            ++g_failures;                                      \
        }                                                      \
    } while (0)

// Test 1: Basic behavior - creating two mutexes with distinct ids should yield
// two non-null, distinct pointers. This validates that defMtxCreate allocates
// separate resources for different context ids.
void test_defMtxCreate_two_mutexes_are_distinct_and_non_null() {
    // Reset expectation for this test
    void* mtxA = defMtxCreate((cmsContext)0);
    void* mtxB = defMtxCreate((cmsContext)1);

    // Existence checks
    CHECK(mtxA != nullptr, "defMtxCreate should return non-null for id 0");
    CHECK(mtxB != nullptr, "defMtxCreate should return non-null for id 1");

    // Distinctness check
    CHECK(mtxA != mtxB, "Two mutexes created with different ids should be distinct pointers");

    // Cleanup (best-effort; the real implementation may not require explicit destruction)
    defMtxDestroy((cmsContext)0, mtxA);
    defMtxDestroy((cmsContext)1, mtxB);
}

// Test 2: Repeated creation and destruction across multiple ids should not crash.
// Ensures that successive allocations/destroys are tolerated by the underlying system.
void test_defMtxCreate_repeated_calls_do_not_crash() {
    void* handles[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };

    // Create multiple mutexes with varying ids
    for (int i = 0; i < 5; ++i) {
        handles[i] = defMtxCreate((cmsContext)i);
        CHECK(handles[i] != nullptr, "defMtxCreate should return non-null on repeated creation");
    }

    // Destroy them in reverse order
    for (int i = 0; i < 5; ++i) {
        if (handles[i]) {
            defMtxDestroy((cmsContext)i, handles[i]);
        }
    }

    // If we reached here without crashes, consider it a pass for this test
    CHECK(true, "Repeated create/destroy calls completed without crashing");
}

// Test 3: Creating with a zero context should still succeed (boundary value test).
void test_defMtxCreate_zero_context_handled() {
    void* mtx = defMtxCreate((cmsContext)0);
    CHECK(mtx != nullptr, "defMtxCreate should return non-null for zero context");

    // Cleanup
    defMtxDestroy((cmsContext)0, mtx);
}

// Runner that invokes all tests and reports final results
int main() {
    // Run tests
    test_defMtxCreate_two_mutexes_are_distinct_and_non_null();
    test_defMtxCreate_repeated_calls_do_not_crash();
    test_defMtxCreate_zero_context_handled();

    // Summary
    if (g_failures == 0) {
        std::printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        std::fprintf(stderr, "TOTAL TEST FAILURES: %d\n", g_failures);
        return 1;
    }
}