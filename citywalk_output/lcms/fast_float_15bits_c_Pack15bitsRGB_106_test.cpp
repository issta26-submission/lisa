// Explanatory: This test suite targets the Pack15bitsRGB function from the 15-bit RGB packing path.
// It verifies that three 16-bit inputs are converted via From16To15 (i.e., masked to 15 bits)
// and written sequentially into the provided 8-byte buffer, returning buffer_start + 8.
// We rely on the project headers to define the cms* types and the From16To15 function.
// Tests are designed to work with a standard C++11 compiler without GoogleTest.
// The tests use simple, non-terminating assertions and report a final summary.

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cstdint>


// Include project internal headers to obtain type definitions (cmsUInt8Number, cmsUInt16Number, etc.)
// and to allow proper linkage with the C implementation of Pack15bitsRGB.

// Declare the function under test with C linkage to ensure correct symbol resolution
extern "C" cmsUInt8Number* Pack15bitsRGB(struct _cmstransform_struct* CMMcargo,
                               cmsUInt16Number Values[],
                               cmsUInt8Number*  Buffer,
                               cmsUInt32Number  Stride);

// Helper: read a 16-bit word from the buffer (preserving machine endianness)
static cmsUInt16Number readWord(const cmsUInt8Number* p) {
    cmsUInt16Number w;
    std::memcpy(&w, p, sizeof(w));
    return w;
}

// Simple test harness (non-terminating assertions)
static int tests_failed = 0;

static void assert_equal_uint16(const cmsUInt16Number& expected, const cmsUInt16Number& actual, const char* msg) {
    if (expected != actual) {
        std::cerr << "ASSERT FAILED: " << msg
                  << " | expected: 0x" << std::hex << static_cast<uint32_t>(expected)
                  << " , actual: 0x" << static_cast<uint32_t>(actual) << std::dec << "\n";
        ++tests_failed;
    }
}

// Test 1: Basic packing with typical values
static void test_Pack15bitsRGB_basic() {
    std::cout << "Running test_Pack15bitsRGB_basic...\n";

    // Prepare input values
    cmsUInt16Number Values[3] = { 0x0123, 0xABCD, 0x7FFF }; // 16-bit values; expect masking to 15 bits
    const size_t BUF_SIZE = 16;
    std::vector<cmsUInt8Number> buffer(BUF_SIZE, 0);
    cmsUInt8Number* start = buffer.data();
    cmsUInt8Number* Buffer = start;

    // Call function
    cmsUInt8Number* ret = Pack15bitsRGB(nullptr, Values, Buffer, 0);

    // Read back the 3 written 16-bit words
    cmsUInt16Number w0 = readWord(start);
    cmsUInt16Number w1 = readWord(start + 2);
    cmsUInt16Number w2 = readWord(start + 4);

    // Expected values: From16To15(x) => x & 0x7FFF
    cmsUInt16Number e0 = Values[0] & 0x7FFF;
    cmsUInt16Number e1 = Values[1] & 0x7FFF;
    cmsUInt16Number e2 = Values[2] & 0x7FFF;

    assert_equal_uint16(e0, w0, "w0 should be From16To15(Values[0])");
    assert_equal_uint16(e1, w1, "w1 should be From16To15(Values[1])");
    assert_equal_uint16(e2, w2, "w2 should be From16To15(Values[2])");

    // Expected return pointer: start + 8
    cmsUInt8Number* expected_ret = start + 8;
    if (ret != expected_ret) {
        std::cerr << "ASSERT FAILED: Return pointer mismatch in basic test."
                  << " expected: " << static_cast<void*>(expected_ret)
                  << " got: " << static_cast<void*>(ret) << "\n";
        ++tests_failed;
    }

    std::cout << "test_Pack15bitsRGB_basic completed.\n";
}

// Test 2: Edge case values to ensure correct 15-bit masking
static void test_Pack15bitsRGB_edgecases() {
    std::cout << "Running test_Pack15bitsRGB_edgecases...\n";

    cmsUInt16Number Values[3] = { 0x8000, 0x0000, 0xFFFF }; // 0x8000 -> 0x0000, 0xFFFF -> 0x7FFF
    std::vector<cmsUInt8Number> buffer(16, 0);
    cmsUInt8Number* start = buffer.data();

    cmsUInt8Number* ret = Pack15bitsRGB(nullptr, Values, start, 0);

    cmsUInt16Number w0 = readWord(start);
    cmsUInt16Number w1 = readWord(start + 2);
    cmsUInt16Number w2 = readWord(start + 4);

    cmsUInt16Number e0 = Values[0] & 0x7FFF; // should be 0x0000
    cmsUInt16Number e1 = Values[1] & 0x7FFF; // should be 0x0000
    cmsUInt16Number e2 = Values[2] & 0x7FFF; // should be 0x7FFF

    assert_equal_uint16(e0, w0, "Edge w0 should be From16To15(0x8000)");
    assert_equal_uint16(e1, w1, "Edge w1 should be From16To15(0x0000)");
    assert_equal_uint16(e2, w2, "Edge w2 should be From16To15(0xFFFF)");
    
    cmsUInt8Number* expected_ret = start + 8;
    if (ret != expected_ret) {
        std::cerr << "ASSERT FAILED: Return pointer mismatch in edgecases test."
                  << " expected: " << static_cast<void*>(expected_ret)
                  << " got: " << static_cast<void*>(ret) << "\n";
        ++tests_failed;
    }

    std::cout << "test_Pack15bitsRGB_edgecases completed.\n";
}

// Test 3: Multiple consecutive Pack15bitsRGB calls in one buffer
static void test_Pack15bitsRGB_multiple_calls() {
    std::cout << "Running test_Pack15bitsRGB_multiple_calls...\n";

    // First set
    cmsUInt16Number Values1[3] = { 0x0101, 0x0202, 0x0303 };
    // Second set
    cmsUInt16Number Values2[3] = { 0x0A0A, 0x1B1B, 0x2C2C };

    std::vector<cmsUInt8Number> buffer(64, 0);
    cmsUInt8Number* start = buffer.data();

    // First call
    cmsUInt8Number* ret1 = Pack15bitsRGB(nullptr, Values1, start, 0);
    // Second call starts where first ended (start + 8)
    cmsUInt8Number* ret2 = Pack15bitsRGB(nullptr, Values2, start + 8, 0);

    // Read back first block
    cmsUInt16Number w0_a = readWord(start);
    cmsUInt16Number w1_a = readWord(start + 2);
    cmsUInt16Number w2_a = readWord(start + 4);

    cmsUInt16Number e0_a = Values1[0] & 0x7FFF;
    cmsUInt16Number e1_a = Values1[1] & 0x7FFF;
    cmsUInt16Number e2_a = Values1[2] & 0x7FFF;

    assert_equal_uint16(e0_a, w0_a, "Block1 w0 should be From16To15(Values1[0])");
    assert_equal_uint16(e1_a, w1_a, "Block1 w1 should be From16To15(Values1[1])");
    assert_equal_uint16(e2_a, w2_a, "Block1 w2 should be From16To15(Values1[2])");

    // Read back second block
    cmsUInt16Number w0_b = readWord(start + 8);
    cmsUInt16Number w1_b = readWord(start + 10);
    cmsUInt16Number w2_b = readWord(start + 12);

    cmsUInt16Number e0_b = Values2[0] & 0x7FFF;
    cmsUInt16Number e1_b = Values2[1] & 0x7FFF;
    cmsUInt16Number e2_b = Values2[2] & 0x7FFF;

    assert_equal_uint16(e0_b, w0_b, "Block2 w0 should be From16To15(Values2[0])");
    assert_equal_uint16(e1_b, w1_b, "Block2 w1 should be From16To15(Values2[1])");
    assert_equal_uint16(e2_b, w2_b, "Block2 w2 should be From16To15(Values2[2])");

    // Return pointers should be +8 for each call
    if (ret1 != start + 8) {
        std::cerr << "ASSERT FAILED: Return pointer mismatch for first call in multi_test.\n";
        ++tests_failed;
    }
    if (ret2 != start + 16) {
        std::cerr << "ASSERT FAILED: Return pointer mismatch for second call in multi_test.\n";
        ++tests_failed;
    }

    std::cout << "test_Pack15bitsRGB_multiple_calls completed.\n";
}

int main() {
    std::cout << "Starting Pack15bitsRGB test suite (C++11, no GTest)\n";

    test_Pack15bitsRGB_basic();
    test_Pack15bitsRGB_edgecases();
    test_Pack15bitsRGB_multiple_calls();

    if (tests_failed == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cerr << "Tests finished with " << tests_failed << " failure(s).\n";
        return 1;
    }
}