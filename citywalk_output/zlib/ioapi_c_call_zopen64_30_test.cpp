// Lightweight C++11 unit test suite for the focal method: call_zopen64
// This test suite does not use GoogleTest. It provides two focused test cases
// that verify both branches of the internal conditional in call_zopen64.

#include <cstring>
#include <string>
#include <cstdint>
#include <iostream>
#include <ioapi.h>


// ------------------------
// Minimal type definitions to mirror the focal code dependencies
// ------------------------

// void pointer type used by the C-style API
typedef void* voidpf;

// Structure mirroring the nested zfile_func64 struct plus the zopen32_file pointer
struct zfile_func64_def {
    voidpf (*zopen64_file)(voidpf opaque, const void* filename, int mode);
    voidpf opaque;
};

// High-level file function structure containing both 64-bit and 32-bit open hooks
struct zlib_filefunc64_32_def {
    zfile_func64_def zfile_func64;
    voidpf (*zopen32_file)(voidpf opaque, const char* filename, int mode);
};

// ------------------------
// Focal method under test
// ------------------------
// This is the same logic as provided in the focal method.
// If zopen64_file is non-NULL, call it with opaque, filename, mode.
// Otherwise, fall back to zopen32_file with opaque and filename cast to const char*.
voidpf call_zopen64 (const zlib_filefunc64_32_def* pfilefunc, const void* filename, int mode) {
    if (pfilefunc->zfile_func64.zopen64_file != NULL)
        return (*(pfilefunc->zfile_func64.zopen64_file)) (pfilefunc->zfile_func64.opaque, filename, mode);
    else
    {
        return (*(pfilefunc->zopen32_file))(pfilefunc->zfile_func64.opaque, (const char*)filename, mode);
    }
}

// ------------------------
// Test scaffolding: mocks and records
// ------------------------

// Global (static) records to verify call parameters for 64-bit open
static void* g_last_opaque64 = nullptr;
static const void* g_last_filename64 = nullptr;
static int g_last_mode64 = 0;

// Global (static) records to verify call parameters for 32-bit open
static void* g_last_opaque32 = nullptr;
static const char* g_last_filename32 = nullptr;
static int g_last_mode32 = 0;

// Mock 64-bit zopen64_file: records parameters and returns a fixed sentinel handle
static void* test64_open(void* opaque, const void* filename, int mode) {
    g_last_opaque64 = opaque;
    g_last_filename64 = filename;
    g_last_mode64 = mode;
    // Return a distinctive non-null pointer to verify the wrapper returns it unchanged
    return reinterpret_cast<void*>(0x12345678);
}

// Mock 32-bit zopen32_file: records parameters and returns a different sentinel
static void* test32_open(void* opaque, const char* filename, int mode) {
    g_last_opaque32 = opaque;
    g_last_filename32 = filename;
    g_last_mode32 = mode;
    // Return a distinctive non-null pointer separate from the 64-bit mock
    return reinterpret_cast<void*>(0x9ABCDEF0);
}

// ------------------------
// Unit Tests
// ------------------------

// Test 1: When zopen64_file is provided (non-NULL), call_zopen64 should use it.
// Verifies that the return value is the 64-bit mock's result and that all
// parameters (opaque, filename, mode) are passed through correctly.
bool test_call_zopen64_uses_zopen64_when_present() {
    // Prepare a pfilefunc with a valid 64-bit open function
    zlib_filefunc64_32_def pfunc;
    pfunc.zfile_func64.zopen64_file = test64_open;
    pfunc.zfile_func64.opaque = reinterpret_cast<void*>(0xA1A2A3A4);
    pfunc.zopen32_file = nullptr; // Ensure we don't use the 32-bit fallback

    char filename[] = "file64_open_test.txt";
    int mode = 3;

    // Reset records
    g_last_opaque64 = nullptr;
    g_last_filename64 = nullptr;
    g_last_mode64 = 0;

    void* result = call_zopen64(&pfunc, static_cast<const void*>(filename), mode);

    // Expected: 64-bit mock returns its sentinel, parameters are as provided
    bool ok = (result == reinterpret_cast<void*>(0x12345678)) &&
              (g_last_opaque64 == pfunc.zfile_func64.opaque) &&
              (g_last_filename64 == static_cast<const void*>(filename)) &&
              (g_last_mode64 == mode);

    // Also verify that 64-bit path was indeed taken (by ensuring the 32-bit last call remains untouched)
    // Since we initialized last_opaque32/filename32 to nullptr and the 32-bit mock wasn't called,
    // their values should be unchanged (nullptr).
    bool untouched32 = (g_last_opaque32 == nullptr) && (g_last_filename32 == nullptr) && (g_last_mode32 == 0);

    return ok && untouched32;
}

// Test 2: When zopen64_file is NULL, call_zopen64 should fall back to zopen32_file.
// Verifies that the 32-bit mock is called with the opaque and filename cast to const char*,
// and that the wrapper returns the 32-bit mock's result.
bool test_call_zopen64_uses_zopen32_when_null() {
    // Prepare a pfilefunc where 64-bit open is NULL and 32-bit open is provided
    zlib_filefunc64_32_def pfunc;
    pfunc.zfile_func64.zopen64_file = nullptr; // Force fallback to 32-bit
    pfunc.zfile_func64.opaque = reinterpret_cast<void*>(0x5A5A5A5A);
    pfunc.zopen32_file = test32_open;

    const char filename[] = "file32_open_test.txt";
    int mode = 7;

    // Reset records
    g_last_opaque32 = nullptr;
    g_last_filename32 = nullptr;
    g_last_mode32 = 0;

    void* result = call_zopen64(&pfunc, static_cast<const void*>(filename), mode);

    // Expected: 32-bit mock returns its sentinel, parameters are as provided
    bool ok = (result == reinterpret_cast<void*>(0x9ABCDEF0)) &&
              (g_last_opaque32 == pfunc.zfile_func64.opaque) &&
              (g_last_filename32 == filename) && // cast to const char* is same as filename
              (g_last_mode32 == mode);

    // Ensure 64-bit path was not used (its function pointer remains NULL)
    bool unused64 = (pfunc.zfile_func64.zopen64_file == nullptr);

    return ok && unused64;
}

// ------------------------
// Simple test harness (no GTest; plain stdout reporting)
// ------------------------

int main() {
    int tests_run = 0;
    int tests_failed = 0;

    auto runTest = [&](const std::string& name, bool ok) {
        tests_run++;
        if (ok) {
            std::cout << "[PASS] " << name << std::endl;
        } else {
            tests_failed++;
            std::cerr << "[FAIL] " << name << std::endl;
        }
    };

    // Run tests
    runTest("test_call_zopen64_uses_zopen64_when_present", test_call_zopen64_uses_zopen64_when_present());
    runTest("test_call_zopen64_uses_zopen32_when_null", test_call_zopen64_uses_zopen32_when_null());

    // Summary
    std::cout << "Total tests: " << tests_run << ", Failures: " << tests_failed << std::endl;
    // Return non-zero if any test failed to aid CI in detecting failure
    return (tests_failed == 0) ? 0 : 1;
}