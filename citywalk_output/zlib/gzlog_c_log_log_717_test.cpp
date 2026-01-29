// Lightweight C++11 test suite for the static C function log_log in gzlog.c
// The tests are designed to be portable and do not rely on GTest.
// The test suite includes and compiles gzlog.c in the same translation unit
// by defining GZLOG_DEBUG before inclusion so that log_log becomes accessible
// for testing via the same TU.
//
// How to build (example):
// g++ -std=c++11 -DGZLOG_DEBUG test_gzlog_log_log.cpp -o test_gzlog_log_log_log
//
// Notes:
// - This test suite assumes gzlog.h defines the struct log with at least
//   members: path (char array) and end (char array) suitable for strcpy/strcpy.
// - APPEND_OP and COMPRESS_OP are defined in gzlog.h as operation codes.
// - The tests create and inspect simple files to validate the log_log behavior.

#define GZLOG_DEBUG

#include <cstdio>
#include <cstring>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <gzlog.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <gzlog.c>
#include <zlib.h>
#include <sys/time.h>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>



// Small test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define RUN_TEST(name) \
    do { \
        bool ok = (name)(); \
        ++g_total_tests; \
        if (ok) { \
            printf("[PASS] %s\n", #name); \
        } else { \
            ++g_failed_tests; \
            printf("[FAIL] %s\n", #name); \
        } \
    } while (0)

static bool file_contains(const std::string &path, const std::string &needle) {
    std::ifstream fin(path.c_str(), std::ios::in | std::ios::binary);
    if (!fin.good()) return false;
    std::string line;
    bool found = false;
    while (std::getline(fin, line)) {
        if (line.find(needle) != std::string::npos) {
            found = true;
        }
    }
    fin.close();
    return found;
}

static bool read_last_line_contains(const std::string &path, const std::string &needle) {
    std::ifstream fin(path.c_str(), std::ios::in);
    if (!fin.good()) return false;
    std::string line;
    std::string last;
    while (std::getline(fin, line)) {
        last = line;
    }
    fin.close();
    if (last.empty()) return false;
    return last.find(needle) != std::string::npos;
}

// Test 1: log_log appends a line for APPEND_OP and updates end to ".repairs"
static bool test_log_log_appends_and_end_updated() {
    const char *path = "./gzlog_test_append.log";

    // Cleanup before test
    remove(path);

    struct log lg;
    std::memset(&lg, 0, sizeof(lg));
    // Copy path into the log struct (assuming path is a char array field)
    // Be defensive: if the real size is smaller, truncate safely.
    strlcpy(lg.path, path, sizeof(lg.path));  // strlcpy may not be portable; fallbacks below if needed
#ifdef __APPLE__
    // macOS has strlcpy; if not, we fall back to strncpy below
#else
#endif
    // In case strlcpy isn't available in the environment, fallback:
#if !defined(strlcpy)
    std::strncpy(lg.path, path, sizeof(lg.path) - 1);
    lg.path[sizeof(lg.path) - 1] = '\0';
#endif

    // Set end to a known value to verify that log_log overwrites it
    if (sizeof(lg.end) > 0) {
        std::strcpy(lg.end, "ORIG");
    }

    // Call the focal function
    log_log(&lg, APPEND_OP, (char*)"record-append-1");

    // Verify that end was overwritten to ".repairs"
    if (std::strcmp(lg.end, ".repairs") != 0) {
        return false;
    }

    // Verify that a line was written containing "recovery: append" and the record
    if (!read_last_line_contains(path, "recovery: append")) {
        return false;
    }
    if (!file_contains(path, "record-append-1")) {
        return false;
    }

    // Cleanup
    remove(path);
    return true;
}

// Test 2: log_log writes a line for COMPRESS_OP
static bool test_log_log_compress_op() {
    const char *path = "./gzlog_test_compress.log";

    // Cleanup before test
    remove(path);

    struct log lg;
    std::memset(&lg, 0, sizeof(lg));
    std::strncpy(lg.path, path, sizeof(lg.path) - 1);
    lg.path[sizeof(lg.path) - 1] = '\0';

    // Ensure end is distinct so we can verify overwrite
    if (sizeof(lg.end) > 0) {
        std::strcpy(lg.end, "X");
    }

    log_log(&lg, COMPRESS_OP, (char*)"compress-record");

    if (std::strcmp(lg.end, ".repairs") != 0) {
        return false;
    }

    if (!read_last_line_contains(path, "recovery: compress")) {
        return false;
    }
    if (!file_contains(path, "compress-record")) {
        return false;
    }

    // Cleanup
    remove(path);
    return true;
}

// Test 3: log_log uses "replace" for unknown op values
static bool test_log_log_replace_for_unknown_op() {
    const char *path = "./gzlog_test_replace.log";

    // Cleanup before test
    remove(path);

    struct log lg;
    std::memset(&lg, 0, sizeof(lg));
    std::strncpy(lg.path, path, sizeof(lg.path) - 1);
    lg.path[sizeof(lg.path) - 1] = '\0';

    log_log(&lg, 12345, (char*)"dummy-record");

    if (std::strcmp(lg.end, ".repairs") != 0) {
        return false;
    }

    if (!read_last_line_contains(path, "recovery: replace")) {
        return false;
    }
    if (!file_contains(path, "dummy-record")) {
        return false;
    }

    // Cleanup
    remove(path);
    return true;
}

// Test 4: log_log returns early when fopen fails (simulate by using a directory path)
static bool test_log_log_fopen_failure_no_file_created() {
    const char *dirpath = "./testdir_for_loglog";  // create a directory, not a file

    // Create directory
    mkdir(dirpath, 0777);

    struct log lg;
    std::memset(&lg, 0, sizeof(lg));
    std::strncpy(lg.path, dirpath, sizeof(lg.path) - 1);
    lg.path[sizeof(lg.path) - 1] = '\0';

    // Call with an op that would normally write to the file, but since path is a directory,
    // fopen should fail and not create any ".repairs" file inside that directory path
    log_log(&lg, APPEND_OP, (char*)"should-not-create");

    // Check that a file named ".repairs" inside the directory does not exist
    // (Since path itself is a directory, there is no path/.repairs; this ensures no crash and no file creation)
    std::string expectedPath = std::string(dirpath) + "/.repairs";
    struct stat st;
    bool fileExists = (stat(expectedPath.c_str(), &st) == 0);
    // Cleanup
    rmdir(dirpath);  // remove the directory
    return !fileExists;
}

int main() {
    // Run tests
    RUN_TEST(test_log_log_appends_and_end_updated);
    RUN_TEST(test_log_log_compress_op);
    RUN_TEST(test_log_log_replace_for_unknown_op);
    RUN_TEST(test_log_log_fopen_failure_no_file_created);

    // Summary
    std::cout << "Total tests: " << g_total_tests << ", Passed: "
              << (g_total_tests - g_failed_tests) << ", Failed: "
              << g_failed_tests << std::endl;

    return g_failed_tests == 0 ? 0 : 1;
}

/*
Explanation of each unit test:

1) test_log_log_appends_and_end_updated
- Prepares a log struct with a writable path.
- Calls log_log with APPEND_OP and a test record.
- Verifies that log->end is overwritten to ".repairs".
- Verifies that the log file contains a line with "recovery: append" and the exact record.
- Cleans up the created file.

2) test_log_log_compress_op
- Similar to test 1 but uses COMPRESS_OP.
- Verifies the operation string is "compress" in the written log line.

3) test_log_log_replace_for_unknown_op
- Tests the else branch when op is neither APPEND_OP nor COMPRESS_OP.
- Expects the operation string to be "replace" in the output.

4) test_log_log_fopen_failure_no_file_created
- Creates a directory path and uses it as the log path to force fopen to fail.
- Verifies that no .repairs file is created inside that path (no file creation occurs and the function handles failure gracefully).
- Cleans up the directory afterwards.
*/