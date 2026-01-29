// test_png_inflate.cpp
// Lightweight, non-terminating unit test harness for the focal function png_zlib_inflate
// This test suite targets the usage of png_zlib_inflate as exercised within pngrutil.c.
// It uses a small macro-based test runner (no GTest) and a minimal mock for inflate to
// observe behavior without pulling in a full zlib implementation.
// Notes:
// - This test relies on the presence of libpng and zlib headers in the include path.
// - It uses a macro-based interception technique: the call to inflate in pngrutil.c is
//   redirected to a mock_inflate function via a preprocessor trick (see below).
// - The test suite demonstrates scenarios for true/false branches of the condition in
//   png_zlib_inflate as described in the focal method.
//
// How it works (high level):
// 1) We provide a mock_inflate implementation that records the inputs it receives and returns
//    a sentinel value. We want to observe how png_zlib_inflate interacts with zstream when
//    the "inflate" call happens.
// 2) To ensure the call inside pngrutil.c uses our mock_inflate, we include pngrutil.c in this
//    translation unit with a preprocessor macro that renames calls to inflate to mock_inflate.
// 3) We construct a libpng png_struct with its zstream fields manipulated by the tests to drive
//    the desired code paths (invalid window size, and the normal path that would call inflate).
// 4) We verify outcomes via non-terminating assertions (log failures and continue).
//
// Important: This test file is intended to be compiled as part of a test executable that links
// against libpng (and without requiring GTest). It overrides inflate via macro inclusion of pngrutil.c.

#include <vector>
#include <string>
#include <zlib.h>
#include <iostream>
#include <pngpriv.h>
#include <pngrutil.c>
#include <cstring>
#include <png.h>


// Include zlib and libpng headers to access required types and constants

// Forward declarations for the mock inflate function we'll provide.
// We declare the mock with C linkage to match the symbol the included code will call.
extern "C" int mock_inflate(z_stream* strm, int flush);

// Global test state for assertions (non-terminating)
struct TestState {
    int total = 0;
    int passed = 0;
    std::vector<std::string> failures;

    void logFail(const std::string& msg) {
        failures.push_back(msg);
    }

    void report() const {
        std::cerr << "Test results: " << passed << "/" << total << " passed\n";
        if (!failures.empty()) {
            std::cerr << "Failures:\n";
            for (const auto& f : failures) std::cerr << "  - " << f << "\n";
        }
    }

    bool allPassed() const { return failures.empty(); }
};

#define ASSERT_TRUE(cond, msg) do { \
    ++g_state.total; \
    if (cond) { ++g_state.passed; } \
    else { g_state.logFail("ASSERT_TRUE failed: " + std::string(msg)); } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    ++g_state.total; \
    if ((a) == (b)) { ++g_state.passed; } \
    else { g_state.logFail("ASSERT_EQ failed: " + std::string(msg) + " (lhs: " + std::to_string((a)) + ", rhs: " + std::to_string((b)) + ")"); } \
} while(0)

//
//
//  Step: Include pngrutil.c with an inflate-alias to mock_inflate
//  We map calls to inflate to mock_inflate by defining a macro before inclusion.
//  Note: This approach requires that pngrutil.c does not get compiled in multiple translation units
//  with conflicting symbol definitions for inflate. Here we do a single TU include.
//

// Prepare to intercept inflate calls inside pngrutil.c by macro trick.
// We need to define the mock_inflate prototype before including.
// We'll also provide a guard to avoid multiple inclusions during compilation.
extern "C" int mock_inflate(z_stream* strm, int flush); // forward declare

// Redirect calls: inside pngrutil.c, calls to inflate(...) will become mock_inflate(...)
#define inflate mock_inflate
// Include the focal C source to compile together with this test harness.
// This will allow the PNG internal code that calls inflate(...) to be routed to mock_inflate.
// Note: ensure this file is accessible in the include path.
#undef inflate

// -----------------------------------------------------------------------------
// Mock implementation details
// -----------------------------------------------------------------------------

// The mock_inflate will be used to observe how png_zlib_inflate would behave when inflate is
// invoked. We implement a simple mock that records whether it was invoked, with the input's
// available input and the 'flush' parameter, and returns a sentinel value so tests can verify
// the control flow without executing real decompression.

struct InflateCallRecord {
    z_stream* strm;
    int flush;
    bool called;
};

InflateCallRecord g_inflateRecord = {nullptr, 0, false};

// The actual mock_inflate used by the test
extern "C" int mock_inflate(z_stream* strm, int flush) {
    g_inflateRecord.strm = strm;
    g_inflateRecord.flush = flush;
    g_inflateRecord.called = true;
    // Do not mutate the stream to keep test deterministic
    // Return a sentinel value to indicate that inflate was invoked
    return 123456; // sentinel success code for test purposes
}

// -----------------------------------------------------------------------------
// Test helpers
// -----------------------------------------------------------------------------

static TestState g_state;

// Helper to create a usable png_struct instance for tests.
// We use libpng's API to create a read-struct which provides a real instance of png_struct.
// We rely on the fact that the focal function accesses fields inside png_struct (zstream,
// zstream_start, etc.). If the library version exposes those fields publicly, this will work.
// If not, this test will need to be adapted to the specific libpng version in use.
static png_structp create_test_png_struct() {
    // Use libpng to allocate a png_struct
    // Error handling callbacks are set to NULL to simplify test setup
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    return png_ptr;
}

// Helper to cleanup png_struct allocated by create_test_png_struct
static void destroy_test_png_struct(png_structp png_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }
}

// Helper to access the zstream and related fields.
// This function assumes the libpng version exposes these internals in png_struct.
// This is version-dependent; if compilation fails due to field access, adjust for your libpng version.
static void prepare_png_ptr_for_test(png_structp png_ptr,
                                     unsigned char first_input_byte,
                                     unsigned int avail_in,
                                     int zstream_start_initialized) {
    // Initialize zstream fields when accessible
    // NOTE: Access to internals may vary by libpng version; adjust field names accordingly.
    // The goal is to drive: if (png_ptr->zstream_start && png_ptr->zstream.avail_in > 0) ...
    // We'll set zstream_start and zstream fields accordingly.

    // Clear msg
    #if PNG_LIBPNG_VER >= 10507 // conservative check for field accessibility
    if (png_ptr->zstream.avail_in == 0) {
        // If avail_in is zero, ensure a non-zero value to trigger the path when needed
        // This is just a safeguard; actual tests will set as desired below.
    }
    #endif

    // Attempt to set internals if accessible
    #if defined(PNG_STRUCT_HAS_ZSTREAM) && PNG_STRUCT_HAS_ZSTREAM
    png_ptr->zstream_start = zstream_start_initialized ? 1 : 0;
    png_ptr->zstream.avail_in = avail_in;
    png_ptr->zstream.next_in = (Bytef*)&first_input_byte; // cast to Bytef (unsigned char)
    png_ptr->zstream.msg = NULL;
    #endif
}

// -----------------------------------------------------------------------------
// Test cases
// -----------------------------------------------------------------------------

// Test 1: When zstream_start is true and avail_in > 0 and the first input byte triggers invalid window size.
// This should return Z_DATA_ERROR and set the zstream_start flag to 0, and set the error msg.
static void test_invalid_window_size_path() {
    png_structp png_ptr = create_test_png_struct();
    if (!png_ptr) {
        g_state.logFail("test_invalid_window_size_path: failed to create png_struct");
        g_state.total++;
        return;
    }

    // Drive the conditions:
    // - zstream_start == 1
    // - zstream.avail_in > 0
    // - next_in >> 4 > 7 (we'll set next_in byte to cause 0x80 >> 4 == 8 > 7)
    unsigned char input_byte = 0x80; // 0x80 >> 4 == 8
    // Access to fields may vary by version; attempt to set if available
    #if defined(PNG_STRUCT_HAS_ZSTREAM) && PNG_STRUCT_HAS_ZSTREAM
    png_ptr->zstream_start = 1;
    png_ptr->zstream.avail_in = 1;
    png_ptr->zstream.next_in = &input_byte;
    png_ptr->zstream.msg = NULL;
    #else
    // If internals are not accessible, skip this test gracefully.
    destroy_test_png_struct(png_ptr);
    g_state.logFail("test_invalid_window_size_path: internals not accessible for this libpng version");
    return;
    #endif

    int res = png_zlib_inflate(png_ptr, 0);

    // Check expected outcomes
    ASSERT_EQ(res, (int)Z_DATA_ERROR, "png_zlib_inflate should return Z_DATA_ERROR for invalid window size");
    #if defined(PNG_STRUCT_HAS_ZSTREAM) && PNG_STRUCT_HAS_ZSTREAM
    ASSERT_TRUE(png_ptr->zstream_start == 0, "png_zlib_inflate should clear zstream_start on error path");
    if (png_ptr->zstream.msg) {
        ASSERT_TRUE(std::strcmp(png_ptr->zstream.msg, "invalid window size (libpng)") == 0,
                    "png_zlib_inflate should set msg to 'invalid window size (libpng)'");
    } else {
        g_state.logFail("test_invalid_window_size_path: zstream.msg was not set");
    }
    #endif

    destroy_test_png_struct(png_ptr);
}

// Test 2: When zstream_start is true and avail_in > 0 and the input does NOT trigger the error.
// Expect inflate to be invoked (our mock_inflate returns sentinel 123456) and zstream_start cleared.
static void test_inflate_called_on_valid_window() {
    png_structp png_ptr = create_test_png_struct();
    if (!png_ptr) {
        g_state.logFail("test_inflate_called_on_valid_window: failed to create png_struct");
        g_state.total++;
        return;
    }

    unsigned char input_byte = 0x00; // 0x00 >> 4 == 0, not causing error
    #if defined(PNG_STRUCT_HAS_ZSTREAM) && PNG_STRUCT_HAS_ZSTREAM
    png_ptr->zstream_start = 1;
    png_ptr->zstream.avail_in = 1;
    png_ptr->zstream.next_in = &input_byte;
    png_ptr->zstream.msg = NULL;
    #else
    destroy_test_png_struct(png_ptr);
    g_state.logFail("test_inflate_called_on_valid_window: internals not accessible for this libpng version");
    return;
    #endif

    int res = png_zlib_inflate(png_ptr, 0);

    // Our mock_inflate returns 123456
    ASSERT_EQ(res, 123456, "png_zlib_inflate should return mock_inflate sentinel value when window is valid");
    #if defined(PNG_STRUCT_HAS_ZSTREAM) && PNG_STRUCT_HAS_ZSTREAM
    ASSERT_TRUE(png_ptr->zstream_start == 0, "png_zlib_inflate should set zstream_start to 0 after starting inflate");
    #endif

    destroy_test_png_struct(png_ptr);
}

// Test 3: When zstream_start is already 0 (or becomes 0) so the pre-check bypasses the inner condition.
// This should call inflate (mock) and return its sentinel value, without error manipulation.
static void test_no_precheck_path_calls_inflate() {
    png_structp png_ptr = create_test_png_struct();
    if (!png_ptr) {
        g_state.logFail("test_no_precheck_path_calls_inflate: failed to create png_struct");
        g_state.total++;
        return;
    }

    unsigned char input_byte = 0x00;
    #if defined(PNG_STRUCT_HAS_ZSTREAM) && PNG_STRUCT_HAS_ZSTREAM
    png_ptr->zstream_start = 0; // precondition: skip inner if entirely
    png_ptr->zstream.avail_in = 1;
    png_ptr->zstream.next_in = &input_byte;
    png_ptr->zstream.msg = NULL;
    #else
    destroy_test_png_struct(png_ptr);
    g_state.logFail("test_no_precheck_path_calls_inflate: internals not accessible for this libpng version");
    return;
    #endif

    int res = png_zlib_inflate(png_ptr, 0);

    // Since inflate is mocked to return 123456, we expect that same sentinel value
    ASSERT_EQ(res, 123456, "png_zlib_inflate should return mock_inflate sentinel value when precheck bypassed");
    destroy_test_png_struct(png_ptr);
}

// -----------------------------------------------------------------------------
// Main: Run tests
// -----------------------------------------------------------------------------

int main() {
    // Run tests
    test_invalid_window_size_path();
    test_inflate_called_on_valid_window();
    test_no_precheck_path_calls_inflate();

    // Report results
    g_state.report();
    // Return non-zero if any test failed
    return g_state.allPassed() ? 0 : 1;
}