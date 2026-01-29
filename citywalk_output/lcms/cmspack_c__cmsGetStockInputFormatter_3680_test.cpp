// Unit tests for cmsFormatter _cmsGetStockInputFormatter
// This test suite targets the focal method described in the prompt.
// It uses a lightweight, self-contained harness (no Google Test) with
// simple, non-terminating assertions to exercise both 16-bit and float
// stock input formatter lookups and the no-match fallback path.
//
// Note: This test assumes the LittleCMS-like types and symbols are available
// via the project headers (e.g., lcms2.h). It uses extern "C" to declare
// the C-style function under test.
//
// Explanatory comments accompany each test to describe intent and coverage.

#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include the library headers that define cmsUInt32Number, cmsFormatter, and
// the CMS_PACK_FLAGS_... constants. The exact header path may vary per project.
// If your environment uses a different include path, adjust accordingly.

// Declare the focal function for C linkage (as it is defined in a C file)
extern "C" cmsFormatter _cmsGetStockInputFormatter(cmsUInt32Number dwInput, cmsUInt32Number dwFlags);

// Simple test framework (non-terminating assertions)
static int gTotalTests = 0;
static int gFailedTests = 0;

#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[ASSERT FAILED] " << (msg) \
                  << " (in " << __FUNCTION__ << ")\n"; \
        ++gFailedTests; \
    } \
    ++gTotalTests; \
} while(false)

#define ASSERT_NOT_NULL(ptr, msg) do { \
    if((ptr) == NULL) { \
        std::cerr << "[ASSERT FAILED] " << (msg) \
                  << " (pointer is NULL in " << __FUNCTION__ << ")\n"; \
        ++gFailedTests; \
    } \
    ++gTotalTests; \
} while(false)

#define ASSERT_NULL(ptr, msg) do { \
    if((ptr) != NULL) { \
        std::cerr << "[ASSERT FAILED] " << (msg) \
                  << " (pointer is non-NULL in " << __FUNCTION__ << ")\n"; \
        ++gFailedTests; \
    } \
    ++gTotalTests; \
} while(false)

// Helper: Attempt to find a 16-bit stock input formatter by probing several
// candidate inputs. Returns true if a match was found (Fmt16 != NULL).
static bool test_find_16bit_match(void) {
    // A set of candidate inputs chosen to likely trigger a match if the
    // library provides at least one 16-bit stock formatter entry.
    const std::vector<uint32_t> candidates = {
        0x00000001u, 0x00000002u, 0x00000010u, 0x01000000u,
        0x00010000u, 0x3F000000u, 0x80000000u, 0xAABBCCDDu,
        0x12345678u, 0xFFFFFFFFu
    };

    for (uint32_t v : candidates) {
        cmsFormatter fr = _cmsGetStockInputFormatter(static_cast<cmsUInt32Number>(v),
                                                     CMS_PACK_FLAGS_16BITS);
        if (fr.Fmt16 != NULL) {
            // Found a 16-bit match
            // We only verify that a 16-bit formatter was chosen; content of Fmt16 is implementation-specific.
            return true;
        }
    }
    // If none of the tested inputs produced a 16-bit match, return false.
    return false;
}

// Helper: Attempt to find a float stock input formatter by probing several
// candidate inputs. Returns true if a match was found (FmtFloat != NULL).
static bool test_find_float_match(void) {
    // Similar candidate set; include values that could trigger a float formatter.
    const std::vector<uint32_t> candidates = {
        0x00000001u, 0x00000004u, 0x00000040u, 0x01000000u,
        0x00020000u, 0x3F800000u, 0x80000000u, 0x00FF00FFu,
        0x9ABCDEF0u, 0xFFFFFFFFu
    };

    for (uint32_t v : candidates) {
        cmsFormatter fr = _cmsGetStockInputFormatter(static_cast<cmsUInt32Number>(v),
                                                     CMS_PACK_FLAGS_FLOAT);
        if (fr.FmtFloat != NULL) {
            // Found a float match
            return true;
        }
    }
    // No matching float formatter found among tested inputs
    return false;
}

// Test 1: Verify that a 16-bit stock input formatter can be selected.
// Rationale: There should exist at least one 16-bit stock formatter entry in the
// implementation. This test ensures the path returns a non-NULL Fmt16 for at
// least one input value and that we don't spuriously populate FmtFloat in this path.
static bool cmsGetStockInputFormatter_matches_16bit_test(void) {
    // Try to locate any 16-bit match; assert success.
    bool ok = test_find_16bit_match();
    ASSERT_TRUE(ok, "Expected to find a 16-bit stock input formatter match (Fmt16 != NULL)");
    // If a match was found, additional check: FmtFloat should not be touched/guaranteed non-NULL.
    // Since fr is a local object and not initialized to a known default, we avoid asserting on FmtFloat here.
    return ok;
}

// Test 2: Verify that a float stock input formatter can be selected.
// Rationale: There should exist at least one float stock formatter entry. This test
// ensures the path returns a non-NULL FmtFloat for at least one input value.
static bool cmsGetStockInputFormatter_matches_float_test(void) {
    bool ok = test_find_float_match();
    ASSERT_TRUE(ok, "Expected to find a float stock input formatter match (FmtFloat != NULL)");
    return ok;
}

// Test 3: Verify that when no match exists, the 16-bit lookup returns NULL for Fmt16.
// Rationale: The default path should clear/leave a NULL formatter when no entry matches.
static bool cmsGetStockInputFormatter_no_match_test(void) {
    // A value that is extremely unlikely to match any entry (best effort).
    const cmsUInt32Number no_match_input = 0xFFFFFFFFu;
    cmsFormatter fr = _cmsGetStockInputFormatter(no_match_input, CMS_PACK_FLAGS_16BITS);
    ASSERT_NULL(fr.Fmt16, "Expected Fmt16 == NULL for non-matching input (no 16-bit formatter)");
    // Do not check FmtFloat here due to uncertainty about uninitialized state when no match occurs.
    return true;
}

// Main entry: run tests and report summary.
int main(void) {
    std::cout << "Starting cmsGetStockInputFormatter unit tests...\n";

    // Run tests; each test prints its own status via ASSERT macros.
    cmsGetStockInputFormatter_matches_16bit_test();
    cmsGetStockInputFormatter_matches_float_test();
    cmsGetStockInputFormatter_no_match_test();

    // Summary
    std::cout << "Tests run: " << gTotalTests
              << ", Failures: " << gFailedTests << "\n";

    // Non-zero exit on failure to aid automation, but tests do not terminate on failure by themselves.
    return (gFailedTests == 0) ? 0 : 1;
}