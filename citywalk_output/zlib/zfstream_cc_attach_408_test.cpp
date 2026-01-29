#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <iostream>
#include <zfstream.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>



/*
  Lightweight test harness (non-terminating assertions)
  - They record failures but continue executing to improve coverage.
  - A final summary exit code indicates overall pass/fail.
*/

static int g_test_failures = 0;

// Simple non-terminating assertion helper
#define ASSERT_TRUE(cond, msg)                                      \
    do {                                                             \
        if (!(cond)) {                                               \
            ++g_test_failures;                                       \
            std::cerr << "ASSERT FAIL: " << (msg)                    \
                      << " (line " << __LINE__ << ")" << std::endl; \
        } else {                                                     \
            std::cout << "ASSERT PASS: " << (msg) << std::endl;     \
        }                                                            \
    } while (false)

// Utility to print a short header for test sections
static void print_header(const std::string& title) {
    std::cout << "\n=== " << title << " ===" << std::endl;
}

/*
  Test 1: Attach with an invalid file descriptor should trigger the false branch
  of gzifstream::attach: sb.attach returns false, leading to setting failbit.
*/
static void test_attach_invalid_fd() {
    print_header("Test 1: gzifstream::attach with invalid fd should set failbit");

    gzifstream in;
    int invalid_fd = -1; // obviously invalid
    in.attach(invalid_fd, std::ios_base::in);

    // Expectation: failbit is set
    ASSERT_TRUE(in.fail(), "gzifstream::attach(-1, in) should set failbit (false branch)");
}

/*
  Test 2: Attach with a valid file descriptor should trigger the true branch
  of gzifstream::attach: sb.attach returns true, and the stream should be cleared
  (state becomes good).
*/
static void test_attach_valid_fd() {
    print_header("Test 2: gzifstream::attach with valid fd should clear state (good)");

    // Open a valid read-only file descriptor. /dev/null is always available on POSIX systems.
    int fd = open("/dev/null", O_RDONLY);
    if (fd < 0) {
        // If opening /dev/null somehow fails, mark test as failed but continue.
        ASSERT_TRUE(false, "Could not open /dev/null to obtain a valid fd");
        return;
    }

    gzifstream in;
    in.attach(fd, std::ios_base::in);

    // Expectation: after successful attach, the stream should be in good state (no failbit).
    ASSERT_TRUE(in.good(), "gzifstream::attach(valid_fd, in) should succeed and leave stream good");
    
    // Clean up the file descriptor
    close(fd);
}

int main() {
    std::cout << "Starting tests for gzifstream::attach...\n";

    test_attach_invalid_fd();
    test_attach_valid_fd();

    if (g_test_failures == 0) {
        std::cout << "\nALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "\nTESTS COMPLETED with " << g_test_failures
                  << " failure(s)" << std::endl;
        return 1;
    }
}