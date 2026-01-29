// C++11 unit tests for convert_utf16_to_utf32 in cmstypes.c
// Notes:
// - We implement a lightweight test harness (no GoogleTest/GMock) and run tests from main.
// - We provide a custom _cmsReadUInt16Number() replacement to feed test data to the focal function.
// - Tests cover BMP values, surrogate pairs, corrupted surrogate cases, and insufficient input scenarios.
// - This file assumes the project provides lcms2_internal.h and the convert_utf16_to_utf32 symbol with C linkage.

#include <cstring>
#include <vector>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>
#include <cstdint>


// Bring in the types used by cms (assuming the environment provides lcms2 headers)

// Forward declaration to ensure correct linkage (C linkage for the focal function)
extern "C" cmsBool convert_utf16_to_utf32(cmsIOHANDLER* io, cmsInt32Number n, wchar_t* output);

// Global test data loader for surrogate/candidate sequences
static std::vector<uint16_t> g_seq;
static size_t g_pos = 0;

// Mock for _cmsReadUInt16Number used by convert_utf16_to_utf32.
// We provide a deterministic supplier of 16-bit values from g_seq.
extern "C" cmsBool _cmsReadUInt16Number(cmsIOHANDLER* io, cmsUInt16Number* value)
{
    (void)io; // io is not used by this test harness
    if (g_pos < g_seq.size()) {
        *value = g_seq[g_pos++];
        return TRUE;
    }
    // No more data available
    return FALSE;
}

// Lightweight assertion helper: does not terminate on failure; accumulates results for reporting.
static int test_passes = 0;
static int test_fails  = 0;

#define TEST_ASSERT(cond, msg)                                      \
    do {                                                              \
        if (cond) {                                                 \
            ++test_passes;                                          \
            std::cout << "[PASS] " << msg << "\n";                \
        } else {                                                    \
            ++test_fails;                                           \
            std::cout << "[FAIL] " << msg << "\n";                \
        }                                                           \
    } while (0)

namespace {

void test_bmp_basic() {
    // BMP characters: 'A', 'B', 'C'
    g_seq = { 0x0041, 0x0042, 0x0043 };
    g_pos = 0;
    wchar_t output[8];
    std::memset(output, 0, sizeof(output));

    // Call the focal function with n = 3 units to read and convert
    cmsBool ok = convert_utf16_to_utf32(nullptr, 3, output);

    TEST_ASSERT(ok == TRUE, "BMP basic conversion should succeed (TRUE).");
    TEST_ASSERT(output[0] == L'A' && output[1] == L'B' && output[2] == L'C',
                "BMP basic output should be 'A','B','C'.");
}

void test_surrogate_pair() {
    // Surrogate pair for U+1F600 (GRINNING FACE) followed by 'A'
    // High surrogate: D83D, Low surrogate: DE00
    g_seq = { 0xD83D, 0xDE00, 0x0041 };
    g_pos = 0;
    wchar_t output[8];
    std::memset(output, 0, sizeof(output));

    cmsBool ok = convert_utf16_to_utf32(nullptr, 3, output);

    TEST_ASSERT(ok == TRUE, "Surrogate pair conversion should succeed (TRUE).");
    // U+1F600 is 0x1F600; verify first element equals this value in wchar_t
    TEST_ASSERT(output[0] == (wchar_t)0x1F600 && output[1] == L'A',
                "Surrogate pair should yield U+1F600 followed by 'A'.");
}

void test_corrupted_surrogate_no_low() {
    // Surrogate high followed by a non-surrogate low; should fail
    g_seq = { 0xD800, 0x0041 }; // high surrogate then a non-surrogate code unit
    g_pos = 0;
    wchar_t output[4];
    std::memset(output, 0, sizeof(output));

    cmsBool ok = convert_utf16_to_utf32(nullptr, 2, output);

    TEST_ASSERT(ok == FALSE, "Corrupted surrogate (high with non-low) should fail (FALSE).");
    // Ensure no output was written on failure
    TEST_ASSERT(output[0] == 0 || output[0] == (wchar_t)0x0000,
                "Output should remain unchanged on failure (first element unchanged).");
}

void test_insufficient_data_for_surrogate() {
    // High surrogate provided but no following low surrogate data
    g_seq = { 0xD800 }; // only high surrogate, insufficient data for surrogate pair
    g_pos = 0;
    wchar_t output[4];
    std::memset(output, 0, sizeof(output));

    cmsBool ok = convert_utf16_to_utf32(nullptr, 1, output);

    TEST_ASSERT(ok == FALSE, "Insufficient data for surrogate should fail (FALSE).");
    TEST_ASSERT(output[0] == 0 || output[0] == (wchar_t)0x0000,
                "Output should remain unchanged when data is insufficient.");
}

void test_mixed_input_bmp_and_surrogate() {
    // Sequence: 'X' (U+0058), high surrogate + low surrogate for U+1F905, then 'Z'
    // U+1F905 is SUITABLE surrogate pair; let's use a known safe pair: U+1F3A4 (SUSHI CHEF) -> D83C DD A4
    g_seq = { 0x0058, 0xD83C, 0xDDA4, 0x005A }; // 'X', surrogate pair, 'Z'
    g_pos = 0;
    wchar_t output[8];
    std::memset(output, 0, sizeof(output));

    cmsBool ok = convert_utf16_to_utf32(nullptr, 4, output);

    TEST_ASSERT(ok == TRUE, "Mixed BMP and surrogate sequence should succeed.");
    TEST_ASSERT(output[0] == L'X' && output[1] == (wchar_t)0x1F3A4 && output[2] == L'Z',
                "Output should be 'X', U+1F3A4, 'Z'.");
}

} // anonymous namespace

int main() {
    // Run tests
    std::cout << "Starting convert_utf16_to_utf32 tests...\n";

    test_bmp_basic();
    test_surrogate_pair();
    test_corrupted_surrogate_no_low();
    test_insufficient_data_for_surrogate();
    test_mixed_input_bmp_and_surrogate();

    int total = test_passes + test_fails;
    std::cout << "Tests run: " << total
              << " | Passed: " << test_passes
              << " | Failed: " << test_fails
              << "\n";

    // Return non-zero if any test failed
    return (test_fails == 0) ? 0 : 1;
}