// Unit tests for the focal method: deflateParams
// This test suite is designed for C++11, does not use GTest, and relies on
// the project-supplied C interfaces (deflate.h, and the deflateParams implementation
// in deflate.c). It exercises key branches of deflateParams while keeping
// test code self-contained and executable via a simple main() harness.
// Comments explain the intent of each test case and the observed expectations.

#include <iostream>
#include <deflate.h>
#include <cstdlib>
#include <cstring>


// Bring in C declarations with C linkage
extern "C" {
}

// Simple non-terminating test harness
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define TEST_EXPECT_EQ(desc, a, b) do { \
    ++g_tests_run; \
    if ((a) == (b)) { \
        std::cout << "[PASS] " << desc << std::endl; \
        ++g_tests_passed; \
    } else { \
        std::cerr << "[FAIL] " << desc << " | Expected: " << (b) << "  Actual: " << (a) << std::endl; \
    } \
} while (0)


// Helper to initialize a z_streamp with a fresh deflate_state
static void init_strm_with_state(z_streamp strm, deflate_state *state) {
    if (!strm || !state) return;
    // Clear the structures to a known baseline
    std::memset(strm, 0, sizeof(z_stream));
    std::memset(state, 0, sizeof(deflate_state));

    // Wire up the relationship expected by deflateParams
    strm->state = state;

    // Provide sane default internal values for the state to allow deflateParams
    // to run through without requiring a full compression setup.
    // These fields are accessed by deflateParams in the provided code path.
    state->level = 0;
    state->strategy = 0;
    state->last_flush = 0;
    state->strstart = 0;
    state->block_start = 0;
    state->lookahead = 0;
    state->matches = 0;
}


// Test 1: Default compression level (Z_DEFAULT_COMPRESSION) should map to 6 in non-FASTEST mode
// and update the internal s->level accordingly.
static void test_deflateParams_maps_default_to_six() {
    z_stream strm;
    deflate_state *state = (deflate_state *)std::malloc(sizeof(deflate_state));
    if (!state) {
        std::cerr << "Failed to allocate deflate_state for test_deflateParams_maps_default_to_six" << std::endl;
        return;
    }
    init_strm_with_state(&strm, state);

    // Pre-condition: level=0, s->level=0, strategy=0
    state->level = 0;
    state->strategy = 0;
    // Ensure avail_in remains zero to avoid side-effects
    strm.avail_in = 0;

    // Call the function under test
    int ret = deflateParams(&strm, Z_DEFAULT_COMPRESSION, 0);

    // Post-conditions:
    // 1) Should return Z_OK
    // 2) s->level should be updated to 6 (the mapped value for default compression)
    TEST_EXPECT_EQ("deflateParams should return Z_OK for default compression mapping", ret, Z_OK);
    TEST_EXPECT_EQ("deflateParams should map Z_DEFAULT_COMPRESSION to level 6", state->level, 6);

    // Cleanup
    std::free(state);
}

// Test 2: Changing level from an existing non-zero level updates internal configuration fields.
// Specifically, s->level should become the requested level and related lazy matching fields
// should reflect configuration_table[level].
static void test_deflateParams_updates_configuration_when_level_changes() {
    z_stream strm;
    deflate_state *state = (deflate_state *)std::malloc(sizeof(deflate_state));
    if (!state) {
        std::cerr << "Failed to allocate deflate_state for test_deflateParams_updates_configuration_when_level_changes" << std::endl;
        return;
    }
    init_strm_with_state(&strm, state);

    // Pre-condition: exist level 2
    state->level = 2;
    state->strategy = 0;
    strm.avail_in = 0;

    // Request a new level (e.g., 4) with same strategy to avoid triggering the flush/deflate path
    int ret = deflateParams(&strm, 4, 0);

    // Post-conditions:
    // 1) Should return Z_OK
    // 2) s->level should be updated to 4
    // 3) s->max_lazy_match, s->good_match, s->nice_match, s->max_chain_length
    //    should reflect configuration_table[4]'s values.
    TEST_EXPECT_EQ("deflateParams should return Z_OK when changing level from 2 to 4", ret, Z_OK);
    TEST_EXPECT_EQ("deflateParams should set level to 4", state->level, 4);

    // Validate configuration_table[4] mapping onto state fields
    TEST_EXPECT_EQ("state->max_lazy_match equals configuration_table[4].max_lazy", state->max_lazy_match, configuration_table[4].max_lazy);
    TEST_EXPECT_EQ("state->good_match equals configuration_table[4].good_length", state->good_match, configuration_table[4].good_length);
    TEST_EXPECT_EQ("state->nice_match equals configuration_table[4].nice_length", state->nice_match, configuration_table[4].nice_length);
    TEST_EXPECT_EQ("state->max_chain_length equals configuration_table[4].max_chain", state->max_chain_length, configuration_table[4].max_chain);

    std::free(state);
}

// Test 3: Invalid parameters should yield Z_STREAM_ERROR
// We test two invalid scenarios: negative level and overly large strategy.
static void test_deflateParams_invalid_parameters() {
    z_stream strm;
    deflate_state *state = (deflate_state *)std::malloc(sizeof(deflate_state));
    if (!state) {
        std::cerr << "Failed to allocate deflate_state for test_deflateParams_invalid_parameters" << std::endl;
        return;
    }
    init_strm_with_state(&strm, state);

    // Case A: negative level
    int ret_neg = deflateParams(&strm, -1, 0);
    TEST_EXPECT_EQ("deflateParams with negative level should return Z_STREAM_ERROR", ret_neg, Z_STREAM_ERROR);

    // Case B: overly large strategy
    int ret_big = deflateParams(&strm, 0, Z_FIXED + 1);
    TEST_EXPECT_EQ("deflateParams with overly large strategy should return Z_STREAM_ERROR", ret_big, Z_STREAM_ERROR);

    std::free(state);
}

// Test 4: Ensure the function handles a zero level and zero strategy cleanly when no conflicting state exists.
// This primarily exercises the path that doesn't flush the last buffer (to keep tests lightweight).
static void test_deflateParams_no_flush_when_parameters_match() {
    z_stream strm;
    deflate_state *state = (deflate_state *)std::malloc(sizeof(deflate_state));
    if (!state) {
        std::cerr << "Failed to allocate deflate_state for test_deflateParams_no_flush_when_parameters_match" << std::endl;
        return;
    }
    init_strm_with_state(&strm, state);

    // Set up to equal current configuration so the flush path is not invoked.
    state->level = 0;
    state->strategy = 0;
    // Match local level to s->level to avoid the deflate(...) call path
    int ret = deflateParams(&strm, 0, 0);

    TEST_EXPECT_EQ("deflateParams with matching level/strategy should return Z_OK", ret, Z_OK);
    TEST_EXPECT_EQ("state->level should remain 0 when input level is 0", state->level, 0);

    std::free(state);
}


// Main runner
int main() {
    // Step 3: We run a small, focused subset of tests that cover the critical branches
    // and avoid depending on static/internal behaviors that are difficult to mock.

    // IMPORTANT: The test suite assumes FASTEST is not defined for these tests to test default mapping.
    // If you compile with -D FASTEST, some branch behaviors change (level mapping to 1 when level != 0).
    // You can also compile an alternate test binary with -D FASTEST to exercise those branches separately.

    test_deflateParams_maps_default_to_six();
    test_deflateParams_updates_configuration_when_level_changes();
    test_deflateParams_invalid_parameters();
    test_deflateParams_no_flush_when_parameters_match();

    std::cout << "Total tests run: " << g_tests_run << ", Passed: " << g_tests_passed << std::endl;
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}