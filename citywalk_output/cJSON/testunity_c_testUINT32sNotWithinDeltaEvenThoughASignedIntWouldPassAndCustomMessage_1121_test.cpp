// A focused Unity-based test suite in C++11 for the focal testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassAndCustomMessage
// This suite imports the focal test functions from testunity.c (externed) and runs them under a separate, independent harness.
// Each test is annotated with comments describing its intent and coverage rationale.

#include <unity.h>
#include <cstdio>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

// Prototypes for focal tests that exist in testunity.c
// These are external tests provided by the repository and will be linked in at build time.
void testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassAndCustomMessage(void);
void testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirst(void);
void testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirstAndCustomMessage(void);
void testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassBigFirst(void);
void testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassBigFirstAndCustomMessage(void);
void testUINT32sWithinDelta(void);
#ifdef __cplusplus
}
#endif

void setUp(void) { /* Optional per-test setup */ }
void tearDown(void) { /* Optional per-test teardown */ }

int main(void)
{
    // Initialize Unity test framework
    UnityBegin("test_focal_suite.cpp");

    // Test 1: Focal case with custom message: Ensure the within-delta check fails when unsigned type would fail
    // and that a custom message is included in the failure output.
    // This directly exercises the exact scenario described by the focal test.
    // Expected: Test should abort and be reported as failed, with the custom message in the output.
    RUN_TEST(testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassAndCustomMessage);
    
    // Test 2: Similar focal scenario but with the signed value placed first (small first scenario).
    // Validates that the "small first" ordering does not make the unsigned comparison pass.
    RUN_TEST(testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirst);
    
    // Test 3: Same focal scenario as Test 1 but with the "small first" variant and a custom message.
    // Verifies the custom message path is taken in the alternate code path.
    RUN_TEST(testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassSmallFirstAndCustomMessage);
    
    // Test 4: Focal scenario with the "big first" ordering of operands (signed value first).
    // Ensures that the path where the signed value is considered in a different order still aborts appropriately.
    RUN_TEST(testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassBigFirst);
    
    // Test 5: Corresponding "big first" variant with a custom message to exercise the message-path in this ordering.
    RUN_TEST(testUINT32sNotWithinDeltaEvenThoughASignedIntWouldPassBigFirstAndCustomMessage);
    
    // Test 6: A baseline within-delta case that should pass (non-aborting) to confirm normal operation works.
    RUN_TEST(testUINT32sWithinDelta);

    // Finalize Unity and return exit code
    return UnityEnd();
}