// C++11 test suite for the focal method Unroll15bitsRGBA in fast_float_15bits.c
// This test suite does not rely on Google Test. It uses a small, self-contained test harness
// with non-terminating EXPECT-style assertions to maximize code coverage.
// The tests assume the project provides the expected cms... typedefs and the Unroll15bitsRGBA function
// linked from fast_float_internal.h (or equivalent) with C linkage when needed.

#include <cstring>
#include <fast_float_internal.h>
#include <iostream>
#include <iomanip>
#include <cstdint>


// Include the project header that declares the types and the function under test.
// Guard with extern "C" to ensure correct linkage when compiled as C++.
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

// Lightweight test harness
static int g_failures = 0;

#define EXPECT_EQ(val1, val2)                                         \
    do {                                                              \
        if (!((val1) == (val2))) {                                   \
            std::cerr << "EXPECT_EQ failed: " #val1 " (" << (val1)       \
                      << ") != " #val2 " (" << (val2) << ") at "        \
                      << __FILE__ << ":" << __LINE__ << "\n";         \
            ++g_failures;                                           \
        }                                                             \
    } while (0)

#define EXPECT_TRUE(cond)                                             \
    do {                                                              \
        if (!(cond)) {                                                \
            std::cerr << "EXPECT_TRUE failed: " #cond                   \
                      << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_failures;                                           \
        }                                                             \
    } while (0)

#define EXPECT_PTR_EQ(ptr1, ptr2)                                      \
    do {                                                               \
        if ((void*)(ptr1) != (void*)(ptr2)) {                        \
            std::cerr << "EXPECT_PTR_EQ failed: " #ptr1 " (" << (void*)(ptr1) \
                      << ") != " #ptr2 " (" << (void*)(ptr2) << ")"    \
                      << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_failures;                                            \
        }                                                              \
    } while (0)

static bool test_Unroll15bitsRGBA_basic_reads_and_converts() {
    // Purpose:
    // - Verify that Unroll15bitsRGBA reads the first three 16-bit values from Buffer,
    //   applies From15To16 to each, stores them in Values[], and returns Buffer+4.
    // - We compare against the same conversion function From15To16 to form the expected results.
    //   This ensures we validate the contract: Values[i] should equal From15To16(inputWord[i]).

    // Prepare input buffer with three 16-bit values in little-endian order.
    cmsUInt8Number buffer[6];
    const cmsUInt16Number inputWords[3] = { 0x0001, 0x00FF, 0x0A0A };

    std::memcpy(buffer, &inputWords[0], 2);
    std::memcpy(buffer + 2, &inputWords[1], 2);
    std::memcpy(buffer + 4, &inputWords[2], 2);

    cmsUInt16Number Values[3] = { 0, 0, 0 };

    // Call the function under test.
    struct _cmstransform_struct* dummyCargo = nullptr;
    cmsUInt8Number* ret = Unroll15bitsRGBA(dummyCargo, Values, buffer, 0);

    // Build expected values using the same conversion routine.
    cmsUInt16Number expected0 = From15To16(inputWords[0]);
    cmsUInt16Number expected1 = From15To16(inputWords[1]);
    cmsUInt16Number expected2 = From15To16(inputWords[2]);

    // Assertions
    bool ok = true;
    if (Values[0] != expected0) {
        std::cerr << "Value[0] mismatch: got " << Values[0]
                  << ", expected " << expected0 << "\n";
        ok = false;
    }
    if (Values[1] != expected1) {
        std::cerr << "Value[1] mismatch: got " << Values[1]
                  << ", expected " << expected1 << "\n";
        ok = false;
    }
    if (Values[2] != expected2) {
        std::cerr << "Value[2] mismatch: got " << Values[2]
                  << ", expected " << expected2 << "\n";
        ok = false;
    }

    // Check returned pointer is Buffer + 4
    EXPECT_PTR_EQ(ret, buffer + 4);

    return ok;
}

static bool test_Unroll15bitsRGBA_basic_with_varied_data() {
    // Purpose:
    // - Run the same test as above but with a different data set to ensure no false positives.
    cmsUInt8Number buffer[6];
    const cmsUInt16Number inputWords[3] = { 0x1234, 0x7FFF, 0x0000 }; // varied data including max 15-bit value

    std::memcpy(buffer, &inputWords[0], 2);
    std::memcpy(buffer + 2, &inputWords[1], 2);
    std::memcpy(buffer + 4, &inputWords[2], 2);

    cmsUInt16Number Values[3] = { 0, 0, 0 };

    struct _cmstransform_struct* dummyCargo = nullptr;
    cmsUInt8Number* ret = Unroll15bitsRGBA(dummyCargo, Values, buffer, 0);

    cmsUInt16Number expected0 = From15To16(inputWords[0]);
    cmsUInt16Number expected1 = From15To16(inputWords[1]);
    cmsUInt16Number expected2 = From15To16(inputWords[2]);

    bool ok = true;
    if (Values[0] != expected0) {
        std::cerr << "Value[0] mismatch: got " << Values[0]
                  << ", expected " << expected0 << "\n";
        ok = false;
    }
    if (Values[1] != expected1) {
        std::cerr << "Value[1] mismatch: got " << Values[1]
                  << ", expected " << expected1 << "\n";
        ok = false;
    }
    if (Values[2] != expected2) {
        std::cerr << "Value[2] mismatch: got " << Values[2]
                  << ", expected " << expected2 << "\n";
        ok = false;
    }

    EXPECT_PTR_EQ(ret, buffer + 4);

    return ok;
}

static bool test_Unroll15bitsRGBA_ignores_stride_and_cargo() {
    // Purpose:
    // - Ensure non-null Stride and CMM cargo do not affect results (they are unused by the function).
    cmsUInt8Number buffer[6];
    const cmsUInt16Number inputWords[3] = { 0x0F0F, 0x0E0E, 0x0D0D };

    std::memcpy(buffer, &inputWords[0], 2);
    std::memcpy(buffer + 2, &inputWords[1], 2);
    std::memcpy(buffer + 4, &inputWords[2], 2);

    cmsUInt16Number Values[3] = { 0, 0, 0 };

    // Non-null cargo and non-zero stride
    struct _cmstransform_struct dummyCargo;
    cmsUInt32Number dummyStride = 12345;

    cmsUInt8Number* ret = Unroll15bitsRGBA(&dummyCargo, Values, buffer, dummyStride);

    cmsUInt16Number expected0 = From15To16(inputWords[0]);
    cmsUInt16Number expected1 = From15To16(inputWords[1]);
    cmsUInt16Number expected2 = From15To16(inputWords[2]);

    bool ok = true;
    if (Values[0] != expected0) {
        std::cerr << "Value[0] mismatch: got " << Values[0]
                  << ", expected " << expected0 << "\n";
        ok = false;
    }
    if (Values[1] != expected1) {
        std::cerr << "Value[1] mismatch: got " << Values[1]
                  << ", expected " << expected1 << "\n";
        ok = false;
    }
    if (Values[2] != expected2) {
        std::cerr << "Value[2] mismatch: got " << Values[2]
                  << ", expected " << expected2 << "\n";
        ok = false;
    }

    EXPECT_PTR_EQ(ret, buffer + 4);

    return ok;
}

int main() {
    std::cout << "Running Unroll15bitsRGBA unit tests...\n";

    int total = 0;
    int passed = 0;

    bool r1 = test_Unroll15bitsRGBA_basic_reads_and_converts();
    total++;
    if (r1) { ++passed; std::cout << "Test 1 passed: basic reads and converts.\n"; } else { std::cerr << "Test 1 failed.\n"; }

    bool r2 = test_Unroll15bitsRGBA_basic_with_varied_data();
    total++;
    if (r2) { ++passed; std::cout << "Test 2 passed: varied data handling.\n"; } else { std::cerr << "Test 2 failed.\n"; }

    bool r3 = test_Unroll15bitsRGBA_ignores_stride_and_cargo();
    total++;
    if (r3) { ++passed; std::cout << "Test 3 passed: stride and cargo ignored as expected.\n"; } else { std::cerr << "Test 3 failed.\n"; }

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    if (g_failures > 0) {
        return 1;
    }
    // Also respect any local failures captured by boolean tests
    return (passed == total) ? 0 : 1;
}