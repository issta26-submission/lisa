/*
  Lightweight C++11 test harness focusing on the focal Unity-style test:
  testUINT8sNotWithinDeltaAndCustomMessage located in testunity.c

  This file provides a minimal, self-contained test suite that exercises
  the core behavior behind UINT8 within-delta checks, including the
  custom message variant. It does not depend on GTest and uses a tiny
 , non-terminating assertion mechanism suitable for integration into
  environments that compile with C++11 but do not pull in the Unity
  framework.

  Key ideas captured (Candidate Keywords):
  - UINT8 within-delta check
  - Delta = 2, expected = 255, actual = 0 (not within delta)
  - Custom message "Custom Message."
  - Macro-like function TEST_ASSERT_UINT8_WITHIN and TEST_ASSERT_UINT8_WITHIN_MESSAGE
  - Static-like storage for last failure message (LastFailureMessage)
  - True and false branches coverage for within/not-within paths
  - Non-GTest, main-driven test invocation
*/

#include <cmath>
#include <cstdint>
#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (annotated for understanding what influences tests):
// - UINT8 within delta logic
// - Delta 2
// - Values 255 and 0 (boundary-like scenario for uint8)
// - Custom Message string
// - TEST_ASSERT_UINT8_WITHIN_MESSAGE behavior on failure

// Minimal, self-contained failure message capture (mimics Unity's messaging)
static std::string LastFailureMessage;

// Basic absolute difference for signed/unsigned operands (uint8_t safe-promotions)
static inline int absDiffUInt8(uint8_t a, uint8_t b)
{
    // Promote to int to avoid underflow/unsigned wrap
    int ia = static_cast<int>(a);
    int ib = static_cast<int>(b);
    return std::abs(ia - ib);
}

// Core assertion: within-delta for uint8_t without message
static bool TEST_ASSERT_UINT8_WITHIN(int delta, uint8_t expected, uint8_t actual)
{
    int diff = absDiffUInt8(expected, actual);
    return diff <= delta;
}

// Core assertion: within-delta for uint8_t with message (simulates Unity's *_WITHIN_MESSAGE)
static bool TEST_ASSERT_UINT8_WITHIN_MESSAGE(int delta, uint8_t expected, uint8_t actual, const char* message)
{
    int diff = absDiffUInt8(expected, actual);
    if (diff <= delta)
    {
        return true;
    }
    // On failure, record the provided message to simulate Unity's message output
    LastFailureMessage = message ? message : "";
    return false;
}

/*
  Test 1: testUINT8sWithinDelta
  Purpose: Verify true branches for the UINT8 within-delta checks using multiple
  representative samples. Ensures no false-positives occur when values are within the delta.
  Expected: All internal checks pass (return true).
*/
static bool testUINT8sWithinDelta(void)
{
    bool a = TEST_ASSERT_UINT8_WITHIN(1, 254, 255); // diff = 1, within
    bool b = TEST_ASSERT_UINT8_WITHIN(5, 251, 255); // diff = 4, within
    bool c = TEST_ASSERT_UINT8_WITHIN(5, 1,   4); // diff = 3, within
    return a && b && c;
}

/*
  Test 2: testUINT8sNotWithinDelta
  Purpose: The focal negative path where the difference exceeds the allowed delta.
  In Unity, this would abort. Here we treat it as expected failure and return success
  for the test if the assertion correctly reports not within delta.
  Expected: The assertion returns false, so the test (negated) returns true.
*/
static bool testUINT8sNotWithinDelta(void)
{
    // delta=2, expected=255, actual=0 -> diff = 255, not within
    return !TEST_ASSERT_UINT8_WITHIN(2, 255, 0);
}

/*
  Test 3: testUINT8sNotWithinDeltaAndCustomMessage
  Purpose: Validate the NOT-within-delta path with a custom message is recorded.
  We simulate "EXPECT_ABORT_BEGIN" by expecting the assertion to fail and then
  verify LastFailureMessage matches the supplied string.
  Expected: The assertion fails and the captured message equals "Custom Message."
*/
static bool testUINT8sNotWithinDeltaAndCustomMessage(void)
{
    LastFailureMessage.clear();
    bool ok = TEST_ASSERT_UINT8_WITHIN_MESSAGE(2, 255, 0, "Custom Message.");
    // The assertion is expected to fail; so 'ok' should be false
    if (ok)
        return false;
    // Verify that the message was captured as expected
    return LastFailureMessage == std::string("Custom Message.");
}

/*
  Test 4: Additional coverage variant
  Purpose: Ensure the within-with-message path does not erroneously modify the
  LastFailureMessage when a check passes.
*/
static bool testUINT8sWithinDeltaWithMessageNoFailure(void)
{
    LastFailureMessage.clear();
    bool ok = TEST_ASSERT_UINT8_WITHIN_MESSAGE(5, 254, 255, "Should not appear");
    // Expect pass; no failure message should be recorded
    return ok && LastFailureMessage.empty();
}

/*
  Test runner setup
  - Collect tests and report PASS/FAIL per test
  - Execute tests from main (as directed by domain knowledge)
*/
int main()
{
    using TestFunc = std::function<bool(void)>;
    struct TestEntry {
        const char* name;
        TestFunc func;
        const char* comment;
    };

    std::vector<TestEntry> tests = {
        {"testUINT8sWithinDelta", [](){ return testUINT8sWithinDelta(); }, "Verifies true branches for within-delta UINT8 checks."},
        {"testUINT8sNotWithinDelta", [](){ return testUINT8sNotWithinDelta(); }, "Verifies false branch when UINT8 is not within delta."},
        {"testUINT8sNotWithinDeltaAndCustomMessage", [](){ return testUINT8sNotWithinDeltaAndCustomMessage(); }, "Ensures custom message is recorded on failure."},
        {"testUINT8sWithinDeltaWithMessageNoFailure", [](){ return testUINT8sWithinDeltaWithMessageNoFailure(); }, "Ensures no message on successful within-delta with message."}
    };

    int total = static_cast<int>(tests.size());
    int passed = 0;

    std::cout << "Running focal method test suite for: testUINT8sNotWithinDeltaAndCustomMessage equivalent\n";

    for (const auto& t : tests)
    {
        bool result = t.func();
        std::cout << "[ " << (result ? "PASS" : "FAIL") << " ] " << t.name
                  << " - " << t.comment << "\n";
        if (result) ++passed;
    }

    std::cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}

/*
  Notes for maintainers:
  - This test suite intentionally avoids terminating on assertion failures to
    maximize code coverage and flow (as per domain guidance about non-terminating
    assertions).
  - It mimics the essential semantics of UNITY's UINT8 within-delta checks and
    the associated Custom Message variant used by the focal method.
  - If you later integrate with a fuller test framework, you can replace the
    TEST_ASSERT_* functions with calls to the framework's assertion macros without
    changing the test-case structure.
*/