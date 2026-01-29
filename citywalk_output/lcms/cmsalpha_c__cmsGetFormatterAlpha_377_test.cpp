/*
Unit Test Suite for cmsFormatterAlphaFn _cmsGetFormatterAlpha (cmsalpha.c)

Step 1: Program Understanding (summary embedded as comments)
- The focal method _cmsGetFormatterAlpha selects a formatter function pointer from a 6x6 static table FormattersAlpha.
- Rows correspond to source alpha widths: 8, 16, 16SE, HLF, FLT, DBL.
- Columns correspond to destination alpha widths: 8, 16, 16SE, HLF, FLT, DBL.
- It computes in_n = FormatterPos(in) and out_n = FormatterPos(out). If either is invalid or outside 0..5, it signals an error and returns NULL.
- Otherwise, returns the function pointer at FormattersAlpha[in_n][out_n].
- We test:
  - Valid mappings return non-NULL pointers.
  - Invalid mappings return NULL.
  - Specific mappings map to expected known functions (copy8, from8toDBL, copy64).

Step 2: Unit Test Generation (targeted tests)
- Validate typical valid mappings:
  - 8 -> 16 should return a non-NULL function (from 8-bit to 16-bit).
  - 16 -> 8 should return a non-NULL function (from 16-bit to 8-bit).
- Validate invalid mappings return NULL:
  - in width out of range (e.g., in = 6) returns NULL.
  - out width out of range (e.g., out = 999) returns NULL.
- Validate exact known mappings by pointer equality:
  - 8 -> 8 should map to copy8.
  - 8 ->  DBL (8 -> 64-bit) should map to from8toDBL.
  -  DBL -> DBL should map to copy64.

Step 3: Test Case Refinement (Domain knowledge)
- Use only the public interface (through lcms2.h) to obtain types; avoid private/internal details beyond function pointers.
- Use simple, non-terminating assertions via standard I/O (no GTest/GMock).
- Treat static/internal details as implementation, test only observable behavior (return values and pointer identities for known mappings).

Notes:
- This test relies on the LittleCMS public header (lcms2.h) and the library being linked at link-time.
- We call with a null context since the behavior under test is independent of the context for these checks.
- We perform pointer equality checks against known internal formatter functions (copy8, from8toDBL, copy64) to verify exact mappings. We declare extern prototypes for these functions to enable pointer comparisons.

Code (single-file C++11 test suite):
*/

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstdint>


// Declarations for internal formatter functions used for pointer equality checks.
// These symbols exist in cmsalpha.c and are exported by the library (not static).
extern "C" void copy8(void*, const void*);
extern "C" void from8toDBL(void*, const void*);
extern "C" void copy64(void*, const void*);

// Declare the focal function under test (internal symbol by name).
extern "C" cmsFormatterAlphaFn _cmsGetFormatterAlpha(cmsContext id, cmsUInt32Number in, cmsUInt32Number out);

int main() {
    bool all_tests_passed = true;

    // Test 1: Valid mapping 8 -> 16 should yield a non-NULL formatter
    {
        cmsFormatterAlphaFn f = _cmsGetFormatterAlpha(nullptr, 0, 1); // 8-bit source to 16-bit destination
        if (f == nullptr) {
            std::cerr << "Test 1 FAILED: Expected non-NULL formatter for 8->16\n";
            all_tests_passed = false;
        } else {
            std::cout << "Test 1 PASSED: Non-NULL formatter returned for 8->16\n";
        }
    }

    // Test 2: Valid mapping 16 -> 8 should yield a non-NULL formatter
    {
        cmsFormatterAlphaFn f = _cmsGetFormatterAlpha(nullptr, 5, 0); // 16-bit source to 8-bit destination
        if (f == nullptr) {
            std::cerr << "Test 2 FAILED: Expected non-NULL formatter for 16->8\n";
            all_tests_passed = false;
        } else {
            std::cout << "Test 2 PASSED: Non-NULL formatter returned for 16->8\n";
        }
    }

    // Test 3: Invalid input width (in out of range)
    {
        cmsFormatterAlphaFn f = _cmsGetFormatterAlpha(nullptr, 6, 0); // invalid 'in'
        if (f != nullptr) {
            std::cerr << "Test 3 FAILED: Expected NULL formatter for invalid in width (6)\n";
            all_tests_passed = false;
        } else {
            std::cout << "Test 3 PASSED: NULL formatter returned for invalid in width\n";
        }
    }

    // Test 4: Invalid output width (out of range)
    {
        cmsFormatterAlphaFn f = _cmsGetFormatterAlpha(nullptr, 0, 999); // invalid 'out'
        if (f != nullptr) {
            std::cerr << "Test 4 FAILED: Expected NULL formatter for invalid out width (999)\n";
            all_tests_passed = false;
        } else {
            std::cout << "Test 4 PASSED: NULL formatter returned for invalid out width\n";
        }
    }

    // Test 5: Exact mapping 8 -> 8 should map to copy8
    {
        cmsFormatterAlphaFn f = _cmsGetFormatterAlpha(nullptr, 0, 0); // 8 -> 8
        if (f == nullptr) {
            std::cerr << "Test 5 FAILED: Expected non-NULL formatter for 8->8\n";
            all_tests_passed = false;
        } else if (f != (cmsFormatterAlphaFn)copy8) {
            std::cerr << "Test 5 FAILED: Expected pointer to copy8 for 8->8, got a different function\n";
            all_tests_passed = false;
        } else {
            std::cout << "Test 5 PASSED: 8->8 maps to copy8 as expected\n";
        }
    }

    // Test 6: Exact mapping 8 -> DBL should map to from8toDBL
    {
        cmsFormatterAlphaFn f = _cmsGetFormatterAlpha(nullptr, 0, 5); // 8 -> DBL
        if (f == nullptr) {
            std::cerr << "Test 6 FAILED: Expected non-NULL formatter for 8->DBL\n";
            all_tests_passed = false;
        } else if (f != (cmsFormatterAlphaFn)from8toDBL) {
            std::cerr << "Test 6 FAILED: Expected pointer to from8toDBL for 8->DBL, got a different function\n";
            all_tests_passed = false;
        } else {
            std::cout << "Test 6 PASSED: 8->DBL maps to from8toDBL as expected\n";
        }
    }

    // Test 7: Exact mapping  DBL -> DBL should map to copy64
    {
        cmsFormatterAlphaFn f = _cmsGetFormatterAlpha(nullptr, 5, 5); // DBL -> DBL
        if (f == nullptr) {
            std::cerr << "Test 7 FAILED: Expected non-NULL formatter for DBL->DBL\n";
            all_tests_passed = false;
        } else if (f != (cmsFormatterAlphaFn)copy64) {
            std::cerr << "Test 7 FAILED: Expected pointer to copy64 for DBL->DBL, got a different function\n";
            all_tests_passed = false;
        } else {
            std::cout << "Test 7 PASSED: DBL->DBL maps to copy64 as expected\n";
        }
    }

    // Final result
    if (all_tests_passed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED\n";
        return 1;
    }
}