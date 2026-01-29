/* File: unity_testable.cpp
   Purpose: Create a testable build of UnityFail by redefining the bail macro
   to non-terminating behavior and capturing printed output for verification
   in a separate C++ test harness (test_unityfail.cpp).
   The Unity library is included in a way that UnityPrint calls are redirected
   to a capture buffer, so we can assert on the produced output without
   terminating the test process. */

/* 1) Redefine bail and related macros to avoid exiting the test harness. */
#define UNITY_FAIL_AND_BAIL return
#define RETURN_IF_FAIL_OR_IGNORE

/* 2) Capture buffer for UnityPrint outputs invoked by UnityFail. */
static char UnityOutputBuffer[4096];
static unsigned int UnityOutputLen = 0;

/* Append a single character to the capture buffer. */
static void UnityAppendChar(char c) {
    if (UnityOutputLen < sizeof(UnityOutputBuffer) - 1) {
        UnityOutputBuffer[UnityOutputLen++] = c;
        UnityOutputBuffer[UnityOutputLen] = '\0';
    }
}

/* Append a C-string to the capture buffer. */
static void UnityAppendString(const char* s) {
    while (*s) {
        UnityAppendChar(*s++);
    }
}

/* Redirect UnityPrint to capture output instead of writing to stdout. */
#define UNITY_OUTPUT_CHAR(c) UnityAppendChar((char)(c))
#define UnityPrint(s) UnityAppendString((s))

/* Expose lightweight C-callable APIs for tests to inspect captured output. */
extern "C" void UnityResetCapture(void);
extern "C" const char* UnityCapturedOutput(void);

/* Reset the capture buffer before each test. */
extern "C" void UnityResetCapture(void) {
    UnityOutputLen = 0;
    UnityOutputBuffer[0] = '\0';
}

/* Retrieve the current captured output buffer. */
extern "C" const char* UnityCapturedOutput(void) {
    return UnityOutputBuffer;
}

/* 3) Include the Unity library with the above macro overrides and capture hooks.
   We include as C to ensure proper linkage, while this translation unit is compiled as C++. */
#include <stddef.h>
#include <unity.h>
#include <cstring>
#include <unity.c>
#include <iostream>
#include <string>
#include <stdio.h>


extern "C" {
}

/* The following static assertion ensures this translation unit
   provides the capture hooks to the test harness. No runtime side-effects. */
static void unity_testable_anchor(void) { (void)UnityResetCapture; (void)UnityCapturedOutput; }

/* End of unity_testable.cpp */


/* File: test_unityfail.cpp
   Purpose: A minimal, non-GTest C++ test harness that exercises UnityFail
   (from unity.c) by passing different msg values and inspecting the captured
   output buffer. The bail macro has been overridden to return, so tests can
   continue running after each call.

   Tests:
   - Null message: ensure no colon is printed (since msg == NULL).
   - Message without leading space: ensure colon and message appear.
   - Message with leading space: ensure colon and message (with leading space) appear.
*/



extern "C" const char* UnityCapturedOutput(void);
extern "C" void UnityResetCapture(void);
extern "C" void UnityFail(const char* msg, UNITY_LINE_TYPE line);

static bool contains(const char* haystack, const char* needle) {
    if (!haystack || !needle) return false;
    return std::strstr(haystack, needle) != nullptr;
}

// Test 1: msg == NULL -> output should not contain a colon separator
bool test_UnityFail_with_null_msg() {
    UnityResetCapture();
    UnityFail(nullptr, 1); // line number is arbitrary for test
    const char* out = UnityCapturedOutput();
    // Expect some output for UnityStrFail, but no ':' since msg is NULL
    bool hasColon = contains(out, ":"); // any colon would indicate a msg was printed
    bool nonEmpty = out != nullptr && std::strlen(out) > 0;
    if (nonEmpty && !hasColon) {
        return true;
    }
    return false;
}

// Test 2: msg without leading space -> colon should appear and message should appear
bool test_UnityFail_with_msg_no_leading_space() {
    UnityResetCapture();
    const char* msg = "ERROR_NO_LEADING_SPACE";
    UnityFail(msg, 2);
    const char* out = UnityCapturedOutput();
    bool hasColon = contains(out, ":");
    bool hasMsg = contains(out, msg);
    return hasColon && hasMsg;
}

// Test 3: msg with leading space -> colon should appear and the exact message (including leading space) should appear
bool test_UnityFail_with_msg_leading_space() {
    UnityResetCapture();
    const char* msg = " HELLO_WITH_LEADING_SPACE";
    UnityFail(msg, 3);
    const char* out = UnityCapturedOutput();
    bool hasColon = contains(out, ":");
    bool hasMsg = contains(out, msg); // includes the leading space
    return hasColon && hasMsg;
}

int main() {
    int failures = 0;

    // Test 1
    bool r1 = test_UnityFail_with_null_msg();
    std::cout << "Test UnityFail with NULL msg: " << (r1 ? "PASS" : "FAIL") << std::endl;
    if (!r1) ++failures;

    // Test 2
    bool r2 = test_UnityFail_with_msg_no_leading_space();
    std::cout << "Test UnityFail with message without leading space: " << (r2 ? "PASS" : "FAIL") << std::endl;
    if (!r2) ++failures;

    // Test 3
    bool r3 = test_UnityFail_with_msg_leading_space();
    std::cout << "Test UnityFail with message with leading space: " << (r3 ? "PASS" : "FAIL") << std::endl;
    if (!r3) ++failures;

    return failures;
}