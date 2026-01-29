/*
Unit test suite for the focal method From16To15(cmsUInt16Number x16) found in fast_float_15bits.c / fast_float_internal.h.

This test focuses on numeric correctness of the 16-bit to 15-bit conversion as implemented:
r64 = (((cmsUInt64Number)x16 << 15)) / 0xFFFFL;
return (cmsUInt16Number)r64;

Test strategy:
- Cover edge cases (0, 65535) and several intermediate values.
- Validate output against an independent reference calculation using long double to avoid relying on the same integer path.
- Use a small, self-contained test harness (no GTest) with non-terminating assertions (reports all fails and returns a non-zero exit on failure).

Dependencies:
- Include fast_float_internal.h to access types (cmsUInt16Number, cmsUInt64Number, and From16To15).
*/

#include <cstdint>
#include <iostream>
#include <cmath>
#include <fast_float_internal.h>


// Helper: test a single case and report results. Any failure flags the suite as failed.
static bool test_case(const char* name, cmsUInt16Number input) {
    cmsUInt16Number actual = From16To15(input);

    // Independent reference calculation using long double to avoid same-path dependency.
    long double val = ((long double)input) * 32768.0L / 65535.0L;
    cmsUInt16Number expected = (cmsUInt16Number) ( (uint64_t)std::floor(val) );

    if (actual != expected) {
        std::cerr << "From16To15 test FAILED - " 
                  << name << ": input=" << input
                  << ", actual=" << actual
                  << ", expected=" << expected << "\n";
        return true; // failure
    } else {
        std::cout << "From16To15 test PASSED - " << name
                  << " (input=" << input << ", output=" << actual << ")\n";
        return false; // success
    }
}

// Suite: executes a collection of representative inputs, including boundary and random-ish values.
static bool test_From16To15_suite() {
    bool suite_failed = false;

    // Representative set of inputs to exercise edge cases and mid-range behavior.
    struct TestCase { const char* name; uint16_t input; };
    TestCase tests[] = {
        { "zero", 0 },
        { "one", 1 },
        { "two", 2 },
        { "three", 3 },
        { "four", 4 },
        { "five", 5 },
        { "six", 6 },
        { "ten", 10 },
        { "hundred", 100 },
        { "thousand", 1000 },
        { "32767", 32767 },
        { "32768", 32768 },
        { "65534", 65534 },
        { "65535", 65535 },
        { "five_wifty_zero", 50000 }
    };

    const size_t N = sizeof(tests) / sizeof(tests[0]);
    for (size_t i = 0; i < N; ++i) {
        if (test_case(tests[i].name, tests[i].input)) {
            suite_failed = true;
        }
    }

    // Additional stress-ish checks using a few randomly selected values
    if (test_case("random_1", 12345)) suite_failed = true;
    if (test_case("random_2", 54321)) suite_failed = true;
    if (test_case("random_3", 40000)) suite_failed = true;

    return suite_failed;
}

int main() {
    bool failed = test_From16To15_suite();
    if (failed) {
        std::cerr << "Unit test suite for From16To15 FAILED.\n";
        return 1;
    } else {
        std::cout << "Unit test suite for From16To15 PASSED.\n";
        return 0;
    }
}