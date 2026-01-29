// Unit test suite for modifier_current_encoding in C code (pngvalid.c)
// Target: C++11 test harness (no GTest), using direct main() invocation.
// The tests rely on the actual definitions of png_modifier and color_encoding
// from the included png.h. We use extern "C" to call the C function under test.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Bring in the C declarations
extern "C" {

// Declaration of the function under test (matching its C signature)
void modifier_current_encoding(const png_modifier *pm, color_encoding *ce);
}

// Lightweight test harness macros (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        fprintf(stderr, "Expectation failed: %s, at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

// Test 1: When current_encoding is non-null, the function should copy the encoding
// and then set gamma to the current_gamma value.
void test_modifier_current_encoding_copies_when_encoding_present()
{
    // Prepare a non-null source encoding with deterministic bytes
    color_encoding src;
    // Fill the entire structure with a recognizable pattern
    std::memset(&src, 0xAA, sizeof(src));
    // Ensure gamma has a known value in the source before copy (it will be overwritten)
    src.gamma = 7.7;

    // Prepare modifier with current_encoding pointing to src and a known gamma
    png_modifier pm;
    pm.current_encoding = &src;
    pm.current_gamma = 2.5; // value that should end up in ce.gamma

    // Destination encoding
    color_encoding ce;
    // Call the function under test
    modifier_current_encoding(&pm, &ce);

    // Expected: a copy of src with gamma overridden by current_gamma
    color_encoding expected;
    std::memcpy(&expected, &src, sizeof(src));
    expected.gamma = pm.current_gamma;

    // Compare memory contents to verify full-structure copy plus gamma override
    bool equal = (std::memcmp(&ce, &expected, sizeof(expected)) == 0);
    EXPECT_TRUE(equal);
}

// Test 2: When current_encoding is null, the function should zero the destination
// encoding and then set gamma to the current_gamma value.
void test_modifier_current_encoding_zeroes_when_encoding_null()
{
    // Prepare modifier with null current_encoding and a specific gamma
    png_modifier pm;
    pm.current_encoding = nullptr;
    pm.current_gamma = 1.25;

    // Destination encoding initialized to a non-zero pattern to ensure memset is exercised
    color_encoding ce;
    std::memset(&ce, 0xFF, sizeof(ce));

    // Call the function under test
    modifier_current_encoding(&pm, &ce);

    // Expected: all-zero structure with gamma set to current_gamma
    color_encoding expected;
    std::memset(&expected, 0, sizeof(expected));
    expected.gamma = pm.current_gamma;

    bool equal = (std::memcmp(&ce, &expected, sizeof(expected)) == 0);
    EXPECT_TRUE(equal);
}

int main(void)
{
    // Run tests
    test_modifier_current_encoding_copies_when_encoding_present();
    test_modifier_current_encoding_zeroes_when_encoding_null();

    if (g_failures != 0) {
        fprintf(stderr, "TEST SUMMARY: %d failure(s) detected.\n", g_failures);
        return 1;
    } else {
        fprintf(stderr, "TEST SUMMARY: All tests passed.\n");
        return 0;
    }
}