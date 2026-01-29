// Test suite for the focal method log_touch in gzlog.c
// This test suite invokes the static function log_touch indirectly by
// taking a function pointer to it within the same translation unit.
// It uses a minimal, non-terminating assertion style suitable for a C++11
// environment without GTest. The tests exercise the expected behavior of log_touch
// on existing and non-existing files, and verify the ".lock" suffix handling.
// Note: This test relies on POSIX APIs (utimes, stat) and is intended to run on
// Unix-like systems with a filesystem that supports utimes.

#define GZLOG_DEBUG
extern "C" {
#include <cstdio>
#include <cstring>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <gzlog.h>
#include <stdlib.h>
#include <stdio.h>
#include <gzlog.c>
#include <zlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

}


// Simple non-terminating test assertion macro
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE FAILED: %s\n", (msg)); \
        ++g_failures; \
    } \
} while(0)

typedef void (*log_touch_t)(struct log*);

// Helper to create a valid temporary file path and ensure its cleanup
static bool ensure_file_exists(const char* path) {
    int fd = open(path, O_CREAT | O_RDWR, 0644);
    if (fd < 0) {
        fprintf(stderr, "Failed to create temp file '%s': %s\n", path, strerror(errno));
        return false;
    }
    // Write something to ensure mtime updates meaningfully
    const char* data = "te";
    write(fd, data, strlen(data));
    close(fd);
    return true;
}

// Test 1: log_touch should update log->lock to the file's st_mtime when the path exists
static bool test_log_touch_updates_lock_when_file_exists() {
    // Create a unique temp file
    char path[256] = "/tmp/gzlog_test_exists_XXXXXX";
    int fd = mkstemp(path);
    if (fd == -1) {
        fprintf(stderr, "mkstemp failed: %s\n", strerror(errno));
        return false;
    }
    close(fd);

    // Prepare a log object as expected by gzlog's log_touch
    struct log l;
    // Initialize path field and end field
    // Assumes gzlog.h defines at least: char path[...]; char end[...]; time_t lock;
    // If actual sizes differ, adapt accordingly in your environment.
    memset(&l, 0, sizeof(l));
    strcpy(l.path, path);
    // end buffer is overwritten by log_touch to ".lock"
    l.end[0] = '\0';
    l.lock = 0;

    log_touch_t lt = log_touch; // take address of static function within same TU
    lt(&l);

    // Validate end suffix and lock update
    struct stat st;
    bool ok = true;
    if (stat(path, &st) != 0) {
        fprintf(stderr, "test_log_touch_exists: stat failed for created file\n");
        ok = false;
    } else {
        // log_touch should set lock to st_mtime when stat succeeds
        if (l.lock != st.st_mtime) {
            fprintf(stderr, "test_log_touch_exists: lock (%ld) != st_mtime (%ld)\n",
                    (long)l.lock, (long)st.st_mtime);
            ok = false;
        }
        // log_touch should set end to \".lock\"
        if (strcmp(l.end, ".lock") != 0) {
            fprintf(stderr, "test_log_touch_exists: end != \".lock\" (got '%s')\n", l.end);
            ok = false;
        }
    }

    // Cleanup
    unlink(path);
    return ok;
}

// Test 2: log_touch should not update log->lock when the path does not exist
static bool test_log_touch_no_update_when_file_missing() {
    // Non-existent path
    const char* path = "/tmp/gzlog_test_missing_abcdef";
    unlink(path); // ensure non-existence

    struct log l;
    memset(&l, 0, sizeof(l));
    strcpy(l.path, path);
    l.end[0] = '\0';
    l.lock = 0;

    log_touch_t lt = log_touch;
    lt(&l);

    // No file exists, so stat should fail and log->lock should remain unchanged (0)
    struct stat st;
    bool ok = true;
    if (stat(path, &st) == 0) {
        // If by any chance the file exists now, that's unexpected
        fprintf(stderr, "test_log_touch_missing: unexpected existing file\n");
        ok = false;
    } else {
        if (l.lock != 0) {
            fprintf(stderr, "test_log_touch_missing: expected lock to stay 0, got %ld\n", (long)l.lock);
            ok = false;
        }
        // end should still be set to ".lock"
        if (strcmp(l.end, ".lock") != 0) {
            fprintf(stderr, "test_log_touch_missing: end != \".lock\" (got '%s')\n", l.end);
            ok = false;
        }
    }

    return ok;
}

// Test 3: log_touch should always set end to ".lock" regardless of file existence
static bool test_log_touch_sets_end_to_lock_indicator() {
    // Existing file path
    char path[256] = "/tmp/gzlog_test_end_XXXXXX";
    int fd = mkstemp(path);
    if (fd == -1) {
        fprintf(stderr, "mkstemp failed: %s\n", strerror(errno));
        return false;
    }
    close(fd);

    struct log l;
    memset(&l, 0, sizeof(l));
    strcpy(l.path, path);
    // Pre-fill end with something else to verify it changes
    strcpy(l.end, "OLD");

    l.lock = 0;

    log_touch_t lt = log_touch;
    lt(&l);

    bool ok = true;
    if (strcmp(l.end, ".lock") != 0) {
        fprintf(stderr, "test_log_touch_end_indicator: end != \".lock\" (got '%s')\n", l.end);
        ok = false;
    }

    // Cleanup
    unlink(path);
    return ok;
}

int main() {
    // Run tests; use non-terminating assertions
    bool t1 = test_log_touch_updates_lock_when_file_exists();
    EXPECT_TRUE(t1, "log_touch should update lock when path exists");

    bool t2 = test_log_touch_no_update_when_file_missing();
    EXPECT_TRUE(t2, "log_touch should not update lock when path is missing");

    bool t3 = test_log_touch_sets_end_to_lock_indicator();
    EXPECT_TRUE(t3, "log_touch should set end to '.lock'");

    if (g_failures == 0) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Tests completed with %d failure(s).\n", g_failures);
        return 1;
    }
}