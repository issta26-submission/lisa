/*
Step 1 - Program Understanding and Candidate Keywords
- Focused method: random_mod(unsigned int max)
  – Behavior: returns random_u16() % max
  – Core dependency: random_u16() (pseudo-random 16-bit value)
  – Domain implications: output in range [0, max-1] for max > 0
  – Note: There are no branching conditions inside random_mod; coverage is achieved by exercising different max values and repeated calls to validate range constraints.

Candidate Keywords to anchor tests
- random_mod, random_u16, modulo, max, unsigned int, range [0, max-1]

Step 2 - Unit Test Generation (Guided by Dependencies)
- Target: Provide a small C++11 test suite (no GTest) that can be linked with the C implementation containing random_mod and random_u16.
- Strategy:
  - Validate base contract: random_mod(max) ∈ [0, max-1] for several max values.
  - Verify special case: max = 1 yields always 0.
  - Validate that for a reasonably large max, the function still returns values within the expected range.
  - Validate distribution property for max = 2 (ensures both 0 and 1 appear across many samples) to exercise the modulo behavior.
  - Ensure tests do not terminate on first failure (non-terminating assertions) and collect a summary.

Step 3 - Test Case Refinement
- Use a lightweight C++11 test harness without GTest.
- Implement non-terminating assertions that accumulate failures but allow all scenarios to run.
- Access C functions via extern "C" to enable linking with the C implementation in pngvalid.c.
- Cover multiple max values, including edge-like large values, to exercise modulo and bounds.

Code: C++11 test suite for random_mod (no GTest)

*/

#include <vector>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Link against C implementation
extern "C" {
    // Focal method under test
    unsigned int random_mod(unsigned int max);
    // Dependency used inside random_mod
    unsigned int random_u16(void);
}

/*
Global test harness state
- g_failures counts non-terminating assertion failures
*/
static int g_failures = 0;

/*
Non-terminating assertion macro
- Does not abort the test; logs failure and continues
*/
#define TU_ASSERT(cond, note) do { \
    if(!(cond)) { \
        ++g_failures; \
        std::cerr << "Test failure: " << (note) \
                  << " (Condition: " #cond ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
} while(0)

/*
Test 1: max == 1 should always yield 0
- This asserts the boundary condition where modulo by 1 collapses to 0
*/
void test_random_mod_max_one() {
    // Expose the contract that modulo by 1 yields 0
    unsigned int max = 1;
    for (int i = 0; i < 1000; ++i) {
        unsigned int v = random_mod(max);
        TU_ASSERT(v == 0, "random_mod(1) must always return 0");
    }
}

/*
Test 2: random_mod(max) always returns a value in [0, max-1] for a variety of max
- Covers true path across multiple positive max values
*/
void test_random_mod_bounds_multiple_max() {
    std::vector<unsigned int> max_values = {
        2, 3, 4, 5, 8, 16, 32, 64, 128, 256, 512, 1024, 4096, 65536,
        1000U, 10000U, 123456U, 200000U
    };

    for (unsigned int max : max_values) {
        // Skip non-positive max to avoid undefined behavior; ensure max > 0
        if (max == 0) continue;
        for (int i = 0; i < 1000; ++i) {
            unsigned int v = random_mod(max);
            TU_ASSERT(v < max, "random_mod must return value in [0, max-1]");
        }
    }
}

/*
Test 3: Distribution sanity check for max == 2
- Ensure modulo behavior actually produces both possible outcomes across many samples
- This guards against pathological RNG biases that might yield only one option
*/
void test_random_mod_two_value_distribution() {
    const unsigned int max = 2;
    bool seen0 = false, seen1 = false;
    for (int i = 0; i < 10000; ++i) {
        unsigned int v = random_mod(max);
        if (v == 0) seen0 = true;
        else if (v == 1) seen1 = true;
        // Early exit if both seen to speed up
        if (seen0 && seen1) break;
    }
    TU_ASSERT(seen0 && seen1, "random_mod(2) should produce both 0 and 1 across samples");
}

/*
Test 4: Large max values do not cause out-of-range results and do not crash
- Ensures function handles larger moduli gracefully
*/
void test_random_mod_large_max_no_crash() {
    unsigned int max = 100000; // larger than 65535 to exercise modulo with large modulus
    for (int i = 0; i < 10000; ++i) {
        unsigned int v = random_mod(max);
        TU_ASSERT(v < max, "random_mod with large max should yield v < max");
    }
}

int main() {
    std::cout << "Starting test suite for random_mod (no GTest, C++11 harness)" << std::endl;

    // Run tests
    test_random_mod_max_one();
    test_random_mod_bounds_multiple_max();
    test_random_mod_two_value_distribution();
    test_random_mod_large_max_no_crash();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "Test suite completed with failures: " << g_failures << std::endl;
        return 1;
    }
}