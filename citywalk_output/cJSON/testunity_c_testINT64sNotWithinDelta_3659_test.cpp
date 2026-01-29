// This file provides a lightweight, non-GTest C++11 test harness
// to exercise the focal behavior represented by the Unity test
// function testINT64sNotWithinDelta (within the provided Unity
// test file). It focuses on 64-bit delta checks for int64_t
// values and simulates the abort-on-failure semantics via a small
// exception-based mechanism.
//
// Notes:
// - The focal test in testunity.c uses Unity macros like TEST_ASSERT_INT64_WITHIN
//   with a delta of 1 against two near-max 64-bit values.
// - We implement a minimal, self-contained test suite that mirrors the
//   essential logic (within delta vs not within delta) without pulling in
//   the full Unity framework. This keeps the tests executable under C++11
//   without GTest, and still exercises true/false branches.
// - We respect UNITY_SUPPORT_64: if not defined, the 64-bit test is skipped.

#include <cstdint>
#include <unity.h>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


#ifdef UNITY_SUPPORT_64
// Candidate Keywords extracted from Step 1:
// - INT64 within delta
// - 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFC (64-bit boundary values)
// - delta = 1
// - abort-on-failure behavior (simulated via exceptions)
// - testINT64sNotWithinDelta (focal method)
// - within-delta path vs not-within-delta path (branch coverage)
// - 64-bit arithmetic safety (128-bit for diff if available)
#endif

// Portable, relatively safe 64-bit within-delta checker using 128-bit arithmetic
// to avoid overflow during difference calculation.
// Returns true if |a - b| <= delta.
static inline bool int64WithinDelta(int64_t delta, int64_t a, int64_t b)
{
#if defined(__SIZEOF_INT128__)  // GCC/Clang support for 128-bit
    __int128 da = ( __int128 ) a;
    __int128 db = ( __int128 ) b;
    __int128 diff = da - db;
    if (diff < 0) diff = -diff;
    __int128 d = ( __int128 ) delta;
    return diff <= d;
#else
    // Fallback path (best-effort): rely on unsigned arithmetic with careful ordering.
    // This path is provided for compilers that do not support 128-bit ints.
    // It avoids overflow by ordering checks to keep subtraction within range.
    // Note: This fallback is only used if 128-bit is unavailable; tests focus on
    // boundary values that are safe in this path.
    uint64_t ua = static_cast<uint64_t>(a);
    uint64_t ub = static_cast<uint64_t>(b);
    uint64_t diff = (ua > ub) ? (ua - ub) : (ub - ua);
    uint64_t ud = static_cast<uint64_t>(delta);
    return diff <= ud;
#endif
}

// Custom exception type to simulate a deliberate abort in tests
class TestAbort : public std::exception
{
public:
    const char* what() const noexcept override { return "TestAbort: abort simulated for assertion failure"; }
};

// FOCAL TESTS
// The following tests emulate the behavior of testINT64sNotWithinDelta in testunity.c
// - test_INT64sNotWithinDelta_Abort: expects an abort (i.e., not within delta) and passes if an abort occurs.
// - test_INT64sWithinDelta_NoAbort: uses values within delta and passes without abort.
// - test_INT64sWithinDelta_BoundaryEdgeCase: edge case using INT64_MIN and INT64_MIN+1 to ensure within-delta path is exercised.

#ifdef UNITY_SUPPORT_64

// Test 1: Not within delta should abort (simulate with throwing TestAbort).
// This corresponds to: TEST_ASSERT_INT64_WITHIN(1, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFC);
void test_INT64sNotWithinDelta_Abort()
{
    // true/false branches:
    // within == false for the provided large numbers with delta=1
    bool within = int64WithinDelta(1, (int64_t)0x7FFFFFFFFFFFFFFF, (int64_t)0x7FFFFFFFFFFFFFFC);

    if (!within)
    {
        // Simulate abort that Unity would trigger on a failed assertion
        throw TestAbort();
    }
    else
    {
        // If somehow within (unexpected for this test), fail the test explicitly
        throw std::runtime_error("Abort not triggered as expected for INT64 within-delta false case");
    }
}

// Test 2: Within delta should NOT abort (path where assertion would pass without abort).
// This corresponds to a true branch where no abort is expected.
void test_INT64sWithinDelta_NoAbort()
{
    // Choose values with difference <= delta
    bool within = int64WithinDelta(1, (int64_t)0x7FFFFFFFFFFFFFFF - 1, (int64_t)0x7FFFFFFFFFFFFFFF);

    if (!within)
    {
        // If within returns false, the test should fail (no abort expected)
        throw std::runtime_error("Expected within-delta to be true, but it was false");
    }
    // If within is true, test passes (no abort)
}

// Test 3: Boundary edge case for within delta using INT64_MIN and INT64_MIN+1
// Ensures edge-case arithmetic does not overflow and the within-delta check works.
void test_INT64sWithinDelta_BoundaryEdgeCase()
{
    bool within = int64WithinDelta(1, std::numeric_limits<int64_t>::min(),
                                   std::numeric_limits<int64_t>::min() + 1);
    if (!within)
    {
        throw std::runtime_error("Boundary edge case within-delta check failed");
    }
    // If within is true, test passes
}

// Public entry points that the test runner will call
void run_focal_int64_within_delta_tests()
{
    // Run all focal tests; exceptions indicate aborts or failures.
    try
    {
        test_INT64sNotWithinDelta_Abort();
    }
    catch (const TestAbort&)
    {
        std::cout << "[PASS] test_INT64sNotWithinDelta_Abort (abort occurred as expected)" << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << "[FAIL] test_INT64sNotWithinDelta_Abort - " << ex.what() << std::endl;
    }

    try
    {
        test_INT64sWithinDelta_NoAbort();
        std::cout << "[PASS] test_INT64sWithinDelta_NoAbort" << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << "[FAIL] test_INT64sWithinDelta_NoAbort - " << ex.what() << std::endl;
    }

    try
    {
        test_INT64sWithinDelta_BoundaryEdgeCase();
        std::cout << "[PASS] test_INT64sWithinDelta_BoundaryEdgeCase" << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cout << "[FAIL] test_INT64sWithinDelta_BoundaryEdgeCase - " << ex.what() << std::endl;
    }
}

#else // UNITY_SUPPORT_64 not defined
// If 64-bit support is not compiled in, skip the 64-bit focal tests gracefully.
void run_focal_int64_within_delta_tests()
{
    std::cout << "[SKIP] test_INT64sNotWithinDelta_Abort and related 64-bit tests are skipped (UNITY_SUPPORT_64 not defined)" << std::endl;
}
#endif // UNITY_SUPPORT_64

int main()
{
    // Explain the test suite scope
    std::cout << "=== Focal Test Suite: INT64sNotWithinDelta and related 64-bit delta checks ===" << std::endl;

    // Run focal tests; the function name mirrors the focal method in testunity.c.
    run_focal_int64_within_delta_tests();

    std::cout << "=== End of Focal Test Suite ===" << std::endl;
    return 0;
}