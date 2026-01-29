// Unit tests for the focal method: write_byte(struct file *file, int b)
// Context: The function writes a single byte to the output file if available.
//          It increments write_count in all cases. If the output file exists and
//          the write fails, it records errno, sets a WRITE_ERROR flag, and calls
//          stop(...). We test the two safe paths: with an output FILE* and without one.
// Note: This test harness is written in C++11 and uses a minimal, non-terminating
//       assertion style (no process termination on failure). It relies on linking
//       against the production pngfix.c where write_byte and the struct file are defined.
//       We declare a compatible struct layout locally to interact with the C function.

#include <unistd.h>
#include <setjmp.h>
#include <cerrno>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Forward declaration and a minimal compatible struct definition to align with the
// production's struct file. We only declare the fields that write_byte touches.
extern "C" {
struct file {
    FILE *out;
    int write_errno;
    int status_code;
    int write_count;
};

// Focal method under test (from pngfix.c)
void write_byte(struct file *file, int b);
}

// Lightweight non-terminating test framework
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_CASE(name) void name()
#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        fprintf(stderr, "TEST FAILURE: %s\n", (msg)); \
        ++g_failed_tests; \
    } \
} while (0)

static void test_write_byte_with_output_file() {
    // Description:
    // true branch when file->out is not NULL and the write call succeeds.
    // We verify: write_count increments, write_errno remains 0, status_code remains unchanged,
    // and the byte is actually written to the output stream (via a temporary in-memory file).

    // Prepare a temporary in-memory file to capture output without polluting stdout.
    FILE *tmp = tmpfile();
    if (tmp == NULL) {
        fprintf(stderr, "SKIP: Unable to create temporary file for test_write_byte_with_output_file\n");
        return;
    }

    struct file f;
    f.out = tmp;
    f.write_errno = 0;
    f.status_code = 0;
    f.write_count = 0;

    int test_byte = 'A'; // 0x41

    // Execute the focal method
    write_byte(&f, test_byte);

    // Validate side effects
    EXPECT_TRUE(f.write_count == 1, "write_count should increment to 1 after writing one byte");
    EXPECT_TRUE(f.write_errno == 0, "write_errno should remain 0 on successful write");
    EXPECT_TRUE(f.status_code == 0, "status_code should remain unchanged on successful write");

    // Validate that the byte was actually written to the output
    fflush(tmp);
    // Rewind to read what was written
    int read_back = -1;
    if (fseek(tmp, 0, SEEK_SET) == 0) {
        read_back = fgetc(tmp);
    }

    EXPECT_TRUE(read_back == test_byte, "Output should contain the written byte 'A'");

    // Cleanup
    fclose(tmp);
}

static void test_write_byte_without_output_file() {
    // Description:
    // false branch when file->out is NULL; ensure write_count increments and no write
    // is attempted.

    struct file f;
    f.out = NULL;
    f.write_errno = 0;
    f.status_code = 0;
    f.write_count = 0;

    int test_byte = 0x42; // 'B'

    write_byte(&f, test_byte);

    // Validate side effects
    EXPECT_TRUE(f.write_count == 1, "write_count should increment to 1 even when no output file exists");
    EXPECT_TRUE(f.write_errno == 0, "write_errno should remain 0 when no output file exists");
    EXPECT_TRUE(f.status_code == 0, "status_code should remain unchanged when no output file exists");
}

static void run_all_tests() {
    test_write_byte_with_output_file();
    test_write_byte_without_output_file();
}

// Explanation comment: The test harness is intended to be invoked from main.
// This mirrors the guidance to "call test methods from the main function" when GTest is not used.
int main() {
    run_all_tests();

    if (g_failed_tests != 0) {
        fprintf(stderr, "Unit tests finished with %d failure(s) out of %d test(s).\n",
                g_failed_tests, g_total_tests);
        return 1;
    } else {
        fprintf(stderr, "All %d unit test(s) passed.\n", g_total_tests);
        return 0;
    }
}