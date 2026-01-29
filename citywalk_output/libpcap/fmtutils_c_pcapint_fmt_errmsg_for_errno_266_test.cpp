// Unit test suite for the focal method: pcapint_fmt_errmsg_for_errno
// This test harness is written in C++11 and does not rely on GTest.
// It targets the wrapper/facade behavior of pcapint_fmt_errmsg_for_errno
// which forwards varargs to pcapint_vfmt_errmsg_for_errno.
// The tests are designed to compile against the C codebase (fmtutils.c and related headers).

#include <string.h>
#include <portability.h>
#include <errno.h>
#include <fmtutils.h>
#include <stdarg.h>
#include <cstdarg>
#include <stddef.h>
#include <cerrno>
#include <stdio.h>
#include <config.h>
#include <charconv.h>
#include <pcap-int.h>
#include <cstring>
#include <string>
#include <iostream>
#include <ftmacros.h>


// Step 1 (Test Design Insight):
// Candidate Keywords (core components involved by the focal method):
// - pcapint_fmt_errmsg_for_errno (wrapper accepting varargs)
// - pcapint_vfmt_errmsg_for_errno (va_list variant, core formatter)
// - errno values (ENOENT, EACCES, etc.)
// - strerror(...) (system-provided error strings on POSIX)
// - buffer management (errbuf, errbuflen)
// - fmt (printf-style format string) and its arguments
// - cross-platform considerations (Windows vs POSIX) as seen in the focal class dep
// - PCAP_ERRBUF_SIZE and related portability concerns
// These keywords inform test cases that (a) verify formatted prefix, (b) verify appended system message, and (c) cover typical formatting with va_args and strings.

// Step 2 (Unit Test Generation):
// Prototypes for the focal C functions (assuming linkage with the C library under test)
extern "C" {
    // Forwarder: uses varargs
    void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t errbuflen, int errnum,
        const char *fmt, ...);

    // va_list variant (the implementation called by the wrapper)
    void pcapint_vfmt_errmsg_for_errno(char *errbuf, size_t errbuflen, int errnum,
        const char *fmt, va_list ap);
}

// Simple C++ test harness (no GTest/GMock)

static bool test_basic_fmt_with_errno_noarg() {
    // This test validates that the wrapper formats the given fmt with provided args,
    // and then appends the system error message for errno ENOENT (No such file or directory),
    // followed by the numeric error in parentheses, preserving the expected layout.
    // Expected: "Test case 42: No such file or directory (2)"
    char buf[1024];
    int err = ENOENT; // 2 on POSIX
    const char *fmt = "Test case %d";
    int arg = 42;

    pcapint_fmt_errmsg_for_errno(buf, sizeof(buf), err, fmt, arg);

    std::string actual(buf);
    const char *errstr = strerror(err);
    unsigned long errno_num = static_cast<unsigned long>(err);
    std::string expected = std::string("Test case 42: ") + errstr + " (" + std::to_string(errno_num) + ")";

    if (actual != expected) {
        std::cerr << "[test_basic_fmt_with_errno_noarg] FAILED\n";
        std::cerr << "  actual  : \"" << actual << "\"\n";
        std::cerr << "  expected: \"" << expected << "\"\n";
        return false;
    }
    return true;
}

// Test 2: verify formatting with a string argument in fmt and non-empty errno message.
static bool test_fmt_with_string_argument() {
    // Expected: "open: secret.txt: <errno_message> (<errno>)"
    char buf[1024];
    int err = EACCES; // 13 on POSIX
    const char *fmt = "open: %s";
    const char *filename = "secret.txt";

    pcapint_fmt_errmsg_for_errno(buf, sizeof(buf), err, fmt, filename);

    std::string actual(buf);
    const char *errstr = strerror(err);
    unsigned long errno_num = static_cast<unsigned long>(err);
    std::string expected = std::string("open: secret.txt: ") + errstr + " (" + std::to_string(errno_num) + ")";

    if (actual != expected) {
        std::cerr << "[test_fmt_with_string_argument] FAILED\n";
        std::cerr << "  actual  : \"" << actual << "\"\n";
        std::cerr << "  expected: \"" << expected << "\"\n";
        return false;
    }
    return true;
}

// Test 3: ensure behavior with a relatively small but non-trivial buffer.
// The initial portion "LongFmt 7" should appear as a prefix if the formatter runs correctly.
static bool test_small_buffer_prefix() {
    // Use a small but non-zero buffer to exercise basic formatting without overflow.
    char buf[32];
    int err = ENOENT;
    const char *fmt = "LongFmt %d";
    int arg = 7;

    pcapint_fmt_errmsg_for_errno(buf, sizeof(buf), err, fmt, arg);

    // Expect the string to start with "LongFmt 7" regardless of how much content is appended after.
    bool starts_with = (std::strncmp(buf, "LongFmt 7", 9) == 0);
    if (!starts_with) {
        std::cerr << "[test_small_buffer_prefix] FAILED\n";
        std::cerr << "  actual: \"" << buf << "\"\n";
        return false;
    }
    return true;
}

// Minimal harness to run tests without a testing framework.
int main() {
    bool all_passed = true;

    // Run tests; keep results informative but avoid terminating on first failure to improve coverage.
    if (!test_basic_fmt_with_errno_noarg()) {
        all_passed = false;
        std::cerr << "Test 1 failed.\n";
    } else {
        std::cout << "Test 1 passed.\n";
    }

    if (!test_fmt_with_string_argument()) {
        all_passed = false;
        std::cerr << "Test 2 failed.\n";
    } else {
        std::cout << "Test 2 passed.\n";
    }

    if (!test_small_buffer_prefix()) {
        all_passed = false;
        std::cerr << "Test 3 failed.\n";
    } else {
        std::cout << "Test 3 passed.\n";
    }

    if (all_passed) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Some tests failed.\n";
        return 1;
    }
}