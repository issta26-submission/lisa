// Unit tests for sqlite3VdbeSerialTypeLen
// Language: C++11
// Notes:
// - This test suite targets the focal method:
//     u32 sqlite3VdbeSerialTypeLen(u32 serial_type)
// - We rely on the following external C declarations from the project:
//     extern "C" u32 sqlite3VdbeSerialTypeLen(u32 serial_type);
//     extern "C" const unsigned char sqlite3SmallTypeSizes[];
// - We implement a minimal test harness (no GoogleTest) with non-terminating assertions.
// - Tests cover both branches of the top-level if condition (serial_type >= 128) and the else branch.

#include <cstdio>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Type alias used by the SQLite codebase for u32
typedef uint32_t u32;

// External C linkage to the focal function and to the small type sizes array.
// We assume the project provides these symbols with C linkage.
extern "C" u32 sqlite3VdbeSerialTypeLen(u32 serial_type);
extern "C" const unsigned char sqlite3SmallTypeSizes[];

// Simple non-terminating assertion macros for test continuity.
// They print a message on failure but do not abort the test suite.
static int gTestFailures = 0;

#define TEST_ASSERT_EQ(actual, expected, message) do { \
    if ((actual) != (expected)) { \
        std::cerr << "[FAIL] " << message \
                  << " | Expected: " << static_cast<unsigned>(expected) \
                  << ", Actual: " << static_cast<unsigned>(actual) \
                  << " (Line " << __LINE__ << ")" << std::endl; \
        ++gTestFailures; \
    } else { \
        std::cout << "[PASS] " << message << std::endl; \
    } \
} while(0)

#define TEST_ASSERT_NEAR(actual, expected, delta, message) do { \
    if ( ((actual) > (expected) + (delta)) || ((actual) < (expected) - (delta)) ) { \
        std::cerr << "[FAIL] " << message \
                  << " | Expected near: " << static_cast<unsigned>(expected) \
                  << ", Actual: " << static_cast<unsigned>(actual) \
                  << " (Line " << __LINE__ << ")" << std::endl; \
        ++gTestFailures; \
    } else { \
        std::cout << "[PASS] " << message << std::endl; \
    } \
} while(0)

// Test 1: Verify the >=128 branch of sqlite3VdbeSerialTypeLen
// This hits the first branch: if (serial_type >= 128) { return (serial_type-12)/2; }
void test_sqlite3VdbeSerialTypeLen_Gte128_branch() {
    // Use a representative value for the >=128 branch.
    // Expected value: (128 - 12) / 2 = 58
    u32 val = sqlite3VdbeSerialTypeLen(128);
    u32 expected = (128u - 12u) / 2u;
    TEST_ASSERT_EQ(val, expected, "sqlite3VdbeSerialTypeLen should compute (serial_type-12)/2 for serial_type >= 128");
}

// Test 2: Verify the else branch (serial_type < 128) uses the sqlite3SmallTypeSizes mapping
// This hits the else branch: return sqlite3SmallTypeSizes[serial_type]
// We compare against the external sqlite3SmallTypeSizes array value for serial_type = 0.
void test_sqlite3VdbeSerialTypeLen_Less128_usesSmallTypeSizes_0() {
    // serial_type = 0 falls into the else branch; the function should return sqlite3SmallTypeSizes[0]
    u32 val = sqlite3VdbeSerialTypeLen(0);
    u32 expected = static_cast<u32>(sqlite3SmallTypeSizes[0]);
    TEST_ASSERT_EQ(val, expected, "sqlite3VdbeSerialTypeLen should return sqlite3SmallTypeSizes[0] for serial_type = 0");
}

// Test 3: Verify the else branch for serial_type = 12
// This ensures the inner assertion condition (which is part of the else branch) does not crash
// and that the function returns sqlite3SmallTypeSizes[12].
void test_sqlite3VdbeSerialTypeLen_Less128_serial12() {
    u32 val = sqlite3VdbeSerialTypeLen(12);
    u32 expected = static_cast<u32>(sqlite3SmallTypeSizes[12]);
    TEST_ASSERT_EQ(val, expected, "sqlite3VdbeSerialTypeLen should return sqlite3SmallTypeSizes[12] for serial_type = 12");
}

// Optional additional test: large value still uses (serial_type-12)/2 for >=128
// This reinforces the top-level branch logic with a different input.
void test_sqlite3VdbeSerialTypeLen_Gte128_otherValue() {
    u32 serial_type = 255;           // >=128 path
    u32 val = sqlite3VdbeSerialTypeLen(serial_type);
    u32 expected = (serial_type - 12) / 2;
    TEST_ASSERT_EQ(val, expected, "sqlite3VdbeSerialTypeLen should compute (serial_type-12)/2 for large serial_type");
}

// Main test runner
int main() {
    std::cout << "Starting tests for sqlite3VdbeSerialTypeLen...\n";

    test_sqlite3VdbeSerialTypeLen_Gte128_branch();
    test_sqlite3VdbeSerialTypeLen_Less128_usesSmallTypeSizes_0();
    test_sqlite3VdbeSerialTypeLen_Less128_serial12();
    test_sqlite3VdbeSerialTypeLen_Gte128_otherValue();

    if (gTestFailures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << gTestFailures << " test(s) FAILED" << std::endl;
        return 1;
    }
}