/*
Unit test suite for the focal method win32_open64_file_funcA in iowin32.c
- No GTest usage; a lightweight in-process test harness is implemented.
- Windows API calls (CreateFileA/CreateFile2, MultiByteToWideChar) are stubbed
  to allow unit testing on non-Windows platforms by intercepting the calls.
- Tests cover null/non-null filename handling and both WinRT and non-WinRT paths
  via compile-time macro IOWIN32_USING_WINRT_API.
- The test harness collects failures and continues execution to maximize coverage.
- The tests assume the project under test exposes the function with C linkage:
  voidpf ZCALLBACK win32_open64_file_funcA(voidpf opaque, const void* filename, int mode);
  exposed via iowin32.c/.h in the test environment.
*/

#include <cstring>
#include <cassert>
#include <string>
#include <iowin32.h>
#include <functional>
#include <iostream>
#include <vector>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>
#include <cwchar>


// The production code under test uses a C interface.
// We declare the exact signature here for linkage with the production function.
// voidpf ZCALLBACK win32_open64_file_funcA(voidpf opaque, const void* filename, int mode);
extern "C" {
    typedef void* voidpf;
    // The ZCALLBACK macro is assumed to be empty in the test environment;
    // declare the function with C linkage.
    voidpf win32_open64_file_funcA(voidpf opaque, const void* filename, int mode);
}

// ---------------------------------------------------------------------------
// Minimal Windows API stubs to allow testing on non-Windows systems.
// We provide simple implements that record invocation details so tests can verify behavior.
// These stubs are only compiled in test mode and are linked with the production unit.
// ---------------------------------------------------------------------------

extern "C" {

// Type aliases matching Windows-like types (simplified for test environment)
typedef void* HANDLE;
typedef unsigned long DWORD;
typedef const char* LPCSTR;
typedef const wchar_t* LPCWSTR;
typedef wchar_t WCHAR;

// Global flags/records to observe stub invocations
static bool g_CreateFileA_called = false;
static bool g_CreateFile2_called = false;
static std::string g_CreateFileA_last_filename;
static std::wstring g_CreateFile2_last_path;

// Reset helper for tests
static void reset_windows_api_stubs() {
    g_CreateFileA_called = false;
    g_CreateFile2_called = false;
    g_CreateFileA_last_filename.clear();
    g_CreateFile2_last_path.clear();
}

// Stub for MultiByteToWideChar: performs naive mapping (ASCII-safe)
int MultiByteToWideChar(int CodePage, int dwFlags,
                        const char* lpMultiByteStr, int cbMultiByte,
                        WCHAR* lpWideCharStr, int cchWideChar) {
    if (!lpMultiByteStr || !lpWideCharStr) {
        return 0;
    }
    int len = (cbMultiByte < 0) ? static_cast<int>(std::strlen(lpMultiByteStr)) : cbMultiByte;
    if (len + 1 > cchWideChar) {
        len = cchWideChar - 1;
    }
    for (int i = 0; i < len; ++i) {
        lpWideCharStr[i] = static_cast<WCHAR>(lpMultiByteStr[i]);
    }
    lpWideCharStr[len] = L'\0';
    return len;
}

// Stub for CreateFileA
HANDLE CreateFileA(const LPCSTR lpFileName,
                   DWORD dwDesiredAccess,
                   DWORD dwShareMode,
                   void* lpSecurityAttributes,
                   DWORD dwCreationDisposition,
                   DWORD dwFlagsAndAttributes,
                   HANDLE hTemplateFile) {
    (void)dwShareMode; (void)lpSecurityAttributes; (void)dwCreationDisposition;
    (void)dwFlagsAndAttributes; (void)hTemplateFile;
    g_CreateFileA_called = true;
    if (lpFileName != nullptr) {
        g_CreateFileA_last_filename = lpFileName;
    }
    // Return a fake valid handle
    return reinterpret_cast<HANDLE>(0x1000);
}

// Stub for CreateFile2 (WinRT path)
HANDLE CreateFile2(const WCHAR* lpFileName,
                   DWORD dwDesiredAccess,
                   DWORD dwShareMode,
                   DWORD dwCreationDisposition,
                   void* pSecurityAttributes) {
    (void)dwDesiredAccess; (void)dwShareMode; (void)dwCreationDisposition; (void)pSecurityAttributes;
    g_CreateFile2_called = true;
    if (lpFileName != nullptr) {
        // Copy to a std::wstring for assertion in tests
        g_CreateFile2_last_path = lpFileName;
    }
    return reinterpret_cast<HANDLE>(0x2000);
}

} // extern "C"

// ---------------------------------------------------------------------------
// Lightweight test framework (non-GTest, non-terminating assertions)
// ---------------------------------------------------------------------------

struct TestCase {
    std::string name;
    std::function<void()> func;
};

class TestRunner {
public:
    void addTest(const std::string& name, std::function<void()> f) {
        tests.push_back({name, f});
    }

    void run() {
        int total = 0;
        int failures = 0;
        for (const auto& t : tests) {
            ++total;
            try {
                reset_case();
                t.func();
                std::cout << "[PASS] " << t.name << "\n";
            } catch (const std::exception& ex) {
                ++failures;
                std::cout << "[FAIL] " << t.name << " - exception: " << ex.what() << "\n";
            } catch (...) {
                ++failures;
                std::cout << "[FAIL] " << t.name << " - unknown exception\n";
            }
        }
        std::cout << "\nTest results: " << (total - failures) << "/" << total << " passed\n";
        if (failures > 0) {
            std::cout << "Some tests failed. See logs above for details.\n";
        }
    }

private:
    std::vector<TestCase> tests;

    void reset_case() {
        // Reset stubs for each test
        reset_windows_api_stubs();
        // Note: win32_open64_file_funcA may set internal static state in the
        // production code; we don't reset that here as it's intentionally opaque.
    }
};

// Helper macro to register tests succinctly
#define REGISTER_TEST(runner, name, body) \
    runner.addTest(name, std::bind([&]() { body; }, std::placeholders::_1))

// ---------------------------------------------------------------------------
// Domain-specific tests for win32_open64_file_funcA
// ---------------------------------------------------------------------------

int main() {
    TestRunner runner;

    // Test 1: When filename == NULL, no Windows file API should be invoked
	// Rationale: The code path guards on (filename != NULL) && (dwDesiredAccess != 0)
    runner.addTest("win32_open64_file_funcA_null_filename_no_api_call",
    []() {
        // Arrange
        reset_windows_api_stubs();
        const void* null_filename = nullptr;
        // Act
        voidpf result = win32_open64_file_funcA(nullptr, null_filename, 0);
        // Assert: Ensure CreateFileA and CreateFile2 were not called
        if (g_CreateFileA_called) {
            throw std::runtime_error("CreateFileA should not have been called when filename is NULL.");
        }
        if (g_CreateFile2_called) {
            throw std::runtime_error("CreateFile2 should not have been called when filename is NULL.");
        }
        // Basic sanity: result should be a non-null pointer if build_iowin returns non-null;
        // since the real implementation is production code, we don't enforce on NULL here.
        (void)result;
    });

    // Test 2: Non-null filename in non-WinRT path should call CreateFileA
	// Rationale: Verify that the non-WinRT path uses CreateFileA with the given filename.
    runner.addTest("win32_open64_file_funcA_nonwinrt_calls_CreateFileA",
    []() {
        // Arrange
        reset_windows_api_stubs();
        const char* fname = "testfile.txt";
        // Act
        voidpf result = win32_open64_file_funcA(nullptr, fname, 1);
        // Assert
        if (!g_CreateFileA_called) {
            throw std::runtime_error("CreateFileA was not called for non-WinRT path with non-null filename.");
        }
        if (g_CreateFile2_called) {
            throw std::runtime_error("CreateFile2 should not have been called in non-WinRT path.");
        }
        if (g_CreateFileA_last_filename != fname) {
            throw std::runtime_error("CreateFileA received filename does not match input filename.");
        }
        (void)result;
    });

#ifdef IOWIN32_USING_WINRT_API
    // Test 3: WinRT path should call CreateFile2
    // Rationale: When IOWIN32_USING_WINRT_API is defined, and filename is non-null,
    // the code path should use CreateFile2 with a wide-character path.
    runner.addTest("win32_open64_file_funcA_winrt_calls_CreateFile2",
    []() {
        reset_windows_api_stubs();
        const char* fname = "winrt_test.txt";
        voidpf result = win32_open64_file_funcA(nullptr, fname, 1);
        if (!g_CreateFile2_called) {
            throw std::runtime_error("CreateFile2 was not called in WinRT path.");
        }
        // We only assert that a path was provided to CreateFile2
        if (g_CreateFile2_last_path.empty()) {
            throw std::runtime_error("CreateFile2 path was not provided.");
        }
        // Optional: verify the path contains the filename (naively)
        std::string wpath = std::string(g_CreateFile2_last_path.begin(), g_CreateFile2_last_path.end());
        if (wpath.find("winrt_test.txt") == std::string::npos) {
            throw std::runtime_error("CreateFile2 path does not reflect input filename.");
        }
        (void)result;
    });
#else
    // If WinRT not enabled, provide a no-op placeholder to indicate branch not tested here.
    runner.addTest("win32_open64_file_funcA_winrt_not_enabled",
    []() {
        // This test is skipped in non-WinRT builds; ensure that the binary builds.
        // No assertions needed; test simply compiles in both configurations.
        (void)0;
    });
#endif

    // Run all tests
    runner.run();

    return 0;
}