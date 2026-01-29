// Test suite for the focal method ReadAllTags in fuzzers.c
// This test harness uses a minimal in-process mocking strategy for the
// cmsGetTagCount, cmsGetTagSignature and cmsReadTag C API functions.
//
// Approach notes:
// - The test includes fuzzers.c inside an extern "C" block to ensure C linkage
//   for the focal function ReadAllTags and to allow linkage with mock C functions.
// - We provide simple global state to drive deterministic behavior for the mocks.
// - We exercise three scenarios: zero tags, all reads succeed, early exit on a NULL read.
// - No GoogleTest is used; a lightweight custom test harness is implemented
//   with non-terminating checks (collects failures but continues execution).

#include <cstdio>
#include <lcms2_plugin.h>
#include <vector>
#include <fuzzers.c>
#include <iostream>
#include <string>
#include <stdint.h>
#include <cstdlib>
#include <sstream>


// Drive in-process mocks for the C API used by ReadAllTags.
// We include the targeted C file so that ReadAllTags gets compiled into this
// translation unit and can be exercised with our mocks.

extern "C" {
}

// Global test state accessible by mock implementations.
static int g_test_current_n = 0;                 // Number of tags to simulate
static int g_read_results[256] = {0};            // Per-index read success (1) or failure (0)
static int g_read_calls = 0;                       // Counts cmsReadTag calls per test

// Helper: reset test state
static void reset_test_state() {
    g_test_current_n = 0;
    for (int i = 0; i < 256; ++i) g_read_results[i] = 0;
    g_read_calls = 0;
}

// Mock implementations of the C API used by ReadAllTags.
// They must match the actual signatures declared in fuzzers.c (via lcms2_plugin.h).

extern "C" {

// cmsGetTagCount(h) -> returns the number of tags to simulate
cmsInt32Number cmsGetTagCount(cmsHPROFILE h) {
    (void)h; // unused in tests; all state is driven by global g_test_current_n
    return (cmsInt32Number)g_test_current_n;
}

// cmsGetTagSignature(h, i) -> provide a signature value for tag i
// For test simplicity, we return i as the signature value.
cmsTagSignature cmsGetTagSignature(cmsHPROFILE h, cmsInt32Number i) {
    (void)h; // signature is a simple index in tests
    return (cmsTagSignature)i;
}

// cmsReadTag(h, sig) -> simulate reading a tag; return non-NULL on success, NULL on failure
void* cmsReadTag(cmsHPROFILE h, cmsTagSignature sig) {
    (void)h; // test harness does not depend on the actual profile
    // Derive tag index from signature
    int idx = (int)sig;
    // bounds check
    if (idx < 0 || idx >= g_test_current_n) {
        ++g_read_calls;
        return NULL;
    }
    // Record read attempt
    ++g_read_calls;
    // Return non-NULL when configured to read successfully
    if (g_read_results[idx]) {
        return (void*)1; // non-NULL value
    }
    return NULL;
}

} // extern "C"

// Lightweight test harness (non-terminating assertions)
// Collect failures but do not abort on first failure.
static std::vector<std::string> g_failures;

static void expect_true(bool cond, const std::string& msg) {
    if (!cond) {
        g_failures.push_back("EXPECT_TRUE failed: " + msg);
        // Do not abort; continue to maximize coverage
    }
}

// Test 1: No tags present; ReadAllTags should perform no reads (no cmsReadTag calls)
static void test_ReadAllTags_zero_tags() {
    reset_test_state();
    g_test_current_n = 0;
    // Ensure the per-tag read results are irrelevant (no tags to read)
    ReadAllTags(nullptr);
    expect_true(g_read_calls == 0, "ReadAllTags with zero tags should not call cmsReadTag");
}

// Test 2: All tags readable; ensure we read all tags and exit normally
static void test_ReadAllTags_all_readable() {
    reset_test_state();
    g_test_current_n = 5; // more than 1 to exercise loop
    for (int i = 0; i < g_test_current_n; ++i) g_read_results[i] = 1; // all succeed

    ReadAllTags(nullptr);

    // We expect cmsReadTag to have been called exactly n times
    int expected_calls = g_test_current_n;
    std::ostringstream oss;
    oss << "ReadAllTags should read all tags when all reads succeed (expected "
        << expected_calls << " calls, got " << g_read_calls << ")";
    expect_true(g_read_calls == expected_calls, oss.str());
}

// Test 3: Early exit on NULL cmsReadTag; ensure we stop after first failure
static void test_ReadAllTags_early_exit_on_failure() {
    reset_test_state();
    g_test_current_n = 4;
    // Simulate: tag 0 read OK, tag 1 read fails (NULL), subsequent reads should not occur
    g_read_results[0] = 1;
    g_read_results[1] = 0;
    g_read_results[2] = 1;
    g_read_results[3] = 1; // should be ignored if early exit works

    ReadAllTags(nullptr);

    // We expect reads for indices 0 and 1 only -> 2 calls
    int expected_calls = 2;
    std::ostringstream oss;
    oss << "ReadAllTags should stop on first NULL tag (expected "
        << expected_calls << " calls, got " << g_read_calls << ")";
    expect_true(g_read_calls == expected_calls, oss.str());
}

// Entry point for tests
int main() {
    // Run tests
    test_ReadAllTags_zero_tags();
    test_ReadAllTags_all_readable();
    test_ReadAllTags_early_exit_on_failure();

    // Report results
    if (g_failures.empty()) {
        std::cout << "All tests passed for ReadAllTags." << std::endl;
        return 0;
    } else {
        std::cout << g_failures.size() << " test failure(s) detected for ReadAllTags:\n";
        for (const auto& f : g_failures) {
            std::cout << " - " << f << std::endl;
        }
        return 1;
    }
}