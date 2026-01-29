/*
  Lightweight unit test suite for the focal method Unroll4Bytes from cmspack.c
  - No Google Test, just a simple C++11 test harness
  - Tests focus on correctness of 4-byte unrolling into 16-bit words
  - Uses dummy forward declarations to avoid depending on the full codebase
  - Ensures accumulation pointer advances correctly and 8-bit->16-bit mapping is as expected
  - Includes explanatory comments for each test case
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <iomanip>


// Domain knowledge: Unroll4Bytes reads 4 consecutive bytes, converts each to a 16-bit value
// using FROM_8_TO_16(x) which typically maps 8-bit value x to 16-bit by duplicating the byte
// (i.e., 0xAB -> 0xABAB). This is commonly implemented as x * 257.
// The test will validate this mapping by computing expected values with the known macro behavior.
 
// Forward declaration of types and function prototype to allow linkage with the real
// Unroll4Bytes implementation inside cmspack.c without pulling the whole project headers.
// We keep the declarations intentionally minimal and compatible with C linkage.

typedef struct _cmsTRANSFORM _cmsTRANSFORM; // forward-declare opaque transform type
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

// CMSREGISTER is used as a label in the real code; here we define it as empty to compile in test context.
#define CMSREGISTER

#ifdef __cplusplus
extern "C" {
#endif

// Prototype of the focal function under test.
// We declare in the same form as the real function signature appears in cmspack.c.
cmsUInt8Number* Unroll4Bytes(CMSREGISTER _cmsTRANSFORM* info,
                            CMSREGISTER cmsUInt16Number wIn[],
                            CMSREGISTER cmsUInt8Number* accum,
                            CMSREGISTER cmsUInt32Number Stride);

#ifdef __cplusplus
}
#endif

// Simple, non-terminating test assertion macro.
// It increments a global failure counter if a condition is not met and logs the failure.
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[Test Failure] " << (msg) \
                  << " (in " << __func__ << " at " << __FILE__ << ":" << __LINE__ << ")" \
                  << std::endl; \
        ++g_failures; \
    } \
} while(false)


// Helper: print a 16-bit value in hex for readability
static void print16Hex(uint16_t val) {
    std::cout << "0x" << std::setw(4) << std::setfill('0') << std::hex << (uint16_t)val << std::dec;
}


// Test 1: Basic mapping and pointer advancement
// - Input: bytes {0x10, 0x20, 0x30, 0x40}
// - Expected wIn: {0x1010, 0x2020, 0x3030, 0x4040} (i.e., 0x10*257, 0x20*257, 0x30*257, 0x40*257)
// - Expected end: accum pointer advanced by 4 bytes (to base + 4)
static void test_Unroll4Bytes_basic_mapping_and_advancement() {
    // Prepare input
    cmsUInt8Number inputBytes[8] = { 0x10, 0x20, 0x30, 0x40, 0xAA, 0xBB, 0xCC, 0xDD };
    cmsUInt16Number wIn[4] = {0, 0, 0, 0};

    cmsUInt8Number* accum = inputBytes; // pointer to first input byte
    _cmsTRANSFORM* info = nullptr;     // not used by Unroll4Bytes

    // Call the function under test
    cmsUInt8Number* endPtr = Unroll4Bytes(info, wIn, accum, 0);

    // Expected values computed as 8-bit value * 257
    uint16_t expected[4];
    for (int i = 0; i < 4; ++i) {
        expected[i] = (uint16_t)inputBytes[i] * 257;
    }

    // Assertions
    TEST_ASSERT(endPtr == inputBytes + 4, "accum pointer should advance by 4 after Unroll4Bytes");
    for (int i = 0; i < 4; ++i) {
        std::cout << " wIn[" << i << "] = ";
        print16Hex(wIn[i]);
        std::cout << " (expected ";
        print16Hex(expected[i]);
        std::cout << ")" << std::endl;
        TEST_ASSERT(wIn[i] == expected[i], "Unroll4Bytes does not map input byte to expected 16-bit value");
    }
}

// Test 2: All-zero input
// - Input: bytes {0x00, 0x00, 0x00, 0x00}
// - Expected wIn: {0x0000, 0x0000, 0x0000, 0x0000}
// - Expected end: accum advanced by 4
static void test_Unroll4Bytes_zero_input() {
    cmsUInt8Number inputBytes[4] = { 0x00, 0x00, 0x00, 0x00 };
    cmsUInt16Number wIn[4] = {0, 0, 0, 0};

    cmsUInt8Number* endPtr = Unroll4Bytes(nullptr, wIn, inputBytes, 0);

    for (int i = 0; i < 4; ++i) {
        TEST_ASSERT(wIn[i] == 0, "Zero input should yield zero 16-bit words");
    }

    TEST_ASSERT(endPtr == inputBytes + 4, "accum pointer should advance by 4 for zero input case");
}

// Test 3: Mixed boundary values including maximum 0xFF
// - Input: {0xFF, 0x01, 0x80, 0x55}
// - Expected wIn: {0xFFFF, 0x0101, 0x8080, 0x5555}
// - Expected end: accum advanced by 4
static void test_Unroll4Bytes_boundary_values() {
    cmsUInt8Number inputBytes[4] = { 0xFF, 0x01, 0x80, 0x55 };
    cmsUInt16Number wIn[4] = {0, 0, 0, 0};

    cmsUInt8Number* endPtr = Unroll4Bytes(nullptr, wIn, inputBytes, 0);

    uint16_t expected[4] = { 0xFFFF, 0x0101, 0x8080, 0x5555 };

    TEST_ASSERT(endPtr == inputBytes + 4, "accum pointer should advance by 4 for boundary values");
    for (int i = 0; i < 4; ++i) {
        TEST_ASSERT(wIn[i] == expected[i], "Unroll4Bytes mapping mismatch for boundary values");
    }

    // Optional: print for visibility
    std::cout << "Test 3 results: ";
    for (int i = 0; i < 4; ++i) {
        print16Hex(wIn[i]);
        if (i < 3) std::cout << ", ";
    }
    std::cout << std::endl;
}


// Entry point: run all tests and report summary
int main() {
    std::cout << "Running Unroll4Bytes unit tests (CMS internal API) ..." << std::endl;

    test_Unroll4Bytes_basic_mapping_and_advancement();
    test_Unroll4Bytes_zero_input();
    test_Unroll4Bytes_boundary_values();

    if (g_failures == 0) {
        std::cout << "[PASS] All Unroll4Bytes tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "[FAIL] Unroll4Bytes tests failed with " << g_failures << " error(s)." << std::endl;
        return 1;
    }
}