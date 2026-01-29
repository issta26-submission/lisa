// C++11 test suite for the focal method log_last in gzlog.c
// Strategy:
// - Compile with -DGZLOG_DEBUG to include the debug/testable portions of gzlog.c.
// - Include gzlog.c in the same translation unit to access the static log_last via a wrapper.
// - Use a small, self-contained test harness (no GTest) with simple assertions.
// - Create realistic file-descriptor-based scenarios to cover both success and failure paths.
// - Validate internal file modifications by inspecting the underlying temp files.

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
#include <cstdlib>


// Enable the debug/testable portions of gzlog.c
#define GZLOG_DEBUG

// Include the C header and the C source in the same translation unit so we can test the static function log_last
extern "C" {
}

// Include the gzlog.c implementation (now part of this translation unit)

// Forward declare the static function as a C symbol for wrapper access (allowed within the same TU)
extern "C" int log_last(struct log*, int);

// Wrapper to expose log_last for testing from this translation unit
extern "C" int test_log_last(struct log* log, int last) {
    // Directly call the focal static function
    return log_last(log, last);
}

// Simple test harness helpers
static bool fail(const char* msg) {
    if (msg) fprintf(stderr, "Test failure: %s\n", msg);
    return true;
}
static void pass(const char* msg) {
    if (msg) fprintf(stdout, "Test pass: %s\n", msg);
}

int main() {
    int overall_status = 0;

    // Test 1: Failure path when file descriptor is invalid
    // This tests the branch that should return -1 if lseek/read fail on back != 8 path.
    {
        struct log lg;
        // Force back path to use real read (last==first)
        lg.first = 0;
        lg.last  = 0;
        lg.back  = 9;  // back > 8 will be used when last == first
        lg.fd    = -1; // invalid fd to force lseek to fail
        lg.stored = 0;

        int res = test_log_last(&lg, 1);
        if (res != -1) {
            fail("Test 1: expected -1 on invalid fd, got different value");
            overall_status = 1;
        } else {
            pass("Test 1: correctly returned -1 on invalid fd");
        }
    }

    // Test 2: Success path with back > 8 (len == 2)
    // This exercises the non-error branch where we read a byte, compute mask, modify buf,
    // perform two PUT2 writes, and write 6 bytes starting at offset (log->last - len).
    {
        // Create a temporary file with enough space (8 bytes) initialized to zero
        char path[] = "/tmp/gzlog_test2XXXXXX";
        int fd = mkstemp(path);
        if (fd < 0) {
            perror("mkstemp");
            return 1;
        }
        unsigned char init[8] = {0,0,0,0,0,0,0,0};
        if (write(fd, init, 8) != 8) {
            perror("write init");
            close(fd);
            unlink(path);
            return 1;
        }

        struct log lg;
        lg.first = 4;
        lg.last  = 4;    // last == first to force back = lg.back
        lg.back  = 9;    // back > 8, so len = 2
        lg.fd    = fd;
        lg.stored = 0;

        int res = test_log_last(&lg, 1); // last != 0 -> non-zero last
        if (res != 0) {
            fail("Test 2: expected 0 on successful log_last execution");
            overall_status = 1;
        } else {
            // Inspect the 6 bytes written starting at offset 2 (log->last - len)
            unsigned char out[6];
            lseek(fd, 2, SEEK_SET);
            ssize_t r = read(fd, out, 6);
            if (r != 6) {
                fail("Test 2: read after write did not yield 6 bytes");
                overall_status = 1;
            } else {
                unsigned char expected[6] = {0x80, 0x00, 0x00, 0x00, 0xFF, 0xFF};
                if (memcmp(out, expected, 6) != 0) {
                    fprintf(stderr, "Test 2: content mismatch. Got: ");
                    for (int i = 0; i < 6; ++i) fprintf(stderr, "%02x ", out[i]);
                    fprintf(stderr, "\nExpected: ");
                    for (int i = 0; i < 6; ++i) fprintf(stderr, "%02x ", expected[i]);
                    fprintf(stderr, "\n");
                    overall_status = 1;
                } else {
                    pass("Test 2: content after log_last matches expected bytes");
                }
            }
        }
        close(fd);
        unlink(path);
    }

    // Test 3: Path where back == 8 (len == 1) without initial read
    // This exercises the branch where the code does not perform lseek/read and uses the
    // mask/two PUT2 writes plus a single-byte write.
    {
        char path[] = "/tmp/gzlog_test3XXXXXX";
        int fd = mkstemp(path);
        if (fd < 0) {
            perror("mkstemp");
            return 1;
        }
        unsigned char init[6] = {0,0,0,0,0,0};
        if (write(fd, init, 6) != 6) {
            perror("write init3");
            close(fd);
            unlink(path);
            return 1;
        }

        struct log lg;
        lg.first = 2;
        lg.last  = 1;     // last != first -> back == 8
        lg.back  = 8;     // not used in this branch
        lg.fd    = fd;
        lg.stored = 0;

        int res = test_log_last(&lg, 1); // non-zero last
        if (res != 0) {
            fail("Test 3: expected 0 on successful log_last execution (back==8)");
            overall_status = 1;
        } else {
            // The 5 bytes written start at offset 0
            unsigned char out[5];
            lseek(fd, 0, SEEK_SET);
            ssize_t r = read(fd, out, 5);
            if (r != 5) {
                fail("Test 3: read after write did not yield 5 bytes");
                overall_status = 1;
            } else {
                unsigned char expected[5] = {0x01, 0x00, 0x00, 0xFF, 0xFF};
                if (memcmp(out, expected, 5) != 0) {
                    fprintf(stderr, "Test 3: content mismatch. Got: ");
                    for (int i = 0; i < 5; ++i) fprintf(stderr, "%02x ", out[i]);
                    fprintf(stderr, "\nExpected: ");
                    for (int i = 0; i < 5; ++i) fprintf(stderr, "%02x ", expected[i]);
                    fprintf(stderr, "\n");
                    overall_status = 1;
                } else {
                    pass("Test 3: content after log_last (back==8) matches expected bytes");
                }
            }
        }
        close(fd);
        unlink(path);
    }

    if (overall_status == 0) {
        pass("All tests passed successfully.");
    } else {
        fail("Some tests failed. See messages above for details.");
    }

    return overall_status;
}