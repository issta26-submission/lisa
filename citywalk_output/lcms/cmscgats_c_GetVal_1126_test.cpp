/*
 * Unit Test Suite for cmsBool GetVal(cmsIT8* it8, char* Buffer, cmsUInt32Number max, const char* ErrorTitle)
 * 
 * Notes:
 * - This test suite is designed to be used with the project's existing cmscgats.c implementation.
 * - It relies on the project-provided public/internal interfaces (lcms2_internal.h and associated string helpers)
 *   to construct cmsIT8 objects and associated string wrappers (string*, StringAlloc/StringCat/StringPtr, etc.).
 * - No GoogleTest is used. A small, non-terminating test harness is implemented to allow multiple tests to run
 *   in a single invocation and report pass/fail for each case.
 * - Each test case includes comments explaining the scenario it covers.
 * - The tests aim to exercise all switch branches (SEOLN, SIDENT, SINUM, SDNUM, SSTRING) and the default error path.
 * - It is assumed that cmsBool, TRUE, FALSE, and the required SYMBOL enumerations (SEOLN, SIDENT, SINUM, SDNUM, SSTRING)
 *   are defined by the included cms headers (e.g., lcms2_internal.h) used in cmscgats.c.
 *
 * Build note:
 * - Compile this test file together with cmscgats.c and link against the library containing cmscgats.c.
 * - Ensure the include path provides lcms2_internal.h (or the correct header that defines cmsIT8, cmsBool, etc.).
 */

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cstring>


// Include the library headers that define cmsIT8, cmsBool, SYMBOLs, and related helpers.
// The actual project header path may vary; adjust as needed for your environment.

// Declare the function under test with C linkage (as it is implemented in cmscgats.c)
extern "C" cmsBool GetVal(cmsIT8* it8, char* Buffer, cmsUInt32Number max, const char* ErrorTitle);

// Helpers for building test strings using the project's string wrapper (string* and related APIs)
namespace TestHelpers {

// Helper to create a string wrapper and populate it with content.
// Uses the project-provided StringAlloc/StringClear/StringCat/StringPtr interfaces.
string* MakeTestString(const char* content) {
    if (content == nullptr) return nullptr;
    // Allocate a string wrapper with enough capacity for content plus null terminator.
    // The max argument semantics are assumed to indicate capacity; we pass strlen(content) + 1.
    string* s = StringAlloc(nullptr, static_cast<int>(strlen(content) + 1));
    if (s == nullptr) {
        std::cerr << "Failed to allocate test string wrapper.\n";
        return nullptr;
    }
    StringClear(s);
    // Append content to the string wrapper
    if (!StringCat(s, content)) {
        std::cerr << "Failed to populate test string wrapper.\n";
        return nullptr;
    }
    return s;
}

// Helper to release resources if the library requires explicit cleanup.
// If the project provides a destructor or a free function for string*, call it here.
// The original CMS library might manage memory differently; for safety, we try to clear.
void FreeTestString(string* s) {
    if (s != nullptr) {
        StringClear(s);
        // If there is a dedicated free function, it should be called here.
        // e.g., cmsFree(s); or similar, depending on the project's API.
    }
}

} // namespace TestHelpers

// Simple non-terminating test harness
struct TestRunner {
    int total{0};
    int failed{0};

    void report(bool pass, const std::string& message) {
        ++total;
        if (pass) {
            std::cout << "[PASS] " << message << "\n";
        } else {
            ++failed;
            std::cout << "[FAIL] " << message << "\n";
        }
    }

    int summary() const {
        std::cout << "\nTest Summary: " << (total - failed) << "/" << total << " passed, "
                  << failed << " failed.\n";
        return failed;
    }
};

// Test 1: SEOLN (Empty value)
bool test_GetVal_SEOLN() {
    cmsIT8 it8;
    it8.sy = SEOLN; // Empty value
    char Buffer[256 + 1] = {0};
    cmsBool rv = GetVal(&it8, Buffer, 256, "Error: SEOLN");
    bool pass = (Buffer[0] == 0) && (rv == TRUE);
    return pass;
}

// Test 2: SIDENT (Identifier string)
bool test_GetVal_SIDENT() {
    cmsIT8 it8;
    it8.sy = SIDENT;

    // Build a test string "IdentName" and assign to it8.id
    string* ident = TestHelpers::MakeTestString("IdentName");
    if (ident == nullptr) return false;
    it8.id = ident; // assume type string*

    char Buffer[256 + 1] = {0};
    cmsBool rv = GetVal(&it8, Buffer, 256, "Error: SIDENT");
    bool pass = (rv == TRUE) && (std::strcmp(Buffer, "IdentName") == 0);
    TestHelpers::FreeTestString(ident);
    return pass;
}

// Test 3: SINUM (Integer number)
bool test_GetVal_SINUM() {
    cmsIT8 it8;
    it8.sy = SINUM;
    it8.inum = 12345;

    char Buffer[256 + 1] = {0};
    cmsBool rv = GetVal(&it8, Buffer, 256, "Error: SINUM");
    bool pass = (rv == TRUE) && (std::strcmp(Buffer, "12345") == 0);
    return pass;
}

// Test 4: SDNUM (Double number) with a provided formatter
bool test_GetVal_SDNUM() {
    cmsIT8 it8;
    it8.sy = SDNUM;
    it8.dnum = 3.14;
    // Provide a formatter that rounds to 2 decimals
    it8.DoubleFormatter = "%0.2f";

    char Buffer[256 + 1] = {0};
    cmsBool rv = GetVal(&it8, Buffer, 256, "Error: SDNUM");
    bool pass = (rv == TRUE) && (std::strcmp(Buffer, "3.14") == 0);
    return pass;
}

// Test 5: SSTRING (String stored in it8->str)
bool test_GetVal_SSTRING() {
    cmsIT8 it8;
    it8.sy = SSTRING;

    // Build test string for it8->str
    string* str = TestHelpers::MakeTestString("HelloCMS");
    if (str == nullptr) return false;
    it8.str = str;

    char Buffer[256 + 1] = {0};
    cmsBool rv = GetVal(&it8, Buffer, 256, "Error: SSTRING");
    bool pass = (rv == TRUE) && (std::strcmp(Buffer, "HelloCMS") == 0);
    TestHelpers::FreeTestString(str);
    return pass;
}

// Test 6: Default/invalid symbol should trigger SynError and return FALSE
bool test_GetVal_DefaultError() {
    cmsIT8 it8;
    // Choose an invalid/sentinel value for sy to hit the default case.
    // We cast a small integer to the symbol type if necessary.
    it8.sy = (SYMBOL)9999;

    char Buffer[256 + 1] = {0};
    cmsBool rv = GetVal(&it8, Buffer, 256, "Error: Default");
    // SynError is typically designed to return FALSE on error; we expect FALSE here.
    bool pass = (rv == FALSE);
    return pass;
}

int main() {
    TestRunner runner;

    // Run individual tests with explanatory comments
    // Test 1: SEOLN - Empty value should produce an empty string
    runner.report(test_GetVal_SEOLN(), "GetVal handles SEOLN by returning empty Buffer and TRUE.");

    // Test 2: SIDENT - Identifier string should be copied correctly into Buffer
    runner.report(test_GetVal_SIDENT(), "GetVal copies id string into Buffer for SIDENT.");

    // Test 3: SINUM - Integer numeric value should be formatted as a string
    runner.report(test_GetVal_SINUM(), "GetVal formats inum as decimal string for SINUM.");

    // Test 4: SDNUM - Floating number should be formatted according to DoubleFormatter
    runner.report(test_GetVal_SDNUM(), "GetVal formats dnum using DoubleFormatter for SDNUM.");

    // Test 5: SSTRING - String value should be copied into Buffer
    runner.report(test_GetVal_SSTRING(), "GetVal copies string content for SSTRING.");

    // Test 6: Default branch - ensure error path is taken for unknown symbol
    runner.report(test_GetVal_DefaultError(), "GetVal returns FALSE and reports error for unknown symbol.");

    // Summary
    return runner.summary();
}