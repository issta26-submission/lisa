// This test suite exercises the focal method log_head from gzlog.c by embedding
// the C implementation into a single translation unit. It uses the public
// definitions from gzlog.h for struct log and header constants (HEAD, EXTRA).
// The tests call the static log_head function directly (made visible by including
// gzlog.c in the same translation unit). No Google Test is used; a small
// C++ test harness is implemented with simple boolean checks and descriptive
// comments.
//
// Build note (example):
// g++ -std=c++11 -I. test_gzlog_head.cpp gzlog.c -o test_gzlog_head
//
// The tests rely on the existence of:
// - struct log (defined in gzlog.h)
// - HEAD and EXTRA macros (defined in gzlog.h)
// - log_gzhead symbol (extern unsigned char log_gzhead[];). If not exposed,
//   this test will still compile but the success-path test will adapt by using
//   whatever log_gzhead provides (we declare extern here to attempt access).

#include <cstdio>
#include <cstring>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <gzlog.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <zlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <cstdlib>


extern "C" {
}

// If log_gzhead is defined in the C code, expose it for use in constructing a valid header.
extern "C" unsigned char log_gzhead[];

extern "C" int log_head(struct log *log); // forward-declare for use in tests (static in gzlog.c)

static int test_log_head_bad_fd() {
    // Test: when fd is invalid (-1), log_head should return -1 immediately.
    struct log l;
    l.fd = -1; // invalid descriptor

    int op = log_head(&l);
    if (op != -1) {
        printf("test_log_head_bad_fd: expected -1, got %d\n", op);
        return 0;
    }
    printf("test_log_head_bad_fd: passed\n");
    return 1;
}

static int test_log_head_insufficient_header() {
    // Test: create a temporary file with 0 bytes (insufficient header).
    // Expect log_head to return -1 due to read() not returning HEAD+EXTRA bytes.
    char path[] = "/tmp/gzlog_test_insufficient_XXXXXX";
    int fd = mkstemp(path);
    if (fd < 0) {
        perror("mkstemp");
        return 0;
    }
    // Do not write anything; leave file as empty.
    close(fd);

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("open");
        unlink(path);
        return 0;
    }

    struct log l;
    l.fd = fd;

    int op = log_head(&l);
    if (op != -1) {
        printf("test_log_head_insufficient_header: expected -1, got %d\n", op);
        close(fd);
        unlink(path);
        return 0;
    }

    close(fd);
    unlink(path);
    printf("test_log_head_insufficient_header: passed\n");
    return 1;
}

static int test_log_head_bad_header_mismatch() {
    // Test: create a header where the first HEAD bytes do not match log_gzhead.
    // This should fail at memcmp and return -1.
    size_t header_size = HEAD + EXTRA;
    std::vector<unsigned char> buf(header_size, 0);

    // Copy something other than log_gzhead into the first HEAD bytes to ensure mismatch.
    // We try to intentionally mismatch by filling with 0xFF.
    for (size_t i = 0; i < HEAD; ++i) buf[i] = 0xFF;

    // Create a temp file and write the constructed header
    char path[] = "/tmp/gzlog_test_bad_header_XXXXXX";
    int fd = mkstemp(path);
    if (fd < 0) {
        perror("mkstemp");
        return 0;
    }

    // Write the header
    if (write(fd, buf.data(), header_size) != (ssize_t)header_size) {
        perror("write header");
        close(fd);
        unlink(path);
        return 0;
    }
    // Reset file descriptor position for reading
    lseek(fd, 0, SEEK_SET);

    struct log l;
    l.fd = fd;

    int op = log_head(&l);
    if (op != -1) {
        printf("test_log_head_bad_header_mismatch: expected -1, got %d\n", op);
        close(fd);
        unlink(path);
        return 0;
    }

    close(fd);
    unlink(path);
    printf("test_log_head_bad_header_mismatch: passed\n");
    return 1;
}

static int write_u64_le(std::vector<unsigned char> &buf, size_t pos, uint64_t v) {
    for (int i = 0; i < 8; ++i) buf[pos + i] = (unsigned char)((v >> (8 * i)) & 0xFF);
    return 8;
}

static int write_u32_le(std::vector<unsigned char> &buf, size_t pos, uint32_t v) {
    for (int i = 0; i < 4; ++i) buf[pos + i] = (unsigned char)((v >> (8 * i)) & 0xFF);
    return 4;
}

static int write_u16_le(std::vector<unsigned char> &buf, size_t pos, uint16_t v) {
    buf[pos] = (unsigned char)(v & 0xFF);
    buf[pos + 1] = (unsigned char)((v >> 8) & 0xFF);
    return 2;
}

static int test_log_head_success() {
    // Test: construct a header that matches log_gzhead and fill fields with deterministic values.
    // Expect log_head to return op = 1 (as constructed below).
    size_t header_size = HEAD + EXTRA;
    std::vector<unsigned char> buf(header_size, 0);

    // Copy the valid log header signature
    memcpy(buf.data(), log_gzhead, HEAD);

    // Fill sample values for the header fields (little-endian)
    uint64_t first = 0x0102030405060708ULL;
    uint64_t last  = 0x1112131415161718ULL;
    uint32_t ccrc   = 0xAABBCCDD;
    uint32_t clen   = 0x11223344;
    uint32_t tcrc   = 0x55667788;
    uint32_t tlen   = 0x99AABBCC;
    uint16_t stored = 0x1234;

    size_t pos = HEAD;
    write_u64_le(buf, pos, first); pos += 8;
    write_u64_le(buf, pos, last);  pos += 8;
    write_u32_le(buf, pos, ccrc);   pos += 4;
    write_u32_le(buf, pos, clen);   pos += 4;
    write_u32_le(buf, pos, tcrc);   pos += 4;
    write_u32_le(buf, pos, tlen);   pos += 4;
    write_u16_le(buf, pos, stored); pos += 2;
    // Set op = 1 and back to 2 (for example)
    // buf[HEAD + 34] = (op << 3) | (back & 7)
    buf[HEAD + 34] = (1 << 3) | (2 & 7); // op=1, back lower 3 bits = 2

    // Ensure the rest of the buffer remains zero or as needed
    // Now write this header into a temporary file
    char path[] = "/tmp/gzlog_test_success_XXXXXX";
    int fd = mkstemp(path);
    if (fd < 0) {
        perror("mkstemp");
        return 0;
    }

    if (write(fd, buf.data(), header_size) != (ssize_t)header_size) {
        perror("write header");
        close(fd);
        unlink(path);
        return 0;
    }

    // Reposition for reading by the test function
    lseek(fd, 0, SEEK_SET);

    struct log l;
    l.fd = fd;

    int op = log_head(&l);

    // Cleanup
    close(fd);
    unlink(path);

    if (op != 1) {
        printf("test_log_head_success: expected op=1, got %d\n", op);
        return 0;
    }

    printf("test_log_head_success: passed (op=%d)\n", op);
    return 1;
}

int main() {
    // Run test suite
    int passed = 0;
    int total = 0;

    printf("Running gzlog log_head tests (C implementation loaded in TU)...\n");

    if (test_log_head_bad_fd()) ++passed;
    ++total;

    if (test_log_head_insufficient_header()) ++passed;
    ++total;

    if (test_log_head_bad_header_mismatch()) ++passed;
    ++total;

    if (test_log_head_success()) ++passed;
    ++total;

    printf("Test results: %d/%d tests passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}