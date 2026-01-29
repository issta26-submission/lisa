// Lightweight C++11 test suite for the focal function win32_tell64_file_func
// This test targets the Windows-specific implementation in iowin32.c
// It avoids GTest and uses a small, self-contained testing harness with
// simple pass/fail reporting.
// Notes:
// - Tests require a Windows environment (HANDLE, CreateFile, SetFilePointerEx, etc.)
// - We rely on the actual Windows API behavior; the tests construct a real file
//   and use a real file handle to exercise the success path.
// - Static/internal helpers in iowin32.c are not mocked here; we exercise
//   only the public interface declared in iowin32.h.
// - The tests intentionally use non-terminating assertions (flag-based) per
//   domain guidance.

#include <cstdio>
#include <windows.h>
#include <string>
#include <iowin32.h>
#include <iostream>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>


extern "C" {
}

// Simple non-terminating test harness
static int total_tests = 0;
static int failed_tests = 0;

#define TEST_PASS(msg) do { ++total_tests; std::cout << "[PASS] " << msg << std::endl; } while(0)
#define TEST_FAIL(msg) do { ++total_tests; ++failed_tests; std::cerr << "[FAIL] " << msg << std::endl; } while(0)

static void log_result(bool condition, const std::string& msg) {
    if (condition) TEST_PASS(msg);
    else TEST_FAIL(msg);
}

// Helper: create a unique temporary file and return its HANDLE and path
static bool create_temp_file(HANDLE& outHandle, std::string& outPath) {
    char tempPath[MAX_PATH] = {0};
    DWORD len = GetTempPathA(MAX_PATH, tempPath);
    if (len == 0 || len > MAX_PATH) return false;

    char fileName[MAX_PATH] = {0};
    // Use a unique prefix; GetTempFileName will create a zero-byte file
    if (GetTempFileNameA(tempPath, "w32t", 0, fileName) == 0) {
        return false;
    }

    // Open for read/write; CREATE_ALWAYS to ensure a new file
    HANDLE h = CreateFileA(
        fileName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (h == INVALID_HANDLE_VALUE) {
        // Cleanup potential file by removing it if needed
        DeleteFileA(fileName);
        return false;
    }

    outHandle = h;
    outPath = std::string(fileName);
    return true;
}

// Test 1: Null stream should return -1 and not touch the stream
// Coverage: predicate (stream != NULL) is false; second predicate is not evaluated.
static void test_null_stream_returns_minus_one() {
    ZPOS64_T ret = win32_tell64_file_func(nullptr, nullptr);
    log_result(ret == (ZPOS64_T)-1, "win32_tell64_file_func with null stream returns -1");
}

// Test 2: Valid stream and valid file handle; expect success path and position value
// Coverage: stream != NULL, hFile != NULL, MySetFilePointerEx success path (true branch)
static void test_valid_handle_tell64_success() {
    HANDLE hFile = INVALID_HANDLE_VALUE;
    std::string path;
    bool created = create_temp_file(hFile, path);
    log_result(created, "Temporary file created for tell64 test");

    if (!created) {
        TEST_FAIL("Skipping Test 2 due to temp file creation failure");
        return;
    }

    // Prepare stream object consistent with iowin32.h expectations
    WIN32FILE_IOWIN streamObj;
    streamObj.hf = hFile;
    streamObj.error = 0;

    ZPOS64_T ret = win32_tell64_file_func(nullptr, (void*)&streamObj);

    // We expect current file pointer to be at position 0 initially
    log_result(ret == 0, "win32_tell64_file_func returns 0 for fresh file (pointer at start)");
    log_result(streamObj.error == 0, "No error recorded in stream after successful tell64");

    CloseHandle(hFile);
    DeleteFileA(path.c_str());
}

// Test 3: Invalid file handle should trigger the error path and return -1
// Coverage: hFile invalid leads to MySetFilePointerEx failure path; stream->error should be set.
static void test_invalid_handle_triggers_error() {
    WIN32FILE_IOWIN badStream;
    badStream.hf = INVALID_HANDLE_VALUE; // deliberately invalid
    badStream.error = 0;

    ZPOS64_T ret = win32_tell64_file_func(nullptr, (void*)&badStream);

    log_result(ret == (ZPOS64_T)-1, "win32_tell64_file_func with invalid handle returns -1");
    log_result(badStream.error != 0, "stream->error is set on failure from invalid handle");
}

// Test 4: Non-null stream with NULL file handle should not touch OS and return -1
// Coverage: branch where hFile check fails (hFile == NULL) and early return occurs.
static void test_null_handle_in_stream_behaviour() {
    WIN32FILE_IOWIN zeroStream;
    zeroStream.hf = NULL;
    zeroStream.error = 0xABCD; // non-zero to verify no modification

    ZPOS64_T ret = win32_tell64_file_func(nullptr, (void*)&zeroStream);

    log_result(ret == (ZPOS64)-1, "win32_tell64_file_func with NULL hf returns -1");
    log_result(zeroStream.error == 0xABCD, "stream->error remains unchanged when hf is NULL");
}

// Entry point
int main() {
    std::cout << "Starting tests for win32_tell64_file_func (iowin32.c) in C++11 environment...\n";

    test_null_stream_returns_minus_one();
    test_valid_handle_tell64_success();
    test_invalid_handle_triggers_error();
    test_null_handle_in_stream_behaviour();

    std::cout << "Tests completed. Passed: " << (total_tests - failed_tests)
              << ", Failed: " << failed_tests
              << ", Total: " << total_tests << std::endl;

    // Return non-zero if any test failed
    return failed_tests > 0 ? 1 : 0;
}