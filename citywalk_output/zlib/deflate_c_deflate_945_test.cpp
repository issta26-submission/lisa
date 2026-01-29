// Unit test suite for the focal deflate method (deflate.c) using a lightweight
// C++11 test harness (no GoogleTest). The tests rely on the public zlib-style API
// exposed in zlib.h and link against the provided deflate.c implementation in the
// project. Each test includes explanatory comments describing the purpose and the
// targeted code paths within deflate().

#include <cstring>
#include <deflate.h>
#include <iostream>
#include <vector>
#include <zlib.h>


// Bring in C declarations for the deflate API and related constants.
extern "C" {
}

// Helper macro for concise test result reporting
#define TEST_PASSED true
#define TEST_FAILED false

// Utility: Run a single test and print its result
static bool run_test(const char* test_name, bool passed, const char* note) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << test_name;
    if (note && strlen(note) > 0) {
        std::cout << " - " << note;
    }
    std::cout << std::endl;
    return passed;
}

// Test 1: deflate should return Z_STREAM_ERROR when an invalid flush value is provided.
// This covers the initial guard clause: if (deflateStateCheck(strm) || flush > Z_BLOCK || flush < 0) { return Z_STREAM_ERROR; }
static bool test_deflate_invalid_flush_returns_error() {
    const int OUT_SIZE = 1024;
    unsigned char outbuf[OUT_SIZE];
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    // Prepare a valid output buffer
    strm.next_out = outbuf;
    strm.avail_out = OUT_SIZE;
    // Initialize the stream (typical usage would be via deflateInit)
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        // If initialization fails, the test cannot proceed reliably
        return TEST_FAILED;
    }

    // Call deflate with an invalid negative flush value to trigger Z_STREAM_ERROR
    int ret = deflate(&strm, -1);

    // Cleanup
    deflateEnd(&strm);

    return ret == Z_STREAM_ERROR;
}

// Test 2: deflate should return Z_STREAM_ERROR when next_out is NULL (no destination buffer).
// This tests the next_out NULL path in the early predicate: if (strm->next_out == Z_NULL || ...)
static bool test_deflate_next_out_null_returns_error() {
    const int OUT_SIZE = 1024;
    unsigned char outbuf[OUT_SIZE];
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    // Do not provide an output buffer
    strm.next_out = Z_NULL;
    strm.avail_out = OUT_SIZE;

    // Initialize the stream
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return TEST_FAILED;
    }

    // Even with zero input, the function should error due to missing next_out
    strm.next_in = Z_NULL;
    strm.avail_in = 0;

    int ret = deflate(&strm, Z_NO_FLUSH);

    deflateEnd(&strm);

    return ret == Z_STREAM_ERROR;
}

// Test 3: deflate should return Z_BUF_ERROR when there is no input available and no pending work.
// This exercises the branch: else if (strm->avail_in == 0 && RANK(flush) <= RANK(old_flush) && flush != Z_FINISH)
static bool test_deflate_no_input_no_pending_returns_buf_error() {
    const int OUT_SIZE = 1024;
    unsigned char outbuf[OUT_SIZE];
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    // Provide a valid output buffer
    strm.next_out = outbuf;
    strm.avail_out = OUT_SIZE;

    // Initialize the stream
    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return TEST_FAILED;
    }

    // Ensure there is no input and no pending data
    strm.next_in = Z_NULL;
    strm.avail_in = 0;

    // We choose flush = Z_NO_FLUSH (not FINISH) so the code takes the branch
    // that yields Z_BUF_ERROR when there is nothing to do.
    int ret = deflate(&strm, Z_NO_FLUSH);

    deflateEnd(&strm);

    return ret == Z_BUF_ERROR;
}

// Optional: Helper to run all tests and summarize results
int main() {
    std::vector<std::pair<const char*, bool>> results;

    results.push_back({ "deflate_invalid_flush_returns_error",
                        test_deflate_invalid_flush_returns_error() });
    results.push_back({ "deflate_next_out_null_returns_error",
                        test_deflate_next_out_null_returns_error() });
    results.push_back({ "deflate_no_input_no_pending_returns_buf_error",
                        test_deflate_no_input_no_pending_returns_buf_error() });

    int passed = 0;
    for (const auto& r : results) {
        const char* name = r.first;
        bool ok = r.second;
        // We can't pass a separate explanatory note for every environment here, so keep notes minimal.
        if (ok) {
            run_test(name, true, "");
            ++passed;
        } else {
            run_test(name, false, "Encountered unexpected behavior or test couldn't be executed under current environment.");
        }
    }

    std::cout << "Summary: " << passed << "/" << results.size() << " tests passed." << std::endl;
    // Return non-zero if any test failed to aid integration in CI systems
    return (passed == (int)results.size()) ? 0 : 1;
}