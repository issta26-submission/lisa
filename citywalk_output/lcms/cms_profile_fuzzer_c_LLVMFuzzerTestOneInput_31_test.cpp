// Lightweight C++11 test harness for the focal method
// This harness mocks the LCMS2 library (lcms2.h) functions to exercise
// the LLVMFuzzerTestOneInput path without requiring the real library.
// It uses a small in-process test runner (no GTest) and prints a summary.

#include <cstring>
#include <cstdio>
#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <lcms2.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstdint>


// Forward declaration of the focal function (to be linked with cms_profile_fuzzer.c)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);

// Global test result counters
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Stub counters to observe internal calls (for basic traceability)
static int g_stub_open_r_calls = 0;
static int g_stub_open_w_calls = 0;

// ---------------------------------------------------------------------------
// Lightweight stubs for LCMS2-like API (to isolate from real library)
// We provide C linkage so the focal function can link against them.
// The stubs are intentionally simple and non-fatal to maximize coverage.
// ---------------------------------------------------------------------------

extern "C" {

// Type aliases compatible with typical LCMS2 typedefs (simplified)
typedef void* cmsHPROFILE;
typedef unsigned int cmsTagSignature;
typedef unsigned int cmsUInt32Number;
typedef int cmsInfoType;

// Array of tags to read (complements sizeof(tagsToRead)/sizeof(tagsToRead[0]))
const cmsTagSignature tagsToRead[] = { 0x11111111, 0x22222222, 0x33333333 };

// Open a profile from a file. We simulate success for both "r" and "w" modes.
cmsHPROFILE cmsOpenProfileFromFile(const char* filename, const char* mode) {
    // Track how many times we open in read vs write mode to aid test assertions
    if (mode && mode[0] == 'r') {
        ++g_stub_open_r_calls;
    } else if (mode && mode[0] == 'w') {
        ++g_stub_open_w_calls;
    }
    // Return a non-null dummy handle to exercise succeeding branches
    return (cmsHPROFILE) (void*) 0x1;
}

// Close a profile (no-op in stub)
void cmsCloseProfile(cmsHPROFILE h) {
    (void)h;
}

// Save profile to a file (no-op in stub)
int cmsSaveProfileToFile(cmsHPROFILE h, const char* filename) {
    (void)h;
    (void)filename;
    return 0;
}

// Read a raw tag (fill buffer if provided)
int cmsReadRawTag(cmsHPROFILE h, cmsTagSignature tag, void* buffer, size_t size) {
    (void)h; (void)tag;
    if (buffer && size == 4) {
        unsigned char* p = reinterpret_cast<unsigned char*>(buffer);
        // Fill with a recognizable pattern
        p[0] = 0xAA; p[1] = 0xBB; p[2] = 0xCC; p[3] = 0xDD;
    }
    return 1; // pretend success
}

// Read a tag (no-op)
void cmsReadTag(cmsHPROFILE h, cmsTagSignature tag) {
    (void)h; (void)tag;
}

// Get profile ASCII info (fill a small buffer)
int cmsGetProfileInfoASCII(cmsHPROFILE h, cmsInfoType info, const char* inTag, const char* outTag, char* outBuffer, int max) {
    (void)h; (void)info; (void)inTag; (void)outTag;
    if (outBuffer && max > 0) {
        const char* msg = "OK";
        size_t len = strlen(msg);
        if ((size_t)max > len) {
            memcpy(outBuffer, msg, len + 1);
        } else if (max > 0) {
            outBuffer[0] = '\0';
        }
    }
    return 0;
}

// Get number of tags (stubbed)
unsigned int cmsGetTagCount(cmsHPROFILE h) {
    (void)h;
    return 0;
}

// Get a tag signature (stubbed)
cmsTagSignature cmsGetTagSignature(cmsHPROFILE h, cmsUInt32Number v) {
    (void)h; (void)v;
    return 0;
}

// Tag linked-to (no-op)
void cmsTagLinkedTo(cmsHPROFILE h, cmsTagSignature tag) {
    (void)h; (void)tag;
}

// End of LCMS2-like stubs
} // extern "C"


// Helpers for test results
static void report_test(bool ok, const char* name) {
    ++g_total_tests;
    if (!ok) {
        ++g_failed_tests;
        std::cerr << "[TEST FAILED] " << name << std::endl;
    } else {
        std::cout << "[TEST PASSED] " << name << std::endl;
    }
}

// Reset stub counters
static void reset_stub_counters() {
    g_stub_open_r_calls = 0;
    g_stub_open_w_calls = 0;
}

// Accessors for stub counters (optional for assertions)
static int get_stub_open_r_calls() { return g_stub_open_r_calls; }
static int get_stub_open_w_calls() { return g_stub_open_w_calls; }

// ---------------------------------------------------------------------------
// Test cases
// Each test calls LLVMFuzzerTestOneInput with different inputs and
// checks that the function returns and completes without crashing.
// We avoid terminating assertions; instead we record failures and continue.
// ---------------------------------------------------------------------------

void test_zero_size_input() {
    // size == 0 should return immediately
    const uint8_t* data = nullptr;
    int ret = LLVMFuzzerTestOneInput(data, 0);
    report_test(ret == 0, "LLVMFuzzerTestOneInput handles size==0");
}

void test_small_input_no_crash() {
    // Small input should not crash; exercise early exit path if size > 0 but not enough data
    uint8_t data[1] = { 0x01 };
    int ret = LLVMFuzzerTestOneInput(data, 1);
    report_test(ret == 0, "LLVMFuzzerTestOneInput handles small input (size=1)");
}

void test_large_input_path_branches() {
    // Large input to exercise more branches (size > 40 and reads).
    uint8_t data[50];
    // Initialize with deterministic bytes; ensure data[0], data[1], and data[5..8] exist
    for (size_t i = 0; i < sizeof(data); ++i) data[i] = static_cast<uint8_t>(i & 0xFF);
    // Ensure some non-zero for info = data[0] % 4
    data[0] = 7;     // info = 3
    data[1] = 0x12;  // tag read variant
    data[5] = 0x01;
    data[6] = 0x02;
    data[7] = 0x03;
    data[8] = 0x04;
    int ret = LLVMFuzzerTestOneInput(data, sizeof(data));
    report_test(ret == 0, "LLVMFuzzerTestOneInput large input path");
}

// Additional test to verify that the function can be invoked multiple times
void test_multiple_invocations() {
    reset_stub_counters();
    uint8_t data1[8] = {0,1,2,3,4,5,6,7};
    LLVMFuzzerTestOneInput(data1, 8);
    uint8_t data2[60];
    for (size_t i = 0; i < sizeof(data2); ++i) data2[i] = static_cast<uint8_t>(i);
    LLVMFuzzerTestOneInput(data2, sizeof(data2));
    // Basic assertion that libraries were opened at least once
    int opens_r = get_stub_open_r_calls();
    int opens_w = get_stub_open_w_calls();
    bool ok = (opens_r > 0) && (opens_w > 0);
    report_test(ok, "LLVMFuzzerTestOneInput multiple invocations hit open/read paths");
}


// ---------------------------------------------------------------------------
// Main driver
// ---------------------------------------------------------------------------

int main() {
    std::cout << "Starting unit tests for LLVMFuzzerTestOneInput (CMS profile fuzzer)\n";

    // Reset counts and run tests
    reset_stub_counters();
    test_zero_size_input();
    test_small_input_no_crash();
    test_large_input_path_branches();
    test_multiple_invocations();

    // Summary
    std::cout << "Test summary: " << g_total_tests << " tests run, "
              << g_failed_tests << " failures." << std::endl;

    return g_failed_tests != 0 ? 1 : 0;
}