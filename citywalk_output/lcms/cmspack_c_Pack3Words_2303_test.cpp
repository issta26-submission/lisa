// Test suite for the focal method: Pack3Words in cmspack.c
// This file provides a small, self-contained unit test harness (no GTest) for C++11.
// It assumes the project exposes the function with C linkage and the necessary types
// alignable with the used typedefs below.
// The tests focus on validating the core behavior of Pack3Words as implemented in the focal method.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Candidate Keywords (core dependencies observed in the focal method and its dependencies):
// - Pack3Words: the function under test
// - cmsUInt8Number, cmsUInt16Number, cmsUInt32Number: basic scalar types used by the function
// - _cmsTRANSFORM: transform info structure (pointer, unused in the focal path)
// - cmsUNUSED_PARAMETER: macro used to silence unused params (info, Stride)
 
// Lightweight, test-friendly type definitions to enable compilation in environments
// where the actual cms types may not be available in the test harness.
// These typedefs align with typical CMS type definitions (uint8_t, uint16_t, uint32_t).
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// Forward declare the opaque _cmsTRANSFORM structure used by Pack3Words.
// The focal function never dereferences this in its body (it is unused), so forwarding is sufficient.
struct _cmsTRANSFORM;

// Declare the focal function with C linkage to match the actual implementation.
// We avoid depending on the CMS headers by providing a minimal, compatible prototype.
// The macro CMSREGISTER is assumed to be empty in typical builds; if not, the non-macro
// signature should still resolve to the same C-linkage symbol "Pack3Words".
extern "C" cmsUInt8Number* Pack3Words(struct _cmsTRANSFORM* info,
                                    cmsUInt16Number wOut[],
                                    cmsUInt8Number* output,
                                    cmsUInt32Number Stride);

// Simple test harness utilities
static bool assert_eq_uint16(const cmsUInt16Number a, const cmsUInt16Number b, const char* desc) {
    if (a != b) {
        std::cerr << "ASSERT FAILED: " << desc << " | Expected: " << static_cast<uint32_t>(b)
                  << ", Got: " << static_cast<uint32_t>(a) << std::endl;
        return false;
    }
    return true;
}
static bool assert_ptr_eq(const void* a, const void* b, const char* desc) {
    if (a != b) {
        std::cerr << "ASSERT FAILED: " << desc << " | Expected pointer: " << b
                  << ", Got: " << a << std::endl;
        return false;
    }
    return true;
}
static bool assert_true(bool cond, const char* desc) {
    if (!cond) {
        std::cerr << "ASSERT FAILED: " << desc << std::endl;
        return false;
    }
    return true;
}

// Test 1: Basic packing of 3 words into a 6-byte buffer
// - Verifies each 16-bit word is stored contiguously in memory (3 words -> 6 bytes)
// - Verifies the returned pointer equals the original output pointer advanced by 6 bytes
// - Uses a mix of small, clearly distinguishable values to verify memory layout
bool test_Pack3Words_BasicPacking() {
    cmsUInt16Number wOut[3] = { 0x0123, 0x0456, 0x0789 };
    cmsUInt8Number buffer[6] = { 0 }; // initialize to known pattern
    cmsUInt8Number* ret = Pack3Words(nullptr, wOut, buffer, 0);

    // Returned pointer should be buffer + 6
    if (!assert_ptr_eq(static_cast<void*>(ret), static_cast<void*>(buffer + 6),
                       "Return value should be buffer + 6")) {
        return false;
    }

    // Read back the 3 packed words from the 6-byte buffer using memcpy to avoid aliasing concerns
    cmsUInt16Number got0 = 0, got1 = 0, got2 = 0;
    std::memcpy(&got0, buffer, 2);
    std::memcpy(&got1, buffer + 2, 2);
    std::memcpy(&got2, buffer + 4, 2);

    bool ok = true;
    ok &= assert_eq_uint16(got0, wOut[0], "First packed word should equal wOut[0]");
    ok &= assert_eq_uint16(got1, wOut[1], "Second packed word should equal wOut[1]");
    ok &= assert_eq_uint16(got2, wOut[2], "Third packed word should equal wOut[2]");

    // Also ensure no writes beyond the 6 bytes were performed
    // (Since buffer is only 6 bytes, this is a sanity check against overflow)
    // Note: As the buffer is fixed at 6, the sanity test is implicit by no memory access beyond 5 index.
    return ok;
}

// Test 2: Packing with different input values including max 0xFFFF
// - Ensures the function handles larger 16-bit numbers correctly
bool test_Pack3Words_VariedInputs() {
    cmsUInt16Number wOut[3] = { 0xFFFF, 0x8000, 0x0001 };
    cmsUInt8Number buffer[6] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF }; // pre-fill to detect changes

    cmsUInt8Number* ret = Pack3Words(nullptr, wOut, buffer, 7); // non-zero Stride to exercise unused param path

    if (!assert_ptr_eq(static_cast<void*>(ret), static_cast<void*>(buffer + 6),
                       "Return value should be buffer + 6 (varied inputs)")) {
        return false;
    }

    cmsUInt16Number got0 = 0, got1 = 0, got2 = 0;
    std::memcpy(&got0, buffer, 2);
    std::memcpy(&got1, buffer + 2, 2);
    std::memcpy(&got2, buffer + 4, 2);

    bool ok = true;
    ok &= assert_eq_uint16(got0, wOut[0], "First packed word (max) should equal wOut[0]");
    ok &= assert_eq_uint16(got1, wOut[1], "Second packed word (max) should equal wOut[1]");
    ok &= assert_eq_uint16(got2, wOut[2], "Third packed word (max) should equal wOut[2]");

    return ok;
}

// Test 3: Ensure memory beyond the 6 packed bytes remains untouched
// - Pre-fills a larger buffer and checks that indices 6 and 7 are unchanged after packing
bool test_Pack3Words_NoOverflowBeyond6() {
    cmsUInt16Number wOut[3] = { 0x1111, 0x2222, 0x3333 };
    cmsUInt8Number buffer[8] = { 0xAA,0xBB,0xCC,0xDD,0xEE,0xFF, 0x11, 0x22 };

    cmsUInt8Number* ret = Pack3Words(nullptr, wOut, buffer, 0);

    if (!assert_ptr_eq(static_cast<void*>(ret), static_cast<void*>(buffer + 6),
                       "Return value should be buffer + 6 (no overflow test)")) {
        return false;
    }

    // Verify first 6 bytes correspond to wOut values (via 16-bit reads)
    cmsUInt16Number got0, got1, got2;
    std::memcpy(&got0, buffer, 2);
    std::memcpy(&got1, buffer + 2, 2);
    std::memcpy(&got2, buffer + 4, 2);

    bool ok = true;
    ok &= assert_eq_uint16(got0, wOut[0], "First packed word (no overflow) should equal wOut[0]");
    ok &= assert_eq_uint16(got1, wOut[1], "Second packed word (no overflow) should equal wOut[1]");
    ok &= assert_eq_uint16(got2, wOut[2], "Third packed word (no overflow) should equal wOut[2]");

    // Check that bytes 6 and 7 are unchanged
    ok &= assert_eq_uint16(buffer[6], static_cast<cmsUInt8Number>(0x11), "Byte at index 6 should remain unchanged");
    ok &= assert_eq_uint16(buffer[7], static_cast<cmsUInt8Number>(0x22), "Byte at index 7 should remain unchanged");

    return ok;
}

// Entry point: run all tests
int main() {
    bool all_passed = true;

    std::cout << "Running Pack3Words unit tests (no GTest, C++11)..." << std::endl;

    if (!test_Pack3Words_BasicPacking()) {
        std::cerr << "Test 1 (BasicPacking) FAILED." << std::endl;
        all_passed = false;
    } else {
        std::cout << "Test 1 (BasicPacking) PASSED." << std::endl;
    }

    if (!test_Pack3Words_VariedInputs()) {
        std::cerr << "Test 2 (VariedInputs) FAILED." << std::endl;
        all_passed = false;
    } else {
        std::cout << "Test 2 (VariedInputs) PASSED." << std::endl;
    }

    if (!test_Pack3Words_NoOverflowBeyond6()) {
        std::cerr << "Test 3 (NoOverflowBeyond6) FAILED." << std::endl;
        all_passed = false;
    } else {
        std::cout << "Test 3 (NoOverflowBeyond6) PASSED." << std::endl;
    }

    if (all_passed) {
        std::cout << "ALL PACK3WORDS TESTS PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << "SOME PACK3WORDS TESTS FAILED." << std::endl;
        return 1;
    }
}