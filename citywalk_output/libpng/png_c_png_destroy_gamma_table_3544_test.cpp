// Minimalistic unit tests for png_destroy_gamma_table implementing a focused subset
// of libpng-like behavior. This test harness is self-contained (no Google Test) and
// uses simple assertion macros. It exercises various branches via preprocessor
//-defined macros.
//
// The tests rely on a lightweight fake "png_struct" and a fake png_free to track frees.
// Macros defined at compile time enable/disable blocks to mirror the original code's
// conditional compilation behavior.
// Compile with: -DPNG_16BIT_SUPPORTED -DPNG_READ_BACKGROUND_SUPPORTED

#include <vector>
#include <functional>
#include <cstdlib>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Compile-time feature flags to mirror the focal code branches
#define PNG_16BIT_SUPPORTED 1
#define PNG_READ_BACKGROUND_SUPPORTED 1

// Forward declaration for the test struct
struct png_struct;

// Typedefs to mimic libpng's style
typedef png_struct* png_structrp;
typedef const png_struct* png_const_structrp;

// Lightweight png_struct with only the fields touched by the focal method
struct png_struct {
    void* gamma_table;
    void** gamma_16_table;
    void* gamma_from_1;
    void* gamma_to_1;
#ifdef PNG_16BIT_SUPPORTED
    void** gamma_16_from_1;
    void** gamma_16_to_1;
#endif
    int gamma_shift; // used to compute istop
};

// Global log to track freed allocations (addresses)
static std::vector<void*> freed_ptrs;

// Lightweight wrapper around memory deallocation used by the focal function.
// It records every freed pointer for post-call verification.
static void png_free(png_structrp png_ptr, void* ptr)
{
    if (ptr != nullptr) {
        freed_ptrs.push_back(ptr);
        free(ptr);
    }
}

// Focal method under test, reproduced in this test harness to be self-contained.
// This mirrors the original logic and conditional compilation blocks.
void png_destroy_gamma_table(png_structrp png_ptr)
{
    {
        png_free(png_ptr, png_ptr->gamma_table);
        png_ptr->gamma_table = NULL;
#ifdef PNG_16BIT_SUPPORTED
        if (png_ptr->gamma_16_table != NULL)
        {
            int i;
            int istop = (1 << (8 - png_ptr->gamma_shift));
            for (i = 0; i < istop; i++)
            {
                png_free(png_ptr, png_ptr->gamma_16_table[i]);
            }
        png_free(png_ptr, png_ptr->gamma_16_table);
        png_ptr->gamma_16_table = NULL;
        }
#endif /* 16BIT */
#if defined(PNG_READ_BACKGROUND_SUPPORTED) || \
   defined(PNG_READ_ALPHA_MODE_SUPPORTED) || \
   defined(PNG_READ_RGB_TO_GRAY_SUPPORTED)
        png_free(png_ptr, png_ptr->gamma_from_1);
        png_ptr->gamma_from_1 = NULL;
        png_free(png_ptr, png_ptr->gamma_to_1);
        png_ptr->gamma_to_1 = NULL;
#ifdef PNG_16BIT_SUPPORTED
        if (png_ptr->gamma_16_from_1 != NULL)
        {
            int i;
            int istop = (1 << (8 - png_ptr->gamma_shift));
            for (i = 0; i < istop; i++)
            {
                png_free(png_ptr, png_ptr->gamma_16_from_1[i]);
            }
        png_free(png_ptr, png_ptr->gamma_16_from_1);
        png_ptr->gamma_16_from_1 = NULL;
        }
        if (png_ptr->gamma_16_to_1 != NULL)
        {
            int i;
            int istop = (1 << (8 - png_ptr->gamma_shift));
            for (i = 0; i < istop; i++)
            {
                png_free(png_ptr, png_ptr->gamma_16_to_1[i]);
            }
        png_free(png_ptr, png_ptr->gamma_16_to_1);
        png_ptr->gamma_16_to_1 = NULL;
        }
#endif /* 16BIT */
#endif /* READ_BACKGROUND || READ_ALPHA_MODE || RGB_TO_GRAY */
    }
}

// Helper to reset the freed_ptrs log between tests
static void reset_freed_log()
{
    freed_ptrs.clear();
}

// Helper to verify exact sequence of freed pointers matches expectation
static bool verify_freed_sequence(const std::vector<void*>& expected)
{
    if (freed_ptrs.size() != expected.size()) {
        return false;
    }
    for (size_t i = 0; i < expected.size(); ++i) {
        if (freed_ptrs[i] != expected[i]) {
            return false;
        }
    }
    return true;
}

// Helper to allocate a block with given size and return its pointer
static void* alloc_block(size_t sz)
{
    void* p = malloc(sz);
    if (p == nullptr) {
        std::cerr << "Memory allocation failed in test setup\n";
        std::exit(1);
    }
    // Optionally fill with non-zero to avoid accidental optimizations
    memset(p, 0xAA, sz);
    return p;
}

// Test 1: All pointers allocated (gamma_table, gamma_16_table and per-entry blocks,
// gamma_from_1/gamma_to_1, gamma_16_from_1/gamma_16_to_1). Verifies full cleanup and
// exact sequence of frees.
static void test_destroy_gamma_table_all_allocated()
{
    // Explanation: This test covers all code paths under the defined macros.
    // It validates that all allocations are freed and pointers are NULL'ed afterwards.

    reset_freed_log();

    png_struct s{};
    s.gamma_shift = 4; // istop = 1 << (8 - 4) = 16
    size_t istop = (1 << (8 - s.gamma_shift)); // 16

    // Allocate gamma_table
    s.gamma_table = alloc_block(128);
    // Allocate gamma_16_table array
    s.gamma_16_table = (void**)alloc_block(sizeof(void*) * istop);
    // Allocate each gamma_16_table[i]
    void* gamma_16_table_ptrs[16];
    for (size_t i = 0; i < istop; ++i) {
        s.gamma_16_table[i] = alloc_block(32);
        gamma_16_table_ptrs[i] = s.gamma_16_table[i];
    }

    // Allocate gamma_from_1 and gamma_to_1
    s.gamma_from_1 = alloc_block(256);
    s.gamma_to_1   = alloc_block(256);

#ifdef PNG_16BIT_SUPPORTED
    // Allocate gamma_16_from_1 and gamma_16_to_1 arrays and per-entry blocks
    s.gamma_16_from_1 = (void**)alloc_block(sizeof(void*) * istop);
    void* gamma_16_from_1_ptrs[16];
    for (size_t i = 0; i < istop; ++i) {
        s.gamma_16_from_1[i] = alloc_block(16);
        gamma_16_from_1_ptrs[i] = s.gamma_16_from_1[i];
    }

    s.gamma_16_to_1 = (void**)alloc_block(sizeof(void*) * istop);
    void* gamma_16_to_1_ptrs[16];
    for (size_t i = 0; i < istop; ++i) {
        s.gamma_16_to_1[i] = alloc_block(16);
        gamma_16_to_1_ptrs[i] = s.gamma_16_to_1[i];
    }
#endif

    // Track original pointers for expected frees
    std::vector<void*> expected;
    expected.push_back(s.gamma_table);
    for (size_t i = 0; i < istop; ++i) expected.push_back(s.gamma_16_table[i]);
    expected.push_back(s.gamma_16_table);
    expected.push_back(s.gamma_from_1);
    expected.push_back(s.gamma_to_1);
#ifdef PNG_16BIT_SUPPORTED
    for (size_t i = 0; i < istop; ++i) expected.push_back(s.gamma_16_from_1[i]);
    expected.push_back(s.gamma_16_from_1);
    for (size_t i = 0; i < istop; ++i) expected.push_back(s.gamma_16_to_1[i]);
    expected.push_back(s.gamma_16_to_1);
#endif

    // Call the focal function
    png_destroy_gamma_table(&s);

    // Verifications
    bool ok = true;
    ok &= (s.gamma_table == NULL);
    ok &= (s.gamma_16_table == NULL);
    ok &= (s.gamma_from_1 == NULL);
    ok &= (s.gamma_to_1 == NULL);
#ifdef PNG_16BIT_SUPPORTED
    ok &= (s.gamma_16_from_1 == NULL);
    ok &= (s.gamma_16_to_1 == NULL);
#endif

    // Compare freed order
    bool freed_ok = verify_freed_sequence(expected);

    // Output results
    std::cout << "[Test All Allocated] "
              << (ok ? "PASS" : "FAIL") << "\n";
    if (!ok) {
        std::cout << "  Pointers not NULL after destroy:\n";
        std::cout << "    gamma_table: " << (s.gamma_table == nullptr ? "NULL" : "NOT NULL") << "\n";
        std::cout << "    gamma_16_table: " << (s.gamma_16_table == nullptr ? "NULL" : "NOT NULL") << "\n";
#ifdef PNG_16BIT_SUPPORTED
        std::cout << "    gamma_16_from_1: " << (s.gamma_16_from_1 == nullptr ? "NULL" : "NOT NULL") << "\n";
        std::cout << "    gamma_16_to_1: " << (s.gamma_16_to_1 == nullptr ? "NULL" : "NOT NULL") << "\n";
#endif
    }

    std::cout << "  Freed sequence match: " << (freed_ok ? "YES" : "NO") << "\n";

    if (!freed_ok) {
        std::cout << "  Freed pointers (order):\n";
        for (size_t i = 0; i < freed_ptrs.size(); ++i) {
            std::cout << "    [" << i << "] " << freed_ptrs[i] << "\n";
        }
        std::cout << "  Expected sequence:\n";
        for (size_t i = 0; i < expected.size(); ++i) {
            std::cout << "    [" << i << "] " << expected[i] << "\n";
        }
    }

    // Cleanup any accidentally leaked pointers if test failed (best effort)
    // In a real test, we'd avoid leaks, but since we freed all, there should be none.
}

// Test 2: Only gamma_table and gamma_16_table allocated; other pointers NULL.
// Verifies partial cleanup and that non-existent allocations do not crash.
static void test_destroy_gamma_table_partial_allocations()
{
    reset_freed_log();

    png_struct s{};
    s.gamma_shift = 4;
    size_t istop = (1 << (8 - s.gamma_shift)); // 16

    // Allocate gamma_table
    s.gamma_table = alloc_block(64);
    // Allocate gamma_16_table array and per-entry blocks
    s.gamma_16_table = (void**)alloc_block(sizeof(void*) * istop);
    for (size_t i = 0; i < istop; ++i) {
        s.gamma_16_table[i] = alloc_block(16);
    }

    // gamma_from_1, gamma_to_1 and 16_from_1/16_to_1 are left NULL to exercise NULL branches
    s.gamma_from_1 = nullptr;
    s.gamma_to_1 = nullptr;
#ifdef PNG_16BIT_SUPPORTED
    s.gamma_16_from_1 = nullptr;
    s.gamma_16_to_1 = nullptr;
#endif

    // Expected freed sequence
    std::vector<void*> expected;
    expected.push_back(s.gamma_table);
    for (size_t i = 0; i < istop; ++i) expected.push_back(s.gamma_16_table[i]);
    expected.push_back(s.gamma_16_table);
    // gamma_from_1/gamma_to_1 are NULL, so nothing further

    png_destroy_gamma_table(&s);

    // Validate
    bool ok = true;
    ok &= (s.gamma_table == NULL);
    ok &= (s.gamma_16_table == NULL);
#ifdef PNG_16BIT_SUPPORTED
    ok &= (s.gamma_16_from_1 == NULL);
    ok &= (s.gamma_16_to_1 == NULL);
#endif

    bool freed_ok = verify_freed_sequence(expected);

    std::cout << "[Test Partial Allocated] "
              << (ok && freed_ok ? "PASS" : "FAIL") << "\n";
    if (!ok || !freed_ok) {
        if (!ok) {
            std::cout << "  Pointers after destroy:\n";
            std::cout << "    gamma_table: " << (s.gamma_table ? "NOT NULL" : "NULL") << "\n";
            std::cout << "    gamma_16_table: " << (s.gamma_16_table ? "NOT NULL" : "NULL") << "\n";
        }
        if (!freed_ok) {
            std::cout << "  Freed sequence did not match expected:\n";
            for (size_t i = 0; i < freed_ptrs.size(); ++i) {
                std::cout << "    freed[" << i << "]=" << freed_ptrs[i] << "\n";
            }
            for (size_t i = 0; i < expected.size(); ++i) {
                std::cout << "    expected[" << i << "]=" << expected[i] << "\n";
            }
        }
    }

    // Cleanup
}

// Test 3: All pointers NULL. Ensures function handles NULLs gracefully without crashes.
static void test_destroy_gamma_table_all_null()
{
    reset_freed_log();

    png_struct s{};
    s.gamma_table = nullptr;
    s.gamma_16_table = nullptr;
    s.gamma_from_1 = nullptr;
    s.gamma_to_1 = nullptr;
#ifdef PNG_16BIT_SUPPORTED
    s.gamma_16_from_1 = nullptr;
    s.gamma_16_to_1 = nullptr;
#endif
    s.gamma_shift = 4;

    // Expected freed sequence is empty
    std::vector<void*> expected;

    png_destroy_gamma_table(&s);

    bool ok = true;
    ok &= (s.gamma_table == nullptr);
    ok &= (s.gamma_16_table == nullptr);
#ifdef PNG_16BIT_SUPPORTED
    ok &= (s.gamma_16_from_1 == nullptr);
    ok &= (s.gamma_16_to_1 == nullptr);
#endif

    bool freed_ok = freed_ptrs.empty();

    std::cout << "[Test All Null] "
              << ((ok && freed_ok) ? "PASS" : "FAIL") << "\n";
}

// Main entry point: run all tests
int main()
{
    std::cout << "Starting png_destroy_gamma_table unit tests (C++11, no GTest)...\n";

    test_destroy_gamma_table_all_allocated();
    test_destroy_gamma_table_partial_allocations();
    test_destroy_gamma_table_all_null();

    std::cout << "Unit tests finished.\n";

    return 0;
}