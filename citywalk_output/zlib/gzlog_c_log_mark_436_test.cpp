// gzlog_log_mark_test.cpp
// Lightweight C++11 unit tests for the C function: log_mark(struct log *log, int op)
// This test suite does not rely on GTest. It uses a small in-file test harness
// with non-terminating assertions to maximize code coverage.
//
// Notes:
// - The tests interact with the real gzlog.h definitions (struct log, HEAD, EXTRA, PUT8/PUT4/PUT2, etc.).
// - Tests assume a POSIX-like environment (open, lseek, read, write, fsync, etc.).
// - Test 1: Successful write path with op = 0; verifies that 35-byte ext chunk is written correctly.
// - Test 2: Successful write path with op = 1; verifies that ext[34] encodes op correctly.
// - Test 3: Failure path due to invalid fd (-1); verifies log_mark returns -1.
// - All tests create and clean up their own temporary files.
// - Static/internal helpers within gzlog.c are not mocked; we exercise the public API surface.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <gzlog.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <cstdlib>


// Include the focal class/dependency header to access the exact types/macros
extern "C" {
}

// Simple non-terminating test harness
static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if (cond) { \
        std::cout << "[PASS] " << msg << "\n"; \
    } else { \
        std::cerr << "[FAIL] " << msg << "\n"; \
        ++g_failures; \
    } \
} while (0)

// Helper: create a unique temporary file and return its fd.
// Caller is responsible for closing and unlinking the file.
static int create_temp_file(char* path_out, size_t len) {
    // Use a template in /tmp
    const char* tmpl = "/tmp/gzlog_testXXXXXX";
    char tmp_path[256];
    if (strlen(tmpl) >= sizeof(tmp_path)) {
        return -1;
    }
    strcpy(tmp_path, tmpl);
    int fd = mkstemp(tmp_path);
    if (fd < 0) {
        return -1;
    }
    // Ensure a clean file
    ftruncate(fd, 0);
    if (path_out && len > 0) {
        strncpy(path_out, tmp_path, len);
        path_out[len-1] = '\0';
    }
    return fd;
}

// Test 1: log_mark should succeed and write a 35-byte ext block starting at HEAD.
// It also verifies that the bytes written exactly match the macros-derived ext content.
static void test_log_mark_success_op0() {
    std::cout << "Test 1: log_mark success path with op = 0\n";

    char path[256];
    int fd = create_temp_file(path, sizeof(path));
    if (fd < 0) {
        std::cerr << "[ERROR] Could not create temporary file for Test 1\n";
        ++g_failures;
        return;
    }

    // Prepare a log struct as expected by log_mark
    struct log l;
    // Initialize all fields that log_mark uses
    l.fd = fd;
    l.first = 0x1122334455667788ULL;
    l.last  = 0x99AABBCCDDEEFF00ULL;
    l.ccrc  = 0x01020304;
    l.clen  = 0x05060708;
    l.tcrc  = 0x090A0B0C;
    l.tlen  = 0x0D0E0F10;
    l.stored = 0x1234;
    l.back  = 5; // ext[34] = back - 3 + (op << 3) => 5 - 3 + (0<<3) = 2

    // Call the focal method
    int ret = log_mark(&l, 0);

    CHECK(ret == 0, "log_mark returned 0 on success (op=0)");

    // Read back the ext block written at HEAD
    unsigned char actual[EXTRA];
    off_t pos = lseek(fd, HEAD, SEEK_SET);
    CHECK(pos == (off_t)HEAD, "lseek to HEAD succeeded for Test 1");
    ssize_t rd = read(fd, actual, EXTRA);
    CHECK(rd == EXTRA, "Read expected EXTRA bytes after log_mark (Test 1)");

    // Build expected ext bytes using the same macros and log fields
    unsigned char expected[EXTRA];
    memset(expected, 0, EXTRA);
    PUT8(expected, l.first);
    PUT8(expected + 8, l.last);
    PUT4(expected + 16, l.ccrc);
    PUT4(expected + 20, l.clen);
    PUT4(expected + 24, l.tcrc);
    PUT4(expected + 28, l.tlen);
    PUT2(expected + 32, l.stored);
    expected[34] = l.back - 3 + (0 << 3);

    bool equal = (memcmp(actual, expected, EXTRA) == 0);
    CHECK(equal, "Ext block matches expected values for op=0");

    close(fd);
    unlink(path);
}

// Test 2: log_mark should succeed for op = 1 and correctly set ext[34].
// This ensures the (op << 3) part of the computation affects the output.
static void test_log_mark_success_op1() {
    std::cout << "Test 2: log_mark success path with op = 1\n";

    char path[256];
    int fd = create_temp_file(path, sizeof(path));
    if (fd < 0) {
        std::cerr << "[ERROR] Could not create temporary file for Test 2\n";
        ++g_failures;
        return;
    }

    struct log l;
    l.fd = fd;
    l.first = 0xAABBCCDDEEFF0011ULL;
    l.last  = 0x1122334455667788ULL;
    l.ccrc  = 0x01020304;
    l.clen  = 0x05060708;
    l.tcrc  = 0x090A0B0C;
    l.tlen  = 0x0D0E0F10;
    l.stored = 0x5678;
    l.back  = 6;

    int ret = log_mark(&l, 1);
    CHECK(ret == 0, "log_mark returned 0 on success (op=1)");

    unsigned char actual[EXTRA];
    off_t pos = lseek(fd, HEAD, SEEK_SET);
    CHECK(pos == (off_t)HEAD, "lseek to HEAD succeeded for Test 2");
    ssize_t rd = read(fd, actual, EXTRA);
    CHECK(rd == EXTRA, "Read expected EXTRA bytes after log_mark (Test 2)");

    unsigned char expected[EXTRA];
    memset(expected, 0, EXTRA);
    PUT8(expected, l.first);
    PUT8(expected + 8, l.last);
    PUT4(expected + 16, l.ccrc);
    PUT4(expected + 20, l.clen);
    PUT4(expected + 24, l.tcrc);
    PUT4(expected + 28, l.tlen);
    PUT2(expected + 32, l.stored);
    // op = 1 => (op << 3) = 8
    expected[34] = l.back - 3 + (1 << 3);

    bool equal = (memcmp(actual, expected, EXTRA) == 0);
    CHECK(equal, "Ext block matches expected values for op=1");

    close(fd);
    unlink(path);
}

// Test 3: log_mark should return -1 when the file descriptor is invalid (-1).
static void test_log_mark_bad_fd() {
    std::cout << "Test 3: log_mark with invalid fd (-1) should fail\n";

    struct log l;
    l.fd = -1;
    l.first = 0x0102030405060708ULL;
    l.last  = 0x1011121314151617ULL;
    l.ccrc  = 0x00112233;
    l.clen  = 0x44556677;
    l.tcrc  = 0x8899AABB;
    l.tlen  = 0xCCDDEEFF;
    l.stored = 0x7A7B;
    l.back  = 4;

    int ret = log_mark(&l, 0);
    CHECK(ret != 0, "log_mark returned non-zero value with invalid fd (expected -1)");
}

// Entry point
int main() {
    std::cout << "Running gzlog log_mark unit tests (C++11, no GTest)\n";

    test_log_mark_success_op0();
    test_log_mark_success_op1();
    test_log_mark_bad_fd();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << "Total failures: " << g_failures << "\n";
    }

    // Return the number of failures as exit code for easy scripting
    return g_failures;
}