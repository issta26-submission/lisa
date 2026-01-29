/*
Step 1 (Conceptual): Candidate Keywords identified from the focal method testDoublePrinting:
- UNITY_EXCLUDE_FLOAT_PRINT
- UNITY_EXCLUDE_DOUBLE
- USING_OUTPUT_SPY
- UNITY_OUTPUT_CHAR
- startPutcharSpy / endPutcharSpy
- getBufferPutcharSpy
- TEST_IGNORE
- TEST_ASSERT_EQUAL_PRINT_FLOATING
- TEST_ASSERT... floating assertions (printing validation)
- TEST_PROTECT / TearDown semantics (Unity test lifecycle)
- TEST_IGNORE_MESSAGE / TEST_FAIL_MESSAGE (ignored paths)
- Floating point literal formats: "0.100469", "4.29497e+09", "9.23457e+300", etc.
- Print formatting for large/small doubles
- Guarded/ungarded branches via preprocessor

Step 2 (Plan): We will provide two small, focused test files to exercise the focal function under two configurations:
- Active path: REAL floating-point print assertions (requires spy path to be enabled).
- Ignore path: compile-time guard triggers TEST_IGNORE() for the focal function.

Step 3 (Domain knowledge): We rely on Unity C API from testunity.c, adapted for C++11 compilation. Each test is annotated with comments explaining which branch is exercised and what is verified.

Note: To compile and run these in your project, compile both testunity.c (the file containing testDoublePrinting) and the test harness translation units in the same project. Ensure the following compiler flags are consistently applied to both translation units:
- For the active-path test: -DUSING_OUTPUT_SPY (to enable the spy path) and do not define UNITY_EXCLUDE_FLOAT_PRINT or UNITY_EXCLUDE_DOUBLE.
- For the ignore-path test: -DUNITY_EXCLUDE_FLOAT_PRINT (or -DUNITY_EXCLUDE_DOUBLE) or do not enable USING_OUTPUT_SPY. The ignore branch will be taken.

Now the code files:

// File: test_double_printing_active.cpp
// Purpose: Run testDoublePrinting with output spying enabled to actually exercise
//          the floating-point printing assertions inside the focal method.

#include <unity.h>
#include <cstdio>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
// The focal method under test is defined in testunity.c.
// We declare it with C linkage to avoid C++ name mangling.
extern void testDoublePrinting(void);
#ifdef __cplusplus
}
#endif

// Unity requires setUp/tearDown hooks for per-test lifecycle (no-op here).
void setUp(void) {}
void tearDown(void) {}

// A simple wrapper test that simply triggers the focal method under the active path.
// The testDoublePrinting function itself contains multiple TEST_ASSERT_EQUAL_PRINT_FLOATING
// assertions which will verify exact textual representations of the provided doubles.
// The active path is enabled by compiling with USING_OUTPUT_SPY (see comments at top).
void testDoublePrinting_ActivePath(void)
{
    // Step: Execute the focal method. If any of the floating-point assertions fail,
    // Unity will record a test failure. If all pass, this test passes.
    testDoublePrinting();
    // Comments:
    // - This path exercises: 0.100469, 4294967295.999999, 4294967296.0, 7e+100, 3e+200, etc.
    // - Negative variants are included in the focal method and will be checked by Unity.
    // - The preprocessor guards in the focal method ensure this branch only executes
    //   when UNITY_EXCLUDE_FLOAT_PRINT is not defined, UNITY_EXCLUDE_DOUBLE is not defined,
    //   and USING_OUTPUT_SPY is defined.
}

int main(void)
{
    UNITY_BEGIN();
    // Run the active-path test. Line number is a placeholder (1) for simplicity.
    RUN_TEST(testDoublePrinting_ActivePath, 1);
    return UNITY_END();
}


// File: test_double_printing_ignored.cpp
// Purpose: Validate that the focal method correctly falls back to TEST_IGNORE()
// when the float print pathway is excluded (IGNORE path).
// This tests the static-preprocessor-guarded branch inside the focal method.


#ifdef __cplusplus
extern "C" {
#endif
extern void testDoublePrinting(void);
#ifdef __cplusplus
}
#endif

void setUp(void) {}
void tearDown(void) {}

void testDoublePrinting_IgnoredPath(void)
{
    // This call should result in TEST_IGNORE() being invoked inside testDoublePrinting
    // due to UNITY_EXCLUDE_FLOAT_PRINT being defined (or UNITY_EXCLUDE_DOUBLE OR !USING_OUTPUT_SPY).
    // The test itself does not assert at runtime; Unity records an ignored test.
    testDoublePrinting();
}

int main(void)
{
    UNITY_BEGIN();
    // Run the ignored-path test. Line number is a placeholder (1) for simplicity.
    RUN_TEST(testDoublePrinting_IgnoredPath, 1);
    return UNITY_END();
}