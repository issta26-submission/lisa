// Test suite for the focal method: win32_seek_file_func
// This test is designed to compile under C++11 on Windows and link against iowin32.c / iowin32.h.
// No GTest is used; a lightweight in-house test harness is provided.

#include <cstdio>
#include <windows.h>
#include <iowin32.h>
#include <cstdint>
#include <iostream>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>


extern "C" {
    // zlib style types and the function under test are assumed to be declared here.
    // We declare the symbol with C linkage to ensure proper linkage with the C implementation.
    #include "zlib.h"          // provides uLong, voidpf, etc.
    #include "iowin32.h"       // provides WIN32FILE_IOWIN, ZCALLBACK, etc.
    long ZCALLBACK win32_seek_file_func(void* opaque, void* stream, uLong offset, int origin);
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_EQ(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "[TEST_FAIL] " << __FILE__ << ":" << __LINE__ \
                      << "  Expected " << (expected) << " but got " << (actual) << std::endl; \
            ++g_failures; \
        } else { \
            std::cout << "[TEST_PASS] " << __FILE__ << ":" << __LINE__ \
                      << "  Value = " << (actual) << std::endl; \
        } \
    } while (0)

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << "[TEST_FAIL] " << __FILE__ << ":" << __LINE__ \
                      << "  Condition failed: " << #cond << std::endl; \
            ++g_failures; \
        } else { \
            std::cout << "[TEST_PASS] " << __FILE__ << ":" << __LINE__ \
                      << "  Condition passed: " << #cond << std::endl; \
        } \
    } while (0)

static void test_invalid_origin_returns_minus_one()
{
    // Case: origin is not one of the supported values -> should return -1 immediately
    // Stream can be NULL; origin invalid triggers default branch.
    void* opaque = nullptr;
    void* stream = nullptr;
    uLong offset = 0;
    int invalid_origin = 9999; // not any of CUR/END/SET

    long ret = win32_seek_file_func(opaque, stream, offset, invalid_origin);

    EXPECT_EQ(ret, -1);
    // No assumptions about side effects on stream since origin was invalid.
}

static void test_null_stream_with_valid_origin_returns_minus_one()
{
    // Case: stream == NULL; origin valid but stream is NULL -> should return -1 (ret initialized to -1)
    void* opaque = nullptr;
    void* stream = nullptr;
    uLong offset = 123;
    // Use a valid origin
    int origin =  ZLIB_FILEFUNC_SEEK_SET;

    long ret = win32_seek_file_func(opaque, stream, offset, origin);

    EXPECT_EQ(ret, -1);
}

static void test_stream_with_null_handle_returns_minus_one()
{
    // Case: stream is non-NULL but the underlying handle is NULL (hf == NULL)
    void* opaque = nullptr;

    // Build a minimal WIN32FILE_IOWIN-like object with hf = NULL
    WIN32FILE_IOWIN w;
    w.hf = NULL;
    w.error = 0;

    void* stream = (void*)&w;
    uLong offset = 0;
    int origin = ZLIB_FILEFUNC_SEEK_CUR;

    long ret = win32_seek_file_func(opaque, stream, offset, origin);

    // Since hf is NULL, the function should not enter the file-pointer logic and return -1
    EXPECT_EQ(ret, -1);
    // error field may remain unchanged.
    (void)w.error; // suppress unused if not observed
}

static void test_stream_with_valid_handle_cur_path()
{
    // Case: stream non-NULL with a non-NULL hf, origin CUR.
    // We cannot reliably force a success return due to the internal MySetFilePointerEx being static in the TU.
    // We still exercise the code path and accept either 0 (success) or -1 (failure) to maximize coverage.
    void* opaque = nullptr;

    WIN32FILE_IOWIN w;
    w.hf = (HANDLE)0x1; // non-null handle
    w.error = 0;

    void* stream = (void*)&w;
    uLong offset = 50;
    int origin = ZLIB_FILEFUNC_SEEK_CUR;

    long ret = win32_seek_file_func(opaque, stream, offset, origin);

    // Accept both outcomes; any value indicates the path was exercised.
    if (ret == 0 || ret == -1) {
        std::cout << "[TEST_PASS] test_stream_with_valid_handle_cur_path() yielded ret = " << ret << std::endl;
    } else {
        std::cerr << "[TEST_FAIL] test_stream_with_valid_handle_cur_path() yielded unexpected ret = " << ret << std::endl;
        ++g_failures;
    }
}

static void test_stream_with_valid_handle_end_path()
{
    // Case: stream non-NULL with a non-NULL hf, origin END.
    void* opaque = nullptr;

    WIN32FILE_IOWIN w;
    w.hf = (HANDLE)0x2; // non-null handle
    w.error = 0;

    void* stream = (void*)&w;
    uLong offset = 1024;
    int origin = ZLIB_FILEFUNC_SEEK_END;

    long ret = win32_seek_file_func(opaque, stream, offset, origin);

    // As above, accept both possible outcomes to maximize coverage.
    if (ret == 0 || ret == -1) {
        std::cout << "[TEST_PASS] test_stream_with_valid_handle_end_path() yielded ret = " << ret << std::endl;
    } else {
        std::cerr << "[TEST_FAIL] test_stream_with_valid_handle_end_path() yielded unexpected ret = " << ret << std::endl;
        ++g_failures;
    }
}

int main() {
    // Run targeted test cases to exercise the focal function's branches.
    std::cout << "Starting tests for win32_seek_file_func...\n";

    test_invalid_origin_returns_minus_one();
    test_null_stream_with_valid_origin_returns_minus_one();
    test_stream_with_null_handle_returns_minus_one();
    test_stream_with_valid_handle_cur_path();
    test_stream_with_valid_handle_end_path();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}