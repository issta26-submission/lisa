// High-level unit tests for sqlite3PagerSetMmapLimit
// Purpose: verify that the focal method correctly updates the mmap limit field
//          and exercises the minimal observable behavior expected from the method.
//
// Assumptions and approach:
// - We rely on the actual SQLite Pager type (Pager) and the function
//   sqlite3PagerSetMmapLimit declared in the project headers.
// - Since pagerFixMaplimit is a static (private) function inside pager.c, we
//   cannot observe its internal behavior directly. Therefore tests focus on
//   the observable effect: the szMmap field of the Pager is updated as requested.
// - Tests are written in plain C++11 (no Google Test) and are executable via a
//   simple main() test harness that reports success/failure.
// - We allocate a Pager instance with memset to a zeroed state. This mirrors a
//   typical unit-test approach when no full Pager initialization is available
//   in isolation. If the static initialization inside pager.c requires more
//   fields to be non-null, the test harness may be extended; for now we rely
//   on the minimal observable effect of the API.
// - All tests are self-contained and include explanatory comments.

#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <wal.h>
#include <cstdint>


// Bring in the C API declarations for the Pager type and sqlite3PagerSetMmapLimit.
// We wrap C includes in extern "C" to avoid C++ name mangling issues when linking.
extern "C" {
}

// Forward declaration of the focal API to ensure linkage in C++ test environment.
// This prototype is typically provided by the project's headers, but we re-declare
// it here to be explicit in the test harness.
extern "C" void sqlite3PagerSetMmapLimit(Pager *pPager, sqlite3_int64 szMmap);

using std::cout;
using std::endl;

// Simple assertion helper for test readability
#define ASSERT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "Assertion failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")\n"; \
        return false; \
    } \
} while (0)


// Test 1: Basic positive value assignment
// - Purpose: Ensure that calling sqlite3PagerSetMmapLimit sets pPager->szMmap to the
//   provided positive value.
// - Rationale: This validates the core behavior of the focal method.
bool test_SetMmapLimit_PositiveValue() {
    // Allocate a Pager object (uninitialized state may exist in real code; we use zero-init)
    Pager *pPager = (Pager *)malloc(sizeof(Pager));
    if (!pPager) {
        std::cerr << "Memory allocation failed for Pager in test_SetMmapLimit_PositiveValue\n";
        return false;
    }
    // Zero initialize to neutral state
    std::memset(pPager, 0, sizeof(Pager));

    // Apply a positive mmap size
    sqlite3_int64 testValue = 4096;
    sqlite3PagerSetMmapLimit(pPager, testValue);

    // Verify observable state
    ASSERT_EQ(pPager->szMmap, testValue);

    free(pPager);
    return true;
}


// Test 2: Zero value assignment
// - Purpose: Ensure that zero is a valid mmap limit and that the API handles it.
bool test_SetMmapLimit_ZeroValue() {
    Pager *pPager = (Pager *)malloc(sizeof(Pager));
    if (!pPager) {
        std::cerr << "Memory allocation failed for Pager in test_SetMmapLimit_ZeroValue\n";
        return false;
    }
    std::memset(pPager, 0, sizeof(Pager));

    sqlite3_int64 testValue = 0;
    sqlite3PagerSetMmapLimit(pPager, testValue);

    ASSERT_EQ(pPager->szMmap, testValue);

    free(pPager);
    return true;
}


// Test 3: Multiple successive updates
// - Purpose: Ensure that subsequent calls update the field correctly rather than
//   being ignored or overwritten by internal state.
// - Rationale: This exercises the method under repeated use.
bool test_SetMmapLimit_MultipleUpdates() {
    Pager *pPager = (Pager *)malloc(sizeof(Pager));
    if (!pPager) {
        std::cerr << "Memory allocation failed for Pager in test_SetMmapLimit_MultipleUpdates\n";
        return false;
    }
    std::memset(pPager, 0, sizeof(Pager));

    // First value
    sqlite3_int64 v1 = 1024;
    sqlite3PagerSetMmapLimit(pPager, v1);
    ASSERT_EQ(pPager->szMmap, v1);

    // Second value
    sqlite3_int64 v2 = 8192;
    sqlite3PagerSetMmapLimit(pPager, v2);
    ASSERT_EQ(pPager->szMmap, v2);

    free(pPager);
    return true;
}


// Test 4: Large value edge case
// - Purpose: Verify behavior with a very large mmap limit value within 64-bit range.
bool test_SetMmapLimit_LargeValue() {
    Pager *pPager = (Pager *)malloc(sizeof(Pager));
    if (!pPager) {
        std::cerr << "Memory allocation failed for Pager in test_SetMmapLimit_LargeValue\n";
        return false;
    }
    std::memset(pPager, 0, sizeof(Pager));

    // Choose a large value (within sqlite3_int64 range)
    sqlite3_int64 largeValue = (sqlite3_int64)1 << 40; // ~1 TB in bytes, just for testing range
    sqlite3PagerSetMmapLimit(pPager, largeValue);
    ASSERT_EQ(pPager->szMmap, largeValue);

    free(pPager);
    return true;
}


// Main entry for running all tests
int main() {
    int failures = 0;

    cout << "Running sqlite3PagerSetMmapLimit unit tests (C++11)..." << endl;

    if (!test_SetMmapLimit_PositiveValue()) {
        std::cerr << "test_SetMmapLimit_PositiveValue FAILED" << endl;
        ++failures;
    } else {
        cout << "test_SetMmapLimit_PositiveValue passed" << endl;
    }

    if (!test_SetMmapLimit_ZeroValue()) {
        std::cerr << "test_SetMmapLimit_ZeroValue FAILED" << endl;
        ++failures;
    } else {
        cout << "test_SetMmapLimit_ZeroValue passed" << endl;
    }

    if (!test_SetMmapLimit_MultipleUpdates()) {
        std::cerr << "test_SetMmapLimit_MultipleUpdates FAILED" << endl;
        ++failures;
    } else {
        cout << "test_SetMmapLimit_MultipleUpdates passed" << endl;
    }

    if (!test_SetMmapLimit_LargeValue()) {
        std::cerr << "test_SetMmapLimit_LargeValue FAILED" << endl;
        ++failures;
    } else {
        cout << "test_SetMmapLimit_LargeValue passed" << endl;
    }

    if (failures == 0) {
        cout << "All tests PASSED." << endl;
        return 0;
    } else {
        cout << failures << " test(s) FAILED." << endl;
        return 1;
    }
}