// Test suite for the focal function: parse_color(char *arg, unsigned int *colors)
// This test suite is designed for C++11, without using GTest.
// It exercises valid inputs that do not trigger exit(1) paths in parse_color.

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Declaration of the C function under test.
// The function is defined in makepng.c (provided in the project).
extern "C" void parse_color(char *arg, unsigned int *colors);

// Helper to declare the test functions
static bool test_no_input();
static bool test_single_color();
static bool test_two_colors();
static bool test_four_colors();

// Entry point for the test suite
int main() {
    int failures = 0;

    struct Test {
        const char* name;
        bool (*func)();
    } tests[] = {
        { "test_no_input", test_no_input },
        { "test_single_color", test_single_color },
        { "test_two_colors", test_two_colors },
        { "test_four_colors", test_four_colors }
    };

    const int N = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < N; ++i) {
        bool ok = tests[i].func();
        if (ok) {
            std::cout << "[PASS] " << tests[i].name << "\n";
        } else {
            std::cerr << "[FAIL] " << tests[i].name << "\n";
            ++failures;
        }
    }

    if (failures) {
        std::cerr << "Total failures: " << failures << " / " << N << "\n";
        return 1;
    } else {
        std::cout << "All tests passed.\n";
        return 0;
    }
}

/*
Test 1: No input
Purpose: Verify that when arg is an empty string, parse_color does not write any colors
and sets colors[0] to 0 (ncolors).
Rationale: Covers the scenario where the while loop is never entered.
*/
static bool test_no_input() {
    unsigned int colors[5];
    char arg[16];
    arg[0] = '\0'; // empty input

    parse_color(arg, colors);

    // Expect 0 colors parsed; colors[0] holds the count
    if (colors[0] != 0) {
        std::cerr << "test_no_input: expected colors[0]==0, got " << colors[0] << "\n";
        return false;
    }
    // Also ensure no unintended writes beyond index 0
    // Note: colors[1..4] are unspecified, but in well-behaved runs they should remain unchanged.
    return true;
}

/*
Test 2: Single color
Purpose: Verify that a single numeric color value is parsed correctly.
Input: "123" -> single color 123
Expected: colors[0] == 1, colors[1] == 123
*/
static bool test_single_color() {
    unsigned int colors[5];
    char arg[16];
    std::strcpy(arg, "123"); // within 0..65535

    parse_color(arg, colors);

    if (colors[0] != 1) {
        std::cerr << "test_single_color: expected colors[0]==1, got " << colors[0] << "\n";
        return false;
    }
    if (colors[1] != 123) {
        std::cerr << "test_single_color: expected colors[1]==123, got " << colors[1] << "\n";
        return false;
    }
    return true;
}

/*
Test 3: Two colors with a separator
Purpose: Verify that two values separated by a non-digit separator are parsed correctly.
Input: "12,34" -> 12 and 34
Expected: colors[0] == 2, colors[1] == 12, colors[2] == 34
*/
static bool test_two_colors() {
    unsigned int colors[5];
    char arg[16];
    std::strcpy(arg, "12,34");

    parse_color(arg, colors);

    if (colors[0] != 2) {
        std::cerr << "test_two_colors: expected colors[0]==2, got " << colors[0] << "\n";
        return false;
    }
    if (colors[1] != 12) {
        std::cerr << "test_two_colors: expected colors[1]==12, got " << colors[1] << "\n";
        return false;
    }
    if (colors[2] != 34) {
        std::cerr << "test_two_colors: expected colors[2]==34, got " << colors[2] << "\n";
        return false;
    }
    return true;
}

/*
Test 4: Four colors
Purpose: Verify that four values are parsed correctly and are placed in colors[1..4],
with colors[0] storing the count.
Input: "1 2 3 4" -> four colors separated by spaces
Expected: colors[0] == 4, colors[1]..colors[4] == 1,2,3,4
*/
static bool test_four_colors() {
    unsigned int colors[5];
    char arg[16];
    std::strcpy(arg, "1 2 3 4");

    parse_color(arg, colors);

    if (colors[0] != 4) {
        std::cerr << "test_four_colors: expected colors[0]==4, got " << colors[0] << "\n";
        return false;
    }
    if (colors[1] != 1) {
        std::cerr << "test_four_colors: expected colors[1]==1, got " << colors[1] << "\n";
        return false;
    }
    if (colors[2] != 2) {
        std::cerr << "test_four_colors: expected colors[2]==2, got " << colors[2] << "\n";
        return false;
    }
    if (colors[3] != 3) {
        std::cerr << "test_four_colors: expected colors[3]==3, got " << colors[3] << "\n";
        return false;
    }
    if (colors[4] != 4) {
        std::cerr << "test_four_colors: expected colors[4]==4, got " << colors[4] << "\n";
        return false;
    }
    return true;
}