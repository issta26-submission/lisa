// Test suite for sqlite3OsDeviceCharacteristics
// Purpose: Verify behavior when id->pMethods is NULL and when it is non-NULL with a valid xDeviceCharacteristics callback.
// Notes:
// - We define minimal internal-like structures to simulate sqlite3_file and sqlite3_io_methods layouts.
// - We rely on the actual sqlite3OsDeviceCharacteristics() implementation (provided in os.c) via external linkage.
// - This test is written for C++11, uses no GTest, and uses a lightweight, non-terminating assertion approach.

#include <iostream>
#include <sqliteInt.h>


// Step A: Minimal internal-like type definitions to match the usage in sqlite3OsDeviceCharacteristics.
// These mirror the parts of the internal sqlite3_file and sqlite3_io_methods that the function touches.
extern "C" {

// Forward declaration matching the usage in os.c
struct sqlite3_file;

// Internal-like io-method table with the xDeviceCharacteristics callback.
// The actual SQLite header defines this; we recreate a compatible subset for testing.
typedef struct sqlite3_io_methods {
    int (*xDeviceCharacteristics)(struct sqlite3_file *id);
} sqlite3_io_methods;

// Minimal sqlite3_file structure with only the pMethods pointer.
// This mirrors typical internal layout (pMethods is first in many builds) and is sufficient for our test.
struct sqlite3_file {
    sqlite3_io_methods *pMethods;
};

// Declaration of the function under test (C linkage)
int sqlite3OsDeviceCharacteristics(struct sqlite3_file *id);
}

// Global test state to track failures in a non-terminating fashion
static int g_failed_tests = 0;

// Helper assertion macro: non-terminating, reports failure but continues execution.
#define EXPECT_EQ(actual, expected, desc) do { \
    if ((actual) != (expected)) { \
        std::cout << "[FAIL] " << desc << " | Expected: " << (expected) \
                  << " Actual: " << (actual) << std::endl; \
        ++g_failed_tests; \
    } else { \
        std::cout << "[PASS] " << desc << std::endl; \
    } \
} while(0)

// Global captured pointer to verify that the callback receives the correct id
static struct sqlite3_file *g_last_seen_id = nullptr;

// Test callback implementations to simulate xDeviceCharacteristics behavior
static int fakeDeviceCharacteristicsReturn111(struct sqlite3_file *id) {
    g_last_seen_id = id;
    return 111;
}

static int fakeDeviceCharacteristicsReturn222(struct sqlite3_file *id) {
    g_last_seen_id = id;
    return 222;
}

// Test 1: When id->pMethods == NULL, sqlite3OsDeviceCharacteristics should return 0 (true branch)
void test_DeviceCharacteristics_NullPMethods_ReturnsZero() {
    // Expose a sqlite3_file with NULL pMethods
    struct sqlite3_file id;
    id.pMethods = nullptr;

    // Call the focal method
    int result = sqlite3OsDeviceCharacteristics(&id);

    // Expect 0 as per the function's early exit path
    EXPECT_EQ(result, 0, "DeviceCharacteristics returns 0 when pMethods is NULL");
}

// Test 2: When id->pMethods != NULL, ensure it calls xDeviceCharacteristics and returns its value (111)
void test_DeviceCharacteristics_NonNullPMethods_Returns111() {
    // Prepare method table with fakeDeviceCharacteristicsReturn111
    struct sqlite3_file id;
    struct sqlite3_io_methods meth;
    meth.xDeviceCharacteristics = fakeDeviceCharacteristicsReturn111;
    id.pMethods = &meth;

    // Reset captured id
    g_last_seen_id = nullptr;

    // Call the focal method
    int result = sqlite3OsDeviceCharacteristics(&id);

    // Verify that the callback was invoked with the correct id and returned 111
    EXPECT_EQ(result, 111, "DeviceCharacteristics returns 111 when xDeviceCharacteristics returns 111");
    EXPECT_EQ(g_last_seen_id, &id, "DeviceCharacteristics passes correct id to xDeviceCharacteristics");
}

// Test 3: When id->pMethods != NULL, ensure it calls xDeviceCharacteristics and returns its value (222)
void test_DeviceCharacteristics_NonNullPMethods_Returns222() {
    // Prepare method table with fakeDeviceCharacteristicsReturn222
    struct sqlite3_file id;
    struct sqlite3_io_methods meth;
    meth.xDeviceCharacteristics = fakeDeviceCharacteristicsReturn222;
    id.pMethods = &meth;

    // Reset captured id
    g_last_seen_id = nullptr;

    // Call the focal method
    int result = sqlite3OsDeviceCharacteristics(&id);

    // Verify that the callback was invoked with the correct id and returned 222
    EXPECT_EQ(result, 222, "DeviceCharacteristics returns 222 when xDeviceCharacteristics returns 222");
    EXPECT_EQ(g_last_seen_id, &id, "DeviceCharacteristics passes correct id to xDeviceCharacteristics (second case)");
}

int main() {
    // Run all tests
    test_DeviceCharacteristics_NullPMethods_ReturnsZero();
    test_DeviceCharacteristics_NonNullPMethods_Returns111();
    test_DeviceCharacteristics_NonNullPMethods_Returns222();

    // Summary
    if (g_failed_tests == 0) {
        std::cout << "[SUMMARY] ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "[SUMMARY] " << g_failed_tests << " TEST(S) FAILED" << std::endl;
        return g_failed_tests;
    }
}