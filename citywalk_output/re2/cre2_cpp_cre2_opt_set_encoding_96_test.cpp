// Minimal C++11 test suite for cre2_opt_set_encoding in cre2.cpp
// This test harness avoids any external testing framework (no GTest).
// It uses a lightweight, non-terminating assertion mechanism to maximize coverage.

#include <cre2.h>
#include <unistd.h>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <sys/types.h>
#include <vector>
#include <sys/wait.h>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include CRE2 types and APIs

// Platform-specific includes for fork-based testing of exit path
#if defined(__linux__) || defined(__APPLE__)
#endif

// Global collection of test failures (non-terminating)
static std::vector<std::string> g_failures;

// Lightweight non-terminating assertion
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        g_failures.push_back(std::string("CHECK FAILED: ") + (msg)); \
    } \
} while(0)

// Helper to report summary of tests
static void report_results() {
    if(g_failures.empty()) {
        std::cout << "[TEST] All tests passed.\n";
    } else {
        std::cout << "[TEST] " << g_failures.size() << " failure(s) detected:\n";
        for(const auto& f : g_failures) {
            std::cout << "       " << f << "\n";
        }
    }
}

// Test 1: Ensure cre2_opt_set_encoding correctly sets EncodingUTF8
static void test_cre2_opt_set_encoding_utf8() {
    // Create a new options object
    cre2_options_t *opt = cre2_opt_new();
    CHECK(opt != nullptr, "cre2_opt_new() should return a non-null pointer");

    // Set encoding to UTF8
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // Verify encoding via the getter
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    CHECK(enc == CRE2_UTF8, "Encoding should be CRE2_UTF8 after setting to CRE2_UTF8");

    // Cleanup
    cre2_opt_delete(opt);
}

// Test 2: Ensure cre2_opt_set_encoding correctly sets EncodingLatin1
static void test_cre2_opt_set_encoding_latin1() {
    // Create a new options object
    cre2_options_t *opt = cre2_opt_new();
    CHECK(opt != nullptr, "cre2_opt_new() should return a non-null pointer");

    // Set encoding to Latin1
    cre2_opt_set_encoding(opt, CRE2_Latin1);

    // Verify encoding via the getter
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    CHECK(enc == CRE2_Latin1, "Encoding should be CRE2_Latin1 after setting to CRE2_Latin1");

    // Cleanup
    cre2_opt_delete(opt);
}

// Test 3: Invalid encoding should cause process exit (default branch of switch)
// This test is executed in a separate process to capture exit status without aborting the whole test run.
static void test_cre2_opt_set_encoding_invalid_exits() {
#if defined(__linux__) || defined(__APPLE__)
    pid_t pid = fork();
    if(pid == -1) {
        g_failures.push_back("FORK FAILED: could not create child process for invalid encoding test");
        return;
    }
    if(pid == 0) {
        // Child process: call with an invalid encoding value
        cre2_options_t *opt = cre2_opt_new();
        // Intentionally cast an invalid value to cre2_encoding_t
        cre2_opt_set_encoding(opt, (cre2_encoding_t)9999);
        // If we return, clean up to avoid leaks in child
        cre2_opt_delete(opt);
        // If for some reason we didn't exit, exit with success to indicate unexpected path
        _exit(0);
    } else {
        // Parent: wait for child and verify it exited with non-zero status
        int status = 0;
        if(waitpid(pid, &status, 0) == -1) {
            g_failures.push_back("waitpid FAILED: could not retrieve child status for invalid encoding test");
            return;
        }
        // Child should have exited due to CRE2 internal error path (EXIT_FAILURE)
        bool child_exited_normally = WIFEXITED(status);
        int exit_code = child_exited_normally ? WEXITSTATUS(status) : -1;
        CHECK(child_exited_normally, "Child process did not exit normally for invalid encoding test");
        CHECK(exit_code != 0, "Child process should exit with non-zero status for invalid encoding");
    }
#else
    // If not POSIX, skip this test gracefully.
    // The non-POSIX platforms might not support fork/exit capture.
    // We still exercise the positive paths in tests 1 and 2.
    (void)g_failures; // silence unused warning if needed
#endif
}

// Entry point: run all tests and report results
int main() {
    // Run tests (Step 2: Unit Test Generation)
    test_cre2_opt_set_encoding_utf8();
    test_cre2_opt_set_encoding_latin1();
    test_cre2_opt_set_encoding_invalid_exits(); // may be skipped on non-POSIX

    // Step 3: Test Case Refinement
    // (Commentary: We could extend tests to cover default path and repeated sets, but
    //  the essential branches of cre2_opt_set_encoding are covered by UTF8, Latin1,
    //  and the invalid encoding exit path.)

    // Report and return
    report_results();
    // If there were any failures, return non-zero; otherwise zero.
    return g_failures.empty() ? 0 : 1;
}