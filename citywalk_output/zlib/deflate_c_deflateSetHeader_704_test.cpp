// Test suite for deflateSetHeader in deflate.c using C++11 (no GTest).
// This test suite targets the behavior of deflateSetHeader with various
// stream states (gzip wrap vs non-gzip wrap) and input scenarios.
// It relies on zlib public API (deflateInit, deflateInit2, deflateSetHeader, deflateEnd)
// to drive the internal state required by deflateSetHeader.

#include <cstring>
#include <deflate.h>
#include <functional>
#include <iostream>
#include <vector>
#include <zlib.h>


// Lightweight test harness (non-terminating assertions)
struct TestCase {
    const char* name;
    std::function<void()> func;
    bool passed;
    std::string message;
};

static std::vector<TestCase> g_tests;

// Helper macro-like function to register tests
#define REGISTER_TEST(test_name) \
    do { \
        TestCase tc; \
        tc.name = #test_name; \
        tc.func = test_name; \
        tc.passed = false; \
        g_tests.push_back(tc); \
    } while (0)

// Utility to print test result
static void reportResults() {
    int total = (int)g_tests.size();
    int ok = 0;
    for (const auto& t : g_tests) {
        if (t.passed) ++ok;
        std::cout << "[" << (t.passed ? "PASS" : "FAIL") << "] "
                  << t.name << (t.passed ? "" : "  - " + t.message) << "\n";
    }
    std::cout << "\nTest Summary: " << ok << "/" << total << " tests passed.\n";
}

// Test 1: Deflate gzip path should succeed and return Z_OK
// Strategy: Initialize a z_stream with gzip wrapper (windowBits = 15 + 16) using deflateInit2.
// Then call deflateSetHeader with a non-null header. Expect Z_OK.
void test_deflateSetHeader_gzip_succeeds() {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    // Initialize as gzip-wrapped deflate stream
    int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                           15 + 16, // windowBits for gzip
                           8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        g_tests.back().passed = false;
        g_tests.back().message = "deflateInit2 failed to initialize gzip stream";
        return;
    }

    gz_header head;
    std::memset(&head, 0, sizeof(head)); // minimal, no actual header fields needed

    // True test: gzip-wrapped stream with a non-null header should succeed
    int res = deflateSetHeader(&strm, &head);
    deflateEnd(&strm);

    g_tests.back().passed = (res == Z_OK);
    if (!g_tests.back().passed) {
        g_tests.back().message = "deflateSetHeader did not return Z_OK for gzip stream with non-null head";
    }
}

// Test 2: Non-gzip (zlib-wrapped) stream should fail (wrap != 2)
 // Strategy: Initialize a normal (non-gzip) stream with deflateInit.
 // Call deflateSetHeader and expect Z_STREAM_ERROR due to wrap != 2.
void test_deflateSetHeader_non_gzip_wrap_fails() {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        g_tests.back().passed = false;
        g_tests.back().message = "deflateInit failed to initialize non-gzip stream";
        return;
    }

    gz_header head;
    std::memset(&head, 0, sizeof(head));

    int res = deflateSetHeader(&strm, &head);
    deflateEnd(&strm);

    g_tests.back().passed = (res == Z_STREAM_ERROR);
    if (!g_tests.back().passed) {
        g_tests.back().message = "deflateSetHeader did not return Z_STREAM_ERROR for non-gzip stream";
    }
}

// Test 3: Null stream pointer should yield Z_STREAM_ERROR (deflateStateCheck path)
 // Strategy: Call with a null z_streamp to exercise the first predicate (deflateStateCheck(strm)).
 // This should short-circuit and return an error without dereferencing a valid state.
void test_deflateSetHeader_null_stream_returns_error() {
    gz_header head;
    std::memset(&head, 0, sizeof(head));

    // Expect an error when strm is NULL
    int res = deflateSetHeader(nullptr, &head);
    g_tests.back().passed = (res == Z_STREAM_ERROR);
    if (!g_tests.back().passed) {
        g_tests.back().message = "deflateSetHeader did not return Z_STREAM_ERROR for null stream";
    }
}

// Test 4: Valid gzip stream but null header should still succeed (head = NULL)
// Strategy: Initialize gzip stream and pass a NULL header; ensure function returns Z_OK.
// Note: The function only stores the header pointer; it does not dereference it here.
// This exercises the true path with a safe NULL header.
void test_deflateSetHeader_gzip_with_null_header_succeeds() {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                           15 + 16, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        g_tests.back().passed = false;
        g_tests.back().message = "deflateInit2 failed to initialize gzip stream for null header test";
        return;
    }

    // NULL head should still be accepted by the function
    int res = deflateSetHeader(&strm, nullptr);
    deflateEnd(&strm);

    g_tests.back().passed = (res == Z_OK);
    if (!g_tests.back().passed) {
        g_tests.back().message = "deflateSetHeader with NULL head did not return Z_OK for gzip stream";
    }
}

// Main to register and run tests
int main() {
    // Register tests
    g_tests.clear();
    // Note: We push dummy test entries to carry test details in results;
    // Each test adds its own result entry with a descriptive name.

    // We'll create test cases by invoking specific wrapper lambdas that perform the checks
    // and then assign results to newly pushed TestCase entries.

    // Wrapper to collect results for test 1
    {
        TestCase t;
        t.name = "deflateSetHeader_gzip_succeeds";
        t.func = []() { test_deflateSetHeader_gzip_succeeds(); };
        t.passed = false;
        g_tests.push_back(t);
        // Run the test
        g_tests.back().func();
    }

    // Wrapper for test 2
    {
        TestCase t;
        t.name = "deflateSetHeader_non_gzip_wrap_fails";
        t.func = []() { test_deflateSetHeader_non_gzip_wrap_fails(); };
        t.passed = false;
        g_tests.push_back(t);
        g_tests.back().func();
    }

    // Wrapper for test 3
    {
        TestCase t;
        t.name = "deflateSetHeader_null_stream_returns_error";
        t.func = []() { test_deflateSetHeader_null_stream_returns_error(); };
        t.passed = false;
        g_tests.push_back(t);
        g_tests.back().func();
    }

    // Wrapper for test 4
    {
        TestCase t;
        t.name = "deflateSetHeader_gzip_with_null_header_succeeds";
        t.func = []() { test_deflateSetHeader_gzip_with_null_header_succeeds(); };
        t.passed = false;
        g_tests.push_back(t);
        g_tests.back().func();
    }

    // Interpret results
    // The test harness sets test results via the test functions themselves.
    // Now we will print results. Note: Some test implementations set a final
    // result within the TestCase, but for simplicity we rely on the
    // "passed" flag of each TestCase after function execution.

    // Since we set TestCase.passed inside the individual tests, we need to propagate
    // those values into the global report. Re-run a minimal pass/fail pass to keep output clean.

    // Produce final report
    reportResults();

    // Return non-zero if any test failed
    bool anyFail = false;
    for (const auto& t : g_tests) {
        if (!t.passed) { anyFail = true; break; }
    }
    return anyFail ? 1 : 0;
}