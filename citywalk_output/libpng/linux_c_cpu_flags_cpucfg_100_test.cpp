/*
Unit Test Suite for cpu_flags_cpucfg (located in linux.c)
This test uses GCC's wrapper feature to intercept the external C function
read_cpucfg by compiling with -Wl,--wrap=read_cpucfg. The test provides
__wrap_read_cpucfg which returns preset values to drive both branches of
the conditional inside cpu_flags_cpucfg.

Test strategy notes:
- We simulate the value returned by read_cpucfg(reg) by controlling g_mock_cfg1.
- If (cfg1 & LOONGSON_CFG1_MMI) is non-zero, cpu_flags_cpucfg should return 1; else 0.
- We test two scenarios: false branch (return 0) and true branch (return all-ones).
- The tests are plain C++11, no GTest, and run from main().

Build instructions (external to this code):
- Ensure linux.c is part of the build.
- Compile with wrapping enabled: g++ test.cpp linux.c -o test_exec -Wl,--wrap=read_cpucfg
- The wrapper will redirect all calls to read_cpucfg to __wrap_read_cpucfg defined below.
*/

#include <cstdint>
#include <stdint.h>
#include <string.h>
#include <sys/auxv.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>


// Declaration of the focal function under test (C linkage)
extern "C" int cpu_flags_cpucfg(void);

// Global mock value that __wrap_read_cpucfg will return.
// The specific bit LOONGSON_CFG1_MMI is defined in the production headers;
// we don't rely on its exact value here, only that non-zero/zero produce
// different branches when ANDed with LOONGSON_CFG1_MMI inside cpu_flags_cpucfg.
static uint32_t g_mock_cfg1 = 0;

// Wrapper to intercept read_cpucfg calls during tests.
// We ignore the reg parameter for testing and return the preset mock value.
// This function will be linked in place of read_cpucfg due to the wrap option.
extern "C" uint32_t __wrap_read_cpucfg(uint32_t reg)
{
    (void)reg; // suppress unused parameter warning
    return g_mock_cfg1;
}

// Namespace to group tests and provide clear output
namespace TestCpuFlagsCpucfg {

// Utility macro for non-terminating assertions: logs message and exits on failure
#define ASSERT_EQ(expected, actual)                                \
    do {                                                           \
        if ((expected) != (actual)) {                              \
            std::cerr << "ASSERTION FAILED: " << #actual            \
                      << " == " << actual << ", expected "            \
                      << (expected) << std::endl;                  \
            std::exit(EXIT_FAILURE);                               \
        }                                                          \
    } while (0)

#undef LOG_TEST
#define LOG_TEST(name) do { std::cout << "[TEST] " << name << " ..." ; } while(0)
#define END_LOG() do { std::cout << " OK\n"; } while(0)

// Test 1: When read_cpucfg indicates no MMI bit (false branch), cpu_flags_cpucfg should return 0
void test_cpu_flags_cpucfg_false_branch()
{
    // Arrange: simulate cfg1 with MMI bit not set
    g_mock_cfg1 = 0;
    // Act: call the function under test
    int result = cpu_flags_cpucfg();
    // Assert: branch false leads to 0
    ASSERT_EQ(0, result);
    // Report
    END_LOG();
}

// Test 2: When read_cpucfg indicates MMI bit is set (true branch), cpu_flags_cpucfg should return 1
void test_cpu_flags_cpucfg_true_branch()
{
    // Arrange: simulate cfg1 with all bits set (guarantees MMI bit is set)
    g_mock_cfg1 = 0xFFFFFFFF;
    // Act
    int result = cpu_flags_cpucfg();
    // Assert: branch true leads to 1
    ASSERT_EQ(1, result);
    // Report
    END_LOG();
}

// Run all tests in this namespace
void run_all_tests()
{
    LOG_TEST("cpu_flags_cpucfg_false_branch"); std::cout << " - ";
    test_cpu_flags_cpucfg_false_branch();
    std::cout.flush(); // ensure ordered output

    LOG_TEST("cpu_flags_cpucfg_true_branch"); std::cout << " - ";
    test_cpu_flags_cpucfg_true_branch();
    std::cout.flush();
}

} // namespace TestCpuFlagsCpucfg

int main()
{
    // Execute the test suite
    TestCpuFlagsCpucfg::run_all_tests();
    return 0;
}