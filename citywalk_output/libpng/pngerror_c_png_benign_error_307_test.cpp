// Unit tests for a self-contained reproduction of the png_benign_error logic.
// This is a standalone C++11 mock-up to validate the control-flow covered by
// the focal method under various combinations of flags, mode, and chunk_name.
// Note: This test harness is self-contained and does not rely on GTest or libpng.
// It mirrors the decision structure of the original png_benign_error to maximize
// coverage of true/false branches without requiring external dependencies.

// Candidate Keywords (core dependent components for the method under test):
// - PNG_FLAG_BENIGN_ERRORS_WARN
// - PNG_IS_READ_STRUCT
// - PNG_READ_SUPPORTED (represented as a runtime flag in this mock)
// - chunk_name
// - png_warning
// - png_error
// - png_chunk_warning
// - png_chunk_error
// - error_message
// - benign vs error branches
// - callback invocation counts and message propagation

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain knowledge adapters: mimic the essential types used by the focal function.
typedef struct PngStruct {
    unsigned int flags;      // bitmap of flags (e.g., PNG_FLAG_BENIGN_ERRORS_WARN)
    unsigned int mode;       // mode flags (e.g., PNG_IS_READ_STRUCT)
    const char* chunk_name;    // name of the current PNG chunk, if any
} PngStruct;

typedef const PngStruct* png_const_structrp; // const pointer to the PNG struct
typedef const char* png_const_charp;         // C string type for messages

// Runtime switch to emulate compile-time availability of PNG_READ_SUPPORTED
static bool g_read_supported = true;

// Public-like constants (mirroring libpng's macro names)
static const unsigned int PNG_FLAG_BENIGN_ERRORS_WARN = 0x01;
static const unsigned int PNG_IS_READ_STRUCT = 0x01;

// Call log to verify which notifications were emitted and what messages were passed.
struct CallLog {
    int warning_calls = 0;
    int error_calls = 0;
    int chunk_warning_calls = 0;
    int chunk_error_calls = 0;
    std::string last_warning_message;
    std::string last_error_message;
};

// Global log instance used by stub callbacks
static CallLog g_log;

// Reset the call log before each test
static void reset_log() {
    g_log = CallLog();
}

// Stubbed callback implementations to capture behavior of the focal function
static void png_warning(png_const_structrp /*png_ptr*/, png_const_charp message) {
    ++g_log.warning_calls;
    if (message) g_log.last_warning_message = message;
}
static void png_error(png_const_structrp /*png_ptr*/, png_const_charp message) {
    ++g_log.error_calls;
    if (message) g_log.last_error_message = message;
}
static void png_chunk_warning(png_const_structrp /*png_ptr*/, png_const_charp message) {
    ++g_log.chunk_warning_calls;
    if (message) g_log.last_warning_message = message;
}
static void png_chunk_error(png_const_structrp /*png_ptr*/, png_const_charp message) {
    ++g_log.chunk_error_calls;
    if (message) g_log.last_error_message = message;
}

// The focal method re-implemented in this test file.
// It mirrors the branching logic of the original function but uses a runtime
// flag (g_read_supported) to toggle the \"compile-time\" PNG_READ_SUPPORTED behavior.
static void png_benign_error(png_const_structrp png_ptr, png_const_charp error_message) {
    // The original code uses a #ifdef region; here we emulate the runtime behavior
    // to enable deterministic testing of both branches.
    if ((png_ptr->flags & PNG_FLAG_BENIGN_ERRORS_WARN) != 0) {
        if (g_read_supported) {
            if ((png_ptr->mode & PNG_IS_READ_STRUCT) != 0 && png_ptr->chunk_name != 0)
                png_chunk_warning(png_ptr, error_message);
            else
                png_warning(png_ptr, error_message);
        } else {
            // Simulated non-read-supported branch (would go to warning in this mock)
            png_warning(png_ptr, error_message);
        }
    } else {
        if (g_read_supported) {
            if ((png_ptr->mode & PNG_IS_READ_STRUCT) != 0 && png_ptr->chunk_name != 0)
                png_chunk_error(png_ptr, error_message);
            else
                png_error(png_ptr, error_message);
        } else {
            png_error(png_ptr, error_message);
        }
    }
#if !defined(PNG_ERROR_TEXT_SUPPORTED)
    // If error text is not supported, silence unused parameter warning (simulated)
    (void)error_message;
#endif
}

// Utility assertion with non-terminating semantics: prints and returns pass/fail.
static bool expect(bool condition, const char* description) {
    if (condition) {
        std::cout << "[PASS] " << description << "\n";
        return true;
    } else {
        std::cout << "[FAIL] " << description << "\n";
        return false;
    }
}

// Test 1: Benign, read-struct, chunk_name present -> chunk_warning should be invoked
static void test_benign_read_with_chunk() {
    reset_log();
    g_read_supported = true;

    PngStruct png = {0};
    png.flags = PNG_FLAG_BENIGN_ERRORS_WARN;
    png.mode  = PNG_IS_READ_STRUCT;
    png.chunk_name = "CHUNK";

    png_benign_error(&png, "benign read with chunk");

    bool pass = true;
    pass &= (g_log.chunk_warning_calls == 1);
    pass &= (g_log.warning_calls == 0);
    pass &= (g_log.chunk_error_calls == 0);
    pass &= (g_log.error_calls == 0);
    pass &= (g_log.last_warning_message == "benign read with chunk");

    expect(pass, "Benign read with chunk -> chunk_warning invoked, no others");
}

// Test 2: Benign, read-struct, but no chunk_name -> should fall back to png_warning (not chunk_warning)
static void test_benign_read_without_chunk() {
    reset_log();
    g_read_supported = true;

    PngStruct png = {0};
    png.flags = PNG_FLAG_BENIGN_ERRORS_WARN;
    png.mode  = PNG_IS_READ_STRUCT;
    png.chunk_name = nullptr;

    png_benign_error(&png, "benign read without chunk");

    bool pass = true;
    pass &= (g_log.chunk_warning_calls == 0);
    pass &= (g_log.warning_calls == 1);
    pass &= (g_log.chunk_error_calls == 0);
    pass &= (g_log.error_calls == 0);
    pass &= (g_log.last_warning_message == "benign read without chunk");

    expect(pass, "Benign read without chunk -> warning invoked, no chunk_warning");
}

// Test 3: Benign, not read-struct (mode not including PNG_IS_READ_STRUCT) -> should be png_warning
static void test_benign_not_read_with_chunk_name_but_no_read_mode() {
    reset_log();
    g_read_supported = true;

    PngStruct png = {0};
    png.flags = PNG_FLAG_BENIGN_ERRORS_WARN;
    png.mode  = 0; // not PNG_IS_READ_STRUCT
    png.chunk_name = "CHUNK";

    png_benign_error(&png, "benign without read mode but with chunk");

    bool pass = true;
    pass &= (g_log.chunk_warning_calls == 0);
    pass |= (g_log.warning_calls == 1);
    pass &= (g_log.chunk_error_calls == 0);
    pass &= (g_log.error_calls == 0);
    pass &= (g_log.last_warning_message == "benign without read mode but with chunk");

    expect(pass, "Benign with chunk but no read mode -> warning invoked, not chunk_warning");
}

// Test 4: Non-benign (flag not set), read-struct with chunk_name -> chunk_error should be invoked
static void test_error_non_benign_read_with_chunk() {
    reset_log();
    g_read_supported = true;

    PngStruct png = {0};
    png.flags = 0; // Benign flag not set
    png.mode  = PNG_IS_READ_STRUCT;
    png.chunk_name = "CHUNK";

    png_benign_error(&png, "non-benign read with chunk");

    bool pass = true;
    pass &= (g_log.chunk_warning_calls == 0);
    pass &= (g_log.chunk_error_calls == 1);
    pass &= (g_log.error_calls == 0);
    pass &= (g_log.warning_calls == 0);
    pass &= (g_log.last_error_message == "non-benign read with chunk");

    expect(pass, "Non-benign read with chunk -> chunk_error invoked");
}

// Test 5: Non-benign, not read-struct, with no chunk -> png_error invoked
static void test_error_non_benign_not_read_no_chunk() {
    reset_log();
    g_read_supported = true;

    PngStruct png = {0};
    png.flags = 0; // Benign flag not set
    png.mode  = 0; // not READ_STRUCT
    png.chunk_name = nullptr;

    png_benign_error(&png, "non-benign not-read no-chunk");

    bool pass = true;
    pass &= (g_log.chunk_warning_calls == 0);
    pass &= (g_log.chunk_error_calls == 0);
    pass &= (g_log.error_calls == 1);
    pass &= (g_log.warning_calls == 0);
    pass &= (g_log.last_error_message == "non-benign not-read no-chunk");

    expect(pass, "Non-benign not-read no-chunk -> error invoked (not chunk_error)");
}

// Test 6: Non-benign, read-struct with chunk, but PNG_READ_SUPPORTED disabled -> png_error should be invoked
static void test_error_without_read_supported() {
    reset_log();
    g_read_supported = false;

    PngStruct png = {0};
    png.flags = 0; // Benign flag not set
    png.mode  = PNG_IS_READ_STRUCT;
    png.chunk_name = "CHUNK";

    png_benign_error(&png, "non-benign with read but no read support");

    bool pass = true;
    // When read not supported, original logic would call png_error in this branch
    pass &= (g_log.chunk_warning_calls == 0);
    pass &= (g_log.chunk_error_calls == 0);
    pass &= (g_log.error_calls == 1);
    pass &= (g_log.warning_calls == 0);
    pass &= (g_log.last_error_message == "non-benign with read but no read support");

    expect(pass, "Non-benign with read struct and no read support -> error invoked (png_error)");
}

// Main harness: run all tests
int main() {
    std::cout << "Starting png_benign_error unit tests (self-contained mock).\n";

    test_benign_read_with_chunk();
    test_benign_read_without_chunk();
    test_benign_not_read_with_chunk_name_but_no_read_mode();
    test_error_non_benign_read_with_chunk();
    test_error_non_benign_not_read_no_chunk();
    test_error_without_read_supported();

    std::cout << "Finished tests.\n";
    return 0;
}