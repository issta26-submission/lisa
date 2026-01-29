// Lightweight unit test suite for CubeSize (cmsUInt32Number CubeSize(const cmsUInt32Number Dims[], cmsUInt32Number b))
// This test harness is designed for C++11 compilation and links against the C implementation
// of CubeSize in cmslut.c. No Google Test or gMock is used per requirements.
// The tests use a simple, non-terminating assertion style (manual test results) to maximize coverage.

#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <lcms2_internal.h>


// Domain knowledge and test requirements:
// - We test true/false branches of the internal conditions: dim <= 1, overflow checks after multiplication,
//   and final overflow check rv > UINT_MAX / 15.
// - We avoid NULL Dims by design; tests supply valid arrays.
// - We do not mock any static/private internals; we only call the public CubeSize function from cmslut.c.
// - We rely on extern "C" to link with the C implementation.

using cmsUInt32Number = unsigned int;

// Declaration of the focal function (C linkage to match CMS implementation)
extern "C" cmsUInt32Number CubeSize(const cmsUInt32Number Dims[], cmsUInt32Number b);

// Simple test harness: a minimal framework to run tests and report results
struct TestCase {
    std::string name;
    std::vector<cmsUInt32Number> dims;
    cmsUInt32Number b;
    cmsUInt32Number expected;
};

// Helper: format dims for output
static std::string formatDims(const std::vector<cmsUInt32Number>& d) {
    std::string s = "[";
    for (size_t i = 0; i < d.size(); ++i) {
        s += std::to_string(d[i]);
        if (i + 1 < d.size()) s += ", ";
    }
    s += "]";
    return s;
}

int main() {
    // Prepare test cases
    std::vector<TestCase> tests = {
        // Test 1: Normal small dimensions, expect product 2*3*4 = 24
        { "Normal case: 2x3x4 with b=3", {2, 3, 4}, 3, 24 },

        // Test 2: Single dimension, b=1 => product is that single dimension
        { "Single dimension with b=1: [5], b=1", {5}, 1, 5 },

        // Test 3: Edge case b=0 => rv starts at 1, final check passes, expect 1
        { "Edge case b=0: any dims, b=0", {2, 3, 4}, 0, 1 },

        // Test 4: A dimension <= 1 in the last b positions should trigger error (return 0)
        { "Dim <= 1 in last b: [2,1,3], b=3 => 0", {2, 1, 3}, 3, 0 },

        // Test 5: Overflow during first multiplication step (dim too large relative to rv)
        // Dims[1] = 90000 makes rv = 90000; overflow check triggers as rv > UINT_MAX / dim
        { "Overflow on first step: [50000, 90000], b=2 => 0", {50000, 90000}, 2, 0 },

        // Test 6: Final overflow check (product exceeds UINT_MAX/15)
        // rv = 100000 * 40000 = 4e9; final check rv > UINT_MAX/15 should return 0
        { "Final overflow check: [100000, 40000], b=2 => 0", {100000, 40000}, 2, 0 },

        // Test 7: Another normal case with small dims, ensures proper multiplication order
        { "Normal case: [3,5], b=2 => 15", {3, 5}, 2, 15 },

        // Test 8: Complex multi-dim case with no overflow
        { "Normal multi-dim: [3,6,7], b=3 => 126", {3, 6, 7}, 3, 126 }
    };

    int total = static_cast<int>(tests.size());
    int passed = 0;

    for (const auto& t : tests) {
        cmsUInt32Number result = 0;
        // Prepare data for CubeSize (remember: it uses Dims[b-1] indexing)
        const cmsUInt32Number* data = nullptr;
        // Safe: ensure we have at least one element when b > 0
        if (!t.dims.empty()) {
            // Pass pointer to first element; CubeSize will access up to b elements from the end
            data = t.dims.data();
        }
        // Call the focal function
        result = CubeSize(data, t.b);

        // Output result with explanatory comments
        bool ok = (result == t.expected);
        std::cout << std::boolalpha;
        std::cout << "Test: " << t.name << "\n";
        std::cout << "  Dims: " << formatDims(t.dims) << "\n";
        std::cout << "  b: " << t.b << "\n";
        std::cout << "  Expected: " << t.expected << ", Got: " << result
                  << (ok ? "  [PASS]" : "  [FAIL]") << "\n\n";

        if (ok) ++passed;
    }

    // Summary
    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}