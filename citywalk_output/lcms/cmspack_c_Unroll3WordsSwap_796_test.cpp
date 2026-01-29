// Test suite for the focal method Unroll3WordsSwap from cmspack.c
// This test is designed to be compiled and linked with the project's cmspack.c
// and any necessary headers. It avoids Google Test and uses a lightweight
// in-house assertion mechanism to maximize code coverage and execution.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Lightweight type aliases to match the focal function signature.
// These definitions are provided so the test code can be compiled
// independently of the full project headers. They are kept minimal
// and match the common CMS/LittleCMS typedefs used in cmspack.c.
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

// Forward declaration of the opaque transform type used by the focal function.
// We do not rely on its internals; we pass a null pointer to satisfy the API.
typedef struct _cmsTRANSFORM cmsTRANSFORM;
#define CMSREGISTER

// Forward declaration of the focal method under test with C linkage.
// The exact signature mirrors the one in the provided focal code.
extern "C" cmsUInt8Number* Unroll3WordsSwap(CMSREGISTER cmsTRANSFORM* info,
                                           CMSREGISTER cmsUInt16Number wIn[],
                                           CMSREGISTER cmsUInt8Number* accum,
                                           CMSREGISTER cmsUInt32Number Stride);

// Simple non-terminating assertion mechanism.
// It records failures but allows execution to continue, enabling broader coverage.
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "ASSERT FAILED: " << (msg) << " (in " << __func__ << ")\n"; \
        ++g_failures; \
    } \
} while(false)

// Utility to write a 16-bit value into a byte buffer in little-endian order.
// This helps create platform-independent test buffers for the function under test.
static inline void write16_le(unsigned short value, unsigned char* buffer) {
    buffer[0] = static_cast<unsigned char>(value & 0xFF);
    buffer[1] = static_cast<unsigned char>((value >> 8) & 0xFF);
}

// Test 1: Basic behavior - verify that Unroll3WordsSwap reads three 16-bit values
// from the accumulator in the order YB, MG, RC and stores them into wIn as
// wIn[2], wIn[1], wIn[0] respectively. Also verify the returned pointer advances by 6 bytes.
static void test_Unroll3WordsSwap_basicOrdering() {
    // Prepare input wIn and memory buffer containing 3 16-bit values (little-endian).
    cmsUInt16Number wIn[3] = {0, 0, 0};
    unsigned char buffer[6];
    // Values to place in accum (will be read in order into wIn[2], wIn[1], wIn[0]).
    unsigned short v0 = 0x1122; // YB (read first)
    unsigned short v1 = 0x3344; // MG (read second)
    unsigned short v2 = 0x5566; // RC (read third)
    write16_le(v0, buffer);
    write16_le(v1, buffer + 2);
    write16_le(v2, buffer + 4);

    cmsTRANSFORM* info = nullptr;
    cmsUInt8Number* accum = buffer;
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* ret = Unroll3WordsSwap(info, wIn, accum, Stride);

    // Expected mapping:
    // wIn[2] <- v0, wIn[1] <- v1, wIn[0] <- v2
    TEST_ASSERT(wIn[0] == v2, "wIn[0] should be equal to v2 (0x5566)");
    TEST_ASSERT(wIn[1] == v1, "wIn[1] should be equal to v1 (0x3344)");
    TEST_ASSERT(wIn[2] == v0, "wIn[2] should be equal to v0 (0x1122)");
    // Returned pointer should point 6 bytes past the start of buffer.
    TEST_ASSERT(ret == buffer + 6, "Returned accum pointer should advance by 6 bytes");
}

// Test 2: Variant with different values to ensure swap/read semantics are preserved
// across multiple invocations and that the function handles arbitrary input data.
static void test_Unroll3WordsSwap_withDifferentValues() {
    cmsUInt16Number wIn[3] = {0, 0, 0};
    unsigned char buffer[6];
    unsigned short a = 0xDEAD;
    unsigned short b = 0xBEEF;
    unsigned short c = 0xCAFE;
    write16_le(a, buffer);
    write16_le(b, buffer + 2);
    write16_le(c, buffer + 4);

    cmsTRANSFORM* info = nullptr;
    cmsUInt8Number* accum = buffer;
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* ret = Unroll3WordsSwap(info, wIn, accum, Stride);

    // wIn[2] should be a, wIn[1] should be b, wIn[0] should be c
    TEST_ASSERT(wIn[0] == c, "wIn[0] should be equal to c (0xCAFE)");
    TEST_ASSERT(wIn[1] == b, "wIn[1] should be equal to b (0xBEEF)");
    TEST_ASSERT(wIn[2] == a, "wIn[2] should be equal to a (0xDEAD)");
    TEST_ASSERT(ret == buffer + 6, "Returned accum pointer should advance by 6 bytes");
}

// Main function to execute tests.
// Per domain knowledge: invoke test methods from main when gtest is not used.
int main() {
    std::cout << "Starting Unroll3WordsSwap unit tests (no GTest)..." << std::endl;

    test_Unroll3WordsSwap_basicOrdering();
    test_Unroll3WordsSwap_withDifferentValues();

    if(g_failures == 0) {
        std::cout << "All Unroll3WordsSwap tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " Unroll3WordsSwap test(s) FAILED." << std::endl;
        return 1;
    }
}