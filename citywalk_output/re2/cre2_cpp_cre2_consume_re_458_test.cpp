#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


/*
 Lightweight non-terminating test harness.
 - Provides EXPECT_TRUE and EXPECT_EQ macros that report failures but do not abort.
 - Accumulates failures in a global counter.
 - Exposes a focused test suite for cre2_consume_re as per the focal method.
*/

// Global failure counter for the lightweight test harness
static int g_failures = 0;

// Non-terminating assertions
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << #cond \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if(!(_a == _b)) { \
        std::cerr << "EXPECT_EQ failed: " << #a << " == " << #b \
                  << " (" << _a << " != " << _b << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(0)

/*
 Test suite for cre2_consume_re (REX variant) as provided by the focal code.
 Assumptions:
 - cre2_new(pattern, pattern_len, opt) creates a compiled regex.
 - cre2_consume_re returns a non-negative integer indicating success (positive)
   or failure (0 or negative) for consumption of a match.
 - cre2_string_t is a simple string container with fields data (char*) and length (int).
 - The tests use anchor value 0 as a safe default for cre2_anchor_t.
 - We avoid freeing match.data to prevent incorrect free assumptions across
   different CRE2 build configurations; the API may allocate internal buffers.
 - We perform a few representative scenarios to exercise true/false paths.
*/

// Simple test wrapper class (methods called from main)
class Cre2ConsumeReTests {
public:
    // Positive case: pattern "abc" in "abcdef" should produce a consumable match.
    // Verifies that the function indicates a match was consumed.
    static void testPositiveMatch() {
        // Build a simple regex
        cre2_regexp_t *rex = cre2_new("abc", 3, NULL);
        EXPECT_TRUE(rex != NULL);

        if (rex) {
            const char *text = "abcdef";
            int text_len = (int)std::strlen(text);
            cre2_string_t match;
            match.data = NULL;
            match.length = 0;

            // Use a default anchor (0) and generic start/end bounds
            int res = cre2_consume_re(rex, text, text_len, 0, text_len, 0, &match, 1);
            // Expecting a positive result indicating a match was consumed
            EXPECT_TRUE(res > 0);

            // Clean up regex object
            cre2_delete(rex);
        }
    }

    // Negative case: pattern not present should return 0 (no consumption)
    static void testNoMatch() {
        cre2_regexp_t *rex = cre2_new("zzz", 3, NULL);
        EXPECT_TRUE(rex != NULL);

        if (rex) {
            const char *text = "abcdef";
            int text_len = (int)std::strlen(text);
            cre2_string_t match;
            match.data = NULL;
            match.length = 0;

            int res = cre2_consume_re(rex, text, text_len, 0, text_len, 0, &match, 1);
            // Expect no match consumed
            EXPECT_EQ(res, 0);

            cre2_delete(rex);
        }
    }

    // Boundary case: start position near the end should still find a match if applicable
    // (Text "zzabc", pattern "abc", startpos=2 should yield a match)
    static void testStartPosMatch() {
        cre2_regexp_t *rex = cre2_new("abc", 3, NULL);
        EXPECT_TRUE(rex != NULL);

        if (rex) {
            const char *text = "zzabc";
            int text_len = (int)std::strlen(text);
            cre2_string_t match;
            match.data = NULL;
            match.length = 0;

            int res = cre2_consume_re(rex, text, text_len, 2, text_len, 0, &match, 1);
            // Expect a successful consumption when matching in range
            EXPECT_TRUE(res > 0);

            cre2_delete(rex);
        }
    }

    // Optional: basic resilience test for invalid/inconsistent inputs
    // Here we check that creating a regex with a plausible pattern succeeds.
    // We purposely avoid NULL pattern to prevent UB; this test ensures API usage works.
    static void testInvalidPatternHandling() {
        // Some implementations may fail with NULL pattern; we skip creating with NULL.
        // Instead, attempt a pattern that should be valid and ensure we get a non-NULL rex.
        cre2_regexp_t *rex = cre2_new("a+", 2, NULL);
        if (rex) {
            // If creation succeeds, perform a trivial consume to ensure API works
            const char *text = "a";
            int text_len = 1;
            cre2_string_t match;
            match.data = NULL;
            match.length = 0;

            int res = cre2_consume_re(rex, text, text_len, 0, text_len, 0, &match, 1);
            // Accept any non-negative outcome
            EXPECT_TRUE(res >= 0);

            cre2_delete(rex);
        } else {
            // If rex creation failed, that's still a valid negative test outcome
            EXPECT_TRUE(rex != NULL); // This will fail and report, but keeps flow
        }
    }
};

// Main function to execute tests in a controlled manner.
// The tests are designed to run without terminating on failures.
int main() {
    // Run individual test cases
    Cre2ConsumeReTests::testPositiveMatch();
    Cre2ConsumeReTests::testNoMatch();
    Cre2ConsumeReTests::testStartPosMatch();
    Cre2ConsumeReTests::testInvalidPatternHandling();

    // Summary
    if (g_failures == 0) {
        std::cout << "Cre2ConsumeReTests: ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "Cre2ConsumeReTests: " << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}