// Test suite for the focal method: win32_read_file_func (iowin32.c)
// This test suite is designed for a C++11 project without Google Test.
// It uses a lightweight, non-terminating assertion approach and a custom main().
// The tests rely on mocked Windows API functions (ReadFile, GetLastError) to
// exercise the behavior without requiring a real Windows environment.

#include <cstring>
#include <iowin32.h>
#include <iostream>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>


// Bring C declarations into the C++ translation unit.
extern "C" {
}

// Define true/false for environments where Windows BOOL may not be available in C++.
// These macros are safe here as we only use numeric values 0/1 for testing.
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// Forward declarations for mocked Windows API functions.
// We declare them with C linkage to replace the real Windows API during tests.
extern "C" {

// The test harness will supply these mocks to intercept ReadFile calls.
BOOL ReadFile(HANDLE hFile, void* lpBuffer, DWORD nNumberOfBytesToRead, DWORD* lpNumberOfBytesRead, void* lpOverlapped);

// Returns the last error code. Used to verify error path mapping.
DWORD GetLastError(void);

} // extern "C"

// Global state for mocks to control behavior and observe interactions.
static bool g_ReadFile_was_called = false;
static bool g_ReadFile_should_succeed = true; // When true, mock reports success; when false, reports failure.
static DWORD g_GetLastError_return = 0;
static DWORD g_ReadFile_bytes_to_read = 0; // Optional: number of bytes requested in ReadFile, for validation.

// Mock implementations of Windows API used by win32_read_file_func.
extern "C" {

// Mock for ReadFile: records invocation, optionally fills buffer, and reports success/failure.
BOOL ReadFile(HANDLE hFile, void* lpBuffer, DWORD nNumberOfBytesToRead, DWORD* lpNumberOfBytesRead, void* /*lpOverlapped*/) {
    g_ReadFile_was_called = true;
    g_ReadFile_bytes_to_read = nNumberOfBytesToRead;

    if (g_ReadFile_should_succeed) {
        if (lpNumberOfBytesRead != nullptr) {
            *lpNumberOfBytesRead = nNumberOfBytesToRead; // Simulate full read
        }
        // Optionally fill buffer with a recognizable pattern for debugging
        if (lpBuffer != nullptr && nNumberOfBytesToRead > 0) {
            unsigned char* p = static_cast<unsigned char*>(lpBuffer);
            for (DWORD i = 0; i < nNumberOfBytesToRead; ++i) {
                p[i] = static_cast<unsigned char>(i & 0xFF);
            }
        }
        return TRUE;
    } else {
        // Simulate a read failure
        return FALSE;
    }
}

// Mock for GetLastError: returns controlled error code set in tests.
DWORD GetLastError(void) {
    return g_GetLastError_return;
}

} // extern "C"

// Simple non-terminating assertion framework.
// The tests will accumulate failures but continue executing to maximize coverage.
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_ASSERT_EQ(actual, expected, desc)                                  \
    do {                                                                        \
        ++g_total_tests;                                                        \
        if ((actual) != (expected)) {                                         \
            std::cerr << "[FAIL] " << (desc) << " | Expected: " << (expected)  \
                      << ", Got: " << (actual) << std::endl;                 \
            ++g_failed_tests;                                                 \
        } else {                                                              \
            std::cout << "[PASS] " << (desc) << std::endl;                   \
        }                                                                     \
    } while (0)

#define TEST_ASSERT_TRUE(condition, desc)                                         \
    TEST_ASSERT_EQ((condition) ? 1 : 0, 1, (desc))

#define RESET_MOCKS()                                                             \
    do {                                                                            \
        g_ReadFile_was_called = false;                                              \
        g_ReadFile_should_succeed = true;                                           \
        g_GetLastError_return = 0;                                                  \
        g_ReadFile_bytes_to_read = 0;                                             \
    } while (0)

// Helper to create a dummy stream object compatible with win32_read_file_func.
// The actual type WIN32FILE_IOWIN is defined in iowin32.h.
// We'll populate the hf field to simulate a valid file handle and ensure the
// function enters the ReadFile path.
static WIN32FILE_IOWIN create_dummy_stream(HANDLE fakeHandle) {
    WIN32FILE_IOWIN s;
    s.hf = fakeHandle;
    s.error = -1; // Initialize to a distinct value to verify that error is updated only on failure.
    return s;
}

// Test 1: When stream is NULL, win32_read_file_func should not attempt to ReadFile
// and should return 0 immediately.
void test_win32_read_file_null_stream() {
    RESET_MOCKS();
    const size_t BUF_SIZE = 8;
    char buffer[BUF_SIZE];
    voidpf opaque = nullptr;
    voidpf stream = nullptr;

    uLong ret = win32_read_file_func(opaque, stream, buffer, BUF_SIZE);

    // Assertions
    TEST_ASSERT_EQ(ret, 0u, "win32_read_file_func returns 0 when stream is NULL");
    TEST_ASSERT_TRUE(g_ReadFile_was_called == false, "ReadFile should not be called when stream is NULL");
}

// Test 2: stream is non-NULL with a valid hFile; ReadFile succeeds and bytes are read.
void test_win32_read_file_success() {
    RESET_MOCKS();
    // Prepare a non-null stream with a non-null file handle.
    HANDLE fakeHandle = reinterpret_cast<HANDLE>(0xABCD1234);
    WIN32FILE_IOWIN stream = create_dummy_stream(fakeHandle);

    const size_t BUF_SIZE = 6;
    char buffer[BUF_SIZE];
    voidpf opaque = nullptr;
    voidpf stream_ptr = static_cast<voidpf>(&stream);

    // Ensure mock is set to succeed
    g_ReadFile_should_succeed = true;

    uLong ret = win32_read_file_func(opaque, stream_ptr, buffer, BUF_SIZE);

    // Assertions
    TEST_ASSERT_EQ(ret, static_cast<uLong>(BUF_SIZE), "win32_read_file_func returns number of bytes read on success");
    TEST_ASSERT_TRUE(g_ReadFile_was_called, "ReadFile should be called when stream has a valid handle");
    TEST_ASSERT_EQ(stream.error, -1, "stream.error should remain unchanged on successful ReadFile");
}

// Test 3: ReadFile fails and GetLastError == ERROR_HANDLE_EOF maps to 0 in stream->error
void test_win32_read_file_error_eof_maps_to_zero() {
    RESET_MOCKS();
    HANDLE fakeHandle = reinterpret_cast<HANDLE>(0xDEADBEEF);
    WIN32FILE_IOWIN stream = create_dummy_stream(fakeHandle);

    const size_t BUF_SIZE = 4;
    char buffer[BUF_SIZE];
    voidpf opaque = nullptr;
    voidpf stream_ptr = static_cast<voidpf>(&stream);

    // Configure mocks for failure and EOF error
    g_ReadFile_should_succeed = false;
    g_GetLastError_return = 38; // ERROR_HANDLE_EOF (common value, 38)

    uLong ret = win32_read_file_func(opaque, stream_ptr, buffer, BUF_SIZE);

    // Assertions
    TEST_ASSERT_EQ(ret, 0u, "win32_read_file_func returns 0 when ReadFile fails");
    TEST_ASSERT_TRUE(g_ReadFile_was_called, "ReadFile should be called and fail");
    TEST_ASSERT_EQ(stream.error, 0, "stream.error should be set to 0 when ERROR_HANDLE_EOF is reported");
}

// Test 4: ReadFile fails with a non-EOF error; stream->error must reflect GetLastError()
void test_win32_read_file_error_other() {
    RESET_MOCKS();
    HANDLE fakeHandle = reinterpret_cast<HANDLE>(0xBEEFCAFE);
    WIN32FILE_IOWIN stream = create_dummy_stream(fakeHandle);

    const size_t BUF_SIZE = 3;
    char buffer[BUF_SIZE];
    voidpf opaque = nullptr;
    voidpf stream_ptr = static_cast<voidpf>(&stream);

    g_ReadFile_should_succeed = false;
    g_GetLastError_return = 123; // Arbitrary non-EOF error

    uLong ret = win32_read_file_func(opaque, stream_ptr, buffer, BUF_SIZE);

    // Assertions
    TEST_ASSERT_EQ(ret, 0u, "win32_read_file_func returns 0 on ReadFile failure");
    TEST_ASSERT_TRUE(g_ReadFile_was_called, "ReadFile should be called on failure");
    TEST_ASSERT_EQ(stream.error, 123, "stream.error should reflect the value returned by GetLastError");
}

// Test 5: Stream non-NULL but hFile == NULL should not call ReadFile and return 0
void test_win32_read_file_null_handle_in_stream() {
    RESET_MOCKS();
    WIN32FILE_IOWIN stream;
    stream.hf = nullptr; // Null handle
    stream.error = -5;

    const size_t BUF_SIZE = 5;
    char buffer[BUF_SIZE];
    voidpf opaque = nullptr;
    voidpf stream_ptr = static_cast<voidpf>(&stream);

    uLong ret = win32_read_file_func(opaque, stream_ptr, buffer, BUF_SIZE);

    // Assertions
    TEST_ASSERT_EQ(ret, 0u, "win32_read_file_func returns 0 when hFile is NULL");
    TEST_ASSERT_TRUE(g_ReadFile_was_called == false, "ReadFile should not be called when hFile is NULL");
}

// Entry point: run all tests and report summary.
int main() {
    std::cout << "Starting test suite for win32_read_file_func...\n";

    test_win32_read_file_null_stream();
    test_win32_read_file_success();
    test_win32_read_file_error_eof_maps_to_zero();
    test_win32_read_file_error_other();
    test_win32_read_file_null_handle_in_stream();

    int total = g_total_tests;
    int failures = g_failed_tests;
    int passed = total - failures;

    std::cout << "Tests run: " << total << ", Passed: " << passed << ", Failed: " << failures << "\n";

    if (failures > 0) {
        std::cerr << "Some tests failed. Review details above for debugging.\n";
    } else {
        std::cout << "All tests passed." << std::endl;
    }

    // Return non-zero if any test failed to aid integration in build pipelines.
    return (failures == 0) ? 0 : 1;
}