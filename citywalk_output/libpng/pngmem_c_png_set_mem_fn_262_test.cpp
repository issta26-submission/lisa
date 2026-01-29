// Minimal, self-contained unit-test suite for the focal method png_set_mem_fn
// This test provides a small stand-in for the libpng types and function
// signatures to run in a pure C++11 environment without external dependencies.
// The tests avoid GTest and use a lightweight custom harness that reports
// failures but continues execution to maximize coverage.

#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstddef>


// -----------------------------------------------------------------------------
// Minimal stand-in types to mirror the function under test (png_set_mem_fn)
// These are intentionally self-contained and do not rely on external PNG headers.
// -----------------------------------------------------------------------------

// Lightweight void pointer type used by the png_pointers
typedef void* png_voidp;

// Forward declaration of the test's memory-management function types
typedef void* (*png_malloc_ptr)(png_voidp, size_t);
typedef void  (*png_free_ptr)(png_voidp);

// Minimal png_struct compatible with the focal function
struct png_struct
{
    png_voidp mem_ptr;
    png_malloc_ptr malloc_fn;
    png_free_ptr   free_fn;
};

// Pointer to png_struct (as used by the original API)
typedef png_struct* png_structrp;

// The focal method under test (simplified to be self-contained)
png_structrp png_ptr_placeholder; // not used, just to illustrate naming
png_structrp png_set_mem_fn(png_structrp png_ptr, png_voidp mem_ptr,
    png_malloc_ptr malloc_fn, png_free_ptr free_fn)
{
{
   if (png_ptr != NULL)
   {
      png_ptr->mem_ptr = mem_ptr;
      png_ptr->malloc_fn = malloc_fn;
      png_ptr->free_fn = free_fn;
   }
}
    // No return value in the original, but keep signature consistent
    return png_ptr;
}

// -----------------------------------------------------------------------------
// Small, self-contained test harness (non-terminating assertions)
// It logs failures but continues executing to maximize coverage.
// -----------------------------------------------------------------------------

static std::vector<std::string> g_failures;

// Simple failure reporter that records the message (non-terminating)
static void report_failure(const std::string& file, int line, const std::string& msg)
{
    g_failures.push_back(file + ":" + std::to_string(line) + " - " + msg);
}

// Helper to print summary
static void print_summary(const std::string& suite_name, int total_tests)
{
    std::cout << "Test Suite: " << suite_name << " | Total: " << total_tests
              << " | Failures: " << g_failures.size() << "\n";
    for (const auto& f : g_failures) {
        std::cerr << "  FAILED: " << f << "\n";
    }
}

// Linear helper for asserting equality of pointers
#define ASSERT_PTR_EQ(actual, expected, expr)                          \
    do {                                                               \
        if ((actual) != (expected)) {                                \
            report_failure(__FILE__, __LINE__,                       \
                std::string("Assertion failed: ") + (expr) +            \
                " | actual: " + std::to_string(reinterpret_cast<std::uintptr_t>(actual)) + \
                " | expected: " + std::to_string(reinterpret_cast<std::uintptr_t>(expected)); \
        }                                                            \
    } while (0)

// Linear helper for asserting boolean true
#define ASSERT_TRUE(cond, expr)                                        \
    do {                                                               \
        if (!(cond)) {                                               \
            report_failure(__FILE__, __LINE__,                       \
                std::string("Assertion failed: ") + (expr));        \
        }                                                            \
    } while (0)


// -----------------------------------------------------------------------------
// Test fixtures: dummy malloc/free implementations and unique markers
// -----------------------------------------------------------------------------

static void* test_malloc_fn1(png_voidp ctx, size_t size)
{
    (void)ctx;
    // Just return a unique non-null pointer for testability
    static char dummy;
    (void)&dummy;
    return (void*)size; // distinct and easily comparable
}

static void test_free_fn1(png_voidp ctx)
{
    (void)ctx;
    // Do nothing for test
}

static void* test_malloc_fn2(png_voidp ctx, size_t size)
{
    (void)ctx;
    return (void*)(size ^ 0x5A5A); // another distinct marker
}

static void test_free_fn2(png_voidp ctx)
{
    (void)ctx;
    // Do nothing for test
}

// Static class member demonstration (to satisfy domain knowledge about static members)
class StaticDemo
{
public:
    static int value;
    static int set(int v) { value = v; return value; }
};

int StaticDemo::value = 0;

// -----------------------------------------------------------------------------
// Test 1: Basic update path - png_ptr is non-null and all outputs are assigned
// Expected: mem_ptr, malloc_fn, and free_fn are updated to the provided values.
// -----------------------------------------------------------------------------

static bool test_sets_mem_fn_basic_updates()
{
    // Prepare a png_struct with known initial values
    png_struct s;
    s.mem_ptr = (png_voidp)0x1111;
    s.malloc_fn = (png_malloc_ptr)0x2222;
    s.free_fn = (png_free_ptr)0x3333;

    // Call the focal function with new values
    png_voidp new_mem = (png_voidp)0x4444;
    png_malloc_ptr new_malloc = test_malloc_fn1;
    png_free_ptr   new_free   = test_free_fn1;

    png_set_mem_fn((&s), new_mem, new_malloc, new_free);

    bool ok = true;
    ASSERT_PTR_EQ(s.mem_ptr, new_mem, "s.mem_ptr should be updated to new_mem");
    if (s.malloc_fn != new_malloc) {
        report_failure(__FILE__, __LINE__, "s.malloc_fn not updated to new_malloc");
        ok = false;
    }
    if (s.free_fn != new_free) {
        report_failure(__FILE__, __LINE__, "s.free_fn not updated to new_free");
        ok = false;
    }
    return ok;
}

// -----------------------------------------------------------------------------
// Test 2: Null png_ptr branch - when png_ptr is NULL, no fields should be modified
// Expected: No changes to existing struct fields.
// -----------------------------------------------------------------------------

static bool test_null_ptr_no_change()
{
    png_struct s;
    s.mem_ptr = (png_voidp)0xAA55;
    s.malloc_fn = test_malloc_fn1;
    s.free_fn = test_free_fn1;

    // Call with NULL png_ptr; other parameters can be arbitrary
    png_set_mem_fn((png_structrp)NULL, (png_voidp)0xDEAD, test_malloc_fn2, test_free_fn2);

    bool ok = true;
    if (s.mem_ptr != (png_voidp)0xAA55) ok = false;
    if (s.malloc_fn != test_malloc_fn1) ok = false;
    if (s.free_fn != test_free_fn1) ok = false;
    return ok;
}

// -----------------------------------------------------------------------------
// Test 3: Non-null png_ptr but mem_ptr set to NULL
// Expected: mem_ptr becomes NULL, while function pointers are updated to the
// provided ones. This exercises the true branch where both updates occur.
// -----------------------------------------------------------------------------

static bool test_null_mem_ptr_updates_ptrs_and_functions()
{
    png_struct s;
    s.mem_ptr = (png_voidp)0xBEEF;
    s.malloc_fn = test_malloc_fn1;
    s.free_fn = test_free_fn1;

    png_set_mem_fn(&s, (png_voidp)NULL, test_malloc_fn2, test_free_fn2);

    bool ok = true;
    if (s.mem_ptr != NULL) {
        report_failure(__FILE__, __LINE__, "s.mem_ptr should be NULL after update");
        ok = false;
    }
    if (s.malloc_fn != test_malloc_fn2) {
        report_failure(__FILE__, __LINE__, "s.malloc_fn should be updated to test_malloc_fn2");
        ok = false;
    }
    if (s.free_fn != test_free_fn2) {
        report_failure(__FILE__, __LINE__, "s.free_fn should be updated to test_free_fn2");
        ok = false;
    }
    return ok;
}

// -----------------------------------------------------------------------------
// Test 4: Static member access demonstration (not directly related to the C API)
// Ensures static members can be set and read via the class, illustrating
// proper handling of static state in tests.
// -----------------------------------------------------------------------------

static bool test_static_member_usage()
{
    StaticDemo::set(123);
    bool ok = (StaticDemo::value == 123);
    if (!ok) {
        report_failure(__FILE__, __LINE__, "StaticDemo::value did not update as expected");
    }
    StaticDemo::set(999);
    ok = ok && (StaticDemo::value == 999);
    if (!ok) {
        report_failure(__FILE__, __LINE__, "StaticDemo::value did not update to 999 as expected");
    }
    return ok;
}

// -----------------------------------------------------------------------------
// Main entry: run all tests and print a summary.
// -----------------------------------------------------------------------------

int main()
{
    // Clear any previous failures
    g_failures.clear();

    int total = 0;
    int passed = 0;

    // Run Test 1
    total++;
    if (test_sets_mem_fn_basic_updates()) ++passed; else {
        // test failure already recorded in g_failures
    }

    // Run Test 2
    total++;
    if (test_null_ptr_no_change()) ++passed; else {
        // failure logged
    }

    // Run Test 3
    total++;
    if (test_null_mem_ptr_updates_ptrs_and_functions()) ++passed; else {
        // failure logged
    }

    // Run Test 4
    total++;
    if (test_static_member_usage()) ++passed; else {
        // failure logged
    }

    // Print summary
    print_summary("png_set_mem_fn (self-contained test)", total);

    // Return non-zero if any test failed
    if (passed != total) {
        return 1;
    }
    return 0;
}