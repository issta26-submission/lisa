// This test suite targets the focal method: png_have_msa(png_structp png_ptr)
// in linux.c. The function reads /proc/self/auxv to determine whether the MSA
// (ARM's MSA) hardware capability bit is set. The test suite is implemented in
// C++11 without any testing framework (no GTest). It uses a lightweight test
// harness that prints PASS/FAIL messages and reports a final summary.

// Important notes for understanding test design:
// - The real function reads /proc/self/auxv and uses AT_HWCAP to extract hwcap.
// - It computes has_msa = (hwcap >> 1) & 1; i.e., it inspects bit 1 (second LSB)
//   of the HWCAP value.
// - We can't reliably force the host to have or not have MSA, so tests compare
//   the function's output with an independently computed expectation from the same
//   /proc/self/auxv source to ensure correctness on the running system.
// - We provide a minimal harness for running tests from main, with non-terminating
//   assertions (printing failures but continuing execution).

#include <stdint.h>
#include <elf.h>
#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


// Declaration of the focal method under test.
// We declare a generic pointer type for the PNG pointer parameter to avoid
// dependency on libpng headers. The function is defined in linux.c.
extern "C" int png_have_msa(void* png_ptr);

// Candidate Keywords (for Step 1 understanding)
//
// - /proc/self/auxv: source of hardware capability information
// - AT_HWCAP: auxiliary vector tag for hardware capabilities
// - hwcap: value carrying CPU feature bits
// - MSA: the bit of interest is (hwcap >> 1) & 1
// - has_msa: return value indicating MSA support (0 or 1)
// - png_warning: potential warning path when /proc/self/auxv cannot be opened
// - PNG_WARNINGS_SUPPORTED: conditional compilation flag that enables warning
//   path (not exercised in these tests since /proc/self/auxv is usually readable)


// Helper function to compute the expected MSA value directly by reading
// /proc/self/auxv, mirroring the logic in png_have_msa.
// This provides a portable (host-dependent) baseline for validating the focal method.
static int compute_expected_msa_from_auxv() {
    Elf64_auxv_t aux;
    int fd = open("/proc/self/auxv", O_RDONLY);
    int has_msa = 0;

    if (fd >= 0) {
        // Read successive auxv entries until AT_HWCAP is found or EOF
        while (read(fd, &aux, sizeof(Elf64_auxv_t)) == sizeof(Elf64_auxv_t)) {
            if (aux.a_type == AT_HWCAP) {
                uint64_t hwcap = aux.a_un.a_val;
                has_msa = (hwcap >> 1) & 1;
                break;
            }
        }
        close(fd);
    }
    // If open failed, has_msa remains 0 (consistent with initial value)
    return has_msa;
}

// Lightweight test harness: non-terminating assertions with summary report.
static int g_failures = 0;

#define TEST_CHECK(cond, msg)                                                        \
    do {                                                                               \
        if (cond) { std::cout << "[PASS] " << msg << std::endl; }                    \
        else { std::cerr << "[FAIL] " << msg << " (line " << __LINE__ << ")" << std::endl; ++g_failures; } \
    } while (0)


// Test 1: png_have_msa should return either 0 or 1 and match the auxv-derived value.
// This validates the core functional path under typical Linux environments.
static void test_png_have_msa_matches_auxv() {
    // Call the focal function with a null PNG pointer (not used by the logic)
    int actual = png_have_msa(nullptr);
    int expected = compute_expected_msa_from_auxv();

    // The return value should be exactly 0 or 1
    TEST_CHECK(actual == 0 || actual == 1, "png_have_msa returns 0 or 1");

    // The actual should match the independently computed expected value
    TEST_CHECK(actual == expected, "png_have_msa matches auxv-derived expectation");
}

// Test 2: Repeated invocation should be stable for the same environment.
// This helps exercise multiple iterations of reading /proc/self/auxv within the same process.
static void test_png_have_msa_stability() {
    int v1 = png_have_msa(nullptr);
    int v2 = png_have_msa(nullptr);
    TEST_CHECK(v1 == v2, "png_have_msa returns stable value across calls");
}

// Test 3: Validate that the test harness can compute the expected value correctly.
// This isn't a direct property of png_have_msa but ensures compute_expected_msa_from_auxv()
// logic aligns with the like-for-like computation.
static void test_expected_value_computation() {
    int expected = compute_expected_msa_from_auxv();
    TEST_CHECK(expected == (expected == 0 || expected == 1), "expected value is 0 or 1");
}

// Entry point: run all tests and summarize results.
// If GTest is not used, we explicitly invoke test functions from main as per domain guidance.
int main() {
    std::cout << "Starting unit tests for png_have_msa...\n";

    test_png_have_msa_matches_auxv();
    test_png_have_msa_stability();
    test_expected_value_computation();

    if (g_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}