/*
Unit Test Suite for DecideTextType (cmsTagTypeSignature DecideTextType(cmsFloat64Number ICCVersion, const void *Data))

 Purpose:
 - Validate the two branches of the decision logic:
     - ICCVersion >= 4.0 -> cmsSigMultiLocalizedUnicodeType
     - otherwise -> cmsSigTextType
 - Validate that the Data parameter is not used by the function (non-terminating coverage focus)
 - Ensure behavior for boundary and non-boundary values, and for non-null Data across branches

 Assumptions:
 - The project provides the Little CMS headers (e.g., lcms2.h) exposing:
     - cmsTagTypeSignature type
     - cmsSigMultiLocalizedUnicodeType and cmsSigTextType constants
     - cmsFloat64Number type (typically a typedef for double)
     - DecideTextType function with C linkage
 - This test is written in C++11 and uses a lightweight, non-GTest approach to assertions
 - Tests are designed to run in a normal C++ program (main function) and print results
 - Link against the actual cms library when compiling (e.g., -llcms2)

 Notes for maintainers:
 - The tests avoid private/static members access and rely only on the public DecideTextType API as requested.
 - All tests are designed to continue execution even if one test fails (non-terminating behavior).

 This file should be compiled together with the cms library sources (or linked against the library providing DecideTextType).
*/

#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include Little CMS headers to obtain the declarations for DecideTextType and related types.
// The exact path may vary in your environment; adjust include path accordingly.

// If the header does not declare DecideTextType with C linkage in C++, you may need
// to declare it explicitly. Most setups declare it with C linkage in the header.
// extern "C" cmsTagTypeSignature DecideTextType(cmsFloat64Number ICCVersion, const void *Data);

// Lightweight test helper to print a test result.
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

// Test 1: ICCVersion exactly 4.0 should return MultiLocalizedUnicodeType
static TestResult test_DecideTextType_ICC4_0() {
    TestResult tr;
    tr.name = "DecideTextType_ICC4_0_returns_MultiLocalizedUnicodeType";

    cmsFloat64Number ICCVersion = 4.0;
    const void* Data = nullptr;

    cmsTagTypeSignature result = DecideTextType(ICCVersion, Data);

    if (result == cmsSigMultiLocalizedUnicodeType) {
        tr.passed = true;
        tr.message = "PASS: ICCVersion=4.0 yields cmsSigMultiLocalizedUnicodeType as expected.";
    } else {
        tr.passed = false;
        tr.message = "FAIL: ICCVersion=4.0 expected cmsSigMultiLocalizedUnicodeType but got a different type.";
    }
    return tr;
}

// Test 2: ICCVersion greater than 4.0 should return MultiLocalizedUnicodeType
static TestResult test_DecideTextType_ICCAbove4() {
    TestResult tr;
    tr.name = "DecideTextType_ICCAbove4_returns_MultiLocalizedUnicodeType";

    cmsFloat64Number ICCVersion = 4.1; // greater than 4.0
    const void* Data = nullptr;

    cmsTagTypeSignature result = DecideTextType(ICCVersion, Data);

    if (result == cmsSigMultiLocalizedUnicodeType) {
        tr.passed = true;
        tr.message = "PASS: ICCVersion=4.1 yields cmsSigMultiLocalizedUnicodeType as expected.";
    } else {
        tr.passed = false;
        tr.message = "FAIL: ICCVersion=4.1 expected cmsSigMultiLocalizedUnicodeType but got a different type.";
    }
    return tr;
}

// Test 3: ICCVersion below 4.0 should return TextType
static TestResult test_DecideTextType_ICCBelow4() {
    TestResult tr;
    tr.name = "DecideTextType_ICCBelow4_returns_TextType";

    cmsFloat64Number ICCVersion = 3.999;
    const void* Data = nullptr;

    cmsTagTypeSignature result = DecideTextType(ICCVersion, Data);

    if (result == cmsSigTextType) {
        tr.passed = true;
        tr.message = "PASS: ICCVersion=3.999 yields cmsSigTextType as expected.";
    } else {
        tr.passed = false;
        tr.message = "FAIL: ICCVersion=3.999 expected cmsSigTextType but got a different type.";
    }
    return tr;
}

// Test 4: Data parameter should be ignored; verify both branches when Data is non-null
static TestResult test_DecideTextType_DataIsIgnored() {
    TestResult tr;
    tr.name = "DecideTextType_DataIsIgnored";

    // Branch 1: ICCVersion >= 4.0 with non-null Data
    cmsFloat64Number ICCVersion1 = 4.0;
    int nonNullPayload = 0;
    const void* Data1 = &nonNullPayload;
    cmsTagTypeSignature res1 = DecideTextType(ICCVersion1, Data1);
    bool pass1 = (res1 == cmsSigMultiLocalizedUnicodeType);

    // Branch 2: ICCVersion < 4.0 with non-null Data
    cmsFloat64Number ICCVersion2 = 3.0;
    const void* Data2 = &nonNullPayload;
    cmsTagTypeSignature res2 = DecideTextType(ICCVersion2, Data2);
    bool pass2 = (res2 == cmsSigTextType);

    if (pass1 && pass2) {
        tr.passed = true;
        tr.message = "PASS: Data parameter did not affect outcomes for both branches.";
    } else {
        tr.passed = false;
        tr.message = "FAIL: Data parameter affected DecideTextType result or branch detection.";
        if (!pass1) tr.message += " Branch1 failed; ";
        if (!pass2) tr.message += " Branch2 failed;";
    }
    return tr;
}

// Simple test harness
int main() {
    std::vector<TestResult> results;

    results.push_back(test_DecideTextType_ICC4_0());
    results.push_back(test_DecideTextType_ICCAbove4());
    results.push_back(test_DecideTextType_ICCBelow4());
    results.push_back(test_DecideTextType_DataIsIgnored());

    int passed = 0;
    int failed = 0;

    std::cout << "DecideTextType Test Suite (C++11) - Status:\n";
    for (const auto& r : results) {
        if (r.passed) {
            ++passed;
        } else {
            ++failed;
        }
        std::cout << "[" << (r.passed ? "OK" : "FAIL") << "] " << r.name << " - " << r.message << "\n";
    }
    std::cout << "Summary: " << passed << " passed, " << failed << " failed, out of " << results.size() << " tests.\n";

    // Return non-zero if any test failed to indicate overall failure to a test harness.
    return (failed == 0) ? 0 : 1;
}