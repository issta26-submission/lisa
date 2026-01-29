// Self-contained C++11 unit tests for the focal method logic (choose_random_filter).
// Note: This test suite is a self-contained, minimal, mock-based reproduction
// of the core branching logic of the original function to facilitate high-coverage testing
// without depending on libpng internals or external RNG behavior.
// The goal is to exercise true/false branches of condition predicates in a deterministic way.

#include <cstdint>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// -----------------------------------------------------------------------------
// Mocked environment to test the core logic of choose_random_filter
// -----------------------------------------------------------------------------

// Define the filter bitmasks (simplified stand-ins for the real PNG constants)
constexpr uint32_t PNG_FILTER_NONE = 0;
constexpr uint32_t PNG_FILTER_SUB  = 1;  // 0b00001
constexpr uint32_t PNG_FILTER_UP   = 2;  // 0b00010
constexpr uint32_t PNG_FILTER_AVG  = 4;  // 0b00100
constexpr uint32_t PNG_FILTER_PAETH = 8; // 0b01000
// All allowed filters (as a bitmask)
constexpr uint32_t PNG_ALL_FILTERS = PNG_FILTER_NONE | PNG_FILTER_SUB | PNG_FILTER_UP
                                   | PNG_FILTER_AVG | PNG_FILTER_PAETH;

// Forward declaration of the function under test (in the real project this is in pngvalid.c)
void png_set_filter(void* pp, int method, uint32_t filters);

// In the real project, random_mod is a function that returns an unsigned int
// in the range [0, max). We expose a testable hook to control the output deterministically.
static std::function<uint32_t(uint32_t)> g_random_mod = [](uint32_t max) -> uint32_t {
    // Default pseudo-random deterministic seed for tests (not used in tests that override it)
    static uint32_t seed = 0xDEADBEEF;
    seed = static_cast<uint32_t>((seed * 1664525u + 1013904223u) % 0xFFFFFFFFu);
    // Return a value in [0, max)
    return seed % max;
};

// A tiny fake "png_struct" to pass through to the mock png_set_filter.
// We only care about tracking the last filters value that would be set by the focal function.
struct FakePNG {
    uint32_t last_filters = 0;
    bool   called = false;
};

// Mock implementation of png_set_filter used by tests.
// It simply records the filters argument and marks that a call occurred.
void png_set_filter(void* pp, int /*method*/, uint32_t filters)
{
    if (pp) {
        FakePNG* f = static_cast<FakePNG*>(pp);
        f->last_filters = filters;
        f->called = true;
    }
}

// The actual focal-like function under test (reproduced here to be self-contained for unit tests).
// It mirrors the logic in the provided <FOCAL_METHOD> choose_random_filter(...) implementation.
void choose_random_filter(FakePNG* pp, int start)
{
    uint32_t filters = PNG_ALL_FILTERS & g_random_mod(256U);
    if (filters != 0) {
        if (start && filters < PNG_FILTER_UP)
            filters |= PNG_FILTER_UP;
        png_set_filter(pp, 0 /*method*/, filters);
    }
}

// -----------------------------------------------------------------------------
// Lightweight test framework (minimal, no external libs required)
// -----------------------------------------------------------------------------

#define TEST_ASSERT(cond, msg)                                      \
    do { if (!(cond)) {                                         \
            std::cerr << "Test failed: " << (msg) << "\n";     \
            return false;                                       \
        }                                                       \
    } while (0)

struct TestCase {
    const char* name;
    std::function<bool()> func;
};

// Simple helper to print test result summaries
static void print_summary(int total, int passed)
{
    std::cout << "\nTest summary: " << passed << " / " << total << " tests passed.\n";
}

// -----------------------------------------------------------------------------
// Unit tests for choose_random_filter logic
// Comments describe the intent and what branch/condition is covered.
// -----------------------------------------------------------------------------

// Test 1: When random_mod returns 0, no call to png_set_filter should be made.
// This covers the "if (filters != 0)" false branch (no operation).
bool test_no_filter_when_random_zero()
{
    FakePNG pp;
    // Set deterministic RNG to return 0
    g_random_mod = [](uint32_t max) -> uint32_t { (void)max; return 0u; };

    choose_random_filter(&pp, 0);

    TEST_ASSERT(!pp.called, "png_set_filter should not be called when random_mod returns 0");
    return true;
}

// Test 2: When random_mod returns a non-zero value, png_set_filter should be called
// without forcing PNG_FILTER_UP (start = 0 means no mandatory UP bit addition).
// This covers the general path where the filter mask is non-zero and start is 0.
bool test_filter_set_without_forced_up_start0()
{
    FakePNG pp;
    // Return a value that is exactly PNG_FILTER_SUB (non-zero)
    g_random_mod = [](uint32_t max) -> uint32_t { (void)max; return PNG_FILTER_SUB; };

    choose_random_filter(&pp, 0);

    TEST_ASSERT(pp.called, "png_set_filter should be called when random_mod returns non-zero");
    TEST_ASSERT(pp.last_filters == PNG_FILTER_SUB, "last_filters should equal PNG_FILTER_SUB");
    return true;
}

// Test 3: When random_mod returns a value less than PNG_FILTER_UP and start is true,
// the function should OR PNG_FILTER_UP to ensure at least one UP filter is present.
// This tests the precise branch where (start && filters < PNG_FILTER_UP) is true.
bool test_force_up_bit_when_start_true_and_low_filter()
{
    FakePNG pp;
    // Return a value that is PNG_FILTER_SUB (1), which is less than PNG_FILTER_UP (2)
    g_random_mod = [](uint32_t max) -> uint32_t { (void)max; return PNG_FILTER_SUB; };

    choose_random_filter(&pp, 1);

    // filters should be 1 (SUB) OR 2 (UP) -> 3
    TEST_ASSERT(pp.called, "png_set_filter should be called when non-zero and start=1");
    TEST_ASSERT(pp.last_filters == (PNG_FILTER_SUB | PNG_FILTER_UP),
                "last_filters should equal SUB | UP (0b11 -> 3)");
    return true;
}

// Test 4: When random_mod returns a value that already includes UP or higher bits,
// with start = 1, we should not drastically alter the mask beyond the OR
// (only OR occurs when necessary). This exercises the non-forced-up path.
bool test_no_forced_up_when_value_includes_up()
{
    FakePNG pp;
    // Return a value that already has UP bit set
    g_random_mod = [](uint32_t max) -> uint32_t { (void)max; return PNG_FILTER_UP; };

    choose_random_filter(&pp, 1);

    TEST_ASSERT(pp.called, "png_set_filter should be called when random_mod returns non-zero including UP bit");
    TEST_ASSERT(pp.last_filters == PNG_FILTER_UP, "last_filters should equal PNG_FILTER_UP (no extra ORed bit)");
    return true;
}

// -----------------------------------------------------------------------------
// Main: run all tests and print results
// -----------------------------------------------------------------------------

int main()
{
    TestCase tests[] = {
        {"test_no_filter_when_random_zero", test_no_filter_when_random_zero},
        {"test_filter_set_without_forced_up_start0", test_filter_set_without_forced_up_start0},
        {"test_force_up_bit_when_start_true_and_low_filter", test_force_up_bit_when_start_true_and_low_filter},
        {"test_no_forced_up_when_value_includes_up", test_no_forced_up_when_value_includes_up}
    };

    int total = sizeof(tests) / sizeof(tests[0]);
    int passed = 0;

    std::cout << "Running unit tests for choose_random_filter (self-contained mock):\n";

    for (const auto& tc : tests) {
        bool ok = tc.func();
        std::cout << " - " << tc.name << ": " << (ok ? "PASSED" : "FAILED") << "\n";
        if (ok) ++passed;
    }

    print_summary(total, passed);
    return (passed == total) ? 0 : 1;
}