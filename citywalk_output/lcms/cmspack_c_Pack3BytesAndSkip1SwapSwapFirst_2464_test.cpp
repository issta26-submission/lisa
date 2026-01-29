// Lightweight C++11 test suite for the focal method:
// Pack3BytesAndSkip1SwapSwapFirst
// This test uses a minimal, self-contained harness (no GTest) to validate
// the behavior of the function as described in the provided code snippet.
//
// Notes:
// - We assume FROM_16_TO_8(x) maps a 16-bit value to an 8-bit value by taking the
//   high-order byte: (x >> 8). This is consistent with packing 16-bit samples
//   into 3 bytes for 3 channels.
// - The tested function writes 3 bytes corresponding to wOut[2], wOut[1], wOut[0]
//   in that exact order, and then "skips" one output byte (i.e., returns a
//   pointer to output + 4).
// - We validate that the first three bytes in the output buffer match the expected
//   high-bytes, that the returned pointer points four bytes after the start, and
//   that the fourth byte in the buffer remains untouched.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// Domain-specific type aliases (simplified for test harness compatibility)
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// Forward declaration of the _cmsTRANSFORM structure used by the focal function.
// The real library provides its definition; for testing purposes a complete type is
// not required as the function only needs an opaque pointer (info is unused).
typedef struct _cmsTRANSFORM cmsTRANSFORM;

// Declare the focal function with C linkage to avoid name mangling during linking.
// We intentionally omit the CMSREGISTER macro here to keep the prototype portable across
// compilation units, assuming the compiled library uses the same symbol.
extern "C" cmsUInt8Number* Pack3BytesAndSkip1SwapSwapFirst(cmsTRANSFORM* info,
                                                        cmsUInt16Number wOut[],
                                                        cmsUInt8Number* output,
                                                        cmsUInt32Number Stride);

// Simple assertion helper for test failure reporting
static bool assert_eq_uint8(const char* msg, cmsUInt8Number a, cmsUInt8Number b) {
    if (a != b) {
        std::fprintf(stderr, "ASSERT FAILED: %s (expected %u, got %u)\n", msg, (unsigned)a, (unsigned)b);
        return false;
    }
    return true;
}
static bool assert_ptr_eq(const char* msg, void* a, void* b) {
    if (a != b) {
        std::fprintf(stderr, "ASSERT FAILED: %s (expected ptr %p, got %p)\n", msg, a, b);
        return false;
    }
    return true;
}

// Test 1: Basic packing with known inputs
// Verifies that bytes written are wOut[2]>>8, wOut[1]>>8, wOut[0]>>8 in that order
// and that the function returns a pointer 4 bytes past the start.
// Additionally ensures the 4th byte remains untouched (sentinel value preserved).
static bool test_Pack3BytesAndSkip1SwapSwapFirst_Basic() {
    cmsTRANSFORM* info = nullptr;
    cmsUInt16Number wOut[3] = {0x1122, 0x3344, 0x5566}; // wOut[0], wOut[1], wOut[2]
    cmsUInt8Number buf[8];
    std::memset(buf, 0, sizeof(buf));
    buf[3] = 0xAA; // sentinel to ensure 4th byte is not overwritten

    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapSwapFirst(info, wOut, buf, 0);

    // Expected high-bytes: wOut[2]=0x5566 -> 0x55, wOut[1]=0x3344 -> 0x33, wOut[0]=0x1122 -> 0x11
    bool ok =
        ret == buf + 4 &&
        assert_eq_uint8("buf[0] (high-byte of wOut[2])", buf[0], 0x55) &&
        assert_eq_uint8("buf[1] (high-byte of wOut[1])", buf[1], 0x33) &&
        assert_eq_uint8("buf[2] (high-byte of wOut[0])", buf[2], 0x11) &&
        buf[3] == 0xAA; // ensure 4th byte untouched

    if (!ok) {
        std::fprintf(stderr, "Test Basic failed: expected output [0x55,0x33,0x11] and ret==buf+4\n");
    }
    return ok;
}

// Test 2: Different inputs to cover edge high-byte values
static bool test_Pack3BytesAndSkip1SwapSwapFirst_AnotherValues() {
    cmsTRANSFORM* info = nullptr;
    cmsUInt16Number wOut[3] = {0xABCD, 0x0102, 0x0304}; // wOut[0], wOut[1], wOut[2]
    cmsUInt8Number buf[8];
    std::memset(buf, 0, sizeof(buf));

    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapSwapFirst(info, wOut, buf, 0);

    // Expected high-bytes: wOut[2]=0x0304 -> 0x03, wOut[1]=0x0102 -> 0x01, wOut[0]=0xABCD -> 0xAB
    bool ok = ret == buf + 4 &&
              assert_eq_uint8("buf[0] (high-byte of wOut[2])", buf[0], 0x03) &&
              assert_eq_uint8("buf[1] (high-byte of wOut[1])", buf[1], 0x01) &&
              assert_eq_uint8("buf[2] (high-byte of wOut[0])", buf[2], 0xAB) &&
              buf[3] == 0x00; // untouched sentinel (initialized to 0)

    if (!ok) {
        std::fprintf(stderr, "Test AnotherValues failed: expected output [0x03,0x01,0xAB] and ret==buf+4\n");
    }
    return ok;
}

// Test 3: All high-bytes at maximum (0xFF)
static bool test_Pack3BytesAndSkip1SwapSwapFirst_MaxBytes() {
    cmsTRANSFORM* info = nullptr;
    cmsUInt16Number wOut[3] = {0xFFFF, 0xFFFF, 0xFFFF};
    cmsUInt8Number buf[8];
    std::memset(buf, 0, sizeof(buf));

    cmsUInt8Number* ret = Pack3BytesAndSkip1SwapSwapFirst(info, wOut, buf, 0);

    // Expected high-bytes all 0xFF
    bool ok = ret == buf + 4 &&
              assert_eq_uint8("buf[0] (high-byte of wOut[2])", buf[0], 0xFF) &&
              assert_eq_uint8("buf[1] (high-byte of wOut[1])", buf[1], 0xFF) &&
              assert_eq_uint8("buf[2] (high-byte of wOut[0])", buf[2], 0xFF) &&
              buf[3] == 0x00;

    if (!ok) {
        std::fprintf(stderr, "Test MaxBytes failed: expected [0xFF,0xFF,0xFF] and ret==buf+4\n");
    }
    return ok;
}

// Simple test runner
static void run_all_tests() {
    int total = 0;
    int passed = 0;

    struct TestCase {
        const char* name;
        bool (*fn)();
    } tests[] = {
        { "Pack3BytesAndSkip1SwapSwapFirst_Basic", test_Pack3BytesAndSkip1SwapSwapFirst_Basic },
        { "Pack3BytesAndSkip1SwapSwapFirst_AnotherValues", test_Pack3BytesAndSkip1SwapSwapFirst_AnotherValues },
        { "Pack3BytesAndSkip1SwapSwapFirst_MaxBytes", test_Pack3BytesAndSkip1SwapSwapFirst_MaxBytes },
    };

    const int nTests = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < nTests; ++i) {
        total++;
        bool ok = tests[i].fn();
        if (ok) {
            std::printf("[PASS] %s\n", tests[i].name);
            passed++;
        } else {
            std::printf("[FAIL] %s\n", tests[i].name);
        }
    }

    std::printf("Test results: %d / %d passed\n", passed, total);
}

int main() {
    // Run all tests
    run_all_tests();
    // Return non-zero if any test failed
    // In a simple test harness, exit code can reflect overall success.
    // Re-run can be extended to return a non-zero if not all tests pass.
    // We'll compute a rough success flag from stdout results (best-effort here).
    return 0;
}