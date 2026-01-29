// Unit test suite for the focal method print_search_results in pngcp.c
// This test is written in C++11 (no GTest) and includes the focal C source
// to exercise the exact function under test.
// The tests capture stdout and perform assertions on the formatted output.

#include <pnglibconf.h>
#include <stdarg.h>
#include <pngcp.c>
#include <string.h>
#include <sys/stat.h>
#include <string>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>


// Include the focal C source to obtain the implementation of print_search_results.
// We wrap in extern "C" to prevent name mangling when compiled as C++.
extern "C" {
}

// Re-declare the minimal struct layout exactly as used by print_search_results.
// This ensures the test supplies a valid display object to the focal function.
// Note: We rely on the fields accessed within print_search_results.
struct display {
    const char *filename;     // used as %s
    long w;                     // used as %ld
    long h;                     // used as %ld
    int bpp;                    // used as %d
    int ct;                     // used as %s through cts() in the actual code
    unsigned long size;         // used as %lu
    unsigned long read_size;    // used as %lu
    unsigned long best_size;    // used as %lu
    const char *best;           // used as %s enclosed in quotes
};

// Forward declare the focal function for type checking in C++
extern "C" void print_search_results(struct display *dp);

// Simple lightweight test harness (no external testing framework)
static int g_passed = 0;
static int g_failed = 0;

// Utility macro to report assertion results
#define TEST_ASSERT(cond, msg)                                        \
    do {                                                              \
        if (cond) {                                                   \
            ++g_passed;                                             \
        } else {                                                      \
            ++g_failed;                                             \
            std::cerr << "Test assertion failed: " << (msg)        \
                      << " (Line " << __LINE__ << ")" << std::endl; \
        }                                                             \
    } while (0)

// Utility to capture stdout around a function call
static std::string capture_stdout_of(void (*func)(void*), void* arg)
{
    // Create a pipe to capture stdout
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        perror("pipe");
        return "";
    }

    // Save original stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        perror("dup");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return "";
    }

    // Redirect stdout to the pipe's write end
    if (dup2(pipe_fd[1], fileno(stdout)) == -1) {
        perror("dup2");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return "";
    }

    // Close the pipe's write end in this process after redirect
    close(pipe_fd[1]);

    // Invoke the function which writes to stdout
    func(arg);

    // Flush and restore stdout
    fflush(stdout);
    if (dup2(saved_stdout, fileno(stdout)) == -1) {
        perror("dup2 restore");
    }
    close(saved_stdout);

    // Read the captured output from the pipe
    std::string output;
    char buffer[4096];
    ssize_t count;
    // Non-blocking read loop
    while ((count = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, (size_t)count);
    }
    close(pipe_fd[0]);

    return output;
}

// Wrapper to adapt test capture to the C-style function pointer used by capture_stdout_of
static void call_print_search_results(void* pv)
{
    // pv is expected to be a pointer to struct display
    struct display* dp = static_cast<struct display*>(pv);
    print_search_results(dp);
}

// Helper to execute a single test case given a prepared display
static std::string run_print_search_results(struct display* dp)
{
    return capture_stdout_of(call_print_search_results, static_cast<void*>(dp));
}

/*
Domain-driven test cases description:
- Test 1: Normal case with representative values.
  - Verifies that print_search_results prints the filename, dimensions, bpp,
    and the numeric fields (size, read_size, best_size) as part of the formatted
    string, and that it ends with "with '<best>'" as per the code.
  - Important: The exact string produced depends on the return value of cts(dp->ct),
    which we do not rely on for exact matching; instead we verify the presence of
    key substrings and numeric values.
  - This ensures the true path (dp->filename != NULL) is exercised and formatting
    is correct.

- Test 2: Another normal case with different values to exercise formatting
  and large numeric values (to exercise %ld and %lu formatting).
  - Checks the same properties as Test 1 but with different numbers and strings.

Notes:
- We intentionally avoid depending on the exact text produced by cts(...) since
  its implementation in the provided skeleton is unclear. Instead, we anchor
  checks on stable parts of the output (filename, dimensions, size fields, and
  the trailing "'best'"). This ensures robust coverage even if cts() returns
  varying strings in different builds.
- We do not attempt to test the assertion dp->filename != NULL directly, since
  that would require intercepting abort() at runtime. The normal path is tested.

*/

// Test 1: Normal path with representative values
static void test_print_search_results_normal_case()
{
    // Prepare a display object with known values
    struct display dp;
    dp.filename = "image1.png";
    dp.w = 640;
    dp.h = 480;
    dp.bpp = 8;
    dp.ct = 1;                 // The actual string from cts() is implementation-defined
    dp.size = 12345;
    dp.read_size = 2345;
    dp.best_size = 3456;
    dp.best = "best1";

    // Run the focal function and capture its stdout
    std::string out = run_print_search_results(&dp);

    // Explanatory comments for the test
    //  - Ensure the output contains the filename and the dimensions in the expected format
    //  - Ensure numeric fields (size, read_size, best_size) appear
    //  - Ensure the trailing best string is present within quotes
    TEST_ASSERT(out.find("image1.png [640 x 480 8 bpp") != std::string::npos,
                "Output should start with filename and dimensions");
    TEST_ASSERT(out.find(", 12345 bytes]") != std::string::npos,
                "Output should contain the correct size in bytes");
    TEST_ASSERT(out.find("2345 -> 3456") != std::string::npos,
                "Output should show read_size and best_size values");
    TEST_ASSERT(out.find("with 'best1'") != std::string::npos,
                "Output should end with the best string in quotes");
}

// Test 2: Another normal case with different values
static void test_print_search_results_another_case()
{
    struct display dp;
    dp.filename = "long_name_image_test_2.png";
    dp.w = 1024;
    dp.h = 768;
    dp.bpp = 24;
    dp.ct = 2;
    dp.size = 987654;
    dp.read_size = 87654;
    dp.best_size = 65432;
    dp.best = "best2";

    std::string out = run_print_search_results(&dp);

    TEST_ASSERT(out.find("long_name_image_test_2.png [1024 x 768 24 bpp") != std::string::npos,
                "Output should contain long filename and correct dimensions");
    TEST_ASSERT(out.find(", 987654 bytes]") != std::string::npos,
                "Output should contain the large size in bytes");
    TEST_ASSERT(out.find("87654 -> 65432") != std::string::npos,
                "Output should show read_size and best_size for large numbers");
    TEST_ASSERT(out.find("with 'best2'") != std::string::npos,
                "Output should end with the new best string in quotes");
}

// Simple test runner
int main(void)
{
    // Run test cases
    test_print_search_results_normal_case();
    test_print_search_results_another_case();

    // Report summary
    std::cout << "Tests run: " << (g_passed + g_failed) << ", "
              << "Passed: " << g_passed << ", "
              << "Failed: " << g_failed << std::endl;

    // Return non-zero if any test failed
    if (g_failed > 0) return 1;
    return 0;
}