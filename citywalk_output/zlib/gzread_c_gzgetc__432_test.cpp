// Unit test suite for the focal method gzgetc_ which simply forwards to gzgetc.
// This test uses real zlib functionality to ensure the wrapper behaves identically
// to the underlying gzgetc across two independently managed streams reading the same data.
//
// Notes based on provided dependencies:
// - gzgetc_(gzFile) -> wrapper around gzgetc(gzFile)
// - The tests compress data into gzip files, then duplicate the compressed file,
//   and read from two independent streams in parallel, asserting that the wrapper
//   (gzgetc_) and the direct function (gzgetc) yield identical results at every step.
// - No GTest is used; a lightweight custom test harness is implemented.

#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <gzguts.h>
#include <zlib.h>
#include <cstdlib>


extern "C" {
}

// Forward declare the wrapper to ensure proper C linkage in C++ test file.
extern "C" int ZEXPORT gzgetc_(gzFile file);

// Helper: copy a file from src to dst (binary copy).
static bool copy_file(const char* src, const char* dst) {
    FILE* in = std::fopen(src, "rb");
    if (!in) {
        std::cerr << "copy_file: failed to open source " << src << "\n";
        return false;
    }
    FILE* out = std::fopen(dst, "wb");
    if (!out) {
        std::cerr << "copy_file: failed to open destination " << dst << "\n";
        std::fclose(in);
        return false;
    }

    const size_t BUF_SIZE = 4096;
    char buffer[BUF_SIZE];
    size_t n;
    while ((n = std::fread(buffer, 1, BUF_SIZE, in)) > 0) {
        if (std::fwrite(buffer, 1, n, out) != n) {
            std::cerr << "copy_file: write error\n";
            std::fclose(in);
            std::fclose(out);
            return false;
        }
    }

    std::fclose(in);
    std::fclose(out);
    return true;
}

// Test 1: Verify gzgetc_ matches gzgetc for a small data set using two independent streams.
// Steps:
//  - Compress a small string to test1_small.gz.
//  - Copy it to test1_small_copy.gz.
//  - Open both files with gzopen in read mode to obtain two independent gzFile streams.
//  - In a loop, compare gzgetc(...) from the first stream with gzgetc_(...) from the second stream.
//  - They should produce identical sequence until EOF, after which both return -1.
static bool test_gzgetc_wrapper_two_streams_small() {
    std::cout << "Running test_gzgetc_wrapper_two_streams_small...\n";

    const char* orig = "test_gzc1_small.gz";
    const char* copy = "test_gzc1_small_copy.gz";
    const std::string data = "ABC";

    // Create compressed file with data
    gzFile f = gzopen(orig, "wb");
    if (!f) {
        std::cerr << "Failed to create " << orig << "\n";
        return false;
    }
    int written = gzwrite(f, data.data(), (unsigned)data.size());
    gzclose(f);
    if (written != (int)data.size()) {
        std::cerr << "gzwrite mismatch: wrote " << written << " expected " << data.size() << "\n";
        return false;
    }

    // Copy the compressed file
    if (!copy_file(orig, copy)) {
        std::cerr << "Failed to copy " << orig << " to " << copy << "\n";
        return false;
    }

    // Open both streams for reading
    gzFile f1 = gzopen(orig, "rb");
    gzFile f2 = gzopen(copy, "rb");
    if (!f1 || !f2) {
        std::cerr << "Failed to open streams for test\n";
        if (f1) gzclose(f1);
        if (f2) gzclose(f2);
        return false;
    }

    bool ok = true;
    for (;;) {
        int a = gzgetc(f1);
        int b = gzgetc_(f2); // wrapper
        if (a != b) {
            std::cerr << "Mismatch at position: a=" << a << " b=" << b << "\n";
            ok = false;
            break;
        }
        if (a == -1) break; // both should be EOF simultaneously
    }

    gzclose(f1);
    gzclose(f2);

    // Cleanup
    std::remove(orig);
    std::remove(copy);

    if (ok) {
        std::cout << "test_gzgetc_wrapper_two_streams_small PASSED\n";
    } else {
        std::cout << "test_gzgetc_wrapper_two_streams_small FAILED\n";
    }

    return ok;
}

// Test 2: Repeat the two-streams comparison with larger data to ensure coverage over longer input.
// Data chosen to include spaces and punctuation to ensure proper decompression handling.
static bool test_gzgetc_wrapper_two_streams_large() {
    std::cout << "Running test_gzgetc_wrapper_two_streams_large...\n";

    const char* orig = "test_gzc2_large.gz";
    const char* copy = "test_gzc2_large_copy.gz";
    const std::string data = "The quick brown fox jumps over the lazy dog. 0123456789";

    // Create compressed file with data
    gzFile f = gzopen(orig, "wb");
    if (!f) {
        std::cerr << "Failed to create " << orig << "\n";
        return false;
    }
    int written = gzwrite(f, data.data(), (unsigned)data.size());
    gzclose(f);
    if (written != (int)data.size()) {
        std::cerr << "gzwrite mismatch: wrote " << written << " expected " << data.size() << "\n";
        return false;
    }

    // Copy the compressed file
    if (!copy_file(orig, copy)) {
        std::cerr << "Failed to copy " << orig << " to " << copy << "\n";
        return false;
    }

    // Open both streams for reading
    gzFile f1 = gzopen(orig, "rb");
    gzFile f2 = gzopen(copy, "rb");
    if (!f1 || !f2) {
        std::cerr << "Failed to open streams for test_large\n";
        if (f1) gzclose(f1);
        if (f2) gzclose(f2);
        return false;
    }

    bool ok = true;
    for (;;) {
        int a = gzgetc(f1);
        int b = gzgetc_(f2);
        if (a != b) {
            std::cerr << "Mismatch in large test at byte: a=" << a << " b=" << b << "\n";
            ok = false;
            break;
        }
        if (a == -1) break;
    }

    gzclose(f1);
    gzclose(f2);

    // Cleanup
    std::remove(orig);
    std::remove(copy);

    if (ok) {
        std::cout << "test_gzgetc_wrapper_two_streams_large PASSED\n";
    } else {
        std::cout << "test_gzgetc_wrapper_two_streams_large FAILED\n";
    }

    return ok;
}

// Entry point: run all tests and report overall result.
int main() {
    bool all_passed = true;

    // Step 3 (Test Case Refinement) - Run refined tests to ensure coverage.
    all_passed &= test_gzgetc_wrapper_two_streams_small();
    all_passed &= test_gzgetc_wrapper_two_streams_large();

    if (all_passed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED\n";
        return 1;
    }
}