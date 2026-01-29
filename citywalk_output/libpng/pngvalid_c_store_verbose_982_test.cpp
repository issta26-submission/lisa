/*
Unit test suite for the focal method store_verbose in the C file pngvalid.c.
This test suite is written in C++11 (no GTest) and targets the behavior of
store_verbose, focusing on the interaction with the optional prefix and the
output to stderr.

Notes on approach:
- We do not attempt to override store_message (to avoid linking conflicts with
  the real project implementation). Instead, we validate the observable behavior:
  when a prefix is provided, it should be emitted to stderr before the rest of
  the message content, and when no prefix is provided, nothing precedes the
  message text.
- We capture stderr using a POSIX pipe to verify the emitted data without
  terminating the test execution.
- We provide explanatory comments for each test to describe intent and coverage.
*/

// Compile with: g++ -std=c++11 -Wall -Wextra -pedantic -D_TEST_PNGVALID_UNIT test_store_verbose.cpp pngvalid.c -o test_store_verbose
// Ensure that the real project headers and sources are accessible to the linker.

#include <unistd.h>
#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <functional>
#include <stdlib.h>
#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Domain-specific type declarations to match the project's API surface
struct png_store;
struct png_struct;
typedef const struct png_struct* png_const_structp;
typedef const char* png_const_charp;
typedef unsigned int png_uint_32;
typedef unsigned char png_byte;

// Forward declarations of the focal function under test (C linkage)
extern "C" void store_verbose(struct png_store* ps, png_const_structp pp,
                              png_const_charp prefix, png_const_charp message);

// We rely on the real store_message from the project; no stub overrides to avoid
// duplicate symbol definitions during linkage.

static int g_test_failures = 0;

// Simple non-terminating assertion macro
#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "FAIL: %s\n", msg); \
            ++g_test_failures; \
        } \
    } while (0)

// Capture stderr output produced by a function call.
// Returns the captured string.
static std::string capture_stderr(const std::function<void(void)>& func)
{
    // Create a pipe to capture stderr
    int fds[2];
    if (pipe(fds) != 0) {
        // Fallback: if pipe fails, execute and return empty output
        func();
        return "";
    }

    // Save current stderr and redirect to the pipe's write end
    int saved_stderr = dup(STDERR_FILENO);
    if (saved_stderr == -1) {
        // Fallback
        close(fds[0]);
        close(fds[1]);
        func();
        return "";
    }

    // Redirect
    if (dup2(fds[1], STDERR_FILENO) == -1) {
        // Fallback
        close(fds[0]);
        close(fds[1]);
        close(saved_stderr);
        func();
        return "";
    }
    // Close write end in this process; it's now dup'd to STDERR
    close(fds[1]);

    // Execute the function that will write to stderr
    func();

    // Flush and read
    fflush(stderr);

    // Read from the pipe
    std::string output;
    char buf[4096];
    ssize_t n;
    while ((n = read(fds[0], buf, sizeof(buf))) > 0) {
        output.append(buf, static_cast<size_t>(n));
    }

    // Restore original stderr
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);
    close(fds[0]);

    return output;
}

// Test 1: When a non-null prefix is provided, the output should begin with the prefix.
// This checks the true-branch of the condition in store_verbose and that
// the prefix is emitted to stderr before the message content.
static void test_store_verbose_prefix_prints_prefix()
{
    const char* prefix = "PFX:";
    const char* message = "hello world";

    auto output = capture_stderr([&]() {
        // ps and pp are not relied upon for this test; pass nulls to keep scope minimal
        store_verbose(nullptr, nullptr, prefix, message);
    });

    // Expect that the very first characters are the provided prefix
    bool starts_with_prefix = (output.size() >= strlen(prefix)) &&
                             (output.compare(0, strlen(prefix), prefix) == 0);

    // Also expect that the output ends with a newline (as the implementation prints a newline)
    bool ends_with_newline = (!output.empty()) && (output.back() == '\n');

    EXPECT_TRUE(starts_with_prefix, "Output should start with the provided prefix when prefix != NULL.");
    EXPECT_TRUE(ends_with_newline, "Output should end with a newline character.");
}

// Test 2: When the prefix is NULL, the output should not start with the previously
// provided prefix (i.e., the true prefix-branch is skipped). This validates the
// false-branch behavior for the prefix predicate.
static void test_store_verbose_no_prefix_skips_prefix()
{
    const char* prefix = nullptr;
    const char* message = "test";

    auto output = capture_stderr([&]() {
        store_verbose(nullptr, nullptr, prefix, message);
    });

    // Ensure the specific prefix is not at the start
    const char* unintended_prefix = "PFX:";
    bool starts_with_unintended = (output.size() >= strlen(unintended_prefix)) &&
                                (output.compare(0, strlen(unintended_prefix), unintended_prefix) == 0);

    bool ends_with_newline = (!output.empty()) && (output.back() == '\n');

    EXPECT_TRUE(!starts_with_unintended, "Output should not start with the unintended prefix when prefix == NULL.");
    EXPECT_TRUE(ends_with_newline, "Output should end with a newline character.");
}

// Simple test runner to execute all tests and report results
static void run_all_tests()
{
    fprintf(stdout, "Running store_verbose unit tests...\n");

    test_store_verbose_prefix_prints_prefix();
    test_store_verbose_no_prefix_skips_prefix();

    if (g_test_failures == 0) {
        fprintf(stdout, "All tests passed.\n");
    } else {
        fprintf(stdout, "%d test(s) failed.\n", g_test_failures);
    }
}

int main(void)
{
    run_all_tests();
    return (g_test_failures == 0) ? 0 : 1;
}