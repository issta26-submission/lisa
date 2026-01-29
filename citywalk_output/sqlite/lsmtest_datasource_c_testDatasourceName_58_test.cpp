/*
  Unit test suite for the focal method: testDatasourceName
  - Targets the function declared/defined to operate on DatasourceDefn structures.
  - Tests cover both branches of the eType predicate (seq vs rnd).
  - Uses a lightweight C++ test harness (no Google Test) with non-terminating assertions.
  - Assumes the surrounding C code (lsmtest.h, testDatasourceName, testMallocPrintf) is linkable.
  - All tests are executed from main() as required when gtest/gmock are not allowed.
*/

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <lsmtest.h>


// Import C headers and declarations
extern "C" {
}

// Declare the focal function (C linkage)
extern "C" char* testDatasourceName(const DatasourceDefn *p);

// Lightweight test harness (non-terminating assertions)
static int g_totalTests = 0;
static int g_failedTests = 0;

// Expectation macros (non-terminating, continue after failure)
#define EXPECT_STR_EQ(actual, expected) do { \
    ++g_totalTests; \
    if((actual) == nullptr && (expected) == nullptr) { \
        /* both null, pass */ \
    } else if((actual) == nullptr || (expected) == nullptr) { \
        ++g_failedTests; \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " expected \"" << (expected ? expected : "(null)") \
                  << "\" but got \"" << (actual ? actual : "(null)") << "\"\n"; \
    } else if(std::strcmp((actual), (expected)) != 0) { \
        ++g_failedTests; \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " expected \"" << (expected) << "\" but got \"" << (actual) << "\"\n"; \
    } \
} while(0)

#define EXPECT_INT_EQ(actual, expected) do { \
    ++g_totalTests; \
    if((actual) != (expected)) { \
        ++g_failedTests; \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " expected " << (expected) << " but got " << (actual) << "\n"; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    ++g_totalTests; \
    if(!(cond)) { \
        ++g_failedTests; \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " condition failed: " #cond "\n"; \
    } \
} while(0)

// Test 1: Verify that when eType == TEST_DATASOURCE_SEQUENCE, the prefix is "seq"
static void test_name_seq_prefix() {
    // Arrange
    DatasourceDefn d;
    d.eType = TEST_DATASOURCE_SEQUENCE;
    d.nMinKey = 1;
    d.nMaxKey = 5;
    d.nMinVal = 10;
    d.nMaxVal = 20;

    // Act
    char* actual = testDatasourceName(&d);

    // Assert
    const char* expected = "seq.(1-5).(10-20)";
    EXPECT_STR_EQ(actual, expected);

    // Cleanup
    free(actual);
}

// Test 2: Verify that when eType != TEST_DATASOURCE_SEQUENCE, the prefix is "rnd"
static void test_name_rnd_prefix() {
    // Arrange
    DatasourceDefn d;
    d.eType = 9999;  // any value other than TEST_DATASOURCE_SEQUENCE
    d.nMinKey = 1;
    d.nMaxKey = 5;
    d.nMinVal = 10;
    d.nMaxVal = 20;

    // Act
    char* actual = testDatasourceName(&d);

    // Assert
    const char* expected = "rnd.(1-5).(10-20)";
    EXPECT_STR_EQ(actual, expected);

    // Cleanup
    free(actual);
}

// Test 3: Verify formatting with negative values to ensure correct sign handling
static void test_name_negative_values() {
    // Arrange
    DatasourceDefn d;
    d.eType = 9999;  // rnd branch
    d.nMinKey = -5;
    d.nMaxKey = 0;
    d.nMinVal = -100;
    d.nMaxVal = -1;

    // Act
    char* actual = testDatasourceName(&d);

    // Assert
    const char* expected = "rnd.(-5-0).(-100--1)";
    EXPECT_STR_EQ(actual, expected);

    // Cleanup
    free(actual);
}

// Optional: a simple test to ensure multiple values produce stable outputs
static void test_name_stability_with_seq() {
    // Arrange
    DatasourceDefn d;
    d.eType = TEST_DATASOURCE_SEQUENCE;
    d.nMinKey = 123;
    d.nMaxKey = 456;
    d.nMinVal = 7890;
    d.nMaxVal = 9876;

    // Act
    char* actual1 = testDatasourceName(&d);

    // Mutate after first call to ensure no internal state leakage (no static state expected)
    // Reset fields to the same values to verify deterministic output
    d.nMinKey = 123;
    d.nMaxKey = 456;
    d.nMinVal = 7890;
    d.nMaxVal = 9876;
    char* actual2 = testDatasourceName(&d);

    // Assert
    const char* expected = "seq.(123-456).(7890-9876)";
    EXPECT_STR_EQ(actual1, expected);
    EXPECT_STR_EQ(actual2, expected);

    // Cleanup
    free(actual1);
    free(actual2);
}

// Entry point to run all test cases
int main() {
    // Run individual test cases
    test_name_seq_prefix();
    test_name_rnd_prefix();
    test_name_negative_values();
    test_name_stability_with_seq();

    // Summary
    std::cout << "Total tests: " << g_totalTests
              << ", Failures: " << g_failedTests << std::endl;

    // Non-zero exit on failure to aid integration in CI environments
    return (g_failedTests != 0) ? 1 : 0;
}