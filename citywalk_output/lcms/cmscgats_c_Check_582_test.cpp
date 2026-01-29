// Minimal C++11 test suite for the focal function: cmsBool Check(cmsIT8* it8, SYMBOL sy, const char* Err)
// The tests are designed to compile and run without GoogleTest, using simple asserts that do not terminate
// the test program on failure (non-terminating assertions). Comments explain intent and coverage rationale.

// This test assumes the environment provides the LittleCMS internal headers and the cmsBool/cmsIT8/SYMBOL types
// as used by cmscgats.c. We include the internal header to obtain correct type definitions.
// The tests rely on the behavior of Check: it returns TRUE when it8->sy == sy, otherwise it returns the
// result of SynError(it8, NoMeta(Err)).

// NOTE: The exact return value of SynError is library-defined; these tests assume that on a mismatch
// the function does not return TRUE (typical behavior is to return FALSE). If the library changes this,
// the test expectation may need adjustment.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Include the internal LittleCMS definitions to obtain cmsBool, cmsIT8, SYMBOL, TRUE/FALSE, etc.

// Ensure linkage with the focal function (C linkage)
extern "C" cmsBool Check(cmsIT8* it8, SYMBOL sy, const char* Err);

static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Lightweight non-terminating assertion helpers
#define EXPECT_TRUE(cond) \
    do { \
        ++g_tests_run; \
        if (static_cast<bool>(cond)) { \
            ++g_tests_passed; \
        } else { \
            std::cerr << "Test failed: " #cond " is false at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_tests_failed; \
        } \
    } while (0)

#define EXPECT_FALSE(cond) \
    do { \
        ++g_tests_run; \
        if (!(static_cast<bool>(cond))) { \
            ++g_tests_passed; \
        } else { \
            std::cerr << "Test failed: " #cond " is true at " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_tests_failed; \
        } \
    } while (0)

#define EXPECT_EQ(a, b) \
    do { \
        ++g_tests_run; \
        if (static_cast<decltype(a)>(a) == static_cast<decltype(a)>(b)) { \
            ++g_tests_passed; \
        } else { \
            std::cerr << "Test failed: " #a " == " #b " evaluates to false at " << __FILE__ << ":" << __LINE__ << "\n"; \
            std::cerr << "  Actual: " << (a) << "  Expected: " << (b) << "\n"; \
            ++g_tests_failed; \
        } \
    } while (0)

#define EXPECT_NE(a, b) \
    do { \
        ++g_tests_run; \
        if (static_cast<decltype(a)>(a) != static_cast<decltype(a)>(b)) { \
            ++g_tests_passed; \
        } else { \
            std::cerr << "Test failed: " #a " != " #b " evaluates to false at " << __FILE__ << ":" << __LINE__ << "\n"; \
            std::cerr << "  Actual: " << (a) << "  Compared with: " << (b) << "\n"; \
            ++g_tests_failed; \
        } \
    } while (0)

// Candidate Keywords (Step 1): { it8, sy, Err, TRUE, SynError, NoMeta }
// These are the core dependent components for the focal Check function.
// The unit tests will exercise both the matching and non-matching branches of the predicate it8->sy != sy.

void test_Check_MatchesReturnsTRUE()
{
    // Purpose: Verify that when it8->sy == sy, Check returns TRUE without invoking SynError.
    // This exercises the true-branch of the predicate.

    // Allocate a minimal cmsIT8 object and set its 'sy' field to a known value.
    cmsIT8* it8 = (cmsIT8*) std::calloc(1, sizeof(cmsIT8));
    if (it8 == NULL) {
        std::cerr << "Memory allocation failed for cmsIT8 in test_Check_MatchesReturnsTRUE\n";
        ++g_tests_failed;
        return;
    }

    // Choose a symbolic value for the test; cast safely to SYMBOL as used by the library.
    SYMBOL syValue = (SYMBOL) 0x1234;
    it8->sy = syValue;

    // Err message (not used when sy matches)
    const char* Err = "MatchErrorNotUsed";

    cmsBool result = Check(it8, syValue, Err);

    // Expect TRUE
    EXPECT_EQ(result, TRUE);

    std::free(it8);
}

void test_Check_MismatchReturnsNotTRUE()
{
    // Purpose: Verify that when it8->sy != sy, Check does not return TRUE.
    // This exercises the false-branch of the predicate and the SynError path.
    // We conservatively expect that SynError returns FALSE in this codebase.

    cmsIT8* it8 = (cmsIT8*) std::calloc(1, sizeof(cmsIT8));
    if (it8 == NULL) {
        std::cerr << "Memory allocation failed for cmsIT8 in test_Check_MismatchReturnsNotTRUE\n";
        ++g_tests_failed;
        return;
    }

    // Set it8->sy to a value different from the test sy
    it8->sy = (SYMBOL) 0x0001;
    SYMBOL testSy = (SYMBOL) 0x0002;

    const char* Err = "MismatchError";

    cmsBool result = Check(it8, testSy, Err);

    // We do not assume SynError's exact return value; we only require that it is not TRUE.
    EXPECT_FALSE(result == TRUE);
    // Additionally, to be strict, we can require result to be FALSE if the library follows common semantics.
    // If needed, uncomment the following line to enforce FALSE:
    // EXPECT_EQ(result, FALSE);

    std::free(it8);
}

int main()
{
    // Run tests
    test_Check_MatchesReturnsTRUE();
    test_Check_MismatchReturnsNotTRUE();

    // Summary
    std::cout << " cmsBool Check tests: "
              << g_tests_passed << " passed, "
              << g_tests_failed << " failed, "
              << g_tests_run << " total.\n";

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}