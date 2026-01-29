// Instrumented test suite for the focal function: __cyg_profile_func_enter
// This test suite is designed to be compiled with a C++11 compiler without
// using GTest. It relies on the actual implementation of __cyg_profile_func_enter
// as provided in instrument-functions.c (and its static print_debug dependency).
// The tests capture stdout to verify that the function prints an ENTER event.

#include <string.h>
#include <vector>
#include <bfd.h>
#include <unistd.h>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <cstdlib>
#include <sys/types.h>
#include <cstring>
#include <string>
#include <iostream>



// Expose the focal C function from the instrument file.
// The function uses C linkage, so declare with extern "C".
extern "C" {
    void __cyg_profile_func_enter(void *this_fn, void *call_site);
}

// A small trampoline to invoke the focal function from C++ as a function pointer.
// This helps to reuse the same capturing mechanism for multiple test cases.
static void trampoline_enter(void *this_fn, void *call_site) {
    __cyg_profile_func_enter(this_fn, call_site);
}

// Type alias for a function pointer matching the focal function signature.
using EnterFuncPtr = void (*)(void *, void *);

// Utility: capture stdout output while invoking a function pointer with given args.
// Returns the captured output as a string.
static std::string capture_output(EnterFuncPtr fn, void *a, void *b) {
    // Create a pipe to capture stdout
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, return empty output
        return "";
    }

    // Save the current stdout file descriptor
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        // If duplicating stdout fails, cleanup pipe and return
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to the write end of the pipe
    // Note: We redirect only temporary for the duration of the call
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        // Restore and cleanup on error
        dup2(saved_stdout, fileno(stdout));
        close(saved_stdout);
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Close the pipe's write end in this process since stdout now points to the pipe
    close(pipefd[1]);

    // Invoke the focal function (via trampoline)
    fn(a, b);

    // Ensure all output is flushed
    fflush(stdout);

    // Read the captured data from the pipe
    char buffer[4096];
    std::string out;
    ssize_t n = 0;
    while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
        buffer[n] = '\0';
        out.append(buffer);
    }

    // Restore original stdout
    fflush(stdout);
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);
    close(pipefd[0]);

    return out;
}

// Helper: count occurrences of a substring within a string.
static int count_occurrences(const std::string &s, const std::string &pat) {
    if (pat.empty()) return 0;
    int count = 0;
    std::size_t pos = 0;
    while ((pos = s.find(pat, pos)) != std::string::npos) {
        ++count;
        pos += pat.size();
    }
    return count;
}

// Simple non-terminating assertion mechanism.
// It records failures but does not abort the test sequence.
static int g_failures = 0;
static void ASSERT_TRUE(bool condition, const char *message) {
    if (!condition) {
        std::cerr << "ASSERT FAILED: " << (message ? message : "") << std::endl;
        ++g_failures;
    }
}

// Test 1: Verify ENTER event is printed for non-null this_fn and call_site.
// This ensures the primary code path of __cyg_profile_func_enter executes printing.
static bool test_enter_prints_enter_with_non_null_args() {
    std::string out = capture_output(trampoline_enter, (void*)0x1, (void*)0x2);
    int occurrences = count_occurrences(out, "ENTER"); // Expect at least one ENTER token
    ASSERT_TRUE(occurrences >= 1, "Expected at least one ENTER token for non-null inputs");
    // Also ensure stdout was indeed captured (non-empty output is a good indicator)
    ASSERT_TRUE(!out.empty(), "Expected some output captured for non-null inputs");
    return (g_failures == 0);
}

// Test 2: Verify ENTER event is printed when this_fn is NULL but call_site is non-null.
// This checks robustness against null pointers on this_fn.
static bool test_enter_prints_enter_with_null_this_fn() {
    std::string out = capture_output(trampoline_enter, nullptr, (void*)0x3);
    int occurrences = count_occurrences(out, "ENTER");
    ASSERT_TRUE(occurrences >= 1, "Expected ENTER token when this_fn is NULL but call_site is non-null");
    ASSERT_TRUE(!out.empty(), "Expected some output captured for NULL this_fn");
    return (g_failures == 0);
}

// Test 3: Verify ENTER event is printed when both arguments are NULL.
// This tests the most permissive input case.
static bool test_enter_prints_enter_with_both_null() {
    std::string out = capture_output(trampoline_enter, nullptr, nullptr);
    int occurrences = count_occurrences(out, "ENTER");
    ASSERT_TRUE(occurrences >= 1, "Expected ENTER token when both arguments are NULL");
    ASSERT_TRUE(!out.empty(), "Expected some output captured for NULL inputs");
    return (g_failures == 0);
}

// Test 4: Verify multiple calls produce multiple ENTER outputs (idempotence over calls).
static bool test_multiple_calls_produce_multiple_enters() {
    std::string out1 = capture_output(trampoline_enter, (void*)0x11, (void*)0x22);
    std::string out2 = capture_output(trampoline_enter, (void*)0x33, (void*)0x44);
    int count1 = count_occurrences(out1, "ENTER");
    int count2 = count_occurrences(out2, "ENTER");
    ASSERT_TRUE(count1 >= 1, "First call should produce at least one ENTER");
    ASSERT_TRUE(count2 >= 1, "Second call should produce at least one ENTER");
    // Also verify that outputs are non-empty and independent
    ASSERT_TRUE(!out1.empty(), "First call produced no output");
    ASSERT_TRUE(!out2.empty(), "Second call produced no output");
    return (g_failures == 0);
}

// Simple test runner
int main() {
    // List of tests
    struct Test {
        const char *name;
        bool (*func)();
    };
    std::vector<Test> tests = {
        {"test_enter_with_non_null_args", test_enter_prints_enter_with_non_null_args},
        {"test_enter_with_null_this_fn", test_enter_prints_enter_with_null_this_fn},
        {"test_enter_with_both_null", test_enter_prints_enter_with_both_null},
        {"test_multiple_calls_produce_enters", test_multiple_calls_produce_multiple_enters},
    };

    int total = static_cast<int>(tests.size());
    int passed = 0;

    std::cout << "Running instrument-function tests (no GTest). Total: " << total << std::endl;
    for (const auto &t : tests) {
        // Reset global failure counter for each test to isolate results
        int saved_failures = g_failures;
        g_failures = 0;

        bool result = t.func();
        if (result && g_failures == 0) {
            ++passed;
            std::cout << "[PASSED] " << t.name << std::endl;
        } else {
            // Restore the original failure count for detailed reporting
            int failures_for_this_test = g_failures;
            g_failures = saved_failures; // Do not spillover to next test
            std::cout << "[FAILED] " << t.name << " (failures=" << failures_for_this_test << ")" << std::endl;
        }
        // Reset failure counter to 0 for the next test
        g_failures = 0;
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}