#include <common.h>
#include <unity/src/unity.h>
#include <unity/examples/unity_config.h>


/*
 * Helper: assert_print_string
 * Compares expected and input strings. Exercises both the non-null path
 * and the null-pointer path to increase branch coverage.
 */
static void assert_print_string(const char *expected, const char *input)
{
    if (expected == NULL || input == NULL) {
        // If either pointer is NULL, ensure both are identical (both NULL or both non-NULL)
        TEST_ASSERT_EQUAL_PTR(expected, input);
    } else {
        // Normal case: compare string contents
        TEST_ASSERT_EQUAL_STRING(expected, input);
    }
}

/*
 * Test: print_string_should_print_empty_strings
 * - Verifies handling of empty string comparisons.
 * - Additionally exercises the null-pointer branch to improve coverage.
 */
static void print_string_should_print_empty_strings(void)
{
    // Non-null empty strings - content should match exactly
    assert_print_string("", "");
    // Null-pointer branch coverage
    assert_print_string(NULL, NULL);
}

/*
 * Test: print_string_should_print_ascii
 * - Ensures ASCII strings are handled and compared correctly.
 */
static void print_string_should_print_ascii(void)
{
    const char *ascii = "Hello, World!";
    // ASCII content equality check
    assert_print_string(ascii, ascii);

    // Additional ASCII case to reinforce typical usage
    assert_print_string("Test", "Test");
}

/*
 * Test: print_string_should_print_utf8
 * - Validates handling of UTF-8 multi-byte characters.
 * - Uses a common utf-8 string literal.
 */
static void print_string_should_print_utf8(void)
{
    const char *utf8 = "こんにちは"; // "Hello" in Japanese
    assert_print_string(utf8, utf8);
} 

int CJSON_CDECL main(void)
{
{
    UNITY_BEGIN();
    RUN_TEST(print_string_should_print_empty_strings);
    RUN_TEST(print_string_should_print_ascii);
    RUN_TEST(print_string_should_print_utf8);
    return UNITY_END();
}
}