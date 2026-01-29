#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstddef>


// Forward declare the C function under test with C linkage.
// We provide a minimal typedef to match the expected signature.
// The real implementation is assumed to be linked from the project under test.
extern "C" {
    typedef const char* png_const_charp;
    int png_check_fp_number(png_const_charp string, size_t size, int *statep,
                            size_t *whereami);
}

// Simple test harness (non-GTest) with explanatory comments for each unit test.

static bool test_empty_string_impl() {
    // Test: empty input should not be considered a valid floating-point number.
    // Expectation: return value should be false (0), and state/whereami unchanged.
    const char *str = "";
    size_t size = 0;
    int state = 0;
    size_t whereami = 0;

    int result = png_check_fp_number(str, size, &state, &whereami);

    bool ok = (result == 0) && (whereami == 0) && (state == 0);
    if (!ok) {
        std::cerr << "test_empty_string_impl failed: result=" << result
                  << " whereami=" << whereami << " state=" << state << "\n";
    }
    return ok;
}

static bool test_simple_integer_impl() {
    // Test: a simple integer without decimal point.
    // Input: "123" should be accepted as a valid number; whereami should advance to 3.
    const char *str = "123";
    size_t size = 3;
    int state = 0;
    size_t whereami = 0;

    int result = png_check_fp_number(str, size, &state, &whereami);

    bool ok = (result != 0) && (whereami == 3);
    if (!ok) {
        std::cerr << "test_simple_integer_impl failed: result=" << result
                  << " whereami=" << whereami << " state=" << state << "\n";
    }
    return ok;
}

static bool test_decimal_impl() {
    // Test: decimal number with fraction part.
    // Input: "12.34" should be accepted; whereami should be 5.
    const char *str = "12.34";
    size_t size = 5;
    int state = 0;
    size_t whereami = 0;

    int result = png_check_fp_number(str, size, &state, &whereami);

    bool ok = (result != 0) && (whereami == 5);
    if (!ok) {
        std::cerr << "test_decimal_impl failed: result=" << result
                  << " whereami=" << whereami << " state=" << state << "\n";
    }
    return ok;
}

static bool test_sign-prefixed_impl() {
    // Test: a sign-prefixed integer.
    // Input: "+123" should be accepted; whereami should be 4.
    const char *str = "+123";
    size_t size = 4;
    int state = 0;
    size_t whereami = 0;

    int result = png_check_fp_number(str, size, &state, &whereami);

    bool ok = (result != 0) && (whereami == 4);
    if (!ok) {
        std::cerr << "test_sign-prefixed_impl failed: result=" << result
                  << " whereami=" << whereami << " state=" << state << "\n";
    }
    return ok;
}

static bool test_exponent_impl() {
    // Test: number with exponent part.
    // Input: "1e10" should be accepted; whereami should be 4.
    const char *str = "1e10";
    size_t size = 4;
    int state = 0;
    size_t whereami = 0;

    int result = png_check_fp_number(str, size, &state, &whereami);

    bool ok = (result != 0) && (whereami == 4);
    if (!ok) {
        std::cerr << "test_exponent_impl failed: result=" << result
                  << " whereami=" << whereami << " state=" << state << "\n";
    }
    return ok;
}

static bool test_trailing_chars_after_number_impl() {
    // Test: number followed by non-numeric characters.
    // Input: "123abc" should be accepted up to the numeric prefix; whereami should be 3.
    const char *str = "123abc";
    size_t size = 6;
    int state = 0;
    size_t whereami = 0;

    int result = png_check_fp_number(str, size, &state, &whereami);

    bool ok = (result != 0) && (whereami == 3);
    if (!ok) {
        std::cerr << "test_trailing_chars_after_number_impl failed: result=" << result
                  << " whereami=" << whereami << " state=" << state << "\n";
    }
    return ok;
}

static bool test_dot_only_fraction_impl() {
    // Test: a leading dot with a fractional digit following.
    // Input: ".5" should be accepted; whereami should be 2.
    const char *str = ".5";
    size_t size = 2;
    int state = 0;
    size_t whereami = 0;

    int result = png_check_fp_number(str, size, &state, &whereami);

    bool ok = (result != 0) && (whereami == 2);
    if (!ok) {
        std::cerr << "test_dot_only_fraction_impl failed: result=" << result
                  << " whereami=" << whereami << " state=" << state << "\n";
    }
    return ok;
}

static bool test_trailing_dot_impl() {
    // Test: number with trailing dot but no digits after the dot.
    // Input: "123." should NOT be considered a valid number; whereami should be 4 and result false.
    const char *str = "123.";
    size_t size = 4;
    int state = 0;
    size_t whereami = 0;

    int result = png_check_fp_number(str, size, &state, &whereami);

    bool ok = (result == 0) && (whereami == 4);
    if (!ok) {
        std::cerr << "test_trailing_dot_impl failed: result=" << result
                  << " whereami=" << whereami << " state=" << state << "\n";
    }
    return ok;
}

int main() {
    // Execute all tests and report a concise summary.
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool ok){
        ++total;
        if (ok) {
            ++passed;
            std::cout << "PASS: " << name << "\n";
        } else {
            std::cerr << "FAIL: " << name << "\n";
        }
    };

    run("test_empty_string_impl", test_empty_string_impl());
    run("test_simple_integer_impl", test_simple_integer_impl());
    run("test_decimal_impl", test_decimal_impl());
    run("test_sign-prefixed_impl", test_sign-prefixed_impl());
    run("test_exponent_impl", test_exponent_impl());
    run("test_trailing_chars_after_number_impl", test_trailing_chars_after_number_impl());
    run("test_dot_only_fraction_impl", test_dot_only_fraction_impl());
    run("test_trailing_dot_impl", test_trailing_dot_impl());

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}