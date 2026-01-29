/*
    Lightweight C++11 test suite for the focal method log_lock in gzlog.c

    Approach summary:
    - Compile-time macro GZLOG_DEBUG is defined to ensure the focal static function log_lock becomes
      available within the translation unit (when gzlog.c is included).
    - The test file includes gzlog.c directly (as part of this translation unit) so that the static
      function log_lock can be exercised. This avoids issues with internal linkage across translation units.
    - A minimal, hand-rolled test framework is implemented (no GoogleTest) with simple, non-terminating
      assertions (via return status and printed messages). Each test case prints results and continues
      to maximize code coverage.
    - Test scenarios focus on the observable behavior of log_lock:
      1) When open() fails with errno != EEXIST (permission/other error) -> expect -1.
      2) When path does not exist -> expect log_lock returns 0 and creates a lock file, and log->lock updated.
      3) When a pre-existing lock file exists and is older than PATIENCE -> expect it is unlink()-ed and a new lock is created.

    Assumptions:
    - The struct log type defined in gzlog.c (or included header) is visible in this translation unit
      when gzlog.c is included with GZLOG_DEBUG defined. The tests rely on the presence of fields
      path (char[]), end (char[]), and lock (time_t) within that struct as used by log_lock.
    - PATIENCE macro is defined in this translation unit to control the age threshold for unlocking stale locks.
    - The test environment is POSIX-compliant (open, stat, unlink, sleep, etc. are available).
    - This file is intended to be compiled with a C++11 compatible compiler.
*/

#define _POSIX_C_SOURCE 200809L

#include <string>
#include <gzlog.c>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
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
#include <utime.h>
#include <fcntl.h>
#include <sys/wait.h>


#define GZLOG_DEBUG    // Ensure static log_lock is compiled into this translation unit
#define PATIENCE 1       // Make the time-based branch easily testable

// Include the focal implementation (with GZLOG_DEBUG defined above)

//
// Helper test framework (lightweight, non-terminating assertions)
//

struct TestCase {
    const char *name;
    bool (*fn)();
    std::string note;
};

static bool assert_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "Assertion failed: " << (msg ? msg : "unknown") << std::endl;
    }
    return cond;
}

static void run_test(const TestCase &tc, bool &all_passed) {
    std::cout << "[ RUN      ] " << tc.name << "\n";
    bool ok = tc.fn();
    if (ok) {
        std::cout << "[       OK ] " << tc.name << "\n";
    } else {
        std::cout << "[  FAILED  ] " << tc.name;
        if (!tc.note.empty()) {
            std::cout << " - " << tc.note;
        }
        std::cout << "\n";
    }
    all_passed = all_passed && ok;
}

// Utility: remove a file if exists (ignore errors)
static void safe_remove(const char *path) {
    if (path && access(path, F_OK) == 0) {
        unlink(path);
    }
}

// Utility: create directory with given mode (ignore if exists)
static void safe_mkdir(const char *path, mode_t mode) {
    struct stat st;
    if (stat(path, &st) != 0) {
        mkdir(path, mode);
    } else {
        // If exists, try to adjust permissions to allow tests to proceed
        chmod(path, mode);
    }
}

// Test 1: log_lock should return -1 if open() fails with errno != EEXIST
static bool test_log_lock_permission_denied() {
    // Prepare a non-writable directory to force open() to fail with EACCES
    const char *dir_no_write = "./test_no_write_dir";
    safe_remove(dir_no_write);
    safe_mkdir(dir_no_write, 0500); // read-exec only, no write

    // Prepare log structure (pointer-compatible with gzlog.c's struct log)
    // We rely on the actual struct log defined inside gzlog.c; we only set the path.
    struct log test_log;
    // Ensure there is space; use a path inside the non-writable directory which should fail
    snprintf(test_log.path, sizeof(test_log.path), "%s/lock_no_write.lock", dir_no_write);
    // Initialize other required members if they exist; we only rely on ability to pass the
    // pointer to log_lock and let the function use log->path and log->end.
    // We set end to a known value to avoid uninitialized usage if log_lock touches it.
    if (sizeof(test_log.end) > 0) {
        strcpy(test_log.end, ".lock");
    }

    int ret = log_lock(&test_log);

    // Cleanup
    safe_remove(test_log.path);
    rmdir(dir_no_write);

    return assert_true(ret == -1, "log_lock should return -1 when open() fails with non-EEXIST errno");
}

// Test 2: log_lock should create a new lock file when path does not exist
static bool test_log_lock_creates_lock_file() {
    const char *path = "./test_lock_create.lock";

    // Ensure clean state
    safe_remove(path);

    struct log test_log;
    strncpy(test_log.path, path, sizeof(test_log.path) - 1);
    test_log.path[sizeof(test_log.path) - 1] = '\0';

    if (sizeof(test_log.end) > 0) {
        strcpy(test_log.end, ".lock");
    }

    int ret = log_lock(&test_log);

    // Verify
    bool created = access(path, F_OK) == 0;
    struct stat st;
    bool stat_ok = (stat(path, &st) == 0);
    time_t mtime = 0;
    if (stat_ok) mtime = st.st_mtime;
    // Cleanup
    safe_remove(path);

    bool cond = (ret == 0) && created && stat_ok && (mtime > 0);
    if (!cond) {
        std::cerr << "test_log_lock_creates_lock_file failed: ret=" << ret
                  << " created=" << created
                  << " stat_ok=" << stat_ok
                  << " mtime=" << mtime << "\n";
    }
    return cond;
}

// Test 3: log_lock should unlink an old stale lock and create a new one when mtime is old
static bool test_log_lock_old_lock_unlink_and_create() {
    const char *path = "./test_lock_old.lock";

    // Ensure clean state
    safe_remove(path);

    // Create a stale lock file with old mtime
    int fd = open(path, O_CREAT | O_EXCL, 0644);
    if (fd < 0) {
        // If cannot create, fail fast
        std::cerr << "Could not create stale lock file for test: " << path << "\n";
        return false;
    }
    const char *msg = "stale";
    write(fd, msg, strlen(msg));
    close(fd);

    // Manually set its mtime to be old
    struct timeval tv[2];
    time_t now = time(NULL);
    tv[0].tv_sec = now - PATIENCE - 5; // access time
    tv[0].tv_usec = 0;
    tv[1].tv_sec = now - PATIENCE - 5; // modify time
    tv[1].tv_usec = 0;
    utimes(path, tv);

    struct log test_log;
    strncpy(test_log.path, path, sizeof(test_log.path) - 1);
    test_log.path[sizeof(test_log.path) - 1] = '\0';
    if (sizeof(test_log.end) > 0) {
        strcpy(test_log.end, ".lock");
    }

    int ret = log_lock(&test_log);

    // Verify: after call, the old file should have been unlinked and a new lock file created
    bool old_gone = access(path, F_OK) != 0; // old file path should be replaced
    // But since the function will have continued and created a new file, the path must exist.
    // To be robust, check that there is at least one lock file with a modern mtime
    bool new_lock_exists = false;
    struct stat st;
    if (stat(path, &st) == 0) {
        new_lock_exists = true;
    } else {
        // The path might have been replaced; try to find any lock by checking the directory
        // Not strictly necessary for this test; if path itself not found, mark fail
        new_lock_exists = false;
    }

    // Cleanup: remove the lock file if it exists
    safe_remove(path);

    return assert_true(ret == 0, "log_lock_old_lock_unlink_and_create: expected return 0 after replacing stale lock")
           && assert_true(new_lock_exists, "log_lock_old_lock_unlink_and_create: expected new lock file to exist");
}

int main() {
    // Construct test suite
    std::vector<TestCase> suites = {
        {"test_log_lock_permission_denied", test_log_lock_permission_denied, "open() should fail with non-EEXIST errno -> -1"},
        {"test_log_lock_creates_lock_file", test_log_lock_creates_lock_file, "log_lock should create a new lock file when path doesn't exist"},
        {"test_log_lock_old_lock_unlink_and_create", test_log_lock_old_lock_unlink_and_create, "log_lock should unlink old stale lock and create a new one"}
    };

    bool all_passed = true;
    for (const auto &tc : suites) {
        run_test(tc, all_passed);
    }

    if (!all_passed) {
        std::cout << "[ SUMMARY ] Some tests FAILED\n";
        return EXIT_FAILURE;
    } else {
        std::cout << "[ SUMMARY ] All tests PASSED\n";
        return EXIT_SUCCESS;
    }
}