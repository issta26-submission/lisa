// Minimal C++11 test harness for the C function put_uLong from fakepng.c
// This harness captures stdout output by redirecting stdout to a pipe
// and verifies the four-byte big-endian representation produced by put_uLong.
//
// Assumptions:
// - The target environment is POSIX (pipe, dup, dup2, read, etc. are available).
// - The C focal function put_uLong is defined in fakepng.c and uses the zlib type uLong.
// - We do not depend on GTest or any other testing framework; we perform direct assertions
//   and report results in main().

#include <vector>
#include <zlib.h>
#include <functional>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>

extern "C" {
}

// Declare the focal C function with C linkage
extern "C" void put_uLong(uLong val);

// Helper: capture exactly 4 bytes written to stdout by put_uLong.
// Returns true if 4 bytes were captured and writes them into out[0..3].
static bool capture_put_uLong(uLong val, unsigned char out[4]) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return false;
    }

    // Save current stdout
    int stdout_fd = fileno(stdout);
    if (stdout_fd == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }
    int saved_stdout = dup(stdout_fd);
    if (saved_stdout == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }

    // Redirect stdout to the write end of the pipe
    if (dup2(pipefd[1], stdout_fd) == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return false;
    }

    // We no longer need the separate write end in the parent
    close(pipefd[1]);

    // Invoke the focal function which writes to stdout
    put_uLong(val);

    // Ensure all output is flushed to the pipe
    fflush(stdout);

    // Restore original stdout
    dup2(saved_stdout, stdout_fd);
    close(saved_stdout);

    // Read exactly 4 bytes from the pipe
    ssize_t n = read(pipefd[0], out, 4);
    close(pipefd[0]);

    // Expect exactly 4 bytes
    return (n == 4);
}

/*
  Test 1: Zero value should produce four zero bytes in big-endian order: 0x00 0x00 0x00 0x00
  Rationale: Basic correctness of byte extraction and order.
*/
static bool test_put_uLong_zero() {
    unsigned char out[4];
    const unsigned char expected[4] = {0x00, 0x00, 0x00, 0x00};
    bool ok = capture_put_uLong((uLong)0, out);
    if (!ok) {
        std::cerr << "test_put_uLong_zero: failed to capture output\n";
        return false;
    }
    return std::memcmp(out, expected, 4) == 0;
}

/*
  Test 2: Maximum value should produce four 0xFF bytes.
  Rationale: Ensure upper-bound byte representation is correct.
*/
static bool test_put_uLong_max() {
    unsigned char out[4];
    const unsigned char expected[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    bool ok = capture_put_uLong((uLong)0xFFFFFFFF, out);
    if (!ok) {
        std::cerr << "test_put_uLong_max: failed to capture output\n";
        return false;
    }
    return std::memcmp(out, expected, 4) == 0;
}

/*
  Test 3: Specific known value to verify exact byte sequence.
  Example: 0x11223344 -> 0x11 0x22 0x33 0x44
*/
static bool test_put_uLong_specific() {
    unsigned char out[4];
    const unsigned char expected[4] = {0x11, 0x22, 0x33, 0x44};
    bool ok = capture_put_uLong((uLong)0x11223344, out);
    if (!ok) {
        std::cerr << "test_put_uLong_specific: failed to capture output\n";
        return false;
    }
    return std::memcmp(out, expected, 4) == 0;
}

/*
  Test 4: Endianness check with value 0x01020304 to ensure big-endian order.
  Expected: 0x01 0x02 0x03 0x04
*/
static bool test_put_uLong_endianness() {
    unsigned char out[4];
    const unsigned char expected[4] = {0x01, 0x02, 0x03, 0x04};
    bool ok = capture_put_uLong((uLong)0x01020304, out);
    if (!ok) {
        std::cerr << "test_put_uLong_endianness: failed to capture output\n";
        return false;
    }
    return std::memcmp(out, expected, 4) == 0;
}

/*
  Test 5: Sequence of two calls to ensure subsequent outputs are correctly produced in order.
  We capture two values in separate calls to ensure no cross-test state leaks.
*/
static bool test_put_uLong_multiple_calls_sequence() {
    unsigned char first[4];
    unsigned char second[4];
    const unsigned char expected_first[4]  = {0x01, 0x02, 0x03, 0x04};
    const unsigned char expected_second[4] = {0xAA, 0xBB, 0xCC, 0xDD};

    bool ok1 = capture_put_uLong((uLong)0x01020304, first);
    bool ok2 = capture_put_uLong((uLong)0xAABBCCDD, second);

    if (!ok1 || !ok2) {
        std::cerr << "test_put_uLong_multiple_calls_sequence: failed to capture output\n";
        return false;
    }

    return (std::memcmp(first, expected_first, 4) == 0) && (std::memcmp(second, expected_second, 4) == 0);
}

// Simple test runner
int main() {
    struct TestEntry {
        const char* name;
        std::function<bool()> fn;
    };

    std::vector<TestEntry> tests = {
        {"test_put_uLong_zero", test_put_uLong_zero},
        {"test_put_uLong_max",  test_put_uLong_max},
        {"test_put_uLong_specific", test_put_uLong_specific},
        {"test_put_uLong_endianness", test_put_uLong_endianness},
        {"test_put_uLong_multiple_calls_sequence", test_put_uLong_multiple_calls_sequence}
    };

    int passed = 0;
    int total  = static_cast<int>(tests.size());

    std::cout << "Running " << total << " test(s) for put_uLong...\n";

    for (const auto& t : tests) {
        bool result = t.fn();
        std::cout << "[" << (result ? "PASS" : "FAIL") << "] "
                  << t.name << "\n";
        if (result) ++passed;
    }

    std::cout << "Summary: " << passed << " / " << total << " passed.\n";
    return (passed == total) ? 0 : 1;
}