// Unit test suite for png_build_16to8_table (C++11, no GTest)
// This test mocks minimal PNG-related types and memory helpers to exercise
// the focal function in a self-contained way.

#include <cstdint>
#include <cassert>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain: Provide a minimal subset of the PNG types and functions used by the focal method.
// We implement the focal function here for testing in a single translation unit.

typedef void* png_structrp;        // Mocked opaque image structure pointer
typedef uint32_t png_uint_32;
typedef uint16_t png_uint_16;
typedef png_uint_16* png_uint_16p;
typedef png_uint_16p* png_uint_16pp;
typedef uint32_t png_fixed_point;  // Fixed-point gamma representation (mocked)

// Memory tracking (to allow clean-up and avoid leaks in tests)
static std::vector<void*> g_allocations;

// Mocked allocation functions used by the focal method
static void* png_calloc(png_structrp /*png_ptr*/, size_t num)
{
    // Allocate and zero-initialize
    void* p = std::calloc(1, num);
    g_allocations.push_back(p);
    return p;
}

static void* png_malloc(png_structrp /*png_ptr*/, size_t size)
{
    void* p = std::malloc(size);
    g_allocations.push_back(p);
    return p;
}

// Mocked gamma correction for 16-bit values.
// For test simplicity and deterministic behavior, return 0 so the boundary
// calculation is predictable (as needed for stable tests).
static png_uint_32 png_gamma_16bit_correct(unsigned int /*value*/, png_fixed_point /*gamma_val*/)
{
    return 0;
}

// Forward declaration of the focal method under test (provided in this TU)
static void png_build_16to8_table(png_structrp png_ptr, png_uint_16pp *ptable,
                                 unsigned int shift, png_fixed_point gamma_val);

// Implementation of the focal method (copied/adapted from the provided snippet)
static void png_build_16to8_table(png_structrp png_ptr, png_uint_16pp *ptable,
                                 unsigned int shift, png_fixed_point gamma_val)
{
    unsigned int num = 1U << (8U - shift);
    unsigned int max = (1U << (16U - shift)) - 1U;
    unsigned int i;
    png_uint_32 last;
    png_uint_16pp table = *ptable =
        (png_uint_16pp)png_calloc(png_ptr, num * (sizeof (png_uint_16p)));

    // Allocate each sub-table
    for (i = 0; i < num; i++)
        table[i] = (png_uint_16p)png_malloc(png_ptr,
            256 * (sizeof (png_uint_16)));

    last = 0;
    for (i = 0; i < 255; ++i) // 8-bit output values (0..254)
    {
        png_uint_16 out = (png_uint_16)(i * 257U); // 16-bit value
        png_uint_32 bound = png_gamma_16bit_correct(out+128U, gamma_val);
        bound = (bound * max + 32768U)/65535U + 1U;
        while (last < bound)
        {
            table[last & (0xffU >> shift)][last >> (8U - shift)] = out;
            last++;
        }
    }

    // Fill the remaining entries with 65535 (0xFFFF)
    while (last < (num << 8))
    {
        table[last & (0xff >> shift)][last >> (8U - shift)] = 65535U;
        last++;
    }
}

// Helper: Accessor for test cleanup (free all allocations)
static void png_free_all()
{
    for (void* p : g_allocations)
        std::free(p);
    g_allocations.clear();
}

// Convenience: Compute the number of tables based on shift
static unsigned compute_num_tables(unsigned shift)
{
    return 1U << (8U - shift);
}

// Test 1: shift = 0, gamma = 0
// Expectation:
// - The first 129 entries along column 0 of the first table are set to 0
//   (i.e., table[0..128][0] == 0) due to i=0 iteration and last progression.
// - The final fill writes 65535 to entries not set by the boundary process,
//   e.g., table[200][0] should be 65535 after completion.
static void test_shift0_gamma0()
{
    // Reset allocations
    png_free_all();

    png_uint_16pp *ptable_ptr = nullptr; // caller will pass address
    png_build_16to8_table(nullptr, ptable_ptr, 0, 0);

    // Validation: ensure a valid table was created
    png_uint_16pp* ptable_pp = ptable_ptr;
    // In our test harness, png_build_16to8_table assigns to *ptable.
    // Since we can't access the inner pointer directly, we simulate by
    // re-calling with a local variable to capture pointer.
    // Instead, we'll implement a minimal stand-in to expose the allocated pointer.
    // To keep tests self-contained, perform the operation again while capturing.

    // Redo with a proper capture
    g_allocations.clear();
    png_uint_16pp table_base = nullptr;
    png_uint_16pp* cap = &table_base;
    png_build_16to8_table(nullptr, cap, 0, 0);

    // Basic assertion: table_base must be non-null
    UTEST_ASSERT(table_base != nullptr, "png_build_16to8_table did not allocate table_base");

    // num tables for shift=0
    const unsigned num_tables = compute_num_tables(0);
    UTEST_ASSERT(num_tables == 256, "Expected 256 tables for shift=0");

    // The first 129 entries along column 0 should be 0
    for (unsigned r = 0; r <= 128; ++r)
    {
        // Access: table[r][0]
        if (table_base[r] == nullptr) {
            UTEST_ASSERT(false, "table_base[r] is null (unexpected)");
            break;
        }
        uint16_t val = table_base[r][0];
        UTEST_ASSERT(val == 0, "Expected value 0 at table_base[r][0] for r in [0..128]");
    }

    // A value that should be filled by the final pass: e.g., table[200][0] should be 65535
    UTEST_ASSERT(table_base[200] != nullptr, "table_base[200] is null");
    uint16_t v200_0 = table_base[200][0];
    UTEST_ASSERT(v200_0 == 65535U, "Expected final fill to set table[200][0] to 65535");

    // Cleanup
    // Free allocated structures (the test harness doesn't rely on them after this)
    png_free_all();
}

// Test 2: shift = 4, gamma = 0
// Expectation:
// - Only a small portion of the first table is affected by the boundary logic.
// - Specifically, table[0][0] should be 0 (set during i=0 iteration).
// - Entries set by the final fill at small indices should be 65535, e.g., table[1][0] and table[0][1].
static void test_shift4_gamma0()
{
    // Reset allocations
    png_free_all();

    // Use a capture to inspect the allocated table
    png_uint_16pp table_base = nullptr;
    png_uint_16pp* cap = &table_base;
    png_build_16to8_table(nullptr, cap, 4, 0);

    UTEST_ASSERT(table_base != nullptr, "png_build_16to8_table (shift=4) failed to allocate table");

    // num tables for shift=4
    const unsigned num_tables = compute_num_tables(4);
    UTEST_ASSERT(num_tables == 16, "Expected 16 tables for shift=4");

    // Check: first entry table[0][0] should be 0
    UTEST_ASSERT(table_base[0] != nullptr, "table_base[0] is null");
    uint16_t t00 = table_base[0][0];
    UTEST_ASSERT(t00 == 0, "Expected table[0][0] == 0 for shift=4");

    // Check: final fill should set some entries to 65535
    // Example: table[1][0] should be 65535 due to last progression
    UTEST_ASSERT(table_base[1] != nullptr, "table_base[1] is null");
    uint16_t t10 = table_base[1][0];
    UTEST_ASSERT(t10 == 65535U, "Expected table[1][0] == 65535 after final fill");

    // Example: table[0][1] should be 65535 due to final fill when last == 16
    UTEST_ASSERT(table_base[0] != nullptr, "table_base[0] is null (second access)");
    uint16_t t01 = table_base[0][1];
    UTEST_ASSERT(t01 == 65535U, "Expected table[0][1] == 65535 after final fill");

    // Cleanup
    png_free_all();
}

// Simple test registry and runner
static void run_all_tests()
{
    int overall_fail = 0;

    // Helper macro for test invocation with descriptive messages
#define RUN_TEST(t) do { \
    std::cout << "Running " #t "... "; \
    t(); \
    std::cout << "done." << std::endl; \
} while(0)

    // Execute tests
    RUN_TEST(test_shift0_gamma0);
    RUN_TEST(test_shift4_gamma0);

    if (overall_fail == 0)
        std::cout << "All tests completed. Summary: PASS" << std::endl;
    else
        std::cout << "Tests completed. Summary: FAIL" << std::endl;
}

// Lightweight assertion macro that does not terminate the test on failure
#define UTEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "UNIT TEST FAILURE: " << (msg) << " (condition: " #cond ")" << std::endl; \
        /* Do not abort to maximize coverage; keep track via static counter if needed */ \
        overall_fail = 1; \
    } \
} while(0)

int main()
{
    // Since UTEST_ASSERT uses a local 'overall_fail' in macros scope,
    // define a static variable accessible in test scopes.
    // We'll wrap tests to use a small global flag. For simplicity, re-run with a dedicated path.

    // Re-run tests using direct calls for reliability in this single-file test harness.
    std::cout << "Starting png_build_16to8_table unit tests (single-translation-unit build)" << std::endl;

    // Because the macro relies on a shared variable, define a small wrapper to reset flag
    // and call tests manually to ensure deterministic behavior.
    {
        // Test 1
        test_shift0_gamma0();
        // Test 2
        test_shift4_gamma0();
    }

    std::cout << "Unit tests finished." << std::endl;
    // Note: In this simplified harness, we rely on console output for pass/fail signals.
    return 0;
}