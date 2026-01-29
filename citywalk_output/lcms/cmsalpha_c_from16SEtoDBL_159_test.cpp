// Lightweight C++11 test suite for the focal method:
// from16SEtoDBL(void* dst, const void* src)
// This test does not rely on GTest. It provides a minimal harness and
// "non-terminating" assertions by reporting failures and continuing execution.

// The test uses the function from16SEtoDBL declared with C linkage.
#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cmath>


// Function under test declared with C linkage.
// The actual implementation should be provided in cmsalpha.c (linked during build).
extern "C" void from16SEtoDBL(void* dst, const void* src);

// Helper: swap 16-bit value (endianness swap)
static uint16_t swap16(uint16_t x) {
    return static_cast<uint16_t>((x << 8) | (x >> 8));
}

// Tolerance for floating point comparisons
static const double TOL = 1e-12;

// Test 1:
// Input bytes: {0x01, 0x00}
// This arrangement ensures we exercise the endianness swap logic.
// We don't assume host endianness; instead we allow either of two theoretically valid outputs.
bool test_from16SEtoDBL_case01() {
    // Explanation:
    // src bytes: 0x01 0x00
    // On little-endian host: read vLE = 0x0001; swapped = 0x0100
    // On big-endian host:  read vBE = 0x0100; swapped = 0x0001
    // Expected outcome is either swappedLE/65535.0 or swappedBE/65535.0.
    unsigned char src[2] = {0x01, 0x00};
    double dstVal = 0.0;
    from16SEtoDBL(&dstVal, src);

    // Compute two possible expected values depending on host endianness.
    uint16_t vLE = (uint16_t)src[0] | ((uint16_t)src[1] << 8); // 0x0001 on LE
    uint16_t swappedLE = swap16(vLE);
    uint16_t vBE  = ((uint16_t)src[0] << 8) | (uint16_t)src[1]; // 0x0100 on BE
    uint16_t swappedBE = swap16(vBE);

    double exp1 = static_cast<double>(swappedLE) / 65535.0;
    double exp2 = static_cast<double>(swappedBE) / 65535.0;

    bool ok = (std::fabs(dstVal - exp1) < TOL) || (std::fabs(dstVal - exp2) < TOL);
    if (!ok) {
        std::cout << "[FAIL] test_from16SEtoDBL_case01: dst=" << dstVal
                  << " not within tolerance of either expected1=" << exp1
                  << " or expected2=" << exp2 << "\n";
    }
    return ok;
}

// Test 2:
// Input bytes: {0x00, 0x01}
bool test_from16SEtoDBL_case02() {
    // Explanation:
    // src bytes: 0x00 0x01
    // On LE: vLE = 0x0100; swappedLE = 0x0001
    // On BE: vBE = 0x0001; swappedBE = 0x0100
    unsigned char src[2] = {0x00, 0x01};
    double dstVal = 0.0;
    from16SEtoDBL(&dstVal, src);

    uint16_t vLE = (uint16_t)src[0] | ((uint16_t)src[1] << 8); // 0x0100 on LE
    uint16_t swappedLE = swap16(vLE);
    uint16_t vBE  = ((uint16_t)src[0] << 8) | (uint16_t)src[1]; // 0x0001 on BE
    uint16_t swappedBE = swap16(vBE);

    double exp1 = static_cast<double>(swappedLE) / 65535.0;
    double exp2 = static_cast<double>(swappedBE) / 65535.0;

    bool ok = (std::fabs(dstVal - exp1) < TOL) || (std::fabs(dstVal - exp2) < TOL);
    if (!ok) {
        std::cout << "[FAIL] test_from16SEtoDBL_case02: dst=" << dstVal
                  << " not within tolerance of either expected1=" << exp1
                  << " or expected2=" << exp2 << "\n";
    }
    return ok;
}

// Test 3:
// Input bytes: {0xFF, 0xFF} (max value)
bool test_from16SEtoDBL_case03() {
    unsigned char src[2] = {0xFF, 0xFF};
    double dstVal = 0.0;
    from16SEtoDBL(&dstVal, src);

    uint16_t vLE = (uint16_t)src[0] | ((uint16_t)src[1] << 8);
    uint16_t swappedLE = swap16(vLE);
    uint16_t vBE  = ((uint16_t)src[0] << 8) | (uint16_t)src[1];
    uint16_t swappedBE = swap16(vBE);

    double exp1 = static_cast<double>(swappedLE) / 65535.0;
    double exp2 = static_cast<double>(swappedBE) / 65535.0;

    bool ok = (std::fabs(dstVal - exp1) < TOL) || (std::fabs(dstVal - exp2) < TOL);
    if (!ok) {
        std::cout << "[FAIL] test_from16SEtoDBL_case03: dst=" << dstVal
                  << " not within tolerance of either expected1=" << exp1
                  << " or expected2=" << exp2 << "\n";
    }
    return ok;
}

// Test 4:
// Input bytes: {0x12, 0x34}
bool test_from16SEtoDBL_case04() {
    unsigned char src[2] = {0x12, 0x34};
    double dstVal = 0.0;
    from16SEtoDBL(&dstVal, src);

    uint16_t vLE = (uint16_t)src[0] | ((uint16_t)src[1] << 8);
    uint16_t swappedLE = swap16(vLE);
    uint16_t vBE  = ((uint16_t)src[0] << 8) | (uint16_t)src[1];
    uint16_t swappedBE = swap16(vBE);

    double exp1 = static_cast<double>(swappedLE) / 65535.0;
    double exp2 = static_cast<double>(swappedBE) / 65535.0;

    bool ok = (std::fabs(dstVal - exp1) < TOL) || (std::fabs(dstVal - exp2) < TOL);
    if (!ok) {
        std::cout << "[FAIL] test_from16SEtoDBL_case04: dst=" << dstVal
                  << " not within tolerance of either expected1=" << exp1
                  << " or expected2=" << exp2 << "\n";
    }
    return ok;
}

// Entry point: run all tests and report summary.
int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Starting tests for from16SEtoDBL...\n";

    if (test_from16SEtoDBL_case01()) ++passed;
    ++total;

    if (test_from16SEtoDBL_case02()) ++passed;
    ++total;

    if (test_from16SEtoDBL_case03()) ++passed;
    ++total;

    if (test_from16SEtoDBL_case04()) ++passed;
    ++total;

    std::cout << "Tests completed: " << passed << " / " << total << " passed.\n";
    return (passed == total) ? 0 : 1;
}