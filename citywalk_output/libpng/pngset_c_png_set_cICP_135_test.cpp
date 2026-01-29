// C++11 test suite for the focal function png_set_cICP
// This test suite provides a minimal, self-contained harness that mimics the
// essential parts of the relevant libpng structures and functions needed to
// verify the behavior of png_set_cICP without requiring the actual libpng
// library. It covers true/false branches of conditions and uses simple
// non-terminating assertions (returning false from test functions on failure).

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>

using std::string;

// Namespace encapsulating the minimal, testable subset of the PNG-related API.
// This is a lightweight stand-in for the real C API to enable unit testing
// of the focal method in isolation.
namespace png_stub {

// Typedefs and constants mirroring the essential parts used by png_set_cICP.
typedef unsigned char png_byte;
#define PNG_INFO_cICP 0x01

// Forward declaration of structs to mimic libpng types.
struct png_struct;
struct png_info;

// Pointers to the opaque structures used by the API.
typedef png_struct* png_const_structrp;
typedef png_struct* png_structrp;
typedef png_info* png_inforp;

// Minimal png_struct to satisfy pointer checks in the unit test.
struct png_struct {
    // Intentionally left empty; the focal function only checks for NULL.
};

// Minimal png_info containing only fields touched by png_set_cICP.
struct png_info {
    unsigned int cicp_colour_primaries;
    unsigned int cicp_transfer_function;
    unsigned int cicp_matrix_coefficients;
    unsigned int cicp_video_full_range_flag;
    unsigned int valid; // bitset; we only care about PNG_INFO_cICP
};

// Internal test-state for capturing warnings emitted by the focal function.
static string last_warning;

// Stubbed diagnostic function signature used by the focal function.
void png_debug1(int /*level*/, const char* /*fmt*/, const char* /*arg*/) {
    // No-op for unit testing; avoids extra output.
}

// Stubbed warning handler that records the last warning message.
void png_warning(png_const_structrp /*png_ptr*/, const char* msg) {
    if (msg != nullptr) {
        last_warning = msg;
    } else {
        last_warning.clear();
    }
}

// The focal function under test, implemented here in a self-contained form.
// It mirrors the logic exactly as given in the problem statement.
void png_set_cICP(png_const_structrp png_ptr, png_inforp info_ptr,
                 png_byte colour_primaries, png_byte transfer_function,
                 png_byte matrix_coefficients, png_byte video_full_range_flag)
{
    // Log/trace (no-op in tests, provided for completeness)
    png_debug1(1, "in %s storage function", "cICP");

    if (png_ptr == NULL || info_ptr == NULL)
        return;

    info_ptr->cicp_colour_primaries = colour_primaries;
    info_ptr->cicp_transfer_function = transfer_function;
    info_ptr->cicp_matrix_coefficients = matrix_coefficients;
    info_ptr->cicp_video_full_range_flag = video_full_range_flag;

    if (info_ptr->cicp_matrix_coefficients != 0)
    {
        png_warning(png_ptr, "Invalid cICP matrix coefficients");
        return;
    }

    info_ptr->valid |= PNG_INFO_cICP;
}

// Helper function to reset the test-state before each test case.
void reset_state() {
    last_warning.clear();
}

} // namespace png_stub

// Convenience alias to bring test symbols into the global namespace for simplicity.
using png_stub::png_ptr;
using png_stub::png_inforp;
using png_stub::png_const_structrp;
using png_stub::png_structrp;
using png_stub::png_info;
using png_stub::png_byte;
using png_stub::PNG_INFO_cICP;
using png_stub::png_set_cICP;
using png_stub::png_debug1;
using png_stub::png_warning;
using png_stub::png_struct;
using png_stub::png_inforp;

// Ensure the test code uses the same function signature as the focal function.
// For clarity in tests, declare a small wrapper to initialize info structs.
static void init_info(png_stub::png_info* info) {
    if (info) {
        info->cicp_colour_primaries = 0;
        info->cicp_transfer_function = 0;
        info->cicp_matrix_coefficients = 0;
        info->cicp_video_full_range_flag = 0;
        info->valid = 0;
    }
}

// Test 1: Null png_ptr should cause no modification to info_ptr and no warnings.
// This covers the true branch where the early return occurs due to NULL ptr.
bool test_null_png_ptr_does_nothing() {
    using namespace png_stub;

    reset_state();

    // Prepare a non-null info_ptr to observe any unintended changes.
    png_info info;
    init_info(&info);

    // Call with png_ptr = NULL
    png_const_structrp null_png_ptr = NULL;
    png_inforp info_ptr = &info;

    png_set_cICP(null_png_ptr, info_ptr,
                 1, 2, 0, 1);

    // Validate that no fields were modified.
    bool ok = true;
    if (info.cicp_colour_primaries != 0) {
        std::cout << "Test 1 FAILED: cicp_colour_primaries changed on NULL png_ptr.\n";
        ok = false;
    }
    if (info.cicp_transfer_function != 0) {
        std::cout << "Test 1 FAILED: cicp_transfer_function changed on NULL png_ptr.\n";
        ok = false;
    }
    if (info.cicp_matrix_coefficients != 0) {
        std::cout << "Test 1 FAILED: cicp_matrix_coefficients changed on NULL png_ptr.\n";
        ok = false;
    }
    if (info.cicp_video_full_range_flag != 0) {
        std::cout << "Test 1 FAILED: cicp_video_full_range_flag changed on NULL png_ptr.\n";
        ok = false;
    }
    if (info.valid != 0) {
        std::cout << "Test 1 FAILED: valid flag changed on NULL png_ptr.\n";
        ok = false;
    }

    // No warning should be issued in this early-return branch.
    if (!png_stub::last_warning.empty()) {
        std::cout << "Test 1 FAILED: Unexpected warning when png_ptr is NULL.\n";
        ok = false;
    }

    return ok;
}

// Test 2: Non-zero matrix_coefficients should trigger a warning and not set PNG_INFO_cICP.
// This covers the false branch where a non-zero matrix coefficient causes early return.
bool test_nonzero_matrix_coefficients_emits_warning_and_does_not_set_valid() {
    using namespace png_stub;

    reset_state();

    // Prepare info_ptr with known initial state.
    png_info info;
    init_info(&info);

    // Prepare non-null png_ptr and non-zero matrix coefficients.
    png_const_structrp fake_png_ptr = reinterpret_cast<png_const_structrp>(0x1); // non-null placeholder
    png_inforp info_ptr = &info;

    png_set_cICP(fake_png_ptr, info_ptr,
                 5, 6, 2, 1); // matrix_coefficients = 2 (non-zero)

    // Verify fields up to the point of early return.
    bool ok = true;
    if (info.cicp_colour_primaries != 5) {
        std::cout << "Test 2 FAILED: cicp_colour_primaries not set correctly.\n";
        ok = false;
    }
    if (info.cicp_transfer_function != 6) {
        std::cout << "Test 2 FAILED: cicp_transfer_function not set correctly.\n";
        ok = false;
    }
    if (info.cicp_matrix_coefficients != 2) {
        std::cout << "Test 2 FAILED: cicp_matrix_coefficients not set correctly.\n";
        ok = false;
    }
    if (info.cicp_video_full_range_flag != 1) {
        std::cout << "Test 2 FAILED: cicp_video_full_range_flag not set correctly.\n";
        ok = false;
    }
    // valid should remain unchanged (not set) because we returned early.
    if (info.valid != 0) {
        std::cout << "Test 2 FAILED: valid flag should not be set when matrix coefficients non-zero.\n";
        ok = false;
    }
    // Ensure that a warning was issued with the expected message.
    if (png_stub::last_warning != "Invalid cICP matrix coefficients") {
        std::cout << "Test 2 FAILED: Warning not emitted for non-zero matrix coefficients or wrong message.\n";
        ok = false;
    }

    return ok;
}

// Test 3: Zero matrix_coefficients should set the cICP fields and mark valid flag.
// This covers the true-branch where normal storage occurs and PNG_INFO_cICP is set.
bool test_zero_matrix_coefficients_stores_and_sets_valid() {
    using namespace png_stub;

    reset_state();

    png_info info;
    init_info(&info);

    png_const_structrp fake_png_ptr = reinterpret_cast<png_const_structrp>(0x1);
    png_inforp info_ptr = &info;

    png_set_cICP(fake_png_ptr, info_ptr,
                 3, 4, 0, 7); // matrix_coefficients = 0 (zero)

    bool ok = true;
    if (info.cicp_colour_primaries != 3) {
        std::cout << "Test 3 FAILED: cicp_colour_primaries not set correctly.\n";
        ok = false;
    }
    if (info.cicp_transfer_function != 4) {
        std::cout << "Test 3 FAILED: cicp_transfer_function not set correctly.\n";
        ok = false;
    }
    if (info.cicp_matrix_coefficients != 0) {
        std::cout << "Test 3 FAILED: cicp_matrix_coefficients not zero as expected.\n";
        ok = false;
    }
    if (info.cicp_video_full_range_flag != 7) {
        std::cout << "Test 3 FAILED: cicp_video_full_range_flag not set correctly.\n";
        ok = false;
    }
    // PNG_INFO_cICP should be set in valid bitfield.
    if ((info.valid & PNG_INFO_cICP) == 0) {
        std::cout << "Test 3 FAILED: valid flag PNG_INFO_cICP not set when matrix coefficients are zero.\n";
        ok = false;
    }

    // No warning should be produced in this path.
    if (!png_stub::last_warning.empty()) {
        std::cout << "Test 3 FAILED: Unexpected warning when matrix coefficients are zero.\n";
        ok = false;
    }

    return ok;
}

// Test 4: Basic integration-like scenario ensuring both non-null pointers.
// Verifies that function doesn't crash and sets fields when inputs are valid
// and matrix_coefficients is zero (positive path).
bool test_basic_integration_zero_coefficients() {
    using namespace png_stub;

    reset_state();

    png_info info;
    init_info(&info);

    png_const_structrp fake_png_ptr = reinterpret_cast<png_const_structrp>(0x1234);
    png_inforp info_ptr = &info;

    png_set_cICP(fake_png_ptr, info_ptr,
                 9, 8, 0, 0); // zero matrix_coefficients

    bool ok = true;
    if (info.cicp_colour_primaries != 9) {
        std::cout << "Test 4 FAILED: cicp_colour_primaries not set as expected.\n";
        ok = false;
    }
    if (info.cicp_transfer_function != 8) {
        std::cout << "Test 4 FAILED: cicp_transfer_function not set as expected.\n";
        ok = false;
    }
    if (info.cicp_matrix_coefficients != 0) {
        std::cout << "Test 4 FAILED: cicp_matrix_coefficients should be 0.\n";
        ok = false;
    }
    if (info.cicp_video_full_range_flag != 0) {
        std::cout << "Test 4 FAILED: cicp_video_full_range_flag not set as expected.\n";
        ok = false;
    }
    if ((info.valid & PNG_INFO_cICP) == 0) {
        std::cout << "Test 4 FAILED: valid flag PNG_INFO_cICP not set in zero-coefficient path.\n";
        ok = false;
    }

    if (!png_stub::last_warning.empty()) {
        std::cout << "Test 4 FAILED: Unexpected warning in zero-coefficient path.\n";
        ok = false;
    }

    return ok;
}

// Entry point for running all tests and reporting results.
int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](bool (*test_func)(), const char* test_name) {
        total++;
        bool result = test_func();
        if (result) {
            ++passed;
            std::cout << "[PASS] " << test_name << "\n";
        } else {
            std::cout << "[FAIL] " << test_name << "\n";
        }
    };

    run(test_null_png_ptr_does_nothing, "test_null_png_ptr_does_nothing");
    run(test_nonzero_matrix_coefficients_emits_warning_and_does_not_set_valid, "test_nonzero_matrix_coefficients_emits_warning_and_does_not_set_valid");
    run(test_zero_matrix_coefficients_stores_and_sets_valid, "test_zero_matrix_coefficients_stores_and_sets_valid");
    run(test_basic_integration_zero_coefficients, "test_basic_integration_zero_coefficients");

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero on failure to indicate test suite failure.
    return (passed == total) ? 0 : 1;
}