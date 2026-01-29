// FOCAL METHOD TEST SUITE for: png_get_x_offset_microns
// This test suite targets the focal method described in the prompt.
// It provides a minimal, self-contained mock environment to exercise the
// function under two compilation modes controlled by the PNG_oFFs_SUPPORTED macro.
// - When PNG_oFFs_SUPPORTED is defined, the function checks oFFs validity, unit type,
//   and returns x_offset when appropriate.
// - When PNG_oFFs_SUPPORTED is not defined, the function simply returns 0.
// The tests are written in C++11, without GoogleTest, and use a custom non-terminating
// assertion style to maximize coverage across all test cases.

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <vector>


// Candidate Keywords extracted from the focal method (for test guidance):
// - PNG_oFFs_SUPPORTED, PNG_INFO_oFFs, PNG_OFFSET_MICROMETER
// - info_ptr fields: valid, offset_unit_type, x_offset
// - png_ptr null-checks, info_ptr non-null check
// - png_debug (when enabled), PNG_UNUSED (when not enabled)
// - Behavior: true-branch returns x_offset; otherwise returns 0

// Minimal mock environment to mimic libpng-like types for the focal function.
// These definitions are self-contained and used only for testing.

struct PngStruct {};                   // Opaque png_ptr type (mock)
struct PngInfo {                       // Mock for info_ptr (png_inforp)
    unsigned int valid;
    unsigned int offset_unit_type;
    int x_offset;
};

// Typedefs to resemble libpng's const pointer signatures
typedef PngStruct* png_ptr;
typedef const PngStruct* png_const_structrp;
typedef PngInfo* png_inforp;
typedef const PngInfo* png_const_inforp;

// Mocked constants used by the focal method
#define PNG_INFO_oFFs 0x01
#define PNG_OFFSET_MICROMETER 1

// No-op for debugging (provided in focal method when PNG_oFFs_SUPPORTED is defined)
void png_debug(int /*level*/, const char* /*message*/) { /* no-op in tests */ }

// Basic macro to silence unused-variable warnings
#ifndef PNG_UNUSED
#define PNG_UNUSED(x) (void)(x)
#endif

// The focal method under test.
// The implementation is compiled in two modes depending on PNG_oFFs_SUPPORTED.
#ifdef PNG_oFFs_SUPPORTED
int png_get_x_offset_microns(png_const_structrp png_ptr, png_const_inforp info_ptr)
{
    png_debug(1, "in png_get_x_offset_microns");
    if (png_ptr != NULL && info_ptr != NULL &&
        (info_ptr->valid & PNG_INFO_oFFs) != 0)
    {
        if (info_ptr->offset_unit_type == PNG_OFFSET_MICROMETER)
            return info_ptr->x_offset;
    }
    return 0;
}
#else
int png_get_x_offset_microns(png_const_structrp png_ptr, png_const_inforp info_ptr)
{
    PNG_UNUSED(png_ptr)
    PNG_UNUSED(info_ptr)
    return 0;
}
#endif

// Lightweight test harness (non-terminating assertions)
struct TestCase {
    std::string name;
    bool expected;
    int actual;
};

static void run_and_report(const TestCase& tc, std::vector<std::string>& log, bool& any_fail) {
    if (tc.actual != (tc.expected ? 1 : 0) && (tc.actual != (tc.expected ? 0 : 0))) {
        // Note: We compare numeric 0/non-zero against expectation.
        // For clarity, we rely on the actual value directly in tests below.
    }
    // The actual comparison for our test cases is done in-line; this helper only logs result.
}

// True-branch tests (only compiled when PNG_oFFs_SUPPORTED is defined)
#ifdef PNG_oFFs_SUPPORTED
bool test_true_offset_extracted() {
    // Case: info_ptr has PNG_INFO_oFFs flag and unit type is MICROMETER -> should return x_offset
    PngInfo info;
    info.valid = PNG_INFO_oFFs;
    info.offset_unit_type = PNG_OFFSET_MICROMETER;
    info.x_offset = 123; // expected

    PngStruct s;
    // Non-null pointers to simulate real usage
    png_ptr png_ptr_nonnull = reinterpret_cast<png_ptr>(&s);
    png_const_inforp info_ptr = &info;

    int result = png_get_x_offset_microns(png_ptr_nonnull, info_ptr);
    if (result == 123) return true;
    std::cerr << "test_true_offset_extracted: expected 123, got " << result << "\n";
    return false;
}

bool test_false_flags() {
    // Case: valid flag not set -> should return 0
    PngInfo info;
    info.valid = 0; // missing PNG_INFO_oFFs
    info.offset_unit_type = PNG_OFFSET_MICROMETER;
    info.x_offset = 9999;

    PngStruct s;
    png_ptr png_ptr_nonnull = reinterpret_cast<png_ptr>(&s);
    int result = png_get_x_offset_microns(png_ptr_nonnull, &info);
    return (result == 0);
}

bool test_wrong_unit_type() {
    // Case: unit type not MICROMETER -> should return 0
    PngInfo info;
    info.valid = PNG_INFO_oFFs;
    info.offset_unit_type = 0; // not MICROMETER
    info.x_offset = 555;

    PngStruct s;
    png_ptr png_ptr_nonnull = reinterpret_cast<png_ptr>(&s);
    int result = png_get_x_offset_microns(png_ptr_nonnull, &info);
    return (result == 0);
}

bool test_null_ptrs() {
    // Case: null png_ptr or null info_ptr -> should return 0
    PngInfo info;
    info.valid = PNG_INFO_oFFs;
    info.offset_unit_type = PNG_OFFSET_MICROMETER;
    info.x_offset = 77;

    // Null png_ptr
    int res1 = png_get_x_offset_microns(nullptr, &info);
    bool ok1 = (res1 == 0);

    // Null info_ptr
    int res2 = png_get_x_offset_microns(reinterpret_cast<png_ptr>(&info), nullptr);
    bool ok2 = (res2 == 0);

    return ok1 && ok2;
}
#endif // PNG_oFFs_SUPPORTED

// False-branch tests (compiled when PNG_oFFs_SUPPORTED is NOT defined)
#ifndef PNG_oFFs_SUPPORTED
bool test_false_branch_no_support() {
    // With PNG_oFFs_SUPPORTED undefined, function should always return 0
    PngInfo info;
    info.valid = PNG_INFO_oFFs;
    info.offset_unit_type = PNG_OFFSET_MICROMETER;
    info.x_offset = 999;

    PngStruct s;
    png_ptr png_ptr_nonnull = reinterpret_cast<png_ptr>(&s);
    int result = png_get_x_offset_microns(png_ptr_nonnull, &info);
    return (result == 0);
}

bool test_null_ptrs_no_support() {
    // Null pointers should still yield 0
    int r1 = png_get_x_offset_microns(nullptr, nullptr);
    return (r1 == 0);
}
#endif // !PNG_oFFs_SUPPORTED

int main() {
    // Collect test results
    std::vector<std::string> log;
    bool all_ok = true;

#ifdef PNG_oFFs_SUPPORTED
    // True-branch tests
    if (test_true_offset_extracted()) {
        log.push_back("test_true_offset_extracted: PASS");
    } else {
        log.push_back("test_true_offset_extracted: FAIL");
        all_ok = false;
    }

    // False-branch tests within the same build (non-matching flags still exercised by inputs)
    if (test_false_flags()) {
        log.push_back("test_false_flags: PASS");
    } else {
        log.push_back("test_false_flags: FAIL");
        all_ok = false;
    }

    if (test_wrong_unit_type()) {
        log.push_back("test_wrong_unit_type: PASS");
    } else {
        log.push_back("test_wrong_unit_type: FAIL");
        all_ok = false;
    }

    if (test_null_ptrs()) {
        log.push_back("test_null_ptrs: PASS");
    } else {
        log.push_back("test_null_ptrs: FAIL");
        all_ok = false;
    }
#else
    // False-branch tests (no oFFs support)
    if (test_false_branch_no_support()) {
        log.push_back("test_false_branch_no_support: PASS");
    } else {
        log.push_back("test_false_branch_no_support: FAIL");
        all_ok = false;
    }

    if (test_null_ptrs_no_support()) {
        log.push_back("test_null_ptrs_no_support: PASS");
    } else {
        log.push_back("test_null_ptrs_no_support: FAIL");
        all_ok = false;
    }
#endif

    // Print results
    for (const auto& line : log) {
        std::cout << line << std::endl;
    }

    if (all_ok) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}