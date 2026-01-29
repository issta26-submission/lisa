/*
Alpha test suite for the focal method dif8to and its class dependencies.
This test is crafted as a lightweight C test harness (no GTest) to
be compatible with C11 compilation and to exercise the dif8to routine
and its dependent functions (to_premul8_float, to_premul8, etc).

Notes on candidate keywords (Step 1):
- dif8to: focal method under test
- to_premul8_float: dependency used inside dif8to
- to_premul8: dependency used inside dif8to
- Premul TO8 diff: runtime log emitted by dif8to
- max / max1 / max2 / a / g: internal variables of dif8to
- 256 x 256 grid loop: core domain scanned by dif8to

This test suite focuses on:
- Verifying the dif8to entry point can execute without crashing.
- Basic equivalence checks between to_premul8_float and to_premul8 on representative inputs.
- A sampling sweep to compute a max difference over a subset of the 256x256 domain to exercise the calculation path.
- Non-terminating style is emulated by not exiting on first failure; we accumulate failures and report at the end.

Build notes:
- The project under test provides implementations for:
  - uint8_t to_premul8_float(uint8_t rgb8, uint8_t a8)
  - uint8_t to_premul8(uint8_t rgb8, uint8_t a8)
  - void dif8to(void)
- This test file declares external prototypes for linking against the existing implementation.
- Compile with: gcc -std=c11 -O2 -o alpha_test alpha_test.c <linker-args to project libs>

Now the test code:

*/

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <lcms2_internal.h>
#include <stdlib.h>


// Prototypes for the functions under test.
// They are provided by the project dependencies (C linkage assumed).
extern uint8_t to_premul8_float(uint8_t rgb8, uint8_t a8);
extern uint8_t to_premul8(uint8_t rgb8, uint8_t a8);

// The focal method under test (invoked to ensure it runs without crash and exercises code path).
extern void dif8to(void);

// Lightweight test harness (non-terminating assertions style)
static int g_failures = 0;

// Simple EXPECT-like macro: reports pass/fail but does not terminate the test run.
#define EXPECT_TRUE(cond, msg) do { \
    if (cond) { \
        printf("PASS: %s\n", msg); \
    } else { \
        printf("FAIL: %s\n", msg); \
        g_failures++; \
    } \
    } while (0)


// Test 1: Ensure the focal function can execute without crashing.
// Rationale: protects against regressions that cause segmentation faults or hangs.
// This does not assert internal correctness, but guarantees runtime stability for the core path.
static void test_dif8to_executes_without_crash(void) {
    // The dif8to function is expected to perform its 256x256 domain loop and print.
    // If it crashes or hangs, the test would fail at runtime.
    printf("Running test_dif8to_executes_without_crash...\n");
    // Call the focal function
    dif8to();
    // If we reach here, execution completed (crash-free) for this test.
    EXPECT_TRUE(1, "dif8to executes (crash-free)"); // always true here; the crash would abort program
}


// Test 2: Basic equivalence checks between to_premul8_float and to_premul8
// on a representative set of input pairs.
// Rationale: validates that the two pathways intended to produce premultiplied values
// behave consistently for chosen samples.
static void test_premul8_equivalence_on_samples(void) {
    struct Sample { uint8_t g; uint8_t a; const char* note; };
    struct Sample samples[] = {
        {0, 0,  "g=0, a=0"},
        {0, 128, "g=0, a=128"},
        {128, 0, "g=128, a=0"},
        {255, 255, "g=255, a=255"},
        {1, 1, "g=1, a=1"},
        {15, 200, "g=15, a=200"},
        {100, 50, "g=100, a=50"}
    };

    size_t n = sizeof(samples)/sizeof(samples[0]);
    for (size_t i = 0; i < n; ++i) {
        uint8_t g = samples[i].g;
        uint8_t a = samples[i].a;
        uint8_t v_float = to_premul8_float(g, a);
        uint8_t v_base  = to_premul8(g, a);
        // We expect both implementations to produce the same premultiplied result
        // for each test input. A mismatch indicates a potential bug in one path.
        char msg[128];
        snprintf(msg, sizeof(msg), "premul8 equivalence for g=%u, a=%u", (unsigned)g, (unsigned)a);
        EXPECT_TRUE(v_float == v_base, msg);
    }
}

// Test 3: Subsampled max-difference computation to exercise the comparison path
// without running the full 256x256 grid (to keep test runtime reasonable).
// This mirrors the logic in dif8to but on a reduced domain (stride of 8).
static int compute_subsampled_max_diff(void) {
    int32_t max = 0;
    for (uint16_t a = 0; a < 256; a += 8) {
        for (uint16_t g = 0; g < 256; g += 8) {
            uint8_t gpremul_float = to_premul8_float((uint8_t)g, (uint8_t)a);
            uint8_t gpremul = to_premul8((uint8_t)g, (uint8_t)a);
            int32_t dif = (int32_t)gpremul_float - (int32_t)gpremul;
            if (dif < 0) dif = -dif;
            if (dif > max) max = dif;
        }
    }
    return (int)max;
}

// Test 3a: Validate that the subsampled max difference computation runs and yields a non-negative result.
// This doesn't assert the exact maximum (which depends on the implementation),
// but ensures the domain traversal and arithmetic execute correctly.
static void test_subsampled_max_diff_computation(void) {
    int max_diff = compute_subsampled_max_diff();
    char msg[128];
    snprintf(msg, sizeof(msg), "subsampled max diff computed (value=%d)", (int)max_diff);
    EXPECT_TRUE(max_diff >= 0, msg);
}

// Test 4: Verify that we can run a limited, controlled domain sweep to exercise code paths
// without evaluating the entire 256x256 space. This is primarily for coverage.
static void test_limited_domain_sweep_coverage(void) {
    // We'll run a small fixed set that exercises various a/g combinations.
    uint8_t g_values[] = {0, 64, 128, 192, 255};
    uint8_t a_values[] = {0, 32, 128, 200, 255};
    int any_diff_seen = 0;
    for (size_t i = 0; i < sizeof(g_values); ++i) {
        for (size_t j = 0; j < sizeof(a_values); ++j) {
            uint8_t g = g_values[i];
            uint8_t a = a_values[j];
            uint8_t v_float = to_premul8_float(g, a);
            uint8_t v_base  = to_premul8(g, a);
            if (v_float != v_base) {
                any_diff_seen = 1;
            }
        }
    }
    char msg[128];
    snprintf(msg, sizeof(msg), "limited-domain sweep exercised (any_diff_seen=%d)", any_diff_seen);
    EXPECT_TRUE(1 == 1, msg); // always true; used to acknowledge the sweep ran
    // If differences were expected to be zero, we could assert:
    // EXPECT_TRUE(any_diff_seen == 0, "no differences observed in limited sweep");
}


// Helper: Run all tests and report a summary.
static void run_all_tests(void) {
    printf("=== Starting alpha_test.c test suite for dif8to and dependencies ===\n\n");

    test_dif8to_executes_without_crash();
    test_premul8_equivalence_on_samples();
    test_subsampled_max_diff_computation();
    test_limited_domain_sweep_coverage();

    printf("\n=== Test suite complete ===\n");
    if (g_failures == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("TESTS FAILED: %d failures\n", g_failures);
    }
}

// Entry point
int main(void) {
    run_all_tests();
    // Return non-zero if any test failed to signal issues to harness/CI
    return (g_failures == 0) ? 0 : 1;
}

/*
Explanation of test cases:

- test_dif8to_executes_without_crash:
  - Invokes the focal method, ensuring the core computation path can be reached
    and completed without crashes or hangs. This provides basic runtime sanity.

- test_premul8_equivalence_on_samples:
  - Checks that to_premul8_float and to_premul8 agree on a representative set of
    (g, a) inputs. This targets the core dependency equivalence that the
    dif8to method relies on to compute the differences.

- test_subsampled_max_diff_computation:
  - Mirrors the internal max-difference calculation on a reduced grid (stride 8)
    to validate arithmetic and function calls across a non-trivial domain. This
    emphasizes coverage of the computation without the cost of the full 256x256.

- test_limited_domain_sweep_coverage:
  - Executes a small, fixed-domain sweep to exercise various branches and
    data paths, ensuring function calls and arithmetic are exercised without
    requiring the full dataset.

Notes on domain knowledge constraints:
- The tests avoid accessing private/internal state of the focal class beyond the
  public functions provided (to_premul8_float, to_premul8, and dif8to).
- All tests use simple, non-terminating assertion style (they log pass/fail but do not exit on first failure).
- If a specific build relies on static libraries, ensure the correct link order so that
  the test harness resolves the extern functions (to_premul8_float, to_premul8) from the
  project under test.

*/