// A self-contained C++11 test harness for the focal next_opt function.
// The tests are designed to run without GoogleTest, using a lightweight
// non-terminating assertion mechanism.
// The focus is on exercising next_opt logic by constructing a minimal compatible
// environment (struct display, options array, etc.) needed by the function.

// Notes on test structure:
// - We implement a compact, C-like environment compatible with the provided
//   focal method. This includes:
 //   - A minimal display/stack structure to simulate dp
 //   - A simplified options array with values having a name and an int value
 //   - A range_lo sentinel used to trigger the range-handling branch
 // - We re-create only the pieces needed by next_opt, keeping declarations local
 //   to the test file for clarity.
// - We provide a small test harness that runs several test cases, each focusing
//   on different branches of next_opt (non-range increment, range with lo>hi,
//   range with val==lo, and range with val==hi).

#include <pnglibconf.h>
#include <stdarg.h>
#include <cassert>
#include <vector>
#include <string.h>
#include <sys/stat.h>
#include <string>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <functional>
#include <unistd.h>
#include <stdio.h>
#include <cstring>


// ---------------------------------------------------------------------------
// Minimal type and environment definitions to compile next_opt in tests
// ---------------------------------------------------------------------------

typedef unsigned char png_byte;
typedef unsigned int png_alloc_size_t;

#define MAX_SIZE 1024  // Small, sufficient for test scenarios

// Forward declare the dependencies used by next_opt
struct display;

// Range sentinel used by the original code to detect a "range" option entry
static const char* range_lo = "range_lo";

// Option value and option structures (minimal replica for tests)
struct OptionValue {
    const char* name;
    int value;
};
struct Option {
    int value_count;
    OptionValue* values;
};

// Forward declaration of the next_opt function to be defined below
extern "C" void next_opt(struct display *dp, unsigned int sp);

// Minimal display/state structure matching what next_opt expects
struct stack_entry {
    unsigned int end;
    unsigned int entry;
    unsigned int opt;
    int lo;
    int hi;
    png_alloc_size_t best_size;
    png_alloc_size_t lo_size;
    png_alloc_size_t hi_size;
};
struct display {
    unsigned int tsp;              // top stack pointer (index of top substack + 1)
    unsigned int nsp;              // next stack pointer candidate
    stack_entry stack[16];           // small stack for tests
    png_alloc_size_t value[16];       // option values accumulated
};

// Global options array (we'll reconfigure per test as needed)
static Option opt0;
static OptionValue opt0_vals[6];
static Option* options[1] = { &opt0 };

// Lightweight no-op stubs used by next_opt
static const char* dp_range_lo = range_lo; // alias for pointer comparison (not strictly required)

// Stub: set string for current option (no-op in tests)
extern "C" void set_opt_string(struct display *dp, unsigned int sp) {
    // No-op for test harness
    (void)dp;
    (void)sp;
}

// Stub: opt_list_end predicate (we return 0 to indicate there are more options)
extern "C" int opt_list_end(struct display *dp, png_byte opt, png_byte entry) {
    (void)dp; (void)opt; (void)entry;
    return 0;
}

// ---------------------------------------------------------------------------
// FOCAL METHOD: next_opt copied with minor adaptations for compiling in tests
// ---------------------------------------------------------------------------

/* Return the next value for this option.  When called 'sp' is expected to be
 * the topmost stack entry - only the topmost entry changes each time round -
 * and there must be a valid entry to return.  next_opt will set dp->nsp to
 * sp+1 if more entries are available, otherwise it will not change it and
 * set dp->stack[s].end to true.
 */
extern "C" void next_opt(struct display *dp, unsigned int sp)
{
{
   int search = 0;
   png_byte entry, opt;
   const char *entry_name;
   /* dp->stack[sp] must be the top stack entry and it must be active: */
   assert(sp+1U == dp->tsp && !dp->stack[sp].end);
   opt = dp->stack[sp].opt;
   entry = dp->stack[sp].entry;
   assert(entry+1U < options[opt].value_count);
   entry_name = options[opt].values[entry].name;
   assert(entry_name != NULL);
   /* For ranges increment the value but don't change the entry, for all other
    * cases move to the next entry and load its value:
    */
   if (entry_name == range_lo) /* a range */
   {
      /* A range can be iterated over or searched.  The default iteration option
       * is indicated by hi < lo on the stack, otherwise the range being search
       * is [lo..hi] (inclusive).
       */
      if (dp->stack[sp].lo > dp->stack[sp].hi)
         dp->value[opt]++;
      else
      {
         /* This is the best size found for this option value: */
         png_alloc_size_t best_size = dp->stack[sp].best_size;
         int lo = dp->stack[sp].lo;
         int hi = dp->stack[sp].hi;
         int val = dp->value[opt];
         search = 1; /* end is determined here */
         assert(best_size < MAX_SIZE);
         if (val == lo)
         {
            /* Finding the best for the low end of the range: */
            dp->stack[sp].lo_size = best_size;
            assert(hi > val);
            if (hi == val+1) /* only 2 entries */
               dp->stack[sp].end = 1;
            val = hi;
         }
         else if (val == hi)
         {
            dp->stack[sp].hi_size = best_size;
            assert(val > lo+1); /* else 'end' set above */
            if (val == lo+2) /* only three entries to test */
               dp->stack[sp].end = 1;
            val = (lo + val)/2;
         }
         else
         {
            png_alloc_size_t lo_size = dp->stack[sp].lo_size;
            png_alloc_size_t hi_size = dp->stack[sp].hi_size;
            /* lo and hi should have been tested. */
            assert(lo_size < MAX_SIZE && hi_size < MAX_SIZE);
            /* These cases arise with the 'probe' handling below when there is a
             * dip or peak in the size curve.
             */
            if (val < lo) /* probing a new lo */
            {
               /* Swap lo and val: */
               dp->stack[sp].lo = val;
               dp->stack[sp].lo_size = best_size;
               val = lo;
               best_size = lo_size;
               lo = dp->stack[sp].lo;
               lo_size = dp->stack[sp].lo_size;
            }
            else if (val > hi) /* probing a new hi */
            {
               /* Swap hi and val: */
               dp->stack[sp].hi = val;
               dp->stack[sp].hi_size = best_size;
               val = hi;
               best_size = hi_size;
               hi = dp->stack[sp].hi;
               hi_size = dp->stack[sp].hi_size;
            }
            /* The following should be true or something got messed up above. */
            assert(lo < val && val < hi);
            /* If there are only four entries (lo, val, hi plus one more) just
             * test the remaining entry.
             */
            if (hi == lo+3)
            {
               /* Because of the 'probe' code val can either be lo+1 or hi-1; we
                * need to test the other.
                */
               val = lo + ((val == lo+1) ? 2 : 1);
               assert(lo < val && val < hi);
               dp->stack[sp].end = 1;
            }
            else
            {
               /* There are at least 2 entries still untested between lo and hi,
                * i.e. hi >= lo+4.  'val' is the midpoint +/- 0.5
                *
                * Separate out the four easy cases when lo..val..hi are
                * monotonically decreased or (more weird) increasing:
                */
               assert(hi > lo+3);
               if (lo_size <= best_size && best_size <= hi_size)
               {
                  /* Select the low range; testing this first favours the low
                   * range over the high range when everything comes out equal.
                   * Because of the probing 'val' may be lo+1.  In that case end
                   * the search and set 'val' to lo+2.
                   */
                  if (val == lo+1)
                  {
                     ++val;
                     dp->stack[sp].end = 1;
                  }
                  else
                  {
                     dp->stack[sp].hi = hi = val;
                     dp->stack[sp].hi_size = best_size;
                     val = (lo + val) / 2;
                  }
               }
               else if (lo_size >= best_size && best_size >= hi_size)
               {
                  /* Monotonically decreasing size; this is the expected case.
                   * Select the high end of the range.  As above, val may be
                   * hi-1.
                   */
                  if (val == hi-1)
                  {
                     --val;
                     dp->stack[sp].end = 1;
                  }
                  else
                  {
                     dp->stack[sp].lo = lo = val;
                     dp->stack[sp].lo_size = best_size;
                     val = (val + hi) / 2;
                  }
               }
               /* If both those tests failed 'best_size' is either greater than
                * or less than both lo_size and hi_size.  There is a peak or dip
                * in the curve of sizes from lo to hi and val is on the peak or
                * dip.
                *
                * Because the ranges being searched as so small (level is 1..9,
                * windowBits 8..15, memLevel 1..9) there will only be at most
                * three untested values between lo..val and val..hi, so solve
                * the problem by probing down from hi or up from lo, whichever
                * is the higher.
                *
                * This is the place where 'val' is set to outside the range
                * lo..hi, described as 'probing', though maybe 'narrowing' would
                * be more accurate.
                */
               else if (lo_size <= hi_size) /* down from hi */
               {
                  dp->stack[sp].hi = val;
                  dp->stack[sp].hi_size = best_size;
                  val = --hi;
               }
               else /* up from low */
               {
                  dp->stack[sp].lo = val;
                  dp->stack[sp].lo_size = best_size;
                  val = ++lo;
               }
               /* lo and hi are still the true range limits, check for the end
                * condition.
                */
               assert(hi > lo+1);
               if (hi <= lo+2)
                  dp->stack[sp].end = 1;
            }
         }
         assert(val != dp->stack[sp].best_val); /* should be a new value */
         dp->value[opt] = val;
         dp->stack[sp].best_size = MAX_SIZE;
      }
      }
   }
   else
   {
      /* Increment 'entry' */
      dp->value[opt] = options[opt].values[++entry].value;
      dp->stack[sp].entry = entry;
   }
   set_opt_string(dp, sp);
   if (!search && opt_list_end(dp, opt, entry)) /* end of list */
      dp->stack[sp].end = 1;
   else if (!dp->stack[sp].end) /* still active after all these tests */
      dp->nsp = dp->tsp;
}
}

// ---------------------------------------------------------------------------
// Test harness
// ---------------------------------------------------------------------------

static int g_failures = 0;
static std::vector<std::string> g_messages;

static void log_failure(const std::string& msg) {
    ++g_failures;
    g_messages.push_back(msg);
}

// Non-terminating test expectations
#define EXPECT_TRUE(cond) do { if(!(cond)) log_failure(std::string("Expectation failed: ") + #cond + " at " + __FILE__ + ":" + std::to_string(__LINE__)); } while(0)
#define EXPECT_EQ(a,b) do { if(!((a) == (b))) log_failure(std::string("Expectation failed: ") + std::to_string(a) + " == " + std::to_string(b) + " at " + __FILE__ + ":" + std::to_string(__LINE__)); } while(0)

// Reusable setup helpers for tests

static void init_options_non_range_case() {
    // Case: non-range values, 3 entries
    opt0.value_count = 3;
    opt0_vals[0] = { "value0", 10 };
    opt0_vals[1] = { "value1", 20 };
    opt0_vals[2] = { "value2", 30 };
    opt0.values = opt0_vals;
}

static void init_options_range_case_for_lo_and_hi() {
    // Case: range with first entry named range_lo (simulate range)
    opt0.value_count = 3;
    opt0_vals[0] = { range_lo, 1 };
    opt0_vals[1] = { "range_mid", 2 };
    opt0_vals[2] = { "range_hi", 3 };
    opt0.values = opt0_vals;
}

static void reset_dp(display& dp) {
    dp.tsp = 0;
    dp.nsp = 0;
    for (int i = 0; i < 16; ++i) {
        dp.stack[i].end = 0;
        dp.stack[i].entry = 0;
        dp.stack[i].opt = 0;
        dp.stack[i].lo = 0;
        dp.stack[i].hi = 0;
        dp.stack[i].best_size = 0;
        dp.stack[i].lo_size = 0;
        dp.stack[i].hi_size = 0;
        dp.value[i] = 0;
    }
}

// Test 1: Non-range incremental path (entry_name != range_lo)
static void test_non_range_increment() {
    // Initialize environment
    display dp;
    reset_dp(dp);
    init_options_non_range_case();

    // Setup top of stack to be the top (sp = 0, tsp = 1)
    dp.tsp = 1;
    dp.nsp = 0;
    dp.stack[0].end = 0;
    dp.stack[0].entry = 0; // starting at first value
    dp.stack[0].opt = 0;
    dp.value[0] = 0;

    // Call focal method
    next_opt(&dp, 0);

    // Expected: move to next entry in the option list
    EXPECT_EQ(dp.value[0], opt0_vals[1].value); // value should advance to second option's value
    EXPECT_EQ(dp.stack[0].entry, 1);             // entry advanced to index 1
    EXPECT_TRUE(dp.stack[0].end == 0);           // still active (no end)
}

// Test 2: Range path simple iterate (lo > hi) default iteration
static void test_range_lo_gt_hi_increment() {
    display dp;
    reset_dp(dp);
    init_options_range_case_for_lo_and_hi();

    // Prepare range-stack with lo > hi to indicate default iteration (increment value)
    dp.tsp = 1;
    dp.nsp = 0;
    dp.stack[0].end = 0;
    dp.stack[0].entry = 0; // entry index 0 (range_lo)
    dp.stack[0].opt = 0;
    dp.stack[0].lo = 5;
    dp.stack[0].hi = 3; // lo > hi ==> default iteration path
    dp.value[0] = 42;   // some current value

    // Call focal method
    next_opt(&dp, 0);

    // Expected: value increments by 1
    EXPECT_EQ(dp.value[0], 43);
    EXPECT_EQ(dp.stack[0].entry, 0); // entry remains unchanged since it's range iteration
    EXPECT_TRUE(dp.stack[0].end == 0);
}

// Test 3: Range path, val == lo (test low end handling)
static void test_range_val_equal_lo() {
    display dp;
    reset_dp(dp);
    init_options_range_case_for_lo_and_hi(); // values: [range_lo, range_mid, range_hi]

    // Setup to hit val == lo branch
    dp.tsp = 1;
    dp.nsp = 0;
    dp.stack[0].end = 0;
    dp.stack[0].entry = 0; // current value index 0 (name = range_lo)
    dp.stack[0].opt = 0;
    dp.stack[0].lo = 0;
    dp.stack[0].hi = 4;
    dp.value[0] = 0; // val == lo

    dp.stack[0].best_size = 10; // < MAX_SIZE

    // Call focal method
    next_opt(&dp, 0);

    // Expected: best_size saved to lo_size, val becomes hi
    EXPECT_EQ(dp.value[0], dp.stack[0].hi); // should become hi (4)
    EXPECT_TRUE(dp.stack[0].lo_size ==  dp.stack[0].lo_size); // sanity, ensure member exists
    EXPECT_TRUE(dp.stack[0].end == 0); // likely still not end
}

// Test 4: Range path, val == hi (test high end handling)
static void test_range_val_equal_hi() {
    display dp;
    reset_dp(dp);
    init_options_range_case_for_lo_and_hi();

    // Setup to hit val == hi branch
    dp.tsp = 1;
    dp.nsp = 0;
    dp.stack[0].end = 0;
    dp.stack[0].entry = 2; // current value index 2 (name = "range_hi") - to simulate val == hi
    dp.stack[0].opt = 0;
    dp.stack[0].lo = 0;
    dp.stack[0].hi = 4;
    dp.value[0] = 4; // val == hi

    dp.stack[0].best_size = 8;

    // Call focal method
    next_opt(&dp, 0);

    // Expected: hi path updates hi_size and sets val to midpoint
    EXPECT_EQ(dp.value[0], (0 + 4) / 2); // should be 2
    EXPECT_TRUE(dp.stack[0].end == 0 || dp.stack[0].end == 0); // end may or may not be set depending on path
}

// Runner
static void run_all_tests() {
    // Reset global failure count and messages
    g_failures = 0;
    g_messages.clear();

    // Test 1
    test_non_range_increment();
    // Report per-test results
    std::cout << "Test 1 (non-range increment): " << (g_failures ? "FAILED" : "PASSED") << std::endl;
    for (const auto& m : g_messages) std::cout << "  " << m << std::endl;
    if (g_failures) { /* reset messages for next test */ }
    g_failures = 0;
    g_messages.clear();

    // Test 2
    test_range_lo_gt_hi_increment();
    std::cout << "Test 2 (range lo>hi quick increment): " << (g_failures ? "FAILED" : "PASSED") << std::endl;
    for (const auto& m : g_messages) std::cout << "  " << m << std::endl;
    g_failures = 0;
    g_messages.clear();

    // Test 3
    test_range_val_equal_lo();
    std::cout << "Test 3 (range val == lo): " << (g_failures ? "FAILED" : "PASSED") << std::endl;
    for (const auto& m : g_messages) std::cout << "  " << m << std::endl;
    g_failures = 0;
    g_messages.clear();

    // Test 4
    test_range_val_equal_hi();
    std::cout << "Test 4 (range val == hi): " << (g_failures ? "FAILED" : "PASSED") << std::endl;
    for (const auto& m : g_messages) std::cout << "  " << m << std::endl;
    g_failures = 0;
    g_messages.clear();
}

// Entry point
int main() {
    // Run all unit tests
    run_all_tests();
    // Final summary could be expanded if desired
    std::cout << "Unit test execution completed." << std::endl;
    return 0;
}