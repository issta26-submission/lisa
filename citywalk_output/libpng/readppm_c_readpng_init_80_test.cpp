/*
Unit test suite for the focal method: readpng_init (from readppm.c / readpng.c family)

Notes:
- The tests are written in C++11 without Google Test. A minimal in-file test harness is used.
- The tests exercise various execution paths of readpng_init:
  - Not a PPM/PPM header: expect return 1.
  - Valid P6 header with width/height/maxval 255: expect return 0 and proper outputs.
  - Presence of comment lines before width/height: ensure comments are skipped.
  - Wrong maxval (not 255): expect return 2.
- We use std::tmpfile() to create in-memory temporary files for feeding to readpng_init.
- The tests rely on the global variables exposed by readpng.h (such as color_type, channels, width, height, bit_depth, etc.). They should be accessible via the included header.
- Explanatory comments are added for each unit test.

Compile guidance:
- Ensure the project provides readpng.h and the corresponding implementation that defines readpng_init and the expected global variables.
- Compile with C++11 support, e.g., g++ -std=c++11 -O2 -Wall test_readpng_init.cpp -o test_readpng_init
*/

#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <readpng.h>
#include <stdio.h>
#include <cstring>


// Import C dependencies. Ensure C linkage for the C library functions.
#ifdef __cplusplus
extern "C" {
}
#else
#endif

// Helper: create a temporary FILE* from a string content using std::tmpfile
static FILE* make_input_file(const std::string& content) {
    FILE* f = std::tmpfile();
    if (!f) {
        std::cerr << "Failed to create temporary input file." << std::endl;
        return nullptr;
    }
    if (!content.empty()) {
        size_t written = std::fwrite(content.data(), 1, content.size(), f);
        (void)written;
    }
    std::fflush(f);
    std::rewind(f);
    return f;
}

/*
Test 1: Not a PPM header
- Provide a first line that does not start with 'P' or the second character is not '6'.
- Expect readpng_init to return 1 (error: not a PPM file).
*/
static bool test_NotPPM_Header() {
    // First line is not a valid P6 header (e.g., "XX\n")
    std::string content = "XX\n";
    FILE* f = make_input_file(content);
    if (!f) return false;

    ulg w = 0, h = 0;
    int ret = readpng_init(f, &w, &h);
    std::fclose(f);

    if (ret != 1) {
        std::cerr << "[NotPPM_Header] Expected return 1, got " << ret << std::endl;
        return false;
    }
    return true;
}

/*
Test 2: Valid P6 header with width/height and maxval 255
- First line "P6\n"
- Second line "WIDTH HEIGHT"
- Third line "255"
- Expect readpng_init to return 0 and set pWidth/pHeight, and set bit_depth to 8, color_type to 2, channels to 3
*/
static bool test_P6_BasicParsing() {
    std::string content = "P6\n800 600\n255\n";
    FILE* f = make_input_file(content);
    if (!f) return false;

    ulg w = 0, h = 0;
    int ret = readpng_init(f, &w, &h);
    std::fclose(f);

    bool ok = (ret == 0) && (w == 800) && (h == 600);
    // Expose internal state via globals (as provided by readpng.h)
    if (!ok) {
        std::cerr << "[P6_BasicParsing] Unexpected return or dimensions: ret=" << ret
                  << " w=" << w << " h=" << h << std::endl;
        return false;
    }
    // Check format specifics that are set globally after parsing
    if (bit_depth != 8) {
        std::cerr << "[P6_BasicParsing] bit_depth expected 8, got " << bit_depth << std::endl;
        return false;
    }
    if (color_type != 2) {
        std::cerr << "[P6_BasicParsing] color_type expected 2, got " << color_type << std::endl;
        return false;
    }
    if (channels != 3) {
        std::cerr << "[P6_BasicParsing] channels expected 3, got " << channels << std::endl;
        return false;
    }
    return true;
}

/*
Test 3: P6 header with comments before width/height
- After first line, include comment lines starting with '#'
- Then provide width/height and maxval 255
- Expect readpng_init to return 0 and correct width/height and formatting globals
*/
static bool test_P6_WithCommentsBeforeDims() {
    std::string content = "P6\n# this is a comment\n# another comment\n320 240\n# comment after dims\n255\n";
    FILE* f = make_input_file(content);
    if (!f) return false;

    ulg w = 0, h = 0;
    int ret = readpng_init(f, &w, &h);
    std::fclose(f);

    bool ok = (ret == 0) && (w == 320) && (h == 240);
    if (!ok) {
        std::cerr << "[P6_WithCommentsBeforeDims] Unexpected: ret=" << ret << " w=" << w << " h=" << h << std::endl;
        return false;
    }
    if (bit_depth != 8) {
        std::cerr << "[P6_WithCommentsBeforeDims] bit_depth expected 8, got " << bit_depth << std::endl;
        return false;
    }
    if (color_type != 2) {
        std::cerr << "[P6_WithCommentsBeforeDims] color_type expected 2, got " << color_type << std::endl;
        return false;
    }
    if (channels != 3) {
        std::cerr << "[P6_WithCommentsBeforeDims] channels expected 3, got " << channels << std::endl;
        return false;
    }
    return true;
}

/*
Test 4: P6 header with maxval not equal to 255
- Expect return 2 (error: maxval != 255)
*/
static bool test_P6_MaxvalNot255() {
    std::string content = "P6\n100 100\n254\n";
    FILE* f = make_input_file(content);
    if (!f) return false;

    ulg w = 0, h = 0;
    int ret = readpng_init(f, &w, &h);
    std::fclose(f);

    if (ret != 2) {
        std::cerr << "[P6_MaxvalNot255] Expected return 2, got " << ret << std::endl;
        return false;
    }
    return true;
}

/*
Test 5: P6 header with an invalid second character (e.g., P8)
- This should fail at the first validation check and return 1
*/
static bool test_P6_InvalidSecondChar() {
    // "P8" is provided; since the code enforces 'P' followed by '6', this should error
    std::string content = "P8\n10 10\n255\n";
    FILE* f = make_input_file(content);
    if (!f) return false;

    ulg w = 0, h = 0;
    int ret = readpng_init(f, &w, &h);
    std::fclose(f);

    if (ret != 1) {
        std::cerr << "[P6_InvalidSecondChar] Expected return 1, got " << ret << std::endl;
        return false;
    }
    return true;
}

/*
Main: run all tests and provide a summary.
*/
int main() {
    int passed = 0;
    int total = 5;

    if (test_NotPPM_Header()) {
        std::cout << "[Test 1] NotPPM_Header: PASSED" << std::endl;
        ++passed;
    } else {
        std::cout << "[Test 1] NotPPM_Header: FAILED" << std::endl;
    }

    if (test_P6_BasicParsing()) {
        std::cout << "[Test 2] P6_BasicParsing: PASSED" << std::endl;
        ++passed;
    } else {
        std::cout << "[Test 2] P6_BasicParsing: FAILED" << std::endl;
    }

    if (test_P6_WithCommentsBeforeDims()) {
        std::cout << "[Test 3] P6_WithCommentsBeforeDims: PASSED" << std::endl;
        ++passed;
    } else {
        std::cout << "[Test 3] P6_WithCommentsBeforeDims: FAILED" << std::endl;
    }

    if (test_P6_MaxvalNot255()) {
        std::cout << "[Test 4] P6_MaxvalNot255: PASSED" << std::endl;
        ++passed;
    } else {
        std::cout << "[Test 4] P6_MaxvalNot255: FAILED" << std::endl;
    }

    if (test_P6_InvalidSecondChar()) {
        std::cout << "[Test 5] P6_InvalidSecondChar: PASSED" << std::endl;
        ++passed;
    } else {
        std::cout << "[Test 5] P6_InvalidSecondChar: FAILED" << std::endl;
    }

    std::cout << "Summary: " << passed << " of " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}