// Candidate Keywords extracted from focal method and its context
// - __cyg_profile_func_exit
// - __cyg_profile_func_enter
// - print_debug
// - action_type (and its value EXIT)
// - EXIT (enum value)
// - this_fn, call_site (function pointers)
// - static (function-scope static declaration of print_debug)
// - get_last_debug_info (test-accessor for internal state)
// - instrument-functions.c (context)
// - ENTER (possible alternative action_type value)
// This test suite targets the focal function __cyg_profile_func_exit and its interaction with the internal static print_debug.

#include <string.h>
#include <bfd.h>
#include <cstddef>
#include <unistd.h>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>


// Domain knowledge notes applied in tests:
// - We simulate a minimal environment to compile in C++11 without GTest by providing a small test harness.
// - We expose a C linkage surface for the functions that would normally live in instrument-functions.c.
// - We cover: basic exit call, exit with null pointers, multiple invocations, and a complementary enter call.
// - We use non-terminating checks (tests print results and do not abort on failures).

// Provide C-compatible definitions and an accessor to inspect internal static state.
// This TU acts as both the implementation of the focal function (in lieu of the real file)
// and as the test harness to validate its behavior.

extern "C" {

// Expose a minimal action_type enum to mirror the focal code's usage.
typedef enum { ENTER = 0, EXIT = 1 } action_type;

// Forward declarations to simulate the instrumented hooks.
// In real code these would be linked from instrument-functions.c.
void __cyg_profile_func_enter(void *this_fn, void *call_site);
void __cyg_profile_func_exit(void *this_fn, void *call_site);

// Accessor to retrieve internal debug state (points and action) for assertions in tests.
// The implementation stores the last observed values from print_debug.
void get_last_debug_info(void** this_fn, void** call_site, action_type* action);

} // extern "C"

// Internal static state and helper (simulating the real file-scope static behavior).
namespace {

static void* g_last_this_fn = nullptr;
static void* g_last_call_site = nullptr;
static action_type g_last_action = ENTER;

// The internal debug print function from the focal code is represented here.
// It stores the last observed arguments into module-global state so tests can verify behavior.
static void print_debug(void *this_fn, void *call_site, action_type action)
{
    g_last_this_fn = this_fn;
    g_last_call_site = call_site;
    g_last_action = action;
}

} // anonymous namespace

extern "C" {

// Realistic (mocked) implementations of the focal entry/exit hooks.
// The EXIT path is what __cyg_profile_func_exit is expected to trigger.
void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
    // Mimic behavior by recording ENTER events.
    print_debug(this_fn, call_site, ENTER);
}

void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
    // The focal method calls print_debug with EXIT; preserve that contract.
    print_debug(this_fn, call_site, EXIT);
}

// Accessor implementation used by tests to read internal state.
void get_last_debug_info(void** this_fn, void** call_site, action_type* action)
{
    if (this_fn)   { *this_fn = g_last_this_fn; }
    if (call_site) { *call_site = g_last_call_site; }
    if (action)    { *action = g_last_action; }
}

} // extern "C"


// ------------------------
// Test Harness (no GTest)
// ------------------------

static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Simple non-terminating assertion macro for tests.
// It records failures and prints a message, but does not abort the program to maximize coverage.
#define TEST_ASSERT(cond, msg) do { \
    if (cond) { \
        ++g_tests_passed; \
        printf("[PASS] %s\n", msg); \
    } else { \
        ++g_tests_failed; \
        printf("[FAIL] %s\n", msg); \
    } \
} while(0)

// Convenience wrapper to fetch the latest debug info for assertions.
static void fetch_latest_debug(void** this_fn, void** call_site, action_type* action)
{
    get_last_debug_info(this_fn, call_site, action);
}

// Test 1: Basic EXIT path - ensure __cyg_profile_func_exit records EXIT with provided pointers.
static void test_exit_basic()
{
    // Arrange
    void* a = (void*)0x1000;
    void* b = (void*)0x2000;

    // Act
    __cyg_profile_func_exit(a, b);

    // Assert
    void* ta = nullptr;
    void* tb = nullptr;
    action_type act = ENTER; // default
    fetch_latest_debug(&ta, &tb, &act);

    TEST_ASSERT(ta == a, "exit_basic: this_fn should be the caller-provided this_fn");
    TEST_ASSERT(tb == b, "exit_basic: call_site should be the caller-provided call_site");
    TEST_ASSERT(act == EXIT, "exit_basic: action should be EXIT");
}

// Test 2: EXIT path with NULL pointers - ensure function does not crash and records EXIT with NULLs.
static void test_exit_nulls()
{
    // Act
    __cyg_profile_func_exit(nullptr, nullptr);

    // Assert
    void* ta = reinterpret_cast<void*>(0); // explicit zero for compare
    void* tb = reinterpret_cast<void*>(0);
    action_type act = ENTER;
    fetch_latest_debug(&ta, &tb, &act);

    TEST_ASSERT(ta == nullptr, "exit_nulls: this_fn should be NULL");
    TEST_ASSERT(tb == nullptr, "exit_nulls: call_site should be NULL");
    TEST_ASSERT(act == EXIT, "exit_nulls: action should be EXIT even for NULL inputs");
}

// Test 3: Multiple EXIT invocations - ensure state updates with the most recent call.
static void test_exit_multiple()
{
    // First call
    __cyg_profile_func_exit((void*)0xDEAD, (void*)0xBEEF);
    void* ta1 = nullptr;
    void* tb1 = nullptr;
    action_type act1 = ENTER;
    fetch_latest_debug(&ta1, &tb1, &act1);
    TEST_ASSERT(ta1 == (void*)0xDEAD, "exit_multiple: first call this_fn captured");
    TEST_ASSERT(tb1 == (void*)0xBEEF, "exit_multiple: first call call_site captured");
    TEST_ASSERT(act1 == EXIT, "exit_multiple: first call action should be EXIT");

    // Second call
    __cyg_profile_func_exit((void*)0x1234, (void*)0xABCD);
    void* ta2 = nullptr;
    void* tb2 = nullptr;
    action_type act2 = ENTER;
    fetch_latest_debug(&ta2, &tb2, &act2);
    TEST_ASSERT(ta2 == (void*)0x1234, "exit_multiple: second call this_fn updated");
    TEST_ASSERT(tb2 == (void*)0xABCD, "exit_multiple: second call call_site updated");
    TEST_ASSERT(act2 == EXIT, "exit_multiple: second call action should be EXIT");
}

// Test 4: Basic ENTER path to exercise companion code and ensure ENTER is recorded.
// Not strictly required by focal method, but increases coverage of the related static flow.
static void test_enter_basic()
{
    // Arrange
    void* a = (void*)0x1111;
    void* b = (void*)0x2222;

    // Act
    __cyg_profile_func_enter(a, b);

    // Assert
    void* ta = nullptr;
    void* tb = nullptr;
    action_type act = EXIT;
    fetch_latest_debug(&ta, &tb, &act);

    TEST_ASSERT(ta == a, "enter_basic: this_fn should be the caller-provided this_fn");
    TEST_ASSERT(tb == b, "enter_basic: call_site should be the caller-provided call_site");
    TEST_ASSERT(act == ENTER, "enter_basic: action should be ENTER");
}

// Runner
static void run_all_tests()
{
    printf("Starting unit tests for __cyg_profile_func_exit (and related hooks)\n");

    test_exit_basic();
    test_exit_nulls();
    test_exit_multiple();
    test_enter_basic();

    printf("Tests completed. Passed: %d, Failed: %d\n", g_tests_passed, g_tests_failed);
}

// Entry point
int main()
{
    run_all_tests();
    // Return non-zero if any test failed to indicate test result to CI systems.
    return (g_tests_failed == 0) ? 0 : 1;
}