// Test suite for the focal C function `def(FILE *source, FILE *dest, int level)`
// This test suite is written in C++11, uses only the C++ standard library plus zlib.
// It avoids any GoogleTest/JMock frameworks, and provides a small in-house test harness.
// The tests rely on the zlib library and the provided zpipe.c implementation that defines `def`.
// How to compile (example):
//   g++ -std=c++11 -Wall -Wextra -lz -o zpipe_tests zpipe_tests.cpp
//   (Make sure zpipe.c is compiled and linked with the test, e.g. g++ ... zpipe.c ...)
// Notes:
// - We test normal operation (compress+decompress round-trip) with small and large inputs.
// - We test edge behavior by passing an invalid compression level (expect non-Z_OK return).
// - We test the failure path where destination is write-protected: expect Z_ERRNO.
// - We do not attempt to mock FILE* or private internals; we rely on real FILE* semantics.
// - All tests are non-terminating (they log failures and continue).

#include <cstdio>
#include <cstring>
#include <string.h>
#include <assert.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>


extern "C" int def(FILE *source, FILE *dest, int level); // focal method under test

// A reasonably sized chunk; used for decompression buffering in tests.
static const int TEST_CHUNK = 16384;

// Simple in-house testing harness (non-terminating assertions)
static int g_total = 0;
static int g_passed = 0;

static void log_pass(const char* name) {
    ++g_total;
    ++g_passed;
    std::cout << "[PASS] " << name << "\n";
}

static void log_fail(const char* name, const char* reason) {
    ++g_total;
    std::cout << "[FAIL] " << name;
    if (reason) {
        std::cout << " - " << reason;
    }
    std::cout << "\n";
}

// Helper: Decompress a zlib-compressed buffer into a string (round-trip verifier)
static bool decompress_buffer(const std::vector<unsigned char>& compressed, std::string& out) {
    z_stream strm{};
    int ret = inflateInit(&strm);
    if (ret != Z_OK) {
        return false;
    }

    strm.next_in = reinterpret_cast<Bytef*>(const_cast<unsigned char*>(compressed.data()));
    strm.avail_in = static_cast<uInt>(compressed.size());

    unsigned char buf[TEST_CHUNK];
    out.clear();

    do {
        strm.avail_out = TEST_CHUNK;
        strm.next_out = buf;
        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) {
            inflateEnd(&strm);
            return false;
        }
        if (ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&strm);
            return false;
        }
        size_t have = TEST_CHUNK - strm.avail_out;
        if (have > 0) {
            out.append(reinterpret_cast<char*>(buf), have);
        }
    } while (strm.avail_out == 0);

    inflateEnd(&strm);
    return ret == Z_STREAM_END;
}

// Test 1: Small input round-trip compression/decompression
// Objective: Ensure defcompresses data correctly and returns Z_OK for normal operation.
// Procedure:
// - Create a small in-memory source with known content.
// - Compress to a destination file via def with a typical level (6).
// - Decompress the destination and verify it matches the original input.
// - Expect return value to be Z_OK.
static bool test_def_small_roundtrip() {
    const char* test_name = "def_small_roundtrip";
    // Prepare source with a small payload
    FILE* src = tmpfile();
    if (!src) {
        log_fail(test_name, "failed to create temporary source file");
        return false;
    }
    const char* input = "The quick brown fox jumps over the lazy dog.";
    size_t in_len = strlen(input);
    if (fwrite(input, 1, in_len, src) != in_len) {
        fclose(src);
        log_fail(test_name, "failed writing to source");
        return false;
    }
    rewind(src);

    // Destination for compressed data
    FILE* dst = tmpfile();
    if (!dst) {
        fclose(src);
        log_fail(test_name, "failed to create temporary destination file");
        return false;
    }

    int ret = def(src, dst, 6);
    if (ret != Z_OK) {
        fclose(src);
        fclose(dst);
        log_fail(test_name, "def did not return Z_OK on small input");
        return false;
    }

    // Read compressed data
    fseek(dst, 0, SEEK_END);
    long comp_size = ftell(dst);
    fseek(dst, 0, SEEK_SET);

    std::vector<unsigned char> compressed;
    if (comp_size > 0) {
        compressed.resize(static_cast<size_t>(comp_size));
        size_t read_bytes = fread(compressed.data(), 1, comp_size, dst);
        if (read_bytes != static_cast<size_t>(comp_size)) {
            fclose(src);
            fclose(dst);
            log_fail(test_name, "failed reading compressed data");
            return false;
        }
    }

    // Decompress and verify
    std::string decompressed;
    bool ok = decompress_buffer(compressed, decompressed);
    if (!ok) {
        fclose(src);
        fclose(dst);
        log_fail(test_name, "decompression of produced data failed");
        return false;
    }

    if (decompressed != input) {
        fclose(src);
        fclose(dst);
        log_fail(test_name, "decompressed data does not match original input");
        return false;
    }

    fclose(src);
    fclose(dst);
    log_pass(test_name);
    return true;
}

// Test 2: Large input round-trip to exercise multiple internal chunks
// Objective: Validate correctness and stability for data larger than CHUNK size.
// Procedure: Similar to Test 1 but with a significantly larger payload.
static bool test_def_large_roundtrip() {
    const char* test_name = "def_large_roundtrip";
    FILE* src = tmpfile();
    if (!src) {
        log_fail(test_name, "failed to create source");
        return false;
    }

    // Create a large payload (> a few CHUNKs)
    const size_t large_size = 400000; // ~400 KB
    std::string payload;
    payload.reserve(large_size);
    for (size_t i = 0; i < large_size; ++i) {
        payload.push_back(static_cast<char>('A' + (i % 26)));
    }

    if (fwrite(payload.data(), 1, payload.size(), src) != payload.size()) {
        fclose(src);
        log_fail(test_name, "failed writing large data to source");
        return false;
    }
    rewind(src);

    FILE* dst = tmpfile();
    if (!dst) {
        fclose(src);
        log_fail(test_name, "failed to create destination file");
        return false;
    }

    int ret = def(src, dst, 6);
    if (ret != Z_OK) {
        fclose(src);
        fclose(dst);
        log_fail(test_name, "def did not return Z_OK on large input");
        return false;
    }

    fseek(dst, 0, SEEK_END);
    long comp_size = ftell(dst);
    fseek(dst, 0, SEEK_SET);

    std::vector<unsigned char> compressed;
    if (comp_size > 0) {
        compressed.resize(static_cast<size_t>(comp_size));
        size_t read_bytes = fread(compressed.data(), 1, comp_size, dst);
        if (read_bytes != static_cast<size_t>(comp_size)) {
            fclose(src);
            fclose(dst);
            log_fail(test_name, "failed reading compressed large data");
            return false;
        }
    }

    std::string decompressed;
    bool ok = decompress_buffer(compressed, decompressed);
    if (!ok) {
        fclose(src);
        fclose(dst);
        log_fail(test_name, "decompression of large data failed");
        return false;
    }

    if (decompressed != payload) {
        fclose(src);
        fclose(dst);
        log_fail(test_name, "large decompressed data does not match original payload");
        return false;
    }

    fclose(src);
    fclose(dst);
    log_pass(test_name);
    return true;
}

// Test 3: Invalid compression level should cause an early error
// Objective: Ensure that non-Z_OK from deflateInit is propagated back to the caller.
// Procedure: Use an obviously invalid level (e.g., -1) and ensure the function returns a non-Z_OK value
// and does not write to dest in that path.
static bool test_def_invalid_level() {
    const char* test_name = "def_invalid_level";
    FILE* src = tmpfile();
    if (!src) {
        log_fail(test_name, "failed to create source");
        return false;
    }
    const char* input = "Sample data for invalid level test.";
    size_t len = strlen(input);
    if (fwrite(input, 1, len, src) != len) {
        fclose(src);
        log_fail(test_name, "failed writing to source");
        return false;
    }
    rewind(src);

    FILE* dst = tmpfile();
    if (!dst) {
        fclose(src);
        log_fail(test_name, "failed to create destination");
        return false;
    }

    int ret = def(src, dst, -1); // invalid level
    // We do not enforce a specific negative code, only that it's not Z_OK
    if (ret == Z_OK) {
        fclose(src);
        fclose(dst);
        log_fail(test_name, "def unexpectedly returned Z_OK for invalid level");
        return false;
    }

    // Ensure that destination was not written (its size should be 0)
    fseek(dst, 0, SEEK_END);
    long pos = ftell(dst);
    fclose(src);
    fclose(dst);
    if (pos != 0) {
        log_fail(test_name, "destination has data even though level was invalid");
        return false;
    }

    log_pass(test_name);
    return true;
}

// Test 4: Destination write-protected should yield Z_ERRNO
// Objective: Verify that when the destination cannot be written to, def returns Z_ERRNO.
// Procedure:
// - Prepare a valid source with some data.
// - Create a real file on disk, then re-open it in read-only mode as the destination.
// - Call def and expect Z_ERRNO as return code.
static bool test_def_dest_writefailure() {
    const char* test_name = "def_dest_writefailure";

    // Prepare source
    FILE* src = tmpfile();
    if (!src) {
        log_fail(test_name, "failed to create source");
        return false;
    }
    const char* input = "Data to attempt write to a read-only destination.";
    if (fwrite(input, 1, strlen(input), src) != strlen(input)) {
        fclose(src);
        log_fail(test_name, "failed to write to source");
        return false;
    }
    rewind(src);

    // Create a real file on disk and keep it read-only
    const char* path = "zpipe_readonly_dest.tmp";
    // Create/truncate the file
    FILE* fcreate = fopen(path, "w");
    if (!fcreate) {
        fclose(src);
        log_fail(test_name, "failed to create disk file for dest");
        return false;
    }
    const char* dummy = "existing content";
    fwrite(dummy, 1, strlen(dummy), fcreate);
    fclose(fcreate);
    // Reopen as read-only
    FILE* dest = fopen(path, "r");
    if (!dest) {
        fclose(src);
        log_fail(test_name, "failed to reopen dest as read-only");
        return false;
    }

    int ret = def(src, dest, 6);
    // Expect an errno-like error code
    if (ret != Z_ERRNO) {
        fclose(src);
        fclose(dest);
        // cleanup
        remove(path);
        log_fail(test_name, "expected Z_ERRNO when dest is read-only, but got different return");
        return false;
    }

    fclose(src);
    fclose(dest);
    remove(path);
    log_pass(test_name);
    return true;
}

// Main: Run all tests and print a short summary
int main() {
    std::cout << "Starting zpipe def(...) test suite (C++11, no GTest)" << std::endl;

    bool t1 = test_def_small_roundtrip();
    bool t2 = test_def_large_roundtrip();
    bool t3 = test_def_invalid_level();
    bool t4 = test_def_dest_writefailure();

    // Summarize
    std::cout << "\nTest Summary: " << g_passed << " passed, " << g_total << " total." << std::endl;
    if (t1 && t2 && t3 && t4) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed. See output above for details." << std::endl;
        return 1;
    }
}