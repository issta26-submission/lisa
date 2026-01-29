/*
Unit test suite for the focal method family exposed by gzlog (public API surface).
Notes and design decisions:
- We cannot access the static internal log_replace function directly from C++ tests
  due to internal linkage (static/local in gzlog.c). Therefore, tests exercise the
  public API around gzlog, which may exercise the same code path that leads to log
  replacement during normal operation (rotation/rotation-like behavior) via
  gzlog_open/gzlog_write/gzlog_compress/gzlog_close.
- This test suite uses a lightweight, self-contained harness (no GoogleTest or GMock)
  and exercises both success and error-like scenarios as far as the public API allows.
- The tests are written for a C/C library integrated with C++11 code. We rely on the
  library’s behavior and system calls (rename/unlink) via the public API. If a particular
  environment differs (e.g., platform-specific behavior of renaming files), some tests
  may need minimal adjustments.
- Test style: non-terminating expectations. Failures are logged but do not abort the test
  run, to maximize coverage and exploration of code paths.
- The code below is intended to compile as a standalone C++11 translation unit
  (link against the existing gzlog.c/.o and gzlog.h in your build system).
- We provide explanatory comments for each test case.

To build (example, adapt to your build system):
g++ -std=c++11 -I/path/to/include -L/path/to/lib test_gzlog_log_replace.cpp -lgzlog -o test_gzlog_log_replace
*/

// Include standard headers for filesystem and I/O
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstring>
#include <errno.h>
#include <time.h>
#include <gzlog.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <cstdio>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


// Include the library header (public API)
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
#define LOG_TEST_SUMMARY() do { \
    std::cout << "\nTest Summary: " << tests_run << " run, " \
              << tests_failed << " failed, " \
              << (tests_run - tests_failed) << " passed.\n"; \
} while (0)

#define EXPECT_TRUE(cond) do { \
    ++tests_run; \
    if (!(cond)) { \
        ++tests_failed; \
        std::cerr << "EXPECT_TRUE FAILED: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
    } \
} while (0)

#define EXPECT_FALSE(cond) do { \
    ++tests_run; \
    if (cond) { \
        ++tests_failed; \
        std::cerr << "EXPECT_FALSE FAILED: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
    } \
} while (0)

#define EXPECT_EQ(a, b) do { \
    ++tests_run; \
    if (!((a) == (b))) { \
        ++tests_failed; \
        std::cerr << "EXPECT_EQ FAILED: " #a " == " #b " (got " << (a) ... << " vs " << (b) << ") at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while (0)

static int tests_run = 0;
static int tests_failed = 0;

/* Helper: create a unique temporary directory for test isolation */
static bool create_temp_dir(std::string &out_path) {
    // Create a unique directory under /tmp
    const char *template = "/tmp/gzlog_test_dir_XXXXXX";
    char *dir = strdup(template);
    if (dir == nullptr) return false;
    char *created = mkdtemp(dir);
    if (created == nullptr) {
        free(dir);
        return false;
    }
    out_path = std::string(created);
    free(dir);
    return true;
}

/* Helper: clean up a directory recursively (best-effort) */
static void rmdir_recursive(const std::string &path) {
    // Best-effort removal: remove files, then directory
    DIR *dir = opendir(path.c_str());
    if (!dir) return;
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        std::string full = path + "/" + entry->d_name;
        struct stat st;
        if (stat(full.c_str(), &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                rmdir_recursive(full);
            } else {
                unlink(full.c_str());
            }
        }
    }
    closedir(dir);
    rmdir(path.c_str());
}

/*
Note on log_replace:
- The focal method log_replace is static/internal in gzlog.c, guarded by GZLOG_DEBUG in the
  provided snippet. Directly calling log_replace is not possible from outside without
  special linkage tricks. Therefore, the following tests exercise the public API that would
  trigger the same rotation/replace behavior in typical production scenarios.
- If you want to exercise the exact internal log_replace function, consider adding a small
  integration test that links gzlog.c with GZLOG_DEBUG enabled and exports a non-static
  wrapper for a dedicated test translation unit. This test suite avoids that complexity and
  relies on the public interface as exercised below.
*/

// Test 1: Basic open/close lifecycle for gzlog object
static void test_gzlog_open_close_basic() {
    std::cout << "[Test] gzlog_open/gzlog_close basic lifecycle" << std::endl;
    std::string dir;
    bool ok = create_temp_dir(dir);
    EXPECT_TRUE(ok);

    gzlog *logd = gzlog_open(const_cast<char*>(dir.c_str())); // const_cast for C API
    EXPECT_TRUE(logd != nullptr);

    int rc = gzlog_close(logd);
    EXPECT_EQ(rc, 0);

    // Cleanup directory after test
    rmdir_recursive(dir);
}

// Test 2: Write some data and close, ensuring public API usable as a rotation trigger
static void test_gzlog_write_and_close_basic() {
    std::cout << "[Test] gzlog_write followed by gzlog_close" << std::endl;
    std::string dir;
    bool ok = create_temp_dir(dir);
    EXPECT_TRUE(ok);

    gzlog *logd = gzlog_open(const_cast<char*>(dir.c_str()));
    EXPECT_TRUE(logd != nullptr);

    const char *payload = "Hello, gzlog unit test!";
    int wres = gzlog_write(logd, (void*)payload, strlen(payload));
    // Some implementations may overwrite or internally buffer; ensure it accepts non-zero length
    EXPECT_TRUE(wres >= 0);

    int cres = gzlog_close(logd);
    EXPECT_EQ(cres, 0);

    rmdir_recursive(dir);
}

// Test 3: Compress after write path (positive path skeleton)
static void test_gzlog_compress_basic() {
    std::cout << "[Test] gzlog_compress basic usage" << std::endl;
    std::string dir;
    bool ok = create_temp_dir(dir);
    EXPECT_TRUE(ok);

    gzlog *logd = gzlog_open(const_cast<char*>(dir.c_str()));
    EXPECT_TRUE(logd != nullptr);

    const char *payload = "Data to compress";
    int wres = gzlog_write(logd, (void*)payload, strlen(payload));
    EXPECT_TRUE(wres >= 0);

    int cres = gzlog_compress(logd);
    // Depending on implementation, compress may succeed or be a no-op if not applicable.
    // We still record the outcome as a non-crashing, non-fatal event.
    EXPECT_TRUE(cres >= 0);

    gzlog_close(logd);

    rmdir_recursive(dir);
}

// Test 4: Multiple consecutive operations to simulate rotate/replace-like behavior
// This test exercises repeated open/write/close cycles to increase coverage around rotation paths.
static void test_gzlog_multiple_cycles() {
    std::cout << "[Test] gzlog multiple rotation-like cycles" << std::endl;
    std::string dir;
    bool ok = create_temp_dir(dir);
    EXPECT_TRUE(ok);

    // First cycle
    gzlog *logd1 = gzlog_open(const_cast<char*>(dir.c_str()));
    EXPECT_TRUE(logd1 != nullptr);
    const char *p1 = "cycle1 data";
    int w1 = gzlog_write(logd1, (void*)p1, strlen(p1));
    EXPECT_TRUE(w1 >= 0);
    gzlog_close(logd1);

    // Second cycle
    gzlog *logd2 = gzlog_open(const_cast<char*>(dir.c_str()));
    EXPECT_TRUE(logd2 != nullptr);
    const char *p2 = "cycle2 data";
    int w2 = gzlog_write(logd2, (void*)p2, strlen(p2));
    EXPECT_TRUE(w2 >= 0);
    int c2 = gzlog_compress(logd2);
    EXPECT_TRUE(c2 >= 0);
    int cl = gzlog_close(logd2);
    EXPECT_EQ(cl, 0);

    rmdir_recursive(dir);
}

// Entry point
int main() {
    std::cout << "Starting gzlog unit tests for log_replace-related behavior (via public API)" << std::endl;

    // Run tests
    test_gzlog_open_close_basic();
    test_gzlog_write_and_close_basic();
    test_gzlog_compress_basic();
    test_gzlog_multiple_cycles();

    LOG_TEST_SUMMARY();
    return (tests_failed > 0) ? 1 : 0;
}