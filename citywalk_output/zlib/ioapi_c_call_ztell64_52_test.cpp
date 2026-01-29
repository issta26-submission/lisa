// This test suite targets the focal method: call_ztell64 from the ioapi.c implementation.
// It verifies the two main branches:
// 1) When zfile_func64.zseek64_file is non-NULL, call its ztell64_file function.
// 2) When zfile_func64.zseek64_file is NULL, fall back to ztell32_file and handle MAXU32 edge.
// The tests are written for C++11, without a third-party test framework (no GTest), using a small
// custom assertion mechanism that does not terminate on failure.

#include <iostream>
#include <ioapi.h>
#include <cstdint>
#include <cstring>


extern "C" {
}

// Lightweight non-terminating assertion framework
static int g_test_failures = 0;

static void log_failure(const char* msg, ZPOS64_T actual, ZPOS64_T expected) {
    std::cerr << "TEST FAILURE: " << msg << " | actual: " << actual << " != expected: " << expected << std::endl;
    ++g_test_failures;
}

#define EXPECT_EQ(actual, expected, message) \
    do { if ((actual) != (expected)) log_failure((message), (actual), (expected)); } while (0)

#define RUN_TEST(test_func) \
    do { std::cout << "RUN: " << #test_func << std::endl; test_func(); std::cout << std::endl; } while (0)

// Forward declarations of test-specific C callbacks (need C linkage for function pointers)
extern "C" {

// 64-bit tell function used by the 64-bit function vector
ZPOS64_T ZCALLBACK test_ztell64(voidpf opaque, voidpf stream) {
    // Simple deterministic value to verify correct dispatch
    return static_cast<ZPOS64_T>(12345);
}

// 32-bit tell function used when 64-bit path is not available
long ZCALLBACK test_ztell32(voidpf opaque, voidpf stream) {
    return static_cast<long>(987);
}

// 64-bit seek function (not used for assertion, but required to fill the structure when testing 64-bit path)
long ZCALLBACK test_zseek64(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin) {
    return 0;
}

// 32-bit tell function that returns MAXU32 to exercise the MAXU32 edge case
long ZCALLBACK test_ztell32_max(voidpf opaque, voidpf stream) {
    return static_cast<long>(MAXU32);
}

} // extern "C"

// Test Case 1: 64-bit path active (zseek64_file non-NULL) should call ztell64_file
static void test_call_ztell64_64path() {
    zlib_filefunc64_32_def pfunc;              // 64-bit/32-bit def pair
    std::memset(&pfunc, 0, sizeof(pfunc));

    // Build 64-bit function vector with non-NULL zseek64_file to trigger 64-bit path
    pfunc.zfile_func64.opaque = reinterpret_cast<void*>(0xDEADBEEF);
    pfunc.zfile_func64.ztell64_file = test_ztell64; // non-NULL -> use 64-bit tell
    pfunc.zfile_func64.zseek64_file = test_zseek64; // non-NULL -> path 1

    // Build 32-bit function vector so that code is well-formed if accessed
    pfunc.zfile_func32.opaque = pfunc.zfile_func64.opaque;
    pfunc.zfile_func32.ztell32_file = test_ztell32;

    voidpf filestream = nullptr;
    ZPOS64_T result = call_ztell64(&pfunc, filestream);

    EXPECT_EQ(result, static_cast<ZPOS64_T>(12345),
              "call_ztell64 should dispatch to ztell64_file when zseek64_file is non-NULL");
}

// Test Case 2: 32-bit path when 64-bit path is unavailable (zseek64_file == NULL)
static void test_call_ztell64_32path_basic() {
    zlib_filefunc64_32_def pfunc;
    std::memset(&pfunc, 0, sizeof(pfunc));

    // Disable 64-bit path
    pfunc.zfile_func64.opaque = reinterpret_cast<void*>(0xA5A5A5A5);
    pfunc.zfile_func64.ztell64_file = nullptr;
    pfunc.zfile_func64.zseek64_file = nullptr;

    // Provide 32-bit tell function that returns a normal value
    pfunc.zfile_func32.opaque = pfunc.zfile_func64.opaque;
    pfunc.zfile_func32.ztell32_file = test_ztell32;

    voidpf filestream = nullptr;
    ZPOS64_T result = call_ztell64(&pfunc, filestream);

    EXPECT_EQ(result, static_cast<ZPOS64_T>(987),
              "call_ztell64 should fall back to ztell32_file when zseek64_file is NULL");
}

// Test Case 3: 32-bit path edge case where ztell32_file returns MAXU32, expecting (ZPOS64_T)-1
static void test_call_ztell64_32path_maxu32() {
    zlib_filefunc64_32_def pfunc;
    std::memset(&pfunc, 0, sizeof(pfunc));

    // Disable 64-bit path
    pfunc.zfile_func64.opaque = nullptr;
    pfunc.zfile_func64.ztell64_file = nullptr;
    pfunc.zfile_func64.zseek64_file = nullptr;

    // 32-bit tell returns MAXU32 to trigger the edge condition
    pfunc.zfile_func32.opaque = nullptr;
    pfunc.zfile_func32.ztell32_file = test_ztell32_max;

    voidpf filestream = nullptr;
    ZPOS64_T result = call_ztell64(&pfunc, filestream);

    // (ZPOS64_T)-1 corresponds to the maximum value for ZPOS64_T when MAXU32 is reached
    EXPECT_EQ(result, static_cast<ZPOS64_T>(-1),
              "call_ztell64 should return (ZPOS64_T)-1 if ztell32_file returns MAXU32");
}

int main() {
    // Step 3 (Test Case Refinement) - Run the targeted tests in sequence
    RUN_TEST(test_call_ztell64_64path);
    RUN_TEST(test_call_ztell64_32path_basic);
    RUN_TEST(test_call_ztell64_32path_maxu32);

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}