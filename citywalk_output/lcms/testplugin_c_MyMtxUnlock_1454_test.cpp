/*
Unit test suite for the focal method:
  void MyMtxUnlock(cmsContext id, void* mtx)

Context:
- The function casts mtx to MyMtx* and decrements its nlocks field.
- It does not inspect id; the id parameter is unused.
- No conditional branches exist inside the function; behavior is a straightforward decrement.

Strategy notes (Step 1-3 mapping):
- Candidate Keywords: cmsContext, MyMtx, mtx, nlocks, MyMtxUnlock, unlock/decrement, casting, memory layout, test context, non-terminating assertions, cross-language linkage (C to C++).
- Dependencies: We rely on the external C function MyMtxUnlock. We provide a minimal MyMtx type with an int member named nlocks to model the expected layout. We declare cmsContext as a pointer type (typedef void* cmsContext) to minimize type mismatches across compilation units while keeping the interface compatible for testing.
- Coverage: Since the focal method has no branches, coverage will focus on verifying the decrement semantics for various initial nlocks values, and verifying that the id parameter does not affect the outcome.

Note on real-world linking:
- This test assumes the actual MyMtxUnlock is linked into the test binary (e.g., compiled as part of the same project). The test creates a local MyMtx type compatible with the expected layout (nlocks as the first member) and calls the function through its C linkage.

Code (C++11 test suite with non-terminating assertions and explanatory comments for each test):
*/

#include <iostream>
#include <testcms2.h>


// Domain-specific declarations (best-effort compatibility with the focal API).
// We assume the function is provided elsewhere with C linkage.
typedef void* cmsContext;

extern "C" void MyMtxUnlock(cmsContext id, void* mtx);

// Local test model of the MyMtx structure expected by MyMtxUnlock.
// We model only the field that MyMtxUnlock touches, i.e., nlocks.
struct MyMtx {
    int nlocks;
};

// Simple non-terminating assertion macro: reports failure but continues.
// This enables higher coverage by executing subsequent test code.
static int g_total_checks = 0;
static int g_failed_checks = 0;
#define CHECK(cond, msg) do { \
    ++g_total_checks; \
    if (!(cond)) { \
        ++g_failed_checks; \
        std::cerr << "CHECK FAILED: " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while (0)


// Test 1: Unlock should decrement from a positive initial value (e.g., 1 -> 0)
static void test_MyMtxUnlock_DecrementsFromOne() {
    // Candidate scenario: initial nlocks = 1
    MyMtx m{1};
    cmsContext ctx = nullptr; // id is unused by the focal method

    // Act
    MyMtxUnlock(ctx, &m);

    // Assert: nlocks should be 0 after decrement
    CHECK(m.nlocks == 0, "Unlock decremented nlocks from 1 to 0");
}

// Test 2: Unlock should decrement from zero to -1 (demonstrates arithmetic underflow/under decrement behavior)
static void test_MyMtxUnlock_DecrementsFromZero() {
    // Candidate scenario: initial nlocks = 0
    MyMtx m{0};
    cmsContext ctx = reinterpret_cast<cmsContext>(0xDEADBEEF); // arbitrary non-null id

    // Act
    MyMtxUnlock(ctx, &m);

    // Assert: nlocks should be -1 after decrement
    CHECK(m.nlocks == -1, "Unlock decremented nlocks from 0 to -1");
}

// Test 3: Independence across multiple mutexes: ensure one object's unlock does not affect another
static void test_MyMtxUnlock_IndependentObjects() {
    // Create two separate mutex-like objects
    MyMtx m1{5};
    MyMtx m2{2};
    cmsContext ctx = nullptr;

    // Act
    MyMtxUnlock(ctx, &m1); // m1.nlocks becomes 4
    MyMtxUnlock(ctx, &m2); // m2.nlocks becomes 1

    // Assert: each should decrement independently
    CHECK(m1.nlocks == 4, "First mutex decremented from 5 to 4");
    CHECK(m2.nlocks == 1, "Second mutex decremented from 2 to 1");
}

// Test 4: Multiple consecutive unlocks on the same object to ensure cumulative decrements
static void test_MyMtxUnlock_MultipleConsecutiveUnlocks() {
    MyMtx m{3};
    cmsContext ctx = nullptr;

    // Act: perform three unlocks
    MyMtxUnlock(ctx, &m);
    MyMtxUnlock(ctx, &m);
    MyMtxUnlock(ctx, &m);

    // Assert: 3 -> 0
    CHECK(m.nlocks == 0, "Three consecutive unlocks decrement from 3 to 0");
}


// Simple test runner: executes all tests and reports summary.
// This approach aligns with non-GTest environments (no terminating assertions).
int main() {
    // Execute tests
    test_MyMtxUnlock_DecrementsFromOne();
    test_MyMtxUnlock_DecrementsFromZero();
    test_MyMtxUnlock_IndependentObjects();
    test_MyMtxUnlock_MultipleConsecutiveUnlocks();

    // Summary
    std::cout << "Total checks: " << g_total_checks
              << ", Failures: " << g_failed_checks << std::endl;

    // Return non-zero if any test failed (common practice for CI), while preserving non-terminating behavior.
    return (g_failed_checks > 0) ? 1 : 0;
}

/*
Notes for maintainers:
- The tests are non-terminating and rely on a lightweight CHECK macro to report failures while continuing execution.
- We deliberately avoid private member access beyond the MyMtx.nlocks field, respecting encapsulation as much as possible in a test scenario.
- If the real code uses a different cmsContext typedef, adjust the cmsContext alias in this test accordingly or include the proper header that defines cmsContext to ensure type alignment.
- The tests assume the real MyMtxUnlock uses the same memory layout for MyMtx as defined here (nlocks as a member). If the actual layout differs, adapt the MyMtx test structure to match the real definition used in testplugin.c.
- Static helper/macro usage is avoided to simplify linking and compilation in a plain C++11 environment without GTest.