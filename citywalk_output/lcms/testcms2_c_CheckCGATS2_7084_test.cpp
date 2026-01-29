/*
Unit test suite for the focal method: CheckCGATS2

Step 1 - Candidate Keywords derived from the focal method:
- CheckCGATS2
- cmsIT8LoadFromMem
- cmsIT8Free
- cmsHANDLE
- cmsUInt8Number
- junk (memory buffer)
- memory-based IT8 loading

Notes:
- The tests are written in C++11 and do not use GTest or any other test framework.
- The tests call the focal function via C linkage (extern "C") since the function is defined with C linkage in the project.
- The test harness uses simple, non-terminating assertions (printouts and return codes) to maximize code coverage and allow multiple tests to run in a single invocation.
- The test relies on the presence of the LittleCMS (or compatible) CRT, headers, and library linked into the test executable.
- You may need to adjust your build system to link against the library that provides CheckCGATS2 (and the referenced cmsIT8LoadFromMem/cmsIT8Free) and to ensure the symbol is found with C linkage.

Usage guidance (build note, not part of runtime code):
- Compile with a C++11-capable compiler, e.g. g++ -std=c++11 -o test_checkcgats2 test_checkcgats2.cpp -lcms -ldl (adjust to your environment).
- Ensure the library that provides CheckCGATS2 and the cmsIT8LoadFromMem/cmsIT8Free symbols is linked.

This file provides two tests:
1) CheckCGATS2_ReturnsOne: Verifies that CheckCGATS2 returns 1 for normal execution path.
2) CheckCGATS2_TwiceReturnsOne: Verifies that two consecutive invocations both return 1, ensuring consistent behavior across multiple calls.

Now the code:

*/

// Compile-time note: declare the focal function with C linkage to match the project's symbol.
// The exact typedef (cmsInt32Number) is typically int; hence using int here is a safe assumption for linkage.
extern "C" int CheckCGATS2(void);

// Lightweight, non-terminating test harness
#include <iostream>
#include <testcms2.h>


// Test 1: Ensure CheckCGATS2 returns 1 on a typical invocation
// Rationale: the focal implementation returns 1 regardless of the internal cmsIT8LoadFromMem result,
// but it does exercise the true branch by attempting to load from memory and conditionally freeing.
int Test_CheckCGATS2_ReturnsOne() {
    int result = CheckCGATS2();
    if (result != 1) {
        std::cerr << "[Test] CheckCGATS2_ReturnsOne: FAIL - expected 1, got " << result << std::endl;
        return 1; // indicates failure, but will not terminate the entire test run
    }
    std::cout << "[Test] CheckCGATS2_ReturnsOne: PASS - got 1 as expected" << std::endl;
    return 0;
}

// Test 2: Ensure repeated invocations remain stable (idempotence across calls)
int Test_CheckCGATS2_TwiceReturnsOne() {
    int r1 = CheckCGATS2();
    int r2 = CheckCGATS2();
    if (r1 == 1 && r2 == 1) {
        std::cout << "[Test] CheckCGATS2_TwiceReturnsOne: PASS - consecutive calls returned 1" << std::endl;
        return 0;
    } else {
        std::cerr << "[Test] CheckCGATS2_TwiceReturnsOne: FAIL - results: " << r1 << ", " << r2 << std::endl;
        return 1;
    }
}

int main() {
    int failures = 0;

    // Run individual tests, accumulate failures without aborting early
    failures += Test_CheckCGATS2_ReturnsOne();
    failures += Test_CheckCGATS2_TwiceReturnsOne();

    if (failures == 0) {
        std::cout << "[Summary] All tests passed." << std::endl;
    } else {
        std::cerr << "[Summary] Total failures: " << failures << std::endl;
    }

    return failures;
}