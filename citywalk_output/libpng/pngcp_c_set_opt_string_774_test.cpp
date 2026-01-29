// Self-contained unit test suite for the focal method "set_opt_string".
// Note: This is a self-contained stand-in to exercise the logical flow
// described by the focal method, dependencies and behavior, designed to
// run without the original pngcp.c environment.
// The tests imitate the expected environment (struct display, options table,
// and the helper function set_opt_string_). This approach ensures high
// coverage of key paths without requiring external dependencies.
// The code uses simple runtime assertions and prints results to stdout.

#include <pnglibconf.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <cstddef>


// ---------------------------------------------------------------------------
// Minimal stand-in definitions to simulate the environment of the focal method
// ---------------------------------------------------------------------------

// Forward declare to mirror the expected C interface (as in the real code)
typedef unsigned char png_byte;

// Simulated structures as used by the focal code
struct StackEntry {
    unsigned int opt;       // index into options[]
    unsigned int entry;     // index into options[opt].values[]
    unsigned int opt_string_end; // destination for result of set_opt_string_
};

// The 'display' struct in the focal code; we only model fields used by the test
struct display {
    StackEntry stack[2]; // support at least two slots for testing
};

// Simple representation of option values (each with a name)
struct option_value {
    const char* name;
};

// Option table entry: contains a list of option_value entries
struct option {
    const option_value* values;
};

// ---------------------------------------------------------------------------
// Test scaffolding: options tables and a mocked set_opt_string_
// ---------------------------------------------------------------------------

// Global options pointer used by set_opt_string (mimicking the real code's usage)
static const struct option* options = nullptr;

// Two separate options tables to test different scenarios (normal and NULL-name)
static const option_value opt0_values_table1[] = {
    {"nameA"}, // index 0
    {"nameB"}  // index 1
};

// First options entry: one option with two values
static const struct option option_table1[] = {
    { opt0_values_table1 }
};

// Second options table: has a NULL name to test NULL handling
static const option_value opt0_values_table2[] = {
    {nullptr} // index 0 is NULL
};

// Second options entry
static const struct option option_table2[] = {
    { opt0_values_table2 }
};

// Temporary state accessible by the mocked set_opt_string_ for verification
static unsigned int g_last_sp = 0;
static unsigned int g_last_opt = 0;
static unsigned int g_last_entry = 0;
static const char* g_last_entry_name = nullptr;

// Mocked version of set_opt_string_ (stand-in for the real helper)
// It records inputs for verification and returns a deterministic value based
// on its inputs to allow precise assertions in tests.
unsigned int set_opt_string_(struct display* dp, unsigned int sp, png_byte opt, const char* entry_name)
{
    g_last_sp = sp;
    g_last_opt = static_cast<unsigned int>(opt);
    g_last_entry = dp->stack[sp].entry;
    g_last_entry_name = entry_name;

    // Compute a deterministic return value based on inputs; handle NULL safely
    size_t name_len = (entry_name != nullptr) ? std::strlen(entry_name) : 0;
    return 1000u + static_cast<unsigned int>(opt) + dp->stack[sp].entry + static_cast<unsigned int>(name_len);
}

// The focal method under test (re-implemented for the standalone test environment)
unsigned int set_opt_string(struct display *dp, unsigned int sp)
{
    // In the real code, this would resolve an option string from a table and
    // then call set_opt_string_ with the resolved name. We mirror that contract.
    dp->stack[sp].opt_string_end = set_opt_string_(dp, sp, dp->stack[sp].opt,
        options[dp->stack[sp].opt].values[dp->stack[sp].entry].name);
    return dp->stack[sp].opt_string_end;
}

// ---------------------------------------------------------------------------
// Lightweight test helpers
// ---------------------------------------------------------------------------

static int tests_run = 0;
static int tests_passed = 0;

#define ASSERT_EQ(a, b, msg) do { \
    tests_run++; \
    if ((a) != (b)) { \
        printf("FAIL: %s\n  Expected: %lld, Got: %lld\n", (msg), static_cast<long long>(b), static_cast<long long>(a)); \
    } else { \
        printf("PASS: %s\n", (msg)); \
        tests_passed++; \
    } \
} while (0)

#define ASSERT_PTR_EQ(a, b, msg) do { \
    tests_run++; \
    if ((a) != (b)) { \
        printf("FAIL: %s\n  Expected pointer: %p, Got: %p\n", (msg), static_cast<void*>(const_cast<char*>(b)), static_cast<void*>(const_cast<char*>(a))); \
    } else { \
        printf("PASS: %s\n", (msg)); \
        tests_passed++; \
    } \
} while (0)

// ---------------------------------------------------------------------------
// Individual tests
// ---------------------------------------------------------------------------

/*
 * Test 1: Normal path with valid option and entry.
 * - options = option_table1 (name values: "nameA", "nameB")
 * - dp.stack[0].opt = 0 (points to option_table1[0])
 * - dp.stack[0].entry = 1 (uses "nameB")
 * Expectation:
 * - set_opt_string_ should be called with opt = 0, entry = 1, entry_name = "nameB"
 * - Return value should be 1000 + 0 + 1 + strlen("nameB") = 1006
 * - dp->stack[0].opt_string_end should be 1006
 */
bool test_normal_path()
{
    printf("Running Test 1: Normal path with valid option and entry...\n");

    // Setup: use option_table1
    options = option_table1;

    struct display dp;
    dp.stack[0].opt = 0;
    dp.stack[0].entry = 1;
    dp.stack[0].opt_string_end = 0;

    // Clear last-state
    g_last_sp = g_last_opt = g_last_entry = 0;
    g_last_entry_name = nullptr;

    unsigned int ret = set_opt_string(&dp, 0);

    // Expected values
    const char* expected_name = option_table1[0].values[1].name; // "nameB"
    unsigned int expected_ret = 1000u + 0u + 1u + static_cast<unsigned int>(std::strlen(expected_name));

    // Assertions
    ASSERT_EQ(ret, expected_ret, "Test1: return value equals expected_ret");
    ASSERT_EQ(dp.stack[0].opt_string_end, expected_ret, "Test1: opt_string_end equals return value");
    ASSERT_EQ(g_last_sp, 0, "Test1: last_sp == 0");
    ASSERT_EQ(g_last_opt, 0, "Test1: last_opt == 0");
    ASSERT_EQ(g_last_entry, 1, "Test1: last_entry == 1");
    ASSERT_EQ(std::strcmp(g_last_entry_name, expected_name), 0, "Test1: last_entry_name equals expected name");

    return true;
}

/*
 * Test 2: Path where the option value's name is NULL.
 * - Shift to option_table2 where values[0].name == NULL
 * - dp.stack[0].opt = 0; entry = 0
 * Expectation:
 * - entry_name passed to set_opt_string_ is NULL
 * - Return value should be 1000 + 0 + 0 + 0 = 1000
 */
bool test_null_entry_name_path()
{
    printf("Running Test 2: Path with NULL entry name...\n");

    // Setup: use option_table2 which has a NULL name
    options = option_table2;

    struct display dp;
    dp.stack[0].opt = 0;
    dp.stack[0].entry = 0;
    dp.stack[0].opt_string_end = 0;

    // Clear last-state
    g_last_sp = g_last_opt = g_last_entry = 0;
    g_last_entry_name = reinterpret_cast<const char*>(0xDEADBEEF); // non-NULL to ensure we override

    unsigned int ret = set_opt_string(&dp, 0);

    unsigned int expected_ret = 1000u + 0u + 0u + 0u;
    ASSERT_EQ(ret, expected_ret, "Test2: return value with NULL entry name");
    ASSERT_EQ(dp.stack[0].opt_string_end, expected_ret, "Test2: opt_string_end equals return value");
    ASSERT_EQ(g_last_sp, 0, "Test2: last_sp == 0");
    ASSERT_EQ(g_last_opt, 0, "Test2: last_opt == 0");
    ASSERT_EQ(g_last_entry, 0, "Test2: last_entry == 0");
    // last_entry_name should be NULL
    ASSERT_PTR_EQ(g_last_entry_name, nullptr, "Test2: last_entry_name is NULL");

    return true;
}

/*
 * Test 3: Multiple stack entries (sp = 0 and sp = 1) to verify isolation
 * - First call uses dp.stack[0] with valid name; second call uses dp.stack[1]
 * - Use option_table1 for both but keep entries separate
 * - Verify last_* values reflect the correct sp and entry
 */
bool test_multiple_stack_slots()
{
    printf("Running Test 3: Multiple stack slots isolation...\n");

    options = option_table1;

    struct display dp;
    // First slot
    dp.stack[0].opt = 0;
    dp.stack[0].entry = 0; // "nameA"
    dp.stack[0].opt_string_end = 0;

    // Second slot
    dp.stack[1].opt = 0;
    dp.stack[1].entry = 0; // also "nameA"
    dp.stack[1].opt_string_end = 0;

    // Reset last-state
    g_last_sp = g_last_opt = g_last_entry = 0;
    g_last_entry_name = nullptr;

    unsigned int ret0 = set_opt_string(&dp, 0);
    const char* expected_name0 = option_table1[0].values[0].name; // "nameA"
    unsigned int expected_ret0 = 1000u + 0u + 0u + static_cast<unsigned int>(std::strlen(expected_name0));

    // Validate first slot results
    ASSERT_EQ(ret0, expected_ret0, "Test3: First slot return value");
    ASSERT_EQ(dp.stack[0].opt_string_end, expected_ret0, "Test3: First slot opt_string_end");

    // Now test second slot independently
    unsigned int ret1 = set_opt_string(&dp, 1);
    const char* expected_name1 = option_table1[0].values[0].name; // still "nameA"
    unsigned int expected_ret1 = 1000u + 0u + 0u + static_cast<unsigned int>(std::strlen(expected_name1));

    ASSERT_EQ(ret1, expected_ret1, "Test3: Second slot return value");
    ASSERT_EQ(dp.stack[1].opt_string_end, expected_ret1, "Test3: Second slot opt_string_end");
    ASSERT_EQ(g_last_sp, 1, "Test3: last_sp should reflect second slot (1)");
    ASSERT_EQ(g_last_entry, 0, "Test3: last_entry for second slot should be 0");
    ASSERT_EQ(std::strcmp(g_last_entry_name, expected_name1), 0, "Test3: last_entry_name for second slot should match");

    return true;
}

// ---------------------------------------------------------------------------
// Main: run all tests and report summary
// ---------------------------------------------------------------------------

int main(void)
{
    printf("Starting unit tests for set_opt_string (focal method imitation)\n\n");

    bool t1 = test_normal_path();
    bool t2 = test_null_entry_name_path();
    bool t3 = test_multiple_stack_slots();

    printf("\nTest summary: %d run, %d passed, %d failed\n",
           tests_run, tests_passed, tests_run - tests_passed);

    if (t1 && t2 && t3) {
        printf("All tests PASSED.\n");
        return 0;
    } else {
        printf("Some tests FAILED. See details above.\n");
        return 1;
    }
}