// Unit tests for GetInterval function (C-like implementation) using C++11 standard libs only.
// The test suite is designed to be self-contained (no GTest) and executable via main().
// Note: This test file contains a verbatim (cleaned) reproduction of GetInterval from the focal method
// and uses minimal type definitions to compile and run in a C++11 environment.

#include <limits>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// Minimal type definitions to mirror the focal function's expected environment.
typedef double cmsFloat64Number;
typedef uint16_t cmsUInt16Number;

struct _cms_interp_struc {
    int Domain[2]; // Domain[0] is used by GetInterval as the number of intervals (must be >= 1 to be valid)
};

// Focal method copied and adapted for isolated testing.
// Original logic is preserved to ensure accurate behavior coverage.
int GetInterval(cmsFloat64Number In, const cmsUInt16Number LutTable[], const struct _cms_interp_struc* p)
{
    int i;
    int y0, y1;
    // A 1 point table is not allowed
    if (p -> Domain[0] < 1) return -1;
    // Let's see if ascending or descending.
    if (LutTable[0] < LutTable[p ->Domain[0]]) {
        // Table is overall ascending
        for (i = (int) p->Domain[0] - 1; i >= 0; --i) {
            y0 = LutTable[i];
            y1 = LutTable[i+1];
            if (y0 <= y1) { // Increasing
                if (In >= y0 && In <= y1) return i;
            }
            else {
                // Decreasing segment within an otherwise ascending table is not expected here,
                // but the original code handles the case gracefully.
                if (y1 < y0) { // Decreasing
                    if (In >= y1 && In <= y0) return i;
                }
            }
        }
    }
    else {
        // Table is overall descending
        for (i=0; i < (int) p -> Domain[0]; i++) {
            y0 = LutTable[i];
            y1 = LutTable[i+1];
            if (y0 <= y1) { // Increasing
                if (In >= y0 && In <= y1) return i;
            }
            else {
                if (y1 < y0) { // Decreasing
                    if (In >= y1 && In <= y0) return i;
                }
            }
        }
    }
    return -1;
}

// Simple test harness (non-terminating assertions style)
static int gFailures = 0;
static void logFail(const std::string& desc, int expected, int actual)
{
    ++gFailures;
    std::cout << "FAILURE: " << desc << " | expected: " << expected << " actual: " << actual << "\n";
}
static void logPass(const std::string& desc)
{
    std::cout << "PASS: " << desc << "\n";
}
static void expectEq(const std::string& desc, int expected, int actual)
{
    if (expected == actual) logPass(desc);
    else logFail(desc, expected, actual);
}

// Test 1: Domain[0] < 1 should return -1 (invalid domain)
void test_DomainTooSmall_ReturnsMinusOne()
{
    _cms_interp_struc p;
    p.Domain[0] = 0; // invalid domain
    cmsUInt16Number Lut[2] = {0, 1}; // size doesn't matter as function should bail out early
    int res = GetInterval(0.5, Lut, &p);
    expectEq("Domain<1 should return -1", -1, res);
}

// Test 2: Ascending table, In inside an interval (e.g., between Lut[1] and Lut[2])
void test_AscendingTable_InBetweenReturnsIndex()
{
    _cms_interp_struc p;
    p.Domain[0] = 3; // 3 intervals => LutTable has 4 entries
    cmsUInt16Number Lut[4] = {0, 10, 20, 30}; // strictly ascending
    int res = GetInterval(25.0, Lut, &p);
    expectEq("Ascending table, In=25 should return index 2", 2, res);

    // Boundary case: In exactly at Lut[1] (10), should return index 1
    res = GetInterval(10.0, Lut, &p);
    expectEq("Ascending table boundary, In=10 should return index 1", 1, res);

    // In between Lut[0] and Lut[1] (e.g., 5) should return 0
    res = GetInterval(5.0, Lut, &p);
    expectEq("Ascending table, In=5 should return index 0", 0, res);
}

// Test 3: Descending table, In inside an interval
void test_DescendingTable_InBetweenReturnsIndex()
{
    _cms_interp_struc p;
    p.Domain[0] = 3; // 3 intervals => LutTable has 4 entries
    cmsUInt16Number Lut[4] = {30, 25, 15, 5}; // strictly descending
    int res = GetInterval(28.0, Lut, &p);
    expectEq("Descending table, In=28 should return index 0", 0, res);

    // Boundary case: In exactly Lut[1] (25) should still map to index 0
    res = GetInterval(25.0, Lut, &p);
    expectEq("Descending table boundary, In=25 should return index 0", 0, res);

    // In between Lut[1] and Lut[2] range (e.g., 18) should return index 1
    res = GetInterval(18.0, Lut, &p);
    expectEq("Descending table, In=18 should return index 1", 1, res);
}

// Test 4: In outside the overall range should return -1
void test_InOutsideRangeReturnsMinusOne()
{
    _cms_interp_struc p;
    p.Domain[0] = 3;
    cmsUInt16Number Lut[4] = {0, 10, 20, 30}; // ascending
    int res = GetInterval(40.0, Lut, &p); // above max
    expectEq("Ascending table, In=40 outside range should return -1", -1, res);

    res = GetInterval(-5.0, Lut, &p); // below min
    expectEq("Ascending table, In=-5 outside range should return -1", -1, res);
}

// Test 5: Mixed scenarios to exercise both branches thoroughly
void test_MixedScenarios()
{
    // Mixed: ensure that the function handles both ascending and descending transitions gracefully.
    // Ascending scenario
    _cms_interp_struc p1;
    p1.Domain[0] = 2;
    cmsUInt16Number LutAsc[3] = {0, 50, 100};
    int res = GetInterval(75.0, LutAsc, &p1);
    expectEq("Mixed scenario ascending: In=75 should return index 1", 1, res);

    // Descending scenario
    _cms_interp_struc p2;
    p2.Domain[0] = 2;
    cmsUInt16Number LutDesc[3] = {100, 50, 0};
    res = GetInterval(60.0, LutDesc, &p2);
    expectEq("Mixed scenario descending: In=60 should return index 0", 0, res);
}

// Main entry: run all tests
int main()
{
    std::cout << "Starting GetInterval unit tests (C++11, stand-alone)..." << std::endl;

    test_DomainTooSmall_ReturnsMinusOne();
    test_AscendingTable_InBetweenReturnsIndex();
    test_DescendingTable_InBetweenReturnsIndex();
    test_InOutsideRangeReturnsMinusOne();
    test_MixedScenarios();

    if (gFailures == 0) {
        std::cout << "ALL TESTS PASSED." << std::endl;
        return 0;
    } else {
        std::cout << gFailures << " TEST(S) FAILED." << std::endl;
        return 1;
    }
}