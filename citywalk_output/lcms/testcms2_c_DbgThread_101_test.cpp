// Unit tests for cmsContext DbgThread(void)
// This test suite targets the focal method DbgThread defined in testcms2.c
// It validates the pointer-like value returned by DbgThread across successive calls,
// including wrap-around after 0xff0 (4080) iterations as per n++ % 0xff0.
// The tests are implemented in C++11 and do not rely on GoogleTest.

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Bring in C declarations with C linkage to ensure correct symbol resolution
extern "C" {
}

// Helper to view cmsContext as an integer for deterministic comparisons
static inline std::uintptr_t as_uintptr(cmsContext ctx) {
    return reinterpret_cast<std::uintptr_t>(ctx);
}

// Step 2: Unit Test Generation
// Test 1: Verify the first two calls yield 1 and 2 respectively.
// Rationale: The static counter n starts at 1, and the function returns (n++ % 0xff0).
// Therefore, the very first call should produce 1, the second 2.
static bool test_dbgthread_sequence() {
    cmsContext c1 = DbgThread();
    cmsContext c2 = DbgThread();

    std::uintptr_t v1 = as_uintptr(c1);
    std::uintptr_t v2 = as_uintptr(c2);

    bool ok = (v1 == 1 && v2 == 2);
    if (!ok) {
        std::cerr << "DbgThread sequence test failed: expected (1, 2), got (" 
                  << v1 << ", " << v2 << ")\n";
    } else {
        std::cout << "[PASS] DbgThread sequence yields 1 then 2\n";
    }
    return ok;
}

// Test 2: Validate wrap-around behavior after 4080 calls and subsequent continuity.
// Rationale: After 4080 calls, n will have progressed such that (n % 0xff0) == 0,
// i.e., DbgThread should return a null-offset pointer (value 0). The next call should yield 1 again.
// This tests the modulo boundary and static counter progression without resetting state.
static bool test_dbgthread_wrap_and_cycle() {
    cmsContext tmp;
    std::uintptr_t val = 0;

    // Advance 4078 more times after the initial two calls from test_dbgthread_sequence().
    // The last iteration (i == 4077) corresponds to the 4080th overall call and should yield 0.
    for (int i = 0; i < 4078; ++i) {
        tmp = DbgThread();
        val = as_uintptr(tmp);
    }

    // After the above loop, val should be 0 (wrap point)
    if (val != 0) {
        std::cerr << "DbgThread wrap test failed: expected 0 at wrap point, got " 
                  << val << "\n";
        return false;
    }

    // The very next call should yield 1 again after wrap
    cmsContext afterWrap = DbgThread();
    std::uintptr_t afterVal = as_uintptr(afterWrap);
    if ( afterVal != 1 ) {
        std::cerr << "DbgThread wrap test failed: expected 1 after wrap, got " 
                  << afterVal << "\n";
        return false;
    }

    std::cout << "[PASS] DbgThread wrap-around to 0 then returns 1 after wrap\n";
    return true;
}

// Step 3: Test Case Refinement (explanatory within tests via comments and output)
// Note: This test suite intentionally relies on the static counter's preserved state
// across calls, matching the real-world behavior of cmsContext DbgThread within a single program run.

int main(void) {
    int failures = 0;

    // Run tests in a deterministic order to ensure state consistency
    if (!test_dbgthread_sequence()) {
        ++failures;
    }

    if (!test_dbgthread_wrap_and_cycle()) {
        ++failures;
    }

    if (failures == 0) {
        std::cout << "All DbgThread unit tests passed.\n";
        return 0;
    } else {
        std::cerr << failures << " DbgThread unit test(s) failed.\n";
        return 1;
    }
}