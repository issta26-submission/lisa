/*
Candidate Keywords (derived from the focal method and its dependencies):
- parse_hex4, hex digits, 4-digit hex, uppercase, lowercase, case-insensitive parsing
- numeric value, code point, conversion, isxdigit, hex value accumulation
- test coverage: all combinations of 4 hex digits, mixed-case inputs
- Unity test framework: RUN_TEST, TEST_ASSERT*, UnityBegin/End
- C linkage for C functions in C++ tests
*/

#include <unity/src/unity.h>
#include <unity/examples/unity_config.h>
#include <cstdio>
#include <common.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
// Declaration of the focal C function under test.
// We assume the function signature is: int parse_hex4(const char* p);
// If the real signature differs in your project, adjust accordingly.
int parse_hex4(const char* p);
#ifdef __cplusplus
}
#endif


// Test 1: parse_hex4_should_parse_all_combinations
// This test exhaustively verifies that every 4-digit hexadecimal string
// (00 00 to FF FF) is parsed to its exact numeric value (0 to 0xFFFF).
// To avoid stopping on the first mismatch, we accumulate mismatches and
// perform a single assertion at the end, while still printing any details.
static void parse_hex4_should_parse_all_combinations(void)
{
    int mismatch_count = 0;
    char buf[5]; // 4 hex digits + null terminator

    for (uint32_t val = 0; val <= 0xFFFF; ++val)
    {
        // Format the value as a 4-digit uppercase hex string.
        // Example: 0x1A2B -> "1A2B"
        snprintf(buf, sizeof(buf), "%04X", static_cast<unsigned int>(val));

        int parsed = parse_hex4(buf);
        if (parsed != static_cast<int>(val))
        {
            // Record mismatch details for debugging without aborting the test early.
            ++mismatch_count;
            printf("Mismatch for input \"%s\": expected %u, got %d\n",
                   buf, static_cast<unsigned int>(val), parsed);
        }
    }

    // Final assertion: no mismatches should have occurred.
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, mismatch_count,
        "parse_hex4 should correctly parse all 4-digit hex combinations to their numeric value");
}

// Test 2: parse_hex4_should_parse_mixed_case
// This test ensures that the parser handles mixed-case hexadecimal input
// correctly, i.e., it's case-insensitive with respect to A-F/a-f.
static void parse_hex4_should_parse_mixed_case(void)
{
    struct TestCase {
        const char* input;
        unsigned int expected;
    } tests[] = {
        {"aBcD", 0xABCD},
        {"AbCd", 0xABCD},
        {"abcd", 0xABCD},
        {"ABCD", 0xABCD},
        {"1234", 0x1234},
        {"ffff", 0xFFFF},
        {"0a0F", 0x0A0F},
    };

    int mismatch_count = 0;

    for (auto &tc : tests)
    {
        int parsed = parse_hex4(tc.input);
        if (parsed != static_cast<int>(tc.expected))
        {
            ++mismatch_count;
            printf("Mixed-case test input \"%s\": expected %u, got %d\n",
                   tc.input, static_cast<unsigned int>(tc.expected), parsed);
        }
    }

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, mismatch_count,
        "parse_hex4 should be case-insensitive across A-F/a-f and correctly parse values");
}

// Main test runner (Unity-based)
int CJSON_CDECL main(void)
{
    // The main is provided here to run the unit tests without GTest.
    // It invokes the two test cases defined above.
    UNITY_BEGIN();
    RUN_TEST(parse_hex4_should_parse_all_combinations);
    RUN_TEST(parse_hex4_should_parse_mixed_case);
    return UNITY_END();
}