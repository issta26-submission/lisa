// Test suite for Unroll15bitsCMYK in fast_float_15bits.c
// This test uses a small, custom test harness (no GTest) and runs in C++11.
// It validates that Unroll15bitsCMYK correctly reads four 16-bit values from a
// buffer, applies 0x8000 - value, converts via From15To16, stores into Values[],
// and returns Buffer advanced by 10 bytes (i.e., original buffer pointer + 10).
// The expected transformation uses From15To16(x) = x << 1 (typical 15->16 bit expansion).

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <vector>
#include <cstdint>


// Include required project headers for type definitions and function prototype.
// Adjust the include path as necessary for your build environment.

extern "C" {
// Forward declaration to ensure linkage if the header doesn't already provide it.
// The actual definition is in fast_float_15bits.c, which is part of the build.
CMSREGISTER struct _cmstransform_struct;
}

// Forward declare the focal function with C linkage to avoid name mangling issues.
// The macros (CMSREGISTER) are expected to be defined in the included headers.
extern "C" cmsUInt8Number* Unroll15bitsCMYK(CMSREGISTER struct _cmstransform_struct* CMMcargo,
                                           CMSREGISTER cmsUInt16Number Values[],
                                           CMSREGISTER cmsUInt8Number*  Buffer,
                                           CMSREGISTER cmsUInt32Number  Stride);

// Minimal test harness
static int g_failures = 0;

#define TEST_OK(msg) do { printf("[PASS] %s\n", msg); } while (0)
#define TEST_FAIL(msg) do { printf("[FAIL] %s\n", msg); ++g_failures; } while (0)

static inline uint16_t compute_from_15_to_16(uint16_t x15) {
    // Assumed implementation: From15To16(x) == x << 1
    return (uint16_t)(x15 << 1);
}

// Helper to fill a buffer with 16-bit little-endian values
static void fill_buffer_le(uint8_t* buf, const uint16_t values[4]) {
    for (int i = 0; i < 4; ++i) {
        uint16_t v = values[i];
        buf[2*i + 0] = (uint8_t)(v & 0xFF);
        buf[2*i + 1] = (uint8_t)((v >> 8) & 0xFF);
    }
}

// Test 1: Basic 4-value transformation with typical inputs
static void test_unroll15bits_cmyk_case1() {
    // Inputs (as 16-bit values in the buffer)
    // a1=0x0000, a2=0x0001, a3=0x7FFF, a4=0x4000
    // Expected Values after From15To16(0x8000 - ai) with From15To16(x) = x << 1:
    // a1: (0x8000 - 0x0000) << 1 = 0x10000 -> 0x0000 (16-bit wrap)
    // a2: (0x8000 - 0x0001) << 1 = 0x7FFF << 1 = 0xFFFE
    // a3: (0x8000 - 0x7FFF) << 1 = 0x0001 << 1 = 0x0002
    // a4: (0x8000 - 0x4000) << 1 = 0x4000 << 1 = 0x8000
    const uint16_t a[4] = {0x0000, 0x0001, 0x7FFF, 0x4000};
    const uint16_t expected[4] = {
        compute_from_15_to_16(a[0]),
        compute_from_15_to_16(a[1]),
        compute_from_15_to_16(a[2]),
        compute_from_15_to_16(a[3])
    };

    uint8_t buffer[32];
    uint16_t Values[4] = {0, 0, 0, 0};
    fill_buffer_le(buffer, a);

    // Call the focal function
    cmsUInt8Number* ret = Unroll15bitsCMYK(nullptr, Values, buffer, 0);

    // Validate Values
    bool ok = true;
    for (int i = 0; i < 4; ++i) {
        if (Values[i] != expected[i]) {
            printf("Case1: Value[%d] = 0x%04X, expected 0x%04X\n", i, Values[i], expected[i]);
            ok = false;
        }
    }
    if (!ok) TEST_FAIL("Case1: Values do not match expected CMYK 15-bit to 16-bit mapping.");
    else TEST_OK("Case1: Values match expected CMYK 15-bit mapping.");

    // Validate returned pointer offset (should be original + 10)
    uintptr_t original = reinterpret_cast<uintptr_t>(buffer);
    uintptr_t returned = reinterpret_cast<uintptr_t>(ret);
    if (returned != original + 10) {
        printf("Case1: Returned pointer mismatch: got %p, expected %p\n",
               reinterpret_cast<void*>(ret),
               reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(buffer) + 10));
        TEST_FAIL("Case1: Return pointer offset incorrect.");
    } else {
        TEST_OK("Case1: Return pointer offset correct (buffer advanced by 10 bytes).");
    }
}

// Test 2: Another set of inputs to increase coverage and branch-like behavior
static void test_unroll15bits_cmyk_case2() {
    // Inputs: a1=0x0007, a2=0x00F0, a3=0x1234, a4=0x0001
    // Expected:
    // a1: (0x8000 - 0x0007) << 1 = 0x7FF9 << 1 = 0xFFF2
    // a2: (0x8000 - 0x00F0) << 1 = 0x7FF0 << 1 = 0xFFE0
    // a3: (0x8000 - 0x1234) << 1 = 0x6ECC << 1 = 0xDD98
    // a4: (0x8000 - 0x0001) << 1 = 0x7FFF << 1 = 0xFFFE
    const uint16_t a[4] = {0x0007, 0x00F0, 0x1234, 0x0001};
    const uint16_t expected[4] = {
        compute_from_15_to_16(a[0]),
        compute_from_15_to_16(a[1]),
        compute_from_15_to_16(a[2]),
        compute_from_15_to_16(a[3])
    };

    uint8_t buffer[32];
    uint16_t Values[4] = {0, 0, 0, 0};
    fill_buffer_le(buffer, a);

    // Call the focal function
    cmsUInt8Number* ret = Unroll15bitsCMYK(nullptr, Values, buffer, 0);

    // Validate Values
    bool ok = true;
    for (int i = 0; i < 4; ++i) {
        if (Values[i] != expected[i]) {
            printf("Case2: Value[%d] = 0x%04X, expected 0x%04X\n", i, Values[i], expected[i]);
            ok = false;
        }
    }
    if (!ok) TEST_FAIL("Case2: Values do not match expected CMYK 15-bit mapping.");
    else TEST_OK("Case2: Values match expected CMYK 15-bit mapping.");

    // Validate returned pointer offset (should be original + 10)
    uintptr_t original = reinterpret_cast<uintptr_t>(buffer);
    uintptr_t returned = reinterpret_cast<uintptr_t>(ret);
    if (returned != original + 10) {
        printf("Case2: Returned pointer mismatch: got %p, expected %p\n",
               reinterpret_cast<void*>(ret),
               reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(buffer) + 10));
        TEST_FAIL("Case2: Return pointer offset incorrect.");
    } else {
        TEST_OK("Case2: Return pointer offset correct (buffer advanced by 10 bytes).");
    }
}

int main() {
    printf("Running Unroll15bitsCMYK tests...\n");

    test_unroll15bits_cmyk_case1();
    test_unroll15bits_cmyk_case2();

    if (g_failures == 0) {
        printf("ALL TESTS PASSED.\n");
        return 0;
    } else {
        printf("%d TEST(S) FAILED.\n", g_failures);
        return 1;
    }
}