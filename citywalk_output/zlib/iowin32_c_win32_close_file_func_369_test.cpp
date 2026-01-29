// Test suite for the focal method: win32_close_file_func in iowin32.c
// This test suite is designed for C++11 compilation without GTest.
// It uses a lightweight, handcrafted test harness and simple assertions.
// Note: We rely on the included headers (zlib.h, ioapi.h, iowin32.h) for type definitions
// such as voidpf, HANDLE, and WIN32FILE_IOWIN.

#include <cstdio>
#include <cstring>
#include <iowin32.h>
#include <iostream>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>
#include <cstdlib>


// Include the focal class dependencies to ensure the correct types are visible.

// The actual win32_close_file_func is implemented in iowin32.c.
// We declare it here with a C linkage-compatible prototype, assuming voidpf is compatible with void*.
extern "C" int win32_close_file_func(void* opaque, void* stream);

// ---------------------------------------------------------------------------
// Minimal test harness utilities
// Notes:
// - We implement a tiny set of assertions that do not terminate tests, to maximize
//   coverage and code execution.
// - Tests print PASS/FAIL per-case and accumulate a final result.
// - We also provide a simple global "mock" for the Windows CloseHandle used by the test.
// ---------------------------------------------------------------------------

static int g_close_handle_calls = 0;
static void* g_last_closed_handle = nullptr;

// Mocked CloseHandle to intercept calls from win32_close_file_func.
// signature chosen to be compatible with typical Windows typedefs (BOOL CloseHandle(HANDLE)).
extern "C" int CloseHandle(void* hObject) {
    ++g_close_handle_calls;
    g_last_closed_handle = hObject;
    // Return value is not used by the tested function, but we provide a non-zero value to mimic success.
    return 1;
}

// Helper to reset global mock state before each test
static void reset_mock_state() {
    g_close_handle_calls = 0;
    g_last_closed_handle = nullptr;
}

// Lightweight assertion helpers
static bool assert_int_equal(const char* test_name, int a, int b) {
    bool ok = (a == b);
    if (!ok) {
        std::cerr << "[FAIL] " << test_name << ": expected " << b << ", got " << a << "\n";
    } else {
        std::cout << "[PASS] " << test_name << "\n";
    }
    return ok;
}

static bool assert_ptr_equal(const char* test_name, const void* a, const void* b) {
    bool ok = (a == b);
    if (!ok) {
        std::cerr << "[FAIL] " << test_name << ": expected pointer " << b << ", got " << a << "\n";
    } else {
        std::cout << "[PASS] " << test_name << "\n";
    }
    return ok;
}

// ---------------------------------------------------------------------------
// Test 1: stream == NULL should return -1 and not call CloseHandle
// Rationale: true branch for stream NULL should bypass all inner logic.
// ---------------------------------------------------------------------------

static bool test_win32_close_file_null_stream() {
    reset_mock_state();
    void* opaque = nullptr;
    void* stream = nullptr; // NULL stream
    int ret = win32_close_file_func(opaque, stream);

    bool ok = true;
    ok &= assert_int_equal("test_win32_close_file_null_stream.ret", ret, -1);
    ok &= assert_int_equal("test_win32_close_file_null_stream.close_calls", g_close_handle_calls, 0);
    ok &= assert_ptr_equal("test_win32_close_file_null_stream.last_handle", g_last_closed_handle, nullptr);
    return ok;
}

// ---------------------------------------------------------------------------
// Test 2: stream non-NULL with a non-NULL hFile should call CloseHandle and return 0
// Rationale: true branch for hFile != NULL and the inner logic should be exercised.
// We verify that CloseHandle is invoked exactly once with the expected handle.
// ---------------------------------------------------------------------------

static bool test_win32_close_file_with_valid_handle() {
    reset_mock_state();
    // Allocate a stream object of the expected type
    WIN32FILE_IOWIN* w = (WIN32FILE_IOWIN*)std::malloc(sizeof(WIN32FILE_IOWIN));
    if (w == nullptr) {
        std::cerr << "[ERROR] Failed to allocate stream object for valid-handle test.\n";
        return false;
    }

    // Populate the hf member with a non-NULL fake handle
    void* fake_handle = (void*)0xDEADBEEF;
    w->hf = (HANDLE)fake_handle; // cast to HANDLE for consistency with code
    void* opaque = nullptr;
    void* stream = (void*)w;

    int ret = win32_close_file_func(opaque, stream);

    bool ok = true;
    ok &= assert_int_equal("test_win32_close_file_with_valid_handle.ret", ret, 0);
    ok &= assert_int_equal("test_win32_close_file_with_valid_handle.close_calls", g_close_handle_calls, 1);
    ok &= assert_ptr_equal("test_win32_close_file_with_valid_handle.last_handle", g_last_closed_handle, fake_handle);

    // The function frees the stream object; we do not attempt to free here to avoid double free.
    // Let the C runtime reclaim the memory after function returns.

    // Note: We do not free w ourselves because the tested function frees it.
    return ok;
}

// ---------------------------------------------------------------------------
// Test 3: stream non-NULL with a NULL hFile should not call CloseHandle and return -1
// Rationale: false branch of the inner if (hFile != NULL) should be exercised.
// ---------------------------------------------------------------------------

static bool test_win32_close_file_with_null_handle() {
    reset_mock_state();
    WIN32FILE_IOWIN* w = (WIN32FILE_IOWIN*)std::malloc(sizeof(WIN32FILE_IOWIN));
    if (w == nullptr) {
        std::cerr << "[ERROR] Failed to allocate stream object for null-handle test.\n";
        return false;
    }

    w->hf = nullptr; // NULL handle
    void* opaque = nullptr;
    void* stream = (void*)w;

    int ret = win32_close_file_func(opaque, stream);

    bool ok = true;
    ok &= assert_int_equal("test_win32_close_file_with_null_handle.ret", ret, -1);
    ok &= assert_int_equal("test_win32_close_file_with_null_handle.close_calls", g_close_handle_calls, 0);
    ok &= assert_ptr_equal("test_win32_close_file_with_null_handle.last_handle", g_last_closed_handle, nullptr);

    // Free attempt: the function should have freed the stream, so we should not free again here.
    return ok;
}

// ---------------------------------------------------------------------------
// Main: Run all tests
// ---------------------------------------------------------------------------

int main() {
    std::cout << "Starting unit tests for win32_close_file_func...\n";

    int total_failures = 0;

    if (!test_win32_close_file_null_stream()) ++total_failures;
    if (!test_win32_close_file_with_valid_handle()) ++total_failures;
    if (!test_win32_close_file_with_null_handle()) ++total_failures;

    if (total_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << total_failures << " TEST(S) FAILED\n";
        return total_failures;
    }
}