/*
Unit Test Suite for PrintProfileInformation (vprf.c)

Overview:
- This test suite is written in C++11, uses a small handcrafted test harness (no GTest).
- It targets the focal method: PrintProfileInformation(cmsHPROFILE hInput).
- Assumptions based on the provided source:
  - If hInput is NULL, the function prints an error to stderr:
      "*Wrong or corrupted profile*\n"
  - If hInput is non-NULL, it calls PrintInfo for a few info types and prints a trailing newline.
  - A global (extern) Verbose controls extra calls to PrintColorantTable when Verbose > 2 (we cannot observe internal calls due to the provided stubs, but we still test observable behavior).
  - PrintInfo and PrintColorantTable are present (likely stubbed in vprf.c); we rely on the actual compiled vprf.c for linkage. We intercept only observable stdout/stderr to verify behavior.
- Testing approach:
  - Test 1: NULL input -> verify correct error message on stderr and no stdout output.
  - Test 2: Non-NULL input -> verify a single newline printed to stdout and no stderr output.
  - Tests do not rely on internal PrintInfo/PrintColorantTable outputs (they are effectively stubs in the provided snippet); we assert observable behavior only.
- How to compile:
  - g++ -std=c++11 test_print_profile_info.cpp vprf.c -o test_print_profile_info
  - Ensure vprf.c provides the expected types and externs used by PrintProfileInformation (cmsHPROFILE, Verbose, etc.)
*/

// NOTE: Provide minimal type aliases to be compatible with the signature used by vprf.c.
// These are only for the test harness to compile and link with vprf.c.
#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <utils.h>


// Forward declarations to compile against the focal function in vprf.c
extern "C" {

// The function under test
typedef void* cmsHPROFILE;
void PrintProfileInformation(cmsHPROFILE hInput);

// The rest are not needed to declare for this test harness since we rely on vprf.c'
// actual implementations for linkage. We declare the Verbose variable here to
// allow tests to adjust its value if it is defined with external linkage.
extern int Verbose;
}

// Simple test harness state
static int g_failures = 0;

// Convenience macros for non-terminating checks (do not abort the test on failure)
#define TEST_EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "TEST FAILED: " << (msg) << std::endl; \
            ++g_failures; \
        } \
    } while (0)

#define TEST_PRINT_MSG(msg) \
    do { std::cout << (msg) << std::endl; } while (0)

// Utility to capture stdout and stderr produced by calling a function.
// Uses tmpfile-based redirection to avoid platform-specific complexities.
class OutputCapture {
public:
    OutputCapture() : stdout_tmp(nullptr), stderr_tmp(nullptr), saved_stdout(-1), saved_stderr(-1) {}

    void start() {
        fflush(stdout);
        fflush(stderr);

        // Create temporary in-memory files
        stdout_tmp = tmpfile();
        stderr_tmp = tmpfile();
        if (!stdout_tmp || !stderr_tmp) {
            std::cerr << "Failed to create temporary files for output capture" << std::endl;
            return;
        }

        // Save current stdout/stderr file descriptors
        saved_stdout = dup(fileno(stdout));
        saved_stderr = dup(fileno(stderr));

        // Redirect stdout/stderr to the temporary files
        int stdout_fd = fileno(stdout_tmp);
        int stderr_fd = fileno(stderr_tmp);
        if (dup2(stdout_fd, fileno(stdout)) == -1 || dup2(stderr_fd, fileno(stderr)) == -1) {
            std::cerr << "Failed to redirect stdout/stderr" << std::endl;
        }
    }

    void stop() {
        fflush(stdout);
        fflush(stderr);

        // Restore original stdout/stderr
        if (saved_stdout != -1) {
            dup2(saved_stdout, fileno(stdout));
        }
        if (saved_stderr != -1) {
            dup2(saved_stderr, fileno(stderr));
        }
        // Close temporary file descriptors
        if (saved_stdout != -1) close(saved_stdout);
        if (saved_stderr != -1) close(saved_stderr);

        // Read captured content from temporary files
        stdout_content.clear();
        stderr_content.clear();

        if (stdout_tmp) {
            rewind(stdout_tmp);
            char buf[4096];
            size_t n;
            while ((n = fread(buf, 1, sizeof(buf), stdout_tmp)) > 0) {
                stdout_content.append(buf, n);
            }
            fclose(stdout_tmp);
            stdout_tmp = nullptr;
        }

        if (stderr_tmp) {
            rewind(stderr_tmp);
            char buf[4096];
            size_t n;
            while ((n = fread(buf, 1, sizeof(buf), stderr_tmp)) > 0) {
                stderr_content.append(buf, n);
            }
            fclose(stderr_tmp);
            stderr_tmp = nullptr;
        }
        // Flush to ensure content is ready
        fflush(stdout);
        fflush(stderr);
    }

    const std::string& getStdout() const { return stdout_content; }
    const std::string& getStderr() const { return stderr_content; }

private:
    FILE* stdout_tmp;
    FILE* stderr_tmp;
    int saved_stdout;
    int saved_stderr;
    std::string stdout_content;
    std::string stderr_content;
};

// Test 1: NULL input should print specific error to stderr and not touch stdout
void test_PrintProfileInformation_null_input() {
    OutputCapture cap;
    cap.start();
    // NULL input case
    PrintProfileInformation(nullptr);
    cap.stop();

    const std::string& errOut = cap.getStderr();
    const std::string& stdOut = cap.getStdout();

    // Expectation: error message on stderr, and no stdout content
    TEST_EXPECT_TRUE(errOut.find("*Wrong or corrupted profile*") != std::string::npos,
                     "Expected error message on stderr for NULL input");
    TEST_EXPECT_TRUE(stdOut.empty(), "Expected no stdout content for NULL input");
}

// Test 2: Non-NULL input should produce a trailing newline on stdout and no stderr
void test_PrintProfileInformation_non_null_input() {
    OutputCapture cap;
    cap.start();

    // Provide a non-null pointer (the actual value is not used by the test harness scripts)
    cmsHPROFILE fakeHandle = reinterpret_cast<cmsHPROFILE>(0x1ull);
    PrintProfileInformation(fakeHandle);

    cap.stop();

    const std::string& errOut = cap.getStderr();
    const std::string& stdOut = cap.getStdout();

    // Expectation: no error messages, and a trailing newline printed to stdout
    TEST_EXPECT_TRUE(errOut.empty(), "Expected no stderr output for non-null input");
    TEST_EXPECT_TRUE(stdOut == "\n", "Expected a single newline in stdout for non-null input");
}

// Main test runner
int main() {
    TEST_PRINT_MSG("Starting unit tests for PrintProfileInformation...");

    // Run tests
    test_PrintProfileInformation_null_input();
    test_PrintProfileInformation_non_null_input();

    if (g_failures == 0) {
        std::cout << "All tests PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " test(s) FAILED" << std::endl;
        return g_failures;
    }
}