/*
  Unit test suite for the focal method:
  voidpf ZCALLBACK win32_open64_file_funcW(voidpf opaque, const void* filename, int mode)

  Notes:
  - This test is designed to exercise the Windows-file-opening branch of win32_open64_file_funcW
    by providing mock implementations for Windows API entry points (CreateFileW and, if compiled
    with IOWIN32_USING_WINRT_API defined, CreateFile2).
  - The static helper win32_translate_open_mode is assumed to populate the OUT parameters (dwDesiredAccess,
    dwShareMode, dwCreationDisposition, dwFlagsAndAttributes) in the target environment. The tests rely on
    that behavior to trigger the correct code paths. If your build uses a different translation, adjust the
    test expectations accordingly.
  - The test suite is written using a tiny, self-contained framework (no GTest) and is intended to be
    compiled with a C++11 compiler. It links against the iowin32.c source in the project under test.
  - To exercise the WinRT path (CreateFile2) you may need to build the test binary with -DIOWIN32_USING_WINRT_API
    so that the code in win32_open64_file_funcW uses CreateFile2. This file demonstrates the non-WinRT path
    (CreateFileW) testing; run the WinRT variant separately if needed.

  Build/run notes:
  - Ensure iowin32.c is compiled and linked with this test.
  - Ensure the mock prototypes for CreateFileW/CreateFile2 match the real signatures used by iowin32.c.
  - This test uses a minimal, non-terminating assertion style; it prints results to stdout and exits with a
    non-zero code on failure.
*/

#include <cstdio>
#include <cstring>
#include <iowin32.h>
#include <cstdint>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>
#include <cwchar>


// Domain knowledge: include required dependencies for the focal API types.
extern "C" {
}

// ----------------------------------------------------------------------
// Mock Windows API surface for testing the iowin32.cpp behavior
// ----------------------------------------------------------------------

extern "C" {

// Define Windows-like types to satisfy the function signatures without including Windows headers.
typedef void* HANDLE;
typedef unsigned long DWORD;
typedef const wchar_t* LPCWSTR;

// Globals to observe mock invocations from the focal function
static int g_CreateFileW_called = 0;
static int g_CreateFile2_called = 0;
static const void* g_CreateFileW_filename = nullptr;
static DWORD g_CreateFileW_dwDesiredAccess = 0;
static DWORD g_CreateFileW_dwShareMode = 0;
static DWORD g_CreateFileW_dwCreationDisposition = 0;
static DWORD g_CreateFileW_dwFlagsAndAttributes = 0;
static const void* g_CreateFile2_filename = nullptr;

// Mock for CreateFileW in non-WinRT path
HANDLE CreateFileW(LPCWSTR lpFileName,
                   DWORD dwDesiredAccess,
                   DWORD dwShareMode,
                   void* lpSecurityAttributes,
                   DWORD dwCreationDisposition,
                   DWORD dwFlagsAndAttributes,
                   void* hTemplateFile)
{
    g_CreateFileW_called++;
    g_CreateFileW_filename = lpFileName;
    g_CreateFileW_dwDesiredAccess = dwDesiredAccess;
    g_CreateFileW_dwShareMode = dwShareMode;
    g_CreateFileW_dwCreationDisposition = dwCreationDisposition;
    g_CreateFileW_dwFlagsAndAttributes = dwFlagsAndAttributes;
    // Simulate a valid file handle when the API would succeed
    return (HANDLE)0x11111111;
}

// Optional mock for CreateFile2 when WinRT path is exercised
HANDLE CreateFile2(LPCWSTR lpFileName,
                   DWORD dwDesiredAccess,
                   DWORD dwShareMode,
                   DWORD dwCreationDisposition,
                   void* /*lpSecurityAttributes*/)
{
    g_CreateFile2_called++;
    g_CreateFile2_filename = lpFileName;
    // Return a distinct fake handle to indicate success
    return (HANDLE)0x22222222;
}

} // extern "C"

// ----------------------------------------------------------------------
// Tiny test framework (non-GTest)
// ----------------------------------------------------------------------

static int g_tests_run = 0;
static int g_tests_failed = 0;

#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::fprintf(stderr, "ASSERT_TRUE failed: %s (%s:%d)\n", (msg), __FILE__, __LINE__); \
        ++g_tests_failed; \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    if(!((a) == (b))) { \
        std::fprintf(stderr, "ASSERT_EQ failed: %s | " #a "=%lld, " #b "=%lld (at %s:%d)\n", (msg), (long long)(a), (long long)(b), __FILE__, __LINE__); \
        ++g_tests_failed; \
    } \
} while(0)

#define RUN_TEST(test_func) do { \
    ++g_tests_run; \
    test_func(); \
} while(0)

// Helper to reset mocks state before each test
static void reset_mocks() {
    g_CreateFileW_called = 0;
    g_CreateFile2_called = 0;
    g_CreateFileW_filename = nullptr;
    g_CreateFileW_dwDesiredAccess = 0;
    g_CreateFileW_dwShareMode = 0;
    g_CreateFileW_dwCreationDisposition = 0;
    g_CreateFileW_dwFlagsAndAttributes = 0;
    g_CreateFile2_filename = nullptr;
}

// ----------------------------------------------------------------------
// Test cases for win32_open64_file_funcW
// ----------------------------------------------------------------------

/*
  Test 1: Non-WinRT path (CreateFileW) should be invoked when filename is non-null and
          the translation function provides a non-zero DesiredAccess.
  This validates the true-branch of the inner if predicate: (filename != NULL) && (dwDesiredAccess != 0)
*/
static void test_open64_calls_CreateFileW_on_valid_filename_and_access()
{
    reset_mocks();

    // Prepare inputs: filename provided, mode triggers non-zero DesiredAccess via translation
    const void* fname = (const void*)L"test_open64_valid.txt";

    // The mode value is chosen to induce a non-zero dwDesiredAccess by the (unknown)
    // translation logic inside iowin32.c. We assume the environment sets it to non-zero.
    int mode = 1; // arbitrary non-zero to attempt non-zero DesiredAccess after translation

    // Call the focal function
    voidpf result = NULL;
    result = win32_open64_file_funcW(NULL, fname, mode);

    // Assertions:
    // 1) CreateFileW should have been invoked exactly once
    ASSERT_EQ(g_CreateFileW_called, 1, "CreateFileW should be called once for valid filename and access.");
    // 2) The function should have attempted to use the provided filename
    ASSERT_TRUE(g_CreateFileW_filename == fname, "CreateFileW should receive the correct filename pointer.");

    // We don't assert the exact value of 'result' since win32_build_iowin is implementation-defined
    // in the test harness (it's a static function in the iowin32.c unit). We only verify behavior
    // of the OS call pathway.
    RUN_TEST([](){return;}); // no-op to keep macro structure consistent
}

// Test 2: False branch when filename is NULL; CreateFileW should not be called
static void test_open64_does_not_call_CreateFileW_when_filename_null()
{
    reset_mocks();

    // filename is NULL
    const void* fname = nullptr;
    int mode = 1; // non-zero to ensure that if filename is null, path is skipped

    voidpf result = NULL;
    result = win32_open64_file_funcW(NULL, fname, mode);

    // Assertions:
    ASSERT_EQ(g_CreateFileW_called, 0, "CreateFileW should not be called when filename is NULL.");
    (void)result; // suppress unused warning if any
}

// Test 3: False branch when DesiredAccess is zero; CreateFileW should not be called
static void test_open64_does_not_call_CreateFileW_when_access_zero()
{
    reset_mocks();

    // filename provided but we assume translation yields zero DesiredAccess
    const void* fname = (const void*)L"test_open64_noaccess.txt";

    int mode = 0; // mode that would yield zero DesiredAccess per translation logic

    voidpf result = NULL;
    result = win32_open64_file_funcW(NULL, fname, mode);

    // Assertions:
    ASSERT_EQ(g_CreateFileW_called, 0, "CreateFileW should not be called when DesiredAccess evaluates to zero.");
    (void)result;
}

// ----------------------------------------------------------------------
// Main runner
// ----------------------------------------------------------------------

int main() {
    // Run tests
    RUN_TEST(test_open64_calls_CreateFileW_on_valid_filename_and_access);
    RUN_TEST(test_open64_does_not_call_CreateFileW_when_filename_null);
    RUN_TEST(test_open64_does_not_call_CreateFileW_when_access_zero);

    // Report summary
    std::printf("Tests run: %d, Failures: %d\n", g_tests_run, g_tests_failed);
    return (g_tests_failed == 0) ? 0 : 1;
}