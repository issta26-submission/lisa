// Lightweight C++11 test suite for gzdopen(int fd, const char *mode) from gzlib.c
// This test avoids external testing frameworks (no GTest) and uses a simple
// non-terminating assertion style to maximize code execution coverage.
// It relies on the zlib/gzlib implementation being available (gzFile, gzdopen, etc. are
// provided by the environment's zlib installation).

#include <fcntl.h>
#include <string>
#include <functional>
#include <iostream>
#include <vector>
#include <gzguts.h>
#include <zlib.h>
#include <unistd.h>


// Simple non-terminating assertion framework
static int total_checks = 0;
static int passed_checks = 0;

#define EXPECT_TRUE(cond, desc)                                      \
    do {                                                               \
        ++total_checks;                                                \
        if (static_cast<bool>(cond)) {                                 \
            ++passed_checks;                                           \
        } else {                                                       \
            std::cerr << "[FAIL] " << (desc) << std::endl;            \
        }                                                              \
    } while (0)

/*
  Step 2 Context:
  - The focal function under test is gzdopen(fd, mode) defined in gzlib.c.
  - It allocates a temporary path string, uses snprintf to format "<fd:%d>" with fd,
    calls gz_open(path, fd, mode), frees the path, and returns the gzFile handle.
  - Branch coverage targets:
      a) fd == -1 -> returns NULL (early exit)
      b) path allocation failure -> returns NULL (not directly observable in tests)
      c) snprintf path construction path -> used when NO_snprintf is not defined
      d) gz_open(...) -> behavior depends on the underlying gz_open implementation
  - We test the observable branches:
      - fd == -1 yields NULL
      - fd >= 0 yields a non-NULL gzFile (basic assertion about success path)
  - The tests assume the environment provides a functioning zlib/gzlib implementation
    compatible with gzdopen, and that gz_open is capable of wiring through the provided fd.
*/

/*
  Test 1: gzdopen should return NULL when fd is -1.
  Rationale: This exercises the early return branch in gzdopen.
*/
void test_gzdopen_negative_one_fd_returns_null() {
    // Given an invalid file descriptor
    int fd = -1;
    // When we attempt to attach using gzdopen
    gzFile gz = gzdopen(fd, "rb");
    // Then we expect NULL as the function cannot attach to an invalid FD
    EXPECT_TRUE(gz == NULL, "gzdopen(-1, \"rb\") should return NULL");
}

/*
  Test 2: gzdopen should return a non-NULL gzFile for a valid, open FD.
  Rationale: This exercises the success path where path is allocated, formatted,
  and gz_open is invoked. We avoid depending on actual data in the FD by simply
  ensuring that a valid, open FD yields a non-NULL handle.
  Setup uses a simple pipe as a readily available valid FD source.
*/
void test_gzdopen_valid_fd_returns_non_null() {
    int pipefds[2];
    // Create a pipe; the read end (pipefds[0]) is a valid FD suitable for gzdopen
    if (pipe(pipefds) != 0) {
        std::cerr << "[WARN] Couldn't create pipe for test_gzdopen_valid_fd_returns_non_null(); skipping test." << std::endl;
        return;
    }

    int read_fd = pipefds[0];
    // Mode string chosen to be typical for reading gzip data; actual content is not read in this test
    const char *mode = "rb";

    // When we attach to the read end of the pipe
    gzFile gz = gzdopen(read_fd, mode);

    // Then we expect a non-NULL gzFile handle (the actual data stream would be used later)
    EXPECT_TRUE(gz != NULL, "gzdopen(valid_fd, \"rb\") should return a non-NULL gzFile");

    // Cleanup: close the FD and ensure no resource leaks
    // Note: Do not close the gzFile here since it's not exercised; close fd separately.
    close(read_fd);
    close(pipefds[1]);// Close both ends to avoid descriptor leaks
}

/*
  Step 3: Test Suite Execution
  - Run the tests and print a concise summary.
  - Use non-terminating expectations so that both tests execute and report results.
  - This main function acts as the entry point and avoids any dependency on a specific
    unit testing framework, complying with the "no GTest" constraint.
*/
int main() {
    std::cout << "Starting gzdopen unit tests (no-GTest harness)" << std::endl;

    // Run tests
    test_gzdopen_negative_one_fd_returns_null();
    test_gzdopen_valid_fd_returns_non_null();

    // Summary
    std::cout << "Tests completed: " << total_checks
              << " checks, " << passed_checks << " passed." << std::endl;

    // If all checks pass, return 0; otherwise return 1 to indicate failures.
    return (passed_checks == total_checks) ? 0 : 1;
}