// Unit test suite for FetchInfo in fuzzers.c without using GTest.
// This test suite provides minimal C++11-based tests with a small in-house
// assertion framework and lightweight mocks for the cms library IO/memory APIs.
// The production function FetchInfo uses cmsGetProfileInfo, _cmsMalloc and
// _cmsFree. We provide C-linkage mock implementations to exercise true/false
// branches and memory flow.
//
// How to run (example):
// - Compile fuzzers.c along with this test file, linking with the objects below.
// - Ensure the test's mocks are linked in before the object containing FetchInfo.
// - Run the executable; it will report test pass/fail based on internal assertions.

#include <lcms2_plugin.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>
#include <cwchar>


// Domain knowledge: Use C++11 standard library only for test scaffolding.

// Forward-declare the focal function with C linkage to avoid name mangling issues.
extern "C" void FetchInfo(void* h, int Info);

// Typedefs mirroring the minimal API used by FetchInfo (as in the focal file).
using cmsHPROFILE = void*;
using cmsInfoType = int;
using cmsInt32Number = int;
using cmsContext = void*;

// Global state for mocks to control behavior and capture interactions.
static int g_mode = 0;                     // 0 = zero-length path, 1 = positive-length path
static bool g_malloc_called = false;
static bool g_free_called = false;
static int g_text_calls = 0;
static std::wstring g_capturedBuffer;       // Captured content from _cmsFree

// Helpers to reset test environment
static void resetMocks() {
    g_mode = 0;
    g_malloc_called = false;
    g_free_called = false;
    g_text_calls = 0;
    g_capturedBuffer.clear();
}

// Set test mode: 0 -> FetchInfo should see len == 0 and return early.
//            1 -> first call returns non-zero length; second call with text -> allocate+free.
static void setMode(int mode) {
    g_mode = mode;
    resetMocks();
}

// Mocks: Provide C linkage so that the linked fuzzers.c uses these definitions.
extern "C" {

// Mock for _cmsMalloc: track usage and allocate wchar_t buffer of requested length.
void* _cmsMalloc(cmsContext id, cmsInt32Number size) {
    (void)id; // unused in tests
    g_malloc_called = true;
    // Allocate as wchar_t buffer to align with FetchInfo's usage.
    return new wchar_t[size];
}

// Mock for _cmsFree: capture content (if any) before freeing buffer.
void _cmsFree(cmsContext id, void* ptr) {
    (void)id;
    g_free_called = true;

    if (ptr == nullptr) return;

    wchar_t* wbuf = static_cast<wchar_t*>(ptr);
    // Determine length by searching for a terminator within a reasonable bound
    size_t len = 0;
    while (len < 256 && wbuf[len] != L'\0') ++len;

    // Store captured content (if any) for assertion in tests.
    if (len > 0) {
        g_capturedBuffer.assign(wbuf, wbuf + len);
    } else {
        g_capturedBuffer.clear();
    }

    delete[] wbuf;
}

// Mock for cmsGetProfileInfo: emulate length query and fill buffer on second call.
// Behavior controlled by g_mode:
 // - If text == NULL && len == 0: return 0 (mode 0) or a non-zero length (mode 1).
 // - If text != NULL: fill buffer with predictable pattern and terminate buffer.
cmsInt32Number cmsGetProfileInfo(cmsHPROFILE h, cmsInfoType Info,
                                 const wchar_t* Lang, const wchar_t* Country,
                                 wchar_t* text, cmsInt32Number len) {
    (void)h; (void)Info; (void)Lang; (void)Country;
    (void)Lang; (void)Country;

    // Initial info query: determine required length
    if (text == nullptr && len == 0) {
        if (g_mode == 1) {
            // Return a non-zero length to exercise the allocation path
            return 5; // allocate space for 5 characters (plus terminator logic in test)
        } else {
            // Mode 0: simulate zero-length/early-return condition
            return 0;
        }
    }

    // Second call: text buffer provided; fill with a deterministic pattern.
    if (text != nullptr) {
        const wchar_t* sample = L"HELLO";
        size_t sample_len = wcslen(sample);
        // len is the buffer size requested by caller; ensure we do not overflow.
        // We fill up to len-1 characters, and put a terminator at len-1.
        size_t n = static_cast<size_t>(len);
        if (n == 0) {
            text[0] = L'\0';
            return len;
        }
        for (size_t i = 0; i < n - 1; ++i) {
            text[i] = (i < sample_len) ? sample[i] : L' ';
        }
        text[n - 1] = L'\0'; // terminator to enable safe length-detection in _cmsFree
        // Indicate the number of characters effectively written
        return len;
    }

    return 0;
}
} // extern "C"

// Lightweight assertion framework (non-terminating)
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        ++g_failures; \
        std::cerr << "Test failure: " << (msg) << " (File: " << __FILE__ \
                  << ", Line: " << __LINE__ << ")\n"; \
    } \
} while(0)

// Test 1: Verifies that when the first cmsGetProfileInfo call returns 0 length,
// FetchInfo should not attempt memory allocation or freeing.
static void test_fetchinfo_len_zero_branch() {
    setMode(0); // mode 0: len should be 0
    // Call with dummy handles
    FetchInfo(nullptr, 0); // Info value is arbitrary here

    TEST_ASSERT(!g_malloc_called, "FetchInfo should not call _cmsMalloc when len==0");
    TEST_ASSERT(!g_free_called, "FetchInfo should not call _cmsFree when no allocation occurred");
    TEST_ASSERT(g_text_calls == 0, "No text-cmsGetProfileInfo calls should occur for len==0");
    TEST_ASSERT(g_capturedBuffer.empty(), "No captured buffer should exist for len==0");
}

// Test 2: Verifies that when the first cmsGetProfileInfo call returns non-zero length,
// FetchInfo should allocate memory, perform the second call to fill text, and free it.
static void test_fetchinfo_len_positive_allocates_and_frees() {
    setMode(1); // mode 1: initial length > 0
    // Reset captured state
    g_text_calls = 0;
    g_capturedBuffer.clear();
    g_malloc_called = false;
    g_free_called = false;

    FetchInfo(nullptr, 0); // Info value is arbitrary here

    TEST_ASSERT(g_malloc_called, "_cmsMalloc should be called when non-zero length is returned");
    TEST_ASSERT(g_free_called, "_cmsFree should be called after filling the buffer");
    TEST_ASSERT(g_text_calls == 1, "cmsGetProfileInfo should be called a second time with non-null text");
    // Verify captured content (should be "HELL" due to 5-char mode with terminator at last pos)
    TEST_ASSERT(!g_capturedBuffer.empty(), "Captured buffer should contain data written by cmsGetProfileInfo");
    // We expect the content to start with "HELL" based on our mock's behavior
    if (!g_capturedBuffer.empty()) {
        if (g_capturedBuffer.find(L"HELL") != 0) {
            std::cerr << "Warning: Captured buffer does not start with expected 'HELL' prefix. Actual content: '"
                      << std::string(g_capturedBuffer.begin(), g_capturedBuffer.end())
                      << "'\n";
        } else {
            // Optional success message for clarity
            std::cout << "Captured content starts with expected 'HELL' prefix.\n";
        }
    }
}

// Simple test runner
static void runAllTests() {
    test_fetchinfo_len_zero_branch();
    test_fetchinfo_len_positive_allocates_and_frees();
}

// Entry point
int main() {
    // Run tests
    runAllTests();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
}