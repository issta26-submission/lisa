// Unit test suite for the focal function read_one_file from checksum-icc.c
// This test suite is implemented in C++11 without Google Test, using a small
// custom assertion harness to maximize test execution coverage.
//
// Summary of strategy:
// - Tests are written to verify the observable behavior of read_one_file without
//   requiring access to private internals.
// - We capture stdout output by redirecting it through a POSIX pipe while the
//   function is executing. This allows us to inspect the generated PNG_ICC_CHECKSUM
//   line and the included length and name parameters.
// - We exercise at least two input lengths to touch different code paths, including
//   the branch where length may exceed the 132-byte header array.
// - We intentionally avoid asserting on exact Adler/Crc values, instead validating
//   presence of key tokens in the output to ensure correct flow and formatting.
// - The tests rely on the external C function prototype:
//     extern "C" int read_one_file(FILE*, const char*);
//
// Candidate Keywords (Step 1): adler32, crc32, header, length, a32, c32, PNG_ICC_CHECKSUM,
// PNG_MD5, u16, u32, getc, EOF, ferror, name, printf, 132-byte header, 64/16-bit
// field extraction macros, NULL, NULL-terminated strings, File I/O, stdout capture.
//
// Note: This file does not require GTest. Compile with something like:
//   g++ -std=c++11 test_checksum_icc.cpp -lz -o test_checksum_icc
// Then run ./test_checksum_icc

#include <unistd.h>
#include <vector>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <zlib.h>
#include <errno.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <cstdio>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>


extern "C" int read_one_file(FILE* ip, const char* name); // Focal function prototype

// Simple non-terminating test assertion harness
static int g_test_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "TEST FAILURE: " << msg << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

// Helper: Run read_one_file on in-memory data written to a temp file.
// Captures stdout output via a pipe and returns the function's return value.
// The output is returned as a string, and the actual file name used is managed internally.
struct TestRunResult {
    int ret;
    std::string output;
};

// Write data to a temporary file using mkstemp, return path and fd closed after write.
static std::string write_temp_file(const std::vector<unsigned char>& data) {
    char tmp_template[] = "/tmp/checksum_icc_test_XXXXXX";
    int fd = mkstemp(tmp_template);
    if (fd == -1) {
        perror("mkstemp");
        return "";
    }
    // Write data to the file
    if (!data.empty()) {
        ssize_t written = 0;
        while (written < (ssize_t)data.size()) {
            ssize_t w = write(fd, data.data() + written, data.size() - written);
            if (w <= 0) {
                // On error, cleanup and bail
                break;
            }
            written += w;
        }
    }
    close(fd);
    return std::string(tmp_template);
}

// Capture stdout produced by read_one_file when reading from the provided file path.
static TestRunResult run_read_one_file_with_content(const std::vector<unsigned char>& data,
                                                  const std::string& name) {
    TestRunResult res;
    res.ret = -1;
    res.output.clear();

    // Prepare temp file containing the test data
    std::string tmp_path = write_temp_file(data);
    if (tmp_path.empty()) {
        std::cerr << "Failed to create temporary file for test." << std::endl;
        g_test_failures++;
        return res;
    }

    FILE* ip = std::fopen(tmp_path.c_str(), "rb");
    if (!ip) {
        perror("fopen");
        unlink(tmp_path.c_str());
        g_test_failures++;
        return res;
    }

    // Create a pipe to capture stdout
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        std::fclose(ip);
        unlink(tmp_path.c_str());
        g_test_failures++;
        return res;
    }

    // Redirect stdout to the pipe's write end
    fflush(stdout);
    int saved_stdout_fd = dup(fileno(stdout));
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        perror("dup2 stdout");
        std::fclose(ip);
        close(pipefd[0]);
        close(pipefd[1]);
        unlink(tmp_path.c_str());
        g_test_failures++;
        return res;
    }
    close(pipefd[1]); // The write end is now duplicated to stdout

    // Execute the focal function
    res.ret = read_one_file(ip, name.c_str());

    // Restore stdout
    fflush(stdout);
    if (dup2(saved_stdout_fd, fileno(stdout)) == -1) {
        perror("dup2 restore stdout");
        // Don't fail test solely on restoration; continue to read output
    }
    close(saved_stdout_fd);

    // Close inputs and restore resources
    std::fclose(ip);

    // Read the captured output from the pipe
    char buffer[4096];
    ssize_t nread;
    std::ostringstream oss;
    while ((nread = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        oss.write(buffer, static_cast<std::size_t>(nread));
    }
    close(pipefd[0]);
    res.output = oss.str();

    // Cleanup temporary file
    unlink(tmp_path.c_str());

    return res;
}

// Test 1: Valid input with length 100 bytes
static void test_read_one_file_length_100() {
    // Prepare 100 bytes of data (e.g., all 'A')
    const size_t length = 100;
    std::vector<unsigned char> data(length, static_cast<unsigned char>('A'));

    TestRunResult r = run_read_one_file_with_content(data, "test1.png");

    // Explanatory test comments:
    // - Expect a successful read: return value should be 1.
    // - Expect PNG_ICC_CHECKSUM line to be printed as part of stdout.
    // - The output should include the provided file name and the length value "100".
    TEST_ASSERT(r.ret == 1, "read_one_file should return 1 for valid input (100 bytes).");
    TEST_ASSERT(!r.output.empty(), "read_one_file should produce stdout output for valid input (100 bytes).");
    TEST_ASSERT(r.output.find("PNG_ICC_CHECKSUM(") != std::string::npos,
                "stdout should contain the PNG_ICC_CHECKSUM prefix.");
    TEST_ASSERT(r.output.find("test1.png") != std::string::npos,
                "stdout should contain the provided file name 'test1.png'.");
    TEST_ASSERT(r.output.find(", 100, \"") != std::string::npos,
                "stdout should include the length indicator '100'.");
}

// Test 2: Valid input with length 150 bytes (exercises header path and longer data)
static void test_read_one_file_length_150() {
    const size_t length = 150;
    std::vector<unsigned char> data(length, static_cast<unsigned char>(0x42)); // arbitrary non-zero data

    TestRunResult r = run_read_one_file_with_content(data, "test2.png");

    // Explanatory test comments:
    // - Ensure function handles length surpassing the 132-byte header storage.
    // - Expect the same high-level behavior: return 1 and proper output including length and name.
    TEST_ASSERT(r.ret == 1, "read_one_file should return 1 for valid input (150 bytes).");
    TEST_ASSERT(!r.output.empty(), "read_one_file should produce stdout output for valid input (150 bytes).");
    TEST_ASSERT(r.output.find("PNG_ICC_CHECKSUM(") != std::string::npos,
                "stdout should contain the PNG_ICC_CHECKSUM prefix (150 bytes).");
    TEST_ASSERT(r.output.find("test2.png") != std::string::npos,
                "stdout should contain the provided file name 'test2.png' (150 bytes).");
    TEST_ASSERT(r.output.find(", 150, \"") != std::string::npos,
                "stdout should include the length indicator '150'.");
}

// Test 3 (optional): Empty input file to ensure graceful handling.
// This test validates that the function still prints a line and returns 1
// or any defined behavior for zero-length input, without crashing.
static void test_read_one_file_length_0() {
    const size_t length = 0;
    std::vector<unsigned char> data(length, 0);

    TestRunResult r = run_read_one_file_with_content(data, "empty.png");

    // Behavior note:
    // The original code does not explicitly guard against zero-length header reads,
    // but it should still return and print the checksum line (with possibly
    // uninitialized header values). We only test that the function returns an int
    // and prints something including the name.
    TEST_ASSERT(r.ret == 1, "read_one_file should return a value (even for 0-length) [empty file].");
    TEST_ASSERT(r.output.find("empty.png") != std::string::npos,
                "stdout should contain the provided file name 'empty.png'.");
}

// Main test runner
int main() {
    std::cout << "Starting read_one_file unit tests (non-GTest, POSIX stdout capture)" << std::endl;

    test_read_one_file_length_100();
    test_read_one_file_length_150();
    // Uncomment the following line to enable the 0-length test if desired
    // test_read_one_file_length_0();

    if (g_test_failures > 0) {
        std::cerr << g_test_failures << " test(s) FAILED." << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}