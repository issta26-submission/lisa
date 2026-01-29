// ================================================================
// Test Suite for readpng_version_info (focal method)
// Target: C++11 compliant unit tests without GoogleTest
// Notes:
// - The focal method prints a message to stderr indicating absence of libpng/zlib/PBMPLUS.
// - Tests capture and verify the exact stderr output without terminating the program.
// - Tests are designed to be executable by a main() function calling test functions.
// - This test suite assumes POSIX availability for pipe-based stderr capture.
// ================================================================

#include <unistd.h>
#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <readpng.h>
#include <stdio.h>
#include <cstring>



// The focal function is defined in the production code as:
// void readpng_version_info();
// We declare it with C linkage to ensure proper linkage when compiled with C++.
// The production may reside in readppm.c; ensure it's linked during compilation.
extern "C" void readpng_version_info();

// ------------------------
// Simple test harness
// ------------------------
static int g_test_failures = 0;

// Utility: non-terminating assertion macro (informative, does not exit)
#define TEST_ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[TEST FAILED] " << (msg) \
                      << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            ++g_test_failures; \
        } else { \
            std::cout << "[TEST PASSED] " << (msg) \
                      << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        } \
    } while (0)

// Domain Knowledge alignment note (Candidate Keywords):
// - readpng_version_info, fprintf(stderr), "Compiled without libpng, zlib or PBMPLUS/NetPBM."
// - Dependency functions from readpng.h: readpng_init, readpng_get_bgcolor, readpng_get_image, readpng_cleanup
// - Handling C linkage, redirection of stderr, and test invocation from main

// ------------------------
// Helper: Capture stderr output while executing a function
// Approach: Redirect stderr to a pipe, run the function, read all captured data, then restore stderr.
// Requires POSIX (pipe, dup, dup2, read, close).
// ------------------------
static std::string capture_stderr(const std::function<void()>& func) {
    // Create a pipe to capture stderr
    int pipefds[2];
    if (pipe(pipefds) != 0) {
        // Fallback: if pipe fails, execute function and return empty capture
        func();
        return "";
    }

    // Save current stderr
    int saved_stderr = dup(fileno(stderr));
    if (saved_stderr == -1) {
        // Fallback
        func();
        close(pipefds[0]);
        close(pipefds[1]);
        return "";
    }

    // Redirect stderr to pipe's write end
    fflush(stderr);
    if (dup2(pipefds[1], fileno(stderr)) == -1) {
        // Fallback
        func();
        // Restore
        dup2(saved_stderr, fileno(stderr));
        close(saved_stderr);
        close(pipefds[0]);
        close(pipefds[1]);
        return "";
    }

    // Close the write end in this process since it's now duplicated to stderr
    close(pipefds[1]);

    // Execute the function that will write to stderr
    func();

    // Flush and read from the pipe
    fflush(stderr);

    // Read all data from pipe
    std::string result;
    char buffer[4096];
    ssize_t r;
    while ((r = read(pipefds[0], buffer, sizeof(buffer))) > 0) {
        result.append(buffer, static_cast<size_t>(r));
    }

    // Restore original stderr
    dup2(saved_stderr, fileno(stderr));
    close(saved_stderr);
    close(pipefds[0]);

    return result;
}

// ------------------------
// Test 1: Single call should emit the expected stderr message
// - Verifies that readpng_version_info prints the exact informative string.
// ------------------------
static bool test_readpng_version_info_single_call_outputs_message() {
    const std::string expected = "Compiled without libpng, zlib or PBMPLUS/NetPBM.";

    auto output = capture_stderr([]() {
        readpng_version_info();
    });

    // Debug print (optional)
    // std::cerr << "Captured stderr: [" << output << "]\n";

    bool found = (output.find(expected) != std::string::npos);
    TEST_ASSERT(found, "readpng_version_info prints expected message on single call");
    return found;
}

// ------------------------
// Test 2: Multiple calls should emit the message more than once
// - Verifies idempotency/consecutive calls produce repeated messages.
// ------------------------
static bool test_readpng_version_info_multiple_calls_outputs_multiple_messages() {
    const std::string expected = "Compiled without libpng, zlib or PBMPLUS/NetPBM.";

    auto output = capture_stderr([]() {
        // Call the focal function twice to ensure multiple emissions
        readpng_version_info();
        readpng_version_info();
    });

    // Count occurrences of the expected message
    size_t pos = 0;
    int count = 0;
    while ((pos = output.find(expected, pos)) != std::string::npos) {
        ++count;
        pos += expected.size();
    }

    bool enough = (count >= 2);
    TEST_ASSERT(enough, "readpng_version_info can be called multiple times (>=2) and prints message each time");
    return enough;
}

// ------------------------
// Main: Entry point for tests (no external test framework)
// - Calls test functions from main as required by the domain knowledge.
// ------------------------
int main() {
    std::cout << "Starting unit tests for readpng_version_info..." << std::endl;

    bool t1 = test_readpng_version_info_single_call_outputs_message();
    bool t2 = test_readpng_version_info_multiple_calls_outputs_multiple_messages();

    int failures = g_test_failures;
    if (failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}