// Win32 tell_fileFunc unit test suite (C++11, no GoogleTest)
// Purpose: validate behavior of win32_tell_file_func from iowin32.c
// Notes:
// - Tests rely on Windows API (CreateFileA, SetFilePointerEx, GetLastError, etc.)
// - This test suite uses a tiny, non-terminating assertion framework (EXPECT_* macros)
// - The code assumes the presence of iowin32.h and the WIN32FILE_IOWIN structure with
//   at least members: hf (HANDLE) and error (int)

#include <cstdio>
#include <windows.h>
#include <iowin32.h>
#include <iostream>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>


// Lightweight non-terminating assertion helpers
static int g_tests_run = 0;
static int g_tests_pass = 0;

#define EXPECT_TRUE(cond, msg) do { \
    g_tests_run++; \
    if (cond) { g_tests_pass++; } \
    else { std::cerr << "[FAIL] " << msg << std::endl; } \
} while(0)

#define EXPECT_EQ(expected, actual, msg) do { \
    g_tests_run++; \
    if (static_cast<decltype(actual)>(actual) == (expected)) { g_tests_pass++; } \
    else { std::cerr << "[FAIL] " << msg \
        << " (expected: " << (expected) << ", actual: " << (actual) << ")" << std::endl; } \
} while(0)

// Test 1: stream == NULL -> should return -1
void test_win32_tell_null_stream() {
    // When stream is NULL, ret should be -1 per function contract
    SetLastError(0);
    long ret = win32_tell_file_func(nullptr, nullptr);
    EXPECT_EQ(-1L, ret, "win32_tell_file_func should return -1 when stream is NULL");
}

// Test 2: stream non-NULL but hf == NULL -> should return -1
void test_win32_tell_null_handle_in_stream() {
    WIN32FILE_IOWIN w;
    w.hf = NULL;     // simulate missing HANDLE
    w.error = 0;
    long ret = win32_tell_file_func(nullptr, (void*)&w);
    EXPECT_EQ(-1L, ret, "win32_tell_file_func should return -1 when stream.hf is NULL");
}

// Test 3: Valid handle with current position set to 100 -> expect 100
void test_win32_tell_valid_handle_moves_to_100() {
    // Create a temporary file (exists on disk) for a real HANDLE
    char dirBuf[MAX_PATH] = {0};
    DWORD lenDir = GetTempPathA(MAX_PATH, dirBuf);
    if (lenDir == 0 || lenDir > MAX_PATH) {
        std::cerr << "[WARN] Unable to obtain temp path. Skipping test_win32_tell_valid_handle_moves_to_100." << std::endl;
        return;
    }

    char fileNameBuf[MAX_PATH] = {0};
    // Generate a unique temp file name
    DWORD n = GetTempFileNameA(dirBuf, "w32tell", 0, fileNameBuf);
    if (n == 0) {
        std::cerr << "[WARN] GetTempFileNameA failed. Skipping test." << std::endl;
        return;
    }

    // Open the temp file with read/write access
    HANDLE hFile = CreateFileA(
        fileNameBuf,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING, // The file exists due to GetTempFileName
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "[WARN] CreateFileA failed. Skipping test." << std::endl;
        return;
    }

    // Move the pointer to offset 100 using the Windows API
    LARGE_INTEGER moveTo;
    moveTo.QuadPart = 100;
    LARGE_INTEGER actual;
    BOOL ok = SetFilePointerEx(hFile, moveTo, &actual, FILE_CURRENT);
    if (!ok) {
        std::cerr << "[WARN] SetFilePointerEx failed to move to 100. Skipping test." << std::endl;
        CloseHandle(hFile);
        return;
    }

    // Now call the function under test; it should return 100
    WIN32FILE_IOWIN w;
    w.hf = hFile;
    w.error = 0;
    long ret = win32_tell_file_func(nullptr, (void*)&w);

    EXPECT_EQ(100L, ret, "win32_tell_file_func should return current position (100) after external move");
    // Also verify that error remains untouched in this success path
    EXPECT_TRUE(w.error == 0, "win32_tell_file_func should not modify error on success");

    // Cleanup
    CloseHandle(hFile);
    // The temp file created by GetTempFileName should be automatically deleted when closing if opened with DeleteOnClose;
    // Since we opened with OPEN_EXISTING, remove it manually
    DeleteFileA(fileNameBuf);
}

// Test 4: Invalid handle should cause failure and populate error with GetLastError
void test_win32_tell_invalid_handle_sets_error() {
    WIN32FILE_IOWIN w;
    // Use an explicitly invalid handle value
    w.hf = (HANDLE)(intptr_t)-1; // Equivalent to INVALID_HANDLE_VALUE
    w.error = 0;
    SetLastError(0);
    long ret = win32_tell_file_func(nullptr, (void*)&w);
    EXPECT_TRUE(ret == -1L, "win32_tell_file_func should return -1 with invalid handle");
    // The error code should be set (non-zero) by the underlying API call
    EXPECT_TRUE(w.error != 0, "win32_tell_file_func should populate error on failure");
}

int main() {
    std::cout << "Starting tests for win32_tell_file_func...\n";

    test_win32_tell_null_stream();
    test_win32_tell_null_handle_in_stream();
    test_win32_tell_valid_handle_moves_to_100();
    test_win32_tell_invalid_handle_sets_error();

    std::cout << "Tests completed. "
              << g_tests_pass << " / " << g_tests_run << " tests passed.\n";

    // Return non-zero if any test failed
    return (g_tests_run == g_tests_pass) ? 0 : 1;
}