/*
Step 1 (Program Understanding)
- Focus: The focal method is lm_init(deflate_state *s) defined as a local (static) function inside deflate.c.
- Purpose: Initialize the deflate_state structure for a deflate stream. It computes window_size, clears hash state, and sets various configuration parameters based on a configuration_table indexed by s->level. It also initializes several counters/markers to default values (mostly zero or MIN_MATCH-1).
- Key Dependent Components (Candidate Keywords):
  - deflate_state structure fields: window_size, w_size, level, strstart, block_start, lookahead, insert, match_length, prev_length, match_available, ins_h
  - CLEAR_HASH macro
  - configuration_table (indexed by s->level) with fields max_lazy, good_length, nice_length, max_chain
  - MIN_MATCH
  - Data types/macros: ulg (likely unsigned long), zlib-like types (z_streamp, Bytef, etc.)
  - The function is static/local; direct calls from tests are not available without modifying the source. Tests should rely on public interfaces that exercise lm_init indirectly.

Step 2 (Unit Test Generation)
- The focal method lm_init is static to deflate.c, so direct unit-testing of lm_init is not feasible from a separate translation unit without altering the source.
- Therefore, the tests will exercise observable behavior that lm_init influences via public APIs:
  - deflateInit2_: Initialize a deflate state with a given level and verify initialization succeeds.
  - Deflate Compression Path: Initialize with different levels and perform a small compression to ensure the internal state transitions to a functional state (indirectly exercising lm_init’s observable effects through initialization and subsequent operations).
- Test Coverage Intent:
  - True/False branches of condition predicates inside lm_init are not directly exposed, but we exercise through multiple initialization levels (e.g., 0 and 9) which cause different configuration_table lookups.
  - Ensure basic invariants that lm_init is responsible for (initial numeric fields, default counters) are reset to expected non-crashing states via public API usage.
  - Verify monotonic behavior and basic correctness of the deflate path after initialization (nonzero compressed output, proper termination).

Step 3 (Test Case Refinement)
- Use a lightweight C++11 test harness (no GTest) with a simple non-terminating assertion style to maximize code coverage while continuing execution.
- Accessing static lm_init directly is avoided; we rely on deflateInit2_ and deflate for observable effects.
- Ensure tests compile with C++11, only standard library plus provided C APIs, and do not rely on private/internal members.

Final code: a self-contained C++11 test runner exercising lm_init indirectly through public APIs (deflateInit2_ and deflate) with explanatory comments for each unit test.

Note: This code assumes the project provides a C API compatible with the included deflate.h (types like z_streamp, Bytef, Z_OK, Z_DEFLATED, Z_FINISH, Z_DEFAULT_STRATEGY, etc., as used by miniz/zlib-like APIs). The tests are designed to be portable across typical deflate.h based projects and do not rely on private/internal state access.

Code begins here:

*/

#include <iostream>
#include <string>
#include <deflate.h>
#include <cstring>


// Step 2: Provide C linkage for the included C headers to test the focal C function via public APIs.
// We rely on the deflate.h interface seen in the dependency listing (<FOCAL_CLASS_DEP>).
extern "C" {
}

// Lightweight test harness (non-terminating assertions) to maximize code coverage.
static int g_total_tests = 0;
static int g_passed_tests = 0;

// Non-terminating assertion helper
#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (cond) { ++g_passed_tests; } else { \
        std::cerr << "Test Failure: " << (msg) << std::endl; \
    } \
} while (0)

// Helper: reset global counters between test cases
static void reset_counters() {
    g_total_tests = 0;
    g_passed_tests = 0;
}

/*
Unit Test 1: Initialization via public API (deflateInit2_) for multiple levels
Rationale:
- lm_init uses configuration_table[s->level].* values. By initializing with different levels (e.g., 0 and 9),
  we exercise potential variations in the configuration path without relying on exact numeric table contents.
- We verify that deflateInit2_ succeeds (returns Z_OK) for each level, ensuring the static lm_init path (via the public API) can successfully initialize the internal state.
*/
static bool test_deflate_init_multiple_levels() {
    const int levels[] = {0, 9}; // True test cases for different configuration branches
    const size_t num_levels = sizeof(levels) / sizeof(levels[0]);
    const char* version = "unittest_version";

    // Prepare a minimal z_stream
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    bool all_ok = true;

    for (size_t i = 0; i < num_levels; ++i) {
        int level = levels[i];
        // Reinitialize stream for each run
        std::memset(&strm, 0, sizeof(strm));

        // Some implementations require non-null zalloc/zfree; we set to Z_NULL if available
        // Note: Z_NULL is typically defined in zlib; if not, rely on 0/NULL by virtue of header.
        // Here we set to Z_NULL if defined, otherwise 0.
#ifdef Z_NULL
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
#else
        strm.zalloc = 0;
        strm.zfree = 0;
        strm.opaque = nullptr;
#endif

        int ret = deflateInit2_(&strm, level, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY,
                               version, 0);
        if (ret != Z_OK) {
            std::cerr << "deflateInit2_ failed for level " << level
                      << " with return code " << ret << std::endl;
            all_ok = false;
        } else {
            deflateEnd(&strm);
        }
        // No static/private access; rely on public init path success.
    }

    return all_ok;
}

/*
Unit Test 2: End-to-end compress with small input for different levels
Rationale:
- After initialization, perform a small compression operation to ensure the internal state (as initialized by lm_init) is functional enough to process data.
- We test with at least two levels to exercise potential code branches influenced by configuration_table values.
- We verify that the compressor produces some output and completes cleanly (Z_STREAM_END).
*/
static bool test_deflate_small_input_compression_levels() {
    const char* input = "The quick brown fox jumps over the lazy dog";
    const size_t input_len = std::strlen(input);
    unsigned char output[1024];

    const int levels[] = {0, 9};
    const size_t num_levels = sizeof(levels) / sizeof(levels[0]);
    bool all_ok = true;

    for (size_t i = 0; i < num_levels; ++i) {
        int level = levels[i];
        z_stream strm;
        std::memset(&strm, 0, sizeof(strm));

        // Initialize zalloc/zfree as per typical usage in tests
#ifdef Z_NULL
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
#else
        strm.zalloc = 0;
        strm.zfree = 0;
        strm.opaque = nullptr;
#endif

        int ret = deflateInit2_(&strm, level, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY,
                               "unittest", 0);
        if (ret != Z_OK) {
            std::cerr << "deflateInit2_ failed for level " << level
                      << " with return code " << ret << std::endl;
            all_ok = false;
            continue;
        }

        strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input));
        strm.avail_in = static_cast<uInt>(input_len);

        strm.next_out = reinterpret_cast<Bytef*>(output);
        strm.avail_out = sizeof(output);

        ret = deflate(&strm, Z_FINISH);
        if (ret != Z_STREAM_END) {
            std::cerr << "deflate did not finish properly for level " << level
                      << ". ret=" << ret << std::endl;
            all_ok = false;
        }

        deflateEnd(&strm);

        // Ensure some output was produced
        EXPECT_TRUE(strm.total_out > 0, "Expected non-zero compressed output length");
    }

    return all_ok;
}

/*
Main: Run the test suite and print a summary.
- We use non-terminating expectations (EXPECT_TRUE) to maximize code execution and path coverage.
- The tests rely solely on public C APIs (deflateInit2_, deflate, deflateEnd) to exercise the focal initialization indirectly.
*/
int main() {
    reset_counters();

    std::cout << "Running unit tests for lm_init via public APIs in deflate.c (no direct access to static lm_init).\n";

    // Test 1: Initialization across multiple levels
    bool t1 = test_deflate_init_multiple_levels();
    EXPECT_TRUE(t1, "Initialization should succeed for levels 0 and 9 via deflateInit2_");

    // Test 2: End-to-end compression after initialization with multiple levels
    bool t2 = test_deflate_small_input_compression_levels();
    EXPECT_TRUE(t2, "Compression should succeed for levels 0 and 9 and produce output");

    // Summary
    std::cout << "Test Summary: " << g_passed_tests << " / " << g_total_tests << " tests passed.\n";

    // Return non-zero if any test failed
    return (g_total_tests == g_passed_tests) ? 0 : 1;
}