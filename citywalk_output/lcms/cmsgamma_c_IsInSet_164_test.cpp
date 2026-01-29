/*
Step 1 (Program Understanding) and Candidate Keywords:
- Core components used by IsInSet: _cmsParametricCurvesCollection, c->nFunctions, c->FunctionTypes[], Type, abs(Type)
- Behavior: iterate over FunctionTypes up to nFunctions, return index of first element equal to abs(Type); otherwise return -1
- Key logic: loop, abs comparison, early return, -1 fallback
- Test goals: verify first match, negative Type handling, not-found case, and zero-length collection
*/

/*
Step 2 & 3 (Unit Test Generation and Refinement):
- Implement a small C++11 test harness (no GTest) that calls the C function IsInSet via extern "C".
- Use the real dependency type _cmsParametricCurvesCollection from lcms2_internal.h.
- Validate:
  1) Basic positive match at first, middle, and last positions.
  2) Negative Type matches via abs(Type).
  3) Not-found scenario returns -1.
  4) Zero-length collection returns -1.
- Ensure tests terminate non-fatally (collect failures and report at end).
- Use only standard library; provide explanatory comments for each test block.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstdlib>


// Ensure C linkage for the focal function when linking with a C library
extern "C" int IsInSet(int Type, _cmsParametricCurvesCollection* c);

static int g_failures = 0;

// Simple non-terminating assertion macro for test reporting
#define ASSERT_EQ(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        ++g_failures; \
        std::cerr << "[FAIL] " << msg << " | Expected: " << (expected) \
                  << ", Actual: " << (actual) << std::endl; \
    } \
} while(0)

int main() {
    // Test 1: Type matches first element
    {
        // Arrange
        _cmsParametricCurvesCollection c;
        int types[] = {1, 5, 7};
        c.nFunctions = 3;
        c.FunctionTypes = types;

        // Act
        int res = IsInSet(5, &c);

        // Assert
        ASSERT_EQ(1, res, "IsInSet should return index 1 for Type=5 in {1,5,7}");
    }

    // Test 2: Type matches a later element (middle)
    {
        _cmsParametricCurvesCollection c;
        int types[] = {1, 5, 7};
        c.nFunctions = 3;
        c.FunctionTypes = types;

        int res = IsInSet(7, &c);
        ASSERT_EQ(2, res, "IsInSet should return index 2 for Type=7 in {1,5,7}");
    }

    // Test 3: Type matches using negative value (abs(Type))
    {
        _cmsParametricCurvesCollection c;
        int types[] = {1, 5, 7};
        c.nFunctions = 3;
        c.FunctionTypes = types;

        int res = IsInSet(-5, &c);
        ASSERT_EQ(1, res, "IsInSet should return index 1 for abs(Type)==5 when Type=-5");
    }

    // Test 4: Type not in set
    {
        _cmsParametricCurvesCollection c;
        int types[] = {2, 4, 6};
        c.nFunctions = 3;
        c.FunctionTypes = types;

        int res = IsInSet(3, &c);
        ASSERT_EQ(-1, res, "IsInSet should return -1 when Type=3 is not in {2,4,6}");
    }

    // Test 5: Empty collection (nFunctions = 0)
    {
        _cmsParametricCurvesCollection c;
        int types[] = {}; // empty
        c.nFunctions = 0;
        c.FunctionTypes = types;

        int res = IsInSet(1, &c);
        ASSERT_EQ(-1, res, "IsInSet should return -1 for empty collection (nFunctions=0)");
    }

    // Test 6: Duplicates in the set; ensure first occurrence is returned
    {
        _cmsParametricCurvesCollection c;
        int types[] = {2, 2, 3, 5};
        c.nFunctions = 4;
        c.FunctionTypes = types;

        int res = IsInSet(2, &c);
        ASSERT_EQ(0, res, "IsInSet should return first index (0) for duplicate value 2");
    }

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}