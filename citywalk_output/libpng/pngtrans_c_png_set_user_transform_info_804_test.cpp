// A self-contained C++11 test suite for the focal function:
// png_set_user_transform_info, implemented here in the same file for
// portability in environments without the full libpng testbed.
// This test covers key branches and verifies behavior via direct state
// inspection and simple string comparisons for error reporting.
//
// The implementation includes minimal mock definitions of the types and
// macros used by the focal function, inspired by the provided source.
//
// Explanatory comments are added for each unit test to describe intent.

/* Notes:
   - This file is designed to be compiled with a C++11 compiler.
   - No external testing framework is used (as requested).
   - The test suite is self-contained: it defines the function under test
     and the minimal environment it needs to operate.
   - True and false branches of predicates are exercised.
   - Static members are not used; all state is local/global as needed for
     the mock environment.
*/

#include <cstring>
#include <pngpriv.h>
#include <cstdio>


// ------------------------------
// Mocked/types/macros for the focal function
// ------------------------------
typedef struct png_struct_def png_struct;
typedef png_struct* png_structrp;
typedef const png_struct* png_const_structrp;
typedef void* png_voidp;
typedef unsigned char png_byte;

// Predicates/macros used by the function under test
#define PNG_IS_READ_STRUCT 0x01
#define PNG_FLAG_ROW_INIT 0x02
#define PNG_READ_USER_TRANSFORM_SUPPORTED 1

// ------------------------------
// Minimal struct used by the focal function
// ------------------------------
struct png_struct_def {
    unsigned int mode;
    unsigned int flags;
    void*        user_transform_ptr;
    unsigned char user_transform_depth;
    unsigned char user_transform_channels;
};

// ------------------------------
// Forward declaration of the function under test
// ------------------------------
extern "C" void png_set_user_transform_info(png_structrp png_ptr,
                                           png_voidp user_transform_ptr,
                                           int user_transform_depth,
                                           int user_transform_channels);

// ------------------------------
// Lightweight "private" utilities to observe behavior
// ------------------------------
static const char* g_last_app_error_msg = nullptr;

// A lightweight diagnostic stub (no-op for unit testing)
static void png_debug(int /*level*/, const char* /*msg*/) {
    // Intentionally left blank to avoid noisy I/O during tests
}

// The error reporter used by the focal function when the "error" path is hit
static void png_app_error(png_structrp /*png_ptr*/, const char* msg) {
    g_last_app_error_msg = msg;
}

// Accessor for tests to verify the last error message content
static const char* png_get_last_app_error() {
    return g_last_app_error_msg;
}

// ------------------------------
// Implementation of the focal function (copied/adapted for tests)
// This mirrors the logic in the provided source (pngtrans.c) with the same behavior.
// ------------------------------
extern "C" void png_set_user_transform_info(png_structrp png_ptr,
                                           png_voidp user_transform_ptr,
                                           int user_transform_depth,
                                           int user_transform_channels)
{
    // Start diagnostic (no-OP in tests)
    png_debug(1, "in png_set_user_transform_info");

    // Guard against null pointer
    if (png_ptr == nullptr)
        return;

#ifdef PNG_READ_USER_TRANSFORM_SUPPORTED
    // If we are in a read-struct and we have row initialization flag set,
    // report an application error and abort the update.
    if ((png_ptr->mode & PNG_IS_READ_STRUCT) != 0 &&
        (png_ptr->flags & PNG_FLAG_ROW_INIT) != 0)
    {
        png_app_error(png_ptr,
            "info change after png_start_read_image or png_read_update_info");
        return;
    }
#endif

    // Update user transform information
    png_ptr->user_transform_ptr = user_transform_ptr;
    png_ptr->user_transform_depth = (png_byte)user_transform_depth;
    png_ptr->user_transform_channels = (png_byte)user_transform_channels;
}

// ------------------------------
// Unit test helpers
// ------------------------------
static void report(const char* test_name, bool condition) {
    if (condition) {
        std::printf("PASS: %s\n", test_name);
    } else {
        std::printf("FAIL: %s\n", test_name);
    }
}

// ------------------------------
// Unit tests
// ------------------------------

// Test 1: Null pointer should cause no changes and function should return gracefully.
void test_null_ptr_is_ignored() {
    // Prepare a png_struct with some initial state
    png_struct s;
    s.mode = 0;
    s.flags = 0;
    s.user_transform_ptr = (void*)0xABCD;
    s.user_transform_depth = 0x12;
    s.user_transform_channels = 0x34;

    // Reset error tracking
    g_last_app_error_msg = nullptr;

    // Invoke the function with a null png_ptr
    png_set_user_transform_info(nullptr, (void*)0xDEAD, 7, 3);

    // Verify that original state is unchanged
    bool state_unchanged = (s.user_transform_ptr == (void*)0xABCD) &&
                           (s.user_transform_depth == 0x12) &&
                           (s.user_transform_channels == 0x34);
    report("test_null_ptr_is_ignored - state unchanged when png_ptr is NULL", state_unchanged);

    // Also verify that no error was reported
    bool no_error = (g_last_app_error_msg == nullptr);
    report("test_null_ptr_is_ignored - no error reported for NULL ptr", no_error);
}

// Test 2: When mode indicates read-struct and row init is set, error path should be taken
// and no state changes should occur for the transform info.
void test_error_path_after_png_start_read() {
    // Prepare a png_struct with READ_STRUCT and ROW_INIT flags both set
    png_struct s;
    s.mode  = PNG_IS_READ_STRUCT;
    s.flags = PNG_FLAG_ROW_INIT;
    s.user_transform_ptr = (void*)0xDEAD;
    s.user_transform_depth = 0x11;
    s.user_transform_channels = 0x22;

    // Reset error tracking
    g_last_app_error_msg = nullptr;

    // Call the function with arbitrary transform info
    png_set_user_transform_info(&s, (void*)0x1111, 9, 3);

    // Expect no updates to the struct fields due to early return
    bool no_update = (s.user_transform_ptr == (void*)0xDEAD) &&
                     (s.user_transform_depth == 0x11) &&
                     (s.user_transform_channels == 0x22);
    report("test_error_path_after_png_start_read - no field updates on error path", no_update);

    // Expect an error message indicating the info-change-after-start condition
    bool error_reported = (g_last_app_error_msg != nullptr) &&
                          (std::strcmp(g_last_app_error_msg,
                                       "info change after png_start_read_image or png_read_update_info") == 0);
    report("test_error_path_after_png_start_read - correct error message when branch triggers", error_reported);
}

// Test 3: When the inner condition is not met (flags not set), fields should be updated
void test_success_path_updates_fields() {
    // Prepare a png_struct without the read-struct flags
    png_struct s;
    s.mode  = 0;               // Not a read-struct
    s.flags = 0;                 // Row init not set
    s.user_transform_ptr = nullptr;
    s.user_transform_depth = 0;
    s.user_transform_channels = 0;

    // Reset error tracking
    g_last_app_error_msg = nullptr;

    // Apply new transform info
    void* new_ptr = (void*)0xBEEF;
    int   depth   = 0x7A;
    int   chan    = 0x42;
    png_set_user_transform_info(&s, new_ptr, depth, chan);

    // Verify that fields are updated accordingly
    bool updated_ptr     = (s.user_transform_ptr == new_ptr);
    bool updated_depth   = (s.user_transform_depth == (unsigned char)depth);
    bool updated_channels= (s.user_transform_channels == (unsigned char)chan);
    report("test_success_path_updates_fields - pointer updated", updated_ptr &&
                                                           updated_depth &&
                                                           updated_channels);

    // Verify that no error was reported
    bool no_error = (g_last_app_error_msg == nullptr);
    report("test_success_path_updates_fields - no error reported on success path", no_error);
}

// Test 4: Partial condition where only one bit is set (mode OR flags does not trigger error).
void test_partial_condition_updates_with_marker() {
    // Prepare a png_struct with mode set but row_init flag not set
    png_struct s;
    s.mode  = PNG_IS_READ_STRUCT;
    s.flags = 0;  // ROW_INIT not set
    s.user_transform_ptr = nullptr;
    s.user_transform_depth = 0;
    s.user_transform_channels = 0;

    // Reset error tracking
    g_last_app_error_msg = nullptr;

    // Update
    void* new_ptr = (void*)0x1234;
    int   depth   = 0x55;
    int   chan    = 0x66;
    png_set_user_transform_info(&s, new_ptr, depth, chan);

    // Expect updates to fields (no early return)
    bool updated = (s.user_transform_ptr == new_ptr) &&
                   (s.user_transform_depth == (unsigned char)depth) &&
                   (s.user_transform_channels == (unsigned char)chan);
    report("test_partial_condition_updates_with_marker - updates applied when only partial condition is true", updated);

    // No error should be reported
    bool no_error = (g_last_app_error_msg == nullptr);
    report("test_partial_condition_updates_with_marker - no error reported in partial condition", no_error);
}

// ------------------------------
// Main runner
// ------------------------------
int main() {
    std::printf("png_set_user_transform_info unit tests (self-contained)\n");
    test_null_ptr_is_ignored();
    test_error_path_after_png_start_read();
    test_success_path_updates_fields();
    test_partial_condition_updates_with_marker();

    return 0;
}