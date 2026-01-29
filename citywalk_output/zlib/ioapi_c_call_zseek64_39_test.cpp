// Test suite for the focal method: call_zseek64
// This test suite is designed to run without Google Test (GTest) and uses a lightweight
// in-process test harness with non-terminating assertions (EXPECT-like).
// It validates both branches of call_zseek64 in ioapi.c as described in the task.

#include <cstring>
#include <limits>
#include <cstdint>
#include <iostream>
#include <ioapi.h>


// Lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define LOG_MSG(msg) std::cout << "[TEST] " << msg << std::endl

// Helpers to print 64-bit values portably
static std::uint64_t to_uint64(ZPOS64_T v) {
    return static_cast<std::uint64_t>(v);
}
static std::uint64_t to_uint64(uLong v) {
    return static_cast<std::uint64_t>(v);
}

// Global capture variables for test callbacks
static voidpf g_zseek64_captured_opaque = nullptr;
static voidpf g_zseek64_captured_stream = nullptr;
static ZPOS64_T g_zseek64_captured_offset = 0;
static int     g_zseek64_captured_origin = 0;
static long    g_zseek64_cb_ret = -9999;

static voidpf g_zseek32_captured_opaque = nullptr;
static voidpf g_zseek32_captured_stream = nullptr;
static uLong   g_zseek32_captured_offset = 0;
static int     g_zseek32_captured_origin = 0;
static long    g_zseek32_cb_ret = -9999;

static bool    g_zseek32_was_called = false;

// Callback implementations that mimic zlib's file seek behaviors
static long ZCALLBACK test_zseek64_cb(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin) {
    g_zseek64_captured_opaque = opaque;
    g_zseek64_captured_stream = stream;
    g_zseek64_captured_offset = offset;
    g_zseek64_captured_origin = origin;
    g_zseek64_cb_ret = 12345; // expected return value from the branch
    return g_zseek64_cb_ret;
}

static long ZCALLBACK test_zseek32_cb(voidpf opaque, voidpf stream, uLong offset, int origin) {
    g_zseek32_was_called = true;
    g_zseek32_captured_opaque = opaque;
    g_zseek32_captured_stream = stream;
    g_zseek32_captured_offset = offset;
    g_zseek32_captured_origin = origin;
    g_zseek32_cb_ret = 987; // expected return value from the 32-bit path
    return g_zseek32_cb_ret;
}

// Simple assertion helpers (non-terminating)
static void EXPECT_EQ_LONG(long a, long b, const char* desc) {
    ++g_total_tests;
    if (a != b) {
        ++g_failed_tests;
        std::cout << "ASSERT FAIL: " << desc << " | expected " << b << ", got " << a << std::endl;
    }
}
static void EXPECT_EQ_ULL(uint64_t a, uint64_t b, const char* desc) {
    ++g_total_tests;
    if (a != b) {
        ++g_failed_tests;
        std::cout << "ASSERT FAIL: " << desc << " | expected " << b << ", got " << a << std::endl;
    }
}
static void EXPECT_TRUE(bool cond, const char* desc) {
    ++g_total_tests;
    if (!cond) {
        ++g_failed_tests;
        std::cout << "ASSERT FAIL: " << desc << " | condition evaluated to false" << std::endl;
    }
}
static void EXPECT_FALSE(bool cond, const char* desc) {
    ++g_total_tests;
    if (cond) {
        ++g_failed_tests;
        std::cout << "ASSERT FAIL: " << desc << " | condition evaluated to true" << std::endl;
    }
}

// Test 1: When zfile_func64.zseek64_file != NULL, call that function
static void test_case_zseek64_file_path() {
    LOG_MSG("Test Case 1: Call through zfile_func64.zseek64_file != NULL");
    // Prepare pfilefunc with zseek64_file implemented
    zlib_filefunc64_32_def filefunc;
    std::memset(&filefunc, 0, sizeof(filefunc));
    // Assign our test callback
    filefunc.zfile_func64.zseek64_file = &test_zseek64_cb;
    filefunc.zfile_func64.opaque = reinterpret_cast<voidpf>(0xABCDEF); // known opaque value
    filefunc.zseek32_file = nullptr; // not used in this path

    voidpf filestream = reinterpret_cast<voidpf>(0x12345);
    ZPOS64_T offset = static_cast<ZPOS64_T>(0x1122334455667788ULL);
    int origin = 7;

    long ret = call_zseek64(&filefunc, filestream, offset, origin);

    EXPECT_EQ_LONG(ret, 12345, "call_zseek64 should return value from zseek64_file callback");
    EXPECT_EQ_LONG(g_zseek64_cb_ret, 12345, "callback should set return value tracker");
    EXPECT_EQ_LONG((long)g_zseek64_captured_opaque, (long)filefunc.zfile_func64.opaque, "opaque passed through to zseek64_file");
    EXPECT_EQ_ULL(to_uint64(g_zseek64_captured_offset), to_uint64(offset), "offset passed to zseek64_file");
    EXPECT_EQ_LONG(g_zseek64_captured_origin, origin, "origin passed to zseek64_file");
}

// Test 2: When zfile_func64.zseek64_file == NULL and offset fits in uLong, call zseek32_file
static void test_case_zseek32_path_when_offset_fits() {
    LOG_MSG("Test Case 2: Call through zseek32_file when zseek64_file == NULL and offset fits in uLong");
    zlib_filefunc64_32_def filefunc;
    std::memset(&filefunc, 0, sizeof(filefunc));

    filefunc.zfile_func64.zseek64_file = nullptr;
    filefunc.zseek32_file = &test_zseek32_cb;
    filefunc.zfile_func64.opaque = reinterpret_cast<voidpf>(0xDEAD);

    voidpf filestream = reinterpret_cast<voidpf>(0xBEEF);
    // Use a small offset that definitely fits in uLong on all typical platforms
    ZPOS64_T offset = static_cast<ZPOS64_T>(12345);
    int origin = 2;

    long ret = call_zseek64(&filefunc, filestream, offset, origin);

    EXPECT_EQ_LONG(ret, 987, "call_zseek64 should return value from zseek32_file callback");
    EXPECT_TRUE(g_zseek32_was_called, "zseek32_file callback should have been invoked");
    EXPECT_EQ_LONG((long)g_zseek32_captured_opaque, (long)filefunc.zfile_func64.opaque, "opaque passed through to zseek32_file");
    EXPECT_EQ_ULL(to_uint64(g_zseek32_captured_offset), to_uint64(static_cast<uLong>(offset)), "offset passed to zseek32_file");
    EXPECT_EQ_LONG(g_zseek32_captured_origin, origin, "origin passed to zseek32_file");
}

// Test 3: When zfile_func64.zseek64_file == NULL and offset does not fit in uLong, expect -1
static void test_case_offset_does_not_fit_in_ulong() {
    LOG_MSG("Test Case 3: offset does not fit in uLong => expect -1 path (no zseek32_file call)");
    zlib_filefunc64_32_def filefunc;
    std::memset(&filefunc, 0, sizeof(filefunc));

    filefunc.zfile_func64.zseek64_file = nullptr;
    filefunc.zseek32_file = &test_zseek32_cb; // Should not be called in this path
    filefunc.zfile_func64.opaque = reinterpret_cast<voidpf>(0xFEED);

    voidpf filestream = reinterpret_cast<voidpf>(0xCAFE);
    // Choose a value that likely doesn't fit into 32 bits on all architectures.
    // If it does fit on the platform, we skip this test gracefully.
    ZPOS64_T big_offset = static_cast<ZPOS64_T>(0x1'0000'0000ULL); // 4294967296

    // Detect at runtime if it truly does not fit into uLong
    const uLong max_ulong = std::numeric_limits<uLong>::max();
    bool does_not_fit = (static_cast<uLong>(big_offset) != big_offset);
    if (!does_not_fit) {
        LOG_MSG("Skipping Test Case 3: offset too large to ensure portability on this platform (offset fits in uLong).");
        return;
    }

    int origin = 3;
    long ret = call_zseek64(&filefunc, filestream, big_offset, origin);

    EXPECT_EQ_LONG(ret, -1, "call_zseek64 should return -1 when offset truncates to a different value");
    EXPECT_FALSE(g_zseek32_was_called, "zseek32_file should not be called when offset does not fit in uLong");
}

// Entry point for tests
int main() {
    LOG_MSG("Starting tests for call_zseek64 in ioapi.c (C++11, no GTest)");
    // Run test cases
    test_case_zseek64_file_path();
    test_case_zseek32_path_when_offset_fits();
    test_case_offset_does_not_fit_in_ulong();

    LOG_MSG("=======================================");
    LOG_MSG("Test results: " << g_total_tests << " tests, "
                          << g_failed_tests << " failures");
    LOG_MSG("=======================================");

    // Return non-zero if any test failed
    return g_failed_tests != 0 ? 1 : 0;
}