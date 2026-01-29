// This is a C++11 test harness for the focal C function:
// png_set_benign_errors(png_structrp png_ptr, int allowed)
// The tests are implemented without any external testing framework (GTest, etc.),
// using a small in-file mock of the necessary types and constants to exercise the logic.
// The tests cover true/false branches, idempotence, bit-preservation of unrelated fields,
// and handling of non-zero values for allowed.

#include <cstdint>
#include <iomanip>
#include <string>
#include <iostream>
#include <pngpriv.h>


// Step 1: Candidate Keywords extracted from the focal method
// - png_structrp / png_ptr: the pointer to the PNG internal state structure
// - PNG_FLAG_BENIGN_ERRORS_WARN, PNG_FLAG_APP_WARNINGS_WARN, PNG_FLAG_APP_ERRORS_WARN: bit flags
// - PNG_FLAG_BENIGN_ERRORS_WARN | PNG_FLAG_APP_WARNINGS_WARN | PNG_FLAG_APP_ERRORS_WARN: the enable mask
// - png_debug: internal debug hook
// - allowed != 0: enable path; else: disable path

// Domain knowledge hints:
// - true/false branches need coverage
// - Non-terminating assertions are preferred (we'll use test harness checks)
// - Only public interface is tested; private/internal details are mocked here

// Step 2: Minimal in-file mock/types to test the focal function logic

// Define the necessary constants (bit flags)
#define PNG_FLAG_BENIGN_ERRORS_WARN 0x01
#define PNG_FLAG_APP_WARNINGS_WARN  0x02
#define PNG_FLAG_APP_ERRORS_WARN    0x04

// The mask of all three relevant flags
#define PNG_BENIGN_ERROR_MASK (PNG_FLAG_BENIGN_ERRORS_WARN | PNG_FLAG_APP_WARNINGS_WARN | PNG_FLAG_APP_ERRORS_WARN)

// Forward declare a minimal png structure to mimic the real libpng struct used by png_set_benign_errors
typedef struct png_struct_s {
    unsigned int flags;
} png_struct;

// Pointer typedefs mimicking the naming in the real code
typedef png_struct* png_structrp;
typedef const png_struct* png_const_structrp;

// Minimal stub for the internal debug function used by the focal method
static void png_debug(int /*level*/, const char* /*message*/) {
    // No-op in test environment
    (void)0;
}

// The focal function under test, implemented to mirror the behavior observed in the provided code.
// We expose it with C linkage to align with the typical C API of libpng.
extern "C" void png_set_benign_errors(png_structrp png_ptr, int allowed) {
    // real code prints debug; we call our stub
    png_debug(1, "in png_set_benign_errors");
    if (allowed != 0) {
        png_ptr->flags |= PNG_BENIGN_ERROR_MASK;
    } else {
        png_ptr->flags &= ~PNG_BENIGN_ERROR_MASK;
    }
}

// Step 3: Simple test harness (no GTest). Implement a few targeted test cases to cover branches.

// Global counters for test results
static int g_pass = 0;
static int g_fail = 0;

// Helper: run a single test case
static void run_case(const std::string& name, unsigned int init_flags, int allowed, unsigned int expected) {
    png_struct s;
    s.flags = init_flags;

    png_set_benign_errors(&s, allowed);

    unsigned int got = s.flags;
    if (got == expected) {
        ++g_pass;
        std::cout << "[PASS] " << name << "\n";
    } else {
        ++g_fail;
        std::cout << "[FAIL] " << name
                  << " | init_flags=0x" << std::hex << init_flags
                  << " allowed=" << std::dec << allowed
                  << " | expected=0x" << std::hex << expected
                  << " got=0x" << got << std::dec << "\n";
    }
}

// Test 1: Enabling benign errors when allowed != 0 from a clean state (0)
static void test_enable_from_zero() {
    run_case(
        "Enable when allowed != 0 from 0",
        0x0,
        1,
        PNG_BENIGN_ERROR_MASK
    );
}

// Test 2: Enabling should preserve unrelated bits outside the mask
static void test_unrelated_bits_preserved_on_enable() {
    // Start with an unrelated bit (0x20) set
    run_case(
        "Enable preserves unrelated bits",
        0x20,
        2, // non-zero; should enable benign error bits
        0x20 | PNG_BENIGN_ERROR_MASK
    );
}

// Test 3: Disabling should clear all three benign-related bits
static void test_disable_from_masked_state() {
    // Start with all benign-related bits set
    run_case(
        "Disable clears benign error bits",
        PNG_BENIGN_ERROR_MASK,
        0,
        0x0
    );
}

// Test 4: Disabling on an unrelated bit only should preserve that bit
static void test_disable_only_affects_mask() {
    // Start with an unrelated bit 0x08 set
    run_case(
        "Disable affects only mask; unrelated preserved",
        0x08,
        0,
        0x08
    );
}

// Test 5: Disabling when flags were fully set and other bits exist
static void test_disable_from_full_flags() {
    // 0xFF -> after disable of mask, should be 0xF8
    run_case(
        "Disable from full flags reduces to non-masked bits only",
        0xFF,
        0,
        0xF8
    );
}

// Test 6: Enabling again from a state with partial bits set
static void test_enable_from_partial_mask() {
    // Start with 0x08 (unrelated) and no benign bits set; enable should set only the three bits
    run_case(
        "Enable from partial mask state",
        0x08,
        5, // non-zero; enable path
        0x08 | PNG_BENIGN_ERROR_MASK
    );
}

// Test 7: Non-zero allowed (e.g., -1) should behave as enabled
static void test_enable_from_negative_allowed() {
    run_case(
        "Enable from negative allowed value",
        0x0,
        -1, // treated as enabled
        PNG_BENIGN_ERROR_MASK
    );
}

// Main: run all tests and report summary
int main() {
    std::cout << "Starting tests for png_set_benign_errors (C API emulation in C++11 test harness)\n";

    test_enable_from_zero();
    test_unrelated_bits_preserved_on_enable();
    test_disable_from_masked_state();
    test_disable_only_affects_mask();
    test_disable_from_full_flags();
    test_enable_from_partial_mask();
    test_enable_from_negative_allowed();

    std::cout << "\nTest summary: "
              << g_pass << " passed, "
              << g_fail << " failed.\n";

    if (g_fail > 0) {
        return 1;
    }
    return 0;
}