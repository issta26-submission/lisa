// test_fakepng_suite.cpp
// A lightweight, GTest-free unit test suite for the focal method "main" in fakepng.c-like code.
// The test-suite runs the external binary ./fakepng, captures its stdout, and validates
// the initial PNG-like header output produced before the infinite loop begins.
// Notes:
// - This test assumes a POSIX-like environment (fork, exec, pipe, etc.).
// - The test focuses on initial output structure (signature and IHDR chunk) rather than
//   the infinite loop behavior, which cannot terminate in a straightforward in-process test.
// - The test uses a small in-process test runner (no external test framework) as required.
//
// How to build (example):
// g++ -std=c++11 -O2 -Wall test_fakepng_suite.cpp -o test_fakepng_suite
// Ensure there is a compiled ./fakepng binary in the same directory as the test is run from.

#include <cstdint>
#include <unistd.h>
#include <cerrno>
#include <vector>
#include <chrono>
#include <signal.h>
#include <string>
#include <zlib.h>
#include <thread>
#include <functional>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>





// Simple test framework (no GTest)
struct TestCase {
    std::string name;
    std::function<bool()> func;
};

static const unsigned char PNG_SIGNATURE[8] = {
    0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A
};

// Helper: read exactly 'n' bytes from fd with a timeout (milliseconds)
static bool read_exact_with_timeout(int fd, unsigned char* buf, size_t n, int timeout_ms) {
    using clock = std::chrono::steady_clock;
    auto end_time = clock::now() + std::chrono::milliseconds(timeout_ms);

    // Set non-blocking mode for the read
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags != -1) {
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }

    size_t total = 0;
    while (total < n) {
        ssize_t r = read(fd, buf + total, n - total);
        if (r > 0) {
            total += static_cast<size_t>(r);
            continue;
        } else if (r == 0) {
            // EOF
            break;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // No data yet, wait a bit
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            } else {
                // Real error
                break;
            }
        }
        if (std::chrono::steady_clock::now() >= end_time) {
            break;
        }
    }

    // Restore blocking mode
    if (flags != -1) {
        fcntl(fd, F_SETFL, flags);
    }

    return total == n;
}

// Test 1: Validate that ./fakepng starts with the PNG signature and IHDR chunk header.
// This test forks a child process to run the binary, captures stdout, and inspects
// the first 16 bytes: 8-byte signature, 4-byte length (big-endian), and 4-byte chunk type "IHDR".
static bool test_signature_and_ihdr_output() {
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        std::cerr << "[ERR] pipe creation failed: " << std::strerror(errno) << "\n";
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "[ERR] fork failed: " << std::strerror(errno) << "\n";
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return false;
    }

    if (pid == 0) {
        // Child process: redirect stdout to pipe and exec the fakepng binary
        dup2(pipe_fd[1], STDOUT_FILENO);
        // Close unused descriptors
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        // Exec the target binary. Adjust path if needed.
        execl("./fakepng", "./fakepng", (char*)nullptr);

        // If exec fails
        _exit(1);
    }

    // Parent process
    close(pipe_fd[1]); // Close write end; we only read
    unsigned char buffer[16];
    bool read_ok = read_exact_with_timeout(pipe_fd[0], buffer, sizeof(buffer), 1000); // 1 second timeout
    // Regardless of read result, terminate the child (the binary will continue in infinite loop)
    kill(pid, SIGKILL);
    int status = 0;
    waitpid(pid, &status, 0);

    close(pipe_fd[0]);

    if (!read_ok) {
        std::cout << "[TEST FAIL] test_signature_and_ihdr_output: failed to read 16 bytes from stdout within timeout.\n";
        return false;
    }

    // Check first 8 bytes signature
    if (std::memcmp(buffer, PNG_SIGNATURE, 8) != 0) {
        std::cout << "[TEST FAIL] test_signature_and_ihdr_output: PNG signature mismatch.\n";
        return false;
    }

    // Check next 4 bytes: length (big-endian). Expect 13 (IHDR data length in PNG)
    uint32_t len = (static_cast<uint32_t>(buffer[8]) << 24) |
                   (static_cast<uint32_t>(buffer[9]) << 16) |
                   (static_cast<uint32_t>(buffer[10]) << 8) |
                   (static_cast<uint32_t>(buffer[11]) );

    if (len != 13u) {
        std::cout << "[TEST FAIL] test_signature_and_ihdr_output: IHDR length expected 13, got " << len << ".\n";
        return false;
    }

    // Check next 4 bytes: chunk type "IHDR"
    if (std::memcmp(buffer + 12, "IHDR", 4) != 0) {
        std::cout << "[TEST FAIL] test_signature_and_ihdr_output: IHDR chunk type not found where expected.\n";
        return false;
    }

    std::cout << "[TEST PASS] test_signature_and_ihdr_output: PNG signature, IHDR length and type observed as expected.\n";
    return true;
}

// Test 2: Validate that the first chunk is IHDR and the lengths are encoded in big-endian format.
// This test can reuse the same execution path as Test 1 but emphasizes the correctness
// of the byte-order encoding for the length field and the presence of IHDR after the length.
static bool test_ihdr_header_big_endian() {
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        std::cerr << "[ERR] pipe creation failed: " << std::strerror(errno) << "\n";
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "[ERR] fork failed: " << std::strerror(errno) << "\n";
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return false;
    }

    if (pid == 0) {
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        execl("./fakepng", "./fakepng", (char*)nullptr);
        _exit(1);
    }

    close(pipe_fd[1]);
    unsigned char buffer[16];
    bool read_ok = read_exact_with_timeout(pipe_fd[0], buffer, sizeof(buffer), 1000);
    kill(pid, SIGKILL);
    int status = 0;
    waitpid(pid, &status, 0);
    close(pipe_fd[0]);

    if (!read_ok) {
        std::cout << "[TEST FAIL] test_ihdr_header_big_endian: failed to read 16 bytes for endian test.\n";
        return false;
    }

    // Reuse checks from Test 1 for consistency
    // 8-byte signature
    if (std::memcmp(buffer, PNG_SIGNATURE, 8) != 0) {
        std::cout << "[TEST FAIL] test_ihdr_header_big_endian: PNG signature mismatch.\n";
        return false;
    }

    uint32_t len = (static_cast<uint32_t>(buffer[8]) << 24) |
                   (static_cast<uint32_t>(buffer[9]) << 16) |
                   (static_cast<uint32_t>(buffer[10]) << 8) |
                   (static_cast<uint32_t>(buffer[11]) );

    if (len != 13u) {
        std::cout << "[TEST FAIL] test_ihdr_header_big_endian: IHDR length not encoded as 13 in big-endian (got " << len << ").\n";
        return false;
    }

    if (std::memcmp(buffer + 12, "IHDR", 4) != 0) {
        std::cout << "[TEST FAIL] test_ihdr_header_big_endian: IHDR type not found (expected 'IHDR').\n";
        return false;
    }

    std::cout << "[TEST PASS] test_ihdr_header_big_endian: Endian encoding validated for IHDR length and type.\n";
    return true;
}

// Runner
int main() {
    std::vector<TestCase> tests = {
        {"test_signature_and_ihdr_output", test_signature_and_ihdr_output},
        {"test_ihdr_header_big_endian", test_ihdr_header_big_endian}
    };

    int passed = 0;
    int total = static_cast<int>(tests.size());

    std::cout << "Running " << total << " test(s) for focal main() of fakepng.c-like code...\n";

    for (auto &t : tests) {
        bool ok = false;
        try {
            ok = t.func();
        } catch (...) {
            std::cout << "[TEST EXCEPTION] " << t.name << ": exception during test execution.\n";
            ok = false;
        }
        if (ok) ++passed;
    }

    std::cout << "Test results: " << passed << "/" << total << " passed.\n";

    // Exit status reflects test success
    return (passed == total) ? 0 : 1;
}