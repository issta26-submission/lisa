/*
Test suite for the focal method log_close (via the public API gzlog_close) in gzlog.c
Implemented as a lightweight C++11 test harness (no GoogleTest) that exercises
gzlog_open, gzlog_write, and gzlog_close.

Notes:
- The static log_close function inside gzlog.c is only callable via the public API
  gzlog_close as long as GZLOG_DEBUG is defined (to ensure the debug path is compiled).
- This test uses the public interface to create a log, write some data, close it,
  and then re-open to ensure basic lifecycle behavior remains sane.
- The test is designed to be non-terminating (continues on failure) to maximize coverage.
- The test code assumes the repository provides gzlog.h/gzlog.c with the signatures
  shown below. Compile with the implementation alongside this test:
    g++ -std=c++11 -DGZLOG_DEBUG test_gzlog.cpp gzlog.c -o test_gzlog
*/ 

#include <cstdio>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <gzlog.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <cstdlib>


// Enable debug code in gzlog.c to ensure log_close is exercised via gzlog_close
#ifndef GZLOG_DEBUG
#define GZLOG_DEBUG
#endif

extern "C" {
}

// Lightweight test framework (non-terminating)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_START(name) \
    std::cout << "[TEST] " << (name) << " ..." << std::endl; \
    ++g_total_tests;

#define TEST_FAIL(msg) \
    do { std::cerr << "  Failure: " << (msg) << std::endl; ++g_failed_tests; } while (0)

#define TEST_PASS() \
    do { std::cout << "  Passed" << std::endl; } while (0)

static void test_summary() {
    std::cout << "\nTest summary: " << g_total_tests << " tests, "
              << g_failed_tests << " failures." << std::endl;
}

// Helper to create a unique temporary file path
static bool create_temp_path(std::string &out_path) {
    char tmpl[] = "/tmp/gzlog_test_XXXXXX";
    int fd = mkstemp(tmpl);
    if (fd < 0) {
        return false;
    }
    // Close the fd; gzlog_open will reopen/create as needed
    close(fd);
    out_path = std::string(tmpl);
    return true;
}

// Test 1: Open a gzlog, write data, then close. Basic lifecycle sanity.
static void test_open_write_close_basic() {
    TEST_START("Open -> Write -> Close basic lifecycle");

    std::string path;
    if (!create_temp_path(path)) {
        TEST_FAIL("Failed to create temporary path for test 1");
        TEST_PASS();
        return;
    }

    gzlog *log = gzlog_open((char*)path.c_str());
    if (log == nullptr) {
        TEST_FAIL("gzlog_open returned null");
        TEST_PASS();
        return;
    }

    // Attempt a simple write; expect non-negative result
    const char *payload = "hello";
    int ret = gzlog_write(log, (void*)payload, 5);
    if (ret < 0) {
        TEST_FAIL("gzlog_write failed to write data before close");
    } else {
        // ok
    }

    // Close the log; we don't rely on its exact return value, but expect it to succeed
    int close_ret = gzlog_close(log);
    if (close_ret != 0) {
        TEST_FAIL("gzlog_close did not return 0 (expected success)");
    } else {
        // close succeeded
        TEST_PASS();
    }

    // Reopen to ensure the path is usable again (sanity)
    gzlog *log2 = gzlog_open((char*)path.c_str());
    if (log2 == nullptr) {
        TEST_FAIL("gzlog_open after close returned null");
    } else {
        // write again to ensure reuse works
        ret = gzlog_write(log2, (void*)"world", 5);
        if (ret < 0) {
            TEST_FAIL("gzlog_write after reopen failed");
        } else {
            // ok
            gzlog_close(log2);
            TEST_PASS();
        }
    }

    // Cleanup: optional removal of temporary file
    unlink(path.c_str());

    // End of test 1
    std::cout << std::endl;
}

// Test 2: Close on an already closed/invalid log should not crash (robustness check)
static void test_close_on_already_closed() {
    TEST_START("Close on already closed log robustness");

    std::string path;
    if (!create_temp_path(path)) {
        TEST_FAIL("Failed to create temporary path for test 2");
        TEST_PASS();
        return;
    }

    gzlog *log = gzlog_open((char*)path.c_str());
    if (log == nullptr) {
        TEST_FAIL("gzlog_open returned null in test 2");
        TEST_PASS();
        return;
    }

    // Close once
    int ret = gzlog_close(log);
    if (ret != 0) {
        TEST_FAIL("First gzlog_close failed in test 2");
        TEST_PASS();
        return;
    }

    // Attempt a second close; should not crash
    ret = gzlog_close(log);
    if (ret != 0) {
        TEST_FAIL("Second gzlog_close failed (should be safe to call multiple times)");
    } else {
        TEST_PASS();
    }

    // Cleanup
    unlink(path.c_str());
    std::cout << std::endl;
}

// Test 3: Reopen after close and ensure basic operations still function (stateless reopen)
static void test_reopen_after_close_basic() {
    TEST_START("Reopen after close basic operation");

    std::string path;
    if (!create_temp_path(path)) {
        TEST_FAIL("Failed to create temporary path for test 3");
        TEST_PASS();
        return;
    }

    gzlog *log = gzlog_open((char*)path.c_str());
    if (log == nullptr) {
        TEST_FAIL("gzlog_open failed in test 3");
        TEST_PASS();
        return;
    }

    // Write something
    int ret = gzlog_write(log, (void*)"abc", 3);
    if (ret < 0) {
        TEST_FAIL("gzlog_write failed in test 3");
        gzlog_close(log);
        unlink(path.c_str());
        TEST_PASS();
        return;
    }

    // Close
    gzlog_close(log);

    // Reopen and write more
    gzlog *log2 = gzlog_open((char*)path.c_str());
    if (log2 == nullptr) {
        TEST_FAIL("gzlog_open (reopen) failed in test 3");
        unlink(path.c_str());
        TEST_PASS();
        return;
    }

    ret = gzlog_write(log2, (void*)"def", 3);
    if (ret < 0) {
        TEST_FAIL("gzlog_write after reopen failed in test 3");
    } else {
        TEST_PASS();
    }

    gzlog_close(log2);
    unlink(path.c_str());
    std::cout << std::endl;
}

// Main: run all tests
int main() {
    // Run tests
    test_open_write_close_basic();
    test_close_on_already_closed();
    test_reopen_after_close_basic();

    test_summary();
    return g_failed_tests > 0 ? 1 : 0;
}