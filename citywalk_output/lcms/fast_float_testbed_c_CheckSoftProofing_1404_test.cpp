// Lightweight C++11 test harness for CheckSoftProofing (no GTest, no Google Mock)
// This test suite invokes the focal method CheckSoftProofing() from the provided C file
// and validates that it executes to completion and emits expected trace output.
// It uses a minimal in-house assertion framework and stdout capture to maximize coverage
// while remaining independent of the original testing framework.
//
// Notes:
// - Extern "C" is used to call the C function CheckSoftProofing.
// - The test validates that the function prints an "Ok" message, indicating successful completion.
// - The test harness is designed to be compiled together with fast_float_testbed.c
//   (which contains the CheckSoftProofing implementation) under a C++11 compiler.
//
// Usage (conceptual):
//   g++ -std=c++11 -I. test_check_soft_proofing.cpp fast_float_testbed.c -o test_check_soft_proofing
//   ./test_check_soft_proofing
//
// The test relies on available ICC profiles and LittleCMS in the environment (as used by the focal code).

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>


extern "C" void CheckSoftProofing(void); // Focal method under test (C linkage)

// Lightweight assertion helpers (no exceptions, no GTest)
// Usage: ASSERT_TRUE(condition, "message")
static int g_total = 0;
static int g_pass = 0;
static std::vector<std::string> g_failures;

#define ASSERT_TRUE(cond, msg) do { \
    g_total++; \
    if (cond) { g_pass++; } \
    else { \
        std::string s = std::string("Assertion failed: ") + (msg); \
        g_failures.push_back(s); \
    } \
} while(0)

#define ASSERT_STR_CONTAINS(str, sub, msg) do { \
    g_total++; \
    if ((str).find(sub) != std::string::npos) { g_pass++; } \
    else { \
        std::string s = std::string("Assertion failed: ") + (msg) + \
                        ". Expected to contain: \"" + (sub) + "\""; \
        g_failures.push_back(s); \
    } \
} while(0)

#define TEST_NAME(name) \
    static void name(); \
    static struct TestRegister_##name { \
        TestRegister_##name() { TestRegistry::instance().registerTest(#name, &name); } \
    } _reg_##name; \
    static void name()

// Simple test registry to run tests from main
class TestRegistry {
public:
    typedef void (*TestFunc)();
    static TestRegistry& instance() {
        static TestRegistry inst;
        return inst;
    }
    void registerTest(const char* name, TestFunc f) {
        tests.emplace_back(name, f);
    }
    const std::vector<std::pair<std::string, TestFunc>>& getTests() const {
        return tests;
    }
private:
    std::vector<std::pair<std::string, TestFunc>> tests;
    TestRegistry() = default;
};

// Helper: capture stdout output while running a function
static std::string captureOutput(void (*fn)(void)) {
    // Create a pipe to capture stdout
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // Fallback: return empty on failure
        return std::string();
    }

    // Save current stdout descriptor
    int saved_stdout = dup(fileno(stdout));
    // Redirect stdout to pipe's write end
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        // Restore and return
        close(pipefd[0]);
        close(pipefd[1]);
        if (saved_stdout != -1) close(saved_stdout);
        return std::string();
    }

    // Run the function (catching any exceptions to avoid leaking through)
    try {
        fn();
    } catch (...) {
        // Ensure stdout is restored even on exceptions
        fflush(stdout);
        dup2(saved_stdout, fileno(stdout));
        close(pipefd[1]);
        if (saved_stdout != -1) close(saved_stdout);
        // Return captured output up to this point
        // Fall through to read but there should be nothing more; still return what we captured
    }

    fflush(stdout);
    // Restore stdout
    if (dup2(saved_stdout, fileno(stdout)) == -1) {
        // Ignore errors; attempt to continue
    }
    close(pipefd[1]);
    // Read from pipe
    std::string output;
    char buffer[4096];
    ssize_t n = 0;
    // Close write end before reading is safe; pipefd[0] remains open
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    if (saved_stdout != -1) close(saved_stdout);
    return output;
}

// Wrapper to call CheckSoftProofing from a function pointer
static void CheckSoftProofing_Wrapper() {
    CheckSoftProofing();
}

// Test 1: Verify that CheckSoftProofing runs to completion and prints "Ok"
TEST_NAME(test_CheckSoftProofing_PrintsOk) {
    // Capture output during the call
    std::string out = captureOutput(CheckSoftProofing_Wrapper);

    // Basic assertions: the function should print "Ok" when complete
    ASSERT_STR_CONTAINS(out, "Ok", "CheckSoftProofing should print 'Ok' on successful completion");

    // Also verify that the initial trace message indicates the test started
    ASSERT_STR_CONTAINS(out, "Checking soft proofing and gamut check", "Expected start trace not found in output");
}

// Test 2: Run the function twice to ensure no crash or state leakage between invocations
TEST_NAME(test_CheckSoftProofing_RepeatedInvocation) {
    // First call
    std::string out1 = captureOutput(CheckSoftProofing_Wrapper);
    ASSERT_STR_CONTAINS(out1, "Ok", "First invocation should print 'Ok'");

    // Second call
    std::string out2 = captureOutput(CheckSoftProofing_Wrapper);
    ASSERT_STR_CONTAINS(out2, "Ok", "Second invocation should print 'Ok' and be repeatable");
}

// Global test runner
static void RunAllTests() {
    // Access registry and execute tests
    const auto& tests = TestRegistry::instance().getTests();
    for (const auto& t : tests) {
        // Reset per-test counters
        g_total = 0;
        g_pass = 0;
        g_failures.clear();

        // Run test
        // Each test is a function with no args
        t.second();

        // Report per-test result
        std::cout << "Test: " << t.first << " - ";
        if (g_total == g_pass && g_total > 0) {
            std::cout << "PASS" << std::endl;
        } else {
            std::cout << "FAIL" << std::endl;
            // Print failures
            for (const auto& s : g_failures) {
                std::cerr << "  " << s << std::endl;
            }
        }
    }

    // Summarize (total across all tests)
    int total = g_total;
    int passed = g_pass;
    std::cout << "\nOverall: " << passed << " / " << (total == 0 ? 1 : total) << " tests passed." << std::endl;
}

// Ensure tests are registered before main starts
int main() {
    // Print a short banner
    std::cout << "Running CheckSoftProofing test suite (no GTest)..." << std::endl;
    RunAllTests();
    // Return non-zero if any test failed
    int totalFailures = g_total - g_pass;
    return (totalFailures > 0) ? 1 : 0;
}

// Instantiate test registrations (global initialization triggers registration)
TEST_NAME(dummy_registrar) {
    // Intentionally empty: this test is not used; the real tests are registered above
}
```