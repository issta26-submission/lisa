// C++11-based unit test suite for the focal C function readin from gzappend.c
// This test suite uses a lightweight, non-GTest framework with non-terminating
// assertions (EXPECT_* macros) and a simple main-based test runner.
// Note: We assume the build environment provides the function with C linkage:
//      int readin(struct file *in);
// and that the actual 'file' type used by readin is compatible with the
// struct defined here (struct file { int fd; char *buf; unsigned size; char *name; unsigned left; char *next; }).

#include <cstdio>
#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// Forward declaration of the C function under test (readin) with the expected C linkage.
extern "C" {
    // The real gzappend.c uses a typedef'd or named type 'file'. We declare a matching struct here.
    struct file;
    int readin(struct file *in);
}

// Minimal user-space representation of the C 'file' struct.
// This mirrors the fields accessed/modified by readin in gzappend.c.
struct file {
    int fd;
    char *buf;
    unsigned size;
    char *name;
    unsigned left;
    char *next;
};

// Lightweight test framework (non-terminating assertions)
namespace TestFramework {
    int g_failures = 0;

    void log_failure(const char* test_name, const char* file, int line, const char* msg) {
        fprintf(stderr, "Test fail in '%s' at %s:%d: %s\n", test_name, file, line, msg);
        ++g_failures;
    }

    // Assertions
    #define EXPECT_TRUE(cond) \
        do { if(!(cond)) TestFramework::log_failure(__func__, __FILE__, __LINE__, "expected true"); } while(0)

    #define EXPECT_EQ_INT(a, b) \
        do { if(((int)(a)) != ((int)(b))) { char _buf[128]; snprintf(_buf, sizeof(_buf), "expected %d, got %d", (int)(b), (int)(a)); TestFramework::log_failure(__func__, __FILE__, __LINE__, _buf); } } while(0)

    #define EXPECT_MEMEQ(a, b, n) \
        do { if(memcmp((a), (b), (n)) != 0) { TestFramework::log_failure(__func__, __FILE__, __LINE__, "memory mismatch"); } } while(0)

    // Simple per-test timing/printing
    void announce(const char* name) {
        printf("=== Running %s ===\n", name);
        fflush(stdout);
    }

    bool all_passed() {
        return g_failures == 0;
    }
}

// Helper to write data into a pipe and provide FD to readin
static inline void write_all(int fd, const char* data, size_t n) {
    size_t written = 0;
    while (written < n) {
        ssize_t w = write(fd, data + written, n - written);
        if (w <= 0) break;
        written += (size_t)w;
    }
}

// Test 1: Reading a small payload into a buffer (less than or equal to 1<<size)
bool test_readin_small_payload() {
    using namespace TestFramework;
    announce("test_readin_small_payload");

    int p[2];
    if (pipe(p) != 0) {
        log_failure(__func__, __FILE__, __LINE__, "failed to create pipe");
        return false;
    }

    // Prepare data to be read: "Hello"
    const char* payload = "Hello";
    write_all(p[1], payload, 5);
    // Close write end to signal EOF after data
    close(p[1]);

    // Prepare 'file' structure expected by readin
    file in;
    in.fd = p[0];
    in.buf = (char*)malloc(16); // ample space for the test
    in.size = 3; // 1 << 3 = 8 bytes max to read
    in.name = (char*)"testin_small";
    in.left = 0;
    in.next = nullptr;

    int len = readin(&in);

    // Assertions
    EXPECT_EQ_INT(len, 5);
    EXPECT_EQ_INT(in.left, 5);
    EXPECT_TRUE(in.next == in.buf);
    EXPECT_TRUE(strncmp(in.buf, payload, 5) == 0);
    // Ensure the rest of the buffer remains unaffected beyond read bytes
    // (Optional: not strictly necessary for coverage, but good practice)
    in.buf[5] = '\0';
    // Cleanup
    free(in.buf);
    close(p[0]);

    return TestFramework::g_failures == 0;
}

// Test 2: Reading more bytes than the buffer window (should cap at 1<<size)
bool test_readin_over_buffer_full() {
    using namespace TestFramework;
    announce("test_readin_over_buffer_full");

    int p[2];
    if (pipe(p) != 0) {
        log_failure(__func__, __FILE__, __LINE__, "failed to create pipe");
        return false;
    }

    // Data longer than 1<<size (8 bytes)
    const char* payload = "0123456789"; // 10 bytes
    write_all(p[1], payload, 10);
    // Close write end
    close(p[1]);

    file in;
    in.fd = p[0];
    in.buf = (char*)malloc(16);
    in.size = 3; // 8 bytes max
    in.name = (char*)"testin_over";
    in.left = 0;
    in.next = nullptr;

    int len = readin(&in);

    // We expect 8 bytes read
    EXPECT_EQ_INT(len, 8);
    EXPECT_EQ_INT(in.left, 8);
    EXPECT_TRUE(in.next == in.buf);
    // First 8 bytes should be "01234567"
    char expect[9] = "01234567";
    expect[8] = '\0';
    in.buf[8] = '\0';
    EXPECT_MEMEQ(in.buf, expect, 8);

    free(in.buf);
    close(p[0]);
    return TestFramework::g_failures == 0;
}

// Test 3: EOF without data (no bytes read, len == 0, left == 0)
bool test_readin_eof_without_data() {
    using namespace TestFramework;
    announce("test_readin_eof_without_data");

    int p[2];
    if (pipe(p) != 0) {
        log_failure(__func__, __FILE__, __LINE__, "failed to create pipe");
        return false;
    }

    // Immediately close write end to simulate EOF with no data
    close(p[1]);

    file in;
    in.fd = p[0];
    in.buf = (char*)malloc(4);
    in.size = 2; // 4 bytes buffer
    in.name = (char*)"testin_eof";
    in.left = 0;
    in.next = nullptr;

    int len = readin(&in);

    // Depending on system behavior, read may return 0 on EOF
    EXPECT_EQ_INT(len, 0);
    EXPECT_EQ_INT(in.left, 0);
    EXPECT_TRUE(in.next == in.buf); // In this edge, next is still assigned to buf

    free(in.buf);
    close(p[0]);
    return TestFramework::g_failures == 0;
}

int main() {
    using namespace TestFramework;

    // Run tests
    bool t1 = test_readin_small_payload();
    bool t2 = test_readin_over_buffer_full();
    bool t3 = test_readin_eof_without_data();

    // Summary
    if (g_failures == 0) {
        printf("All tests passed.\n");
    } else {
        printf("Total failures: %d\n", g_failures);
    }

    // Return non-zero if any test failed
    return (g_failures == 0) ? 0 : 1;
}