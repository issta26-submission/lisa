// Test suite for the focal method: standard_check_text (pngvalid.c)
// This test suite is written in C++11 (no GTest) and uses a C linkage
// to call into the C function under test. It relies on libpng headers
// to provide the actual types (png_text, TEXT_COMPRESSION, etc.).
// A lightweight setjmp/longjmp-based error capture is used to avoid
// terminating the program in the C code under test.

#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Step 1: Provide C linkage for the focal function and relevant types
extern "C" {
    // Prototype of the focal function under test
    void standard_check_text(png_const_structp pp, png_const_textp tp,
                             png_const_charp keyword, png_const_charp text);

    // PNG error hook used by the code under test (we implement our own to capture messages)
    void png_error(png_const_structp pp, png_const_charp msg);
}

// Step 1 Candidate Keywords (conceptual understanding for test coverage)
// - tp->compression (TEXT_COMPRESSION path vs non-TEXT_COMPRESSION)
// - tp->key (matching keyword vs NULL/mismatching keyword)
// - tp->text (NULL -> "text lost", non-NULL -> content/length checks)
// - tp->text_length vs strlen(text) (length change reporting)
// - tp->itxt_length (non-zero -> "iTXt length set")
// - tp->lang (non-NULL -> language reporting)
// - tp->lang_key (non-NULL -> iTXt keyword reporting)

// Global state for capturing error messages via png_error
static jmp_buf g_png_test_env;
static char g_last_error[4096];

// Our png_error replacement to capture messages and perform non-local jumps
extern "C" void png_error(png_const_structp pp, png_const_charp msg)
{
    // Copy the message for verification by tests
    if (msg) {
        std::strncpy(g_last_error, msg, sizeof(g_last_error) - 1);
        g_last_error[sizeof(g_last_error) - 1] = '\0';
    } else {
        g_last_error[0] = '\0';
    }
    // Jump back to the test harness to record the error without terminating the process
    longjmp(g_png_test_env, 1);
}

// Helper to run a single test case
static bool run_test_case(const char* test_name,
                          png_text* tp,                 // input text record
                          const char* keyword,            // keyword input
                          const char* text,               // text input (can be NULL)
                          bool expect_error,                // whether an error is expected
                          const char* expected_substring)     // substring to look for in error
{
    // Ensure previous error message is cleared
    g_last_error[0] = '\0';

    int jmp_val = setjmp(g_png_test_env);
    if (jmp_val == 0) {
        // Call the focal function. If it triggers an error, png_error will longjmp back here.
        // Important: We pass the actual libpng structures/types so behavior matches production.
        standard_check_text(nullptr, (png_const_textp)tp, keyword, text);
        // If we reach here, no error occurred
        if (expect_error) {
            printf("  [FAILED] %s: expected an error but none occurred.\n", test_name);
            return false;
        } else {
            printf("  [PASSED] %s: no error as expected.\n", test_name);
            return true;
        }
    } else {
        // Returned via longjmp from png_error
        if (!expect_error) {
            printf("  [FAILED] %s: unexpected error: %s\n", test_name, g_last_error);
            return false;
        } else {
            // If an expected error substring is provided, verify it is contained
            if (expected_substring != nullptr) {
                if (std::strstr(g_last_error, expected_substring) != nullptr) {
                    printf("  [PASSED] %s: error matched expected substring.\n", test_name);
                    return true;
                } else {
                    printf("  [FAILED] %s: error did not contain expected substring. Got: %s\n",
                           test_name, g_last_error);
                    return false;
                }
            } else {
                // No specific substring required; any error is considered a pass
                printf("  [PASSED] %s: error occurred as expected.\n", test_name);
                return true;
            }
        }
    }
}

// Test 1: No error path when all fields match expected values
static bool test_no_error_path()
{
    printf("Test: No error path when all fields align (compression, key, text).\n");

    static char key1[] = "myKey";
    static char text1[] = "HelloWorld";

    png_text tp;
    tp.compression = TEXT_COMPRESSION; // should equal expected compression path
    tp.key = key1;
    tp.text = text1;
    tp.text_length = std::strlen(text1);
    tp.itxt_length = 0;
    tp.lang = NULL;
    tp.lang_key = NULL;

    const char* keyword = key1;
    const char* text = text1;

    return run_test_case("NoErrorPath", &tp, keyword, text, false, nullptr);
}

// Test 2: Compression mismatch triggers error
static bool test_compression_mismatch()
{
    printf("Test: Compression mismatch triggers error with report including compression matrix.\n");

    static char key1[] = "myKey";
    static char text1[] = "HelloWorld";

    png_text tp;
    tp.compression = TEXT_COMPRESSION + 1; // force non-equal compression value
    tp.key = key1;
    tp.text = text1;
    tp.text_length = std::strlen(text1);
    tp.itxt_length = 0;
    tp.lang = NULL;
    tp.lang_key = NULL;

    const char* keyword = key1;
    const char* text = text1;

    // Expect an error containing "compression [0->"
    return run_test_case("CompressionMismatch", &tp, keyword, text, true,
                         "compression [0->");
}

// Test 3: Keyword mismatch triggers error referencing incorrect keyword
static bool test_keyword_mismatch()
{
    printf("Test: Keyword mismatch triggers error and prints the mismatching keyword.\n");

    static char key1[] = "actualKey";
    static char key2[] = "expectedKey";
    static char text1[] = "SampleText";

    png_text tp;
    tp.compression = TEXT_COMPRESSION; // keep compression normal
    tp.key = key1;                     // mismatching key
    tp.text = text1;
    tp.text_length = std::strlen(text1);
    tp.itxt_length = 0;
    tp.lang = NULL;
    tp.lang_key = NULL;

    const char* keyword = key2;        // expected keyword
    const char* text = text1;

    // Expect an error containing 'keyword "actualKey"'
    return run_test_case("KeywordMismatch", &tp, keyword, text, true,
                         "keyword \"actualKey\"");
}

// Test 4: Text missing triggers "text lost" path
static bool test_text_missing()
{
    printf("Test: Text missing triggers 'text lost' message.\n");

    static char key1[] = "myKey";

    png_text tp;
    tp.compression = TEXT_COMPRESSION;
    tp.key = key1;
    tp.text = NULL;                   // text missing
    tp.text_length = 0;
    tp.itxt_length = 0;
    tp.lang = NULL;
    tp.lang_key = NULL;

    const char* keyword = key1;
    const char* text = "Irrelevant";

    // Expect an error containing 'text lost'
    return run_test_case("TextMissing", &tp, keyword, text, true,
                         "text lost");
}

// Test 5: Text length change triggers a length mismatch message
static bool test_text_length_change()
{
    printf("Test: Text length change triggers length mismatch reporting.\n");

    static char key1[] = "myKey";
    static char text_actual[] = "abcdef"; // 6 chars

    png_text tp;
    tp.compression = TEXT_COMPRESSION;
    tp.key = key1;
    tp.text = text_actual;
    tp.text_length = 3; // intentionally wrong length to trigger message
    tp.itxt_length = 0;
    tp.lang = NULL;
    tp.lang_key = NULL;

    const char* keyword = key1;
    const char* text = text_actual;

    // Expect a length change message like "text length changed[6->3]"
    return run_test_case("TextLengthChange", &tp, keyword, text, true,
                         "text length changed[");
}

int main(void)
{
    // Run all tests, collect and report results
    bool passes = true;

    passes &= test_no_error_path();
    passes &= test_compression_mismatch();
    passes &= test_keyword_mismatch();
    passes &= test_text_missing();
    passes &= test_text_length_change();

    if (passes)
        printf("All tests PASSED.\n");
    else
        printf("Some tests FAILED.\n");

    // The test harness ends here; helper code prints per-test results.
    // Return 0 for success, non-zero otherwise (non-strict requirement; we keep 0/1 style).
    return passes ? 0 : 1;
}