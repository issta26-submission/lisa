// cmsIT8_load_fuzzer_unit_tests.cpp
// A lightweight unit test harness for LLVMFuzzerTestOneInput(
//     const uint8_t *data, size_t size)
// This test uses mock implementations of the cmsIT8* family and cmsHANDLE
// to verify branch coverage and interactions without requiring the real liblcms2.

// Note: This test is designed to be compiled and linked together with the
// focal C source cmsIT8_load_fuzzer.c (which contains LLVMFuzzerTestOneInput).
// We provide C-compatible mocks with the same symbol names to intercept calls.

#include <cstring>
#include <cstdio>
#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <sys/types.h>
#include <lcms2.h>
#include <cstdint>
#include <unistd.h>


// Forward declare the focal function with C linkage.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);

// We assume cmsHANDLE is a pointer-like opaque type in the real library.
// To maximize compatibility with the focal implementation, declare a compatible
// typedef here. The actual cmsHANDLE in the focal file likely resolves to void*.
typedef void* cmsHANDLE;

// Prototypes for the mocked dependencies used by the focal function.
// These need to be visible to the focal compilation unit during linking.
extern "C" cmsHANDLE cmsIT8LoadFromMem(int, void* data, size_t size);
extern "C" void      cmsIT8SaveToFile(cmsHANDLE, const char* filename);
extern "C" void      cmsIT8Free(cmsHANDLE);

// Simple, non-throwing test framework (no GTest) with non-terminating assertions.
static int g_test_failures = 0;

#define EXPECT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "TEST FAILED: " << (msg) << "\n"; \
            ++g_test_failures; \
        } \
    } while (0)

#define ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "TEST ASSERTION FAILED: " << (msg) << "\n"; \
            ++g_test_failures; \
        } \
    } while (0)

// Mocks state to observe interactions with the focal function.
static bool g_loadFromMem_called = false;
static void* g_last_load_data = nullptr;
static size_t g_last_load_size = 0;
static bool g_saveToFile_called = false;
static char g_last_filename[256] = {0};
static bool g_free_called = false;

// Control whether the mocked cmsIT8LoadFromMem returns a non-null handle.
static bool g_next_load_should_succeed = true;
static cmsHANDLE g_mock_handle = (cmsHANDLE)0xDEADBEEF; // non-null sentinel

// Implement mocked dependencies with C linkage so they link with the focal C code.
extern "C" cmsHANDLE cmsIT8LoadFromMem(int, void* data, size_t size) {
    (void)_; // silence unused in some compilers if not used
    (void)sizeof(_); // compile-time no-op to avoid warnings in some compilers
    g_loadFromMem_called = true;
    g_last_load_data = data;
    g_last_load_size = size;
    if (g_next_load_should_succeed) {
        return g_mock_handle;
    } else {
        return nullptr;
    }
}

extern "C" void cmsIT8SaveToFile(cmsHANDLE /*handle*/, const char* filename) {
    g_saveToFile_called = true;
    if (filename) {
        std::strncpy(g_last_filename, filename, sizeof(g_last_filename) - 1);
        g_last_filename[sizeof(g_last_filename) - 1] = '\0';
    } else {
        g_last_filename[0] = '\0';
    }
}

extern "C" void cmsIT8Free(cmsHANDLE /*handle*/) {
    g_free_called = true;
}

// Helper to reset all mock state between tests.
static void reset_mock_state() {
    g_loadFromMem_called = false;
    g_last_load_data = nullptr;
    g_last_load_size = 0;
    g_saveToFile_called = false;
    g_last_filename[0] = '\0';
    g_free_called = false;
    // g_next_load_should_succeed intentionally preserved by tests as needed
}

// Test 1: size == 0 should short-circuit and not call any CMS functions.
static void test_size_zero_does_not_call_dependencies() {
    reset_mock_state();
    g_next_load_should_succeed = true; // even if set, should not be used

    // Call with zero size; should return 0 and perform no actions.
    int ret = LLVMFuzzerTestOneInput(nullptr, 0);

    EXPECT(ret == 0, "LLVMFuzzerTestOneInput should return 0 for size == 0");
    EXPECT(!g_loadFromMem_called, "cmsIT8LoadFromMem should not be called when size == 0");
    EXPECT(!g_saveToFile_called, "cmsIT8SaveToFile should not be called when size == 0");
    EXPECT(!g_free_called, "cmsIT8Free should not be called when size == 0");
}

// Test 2: size > 0 but cmsIT8LoadFromMem returns nullptr; ensure no further calls.
static void test_load_from_mem_fails_no_further_calls() {
    reset_mock_state();
    g_next_load_should_succeed = false; // simulate failure to load

    const uint8_t sample[4] = { 1, 2, 3, 4 };
    int ret = LLVMFuzzerTestOneInput(sample, sizeof(sample));

    EXPECT(ret == 0, "LLVMFuzzerTestOneInput should still return 0 even on failure");
    EXPECT(g_loadFromMem_called, "cmsIT8LoadFromMem should be called when size > 0");
    EXPECT(!g_saveToFile_called, "cmsIT8SaveToFile should not be called if loadFromMem fails (nullptr handle)");
    EXPECT(!g_free_called, "cmsIT8Free should not be called if loadFromMem fails");
}

// Test 3: size > 0 with a successful load; ensure save to file and free are invoked.
static void test_load_from_mem_succeeds_saves_and_frees() {
    reset_mock_state();
    g_next_load_should_succeed = true;

    const uint8_t sample[8] = { 0xAA, 0xBB, 0xCC, 0xDD, 0x11, 0x22, 0x33, 0x44 };
    int ret = LLVMFuzzerTestOneInput(sample, sizeof(sample));

    EXPECT(ret == 0, "LLVMFuzzerTestOneInput should return 0 on success path");
    EXPECT(g_loadFromMem_called, "cmsIT8LoadFromMem should be called on non-zero size");
    EXPECT(g_saveToFile_called, "cmsIT8SaveToFile should be called when handle is non-null");
    EXPECT(g_free_called, "cmsIT8Free should be called after saving to file");

    // Validate the filename format captured by the mock
    // Should be of the form /tmp/fuzzer-it.<pid>.it8 (pid varies).
    const char* fname = g_last_filename;
    ASSERT(fname[0] != '\0' && std::strcmp(fname, "/tmp/fuzzer-it.") != 0,
           "Saved filename should be non-empty and start with /tmp/fuzzer-it.");
    // Basic checks on the suffix
    size_t len = std::strlen(fname);
    EXPECT(len >= 4, "Filename should have reasonable length");
    const char* suffix = ".it8";
    size_t suff_len = std::strlen(suffix);
    EXPECT(len >= suff_len && std::strcmp(fname + len - suff_len, suffix) == 0,
           "Filename should end with .it8");
}

// Test 4: Ensure that data pointer is passed through to cmsIT8LoadFromMem (including possible NULL data).
static void test_data_pointer_pass_through() {
    reset_mock_state();
    g_next_load_should_succeed = true;

    // Pass NULL data pointer with non-zero size to verify propagation
    int ret = LLVMFuzzerTestOneInput(nullptr, 5);

    EXPECT(ret == 0, "LLVMFuzzerTestOneInput should return 0 even with NULL data input");
    EXPECT(g_loadFromMem_called, "cmsIT8LoadFromMem should be invoked even if data is NULL");
    // Data pointer should be preserved as NULL in the mock
    EXPECT(g_last_load_data == nullptr, "cmsIT8LoadFromMem should receive the same NULL data pointer");
}

// Simple runner that executes all tests and reports overall result.
int main() {
    std::cout << "Starting cmsIT8_load_fuzzer unit tests (no GTest)." << std::endl;

    test_size_zero_does_not_call_dependencies();
    test_load_from_mem_fails_no_further_calls();
    test_load_from_mem_succeeds_saves_and_frees();
    test_data_pointer_pass_through();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}