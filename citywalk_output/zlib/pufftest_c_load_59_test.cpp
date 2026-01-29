// C++11 unit test suite for the C function `load` located in pufftest.c
// This test suite exercises key execution paths of load by creating test files,
// invoking load, and validating returned buffers and lengths.
// Notes:
// - This test assumes the test binary is linked with the Puff source that defines `load`.
// - Accessibility of `load` is attempted via an extern "C" declaration to ensure linkage across C/C++ boundaries.
// - Tests are written with plain asserts (no Google Test) per the constraints.
// - Each test is annotated with comments explaining its purpose and coverage focus.

#include <cstdio>
#include <cstring>
#include <cassert>
#include <stdio.h>
#include <puff.h>
#include <stdlib.h>
#include <cstdlib>


// Declaration of the focal function under test.
// If `load` is defined with internal linkage in pufftest.c, this forward declaration
// should still be resolvable when the test is linked with the same translation unit
// (or when the linker exposes the symbol). The extern "C" ensures C linkage for C/C++.
extern "C" void* load(const char* name, size_t* len);

// Helper: write data to a file (binary mode)
static void write_file(const char* path, const void* data, size_t len) {
    FILE* f = fopen(path, "wb");
    if (f == NULL) {
        perror("fopen for write");
        exit(EXIT_FAILURE);
    }
    if (len > 0 && data != NULL) {
        if (fwrite(data, 1, len, f) != len) {
            perror("fwrite");
            fclose(f);
            exit(EXIT_FAILURE);
        }
    }
    fclose(f);
}

// Helper: remove a file if it exists (best-effort)
static void remove_file(const char* path) {
    if (path == NULL) return;
    remove(path);
}

// Test 1: Non-existent file should yield a non-NULL buffer with len == 0
static void test_load_nonexistent_file() {
    const char* path = "test_puff_load_nonexistent.bin";
    // Ensure path does not exist prior to test
    remove_file(path);

    size_t len = 0;
    void* buf = load(path, &len);
    // Expect a valid (non-NULL) buffer allocated by load, with zero length read
    assert(buf != NULL);
    assert(len == 0);

    // Clean up
    free(buf);
    remove_file(path);
}

// Test 2: Small file (< 4096 bytes) should be read entirely in one read
static void test_load_small_file() {
    const char* path = "test_puff_load_small.bin";
    const char small_data[] = "0123456789"; // 10 bytes
    write_file(path, small_data, sizeof(small_data) - 1);

    size_t len = 0;
    void* buf = load(path, &len);
    assert(buf != NULL);
    assert(len == sizeof(small_data) - 1);
    // Compare content
    assert(std::memcmp(buf, small_data, len) == 0);

    // Cleanup
    free(buf);
    remove_file(path);
}

// Test 3: Zero-byte file should return len == 0 and non-NULL buffer
static void test_load_zero_byte_file() {
    const char* path = "test_puff_load_zero.bin";
    // Create an empty file
    {
        FILE* f = fopen(path, "wb");
        if (f == NULL) {
            perror("fopen zero-byte");
            exit(EXIT_FAILURE);
        }
        fclose(f);
    }

    size_t len = 0;
    void* buf = load(path, &len);
    assert(buf != NULL);
    // len should be 0 for an empty file
    assert(len == 0);

    // Cleanup
    free(buf);
    remove_file(path);
}

// Test 4: Large file (> 4096 bytes) to exercise realloc path
static void test_load_large_file() {
    const char* path = "test_puff_load_large.bin";
    const size_t large_size = 8000; // larger than initial 4096 buffer
    unsigned char* large_data = new unsigned char[large_size];
    // Fill with a repeating pattern for content verification
    for (size_t i = 0; i < large_size; ++i) {
        large_data[i] = (unsigned char)(i & 0xFF);
    }

    write_file(path, large_data, large_size);

    size_t len = 0;
    void* buf = load(path, &len);
    assert(buf != NULL);
    assert(len == large_size);
    // Verify content integrity
    assert(std::memcmp(buf, large_data, len) == 0);

    // Cleanup
    free(buf);
    delete[] large_data;
    remove_file(path);
}

// Test 5: Various sequences to ensure the function handles mixed runs correctly
// (Additional sanity: ensure multiple small reads followed by a resize behave as expected)
static void test_load_multiple_small_and_large_segments() {
    const char* path = "test_puff_load_mix.bin";
    // Create a medium file: two segments that force at least one realloc
    // First 3500 bytes, then 4000 bytes -> total 7500 bytes
    const size_t first = 3500;
    const size_t second = 4000;
    unsigned char* data = new unsigned char[first + second];
    for (size_t i = 0; i < first + second; ++i) {
        data[i] = (unsigned char)((i * 7) & 0xFF);
    }

    write_file(path, data, first + second);

    size_t len = 0;
    void* buf = load(path, &len);
    assert(buf != NULL);
    assert(len == first + second);
    assert(std::memcmp(buf, data, len) == 0);

    // Cleanup
    free(buf);
    delete[] data;
    remove_file(path);
}

// Main test runner
int main(void) {
    test_load_nonexistent_file();
    test_load_small_file();
    test_load_zero_byte_file();
    test_load_large_file();
    test_load_multiple_small_and_large_segments();

    // If all asserts pass
    return EXIT_SUCCESS;
}