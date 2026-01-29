/*
  Unit test suite for Unroll1ByteReversed in cmspack.c
  - Uses a lightweight, non-terminating test harness (no GTest)
  - Tests core behavior: wIn[0..2] equal to REVERSE_FLAVOR_16(FROM_8_TO_16(*accum)) for the
    input byte, and the accum pointer is advanced by one.
  - Covers multiple input values and multiple successive calls.
  - Assumes the test environment provides lcms2_internal.h (types/macros) used by the focal method.
  - All tests are self-contained and executable under C++11.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Include necessary LCMS internal declarations to match the focal method's environment.
// This header should define cmsUInt8Number, cmsUInt16Number, cmsUInt32Number, CMSREGISTER, etc.

// Declare the focal function with C linkage to ensure proper binding when included in C++ tests.
extern "C" cmsUInt8Number* Unroll1ByteReversed(CMSREGISTER _cmsTRANSFORM* info,
                                              CMSREGISTER cmsUInt16Number wIn[],
                                              CMSREGISTER cmsUInt8Number* accum,
                                              CMSREGISTER cmsUInt32Number Stride);

// Lightweight test harness (non-terminating assertions allowed to maximize coverage)
struct TestResult {
    const char* name;
    bool passed;
};

// Test 1: Basic zero input
// Verifies that for input byte 0, wIn[0..2] are all equal to 0 and accum advances by one.
bool test_Unroll1ByteReversed_basic_zero() {
    cmsUInt16Number wIn[3] = {0, 1, 2}; // pre-fill with non-equal values to ensure reset
    cmsUInt8Number accumBuf[4] = {0, 0, 0, 0}; // initial accum value is 0
    cmsUInt8Number* ret = Unroll1ByteReversed(nullptr, wIn, accumBuf, 0);

    bool ok = true;
    // Expect the return pointer to point to the next accumulator byte
    if (ret != accumBuf + 1) {
        std::cerr << "FAIL test_basic_zero: return pointer not advanced by 1" << std::endl;
        ok = false;
    }
    // Expect all filled wIn entries to be the same and equal to 0
    if (!(wIn[0] == 0 && wIn[1] == 0 && wIn[2] == 0)) {
        std::cerr << "FAIL test_basic_zero: wIn values are not all zero as expected" << std::endl;
        ok = false;
    }

    return ok;
}

// Test 2: Basic non-zero input (1)
// Verifies that for input byte 1, wIn[0..2] equal to 0x0101 and accum advances by one.
bool test_Unroll1ByteReversed_basic_one() {
    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt8Number accumBuf[4] = {1, 0, 0, 0}; // initial accum value is 1
    cmsUInt8Number* ret = Unroll1ByteReversed(nullptr, wIn, accumBuf, 0);

    bool ok = true;
    if (ret != accumBuf + 1) {
        std::cerr << "FAIL test_basic_one: return pointer not advanced by 1" << std::endl;
        ok = false;
    }
    // 0x01 -> FROM_8_TO_16(1) == 0x0101; REVERSE_FLAVOR_16 keeps 0x0101 for identical bytes
    if (wIn[0] != 0x0101 || wIn[1] != 0x0101 || wIn[2] != 0x0101) {
        std::cerr << "FAIL test_basic_one: wIn values are not 0x0101 as expected" << std::endl;
        ok = false;
    }

    return ok;
}

// Test 3: Basic non-zero input (255)
// Verifies that for input byte 255, wIn[0..2] equal to 0xFFFF and accum advances by one.
bool test_Unroll1ByteReversed_basic_255() {
    cmsUInt16Number wIn[3] = {123, 456, 789}; // ensure pre-values are different
    cmsUInt8Number accumBuf[4] = {255, 0, 0, 0}; // initial accum value is 255
    cmsUInt8Number* ret = Unroll1ByteReversed(nullptr, wIn, accumBuf, 0);

    bool ok = true;
    if (ret != accumBuf + 1) {
        std::cerr << "FAIL test_basic_255: return pointer not advanced by 1" << std::endl;
        ok = false;
    }
    if (wIn[0] != 0xFFFF || wIn[1] != 0xFFFF || wIn[2] != 0xFFFF) {
        std::cerr << "FAIL test_basic_255: wIn values are not 0xFFFF as expected" << std::endl;
        ok = false;
    }

    return ok;
}

// Test 4: Multiple successive calls using successive accum values
// Verifies that two consecutive calls with accum values 0x02 and then 0x03 produce 0x0202 and 0x0303
bool test_Unroll1ByteReversed_multiple_calls() {
    cmsUInt16Number wIn[3];
    cmsUInt8Number accumBuf[4] = {2, 3, 0, 0}; // first accum value is 2, then 3
    cmsUInt8Number* ret1 = Unroll1ByteReversed(nullptr, wIn, accumBuf, 0);

    bool ok = true;
    if (ret1 != accumBuf + 1) {
        std::cerr << "FAIL test_multiple_calls: first call - return pointer not advanced by 1" << std::endl;
        ok = false;
    }
    if (wIn[0] != 0x0202 || wIn[1] != 0x0202 || wIn[2] != 0x0202) {
        std::cerr << "FAIL test_multiple_calls: first call - wIn values not 0x0202" << std::endl;
        ok = false;
    }

    // Second call using the pointer returned from first call (accumBuf+1)
    cmsUInt16Number wIn2[3] = {0, 0, 0};
    cmsUInt8Number* ret2 = Unroll1ByteReversed(nullptr, wIn2, ret1, 0);
    if (ret2 != accumBuf + 2) {
        std::cerr << "FAIL test_multiple_calls: second call - return pointer not advanced by 1" << std::endl;
        ok = false;
    }
    if (wIn2[0] != 0x0303 || wIn2[1] != 0x0303 || wIn2[2] != 0x0303) {
        std::cerr << "FAIL test_multiple_calls: second call - wIn values not 0x0303" << std::endl;
        ok = false;
    }

    return ok;
}

// Runner that executes all tests and prints a summary
int main() {
    TestResult results[4];
    int idx = 0;

    // Run tests
    results[idx++] = { "Unroll1ByteReversed_basic_zero", test_Unroll1ByteReversed_basic_zero() ? true : false };
    results[idx - 1].name = "Unroll1ByteReversed_basic_zero";

    // Actually store each test result explicitly to ensure unique naming in output
    results[0] = { "Unroll1ByteReversed_basic_zero", test_Unroll1ByteReversed_basic_zero() };
    results[1] = { "Unroll1ByteReversed_basic_one",  test_Unroll1ByteReversed_basic_one() };
    results[2] = { "Unroll1ByteReversed_basic_255", test_Unroll1ByteReversed_basic_255() };
    results[3] = { "Unroll1ByteReversed_multiple_calls", test_Unroll1ByteReversed_multiple_calls() };

    int total = 4;
    int passed = 0;

    std::cout << "Running Unroll1ByteReversed unit tests (no GTest, lightweight harness)..." << std::endl;
    for (int i = 0; i < total; ++i) {
        const auto& r = results[i];
        bool ok = r.passed;
        // Some compilers may not populate TestResult.passed for all entries due to how we filled it;
        // ensure we treat a truthy 'passed' as success.
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << r.name << std::endl;
        } else {
            std::cout << "[FAIL] " << r.name << std::endl;
        }
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}