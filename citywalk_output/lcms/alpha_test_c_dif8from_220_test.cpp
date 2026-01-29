// Auto-generated C++11 test suite for the focal method dif8from and its dependencies.
// This test suite is designed to be light-weight (no Google Test) and relies on the
// standard C/C++ runtime. It uses a small custom test harness with non-terminating
// assertions (tests continue after failures) as requested by the domain guidance.

// NOTE: The focal function is defined in C code (dif8from) and depends on
// from_premul8_float and from_premul8. We declare them with C linkage to ensure
// proper linking when compiling with a C/C++ mix.

#include <cstdio>
#include <sys/stat.h>
#include <math.h>
#include <fcntl.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>
#include <sys/types.h>
#include <lcms2_internal.h>
#include <unistd.h>
#include <cstdint>


// Prototypes for the focal C functions (extern "C" to prevent name mangling)
extern "C" {
    uint8_t to_premul8_float(uint8_t rgb8, uint8_t a8);
    uint8_t from_premul8_float(uint8_t rgb8, uint8_t a8);
    uint16_t to_premul16_float(uint16_t rgb16, uint16_t a16);
    uint16_t from_premul16_float(uint16_t rgb16, uint16_t a16);
    uint16_t to_premul16(uint16_t rgb16, uint16_t a16);
    uint16_t from_premul16(uint16_t rgb16, uint16_t a16);
    uint8_t to_premul8(uint8_t rgb8, uint8_t a8);
    uint8_t from_premul8(uint8_t rgb8, uint8_t a8);
    void dif16to(void);
    void dif16from(void);
    void dif8to(void);
    void dif8from(void);
}

// Helper: call wrapper for the dif8from function to use with stdout capture
static void call_dif8from_wrapper() {
    // Execute the focal method under test
    dif8from();
}

// Helper: Capture stdout produced by calling a function that writes to stdout.
// Uses a POSIX pipe to redirect stdout during the function execution.
static std::string capture_stdout(void (*func)()) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If piping fails, return an empty string to indicate capture failure
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    // Redirect stdout to the write end of the pipe
    dup2(pipefd[1], fileno(stdout));
    close(pipefd[1]);

    // Run the function that writes to stdout
    func();

    // Flush and restore stdout
    fflush(stdout);
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);

    // Read captured output from the read end of the pipe
    std::string output;
    char buffer[4096];
    ssize_t count;
    // Close writing end now that we're done writing
    // (pipefd[0] still valid for reading)
    while ((count = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(count));
    }
    close(pipefd[0]);
    return output;
}

// Simple non-terminating assertion mechanism.
// The test runner will accumulate failures but will not abort subsequent tests.
static int g_total_tests = 0;
static int g_failed_tests = 0;
static int g_total_asserts = 0;
static int g_failed_asserts = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++g_total_asserts; \
    if (!(cond)) { \
        ++g_failed_asserts; \
        std::cerr << "[ASSERT FAILED] " << (msg) << std::endl; \
    } \
} while (0)

#define TEST_CASE(name) void name()

#define RUN_TEST(name) do { \
    ++g_total_tests; \
    std::cout << "[RUN] " #name " ..." << std::endl; \
    name(); \
} while (0)

// Explanatory wrapper-friendly test helpers

// Test 1: Ensure that the dif8from function prints the expected headers.
// This confirms the function runs and produces the key diagnostic messages.
TEST_CASE(test_dif8from_prints_headers) {
    std::string captured = capture_stdout(call_dif8from_wrapper);
    TEST_ASSERT(!captured.empty(), "Capture failed or stdout is empty from dif8from.");
    TEST_ASSERT(captured.find("Premul FROM8 diff") != std::string::npos,
                "Expected header 'Premul FROM8 diff' not found in output.");
    TEST_ASSERT(captured.find("Error max=") != std::string::npos,
                "Expected 'Error max=' summary line not found in output.");
}

// Test 2: Validate the behavior of from_premul8_float and from_premul8 for edge alpha.
// - When alpha = 0, premultiplication should yield 0 for any rgb value.
// - When alpha = 255, premultiplication should yield the original rgb value.
TEST_CASE(test_premul8_edge_alpha_zero_and_full) {
    // alpha = 0 -> result should be 0 for both functions
    TEST_ASSERT(from_premul8_float(123, 0) == 0, "from_premul8_float with alpha=0 should yield 0.");
    TEST_ASSERT(from_premul8(123, 0) == 0, "from_premul8 with alpha=0 should yield 0.");

    // alpha = 255 -> result should be original rgb for both functions
    TEST_ASSERT(from_premul8_float(123, 255) == 123, "from_premul8_float with alpha=255 should yield original rgb.");
    TEST_ASSERT(from_premul8(123, 255) == 123, "from_premul8 with alpha=255 should yield original rgb.");
}

// Test 3: Sanity check for a few representative values to ensure no crashes and sane ranges.
// These tests rely on the expectation that outputs are bounded within 0..255 for 8-bit channels.
TEST_CASE(test_premul8_output_range) {
    // Basic range check for several rgb/g combinations
    uint8_t inputs[][2] = {
        {0, 0}, {0, 128}, {128, 0}, {255, 128},
        {64, 192}, {200, 255}
    };
    for (const auto &pair : inputs) {
        uint8_t rgb = pair[0];
        uint8_t a   = pair[1];
        uint8_t r_float = from_premul8_float(rgb, a);
        uint8_t r_plain = from_premul8(rgb, a);
        TEST_ASSERT(r_float <= 255, "from_premul8_float produced value out of range (0..255).");
        TEST_ASSERT(r_plain <= 255, "from_premul8 produced value out of range (0..255).");
        // Also ensure that when alpha is 0, both yield 0 (redundant safety check)
        TEST_ASSERT((a == 0 && r_float == 0 && r_plain == 0),
                    "When alpha is 0, results should be 0 for both functions.");
    }
}

// Test 4: Quick smoke test to ensure the end-to-end dif8from runs without crashing for a subset of input.
// We reuse the stdout capture to verify that the function completes and produces the header line.
TEST_CASE(test_dif8from_smoke) {
    std::string captured = capture_stdout(call_dif8from_wrapper);
    TEST_ASSERT(!captured.empty(), "Smoke test: capture failed or no stdout.");
    TEST_ASSERT(captured.find("Premul FROM8 diff") != std::string::npos,
                "Smoke test: expected header printed by dif8from.");
}

// Main: run all tests and print a concise summary.
int main() {
    // Reset counters
    g_total_tests = g_failed_tests = g_total_asserts = g_failed_asserts = 0;

    // Run tests
    RUN_TEST(test_dif8from_prints_headers);
    RUN_TEST(test_premul8_edge_alpha_zero_and_full);
    RUN_TEST(test_premul8_output_range);
    RUN_TEST(test_dif8from_smoke);

    // Summarize results
    std::cout << "=====================================" << std::endl;
    std::cout << "Total tests run: " << g_total_tests << std::endl;
    std::cout << "Total assertions: " << g_total_asserts << std::endl;
    if (g_failed_asserts == 0) {
        std::cout << "[OK] All assertions passed." << std::endl;
    } else {
        std::cout << "[WARN] Some assertions failed: " << g_failed_asserts
                  << " out of " << g_total_asserts << "." << std::endl;
    }
    // Return non-zero if any assertion failed
    return (g_failed_asserts == 0) ? 0 : 1;
}