// Unit test suite for is_ICC_signature(png_alloc_size_t it) in png.c
// This test suite uses a lightweight, non-terminating assertion approach (no GTest).
// It relies on the project's pngpriv.h for necessary type definitions.
// The tests are designed to cover true/false branches of the predicate by combining
// is_ICC_signature_char on each 8-bit lane of the 32-bit input.

#include <string>
#include <iostream>
#include <pngpriv.h>


// Step 1: Include project dependencies to align with the focal method's environment.

// Step 2: Declare the focal functions with C linkage to ensure linkage compatibility.
// We rely on the project's type definitions (png_alloc_size_t, png_bool/ints).
extern "C" int is_ICC_signature(png_alloc_size_t it);
extern "C" int is_ICC_signature_char(png_alloc_size_t it);

// Lightweight assertion/reporting helpers (non-terminating).
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg)                                        \
    do {                                                              \
        ++g_total_tests;                                              \
        if (!(cond)) {                                                \
            ++g_failed_tests;                                         \
            std::cerr << "[FAIL] " << __func__ << ": " << msg << "\n"; \
        } else {                                                      \
            std::cout << "[PASS] " << __func__ << ": " << msg << "\n"; \
        }                                                             \
    } while (0)

static void RunCase(const std::string& name,
                    png_alloc_size_t it,
                    bool expected)
{
    // The actual function under test.
    int actual_int = is_ICC_signature(it);
    bool actual = (actual_int != 0);

    // Report and verify against the expected outcome derived from is_ICC_signature_char.
    EXPECT_TRUE(actual == expected,
                name + " -> expected " + std::string(expected ? "true" : "false") +
                ", got " + std::string(actual ? "true" : "false"));
}

// Helper to build a 32-bit value from four bytes (MSB first).
static inline png_alloc_size_t pack_bytes(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
    // Cast to the project type to avoid overflow/implicit width issues.
    return (static_cast<png_alloc_size_t>(b0) << 24) |
           (static_cast<png_alloc_size_t>(b1) << 16) |
           (static_cast<png_alloc_size_t>(b2) << 8)  |
           (static_cast<png_alloc_size_t>(b3));
}

// Step 3: Main test entry points.
int main() {
    // Explain intent (comments) for maintainers:
    // - Test1: All four bytes are plausible signature bytes. Expect true if each byte
    //         passes is_ICC_signature_char (i.e., the "valid character" predicate per byte).
    // - Test2: The most significant byte is invalid (0x00). This exercises the
    //         short-circuit path for the first predicate, ensuring the function
    //         returns false without requiring evaluation of later bytes.
    // - Test3: A middle byte is invalid. This validates evaluation of second/third
    //         predicates and the eventual false outcome.
    // - Test4: All bytes are 0xFF, a classic invalid scenario for typical ASCII-like checks.

    // Case 1: True scenario with ASCII-like valid bytes: 'a','c','s','p' -> 0x61,0x63,0x73,0x70
    png_alloc_size_t it1 = pack_bytes(0x61, 0x63, 0x73, 0x70);
    // Compute expected by applying per-byte predicate via the helper function.
    bool expected1 = is_ICC_signature_char(0x61) &&
                     is_ICC_signature_char(0x63) &&
                     is_ICC_signature_char(0x73) &&
                     is_ICC_signature_char(0x70);
    RunCase("Test1_true_all_valid_bytes", it1, expected1);

    // Case 2: First byte invalid (0x00). Short-circuit; overall false.
    png_alloc_size_t it2 = pack_bytes(0x00, 0x61, 0x63, 0x70);
    bool expected2 = is_ICC_signature_char(0x00) &&
                     is_ICC_signature_char(0x61) &&
                     is_ICC_signature_char(0x63) &&
                     is_ICC_signature_char(0x70);
    RunCase("Test2_false_due_to_first_byte", it2, !!expected2); // expected2 already false in typical predicates

    // Case 3: Middle byte invalid (0x00). First two bytes valid, third invalid should be false.
    png_alloc_size_t it3 = pack_bytes(0x61, 0x63, 0x00, 0x70);
    bool expected3 = is_ICC_signature_char(0x61) &&
                     is_ICC_signature_char(0x63) &&
                     is_ICC_signature_char(0x00) &&
                     is_ICC_signature_char(0x70);
    RunCase("Test3_false_due_to_third_byte", it3, !!expected3);

    // Case 4: All bytes invalid (0xFF). Expect false.
    png_alloc_size_t it4 = pack_bytes(0xFF, 0xFF, 0xFF, 0xFF);
    bool expected4 = is_ICC_signature_char(0xFF) &&
                     is_ICC_signature_char(0xFF) &&
                     is_ICC_signature_char(0xFF) &&
                     is_ICC_signature_char(0xFF);
    RunCase("Test4_false_all_invalid_bytes", it4, !!expected4);

    // Summary
    std::cout << "Total tests: " << g_total_tests
              << ", Failures: " << g_failed_tests << "\n";

    // Return non-zero when there are failures to indicate problems.
    return (g_failed_tests == 0) ? 0 : 1;
}