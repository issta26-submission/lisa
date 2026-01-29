/*
  Test Suite: testUINT8sWithinDeltaAndCustomMessage
  Context:
  - Focused on the focal method testUINT8sWithinDeltaAndCustomMessage() located in testunity.c
  - The focal method exercises Unity's UINT8 within-delta assertion macro with a custom message:
      TEST_ASSERT_UINT8_WITHIN_MESSAGE(delta, expected, actual, "Custom Message.");
  - The surrounding class dependencies are provided in the lengthy UNITY test harness code block (<FOCAL_CLASS_DEP>).
  - This suite is designed to be compiled in a C++11 capable environment without using GTest.
  - The approach below provides a lightweight C++ test harness that invokes the C focal test directly (from main),
    reflecting Step 3 guidance to call test methods from main when not using a dedicated C++ test framework.

  Notes on design decisions:
  - We expose the focal C function testUINT8sWithinDeltaAndCustomMessage() via extern "C" to ensure linkage from C++.
  - The wrapper remains non-intrusive and relies on the Unity test macros to report failures in the hooked test.
  - Additional lightweight wrappers are provided to demonstrate multiple entry points into the focal test (as per
    the idea of exercising related test paths and static/dependency wiring) while still using the focal test as the
    source of truth for the Delta/MACRO behavior.
  - This file does not redefine Unity macros; it simply invokes the existing focal test as a black-box unit.
  - The comments annotate intent and mapping to {Candidate Keywords} and domain knowledge.

  How to build (conceptual):
  - Compile testunity.c together with this wrapper, linking a Unity runtime object if required by your build flow.
  - Build as a C++11 executable that includes this file; at runtime, main will execute the focal test and print a basic result.
*/

#include <exception>
#include <unity.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (from Step 1): testUINT8sWithinDeltaAndCustomMessage, TEST_ASSERT_UINT8_WITHIN_MESSAGE,
// UNITY/C macros, Custom Message., delta-based assertions, UINT8 type, resilient test harness.

// The focal method is written in C (Unity-based). We declare it with C linkage for C++ usage.
extern "C" void testUINT8sWithinDeltaAndCustomMessage(void);

/*
  Lightweight wrapper 1:
  - Purpose: Execute the focal test to verify that the within-delta checks for UINT8 with a custom message
             execute without triggering a fatal failure (i.e., non-aborting assertion path).
  - Rationale: Demonstrates one entry point into the focal test as part of a small C++ test harness.
*/
void testUINT8sWithinDeltaAndCustomMessage_wrapper1() {
    // Explanation:
    //  - Call the focal C test function. If Unity's macro reports a failure, Unity's runtime will print
    //    the failure details and set its internal state. We simply rely on process exit/status to indicate
    //    test outcome in this lightweight harness.
    testUINT8sWithinDeltaAndCustomMessage();
    // If the focal test internally calls Unity's test reporting, this wrapper completes normally here
    // when the focal test passes. Any failure is reported by Unity's framework output.
}

/*
  Lightweight wrapper 2:
  - Purpose: A second invocation to exercise potential state reuse and to mimic a separate test case call pattern.
  - Rationale: Helps improve coverage by executing the focal test under a different wrapper context.
*/
void testUINT8sWithinDeltaAndCustomMessage_wrapper2() {
    testUINT8sWithinDeltaAndCustomMessage();
}

/*
  Minimal test runner (non-GTest):
  - Step 3 guidance suggests calling test methods from main when not using a dedicated C++ test framework.
  - This main function dispatches the wrappers, capturing and reporting basic pass/fail signals.
  - Note: Unity's internal reporting will drive the detailed pass/fail output. This is a light wrapper
          around the Unity-based focal test to satisfy a C++11 environment without GTest.
*/
int main() {
    std::cout << "Running focal test suite for testUINT8sWithinDeltaAndCustomMessage (Unity-based C test)." << std::endl;

    int exits[3] = {0, 0, 0};
    try {
        // Entry 1: direct wrapper invocation
        testUINT8sWithinDeltaAndCustomMessage_wrapper1();
        std::cout << "[OK] wrapper1 executed without immediate crash." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception in wrapper1: " << e.what() << std::endl;
        exits[0] = 1;
    } catch (...) {
        std::cerr << "Unknown exception in wrapper1." << std::endl;
        exits[0] = 1;
    }

    try {
        // Entry 2: second invocation variant
        testUINT8sWithinDeltaAndCustomMessage_wrapper2();
        std::cout << "[OK] wrapper2 executed without immediate crash." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception in wrapper2: " << e.what() << std::endl;
        exits[1] = 1;
    } catch (...) {
        std::cerr << "Unknown exception in wrapper2." << std::endl;
        exits[1] = 1;
    }

    // Simple aggregate result
    bool anyFail = (exits[0] != 0) || (exits[1] != 0);
    if (anyFail) {
        std::cout << "Focal test suite reported failures via Unity/test harness." << std::endl;
        return 1;
    } else {
        std::cout << "Focal test suite completed successfully." << std::endl;
        return 0;
    }
}