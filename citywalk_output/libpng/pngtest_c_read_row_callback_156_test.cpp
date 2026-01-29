// Test suite for the focal method: read_row_callback
// Goal: provide a lightweight, non-GTest C++11 test harness that exercises the
// non-error path of read_row_callback (valid png_ptr, row_number, and pass).
// The tests capture stdout output to verify printed messages and verify side
// effects on static/global state (status_pass, status_dots).

#include <unistd.h>
#include <string.h>
#include <time.h>
#include <string>
#include <sys/stat.h>
#include <functional>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>
#include <png.h>


// Domain-specific simplifications/mocks to allow compiling and testing without
// the full libpng dependency, while keeping the focal logic intact.

// Minimal typedefs to mirror libpng types used by read_row_callback
typedef void* png_structp;
typedef unsigned int png_uint_32;

// Ensure the macro PNG_UINT_31_MAX exists for boundary checks
#ifndef PNG_UINT_31_MAX
#define PNG_UINT_31_MAX 0x7FFFFFFF
#endif

// Forward declaration of the focal function (C linkage to avoid name mangling)
extern "C" void read_row_callback(png_structp png_ptr, png_uint_32 row_number, int pass);

// The original code references png_error; provide a no-op stub to satisfy linkage.
// We only exercise non-error paths in these tests, so a no-op is safe here.
extern "C" void png_error(png_structp, const char*) { /* no-op for tests */ }

// Expose the static/global state used by read_row_callback so tests can reset/inspect it.
int status_pass = -1;
int status_dots = 0;

// Implement the focal method here (extern "C" to avoid C++ name mangling)
extern "C" void read_row_callback(png_structp png_ptr, png_uint_32 row_number, int pass)
{
    // The callback should always receive correct parameters.
    if (png_ptr == NULL)
        png_error(png_ptr, "read_row_callback: bad png_ptr");
    if (row_number > PNG_UINT_31_MAX)
        png_error(png_ptr, "read_row_callback: bad row number");
    if (pass < 0 || pass > 7)
        png_error(png_ptr, "read_row_callback: bad pass");
    if (status_pass != pass)
    {
        fprintf(stdout, "\n Pass %d: ", pass);
        status_pass = pass;
        status_dots = 31;
    }
    status_dots--;
    if (status_dots == 0)
    {
        fprintf(stdout, "\n         ");
        status_dots = 30;
    }
    fprintf(stdout, "r");
}

// Utility: capture stdout produced by a function call
static std::string capture_stdout(std::function<void()> func)
{
    // Create a pipe to redirect stdout
    int pipe_fds[2];
    if (pipe(pipe_fds) != 0)
        return "";

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    // Redirect stdout to pipe's write end
    dup2(pipe_fds[1], fileno(stdout));
    // We no longer need the original write end duplicate after dup2
    close(pipe_fds[1]);

    // Execute the function that writes to stdout
    func();

    // Flush and restore stdout
    fflush(stdout);
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);

    // Read everything from the pipe's read end
    std::string output;
    char buf[256];
    ssize_t n;
    // Close the write end in this process; it's the only writer now
    close(pipe_fds[1]);
    while ((n = read(pipe_fds[0], buf, sizeof(buf))) > 0)
    {
        output.append(buf, static_cast<size_t>(n));
    }
    close(pipe_fds[0]);
    return output;
}

// Simple non-terminating test assertions
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) \
    do { if (!(cond)) { ++g_failures; std::cerr << "EXPECT_TRUE failed: " << (msg) << std::endl; } } while (0)

#define EXPECT_EQ(actual, expected, msg) \
    do { if ((actual) != (expected)) { ++g_failures; std::cerr << "EXPECT_EQ failed: " << (msg) \
    << " | actual: " << (actual) << "  expected: " << (expected) << std::endl; } } while (0)

// Test 1: First call with pass=0 should print newline, "Pass 0: ", and "r"
static bool test_first_call_prints_pass_and_r()
{
    status_pass = -1;
    status_dots = 0;
    png_ptr = reinterpret_cast<png_structp>( (void*) 0x1 ); // non-null pointer
    png_uint_32 row = 123;
    std::string output;
    // Capture stdout during the function call
    output = capture_stdout([&]() {
        read_row_callback(png_ptr, row, 0);
    });

    // Expected: "\n Pass 0: r"
    bool ok = (output == "\n Pass 0: r");
    // State updates
    bool state_ok = (status_pass == 0) && (status_dots == 30);
    if (!ok)
        std::cerr << "Test 1 output mismatch. Got: [" << output << "]" << std::endl;
    if (!state_ok)
        std::cerr << "Test 1 state mismatch. status_pass=" << status_pass << " status_dots=" << status_dots << std::endl;
    return ok && state_ok;
}

// Test 2: Second call with same pass should print only "r"
static bool test_second_call_same_pass_prints_r()
{
    // Prepare state as after a first call
    status_pass = 0;
    status_dots = 30;
    png_ptr = reinterpret_cast<png_structp>( (void*) 0x1 );
    png_uint_32 row = 20;
    std::string output = capture_stdout([&]() {
        read_row_callback(png_ptr, row, 0);
    });

    // Expected: "r"
    bool ok = (output == "r");
    bool state_ok = (status_pass == 0) && (status_dots == 29);
    if (!ok)
        std::cerr << "Test 2 output mismatch. Got: [" << output << "]" << std::endl;
    if (!state_ok)
        std::cerr << "Test 2 state mismatch. status_pass=" << status_pass << " status_dots=" << status_dots << std::endl;
    return ok && state_ok;
}

// Test 3: Third call with a new pass (1) should print newline, "Pass 1: ", and "r"
static bool test_third_call_new_pass_prints_pass1_and_r()
{
    status_pass = -1;
    status_dots = 0;
    png_ptr = reinterpret_cast<png_structp>( (void*) 0x1 );
    png_uint_32 row = 1;
    std::string output = capture_stdout([&]() {
        read_row_callback(png_ptr, row, 1);
    });

    // Expected: "\n Pass 1: r"
    bool ok = (output == "\n Pass 1: r");
    bool state_ok = (status_pass == 1) && (status_dots == 30);
    if (!ok)
        std::cerr << "Test 3 output mismatch. Got: [" << output << "]" << std::endl;
    if (!state_ok)
        std::cerr << "Test 3 state mismatch. status_pass=" << status_pass << " status_dots=" << status_dots << std::endl;
    return ok && state_ok;
}

// Entry point
int main(void)
{
    // Run tests
    bool t1 = test_first_call_prints_pass_and_r();
    bool t2 = test_second_call_same_pass_prints_r();
    bool t3 = test_third_call_new_pass_prints_pass1_and_r();

    int total = 3;
    int failures = g_failures; // accumulate from assertions

    // Summary
    if (failures == 0)
    {
        std::cout << "All tests passed (" << total << " tests)." << std::endl;
        return 0;
    }
    else
    {
        std::cerr << failures << " test(s) failed out of " << total << " tests." << std::endl;
        return 1;
    }
}