/*
Unit test suite for the focal method:
long ZCALLBACK win32_seek64_file_func(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin);

This test suite is designed to be compiled with C++11 (no GoogleTest) and Windows API support.
It creates a minimal, self-contained test harness with lightweight, non-terminating assertions.

Step-by-step rationale (summarized for traceability):
- Step 1 (Program Understanding): The function maps a 64-bit offset and an origin to a Windows file pointer move method (FILE_BEGIN, FILE_CURRENT, FILE_END). It uses a stream wrapper WIN32FILE_IOWIN to extract an underlying HANDLE (hf). It validates origin, and if a valid handle exists, it delegates the pointer movement to MySetFilePointerEx. On failure, it records the Windows error code into stream->error and returns -1; on success, returns 0.
- Step 2 (Unit Test Generation): Tests target:
  - Null stream => -1
  - Non-null stream with NULL hf => -1
  - Valid stream and valid file handle: seek succeeds and resulting pointer matches expected
  - Invalid origin: -1
- Step 3 (Test Case Refinement): Use Windows APIs to verify file pointer position (GetFilePointerEx) after a successful seek. Ensure tests are robust against non-terminating test execution.

Candidate Keywords extracted:
- dwMoveMethod, FILE_CURRENT, FILE_END, FILE_BEGIN
- ZLIB_FILEFUNC_SEEK_CUR, ZLIB_FILEFUNC_SEEK_END, ZLIB_FILEFUNC_SEEK_SET
- hFile, HF, WIN32FILE_IOWIN, hf
- MySetFilePointerEx, GetLastError, error
- offset (ZPOS64_T), origin
- ret, -1, 0
- stream, NULL, non-NULL
- GetFilePointerEx, LARGE_INTEGER, FILE_CURRENT
- extern "C", ZCALLBACK, voidpf, ZPOS64_T
- iowin32.h, ioapi.h, zlib.h, Windows.h

Note: This test suite relies on the project providing a proper iowin32.h with type WIN32FILE_IOWIN and the actual implementation of MySetFilePointerEx in the linked iowin32.c. The tests interact with a real Windows file handle to verify pointer movement.

Now the test code:

*/

// Only compile on Windows
#ifdef _WIN32

#include <cstdio>
#include <cstring>
#include <windows.h>
#include <iowin32.h>
#include <cstdint>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>


// Include zlib/io headers as used by the focal method
extern "C" {
}

// Forward declaration to ensure correct linkage in C++
extern "C" long ZCALLBACK win32_seek64_file_func(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin);

// Lightweight non-terminating test framework
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        fprintf(stderr, "[TEST FAILED] %s\n", (msg)); \
        ++g_tests_failed; \
    } else { \
        /* success path: keep testing */ \
        ; \
    } \
    ++g_tests_run; \
} while(0)

#define EXPECT_EQ_INT(a, b, msg) do { \
    if((int)(a) != (int)(b)) { \
        fprintf(stderr, "[TEST FAILED] %s: expected %d, got %d\n", (msg), (int)(b), (int)(a)); \
        ++g_tests_failed; \
    } else { \
        /* success */ \
        ; \
    } \
    ++g_tests_run; \
} while(0)

#define EXPECT_EQ64(a, b, msg) do { \
    if((long long)(a) != (long long)(b)) { \
        fprintf(stderr, "[TEST FAILED] %s: expected %lld, got %lld\n", (msg), (long long)(b), (long long)(a)); \
        ++g_tests_failed; \
    } else { \
        ; \
    } \
    ++g_tests_run; \
} while(0)

// Helper to create a temporary file and return its HANDLE and path
static bool create_temp_file(HANDLE* phFile, char* outPath, DWORD pathBufSize) {
    if (!phFile || !outPath) return false;
    char tempPath[MAX_PATH] = {0};
    if (GetTempPathA(MAX_PATH, tempPath) == 0) return false;
    char tempFileName[MAX_PATH] = {0};
    if (GetTempFileNameA(tempPath, "tst", 0, tempFileName) == 0) return false;

    HANDLE h = CreateFileA(
        tempFileName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (h == INVALID_HANDLE_VALUE) {
        return false;
    }

    // Return results
    *phFile = h;
    // Copy path for cleanup
    strncpy(outPath, tempFileName, pathBufSize - 1);
    outPath[pathBufSize - 1] = '\0';
    return true;
}

// Test 1: Null stream should fail (-1)
static void test_seek64_null_stream() {
    long ret = win32_seek64_file_func(nullptr, nullptr, (ZPOS64_T)0, ZLIB_FILEFUNC_SEEK_SET);
    EXPECT_EQ_INT(ret, -1, "seek64 with null stream should return -1");
}

// Test 2: Non-null stream but NULL hf should fail (-1)
static void test_seek64_stream_null_hf() {
    // Create a dummy stream structure with hf = NULL
    WIN32FILE_IOWIN dummy = {0};
    dummy.hf = NULL;
    voidpf stream = &dummy;

    long ret = win32_seek64_file_func(nullptr, stream, (ZPOS64_T)0, ZLIB_FILEFUNC_SEEK_SET);
    EXPECT_EQ_INT(ret, -1, "seek64 with non-null stream but NULL hf should return -1");
}

// Test 3: Valid stream and valid file handle -> expect success and pointer equals offset
static void test_seek64_valid_seek() {
    HANDLE hFile = INVALID_HANDLE_VALUE;
    char tmpPath[MAX_PATH] = {0};
    if (!create_temp_file(&hFile, tmpPath, sizeof(tmpPath))) {
        fprintf(stderr, "[TEST SKIPPED] Could not create temp file for valid seek test\n");
        return;
    }

    WIN32FILE_IOWIN winStream = {0};
    winStream.hf = hFile;

    voidpf stream = &winStream;
    ZPOS64_T offset = 512;
    long ret = win32_seek64_file_func(nullptr, stream, offset, ZLIB_FILEFUNC_SEEK_SET);
    EXPECT_EQ_INT(ret, 0, "seek64 should return 0 on success");

    // Verify the new pointer position using GetFilePointerEx
    LARGE_INTEGER current = {0};
    BOOL got = GetFilePointerEx(hFile, /*DistanceToMove*/ 0, &current, FILE_CURRENT);
    if(!got) {
        fprintf(stderr, "[TEST FAILED] Could not query file pointer after seek\n");
        ++g_tests_run;
        ++g_tests_failed;
    } else {
        EXPECT_EQ64(current.QuadPart, (long long)offset, "Current file pointer after seek64_SET should equal offset");
    }

    // Cleanup
    CloseHandle(hFile);
    DeleteFileA(tmpPath);
}

// Test 4: Invalid origin should fail (-1)
static void test_seek64_invalid_origin() {
    HANDLE hFile = INVALID_HANDLE_VALUE;
    char tmpPath[MAX_PATH] = {0};
    if (!create_temp_file(&hFile, tmpPath, sizeof(tmpPath))) {
        fprintf(stderr, "[TEST SKIPPED] Could not create temp file for invalid-origin test\n");
        return;
    }

    WIN32FILE_IOWIN winStream = {0};
    winStream.hf = hFile;
    voidpf stream = &winStream;

    // Use an invalid origin value that is not one of the three handled cases
    int invalidOrigin = 9999;
    long ret = win32_seek64_file_func(nullptr, stream, (ZPOS64_T)0, invalidOrigin);
    EXPECT_EQ_INT(ret, -1, "seek64 with invalid origin should return -1");

    CloseHandle(hFile);
    DeleteFileA(tmpPath);
}

int main() {
    // Phase: Run tests sequentially, collecting results without terminating on first failure
    fprintf(stdout, "Running win32_seek64_file_func unit tests (C++11, Windows)...\n");

    test_seek64_null_stream();
    test_seek64_stream_null_hf();
    test_seek64_valid_seek();
    test_seek64_invalid_origin();

    // Summary
    fprintf(stdout, "Tests run: %d, Failures: %d\n", g_tests_run, g_tests_failed);
    return (g_tests_failed == 0) ? 0 : 1;
}

#else
// Non-Windows: provide a brief message that tests are not applicable
int main() {
    fprintf(stdout, "win32_seek64_file_func tests are Windows-specific and were not executed on this platform.\n");
    return 0;
}
#endif