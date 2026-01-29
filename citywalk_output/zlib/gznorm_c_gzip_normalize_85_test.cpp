// Minimal C++11 test suite for the focal gzip_normalize method.
// This test harness avoids Google Test and uses a small, non-terminating assertion approach.
// It targets the behavior of gzip_normalize when given an empty input, validating
// basic output structure (gzip header + trailer) and return semantics.
//
// Notes and assumptions:
// - The test uses fmemopen to create in/out FILE* streams backed by memory buffers.
// - The environment must provide fmemopen (common on Linux/glibc). If not available,
//   adapt to use a temporary file-based approach instead.
// - gzip_normalize is provided by the project under test (gznorm.c) and is linked in.
// - The test does not attempt to craft a full valid gzip member; it verifies the
//   function handles empty input gracefully and writes a canonical header and trailer.
// - The macro BYE and other internal details are encapsulated within the target code; tests
//   only rely on the public interface: gzip_normalize(FILE*, FILE*, char**).
//
// How to run (example):
//   g++ -std=c++11 -O2 -c test_gznorm.cpp
//   g++ -std=c++11 -O2 test_gznorm.o gznorm.o -lz
//   ./a.out
//
// The test will print a summary indicating passed/failed tests.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <string>
#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <cstdlib>


// Declaration of the focal function with C linkage
extern "C" int gzip_normalize(FILE *in, FILE *out, char **err);

static const size_t OUTPUT_BUFFER_SIZE = 4096;

// Simple test harness helpers
static void log_fail(const std::string& test_name, const std::string& detail) {
    std::cerr << "[FAIL] " << test_name << ": " << detail << std::endl;
}

static void log_pass(const std::string& test_name) {
    std::cout << "[PASS] " << test_name << std::endl;
}

// Test 1: Empty input should produce a valid (header + trailer) gzip-like output
// and return 0 with err == NULL.
// Validation strategy:
// - The function should not crash or error on empty input.
// - The output should start with the canonical gzip header: 0x1f, 0x8b, 0x08
// - The trailer (last 8 bytes) should be 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 (CRC and length = 0).
static bool test_empty_input_basic_structure() {
    // Prepare an empty input using fmemopen with size 0.
    // Some standard libraries allow a NULL buffer with size 0; we provide an explicit buffer for portability.
    char empty_in[1] = {0};
    FILE *fin = fmemopen(empty_in, 0, "r");
    if (!fin) {
        log_fail("test_empty_input_basic_structure", "fmemopen for input failed");
        return false;
    }

    // Prepare an output memory buffer
    unsigned char out_buf[OUTPUT_BUFFER_SIZE];
    memset(out_buf, 0, OUTPUT_BUFFER_SIZE);
    FILE *fout = fmemopen((char*)out_buf, OUTPUT_BUFFER_SIZE, "w+b");
    if (!fout) {
        fclose(fin);
        log_fail("test_empty_input_basic_structure", "fmemopen for output failed");
        return false;
    }

    char *err = NULL;
    int ret = gzip_normalize(fin, fout, &err);

    // Gather the amount of data written to output
    fflush(fout);
    long written = ftell(fout);
    if (written < 0) {
        fclose(fin);
        fclose(fout);
        log_fail("test_empty_input_basic_structure", "ftell on output failed");
        return false;
    }

    // Basic return value and error check
    bool ok = (ret == 0) && (err == NULL);
    if (!ok) {
        log_fail("test_empty_input_basic_structure",
                 "gzip_normalize did not return 0 or err was not NULL");
        // Provide diagnostic
        std::string detail = "ret=" + std::to_string(ret) + ", err=" + (err ? std::string(err) : "NULL");
        log_fail("test_empty_input_basic_structure", detail);
        // Clean up
        fclose(fin);
        fclose(fout);
        return false;
    }

    // Validate header: first three bytes should be 0x1f, 0x8b, 0x08
    bool header_ok = (written >= 3) && (out_buf[0] == 0x1F) && (out_buf[1] == 0x8B) && (out_buf[2] == 0x08);
    if (!header_ok) {
        log_fail("test_empty_input_basic_structure", "invalid gzip header bytes");
        fclose(fin);
        fclose(fout);
        return false;
    }

    // Validate trailer: last 8 bytes are CRC and length; for empty input, CRC and length are 0
    if (written < 8) {
        log_fail("test_empty_input_basic_structure", "output too short for trailer");
        fclose(fin);
        fclose(fout);
        return false;
    }
    size_t trailer_start = (size_t)written - 8;
    bool trailer_zero = true;
    for (size_t i = 0; i < 8; ++i) {
        if (out_buf[trailer_start + i] != 0) {
            trailer_zero = false;
            break;
        }
    }
    if (!trailer_zero) {
        log_fail("test_empty_input_basic_structure", "trailer bytes are not all zeros as expected for empty input");
        fclose(fin);
        fclose(fout);
        return false;
    }

    fclose(fin);
    fclose(fout);
    log_pass("test_empty_input_basic_structure");
    return true;
}

// Test 2: Repeated invocation with empty input should be deterministic and not crash.
// Validate that the second call also returns 0 and writes header + trailer.
// This helps ensure no static state persists across calls that would corrupt output.
static bool test_empty_input_twice_consistency() {
    for (int run = 1; run <= 2; ++run) {
        char empty_in[1] = {0};
        FILE *fin = fmemopen(empty_in, 0, "r");
        if (!fin) {
            log_fail("test_empty_input_twice_consistency", "fmemopen for input failed on run " + std::to_string(run));
            return false;
        }

        unsigned char out_buf[OUTPUT_BUFFER_SIZE];
        memset(out_buf, 0, OUTPUT_BUFFER_SIZE);
        FILE *fout = fmemopen((char*)out_buf, OUTPUT_BUFFER_SIZE, "w+b");
        if (!fout) {
            fclose(fin);
            log_fail("test_empty_input_twice_consistency", "fmemopen for output failed on run " + std::to_string(run));
            return false;
        }

        char *err = NULL;
        int ret = gzip_normalize(fin, fout, &err);

        fflush(fout);
        long written = ftell(fout);
        if (written < 0) {
            fclose(fin);
            fclose(fout);
            log_fail("test_empty_input_twice_consistency",
                     "ftell on output failed on run " + std::to_string(run));
            return false;
        }

        bool ok = (ret == 0) && (err == NULL);
        if (!ok) {
            log_fail("test_empty_input_twice_consistency",
                     "run " + std::to_string(run) + ": gzip_normalize did not return 0 or err was not NULL");
            fclose(fin);
            fclose(fout);
            return false;
        }

        // Validate header
        bool header_ok = (written >= 3) && (out_buf[0] == 0x1F) && (out_buf[1] == 0x8B) && (out_buf[2] == 0x08);
        if (!header_ok) {
            log_fail("test_empty_input_twice_consistency",
                     "run " + std::to_string(run) + ": invalid gzip header bytes");
            fclose(fin);
            fclose(fout);
            return false;
        }

        // Validate trailer zeros
        if (written < 8) {
            log_fail("test_empty_input_twice_consistency",
                     "run " + std::to_string(run) + ": output too short for trailer");
            fclose(fin);
            fclose(fout);
            return false;
        }
        size_t trailer_start = (size_t)written - 8;
        bool trailer_zero = true;
        for (size_t i = 0; i < 8; ++i) {
            if (out_buf[trailer_start + i] != 0) {
                trailer_zero = false;
                break;
            }
        }
        if (!trailer_zero) {
            log_fail("test_empty_input_twice_consistency",
                     "run " + std::to_string(run) + ": trailer bytes are not all zeros");
            fclose(fin);
            fclose(fout);
            return false;
        }

        fclose(fin);
        fclose(fout);
    }

    log_pass("test_empty_input_twice_consistency");
    return true;
}

// Entry point for the test suite
int main() {
    int failures = 0;

    // Run tests with explanatory citations
    if (!test_empty_input_basic_structure()) ++failures;
    if (!test_empty_input_twice_consistency()) ++failures;

    if (failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << failures << " test(s) failed." << std::endl;
        return 1;
    }
}