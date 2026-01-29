// Minimal C++11 test suite for the focal method: SlopeLimiting
// This test suite avoids GTest and uses a lightweight, non-terminating assertion style.
// It targets the SlopeLimiting function declared in the provided FOCAL_CLASS_DEP.
// The tests focus on exercising both branches of the conditional predicate and
// key aspects of the slope-limiting logic.
//
// Notes:
// - We assume cmsUInt16Number is a 16-bit unsigned type (uint16_t) consistent with Little CMS.
// - We avoid depending on private/internal details beyond what SlopeLimiting uses.
// - The tests are designed to be safe with respect to saturating behavior by choosing inputs
//   that yield well-defined, deterministic outputs (within 0..0xFFFF).

#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdint>


// Forward declare the focal function as it appears in the C source.
// We assume the function uses C linkage; hence extern "C" is used to avoid name mangling.
extern "C" void SlopeLimiting(uint16_t* Table16, int nEntries);

// Type alias matching the focal code's expected types
using cmsUInt16Number = uint16_t;

// 0xFFFF constant for readability
static constexpr cmsUInt16Number WORD_MAX = 0xFFFF;

// Lightweight test harness (non-terminating, accumulative results)
static bool test_SlopeLimiting_Increasing_Branch_LastIsFFFF() {
    // Case: Table16[0] <= Table16[nEntries-1], take the "else" branch.
    // nEntries = 50 yields AtBegin = floor(0.02*50 + 0.5) = floor(1.0 + 0.5) = 1
    const int nEntries = 50;
    std::vector<cmsUInt16Number> Table16(nEntries);

    // Create a strictly increasing table to ensure the "else" branch executes.
    // Values are small enough to avoid implicit saturations during arithmetic.
    for (int i = 0; i < nEntries; ++i) {
        Table16[i] = static_cast<cmsUInt16Number>(i * 10);
    }

    // Call focal function
    SlopeLimiting(Table16.data(), nEntries);

    // Assertions (non-terminating): verify key expected outcomes
    int failures = 0;

    // Begin value should be preserved to 0 within the first 2% region for this setup
    if (Table16[0] != 0) {
        std::cerr << "[Increasing] Table16[0] = " << Table16[0] << " (expected 0)\n";
        ++failures;
    }

    // End value should be saturated to 0xFFFF due to the end boundary condition
    if (Table16[nEntries - 1] != WORD_MAX) {
        std::cerr << "[Increasing] Table16[" << (nEntries - 1) << "] = " << Table16[nEntries - 1]
                  << " (expected 0xFFFF)\n";
        ++failures;
    }

    // Check an interior point that should have been affected by the end-slope computation:
    // We computed earlier that index nEntries-2 should become 480 for this input.
    const cmsUInt16Number expectedPenultimate = 480;
    if (Table16[nEntries - 2] != expectedPenultimate) {
        std::cerr << "[Increasing] Table16[" << (nEntries - 2) << "] = "
                  << Table16[nEntries - 2] << " (expected " << expectedPenultimate << ")\n";
        ++failures;
    }

    if (failures > 0) {
        std::cerr << "[Increasing] Test FAILED with " << failures << " issue(s).\n";
        return false;
    }
    return true;
}

static bool test_SlopeLimiting_Decreasing_Branch_FirstElementFFFFLastZero() {
    // Case: Table16[0] > Table16[nEntries-1], take the "if" branch.
    // Use a decreasing sequence to trigger the true predicate.
    const int nEntries = 50;
    std::vector<cmsUInt16Number> Table16(nEntries);

    // Decreasing sequence (first > last)
    for (int i = 0; i < nEntries; ++i) {
        // Ensure a clean decreasing progression
        // Start at 1000 and decrease by 20 per step
        Table16[i] = static_cast<cmsUInt16Number>(1000 - i * 20);
    }

    // Call focal function
    SlopeLimiting(Table16.data(), nEntries);

    // Expectations derived from analytic calculation for this input pattern:
    // - Table16[0] should saturate to 0xFFFF (BeginVal branch)
    // - Table16[nEntries-1] should become 0
    // - Table16[nEntries-2] (i.e., index 48) should equal the original Val at AtEnd (40 in this setup)
    int failures = 0;

    if (Table16[0] != WORD_MAX) {
        std::cerr << "[Decreasing] Table16[0] = " << Table16[0]
                  << " (expected 0xFFFF)\n";
        ++failures;
    }

    if (Table16[nEntries - 1] != 0) {
        std::cerr << "[Decreasing] Table16[" << (nEntries - 1) << "] = "
                  << Table16[nEntries - 1] << " (expected 0)\n";
        ++failures;
    }

    // For this input, AtEnd = nEntries-2, and initial Table16[AtEnd] was 40
    const cmsUInt16Number expectedPenultimate = 40;
    if (Table16[nEntries - 2] != expectedPenultimate) {
        std::cerr << "[Decreasing] Table16[" << (nEntries - 2) << "] = "
                  << Table16[nEntries - 2] << " (expected " << expectedPenultimate << ")\n";
        ++failures;
    }

    if (failures > 0) {
        std::cerr << "[Decreasing] Test FAILED with " << failures << " issue(s).\n";
        return false;
    }
    return true;
}

static bool test_SlopeLimiting_AtBeginTwo_AllZeros() {
    // Case: nEntries chosen to yield AtBegin = floor(n*0.02 + 0.5) = 2.
    // We purposefully fill the array with zeros to keep things deterministic
    // and to ensure the first two elements are processed by the first loop
    // and the remainder by the end loop without saturations.
    const int nEntries = 76; // 0.02*76 + 0.5 ≈ 2.02 -> AtBegin = 2
    std::vector<cmsUInt16Number> Table16(nEntries, 0);

    // All zeros; this should remain all zeros after processing
    SlopeLimiting(Table16.data(), nEntries);

    int failures = 0;
    for (int i = 0; i < nEntries; ++i) {
        if (Table16[i] != 0) {
            std::cerr << "[AtBeginTwo_AllZeros] Table16[" << i << "] = "
                      << Table16[i] << " (expected 0)\n";
            ++failures;
        }
    }

    if (failures > 0) {
        std::cerr << "[AtBeginTwo_AllZeros] Test FAILED with " << failures << " issue(s).\n";
        return false;
    }
    return true;
}

// Entry point for the test suite
int main() {
    std::cout << "Running SlopeLimiting test suite (C++11, no GTest):\n";

    bool all_ok = true;

    std::cout << "Test 1: Increasing table, last should saturate to 0xFFFF\n";
    if (!test_SlopeLimiting_Increasing_Branch_LastIsFFFF()) {
        all_ok = false;
    } else {
        std::cout << "Test 1 PASSED.\n";
    }

    std::cout << "Test 2: Decreasing table, first should saturate to 0xFFFF and last to 0\n";
    if (!test_SlopeLimiting_Decreasing_Branch_FirstElementFFFFLastZero()) {
        all_ok = false;
    } else {
        std::cout << "Test 2 PASSED.\n";
    }

    std::cout << "Test 3: AtBegin=2 path with all zeros to ensure loop execution\n";
    if (!test_SlopeLimiting_AtBeginTwo_AllZeros()) {
        all_ok = false;
    } else {
        std::cout << "Test 3 PASSED.\n";
    }

    if (all_ok) {
        std::cout << "ALL TESTS PASSED.\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED. See diagnostics above.\n";
        return 1;
    }
}