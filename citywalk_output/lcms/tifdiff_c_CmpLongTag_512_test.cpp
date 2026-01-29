/*
Step 1: Candidate Keywords extracted from the focal method CmpLongTag in tifdiff.c
- TIFF*: TIFF image handle type
- TIFFGetField: reads a field (tag) value from a TIFF file
- TIFFSetField: writes a field (tag) value to a TIFF file
- TIFFTAG_IMAGEWIDTH: a tag representing the image width (uint32)
- uint32: 32-bit unsigned integer type used by TIFF fields
- tag (int): the tag identifier to fetch/set
- early-return on missing fields (returns 0)
- equality comparison v1 == v2 (returns 1 if equal, else 0)
- CmpLongTag: the function under test
This test suite focuses on validating the behavior of CmpLongTag against true/false branches,
presence/absence of fields, and value equality.

Step 2: Unit Test Generation
The tests below rely on the real libtiff API (TIFFOpen, TIFFSetField, TIFFGetField, TIFFClose),
to exercise CmpLongTag with various scenarios:
- Both TIFFs have the tag with equal values -> expect 1
- Both TIFFs have the tag with different values -> expect 0
- First TIFF missing the tag -> expect 0
- Second TIFF missing the tag -> expect 0
- Both TIFFs missing the tag -> expect 0

Step 3: Test Case Refinement
Tests are implemented in plain C++11 (no GTest or GMock). They create temporary TIFF files
in the current directory, write fields using TIFFSetField, invoke CmpLongTag, then clean up
the files. Each test has explanatory comments describing its purpose. Assertions are done via
custom non-terminating checks so all tests run in a single pass.

Note: The tests assume libtiff is available and that CmpLongTag is linked from tifdiff.c.
To build: g++ -std=c++11 -o test_cmp_long_tag test_cmp_long_tag.cpp tifdiff.c -ltiff

Now the test code:

*/

#include <cstdio>
#include <iostream>
#include <string>
#include <utils.h>
#include <tiffio.h>


// Forward declaration of the focal function (C linkage expected)
extern "C" int CmpLongTag(TIFF* tif1, TIFF* tif2, int tag);

/*
Companion testing harness and helpers
- Generates simple TIFF files, optionally writing the targeted tag (TIFFTAG_IMAGEWIDTH)
- Runs a set of focused tests and prints pass/fail per test
- Keeps a running summary (non-terminating assertions)
*/

// Global counters for test summary
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Generates a unique path for temporary TIFF files (in current directory)
static std::string make_path(int id) {
    char buf[64];
    std::snprintf(buf, sizeof(buf), "tifdiff_test_%d.tiff", id);
    return std::string(buf);
}

// Creates a TIFF file at `path`.
// If setWidth is true, writes TIFFTAG_IMAGEWIDTH with given width.
static TIFF* createTiff(const std::string& path, bool setWidth, uint32_t width) {
    TIFF* t = TIFFOpen(path.c_str(), "w");
    if (!t) return nullptr;
    if (setWidth) {
        TIFFSetField(t, TIFFTAG_IMAGEWIDTH, width);
    }
    return t;
}

// Marks a single test result and prints a short summary line.
static void end_test(const std::string& name, bool passed) {
    ++g_total_tests;
    if (passed) {
        std::cout << "[PASS] " << name << std::endl;
    } else {
        ++g_failed_tests;
        std::cout << "[FAIL] " << name << std::endl;
    }
}

// Test 1: Both TIFFs contain the tag TIFFTAG_IMAGEWIDTH with equal values.
// Expect CmpLongTag to return 1 (true).
static void test_both_present_equal() {
    auto p1 = make_path(1);
    auto p2 = make_path(2);

    TIFF* t1 = createTiff(p1, true, 100);
    TIFF* t2 = createTiff(p2, true, 100);

    if (!t1 || !t2) {
        end_test("test_both_present_equal: TIFFOpen failed to create test files", false);
        if (t1) TIFFClose(t1);
        if (t2) TIFFClose(t2);
        std::remove(p1.c_str());
        std::remove(p2.c_str());
        return;
    }

    int res = CmpLongTag(t1, t2, TIFFTAG_IMAGEWIDTH);

    TIFFClose(t1);
    TIFFClose(t2);
    std::remove(p1.c_str());
    std::remove(p2.c_str());

    end_test("test_both_present_equal: equal widths should return 1", res == 1);
}

// Test 2: Both TIFFs contain the tag with different values.
// Expect CmpLongTag to return 0 (false).
static void test_both_present_unequal() {
    auto p1 = make_path(3);
    auto p2 = make_path(4);

    TIFF* t1 = createTiff(p1, true, 123);
    TIFF* t2 = createTiff(p2, true, 456);

    if (!t1 || !t2) {
        end_test("test_both_present_unequal: TIFFOpen failed", false);
        if (t1) TIFFClose(t1);
        if (t2) TIFFClose(t2);
        std::remove(p1.c_str());
        std::remove(p2.c_str());
        return;
    }

    int res = CmpLongTag(t1, t2, TIFFTAG_IMAGEWIDTH);

    TIFFClose(t1);
    TIFFClose(t2);
    std::remove(p1.c_str());
    std::remove(p2.c_str());

    end_test("test_both_present_unequal: unequal widths should return 0", res == 0);
}

// Test 3: First TIFF lacks the tag while the second TIFF has the tag.
// Expect CmpLongTag to return 0 due to missing field in tif1.
static void test_first_missing_field() {
    auto p1 = make_path(5);
    auto p2 = make_path(6);

    TIFF* t1 = createTiff(p1, false, 0); // do not set width
    TIFF* t2 = createTiff(p2, true, 50);

    if (!t1 || !t2) {
        end_test("test_first_missing_field: TIFFOpen failed", false);
        if (t1) TIFFClose(t1);
        if (t2) TIFFClose(t2);
        std::remove(p1.c_str());
        std::remove(p2.c_str());
        return;
    }

    int res = CmpLongTag(t1, t2, TIFFTAG_IMAGEWIDTH);

    TIFFClose(t1);
    TIFFClose(t2);
    std::remove(p1.c_str());
    std::remove(p2.c_str());

    end_test("test_first_missing_field: first TIFF missing tag should return 0", res == 0);
}

// Test 4: Second TIFF lacks the tag while the first TIFF has the tag.
// Expect CmpLongTag to return 0 due to missing field in tif2.
static void test_second_missing_field() {
    auto p1 = make_path(7);
    auto p2 = make_path(8);

    TIFF* t1 = createTiff(p1, true, 77);
    TIFF* t2 = createTiff(p2, false, 0); // do not set width

    if (!t1 || !t2) {
        end_test("test_second_missing_field: TIFFOpen failed", false);
        if (t1) TIFFClose(t1);
        if (t2) TIFFClose(t2);
        std::remove(p1.c_str());
        std::remove(p2.c_str());
        return;
    }

    int res = CmpLongTag(t1, t2, TIFFTAG_IMAGEWIDTH);

    TIFFClose(t1);
    TIFFClose(t2);
    std::remove(p1.c_str());
    std::remove(p2.c_str());

    end_test("test_second_missing_field: second TIFF missing tag should return 0", res == 0);
}

// Test 5: Both TIFFs lack the tag.
// Expect CmpLongTag to return 0 due to missing field in tif1 (first check fails).
static void test_both_missing_field() {
    auto p1 = make_path(9);
    auto p2 = make_path(10);

    TIFF* t1 = createTiff(p1, false, 0);
    TIFF* t2 = createTiff(p2, false, 0);

    if (!t1 || !t2) {
        end_test("test_both_missing_field: TIFFOpen failed", false);
        if (t1) TIFFClose(t1);
        if (t2) TIFFClose(t2);
        std::remove(p1.c_str());
        std::remove(p2.c_str());
        return;
    }

    int res = CmpLongTag(t1, t2, TIFFTAG_IMAGEWIDTH);

    TIFFClose(t1);
    TIFFClose(t2);
    std::remove(p1.c_str());
    std::remove(p2.c_str());

    end_test("test_both_missing_field: both missing should return 0", res == 0);
}

// Run all tests
static void run_all_tests() {
    test_both_present_equal();
    test_both_present_unequal();
    test_first_missing_field();
    test_second_missing_field();
    test_both_missing_field();
}

// Entry point
int main(int argc, char* argv[]) {
    (void)argc; (void)argv; // suppress unused parameter warnings

    std::cout << "Starting CmpLongTag unit tests (libtiff-based, C++11)..." << std::endl;
    run_all_tests();

    int passed = g_total_tests - g_failed_tests;
    std::cout << "Summary: " << passed << " / " << g_total_tests << " tests passed. "
              << g_failed_tests << " failed." << std::endl;

    return g_failed_tests == 0 ? 0 : 1;
}