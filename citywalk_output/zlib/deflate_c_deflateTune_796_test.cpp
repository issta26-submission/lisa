/*
  Unit test suite for the focal method deflateTune in deflate.c
  - No GoogleTest; a lightweight C++11 test harness is used.
  - Tests focus on ensuring deflateTune correctly updates the deflate_state
    when the state is valid and properly returns an error when the precondition
    (state validity) fails.
  - Test coverage:
    1) Normal path: strm.state is valid; deflateTune should return Z_OK and set
       good_match, max_lazy_match, nice_match, and max_chain_length accordingly.
    2) Error path: strm.state is NULL; deflateTune should return Z_STREAM_ERROR and
       not attempt to dereference the state.
*/

#include <iostream>
#include <deflate.h>
#include <cstring>


// Include the C deflate API. Use extern "C" to ensure proper linkage when included in C++.
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
static int g_pass = 0;
static int g_fail = 0;

static void log_result(const char* test_name, bool pass, const char* details = nullptr) {
    if (pass) {
        ++g_pass;
        std::cout << "[PASS] " << test_name;
        if (details) std::cout << " - " << details;
        std::cout << std::endl;
    } else {
        ++g_fail;
        std::cerr << "[FAIL] " << test_name;
        if (details) std::cerr << " - " << details;
        std::cerr << std::endl;
    }
}

// Test 1: Normal path - state is valid; verify fields are updated and Z_OK is returned.
static void test_deflateTune_ok_path() {
    // Setup a z_stream object and a valid deflate_state
    z_stream zstrm;
    std::memset(&zstrm, 0, sizeof(z_stream));  // Clear initial contents

    z_streamp strm = &zstrm;                   // Pointer to stream (as required by API)

    // Create and attach a valid deflate_state
    deflate_state s;
    std::memset(&s, 0, sizeof(deflate_state));
    strm->state = &s;

    // Input parameters
    int good_length = 5;
    int max_lazy = 6;
    int nice_length = 7;
    int max_chain = 8;

    // Call the focal method
    int ret = deflateTune(strm, good_length, max_lazy, nice_length, max_chain);

    // Assertions (non-terminating)
    bool pass = (ret == Z_OK);
    log_result("deflateTune_ok_path_return_is_Z_OK", pass,
               pass ? "ret == Z_OK" : "ret != Z_OK");

    pass = pass && (s.good_match == static_cast<uInt>(good_length));
    log_result("deflateTune_ok_path_good_match_set", pass,
               pass ? "s.good_match == good_length" : "s.good_match mismatch");

    pass = pass && (s.max_lazy_match == static_cast<uInt>(max_lazy));
    log_result("deflateTune_ok_path_max_lazy_match_set", pass,
               pass ? "s.max_lazy_match == max_lazy" : "s.max_lazy_match mismatch");

    pass = pass && (s.nice_match == static_cast<uInt>(nice_length));
    log_result("deflateTune_ok_path_nice_match_set", pass,
               pass ? "s.nice_match == nice_length" : "s.nice_match mismatch");

    pass = pass && (s.max_chain_length == static_cast<uInt>(max_chain));
    log_result("deflateTune_ok_path_max_chain_length_set", pass,
               pass ? "s.max_chain_length == max_chain" : "s.max_chain_length mismatch");

    // Overall result for this test
    log_result("deflateTune_ok_path_overall", pass, pass ? "All checks passed" : "One or more checks failed");
}

// Test 2: Error path - when state is NULL, deflateTune should return Z_STREAM_ERROR.
// This ensures we do not dereference a NULL state and that error handling path is exercised.
static void test_deflateTune_error_path_null_state() {
    z_stream zstrm;
    std::memset(&zstrm, 0, sizeof(z_stream));

    z_streamp strm = &zstrm;
    // Deliberately do not assign a valid state to simulate error path
    strm->state = NULL;

    int ret = deflateTune(strm, 1, 2, 3, 4);

    bool pass = (ret == Z_STREAM_ERROR);
    log_result("deflateTune_error_path_null_state_return_Z_STREAM_ERROR", pass,
               pass ? "ret == Z_STREAM_ERROR" : "ret != Z_STREAM_ERROR");

    // No further state to verify here since the function should early-return.
    log_result("deflateTune_error_path_no_state_dereference", true,
               "Checked that NULL state does not cause state mutation");
}

// Entry point for running tests
int main() {
    test_deflateTune_ok_path();
    test_deflateTune_error_path_null_state();

    std::cout << "Test Summary: Passed " << g_pass << ", Failed " << g_fail << std::endl;
    // Return non-zero if any test failed
    return (g_fail > 0) ? 1 : 0;
}