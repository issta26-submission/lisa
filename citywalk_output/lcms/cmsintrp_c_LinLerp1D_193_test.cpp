// This is a C++11 test harness (no GoogleTest) for the focal function LinLerp1D
// that is defined in cmsintrp.c. It relies on the public-facing type names
// commonly used in LittleCMS (cmsUInt16Number, cmsInterpParams, etc.).
// The tests cover both branches of the predicate inside LinLerp1D.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Domain knowledge assumptions for test scaffolding
// We provide minimal type aliases and a compatible cmsInterpParams layout
// that should align with the declarations used by cmsintrp.c.
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// The actual cmsInterpParams in the project typically exposes at least:
// - void* Table
// - cmsUInt32Number Domain[...]
// We'll provide a flexible Domain[4] to be safe for test usage
typedef struct cmsInterpParams {
    void* Table;
    cmsUInt32Number Domain[4];
} cmsInterpParams;

// Macro used in the focal function; defined empty for the test harness
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Declaration of the focal function (assumed to have C linkage)
extern "C" void LinLerp1D(CMSREGISTER const cmsUInt16Number Value[],
                          CMSREGISTER cmsUInt16Number Output[],
                          CMSREGISTER const cmsInterpParams* p);

// Helper macro for simple non-terminating assertions (non-GTest)
#define ASSERT_EQ(expected, actual, label)                                 \
    do {                                                                     \
        if ((static_cast<cmsUInt32Number>(actual)) != (static_cast<cmsUInt32Number>(expected))) { \
            std::cerr << "FAIL: " << label                                 \
                      << " | expected: " << static_cast<uint32_t>(expected)   \
                      << ", actual: " << static_cast<uint32_t>(actual)       \
                      << std::endl;                                         \
            return false;                                                  \
        } else {                                                           \
            std::cout << "PASS: " << label << std::endl;                   \
        }                                                                  \
    } while (0)

// Test A: Ensure the "last value or just one point" path is taken
// Condition: Value[0] == 0xffff or p->Domain[0] == 0
// We choose a nonzero Domain[0] to test the sentinel path via Domain[0].
bool test_branch_last_value_or_single_point() {
    // Setup a simple LUT with predictable values
    cmsUInt16Number lut[] = { 10, 20, 30, 40 }; // lut[0]..lut[3]
    cmsInterpParams p;
    std::memset(&p, 0, sizeof(p));
    p.Table = lut;
    p.Domain[0] = 2; // pick an index within lut

    cmsUInt16Number Value[1] = { 0xffff }; // sentinel triggers first branch
    cmsUInt16Number Output[1] = { 0 };

    LinLerp1D(Value, Output, &p);

    // Expected: Output[0] should be lut[ Domain[0] ]
    uint32_t expected = lut[p.Domain[0]]; // lut[2] == 30
    ASSERT_EQ(expected, Output[0], "A: last-value branch returns lut[Domain[0]]");
    return true;
}

// Test B: False branch is taken when Domain[0] != 0 and Value[0] != 0xffff
// We ensure that Output equals lut[cell0] (or Interpolated value depending on rest).
// To avoid dealing with fixed-point math intricacies, ensure y0 and y1 are equal
// so LinearInterp(rest, y0, y1) yields y0 regardless of rest.
bool test_branch_interpolation_path_with_equal_neighbors() {
    cmsUInt16Number lut[] = { 0, 20, 20, 40, 60 }; // ensure lut[1] == lut[2] == 20
    cmsInterpParams p;
    std::memset(&p, 0, sizeof(p));
    p.Table = lut;
    p.Domain[0] = 1; // choose a domain that yields cell0 == 1, rest possibly nonzero

    cmsUInt16Number Value[1] = { 1 }; // non-sentinel value
    cmsUInt16Number Output[1] = { 0 };

    LinLerp1D(Value, Output, &p);

    // With lut[1] == lut[2], the interpolation should yield 20 regardless of rest
    uint32_t expected = lut[1]; // 20
    ASSERT_EQ(expected, Output[0], "B: else-branch with equal y0 and y1 yields y0 (20)");
    return true;
}

// Test C: Ensure the interpolation path executes and yields a known value
// This test uses a scenario where y0 != y1 but we still can predict the result
// by selecting rest such that LinearInterp(rest, y0, y1) equals y0.
// Since we do not rely on the exact fixed-point math (which is hidden),
// we instead ensure that the function completes and outputs a value equal to
// one of the LUT endpoints under a controlled setup.
bool test_branch_interpolation_path_basic() {
    cmsUInt16Number lut[] = { 5, 25, 45, 65 }; // distinct endpoints
    cmsInterpParams p;
    std::memset(&p, 0, sizeof(p));
    p.Table = lut;
    p.Domain[0] = 1; // cell0 will likely be 1 given typical fixed-point interpretation

    cmsUInt16Number Value[1] = { 1 }; // triggers else-branch
    cmsUInt16Number Output[1] = { 0 };

    LinLerp1D(Value, Output, &p);

    // We can't predict exact rest without knowing _cmsToFixedDomain,
    // but we can validate that Output is one of the LUT endpoints (either 25 or 45).
    uint16_t got = Output[0];
    if (got == lut[1] || got == lut[2]) {
        std::cout << "PASS: C: interpolation path produced a valid endpoint value (" << got << ")" << std::endl;
        return true;
    } else {
        std::cerr << "FAIL: C: interpolation path produced unexpected value (" << got << ")" << std::endl;
        return false;
    }
}

// Main: run all tests and report summary
int main() {
    bool pass = true;

    std::cout << "Running LinLerp1D unit tests (no GTest)..." << std::endl;

    pass &= test_branch_last_value_or_single_point();
    pass &= test_branch_interpolation_path_with_equal_neighbors();
    pass &= test_branch_interpolation_path_basic();

    if (pass) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}