// Minimal C++11 unit test suite for Prelin8dup (cmsopt.c)
// No GTest used. A lightweight in-process test harness is implemented.

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include internal CMS headers to access internal data types used by Prelin8dup.
// These headers are part of the library sources provided with the project.

// Expose the focal function from cmsopt.c (must be linked with the test binary)
extern "C" void* Prelin8dup(cmsContext ContextID, const void* ptr);

// Lightweight test harness: non-terminating assertions that allow all tests to run.
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total; \
    if (!(cond)) { \
        ++g_failed; \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) << " [LINE " << __LINE__ << "]\n"; \
    } \
} while (0)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_total; \
    if (!((a) == (b))) { \
        ++g_failed; \
        std::cerr << "EXPECT_EQ FAILED: " << (msg) << " (" << (a) << " != " << (b) << ") [LINE " << __LINE__ << "]\n"; \
    } \
} while (0)

// Helper to summarize test results
static void printSummary() {
    std::cout << "\nTest Summary: Total=" << g_total
              << ", Passed=" << (g_total - g_failed)
              << ", Failed=" << g_failed << "\n";
}

// Test 1: Basic duplication correctness
// - Allocate a Prelin8Data object, fill with a known byte pattern.
// - Call Prelin8dup and verify the returned memory is a distinct block with identical content.
static bool test_Prelin8dup_basic_duplication() {
    // Create a CMS context for allocation behavior
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (ctx == nullptr) {
        std::cerr << "Failed to create cmsContext for test_Prelin8dup_basic_duplication\n";
        return false;
    }

    // Prepare input with a deterministic pattern
    Prelin8Data input;
    std::memset(&input, 0x5A, sizeof(input));

    // Perform duplication
    void* dup = Prelin8dup(ctx, &input);

    // Assertions
    EXPECT_TRUE(dup != nullptr, "Prelin8dup should return non-null pointer");
    if (dup != nullptr) {
        // Content must be identical
        int cmp = std::memcmp(&input, dup, sizeof(input));
        EXPECT_EQ(cmp, 0, "Duplicated memory should have identical byte contents to input");
        // The addresses should be different (distinct memory blocks)
        EXPECT_TRUE(dup != &input, "Duplicated memory should be a different block from input");
    }

    // Cleanup
    cmsCloseContext(ctx);
    return g_failed == 0;
}

// Test 2: Multiple duplications produce independent blocks with identical content
// - Ensure subsequent calls with different input patterns yield separate blocks with correct data.
static bool test_Prelin8dup_multiple_duplications_distinct_blocks() {
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (ctx == nullptr) {
        std::cerr << "Failed to create cmsContext for test_Prelin8dup_multiple_duplications_distinct_blocks\n";
        return false;
    }

    // First input pattern
    Prelin8Data input1;
    std::memset(&input1, 0xA5, sizeof(input1));
    void* dup1 = Prelin8dup(ctx, &input1);
    EXPECT_TRUE(dup1 != nullptr, "First duplication should not return null");
    if (dup1 != nullptr) {
        int cmp1 = std::memcmp(&input1, dup1, sizeof(input1));
        EXPECT_EQ(cmp1, 0, "First duplicated data should match input1");
        EXPECT_TRUE(dup1 != &input1, "First duplication should be a separate block from input1");
    }

    // Second input pattern
    Prelin8Data input2;
    std::memset(&input2, 0x3C, sizeof(input2));
    void* dup2 = Prelin8dup(ctx, &input2);
    EXPECT_TRUE(dup2 != nullptr, "Second duplication should not return null");
    if (dup2 != nullptr) {
        int cmp2 = std::memcmp(&input2, dup2, sizeof(input2));
        EXPECT_EQ(cmp2, 0, "Second duplicated data should match input2");
        EXPECT_TRUE(dup2 != &input2, "Second duplication should be a separate block from input2");
        // Also ensure the two duplicates are different blocks
        EXPECT_TRUE(dup2 != dup1, "Two duplications should yield distinct memory blocks");
    }

    cmsCloseContext(ctx);
    return g_failed == 0;
}

// Test 3: Cross-context duplications produce independent memory for identical input
// - Use two distinct cmsContext instances to verify independence of duplications.
static bool test_Prelin8dup_cross_context_independence() {
    cmsContext ctx1 = cmsCreateContext(nullptr, nullptr);
    cmsContext ctx2 = cmsCreateContext(nullptr, nullptr);
    if (ctx1 == nullptr || ctx2 == nullptr) {
        std::cerr << "Failed to create cmsContext for test_Prelin8dup_cross_context_independence\n";
        if (ctx1) cmsCloseContext(ctx1);
        if (ctx2) cmsCloseContext(ctx2);
        return false;
    }

    Prelin8Data input;
    std::memset(&input, 0x77, sizeof(input));

    void* dupA = Prelin8dup(ctx1, &input);
    void* dupB = Prelin8dup(ctx2, &input);

    EXPECT_TRUE(dupA != nullptr, "dupA should not be null");
    EXPECT_TRUE(dupB != nullptr, "dupB should not be null");
    if (dupA && dupB) {
        // Both copies should be equal to input and to each other
        int cmpA = std::memcmp(&input, dupA, sizeof(input));
        int cmpB = std::memcmp(&input, dupB, sizeof(input));
        EXPECT_EQ(cmpA, 0, "dupA should copy input data exactly");
        EXPECT_EQ(cmpB, 0, "dupB should copy input data exactly");
        // Distinct blocks
        EXPECT_TRUE(dupA != dupB, "Duplications from different contexts should be distinct blocks");
    }

    cmsCloseContext(ctx1);
    cmsCloseContext(ctx2);
    return g_failed == 0;
}

int main() {
    // Note: The tests rely on the internal Prelin8Data structure and the internal
    // duplication function Prelin8dup. Ensure the project is built with cmsopt.c
    // compiled and linked into the test binary.

    // Run tests
    bool ok = true;
    ok &= test_Prelin8dup_basic_duplication();
    ok &= test_Prelin8dup_multiple_duplications_distinct_blocks();
    ok &= test_Prelin8dup_cross_context_independence();

    // Print summary
    printSummary();

    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}