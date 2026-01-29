// gun_test.cpp
// A lightweight, self-contained test suite for the focal method "out".
// Notes:
// - This test suite is crafted under the assumption that the focal function
//   out(void *out_desc, unsigned char *buf, unsigned len) is available for
//   testing in the current build (linkable). It targets core behaviors
//   (branch coverage) of the local out implementation described in the prompt.
// - Due to potential static linkage of the focal function in the original source,
//   this test uses a minimal, self-contained approach to verify behavior via
//   a plausible external interface while avoiding GTest usage.
// - The tests are designed to be executable in a C++11 environment with the C
//   runtime available (POSIX). They rely on a binary pipe to validate in/out
//   data when outfile is a valid descriptor and to exercise the write path.
// - The tests emphasize non-terminating, accumulative checks (do not exit on
//   first failure). They print results and continue, collecting a summary at the end.

#include <cstring>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <utime.h>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <zlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>


// Forward declaration of the focal function as it would appear in the real code.
// If the original project exposes it with C linkage, this declaration should
// match the actual signature.
extern "C" int out(void *out_desc, unsigned char *buf, unsigned len);

// Lightweight assertion helper that does not terminate the test.
// It records failures, prints a message, and continues.
static int g_test_failures = 0;
#define test_expect(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": " << msg << "\n"; \
        ++g_test_failures; \
    } else { \
        std::cout << "[PASS] " << __FUNCTION__ << ": " << msg << "\n"; \
    } \
} while (0)

// Lightweight wrapper for the test "outd" descriptor.
// We assume the internal struct layout used by the focal method is:
// struct outd { int check; unsigned int crc; unsigned long long total; int outfile; };
// The test provides memory matching this layout, with fields filled accordingly.
// This approach avoids requiring a separate header from the original project.
struct TestOutdDescriptor {
    int check;
    unsigned int crc;
    unsigned long long total;
    int outfile;
    // Padding fields are intentionally omitted; test relies on assumed layout.
};

// Helper: compute expected crc32 for a given buffer starting from 0.
static unsigned int expected_crc32(const unsigned char* buf, unsigned len) {
    return crc32(0U, buf, len);
}

// Helper: read all data from a pipe until EOF and return as a string.
static std::string pipe_read_all(int pipe_fd) {
    std::string acc;
    char tmp[256];
    ssize_t n;
    while ((n = read(pipe_fd, tmp, sizeof(tmp))) > 0) {
        acc.append(tmp, tmp + n);
    }
    return acc;
}

// Test Case A: me->check = 1, me->outfile = -1
// Expect: total increases by len, crc updated, no file I/O, return 0.
static bool test_out_case_crc_and_total_no_file() {
    TestOutdDescriptor me;
    me.check = 1;
    me.crc = 0;
    me.total = 0;
    me.outfile = -1;

    const char* data = "HelloWorld"; // 10 bytes
    unsigned len = (unsigned)strlen(data);

    int ret = out((void*)&me, (unsigned char*)data, len);

    test_expect(ret == 0, "Case A: return 0 when outfile is -1");
    test_expect(me.total == len, "Case A: total increased by len when check is true");
    unsigned int exp_crc = expected_crc32((const unsigned char*)data, len);
    test_expect(me.crc == exp_crc, "Case A: crc updated to crc32 of input when check is true");

    return g_test_failures == 0;
}

// Test Case B: me->check = 0, me->outfile = -1
// Expect: total remains unchanged, crc unchanged, return 0.
static bool test_out_case_no_check_no_file() {
    TestOutdDescriptor me;
    me.check = 0;
    me.crc = 0xA5A5A5A5;
    me.total = 12345ULL;
    me.outfile = -1;

    const char* data = "ABCDEF";
    unsigned len = (unsigned)strlen(data);

    int ret = out((void*)&me, (unsigned char*)data, len);

    test_expect(ret == 0, "Case B: return 0 when no outfile and not checking");
    test_expect(me.total == 12345ULL, "Case B: total unchanged when check is false");
    test_expect(me.crc == 0xA5A5A5A5, "Case B: crc unchanged when check is false");

    return g_test_failures == 0;
}

// Test Case C: me->outfile = valid pipe write end, me->check = 1
// Expect: data written to pipe equals input; total updated; return 0 (no error path)
static bool test_out_case_write_to_pipe() {
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        std::cerr << "[INIT] Pipe creation failed: " << strerror(errno) << "\n";
        return false;
    }
    int write_end = pipe_fd[1];
    int read_end = pipe_fd[0];

    TestOutdDescriptor me;
    me.check = 1;
    me.crc = 0;
    me.total = 0;
    me.outfile = write_end;

    const std::string data = "PipeData12345";
    unsigned len = (unsigned)data.size();

    int ret = out((void*)&me, (unsigned char*)data.data(), len);

    // Close write end so reader can observe EOF
    close(write_end);

    std::string read_back = pipe_read_all(read_end);
    close(read_end);

    test_expect(ret == 0, "Case C: return 0 on successful write to pipe");
    test_expect(read_back == data, "Case C: pipe received exactly the input data");

    // Also verify total updated and crc updated
    test_expect(me.total == len, "Case C: total updated to input length after write");
    unsigned int exp_crc = expected_crc32((const unsigned char*)data.data(), len);
    test_expect(me.crc == exp_crc, "Case C: crc computed over input data");

    return g_test_failures == 0;
}

// Test Case D: me->outfile = invalid fd (e.g., 99999) and me->check = 1
// Expect: function returns 1 on write failure; total and crc updated prior to write.
static bool test_out_case_invalid_fd() {
    TestOutdDescriptor me;
    me.check = 1;
    me.crc = 0;
    me.total = 0;
    me.outfile = 99999; // deliberately invalid FD

    const char* data = "XYZ";
    unsigned len = (unsigned)strlen(data);

    int ret = out((void*)&me, (unsigned char*)data, len);

    test_expect(ret == 1, "Case D: return 1 on write failure with invalid fd");
    test_expect(me.total == len, "Case D: total updated to len even on write failure");
    unsigned int exp_crc = expected_crc32((const unsigned char*)data, len);
    test_expect(me.crc == exp_crc, "Case D: crc updated even on write failure");

    return g_test_failures == 0;
}

// Runner
int main() {
    std::cout << "Starting gun_out() focal method tests...\n";

    bool ok = true;
    ok &= test_out_case_crc_and_total_no_file();
    ok &= test_out_case_no_check_no_file();
    ok &= test_out_case_write_to_pipe();
    ok &= test_out_case_invalid_fd();

    if (g_test_failures == 0) {
        std::cout << "[OVERALL PASS] All tests succeeded.\n";
        return 0;
    } else {
        std::cout << "[OVERALL FAIL] " << g_test_failures << " test(s) failed.\n";
        return 1;
    }
}