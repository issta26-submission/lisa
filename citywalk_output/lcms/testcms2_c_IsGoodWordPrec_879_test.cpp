// Unit test suite for the focal method: IsGoodWordPrec
// This test targets the function:
//     cmsBool IsGoodWordPrec(const char *title, cmsUInt16Number in, cmsUInt16Number out, cmsUInt16Number maxErr)
// Behavior: returns TRUE if abs(in - out) <= maxErr, otherwise calls Fail(...) and returns FALSE.
// Notes:
// - The test suite is written in C++11 and does not rely on any external testing framework.
// - It assumes testcms2.h provides the necessary declarations and the C function IsGoodWordPrec is linkable.
// - We use a lightweight custom test harness to perform non-terminating assertions and report results.
// - We intentionally cover true and false branches, including edge cases with unsigned 16-bit values.

#include <vector>
#include <iostream>
#include <cstdint>
#include <testcms2.h>


extern "C" {
}

// Lightweight test harness data structure
struct TestCase {
    const char* title;
    cmsUInt16Number in;
    cmsUInt16Number out;
    cmsUInt16Number maxErr;
    bool expected; // true => IsGoodWordPrec should return TRUE, false => should return FALSE
};

// Helper to print a 16-bit value in hex for diagnostics
static void print16Hex(std::ostream& os, cmsUInt16Number v) {
    os << "0x" << std::hex << static_cast<uint32_t>(v) << std::dec;
}

// Main function implementing the test suite
int main() {
    // Candidate Keywords extracted from the focal method and its dependencies:
    // - abs, in, out, maxErr, Fail, cmsBool, TRUE, FALSE
    // - Title string usage, numeric precision
    //
    // The tests focus on: true branch (abs(in - out) <= maxErr) and false branch
    // (abs(in - out) > maxErr) of the condition.
    
    // Step 2: Define test cases to cover true/false branches and edge conditions
    std::vector<TestCase> tests = {
        // Test 1: Exact match, diff = 0, maxErr = 0 => TRUE
        { "TestExactMatch", 0x1234, 0x1234, 0, true },
        // Test 2: diff == maxErr => TRUE
        { "TestWithinMax", 0x1000, 0x1005, 0x0005, true },
        // Test 3: diff > maxErr => FALSE
        { "TestExceedMax", 0x1234, 0x1280, 0x000F, false },
        // Test 4: Large unsigned difference, within maxErr boundary edge
        // in > out with large diff; maxErr = 32767, diff = 32768 => FALSE
        { "TestUnsignedOverflow", 0x8000, 0x0000, 0x7FFF, false },
        // Test 5: Edge boundary with very small diff <= maxErr => TRUE
        { "TestEdgeBoundary", 0xFFFF, 0xFFFE, 1, true }
    };

    int total = static_cast<int>(tests.size());
    int passed = 0;

    // Step 3: Execute tests
    for (const auto& t : tests) {
        // The focal function is implemented in C; call with C linkage
        cmsBool r = IsGoodWordPrec(t.title, t.in, t.out, t.maxErr);
        bool actual = (r != FALSE);

        // Diagnostic output for each test case
        std::cout << "[TEST] " << t.title
                  << " | in=";
        print16Hex(std::cout, t.in);
        std::cout << " | out=";
        print16Hex(std::cout, t.out);
        std::cout << " | maxErr=";
        print16Hex(std::cout, t.maxErr);
        std::cout << " | expected=" << (t.expected ? "TRUE" : "FALSE")
                  << " | actual=" << (actual ? "TRUE" : "FALSE");

        if (actual == t.expected) {
            std::cout << " -> PASS" << std::endl;
            ++passed;
        } else {
            std::cout << " -> FAIL" << std::endl;
        }
    }

    // Final summary
    std::cout << "\nTest results: " << passed << " / " << total << " passed."
              << std::endl;

    // Non-terminating style: return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}