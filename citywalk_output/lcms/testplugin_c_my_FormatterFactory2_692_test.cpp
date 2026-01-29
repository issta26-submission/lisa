// Test suite for cmsFormatter my_FormatterFactory2
// Target: ensure correct behavior of the function under various input predicates
// This test does not rely on GTest; a lightweight, non-terminating assertion style is used.

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Include the CMS-related definitions. The tests are designed to work with
// the provided dependencies in testplugin.c environment.

// Ensure the C function under test is visible to C++ code.
#ifdef __cplusplus
extern "C" {
#endif
    cmsFormatter my_FormatterFactory2(cmsUInt32Number Type,
                                    cmsFormatterDirection Dir,
                                    cmsUInt32Number dwFlags);
#ifdef __cplusplus
}
#endif

// Lightweight test harness
static int total_failures = 0;
#define CHECK(cond, msg) do { if(!(cond)) { std::cerr << "CHECK FAILED: " << msg << " [Condition: " << #cond << "]\n"; ++total_failures; } } while(0)

// Test 1: All predicates true -> Fmt16 should be non-NULL
// Purpose: verify that when Type == TYPE_RGB_565, CMS_PACK_FLAGS_FLOAT not set, and Dir == cmsFormatterOutput,
//          the function assigns a valid formatter pointer to Result.Fmt16.
static void test_all_true_sets_fmt16() {
    cmsFormatter res = my_FormatterFactory2(TYPE_RGB_565, cmsFormatterOutput, 0);
    CHECK(res.Fmt16 != NULL, "Fmt16 should be non-NULL when all conditions are true (Type=TYPE_RGB_565, Dir=cmsFormatterOutput, no float flag).");
}

// Test 2: Type != TYPE_RGB_565 -> Fmt16 should be NULL
// Purpose: ensure that incorrect Type prevents assignment.
static void test_type_not_rgb565_null() {
    // Choose a Type value that is not TYPE_RGB_565 (any non-matching value should suffice)
    cmsUInt32Number wrongType = TYPE_RGB_565 + 1; // assuming enum-like increment is safe; otherwise pick a known non-match
    cmsFormatter res = my_FormatterFactory2(wrongType, cmsFormatterOutput, 0);
    CHECK(res.Fmt16 == NULL, "Fmt16 should be NULL when Type != TYPE_RGB_565.");
}

// Test 3: Dir != cmsFormatterOutput -> Fmt16 should be NULL
// Purpose: ensure that only when Dir matches cmsFormatterOutput the formatter is set.
static void test_dir_not_output_null() {
    cmsFormatter res = my_FormatterFactory2(TYPE_RGB_565, cmsFormatterInput, 0);
    CHECK(res.Fmt16 == NULL, "Fmt16 should be NULL when Dir != cmsFormatterOutput.");
}

// Test 4: CMS_PACK_FLAGS_FLOAT bit set in dwFlags -> Fmt16 should be NULL
// Purpose: ensure that the presence of the float flag disables packing formatters.
static void test_float_flag_set_null() {
    cmsFormatter res = my_FormatterFactory2(TYPE_RGB_565, cmsFormatterOutput, CMS_PACK_FLAGS_FLOAT);
    CHECK(res.Fmt16 == NULL, "Fmt16 should be NULL when CMS_PACK_FLAGS_FLOAT bit is set in dwFlags.");
}

// Optional combined-case test: TypeRGB565 + DirOutput + float flag together should still be NULL
static void test_combined_false_when_any_condition_fails() {
    cmsFormatter res = my_FormatterFactory2(TYPE_RGB_565, cmsFormatterOutput, CMS_PACK_FLAGS_FLOAT);
    CHECK(res.Fmt16 == NULL, "Fmt16 should be NULL when any one of the conditions is not satisfied (float flag present).");
}

// Entry point for the test suite
int main() {
    // Run tests
    test_all_true_sets_fmt16();
    test_type_not_rgb565_null();
    test_dir_not_output_null();
    test_float_flag_set_null();
    test_combined_false_when_any_condition_fails();

    // Report summary
    if (total_failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cout << total_failures << " test(s) failed.\n";
    }

    // Return non-zero if any test failed
    return total_failures;
}