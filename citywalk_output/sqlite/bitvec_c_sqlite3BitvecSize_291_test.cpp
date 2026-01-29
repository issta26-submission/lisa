// Test suite for sqlite3BitvecSize(Bitvec *p)
// Purpose: Verify that sqlite3BitvecSize returns the iSize field set by sqlite3BitvecCreate.
// Constraints: No GTest; use C++11 standard library only; main() drives tests.
// This test treats Bitvec as an opaque type from the perspective of the test; only
// the size value is asserted via the public API (sqlite3BitvecCreate / sqlite3BitvecSize).

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Step 2 Domain: Declare the required C interfaces from the focal class/dependencies.
// We treat Bitvec as an opaque type to avoid depending on its internal layout.
extern "C" {
    // Forward declaration of the Bitvec type as an opaque struct.
    typedef struct Bitvec Bitvec;

    // Public API used by the focal method under test.
    Bitvec* sqlite3BitvecCreate(uint32_t iSize);
    uint32_t sqlite3BitvecSize(Bitvec* p);
    void sqlite3BitvecDestroy(Bitvec* p);
}

// Global test state: count failures to report a summary at the end.
static int g_failures = 0;

// Simple non-terminating assertion macro for numeric comparisons.
// On mismatch, prints a diagnostic message but does not abort the test suite.
#define EXPECT_EQ(actual, expected, description) \
    do { \
        uint64_t _act = static_cast<uint64_t>(actual); \
        uint64_t _exp = static_cast<uint64_t>(expected); \
        if (_act != _exp) { \
            std::cerr << "FAIL: " << (description) \
                      << " | expected: " << _exp \
                      << ", actual: " << _act << std::endl; \
            ++g_failures; \
        } else { \
            /* Optional informational pass can be enabled if needed */ \
        } \
    } while (0)

// Test 1: Basic correctness for a variety of iSize values.
// - Create a Bitvec with a given iSize.
// - Verify sqlite3BitvecSize(p) returns exactly that iSize.
// - Destroy the Bitvec to avoid leaks.
void test_sqlite3BitvecSize_basic_variants() {
    // Expose deterministic sizes to validate the mapping from create() to size().
    const uint32_t test_sizes[] = {0, 1, 12345, 65535};
    for (uint32_t sz : test_sizes) {
        // Create a Bitvec with iSize = sz
        Bitvec* p = sqlite3BitvecCreate(sz);
        // Ensure size accessor returns the same value that was requested
        uint32_t returned = sqlite3BitvecSize(p);
        EXPECT_EQ(returned, sz, "sqlite3BitvecSize should return the iSize passed to sqlite3BitvecCreate");

        // Cleanup
        sqlite3BitvecDestroy(p);
    }
}

// Test 2: Repeated create/destroy with varying sizes to ensure no state corruption.
// - Reuse the API across multiple lifetimes and confirm each is isolated.
void test_sqlite3BitvecSize_repeated_lifetimes() {
    // A sequence of sizes to exercise different allocations
    const uint32_t sizes[] = {8, 256, 4096, 100000};
    for (uint32_t sz : sizes) {
        Bitvec* p = sqlite3BitvecCreate(sz);
        uint32_t returned = sqlite3BitvecSize(p);
        EXPECT_EQ(returned, sz, "Repeated-lifecycle: sqlite3BitvecSize should match iSize for each create");
        sqlite3BitvecDestroy(p);
    }
}

// Optional: main harness to drive tests without external test framework.
// Outputs a summary and returns non-zero if any test failed.
int main() {
    // Run tests
    test_sqlite3BitvecSize_basic_variants();
    test_sqlite3BitvecSize_repeated_lifetimes();

    // Summary
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED: sqlite3BitvecSize behavior is correct for tested cases." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED." << std::endl;
        return 1;
    }
}