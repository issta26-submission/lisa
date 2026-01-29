/*
Unit Test Suite for lsmVarintPut32 (focal method)
Approach:
- We analyze the focal method: int lsmVarintPut32(u8 *aData, int iVal){
    return lsmSqlite4PutVarint64(aData, (u64)iVal);
  }
- It delegates to lsmSqlite4PutVarint64 with iVal cast to u64.
- We design tests to cover a representative set of iVal values (including negative and boundary values).
- Since we can't rely on external test frameworks (GTest) per requirements, we implement a lightweight test harness.
- We assume the project compiles as C with C++11 test harness. We include lsm_varint.c in this TU to access lsmVarintPut32 and its dependencies.
- To observe deterministic behavior, we do not mock external dependencies manually; instead we validate return value and aData[0] content, where possible. If the internal implementation of lsmSqlite4PutVarint64 is provided by the same TU, our checks align with the observed behavior.
Notes:
- The tests rely on the following to be available in the translation unit: lsmVarintPut32, lsmSqlite4PutVarint64, u8, u64, etc.
- We use a minimal assertion framework (CHECK macro) that logs failures but continues executing to maximize code coverage.
- We run tests from main() and print a concise summary.
- We include explanatory comments for each test case.
*/

#include <vector>
#include <lsm_varint.c>
#include <lsmInt.h>
#include <climits>
#include <iostream>


using namespace std;

/*
Step 1/3: Include the focal code in this translation unit so tests can exercise
lsmVarintPut32 and its static/inline dependencies.
We also pull in lsmInt.h to define u8 and u64 types used by the focal method.
*/
extern "C" {
/*
We include the C source file directly to ensure lsmVarintPut32 and its
dependency (lsmSqlite4PutVarint64) are in the same translation unit, allowing
our test to observe actual behavior without linking complications.
*/
}

/*
Domain notes:
- We are not using GTest. We implement a tiny test harness with CHECK(condition, msg)
that prints failures but doesn't abort the test suite, enabling higher coverage.
- The test values cover positive, zero, and negative integers to exercise the cast to u64.
- We also verify that the aData buffer is written to by the mock/implementation when applicable.
*/

// Lightweight assertion macro that logs failures but continues
static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test Failure: " << msg << " (in " << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(false)

// Helper to compute expected result based on iVal, mirroring the contract:
// return value = (int)((u64)iVal & 0xFF) + 1
static int expected_lsmVarintPut32_result(int iVal) {
    uint64_t castVal = static_cast<uint64_t>(iVal);
    return static_cast<int>((castVal & 0xFFULL) + 1ULL);
}

// Helper to compute expected first byte written to aData by the mock/implementation.
// If lsmSqlite4PutVarint64 writes z[0] with (u64)iVal & 0xFF, this should be:
// expected_byte = (unsigned char)((uint64_t)iVal & 0xFF)
static unsigned char expected_lsmVarintPut32_firstByte(int iVal) {
    uint64_t castVal = static_cast<uint64_t>(iVal);
    return static_cast<unsigned char>(castVal & 0xFFULL);
}

/*
Test suite entry
- We exercise lsmVarintPut32 with a variety of iVal values and verify:
  1) Returned value matches expected_lsmVarintPut32_result(iVal)
  2) If aData buffer is provided, its first byte matches expected_lsmVarintPut32_firstByte(iVal)
- We use a small inlined buffer and a few representative values.
*/
static void test_lsmVarintPut32_basic_values() {
    const int test_vals[] = {
        0,
        -1,
        1,
        12345,
        -12345,
        INT_MAX,
        INT_MIN
    };
    const size_t num_vals = sizeof(test_vals) / sizeof(test_vals[0]);

    for (size_t idx = 0; idx < num_vals; ++idx) {
        int iVal = test_vals[idx];
        unsigned char data[8] = {0}; // buffer used by lsmVarintPut32

        // Call the focal method
        int ret = lsmVarintPut32(data, iVal);

        // Expected computations
        int expected_ret = expected_lsmVarintPut32_result(iVal);
        unsigned char expected_byte = expected_lsmVarintPut32_firstByte(iVal);

        // Validate: return value
        CHECK(ret == expected_ret,
              "lsmVarintPut32 return value should match expected for iVal=" + std::to_string(iVal) +
              " (ret=" + std::to_string(ret) +
              ", expected=" + std::to_string(expected_ret) + ")");

        // Validate: first data byte written (if implementation writes to data)
        CHECK(data[0] == expected_byte,
              "lsmVarintPut32 should write first data byte as expected for iVal=" + std::to_string(iVal) +
              " (data[0]=" + std::to_string(data[0]) +
              ", expected=" + std::to_string(expected_byte) + ")");
    }
}

// Additional targeted test: verify behavior with a few more boundary-like values
static void test_lsmVarintPut32_boundary_like() {
    const int values[] = { 0, 0x7F, 0x80, 0xFF, -0x1, INT_MAX-1, INT_MIN/2 };
    for (int iVal : values) {
        unsigned char data[8];
        for (size_t i = 0; i < 8; ++i) data[i] = 0xAA; // fill with sentinel

        int ret = lsmVarintPut32(data, iVal);
        int expected_ret = expected_lsmVarintPut32_result(iVal);
        unsigned char expected_byte = expected_lsmVarintPut32_firstByte(iVal);

        CHECK(ret == expected_ret,
              "Boundary test: lsmVarintPut32 return value mismatch for iVal=" + std::to_string(iVal) +
              " (ret=" + std::to_string(ret) + ", expected=" + std::to_string(expected_ret) + ")");
        CHECK(data[0] == expected_byte,
              "Boundary test: lsmVarintPut32 first byte mismatch for iVal=" + std::to_string(iVal) +
              " (data[0]=" + std::to_string(data[0]) + ", expected=" + std::to_string(expected_byte) + ")");
    }
}

int main() {
    // Run tests
    test_lsmVarintPut32_basic_values();
    test_lsmVarintPut32_boundary_like();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << g_failures << " tests failed.\n";
        return 1;
    }
}