// Lightweight C++11 test suite for the focal method log_append (gzlog.c)
// Note: We exercise the public APIs that exercise log_append indirectly via gzlog_write/gzlog_open.
// The tests are written to be GTest-free, using a small, non-terminating assertion style.
// The goal is to provide a stand-alone test harness that compiles with C++11 and links against the
// C implementation gzlog.c (and zlib if required).

/*
Step 1 (Program Understanding, summarized for test design):
- The focal function is log_append(struct log *log, unsigned char *data, size_t len).
- It appends data to the current "stored" block, creates new blocks as needed, updates CRC and length,
  writes a trailer with CRC and length, and marks the log file done by manipulating .add/.path files.
- Dependencies include file I/O (write, lseek, ftruncate), CRC (crc32), block management (log_last, MAX_STORE),
  and filesystem state (log_mark, unlink, strcpy).
- We will test the observable behavior by exercising gzlog_write and gzlog_close after opening a log.
- Since log_append is static/internal, we access it via the public API wrappers in gzlog.c.
*/

/* <DOMAIN_KNOWLEDGE> notes preserved:
- We must compile with C++11, and avoid GTest.
- Access to static/internal functions should be through public interfaces; no private fields accessed directly.
- Use standard library only; keep tests non-terminating on failures (i.e., continue running where possible).
- If additional mocking is needed, it should be done via test doubles that do not rely on private internals.
</DOMAIN_KNOWLEDGE> */

// Include C headers via C linkage to match the C implementation
extern "C" {
#include <string>
#include <dirent.h>
#include <mkstemps.h>
#include <unistd.h>
#include <cstdlib>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include <errno.h>
#include <time.h>
#include <gzlog.h>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <cstdio>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

}


// Simple non-terminating test harness
static void log_note(const char* msg) {
    std::cout << "[NOTE] " << msg << std::endl;
}
static void log_pass(const char* test) {
    std::cout << "[PASS] " << test << std::endl;
}
static void log_fail(const char* test, const char* reason) {
    std::cerr << "[FAIL] " << test << " - " << reason << std::endl;
}

// Helper: create a unique test directory and return its path.
// Caller is responsible for cleanup (rmdir) if the directory is empty.
static std::string create_test_dir() {
    // Template for a temporary directory
    char tmpl[] = "/tmp/gzlog_testdir_XXXXXX";
    char* dir = mkdtemp(tmpl);
    if (!dir) {
        throw std::runtime_error("Failed to create temporary test directory");
    }
    return std::string(dir);
}

// Helper: build a path inside a directory
static std::string make_path_in_dir(const std::string& dir, const std::string& basename) {
    return dir + "/" + basename;
}

// Helper: remove a path if it exists (best-effort)
static void safe_remove(const std::string& path) {
    if (path.empty()) return;
    if (unlink(path.c_str()) == 0) {
        // removed
        return;
    }
    // If it's a directory, try rmdir
    if (rmdir(path.c_str()) == 0) {
        return;
    }
    // Otherwise ignore silently
}

// Test 1: Basic single-write path using log_append via public API
// - Open a new gzlog at a unique path
// - Write a small payload
// - Close the log
// - Expect success (0 return codes)
static bool test_basic_write() {
    log_note("Test 1: Basic single-write path through public API (gzlog_open/gzlog_write/gzlog_close)");

    // Setup temporary directory and file path
    std::string dir = create_test_dir();
    std::string path = make_path_in_dir(dir, "testlog.gzadd"); // .add-based naming may be used by gzlog

    // Open log
    gzlog *logd = gzlog_open(const_cast<char*>(path.c_str()));
    if (logd == nullptr) {
        log_fail("test_basic_write", "gzlog_open returned null");
        // Cleanup directory
        safe_remove(path);
        rmdir(dir.c_str());
        return false;
    }

    // Prepare data
    const char *payload = "HelloGzlog";
    size_t len = std::strlen(payload);

    // Write data
    int wres = gzlog_write(logd, (void*)payload, len);
    if (wres != 0) {
        log_fail("test_basic_write", "gzlog_write (first call) returned non-zero");
        gzlog_close(logd);
        safe_remove(path);
        rmdir(dir.c_str());
        return false;
    }

    // Finalize
    int cres = gzlog_close(logd);
    if (cres != 0) {
        log_fail("test_basic_write", "gzlog_close returned non-zero");
        safe_remove(path);
        rmdir(dir.c_str());
        return false;
    }

    // Cleanup: remove generated artifacts (best-effort)
    safe_remove(path);
    rmdir(dir.c_str());

    log_pass("test_basic_write");
    return true;
}

// Test 2: Multiple writes across blocks to trigger internal block-management logic
// - Open a log
// - Write several small chunks in succession
// - Close the log
// - Expect success
static bool test_multiple_writes() {
    log_note("Test 2: Multiple writes across blocks (gzlog_write called multiple times)");

    std::string dir = create_test_dir();
    std::string path = make_path_in_dir(dir, "testlog_multi.gzadd");

    gzlog *logd = gzlog_open(const_cast<char*>(path.c_str()));
    if (logd == nullptr) {
        log_fail("test_multiple_writes", "gzlog_open returned null");
        safe_remove(path);
        rmdir(dir.c_str());
        return false;
    }

    // Prepare multiple payload chunks
    const char* chunks[] = {
        "Chunk1-", "Chunk2-", "Chunk3-",
        "Chunk4-", "Chunk5-"
    };
    size_t total = 0;
    for (const char* c : chunks) {
        size_t l = std::strlen(c);
        int r = gzlog_write(logd, (void*)c, l);
        if (r != 0) {
            log_fail("test_multiple_writes", "gzlog_write failed on a chunk");
            gzlog_close(logd);
            safe_remove(path);
            rmdir(dir.c_str());
            return false;
        }
        total += l;
    }

    int cres = gzlog_close(logd);
    if (cres != 0) {
        log_fail("test_multiple_writes", "gzlog_close failed after multiple writes");
        safe_remove(path);
        rmdir(dir.c_str());
        return false;
    }

    // Cleanup
    safe_remove(path);
    rmdir(dir.c_str());

    log_pass("test_multiple_writes");
    return true;
}

// Test 3: Opening with a directory path should fail (static create/open semantics should reject non-file path)
// - Create a directory
// - Attempt to open a gzlog with that directory path
// - Expect null return from gzlog_open
static bool test_open_with_dir_path() {
    log_note("Test 3: Open with directory path should fail (gzlog_open returns null)");

    // Create a directory
    std::string dir = create_test_dir();
    // Use the directory path as the file path
    gzlog *logd = gzlog_open(const_cast<char*>(dir.c_str()));
    if (logd != nullptr) {
        log_fail("test_open_with_dir_path", "gzlog_open unexpectedly succeeded with directory path");
        gzlog_close(logd);
        rmdir(dir.c_str());
        return false;
    }

    // Cleanup
    // Directory likely still exists; remove it
    rmdir(dir.c_str());

    log_pass("test_open_with_dir_path");
    return true;
}

// Entry point
int main() {
    // Basic harness: run three tests and report summary
    int total = 0;
    int passed = 0;

    log_note("Starting unit tests for log_append (via public gzlog API).");

    if (test_basic_write()) { passed++; }
    total++;

    if (test_multiple_writes()) { passed++; }
    total++;

    if (test_open_with_dir_path()) { passed++; }
    total++;

    std::cout << "Test Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}