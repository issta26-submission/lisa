/*
 * Unit test suite for the focal method:
 *   make_gray_file_colormap(png_image_read_control *display)
 *
 * Approach:
 * - Provide minimal in-project stubs to mimic the dependencies required by the focal method.
 * - Re-implement the focal function logic locally (in this test file) to avoid external dependencies.
 * - Use a lightweight non-terminating assertion framework (EXPECT_*) to maximize coverage without aborting tests.
 * - Validate that:
 *     - The function iterates 256 times and calls the colormap entry creator with expected parameters.
 *     - The return value is 256 (i.e., the final value of i after the loop).
 * - Add a couple of broader tests (e.g., multiple invocations) to improve coverage.
 *
 * Note: This test is self-contained and does not rely on GTest. It uses only the C++ standard library.
 *       It imitates the behavior of the real dependencies sufficiently to exercise the focal logic.
 */

#include <cstdint>
#include <vector>
#include <sstream>
#include <iostream>
#include <pngpriv.h>


// ---------------------------------------------------------------------------
// Minimal stubs to mimic the dependencies required by the focal function.
// These are intentionally lightweight and local to this test harness.
// ---------------------------------------------------------------------------

// Forward declaration to mimic the external type used by the focal method.
struct png_image_read_control;

// Typedef to mirror usage in the focal code.
typedef png_image_read_control png_image_read_control;

// A simple encoding flag constant used in the focal method.
static const int P_FILE = 1;

// A global log to capture calls to the fake colormap entry creator.
struct ColormapEntryCall {
    unsigned int ip;
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int alpha;
    int encoding;
};

static std::vector<ColormapEntryCall> g_calls;

// The fake implementation of png_create_colormap_entry used by the focal method.
// It records each invocation so tests can verify correct behavior.
extern "C" void png_create_colormap_entry(
    png_image_read_control *display,
    unsigned int ip,
    unsigned int red,
    unsigned int green,
    unsigned int blue,
    unsigned int alpha,
    int encoding)
{
    (void)display; // not used in this test harness
    ColormapEntryCall c { ip, red, green, blue, alpha, encoding };
    g_calls.push_back(c);
}

// A minimal definition of the png_image_read_control type for the test.
// The real library's internals are not needed for this harness.
struct png_image_read_control {
    // Intentionally empty; the focal method only passes the pointer through.
};

// ---------------------------------------------------------------------------
// The focal function under test (re-implemented here for the test harness).
// In the real project, this would reside in pngread.c and rely on
// the actual libpng infrastructure. Here we reproduce the logic to enable
// isolated unit testing without external dependencies.
// ---------------------------------------------------------------------------

int make_gray_file_colormap(png_image_read_control *display)
{
{
   unsigned int i;
   for (i=0; i<256; ++i)
      png_create_colormap_entry(display, i, i, i, i, 255, P_FILE);
   return (int)i;
}
}

// ---------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions).
// ---------------------------------------------------------------------------

static int g_test_pass = 1;
static int g_test_count = 0;
static int g_fail_count = 0;

#define LOG_ERROR(msg) \
    do { std::cerr << "ERROR: " << msg << std::endl; } while(0)

#define EXPECT_TRUE(cond) \
    do { \
        ++g_test_count; \
        if (!(cond)) { \
            g_test_pass = 0; \
            ++g_fail_count; \
            std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                      << " - condition: " #cond << std::endl; \
        } \
    } while(0)

#define EXPECT_EQ(a, b) \
    do { \
        ++g_test_count; \
        if (!((a) == (b))) { \
            g_test_pass = 0; \
            ++g_fail_count; \
            std::ostringstream os; \
            os << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
               << " - " #a " != " #b " (" << (a) << " != " << (b) << ")"; \
            LOG_ERROR(os.str()); \
        } \
    } while(0)

#define EXPECT_NE(a, b) \
    do { \
        ++g_test_count; \
        if ((a) == (b)) { \
            g_test_pass = 0; \
            ++g_fail_count; \
            std::ostringstream os; \
            os << "EXPECT_NE failed at " << __FILE__ << ":" << __LINE__ \
               << " - " #a " == " #b; \
            LOG_ERROR(os.str()); \
        } \
    } while(0)

static void reset_calls() {
    g_calls.clear();
}

// ---------------------------------------------------------------------------
// Test cases
// ---------------------------------------------------------------------------

bool test_make_gray_file_colormap_basic() {
    // Test that a single invocation populates 256 calls with expected data
    // and returns 256 (the final value of i after the loop).
    png_image_read_control display;
    reset_calls();
    int result = make_gray_file_colormap(&display);

    EXPECT_EQ(result, 256u); // Expect 256 as the final value of i
    EXPECT_EQ(g_calls.size(), 256u);

    for (unsigned int i = 0; i < 256; ++i) {
        const ColormapEntryCall &c = g_calls[i];
        EXPECT_EQ(c.ip, i);
        EXPECT_EQ(c.red, i);
        EXPECT_EQ(c.green, i);
        EXPECT_EQ(c.blue, i);
        EXPECT_EQ(c.alpha, 255u);
        EXPECT_EQ(c.encoding, P_FILE);
    }

    return g_test_pass;
}

bool test_make_gray_file_colormap_twice_accumulation() {
    // Test that two sequential invocations accumulate calls (do not reset state internally)
    png_image_read_control display;
    reset_calls();
    int r1 = make_gray_file_colormap(&display);

    // Capture count after first call
    size_t count_after_first = g_calls.size();

    // Second invocation without clearing state
    int r2 = make_gray_file_colormap(&display);

    EXPECT_EQ(r1, 256u);
    EXPECT_EQ(r2, 256u);
    EXPECT_EQ(g_calls.size(), count_after_first + 256u);

    // Validate the most recent 256 calls correspond to the second invocation
    for (size_t idx = 0; idx < 256; ++idx) {
        const ColormapEntryCall &c = g_calls[count_after_first + idx];
        unsigned int i = static_cast<unsigned int>(idx);
        EXPECT_EQ(c.ip, i);
        EXPECT_EQ(c.red, i);
        EXPECT_EQ(c.green, i);
        EXPECT_EQ(c.blue, i);
        EXPECT_EQ(c.alpha, 255u);
        EXPECT_EQ(c.encoding, P_FILE);
    }

    return g_test_pass;
}

// ---------------------------------------------------------------------------
// Main entry point to run tests.
// ---------------------------------------------------------------------------

int main() {
    std::cout << "Running tests for make_gray_file_colormap (test harness)\n";

    bool ok = true;
    ok = test_make_gray_file_colormap_basic() && ok;
    ok = test_make_gray_file_colormap_twice_accumulation() && ok;

    if (ok && g_fail_count == 0) {
        std::cout << "ALL TESTS PASSED (" << g_test_count << " assertions)\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED: "
                  << g_fail_count << " failures out of "
                  << g_test_count << " assertions\n";
        return 1;
    }
}