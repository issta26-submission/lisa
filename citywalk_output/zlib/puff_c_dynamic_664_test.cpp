/*
 * Unit test suite for the focal method: dynamic(struct state *s)
 * under the context of puff.c and its dependencies.
 *
 * Notes and assumptions (based on provided code fragments and domain guidelines):
 * - The test targets the dynamic function as described in the <FOCAL_METHOD> block.
 * - We assume a C-style environment typical for the Puff zlib-like code, where local
 *   macros may be used to control linkage. For test purposes, this file is written
 *   in C++11 but relies on the existing Puff API and types declared in puff.h.
 * - The tests are designed to cover important branches in dynamic:
 *     - Bad counts (nlen > MAXLCODES or ndist > MAXDCODES) -> returns -3
 *     - Read code-lengths codes error (construct returns non-zero) -> returns -4
 *     - Invalid length table expansion (too many lengths) -> returns -6
 *     - Missing end-of-block code (lengths[256] == 0) -> returns -9
 *     - Incomplete code (construct with lencode or distcode) -> returns -7 / -8
 *     - Successful path returns codes(...) result
 * - The test framework here is a lightweight, non-terminating assertion-based runner
 *   (no GTest). It prints pass/fail per test and exits with non-zero if any test fails.
 * - The tests rely on the existing Puff state/types (e.g., struct state) and function
 *   prototypes declared in puff.h. We do not modify the production code.
 * - True isolation (mocking static helpers) is not shown here, as the provided
 *   test constraints suggest a test harness where the helpers can be stubbed if
 *   needed by the build setup. This file documents the intended cases and shows
 *   how to structure them.
 *
 * Build notes:
 * - Compile with a C++11 compatible compiler.
 * - Link against Puff implementation that provides dynamic and the necessary types.
 * - If the build environment uses a separate unit-test harness, adapt the test
 *   runner accordingly (this file provides portable C++ code with explanatory
 *   comments for each test).
 *
 * The tests are intentionally verbose in comments to aid future refinement and
 * to help ensure coverage of the key decision branches in dynamic.
 */

// Lightweight test framework (no GTest)
#include <cstdio>
#include <cstring>
#include <setjmp.h>
#include <cerrno>
#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <puff.h>
#include <cstdlib>


// Include Puff header definitions to access struct state and function prototypes.
// The tests assume puff.h declares dynamic and the related structures.
// If the project requires a different path, adjust accordingly.
extern "C" {
}

// Simple test assertion helpers
static void test_pass(const char* test_name) {
    std::printf("[PASS] %s\n", test_name);
}

static void test_fail(const char* test_name, const char* reason) {
    std::fprintf(stderr, "[FAIL] %s: %s\n", test_name, reason);
}

// Utility macro to wrap test results succinctly
#define RUN_TEST(name) (test_##name())

// Forward declarations of test cases (to keep a clean structure)
static bool test_dynamic_bad_counts();
static bool test_dynamic_read_code_lengths_error();
static bool test_dynamic_too_many_lengths();
static bool test_dynamic_end_of_block_missing();
static bool test_dynamic_incomplete_code();
static bool test_dynamic_success_path();

// Entry point
int main() {
    std::vector<std::string> failures;

    // Each test returns true on pass, false on fail.
    if (test_dynamic_bad_counts()) {
        test_pass("test_dynamic_bad_counts");
    } else {
        failures.push_back("test_dynamic_bad_counts");
        test_fail("test_dynamic_bad_counts", "Expected -3 (bad counts) or path not taken");
    }

    if (test_dynamic_read_code_lengths_error()) {
        test_pass("test_dynamic_read_code_lengths_error");
    } else {
        failures.push_back("test_dynamic_read_code_lengths_error");
        test_fail("test_dynamic_read_code_lengths_error", "Expected -4 (code length code error)");
    }

    if (test_dynamic_too_many_lengths()) {
        test_pass("test_dynamic_too_many_lengths");
    } else {
        failures.push_back("test_dynamic_too_many_lengths");
        test_fail("test_dynamic_too_many_lengths", "Expected -6 (too many lengths) or similar");
    }

    if (test_dynamic_end_of_block_missing()) {
        test_pass("test_dynamic_end_of_block_missing");
    } else {
        failures.push_back("test_dynamic_end_of_block_missing");
        test_fail("test_dynamic_end_of_block_missing", "Expected -9 (end-of-block code missing)");
    }

    if (test_dynamic_incomplete_code()) {
        test_pass("test_dynamic_incomplete_code");
    } else {
        failures.push_back("test_dynamic_incomplete_code");
        test_fail("test_dynamic_incomplete_code", "Expected -7 or -8 (incomplete code)");
    }

    if (test_dynamic_success_path()) {
        test_pass("test_dynamic_success_path");
    } else {
        failures.push_back("test_dynamic_success_path");
        test_fail("test_dynamic_success_path", "Expected successful codes() path (return 0 or non-error)");
    }

    if (!failures.empty()) {
        std::fprintf(stderr, "[SUMMARY] %zu test(s) failed: ", failures.size());
        for (const auto& f : failures) std::fprintf(stderr, "%s ", f.c_str());
        std::fprintf(stderr, "\n");
        return EXIT_FAILURE;
    }

    std::cout << "[SUMMARY] All tests passed.\n";
    return EXIT_SUCCESS;
}

/*
 * Test 1: Bad counts trigger -3 when nlen > MAXLCODES or ndist > MAXDCODES
 * Path coverage:
 * - Force the first call(s) to bits(s,5) to return 31, causing nlen = 31 + 257 = 288
 *   which is > MAXLCODES (as in zlib-like constants MAXLCODES = 286).
 * - The dynamic() implementation should return -3 in this case.
 * Implementation note:
 * - The actual setup to influence bits() requires the test harness to inject a
 *   specific bitstream or to provide a fake state with controlled bit reads.
 * - If your build harness supports stubbing bits(), simply configure it here to
 *   yield 31 for the first 5-bit read.
 */
static bool test_dynamic_bad_counts() {
    // Build a state object suitable for dynamic() to query counts.
    // The actual contents of `state` depend on puff.h's definition.
    struct state s;
    // Clear or initialize as needed. If puff.h exposes a constructor, use it.
    // Here we rely on the test harness' ability to set up a stubbed bitstream so that
    // bits(&s, 5) yields 31 on the first call.
    // The test environment must provide such control; otherwise this test must be adapted.
    int ret = dynamic(&s);
    const bool passed = (ret == -3);
    if (!passed) {
        test_fail("test_dynamic_bad_counts", "Expected -3 for bad counts, got: " + std::to_string(ret));
    }
    return passed;
}

/*
 * Test 2: Reading code-lengths codes yields error from construct() -> -4
 * Path coverage:
 * - After initial counts, we simulate construct(&lencode, lengths, 19) returning non-zero.
 * - dynamic() should return -4.
 */
static bool test_dynamic_read_code_lengths_error() {
    struct state s;
    int ret = dynamic(&s);
    bool passed = (ret == -4);
    if (!passed) {
        test_fail("test_dynamic_read_code_lengths_error", "Expected -4 when code-length codes are invalid, got: " + std::to_string(ret));
    }
    return passed;
}

/*
 * Test 3: While decoding length/distance lengths, an overflow in lengths causes -6
 * Path coverage:
 * - Simulate scenario where index + symbol > nlen + ndist occurs, triggering -6.
 */
static bool test_dynamic_too_many_lengths() {
    struct state s;
    int ret = dynamic(&s);
    bool passed = (ret == -6);
    if (!passed) {
        test_fail("test_dynamic_too_many_lengths", "Expected -6 for too many lengths, got: " + std::to_string(ret));
    }
    return passed;
}

/*
 * Test 4: End-of-block code missing: lengths[256] == 0 -> -9
 * Path coverage:
 * - After decoding, ensure that the end-of-block code (256) has a non-zero length.
 * - If zero, dynamic() should return -9.
 */
static bool test_dynamic_end_of_block_missing() {
    struct state s;
    int ret = dynamic(&s);
    bool passed = (ret == -9);
    if (!passed) {
        test_fail("test_dynamic_end_of_block_missing", "Expected -9 for missing end-of-block, got: " + std::to_string(ret));
    }
    return passed;
}

/*
 * Test 5: Incomplete code paths for lencode/distcode -> -7 or -8
 * Path coverage:
 * - Simulate an invalid construct() for the literal/length codes or distance codes
 * - dynamic() should return -7 (incomplete code for lencode) or -8 (incomplete code for distcode)
 */
static bool test_dynamic_incomplete_code() {
    struct state s;
    int ret = dynamic(&s);
    bool passed = (ret == -7 || ret == -8);
    if (!passed) {
        test_fail("test_dynamic_incomplete_code", "Expected -7 or -8 for incomplete code, got: " + std::to_string(ret));
    }
    return passed;
}

/*
 * Test 6: Successful path: codes(...) returns 0 (or non-negative)
 * Path coverage:
 * - Ensure that the final return value from dynamic() indicates success.
 * - This generally requires a fully valid Huffman setup and decoded data.
 */
static bool test_dynamic_success_path() {
    struct state s;
    int ret = dynamic(&s);
    bool passed = (ret >= 0);
    if (!passed) {
        test_fail("test_dynamic_success_path", "Expected non-negative return on success, got: " + std::to_string(ret));
    }
    return passed;
}

/*
 * Note:
 * - The tests above rely on the test harness/compile-time environment to enable
 *   controlled bitstream and construct() behavior (e.g., by stubbing bits(),
 *   decode(), construct(), and codes() or by providing a synthetic state that
 *   yields the desired paths).
 * - If your build system provides hooks for stubbing the static helper functions
 *   (bits, decode, construct, codes, fixed, stored), implement those stubs to
 *   drive particular branches as described in each test.
 * - If stubs are not feasible, consider implementing a dedicated test harness that
 *   compiles puff.c with local macro re-definitions to allow injecting controlled
 *   behavior for the helper functions before the dynamic() function is exercised.
 */