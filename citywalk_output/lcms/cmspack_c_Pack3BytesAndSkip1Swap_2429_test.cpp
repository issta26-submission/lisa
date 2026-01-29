/*
Unit test suite for cmsUInt8Number* Pack3BytesAndSkip1Swap(...)
Core candidate keywords (Step 1 insights):
- Pack3BytesAndSkip1Swap
- FROM_16_TO_8 macro
- _cmsTRANSFORM / CMSREGISTER
- wOut[] (cmsUInt16Number)
- output (cmsUInt8Number*)
- Stride (cmsUInt32Number)
- info (CMSREGISTER _cmsTRANSFORM*)
- Memory layout: return value should be original output pointer advanced by 4
- Order of writes: wOut[2], wOut[1], wOut[0]
- Test coverage goal: true/false macro expansion, boundary values, and memory boundaries
This test relies on the same macro FROM_16_TO_8 defined in the cms headers, so expected values are computed in the test using the same macro.
*/

#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include the LittleCMS CMS headers to access the types/macros.
// The environment for the tests is assumed to have the CMS headers available in the include path.

using namespace std;

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_ASSERT_EQ(actual, expected, testName, commentary) do { \
    ++g_total_tests; \
    if ((actual) != (expected)) { \
        ++g_failed_tests; \
        cerr << "[FAILED] " << testName << ": " << commentary \
             << " | Expected: " << static_cast<unsigned int>(expected) \
             << ", Actual: " << static_cast<unsigned int>(actual) << endl; \
    } else { \
        cerr << "[PASSED] " << testName << endl; \
    } \
} while(0)

#define TEST_ASSERT_TRUE(cond, testName, commentary) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        cerr << "[FAILED] " << testName << ": " << commentary << endl; \
    } else { \
        cerr << "[PASSED] " << testName << endl; \
    } \
} while(0)

/*
Note on types/macros:
- cmsUInt8Number, cmsUInt16Number, cmsUInt32Number are defined in lcms2.h
- CMSREGISTER is a macro typically defined in cmsbase to handle calling conventions; tests pass through transparently
- _cmsTRANSFORM is a struct type used by the API (we provide a valid pointer but tests do not require working transform data)
- FROM_16_TO_8(x) is a macro defined in the CMS headers that converts a 16-bit word to an 8-bit value; we reuse it to compute expected bytes
*/

// Prototypes for the focal function (link-time binding will resolve the real implementation from cmspack.c)
extern "C" cmsUInt8Number* Pack3BytesAndSkip1Swap(CMSREGISTER _cmsTRANSFORM* info,
                                                CMSREGISTER cmsUInt16Number wOut[],
                                                CMSREGISTER cmsUInt8Number* output,
                                                CMSREGISTER cmsUInt32Number Stride);

// Basic test: verify the function writes the three bytes in the correct order and returns base+4.
// It also ensures the first base position remains untouched (memory before the write location is not modified).
void test_Pack3BytesAndSkip1Swap_BasicWrites() {
    // Arrange
    // Prepare a buffer with a sentinel value that should remain at index 0 (before the write region)
    const size_t BUF_SIZE = 8;
    std::vector<cmsUInt8Number> buffer(BUF_SIZE, 0xAB); // sentinel 0xAB

    cmsUInt8Number* base = buffer.data();

    // Input words (3 values)
    cmsUInt16Number wOut[3] = { 0x1122, 0x3344, 0x5566 }; // [0]=0x1122, [1]=0x3344, [2]=0x5566

    // Compute expected bytes using the same macro used in the function
    cmsUInt8Number expected_byte2 = FROM_16_TO_8(wOut[2]);
    cmsUInt8Number expected_byte1 = FROM_16_TO_8(wOut[1]);
    cmsUInt8Number expected_byte0 = FROM_16_TO_8(wOut[0]);

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1Swap(nullptr, wOut, base, 0);

    // Assert
    cmsUInt8Number* expected_return = base + 4;
    TEST_ASSERT_EQ(reinterpret_cast<uintptr_t>(ret), reinterpret_cast<uintptr_t>(expected_return),
                   "Pack3BytesAndSkip1Swap_ReturnPointer", "Return pointer should be base+4");

    // Ensure memory before the write region hasn't been modified
    TEST_ASSERT_EQ(buffer[0], 0xAB, "Pack3BytesAndSkip1Swap_MemoryPreWrite", "buffer[0] should remain sentinel");

    // Validate the three written bytes in order
    TEST_ASSERT_EQ(buffer[1], expected_byte2, "Pack3BytesAndSkip1Swap_WriteByte2", "buffer[1] should equal FROM_16_TO_8(wOut[2])");
    TEST_ASSERT_EQ(buffer[2], expected_byte1, "Pack3BytesAndSkip1Swap_WriteByte1", "buffer[2] should equal FROM_16_TO_8(wOut[1])");
    TEST_ASSERT_EQ(buffer[3], expected_byte0, "Pack3BytesAndSkip1Swap_WriteByte0", "buffer[3] should equal FROM_16_TO_8(wOut[0])");
}

// Secondary test: verify behavior with zeroed inputs and ensure deterministic outputs.
// This checks handling of edge-case bytes (all zeros) via the macro.
void test_Pack3BytesAndSkip1Swap_ZeroInputs() {
    // Arrange
    std::vector<cmsUInt8Number> buffer(8, 0x00);
    cmsUInt8Number* base = buffer.data();

    cmsUInt16Number wOut[3] = { 0x0000, 0x0000, 0x0000 }; // all zeros

    cmsUInt8Number expected_byte2 = FROM_16_TO_8(wOut[2]);
    cmsUInt8Number expected_byte1 = FROM_16_TO_8(wOut[1]);
    cmsUInt8Number expected_byte0 = FROM_16_TO_8(wOut[0]);

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1Swap(nullptr, wOut, base, 0);

    // Assert
    TEST_ASSERT_EQ(reinterpret_cast<uintptr_t>(ret), reinterpret_cast<uintptr_t>(base + 4),
                   "Pack3BytesAndSkip1Swap_ZeroInputs_ReturnPointer",
                   "Return pointer should be base+4 for zero inputs");

    TEST_ASSERT_EQ(buffer[1], expected_byte2,
                   "Pack3BytesAndSkip1Swap_ZeroInputs_Byte2",
                   "buffer[1] should equal FROM_16_TO_8(wOut[2]) (zero)");
    TEST_ASSERT_EQ(buffer[2], expected_byte1,
                   "Pack3BytesAndSkip1Swap_ZeroInputs_Byte1",
                   "buffer[2] should equal FROM_16_TO_8(wOut[1]) (zero)");
    TEST_ASSERT_EQ(buffer[3], expected_byte0,
                   "Pack3BytesAndSkip1Swap_ZeroInputs_Byte0",
                   "buffer[3] should equal FROM_16_TO_8(wOut[0]) (zero)");
}

// Third test: large high-bit values to exercise macro paths.
// Ensure correct behavior when 16-bit words have high bytes and macro selects appropriate 8-bit value.
void test_Pack3BytesAndSkip1Swap_LargeValues() {
    // Arrange
    std::vector<cmsUInt8Number> buffer(12, 0x00);
    cmsUInt8Number* base = buffer.data();

    cmsUInt16Number wOut[3] = { 0xFFFF, 0xABCD, 0x1234 };

    cmsUInt8Number expected_byte2 = FROM_16_TO_8(wOut[2]);
    cmsUInt8Number expected_byte1 = FROM_16_TO_8(wOut[1]);
    cmsUInt8Number expected_byte0 = FROM_16_TO_8(wOut[0]);

    // Act
    cmsUInt8Number* ret = Pack3BytesAndSkip1Swap(nullptr, wOut, base, 0);

    // Assert
    TEST_ASSERT_EQ(reinterpret_cast<uintptr_t>(ret), reinterpret_cast<uintptr_t>(base + 4),
                   "Pack3BytesAndSkip1Swap_LargeValues_ReturnPointer",
                   "Return pointer should be base+4");

    TEST_ASSERT_EQ(buffer[1], expected_byte2,
                   "Pack3BytesAndSkip1Swap_LargeValues_Byte2",
                   "buffer[1] should equal FROM_16_TO_8(wOut[2])");
    TEST_ASSERT_EQ(buffer[2], expected_byte1,
                   "Pack3BytesAndSkip1Swap_LargeValues_Byte1",
                   "buffer[2] should equal FROM_16_TO_8(wOut[1])");
    TEST_ASSERT_EQ(buffer[3], expected_byte0,
                   "Pack3BytesAndSkip1Swap_LargeValues_Byte0",
                   "buffer[3] should equal FROM_16_TO_8(wOut[0])");
}

// Entry point: run all tests
int main() {
    cerr << "Running Pack3BytesAndSkip1Swap unit tests (CMS LittleCMS binding, C API tested via C++ harness)...\n";

    test_Pack3BytesAndSkip1Swap_BasicWrites();
    test_Pack3BytesAndSkip1Swap_ZeroInputs();
    test_Pack3BytesAndSkip1Swap_LargeValues();

    cerr << "Summary: Total=" << g_total_tests << " Failed=" << g_failed_tests << "\n";

    // Return non-zero if any test failed to aid CI systems
    return (g_failed_tests == 0) ? 0 : 1;
}