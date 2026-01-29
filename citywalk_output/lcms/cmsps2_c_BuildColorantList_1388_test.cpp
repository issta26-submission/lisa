// Unit test suite for the focal method: BuildColorantList
// This test harness is designed for C++11 and does not rely on GTest.
// It uses a small, non-terminating assertion mechanism and prints a summary at the end.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <lcms2_internal.h>


// Minimal type definitions to allow linking with the focal function.
// We assume the real project defines these in its headers; the test only needs compatible usage.
typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;

// Declare the focal function with C linkage to match the library implementation.
extern "C" void BuildColorantList(char *Colorant, cmsUInt32Number nColorant, cmsUInt16Number Out[]);

// Simple non-terminating assertion helper for string comparison
static bool checkEquals(const std::string& actual, const std::string& expected, const char* testName) {
    bool ok = (actual == expected);
    if (ok) {
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cout << "[FAIL] " << testName
                  << " - expected: \"" << expected << "\""
                  << " actual: \"" << actual << "\"\n";
    }
    return ok;
}

// Test 1: Basic case with 3 colorant channels
// Verifies normal operation: outputs 3 numbers with three decimals separated by a single space.
static void test_BuildColorantList_basic() {
    // Arrange
    char Colorant[64] = {0};                 // Destination buffer
    cmsUInt16Number Out[3] = {0, 32768, 65535}; // 0, ~0.5, ~1.0

    // Act
    BuildColorantList(Colorant, 3, Out);

    // Assert
    std::string actual(Colorant);
    std::string expected = "0.000 0.500 1.000";
    checkEquals(actual, expected, "BuildColorantList_basic");
}

// Test 2: Zero colorants
// Verifies that when nColorant is 0 the function returns an empty string.
static void test_BuildColorantList_zero() {
    // Arrange
    char Colorant[64] = {0};
    cmsUInt16Number Out[1] = {0};

    // Act
    BuildColorantList(Colorant, 0, Out);

    // Assert
    std::string actual(Colorant);
    std::string expected = "";
    checkEquals(actual, expected, "BuildColorantList_zero");
}

// Test 3: Clamp to cmsMAXCHANNELS when nColorant exceeds the maximum allowed
// Verifies that extra channels beyond the limit are ignored in the output.
static void test_BuildColorantList_clamp() {
    // Arrange
    char Colorant[256] = {0};
    const int MAX_TEST_CHANNELS = 20; // We provide 20; function should clamp to cmsMAXCHANNELS (commonly 16)
    cmsUInt16Number Out[MAX_TEST_CHANNELS];
    for (int i = 0; i < 16; ++i) Out[i] = 65535; // All max => all "1.000"
    for (int i = 16; i < MAX_TEST_CHANNELS; ++i) Out[i] = 0;

    // Act
    BuildColorantList(Colorant, MAX_TEST_CHANNELS, Out);

    // Assert
    std::string actual(Colorant);
    // Expect 16 numbers of "1.000" separated by spaces
    std::string expected;
    for (int i = 0; i < 16; ++i) {
        if (i > 0) expected += ' ';
        expected += "1.000";
    }
    checkEquals(actual, expected, "BuildColorantList_clamp");
}

// Test 4: Mixed values
// Verifies correct formatting for a mix of 0, max, and mid values.
static void test_BuildColorantList_mixed() {
    // Arrange
    char Colorant[64] = {0};
    cmsUInt16Number Out[3] = {0, 65535, 32768}; // 0.000, 1.000, ~0.500
    // Act
    BuildColorantList(Colorant, 3, Out);

    // Assert
    std::string actual(Colorant);
    std::string expected = "0.000 1.000 0.500";
    checkEquals(actual, expected, "BuildColorantList_mixed");
}

// Runner that executes all tests and prints a summary
int main() {
    // Call all tests
    test_BuildColorantList_basic();
    test_BuildColorantList_zero();
    test_BuildColorantList_clamp();
    test_BuildColorantList_mixed();

    // Simple summary (recompute by re-running checks isn't easily summarized here
    // since checkEquals already prints PASS/FAIL per test.)
    std::cout << "Unit tests for BuildColorantList completed.\n";
    return 0;
}