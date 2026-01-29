// Unit test suite for the focal method: win32_write_file_func
// Language: C++11
// Notes:
// - This test suite provides a lightweight, non-GTest harness.
// - It uses simple, non-terminating expectations to maximize code coverage.
// - Windows API calls (WriteFile, GetLastError) are mocked to validate behavior
//   without requiring an actual Windows environment.
// - The tests rely on the project-provided declarations from iowin32.h and the
//   WIN32FILE_IOWIN struct defined therein.

#include <cstring>
#include <iowin32.h>
#include <cstdint>
#include <iostream>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>


// Include the focal class dependencies and the function under test.
// Assumption: iowin32.h provides the necessary Windows-like types
// (HANDLE, DWORD, BOOL, uLong, etc.) and defines WIN32FILE_IOWIN.

// Allow environment to override common Windows constants if not defined.
#ifndef ERROR_HANDLE_EOF
#define ERROR_HANDLE_EOF 38
#endif

// Lightweight, non-terminating assertion helpers
static int g_pass = 0;
static int g_fail = 0;

#define EXPECT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cerr << "EXPECT_FAIL: " << (msg) \
                  << " | expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
        ++g_fail; \
    } else { \
        ++g_pass; \
    } \
} while(0)

#define EXPECT_TRUE(condition, msg) do { \
    if (!(condition)) { \
        std::cerr << "EXPECT_FAIL: " << (msg) << std::endl; \
        ++g_fail; \
    } else { \
        ++g_pass; \
    } \
} while(0)

// Mocks for Windows API used by win32_write_file_func
// We provide C-linkage so the linker can resolve symbols from the C file under test.
extern "C" {

// The Windows types (HANDLE, DWORD, BOOL, etc.) are assumed to be defined by iowin32.h.
// We declare only the mocks with the same signatures used by the production code.

    // Simple mock control flags to drive behavior in tests
    static bool mock_write_succeed = true;
    static DWORD mock_last_error = 0;
    static DWORD mock_last_written = 0;

    // Mock for WriteFile: returns success or failure based on mock_write_succeed
    // Signature matches typical Windows API: BOOL WriteFile(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED)
    BOOL WINAPI WriteFile(HANDLE hFile,
                          const void* lpBuffer,
                          DWORD nNumberOfBytesToWrite,
                          DWORD* lpNumberOfBytesWritten,
                          void* lpOverlapped)
    {
        (void)lpBuffer;        // unused in mock
        (void)lpOverlapped;    // unused in mock
        (void)hFile;           // we only care whether it's NULL (handled in production)
        if (mock_write_succeed)
        {
            if (lpNumberOfBytesWritten != nullptr)
                *lpNumberOfBytesWritten = nNumberOfBytesToWrite;
            mock_last_written = nNumberOfBytesToWrite;
            return TRUE;
        }
        else
        {
            if (lpNumberOfBytesWritten != nullptr)
                *lpNumberOfBytesWritten = 0;
            return FALSE;
        }
    }

    // Mock for GetLastError
    DWORD GetLastError(void)
    {
        return mock_last_error;
    }

}

// Test driver
int main() {
    // Prepare a test buffer
    const unsigned char test_buf[] = "0123456789"; // 10 bytes
    const uLong test_size = static_cast<uLong>(sizeof(test_buf) - 1); // 10

    // Test 1: stream == NULL should do nothing and return 0
    {
        // Reset mock state
        mock_write_succeed = true;
        mock_last_error = 0;
        mock_last_written = 0;

        uLong ret = win32_write_file_func(nullptr, nullptr, test_buf, test_size);

        // Expect 0 as no stream and hence no file handle
        EXPECT_EQ(static_cast<uint64_t>(ret), static_cast<uint64_t>(0),
                  "win32_write_file_func with NULL stream should return 0");
    }

    // Test 2: stream non-NULL and WriteFile succeeds
    {
        // Reset mock state
        mock_write_succeed = true;
        mock_last_error = 0;
        mock_last_written = 0;

        WIN32FILE_IOWIN stream;
        // Provide a non-NULL file handle; actual value is not used by mock beyond NULL check
        stream.hf = reinterpret_cast<void*>(0xDEADBEEF);
        stream.error = 0;

        uLong ret = win32_write_file_func(nullptr, &stream, test_buf, test_size);

        // On success, WriteFile writes 'test_size' bytes and ret should be equal to size
        EXPECT_EQ(static_cast<uint64_t>(ret), static_cast<uint64_t>(test_size),
                  "win32_write_file_func should return the number of bytes written on success");
        // Error should remain untouched (0)
        EXPECT_EQ(static_cast<int>(stream.error), 0,
                  "win32_write_file_func should not set error on success");
    }

    // Test 3: stream non-NULL and WriteFile fails with a generic error
    {
        // Reset mock state
        mock_write_succeed = false;
        mock_last_error = 123; // arbitrary error code
        mock_last_written = 0;

        WIN32FILE_IOWIN stream;
        stream.hf = reinterpret_cast<void*>(0xCAFEBABE);
        stream.error = 0;

        uLong ret = win32_write_file_func(nullptr, &stream, test_buf, test_size);

        // Expect no bytes written on failure
        EXPECT_EQ(static_cast<uint64_t>(ret), static_cast<uint64_t>(0),
                  "win32_write_file_func should return 0 when WriteFile fails");
        // The error field should reflect GetLastError() value
        EXPECT_EQ(static_cast<int>(stream.error), 123,
                  "win32_write_file_func should propagate GetLastError() to stream.error on failure");
    }

    // Test 4: WriteFile fails with ERROR_HANDLE_EOF, which should be normalized to 0
    // This exercises the branch where the code converts ERROR_HANDLE_EOF to 0.
    {
        // Reset mock state
        mock_write_succeed = false;
        mock_last_error = ERROR_HANDLE_EOF;
        mock_last_written = 0;

        WIN32FILE_IOWIN stream;
        stream.hf = reinterpret_cast<void*>(0xBADC0DE);
        stream.error = -1; // ensure it changes

        uLong ret = win32_write_file_func(nullptr, &stream, test_buf, test_size);

        // Expect failure
        EXPECT_EQ(static_cast<uint64_t>(ret), static_cast<uint64_t>(0),
                  "win32_write_file_func should return 0 when WriteFile fails with ERROR_HANDLE_EOF");
        // ERROR_HANDLE_EOF should be normalized to 0
        EXPECT_EQ(static_cast<int>(stream.error), 0,
                  "win32_write_file_func should normalize ERROR_HANDLE_EOF to 0 in stream.error");
    }

    // Summary
    std::cout << "Tests run: " << (g_pass + g_fail)
              << " | Passed: " << g_pass
              << " | Failed: " << g_fail << std::endl;

    // Return non-zero if any test failed
    return (g_fail == 0) ? 0 : 1;
}