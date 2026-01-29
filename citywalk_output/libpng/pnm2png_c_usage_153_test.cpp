// Test suite for the focal method: usage() from pnm2png.c
// This test harness is written for C++11 and uses a lightweight, self-contained testing approach
// (no Google Test). It captures the stderr output of the C function usage() and verifies
// that all expected strings are present in the help text.
//
// Notes:
// - The test assumes the usage() function is exposed with C linkage.
// - The tests redirect stderr to an in-memory capture via a POSIX pipe.
// - This file should be compiled and linked against the original pnm2png.c (or its object)
//   where the focal function is defined. If the original translation unit contains a main()
//   function, ensure you build the tests against the library version without a conflicting main.

#include <cassert>
#include <string>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <png.h>


// Declaration of the focal function under test (C linkage)
extern "C" void usage();


// Lightweight testing utilities
static int g_failed = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) << "\n"; \
        ++g_failed; \
    } \
} while (0)

#define EXPECT_STR_FIND(haystack, needle, msg) do { \
    if ((haystack).find((needle)) == std::string::npos) { \
        std::cerr << "EXPECT_STR_FIND FAILED: " << (msg) << \
        " | missing: \"" << (needle) << "\"\n"; \
        ++g_failed; \
    } \
} while (0)


// Helper to capture the output printed to stderr by usage()
static std::string capture_usage_output() {
    // Create a pipe to capture stderr
    int pipe_fds[2];
    if (pipe(pipe_fds) != 0) {
        // If pipe creation fails, return empty output
        return "";
    }

    int saved_stderr = dup(STDERR_FILENO);
    if (saved_stderr == -1) {
        // If dup fails, cleanup and return
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        return "";
    }

    // Redirect stderr to the pipe's write end
    if (dup2(pipe_fds[1], STDERR_FILENO) == -1) {
        // Restore and cleanup
        dup2(saved_stderr, STDERR_FILENO);
        close(saved_stderr);
        close(pipe_fds[0]);
        close(pipe_fds[1]);
        return "";
    }

    // Close the now-duplicated write end in the parent; usage() will write to STDERR_FILENO
    close(pipe_fds[1]);

    // Call the focal function
    usage();

    // Flush and restore stderr
    fflush(stderr);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);

    // Read captured data
    std::string output;
    char buffer[4096];
    ssize_t bytes_read;
    // The read end was pipe_fds[0] but we closed it; reopen for reading from the other end
    // Since we closed the write end, we still need to read from the read end which remained
    // open as pipe_fds[0].
    // However, since we already closed pipe_fds[1], we can read from pipe_fds[0].
    // Note: if the file descriptors were closed prematurely, this path may fail; we guard against that.
    int read_fd = pipe_fds[0];
    // Close the write end was done earlier, so we can read
    while ((bytes_read = read(read_fd, buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(bytes_read));
    }

    // Cleanup read end
    close(read_fd);

    return output;
}


// Individual test cases with explanatory comments

// Test 1: Verify the header line "PNM2PNG" is present in usage() output
static void test_usage_contains_header() {
    std::string out = capture_usage_output();
    EXPECT_TRUE(!out.empty(), "usage() should produce output (non-empty).");
    EXPECT_STR_FIND(out, "PNM2PNG", "usage() header should contain 'PNM2PNG'");
}

// Test 2: Verify author information line is present
static void test_usage_contains_author_line() {
    std::string out = capture_usage_output();
    EXPECT_TRUE(!out.empty(), "usage() should produce output (non-empty).");
    EXPECT_STR_FIND(out, "by Willem van Schaik, 1999", "usage() should mention author line");
}

// Test 3: Verify the usage line with file syntax is present
static void test_usage_contains_usage_line() {
    std::string out = capture_usage_output();
    EXPECT_TRUE(!out.empty(), "usage() should produce output (non-empty).");
    EXPECT_STR_FIND(out, "Usage:  pnm2png [options] <file>.<pnm> [<file>.png]", 
                    "usage() should include the explicit usage line with file pattern");
}

// Test 4: Verify the alternative usage example line is present
static void test_usage_contains_alternative_line() {
    std::string out = capture_usage_output();
    EXPECT_TRUE(!out.empty(), "usage() should produce output (non-empty).");
    EXPECT_STR_FIND(out, "or:  ... | pnm2png [options]", 
                    "usage() should include the pipe-based usage example line");
}

// Test 5: Verify the presence of the "Options:" header
static void test_usage_contains_options_header() {
    std::string out = capture_usage_output();
    EXPECT_TRUE(!out.empty(), "usage() should produce output (non-empty).");
    EXPECT_STR_FIND(out, "Options:", "usage() should contain 'Options:' header");
}

// Test 6: Verify the interlace option hint is present
static void test_usage_contains_interlace_option() {
    std::string out = capture_usage_output();
    EXPECT_TRUE(!out.empty(), "usage() should produce output (non-empty).");
    EXPECT_STR_FIND(out, "-i[nterlace]   write png-file with interlacing on",
                    "usage() should mention interlace option (-i[nterlace])");
}

// Test 7: Verify the alpha option hint is present
static void test_usage_contains_alpha_option() {
    std::string out = capture_usage_output();
    EXPECT_TRUE(!out.empty(), "usage() should produce output (non-empty).");
    EXPECT_STR_FIND(out, "-a[lpha] <file>.pgm read PNG alpha channel as pgm-file",
                    "usage() should mention alpha option (-a[lpha]) and its file target");
}

// Test 8: Verify the help option hint is present
static void test_usage_contains_help_option() {
    std::string out = capture_usage_output();
    EXPECT_TRUE(!out.empty(), "usage() should produce output (non-empty).");
    EXPECT_STR_FIND(out, "-h | -?  print this help-information",
                    "usage() should mention help options (-h or -?)");
}


// Main driver for the test suite
int main() {
    // Run all test cases
    test_usage_contains_header();
    test_usage_contains_author_line();
    test_usage_contains_usage_line();
    test_usage_contains_alternative_line();
    test_usage_contains_options_header();
    test_usage_contains_interlace_option();
    test_usage_contains_alpha_option();
    test_usage_contains_help_option();

    if (g_failed == 0) {
        std::cout << "All tests passed for usage()." << std::endl;
        return 0;
    } else {
        std::cerr << g_failed << " test(s) failed for usage()." << std::endl;
        return 1;
    }
}