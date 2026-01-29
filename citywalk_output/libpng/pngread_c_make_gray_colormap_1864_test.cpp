// Test suite for the focal method: make_gray_colormap
// This test uses a lightweight mock of the dependent function
// png_create_colormap_entry to verify that make_gray_colormap
// generates 256 grayscale colormap entries with expected parameters.
//
// Notes:
// - This test avoids GTest; uses a simple, non-terminating expectation system.
// - The test mocks only the required dependencies to isolate make_gray_colormap.
// - We verify return value, number of calls, and the per-call parameters (ip, r, g, b, a, encoding).
// - The encoding value is kept consistent across all calls (lack of knowledge about
//   the exact P_sRGB value is handled by asserting consistency).

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>


// ------------------------------------------------------------------------------------
// Mock and scaffolding to support testing of make_gray_colormap in the absence of the
// actual libpng dependencies. We provide minimal type definitions and a fake
// implementation of png_create_colormap_entry to record calls.
// ------------------------------------------------------------------------------------

extern "C" {

// Forward declaration of the function under test (as it would be exposed by pngread.c)
int make_gray_colormap(struct png_image_read_control *display);

// Minimal type definitions to match the expectations of the focal function.
typedef unsigned int png_uint_32;
struct png_image_read_control {
    // Intentionally left opaque for this mock. The real type would hold
    // state for libpng; the test only needs a valid non-null pointer.
};

// Encoding constants are normally provided by the libpng headers.
// We don't rely on their actual values here; we just ensure all calls use a consistent value.
// The production code uses P_sRGB; the exact numeric value is not required for this test.
const int P_sRGB = 0;

// Struct to capture each call to png_create_colormap_entry
struct Call {
    unsigned ip;
    unsigned red;
    unsigned green;
    unsigned blue;
    unsigned alpha;
    unsigned encoding;
};

// Global container to collect all calls made by make_gray_colormap during the test.
// This must be visible to both the test's setup and the mock implementation.
static std::vector<Call> g_calls;

// Mock implementation of png_create_colormap_entry used by make_gray_colormap.
// This captures all inputs for verification.
int png_create_colormap_entry(png_image_read_control *display,
                              png_uint_32 ip,
                              png_uint_32 red,
                              png_uint_32 green,
                              png_uint_32 blue,
                              png_uint_32 alpha,
                              int encoding)
{
    (void)display; // suppress unused parameter warning in environments where it's unused
    Call c = {
        static_cast<unsigned>(ip),
        static_cast<unsigned>(red),
        static_cast<unsigned>(green),
        static_cast<unsigned>(blue),
        static_cast<unsigned>(alpha),
        static_cast<unsigned>(encoding)
    };
    g_calls.push_back(c);
    // Simulate a successful entry creation
    return 0;
}

// Helper: reset the mock state
void reset_mock_state() {
    g_calls.clear();
}

// Helper: simple test assertion that does not terminate tests
static int g_test_failures = 0;
#define EXPECT_EQ(a, b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if (!(_a == _b)) { \
        std::cerr << "EXPECT_EQ failed: " << #a << " (" << _a << ") != " << #b \
                  << " (" << _b << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: condition " << #cond << " is false at " \
                  << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

} // extern "C"

// ------------------------------------------------------------------------------------
// Test harness
// We implement two checks:
// 1) The function returns 256 (the loop runs 256 iterations).
// 2) Exactly 256 calls to png_create_colormap_entry are made with ip/ RGB values matching i
//    (i.e., grayscale values) and alpha set to 255. All encodings should be identical across
//    calls (we don't rely on the physical value of P_sRGB in this isolated test).
// ------------------------------------------------------------------------------------
namespace test_make_gray_colormap {

void run_basic_behavior_test() {
    // Prepare a display object (opaque in this mock)
    png_image_read_control display;
    reset_mock_state();

    // Execute the function under test
    int result = make_gray_colormap(&display);

    // Validation
    EXPECT_EQ(result, 256);
    EXPECT_EQ(g_calls.size(), 256u);

    // Validate per-call parameters
    for (unsigned i = 0; i < 256; ++i) {
        const Call &c = g_calls[i];
        EXPECT_EQ(c.ip, i);
        EXPECT_EQ(c.red, i);
        EXPECT_EQ(c.green, i);
        EXPECT_EQ(c.blue, i);
        EXPECT_EQ(c.alpha, 255u);

        if (i > 0) {
            // All encodings should be identical across calls
            EXPECT_EQ(c.encoding, g_calls[0].encoding);
        }
    }
}

// Entry point for the test suite
void run_all() {
    // Run individual tests
    run_basic_behavior_test();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "All tests passed for make_gray_colormap." << std::endl;
    } else {
        std::cerr << "make_gray_colormap tests finished with " << g_test_failures
                  << " failure(s)." << std::endl;
    }
}

} // namespace test_make_gray_colormap

// ------------------------------------------------------------------------------------
// Main
// We invoke the test suite in main() as the test runner (no GTest required).
// ------------------------------------------------------------------------------------
int main() {
    // Run the test suite
    test_make_gray_colormap::run_all();
    // If any failures occurred, return non-zero to indicate test failure.
    return (test_make_gray_colormap::g_test_failures == 0) ? 0 : 1;
}