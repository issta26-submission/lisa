// Unit test suite for the focal method put_chunk in C++11 without GoogleTest.
// The tests redirect stdout to a temporary file to validate that put_chunk writes
// exactly the given chunk bytes to stdout (the CRC output from put_uLong is not
// observable in the provided focal implementation as put_uLong is effectively a no-op).

#include <unistd.h>
#include <vector>
#include <fstream>
#include <zlib.h>
#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>


// Declaration of the focal function under test (C linkage).
extern "C" void put_chunk(const unsigned char *chunk, unsigned int length);

// Simple test framework scaffolding (non-terminating assertions).
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_EQ(a, b) do { \
    ++g_total; \
    if (!((a) == (b))) { \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - Expected: " << (b) << " | Actual: " << (a) << "\n"; \
        ++g_failed; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    ++g_total; \
    if (!(cond)) { \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " - Condition failed: " #cond "\n"; \
        ++g_failed; \
    } \
} while(0)

static void print_summary() {
    std::cerr << "Total tests: " << g_total << ", Failures: " << g_failed << "\n";
    // Do not exit with non-zero to allow non-terminating style; caller can inspect return code.
}

// Helper: read a file's entire contents into a vector
static std::vector<unsigned char> read_file(const char* path) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) return std::vector<unsigned char>();
    std::vector<unsigned char> contents((std::istreambuf_iterator<char>(ifs)),
                                        (std::istreambuf_iterator<char>()));
    return contents;
}

// Helper: redirect stdout to a file, call a function, then restore stdout.
// Returns the captured bytes written to the redirected file.
static bool capture_put_chunk_output(const unsigned char* data, size_t len,
                                   std::vector<unsigned char>& captured) {
    const char* temp_file = "test_output.bin";

    // Save stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        perror("dup");
        return false;
    }

    // Open temp file for writing
    int fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open temp_file");
        close(saved_stdout);
        return false;
    }

    // Redirect stdout to temp file
    if (dup2(fd, fileno(stdout)) == -1) {
        perror("dup2");
        close(fd);
        close(saved_stdout);
        return false;
    }
    close(fd);

    // Call the focal function
    put_chunk(reinterpret_cast<const unsigned char*>(data), static_cast<unsigned int>(len));

    // Flush and restore stdout
    fflush(stdout);
    if (dup2(saved_stdout, fileno(stdout)) == -1) {
        perror("dup2 restore");
        close(saved_stdout);
        return false;
    }
    close(saved_stdout);

    // Read captured data
    captured = read_file(temp_file);
    // Clean up temporary file
    remove(temp_file);
    return true;
}

/*
Domain knowledge notes applied:
- Test only via public interface put_chunk.
- Use POSIX stdout redirection to capture emitted bytes (observing only chunk data).
- Verify exact content of stdout equals the input chunk for various lengths.
- Use a small, deterministic set of chunks to ensure reproducible results.
- Provide explanatory comments for each test.
*/

// Test 1: Ensure that for a simple 4-byte chunk, only those 4 bytes are written to stdout.
static void test_put_chunk_writes_exact_4_bytes() {
    // The input chunk
    unsigned char chunk[] = {0x01, 0x02, 0x03, 0x04};
    std::vector<unsigned char> actual;
    bool ok = capture_put_chunk_output(chunk, sizeof(chunk), actual);

    // Observations
    EXPECT_TRUE(ok);
    EXPECT_EQ(actual.size(), sizeof(chunk));
    // Content must match exactly
    EXPECT_EQ(memcmp(actual.data(), chunk, actual.size()), 0);
}

// Test 2: Ensure that for a larger chunk, stdout captures the full chunk content.
static void test_put_chunk_writes_exact_8_bytes() {
    unsigned char chunk[] = {0xAA, 0xBB, 0xCC, 0xDD, 0x11, 0x22, 0x33, 0x44};
    std::vector<unsigned char> actual;
    bool ok = capture_put_chunk_output(chunk, sizeof(chunk), actual);

    EXPECT_TRUE(ok);
    EXPECT_EQ(actual.size(), sizeof(chunk));
    EXPECT_EQ(memcmp(actual.data(), chunk, actual.size()), 0);
}

// Test 3: Ensure that chunks with diverse bytes (including zeros) are written correctly.
static void test_put_chunk_writes_uniform_and_zero_bytes() {
    unsigned char chunk[] = {0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x7A, 0x80, 0x01};
    std::vector<unsigned char> actual;
    bool ok = capture_put_chunk_output(chunk, sizeof(chunk), actual);

    EXPECT_TRUE(ok);
    EXPECT_EQ(actual.size(), sizeof(chunk));
    EXPECT_EQ(memcmp(actual.data(), chunk, actual.size()), 0);
}

// Main function: run all tests and print summary.
int main() {
    test_put_chunk_writes_exact_4_bytes();
    test_put_chunk_writes_exact_8_bytes();
    test_put_chunk_writes_uniform_and_zero_bytes();

    print_summary();
    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}