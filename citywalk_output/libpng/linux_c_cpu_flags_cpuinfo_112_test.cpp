/*
   Test suite for cpu_flags_cpuinfo in linux.c without using GTest.
   Approach:
   - Provide a lightweight fopen wrapper to simulate /proc/cpuinfo content
     via memory-backed buffers (fmemopen), avoiding real /proc access.
   - Expose a test control API set_cpuinfo_scenario(int) to switch among
     scenarios: Loongson-3 in cpu model, loongson-mmi in ASEs, file-not-found,
     and no-match content.
   - Implement four tests to cover true/false branches of both predicates
     used in cpu_flags_cpuinfo.
   - Use non-terminating assertions (custom EXPECT_* macros) to maximize
     execution coverage.
   - The test suite assumes linux.c is compiled with this test.cpp in the same build.
*/

#include <stdint.h>
#include <string.h>
#include <string>
#include <sys/auxv.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>


// Forward declaration of focal function from linux.c
extern "C" int cpu_flags_cpuinfo(void);

// Test scenario control and interposition-free wrapper
// (No -ldl needed; simple in-process interposition via our own fopen)

enum CpuInfoScenario {
    SC_DEFAULT = 0,
    SC_NOFILE = 1,
    SC_CPUINFO_LOONGSON3 = 2,
    SC_CPUINFO_MMI = 3,
    SC_CPUINFO_OTHER = 4
};

// Internal state for test control
static int g_cpuinfo_scenario = SC_DEFAULT;
static char g_cpuinfo_buf[1024];
static size_t g_cpuinfo_len = 0;

// API to switch the fake /proc/cpuinfo content
extern "C" void set_cpuinfo_scenario(int s) {
    g_cpuinfo_scenario = s;
    g_cpuinfo_len = 0;
    // Note: content buffer will be populated lazily by fopen wrapper when needed
}

// Simple memory-backed content switcher for /proc/cpuinfo
extern "C" FILE* fopen(const char* path, const char* mode) {
    // Intercept only /proc/cpuinfo opened for read
    if (path && mode && std::strcmp(path, "/proc/cpuinfo") == 0 && std::strcmp(mode, "r") == 0) {
        // Scenario 1: simulate file-not-found
        if (g_cpuinfo_scenario == SC_NOFILE) {
            return NULL;
        }

        // Prepare content based on scenario
        const char* content = nullptr;
        switch (g_cpuinfo_scenario) {
            case SC_CPUINFO_LOONGSON3:
                // Ensure presence of "Loongson-3 " substring after "cpu model"
                content = "cpu model\t: Loongson-3  x\n";
                break;
            case SC_CPUINFO_MMI:
                // Ensure presence of " loongson-mmi" with a leading space
                content = "ASEs implemented\t: loongson-mmi\n";
                break;
            case SC_CPUINFO_OTHER:
                content = "cpu model\t: Xeon\n";
                break;
            case SC_DEFAULT:
            default:
                // Default content to exercise a non-matching scenario lightly
                content = "cpu model\t: Xeon E5\n";
                break;
        }

        // Copy content into internal buffer for safety
        g_cpuinfo_len = std::strlen(content);
        if (g_cpuinfo_len > sizeof(g_cpuinfo_buf) - 1) {
            g_cpuinfo_len = sizeof(g_cpuinfo_buf) - 1;
        }
        std::memcpy(g_cpuinfo_buf, content, g_cpuinfo_len);
        g_cpuinfo_buf[g_cpuinfo_len] = '\0';

        // Use memory-backed stream
        return fmemopen(g_cpuinfo_buf, g_cpuinfo_len, "r");
    }

    // For any other paths, return NULL (not used by focal code paths)
    return NULL;
}

// Lightweight, non-terminating test framework
static int TESTS_RUN = 0;
static int TESTS_FAIL = 0;

#define EXPECT_TRUE(cond, msg) do {                                      \
    TESTS_RUN++;                                                         \
    if (!(cond)) {                                                         \
        TESTS_FAIL++;                                                    \
        std::cerr << "[FAIL] " << msg << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
    } else {                                                               \
        std::cout << "[OK]   " << msg << "\n";                          \
    }                                                                    \
} while (0)

// Individual test cases with explanations as comments

// Test 1: CPU model line contains Loongson-3, should set flags = 1
// This exercises the first branch: on seeing "cpu model" line, if it contains
// "Loongson-3 ", flags becomes 1 and loop breaks.
void test_cpuinfo_cpu_model_loongson3() {
    // Arrange
    set_cpuinfo_scenario(SC_CPUINFO_LOONGSON3);

    // Act
    int flags = cpu_flags_cpuinfo();

    // Assert
    EXPECT_TRUE(flags == 1,
        "cpu_flags_cpuinfo should be 1 when CPU model line contains 'Loongson-3 '");
}

// Test 2: ASEs implemented line contains 'loongson-mmi', should set flags = 1
// This exercises the second branch: if the first trigger is ASEs implemented, and
// it contains ' loongson-mmi', flags becomes 1 and loop breaks.
void test_cpuinfo_ase_loongson_mmi() {
    // Arrange
    set_cpuinfo_scenario(SC_CPUINFO_MMI);

    // Act
    int flags = cpu_flags_cpuinfo();

    // Assert
    EXPECT_TRUE(flags == 1,
        "cpu_flags_cpuinfo should be 1 when ASEs implemented line contains ' loongson-mmi'");
}

// Test 3: File not found (fopen returns NULL) should return 0 flags
void test_cpuinfo_file_not_found() {
    // Arrange
    set_cpuinfo_scenario(SC_NOFILE);

    // Act
    int flags = cpu_flags_cpuinfo();

    // Assert
    EXPECT_TRUE(flags == 0,
        "cpu_flags_cpuinfo should return 0 when /proc/cpuinfo cannot be opened (NULL file)");
}

// Test 4: No matching content should return 0
// Provide content with CPU model not Loongson-3 and ASEs not containing loongson-mmi.
// Since the code breaks after the first line, it should not set any flag.
void test_cpuinfo_no_match() {
    // Arrange
    set_cpuinfo_scenario(SC_OTHER);

    // Act
    int flags = cpu_flags_cpuinfo();

    // Assert
    EXPECT_TRUE(flags == 0,
        "cpu_flags_cpuinfo should return 0 when content does not match any expected patterns");
}

// Entry point
int main() {
    std::cout << "Starting cpu_flags_cpuinfo test suite (no GTest, no LD_PRELOAD).\n";

    test_cpuinfo_cpu_model_loongson3();
    test_cpuinfo_ase_loongson_mmi();
    test_cpuinfo_file_not_found();
    test_cpuinfo_no_match();

    std::cout << "Test results: " << TESTS_RUN << " ran, " << TESTS_FAIL << " failed.\n";

    // Return non-zero if any test failed to indicate overall failure
    return TESTS_FAIL;
}