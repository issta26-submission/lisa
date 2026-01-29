#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>


// Forward declaration of the focal function.
// We assume the function has C linkage when compiled in a C++ test environment.
extern "C" void gzinit(unsigned long *crc, unsigned long *tot, FILE *out);

// Simple non-terminating test harness (no GTest dependency)
static int g_failures = 0;

template <typename T>
static void log_failure(const char* test_name, const char* message, const T& got, const T& expected) {
    std::cerr << "[FAILED] " << test_name << ": " << message
              << " | got: " << got << " | expected: " << expected << std::endl;
    ++g_failures;
}

static void expect_eq_u64(const char* test_name, const char* expr, unsigned long a, unsigned long b) {
    if (a != b) {
        char msg[256];
        std::snprintf(msg, sizeof(msg), "%s (%s) mismatch", test_name, expr);
        log_failure(test_name, "unequal values", a, b);
    }
}

static void expect_true(const char* test_name, const char* expr, bool cond) {
    if (!cond) {
        log_failure(test_name, "condition is false", 0u, 1u);
    }
}

static bool test_gzinit_basic_header_and_crc_tot() {
    // Test that gzinit writes the correct 10-byte gzip header and initializes crc/tot.
    const unsigned char expected_header[10] = {
        0x1f, 0x8b, 0x08, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xff
    };

    unsigned long crc = 12345; // non-zero initial to ensure reset happens
    unsigned long tot = 98765;
    FILE *out = tmpfile();
    if (!out) {
        std::cerr << "[ERROR] Could not create temporary file for test_gzinit_basic_header_and_crc_tot" << std::endl;
        g_failures++;
        return false;
    }

    gzinit(&crc, &tot, out);

    // Rewind and read the header
    fseek(out, 0, SEEK_SET);
    unsigned char buf[10];
    size_t read = fread(buf, 1, 10, out);
    fclose(out);

    bool ok = true;
    if (read != 10) {
        log_failure("test_gzinit_basic_header_and_crc_tot", "header length != 10", (unsigned long)read, 10ul);
        ok = false;
    } else {
        if (std::memcmp(buf, expected_header, 10) != 0) {
            ok = false;
            std::cerr << "[FAILED] test_gzinit_basic_header_and_crc_tot: header bytes do not match expected gzip header." << std::endl;
        }
    }

    // crc and tot should be reset to 0
    expect_eq_u64("test_gzinit_basic_header_and_crc_tot", "crc == 0", crc, 0ul);
    expect_eq_u64("test_gzinit_basic_header_and_crc_tot", "tot == 0", tot, 0ul);

    return ok && g_failures == 0;
}

static bool test_gzinit_twice_appends_headers() {
    // Test that calling gzinit twice appends two 10-byte headers and both crc/tot reset properly.
    const unsigned char header[10] = {
        0x1f, 0x8b, 0x08, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xff
    };

    unsigned long crc1 = 0, tot1 = 0;
    unsigned long crc2 = 0, tot2 = 0;
    FILE *out = tmpfile();
    if (!out) {
        std::cerr << "[ERROR] Could not create temporary file for test_gzinit_twice_appends_headers" << std::endl;
        g_failures++;
        return false;
    }

    gzinit(&crc1, &tot1, out);
    gzinit(&crc2, &tot2, out);
    fseek(out, 0, SEEK_SET);

    unsigned char buf[20];
    size_t read = fread(buf, 1, 20, out);
    fclose(out);

    bool ok = true;
    if (read != 20) {
        log_failure("test_gzinit_twice_appends_headers", "combined header length != 20", (unsigned long)read, 20ul);
        ok = false;
    } else {
        if (std::memcmp(buf, header, 10) != 0) {
            log_failure("test_gzinit_twice_appends_headers", "first header bytes mismatch", 0ul, 0ul);
            ok = false;
        }
        if (std::memcmp(buf + 10, header, 10) != 0) {
            log_failure("test_gzinit_twice_appends_headers", "second header bytes mismatch", 0ul, 0ul);
            ok = false;
        }
    }

    expect_eq_u64("test_gzinit_twice_appends_headers", "crc1 == 0", crc1, 0ul);
    expect_eq_u64("test_gzinit_twice_appends_headers", "tot1 == 0", tot1, 0ul);
    expect_eq_u64("test_gzinit_twice_appends_headers", "crc2 == 0", crc2, 0ul);
    expect_eq_u64("test_gzinit_twice_appends_headers", "tot2 == 0", tot2, 0ul);

    return ok && g_failures == 0;
}

int main() {
    std::cout << "Starting gzinit unit tests (C linkage)..." << std::endl;

    bool t1 = test_gzinit_basic_header_and_crc_tot();
    bool t2 = test_gzinit_twice_appends_headers();

    int total_tests = 2;
    int passed = (t1 ? 1 : 0) + (t2 ? 1 : 0);
    if (g_failures == 0) {
        std::cout << "[OK] All tests passed (" << passed << "/" << total_tests << ")." << std::endl;
    } else {
        std::cerr << "[ERROR] Some tests failed: " << g_failures << " failure(s)." << std::endl;
        return 1;
    }
    return (passed == total_tests) ? 0 : 1;
}