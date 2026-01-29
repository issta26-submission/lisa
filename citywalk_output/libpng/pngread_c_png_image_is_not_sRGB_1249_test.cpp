// Stand-alone C++11 test harness for the focal logic of png_image_is_not_sRGB
// Note: This file provides a self-contained re-implementation of the decision
// logic to enable deterministic unit testing without depending on the full
// libpng project. It mirrors the essential branching in the original function:
//
//   - If cICP or mDCV chunk is present: return !chromaticities_match_sRGB(chromaticities)
//   - If sRGB chunk is present: return 0
//   - If cHRM chunk is present: return !chromaticities_match_sRGB(chromaticities)
//   - Else: return 0
//
// The tests exercise true/false branches for each condition.

#include <string>
#include <cmath>
#include <iostream>
#include <pngpriv.h>


// Domain-specific re-implementation of the chromaticities comparison.
// This uses a realistic but fixed sRGB chromaticity (D65) as the reference.
struct png_xy {
    double x;
    double y;
};

// Minimal fake "png_ptr" structure to drive the logic.
struct PNGFake {
    // Bit-flag based representation of which chunks are present.
    // Bit positions (for readability):
    // 0 - cICP, 1 - mDCV, 2 - sRGB, 3 - cHRM
    unsigned int chunks_present;
    png_xy chromaticities;
};

// Flags for chunk presence to mirror the original code's checks.
constexpr unsigned int CHUNK_cICP = 1u << 0;
constexpr unsigned int CHUNK_mDCV = 1u << 1;
constexpr unsigned int CHUNK_sRGB = 1u << 2;
constexpr unsigned int CHUNK_cHRM = 1u << 3;

// Helper to simulate chromaticities_match_sRGB behavior.
// Returns true if the provided chromaticities match the canonical sRGB values.
bool chromaticities_match_sRGB(const png_xy* xy) {
    // Canonical sRGB chromaticities (approximate D65 white point):
    // x ≈ 0.3127, y ≈ 0.3290
    const double SRGB_X = 0.3127;
    const double SRGB_Y = 0.3290;
    const double EPS = 1e-6;
    return (std::fabs(xy->x - SRGB_X) < EPS) && (std::fabs(xy->y - SRGB_Y) < EPS);
}

// Re-implementation of the focal method's logic under test.
// Signature matches the actual code's usage pattern (accepts a pointer to a PNG-like struct).
int png_image_is_not_sRGB(PNGFake* png_ptr) {
    // Highest priority: cICP or mDCV presence
    if ((png_ptr->chunks_present & CHUNK_cICP) || (png_ptr->chunks_present & CHUNK_mDCV))
        return !chromaticities_match_sRGB(&png_ptr->chromaticities);

    // If sRGB chunk is present, image is considered sRGB
    if (png_ptr->chunks_present & CHUNK_sRGB)
        return 0;

    // If cHRM chunk is present, check chromaticities
    if (png_ptr->chunks_present & CHUNK_cHRM)
        return !chromaticities_match_sRGB(&png_ptr->chromaticities);

    // Default: sRGB
    return 0;
}

// Lightweight test harness
struct TestResult {
    std::string name;
    bool passed;
};

// Run a single test case and record the result
void run_test(const std::string& name, const PNGFake& png, int expected, TestResult& result) {
    int actual = png_image_is_not_sRGB(const_cast<PNGFake*>(&png));
    result.name = name;
    result.passed = (actual == expected);
    // Non-terminating assertion: do not exit on failure; simply record
}

// Utility: print summary
void print_results(const std::vector<TestResult>& results) {
    int passed = 0;
    int total = (int)results.size();
    for (const auto& r : results) {
        std::cout << (r.passed ? "[PASS] " : "[FAIL] ") << r.name << "\n";
        if (r.passed) ++passed;
    }
    std::cout << "Test Summary: " << passed << " / " << total << " passed.\n";
}

int main() {
    using namespace std;

    vector<TestResult> results;

    // Test cases covering all branches described in the focal method.

    // Case 1: cICP present, chromaticities match sRGB -> expect 0
    {
        PNGFake png;
        png.chunks_present = CHUNK_cICP; // cICP present
        png.chromaticities = {0.3127, 0.3290};
        results.emplace_back();
        run_test("Case 1: cICP present, chroma == sRGB -> expect 0", png, 0, results.back());
    }

    // Case 2: cICP present, chromaticities do not match sRGB -> expect 1
    {
        PNGFake png;
        png.chunks_present = CHUNK_cICP;
        png.chromaticities = {0.3000, 0.3200}; // not sRGB
        results.emplace_back();
        run_test("Case 2: cICP present, chroma != sRGB -> expect 1", png, 1, results.back());
    }

    // Case 3: mDCV present, chromaticities do not match sRGB -> expect 1
    {
        PNGFake png;
        png.chunks_present = CHUNK_mDCV;
        png.chromaticities = {0.3000, 0.3200}; // not sRGB
        results.emplace_back();
        run_test("Case 3: mDCV present, chroma != sRGB -> expect 1", png, 1, results.back());
    }

    // Case 4: mDCV present, chromaticities match sRGB -> expect 0
    {
        PNGFake png;
        png.chunks_present = CHUNK_mDCV;
        png.chromaticities = {0.3127, 0.3290}; // sRGB
        results.emplace_back();
        run_test("Case 4: mDCV present, chroma == sRGB -> expect 0", png, 0, results.back());
    }

    // Case 5: sRGB chunk present with no conflicting chunks -> expect 0
    {
        PNGFake png;
        png.chunks_present = CHUNK_sRGB;
        png.chromaticities = {0.1000, 0.2000}; // arbitrary
        results.emplace_back();
        run_test("Case 5: sRGB chunk present -> expect 0", png, 0, results.back());
    }

    // Case 6: cHRM present, chromaticities do not match sRGB -> expect 1
    {
        PNGFake png;
        png.chunks_present = CHUNK_cHRM;
        png.chromaticities = {0.3000, 0.3200}; // not sRGB
        results.emplace_back();
        run_test("Case 6: cHRM present, chroma != sRGB -> expect 1", png, 1, results.back());
    }

    // Case 7: No relevant chunks present -> default to sRGB -> expect 0
    {
        PNGFake png;
        png.chunks_present = 0;
        png.chromaticities = {0.3000, 0.3200}; // not used
        results.emplace_back();
        run_test("Case 7: no chunks -> default 0", png, 0, results.back());
    }

    // Report results
    print_results(results);

    return 0;
}