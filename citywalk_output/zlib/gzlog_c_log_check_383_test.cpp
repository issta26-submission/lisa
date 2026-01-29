/*
    Test suite for the focal method log_check located in gzlog.c.
    This test suite uses a lightweight, non-terminating assertion approach
    (no exceptions or test framework dependencies) and runs under a single
    main() function as required (GZLOG_DEBUG is defined to ensure the internal
    log_check symbol is compiled in).
    
    Test goals:
    - Validate behavior when the log file path does not exist (stat fails).
    - Validate behavior when the file exists but its mtime != log->lock.
    - Validate successful path when the file exists and mtime == log->lock
      and that log_check updates log->end to ".lock".
    
    Notes:
    - We include the gzlog.h header and the gzlog.c implementation directly
      into this translation unit to access the static function log_check
      (made possible by including within the same translation unit and defining
      GZLOG_DEBUG so the function is compiled).
    - We use temporary files in /tmp to simulate various filesystem states.
    - Static/internal helpers from gzlog.c are left untouched; tests only rely on
      public struct log fields (path, end, lock) exposed via gzlog.h.
*/

#define GZLOG_DEBUG 1

#include <cstdio>
#include <cstring>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <gzlog.h>
#include <utime.h>
#include <stdlib.h>
#include <stdio.h>
#include <gzlog.c>
#include <ctime>
#include <zlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <cstdlib>


/* Include C interfaces and definitions for gzlog; ensure C linkage for tests. */
extern "C" {
}

/* Lightweight test harness: non-terminating assertions. */
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_FAIL_MSG(prefix, msg) \
    do { \
        g_failed_tests++; \
        std::fprintf(stderr, "[TEST FAILURE] %s: %s (line %d)\n", (prefix), (msg), __LINE__); \
    } while (0)

#define TEST_OK_MSG(msg) \
    do { \
        std::fprintf(stdout, "[TEST PASS] %s\n", (msg)); \
    } while (0)

#define EXPECT_TRUE(cond, description) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            TEST_FAIL_MSG("EXPECT_TRUE", (description)); \
        } \
    } while (0)

#define EXPECT_EQ(a, b, description) \
    do { \
        ++g_total_tests; \
        if (!((a) == (b))) { \
            TEST_FAIL_MSG("EXPECT_EQ", (description)); \
            std::fprintf(stderr, "  Expected: %ld  Got: %ld\n", (long)(a), (long)(b)); \
        } \
    } while (0)

/* Helper to create a unique temporary file path (contents are not important). */
static bool create_temp_file_path(char* buffer, size_t len) {
    const char* template = "/tmp/gzlog_test_XXXXXX";
    if (snprintf(buffer, len, "%s", template) >= (int)len) {
        return false;
    }
    int fd = mkstemp(buffer);
    if (fd == -1) return false;
    close(fd);
    return true;
}

/* Test 1: log_check should return 1 when the log file path does not exist. */
static void test_log_check_missing_file() {
    // Initialize a dummy log structure
    struct log logobj;
    std::memset(&logobj, 0, sizeof(logobj));

    char path[256];
    // Create a temporary path and immediately remove the file to simulate missing file
    if (!create_temp_file_path(path, sizeof(path))) {
        std::fprintf(stderr, "Failed to create temp file path for missing-file test.\n");
        return;
    }
    // Remove the file to ensure non-existence
    unlink(path);

    // Copy path into log struct (path field should exist)
    std::strcpy(logobj.path, path);
    // Some arbitrary lock value
    logobj.lock = (time_t)time(nullptr);

    // Call focal function log_check
    int res = log_check(&logobj);

    EXPECT_EQ(res, 1, "log_check should return 1 when stat fails (missing file)");
    // Clean up just in case
    unlink(path);
    TEST_OK_MSG("test_log_check_missing_file completed");
}

/* Test 2: log_check should return 1 when the file exists but its mtime != log->lock. */
static void test_log_check_mtime_mismatch() {
    struct log logobj;
    std::memset(&logobj, 0, sizeof(logobj));

    char path[256];
    if (!create_temp_file_path(path, sizeof(path))) {
        std::fprintf(stderr, "Failed to create temp file path for mtime-mismatch test.\n");
        return;
    }

    // Create the file and set its mtime to a value
    int fd = open(path, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        std::perror("open");
        unlink(path);
        return;
    }
    close(fd);

    // Set file's mtime to now
    time_t t = time(nullptr);
    struct utimbuf times;
    times.actime = t;
    times.modtime = t;
    if (utime(path, &times) != 0) {
        std::perror("utime");
        unlink(path);
        return;
    }

    // Prepare log with a different lock value
    std::strcpy(logobj.path, path);
    logobj.lock = t + 120; // difference

    int res = log_check(&logobj);

    EXPECT_EQ(res, 1, "log_check should return 1 when st_mtime != log->lock");

    unlink(path);
    TEST_OK_MSG("test_log_check_mtime_mismatch completed");
}

/* Test 3: log_check should return 0 when the file exists and mtime == log->lock
   and should set log->end to ".lock". */
static void test_log_check_success_and_end_update() {
    struct log logobj;
    std::memset(&logobj, 0, sizeof(logobj));

    char path[256];
    if (!create_temp_file_path(path, sizeof(path))) {
        std::fprintf(stderr, "Failed to create temp file path for success-test.\n");
        return;
    }

    int fd = open(path, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        std::perror("open");
        unlink(path);
        return;
    }
    close(fd);

    time_t t = time(nullptr);
    struct utimbuf times;
    times.actime = t;
    times.modtime = t;
    if (utime(path, &times) != 0) {
        std::perror("utime");
        unlink(path);
        return;
    }

    std::strcpy(logobj.path, path);
    logobj.lock = t;

    int res = log_check(&logobj);

    EXPECT_EQ(res, 0, "log_check should return 0 when st_mtime == log->lock");
    // The function should have updated logobj.end to ".lock"
    if (std::strcmp(logobj.end, ".lock") != 0) {
        std::fprintf(stderr, "[TEST FAILURE] log_check did not update end to \".lock\" as expected\n");
        g_failed_tests++;
        g_total_tests++;
    } else {
        TEST_OK_MSG("log_check updated end to .lock as expected");
    }

    unlink(path);
    TEST_OK_MSG("test_log_check_success_and_end_update completed");
}

/* Main tester: run all tests and report summary. */
int main() {
    test_log_check_missing_file();
    test_log_check_mtime_mismatch();
    test_log_check_success_and_end_update();

    std::fprintf(stdout, "\nTest Summary: Total=%d, Failed=%d\n", g_total_tests, g_failed_tests);
    if (g_failed_tests > 0) {
        return 1;
    }
    return 0;
}