// A self-contained C++11 unit test harness for the focal function
// png_read_push_finish_row, re-implemented in this test environment
// to allow testing without external dependencies (e.g., libpng headers).

#include <cassert>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain knowledge hints applied:
// - Use only standard library features.
// - Implement non-terminating assertions (test continues after failures).
// - Organize tests with explanatory comments above each test.
// - Drive both true/false branches of predicates.
// - Access "static-like" members via file-scope or global variables as needed.
// - Provide a main() that runs test methods (no GTest).

// --------------------------- Mock Environment --------------------------- //

// Enable the interlacing code path (as in PNG_READ_INTERLACING_SUPPORTED)
#define PNG_READ_INTERLACING_SUPPORTED 1
// Bitmask used to simulate transformations flags (PNG_INTERLACE)
#define PNG_INTERLACE 0x01

// Forward declaration of the test's mock type to match the focal prototype
struct MockPNG;
typedef MockPNG* png_structrp;

// Global arrays used by the focal function for interlacing calculations.
// These mimic the PNG pass geometry arrays in libpng.
int png_pass_inc[8]   = {0, 5, 0, 0, 0, 0, 0, 0}; // chosen so pass 1 works
int png_pass_start[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int png_pass_yinc[8]  = {0, 2, 0, 0, 0, 0, 0, 0};
int png_pass_ystart[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// --------------------------- Mock Type Definition --------------------------- //

struct MockPNG {
    // Core state used by png_read_push_finish_row
    int row_number;
    int num_rows;
    int interlaced;
    int width;
    int height;
    int rowbytes;
    int transformations;
    unsigned char* prev_row;

    // Additional fields touched by the focal code
    int iwidth;
    int pass;
};

// --------------------------- Focal Function (Self-contained) --------------------------- //

// The focal function under test. It mirrors the logic from <FOCAL_METHOD>
// but uses the test's MockPNG type and arrays above.
void png_read_push_finish_row(png_structrp png_ptr)
{
    {
       png_ptr->row_number++;
       if (png_ptr->row_number < png_ptr->num_rows)
          return;
#ifdef PNG_READ_INTERLACING_SUPPORTED
       if (png_ptr->interlaced != 0)
       {
          png_ptr->row_number = 0;
          memset(png_ptr->prev_row, 0, png_ptr->rowbytes + 1);
          do
          {
             png_ptr->pass++;
             if ((png_ptr->pass == 1 && png_ptr->width < 5) ||
                 (png_ptr->pass == 3 && png_ptr->width < 3) ||
                 (png_ptr->pass == 5 && png_ptr->width < 2))
                png_ptr->pass++;
             if (png_ptr->pass > 7)
                png_ptr->pass--;
             if (png_ptr->pass >= 7)
                break;
             png_ptr->iwidth = (png_ptr->width +
                 png_pass_inc[png_ptr->pass] - 1 -
                 png_pass_start[png_ptr->pass]) /
                 png_pass_inc[png_ptr->pass];
             if ((png_ptr->transformations & PNG_INTERLACE) != 0)
                break;
             png_ptr->num_rows = (png_ptr->height +
                 png_pass_yinc[png_ptr->pass] - 1 -
                 png_pass_ystart[png_ptr->pass]) /
                 png_pass_yinc[png_ptr->pass];
          } while (png_ptr->iwidth == 0 || png_ptr->num_rows == 0);
       }
#endif /* READ_INTERLACING */
    }
}

// --------------------------- Minimal Test Framework --------------------------- //

static int total_tests = 0;
static int failed_tests = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++total_tests; \
        if (!(cond)) { \
            std::cerr << "EXPECT_TRUE FAILED: " << msg << " (line " << __LINE__ << ")\n"; \
            ++failed_tests; \
        } \
    } while (0)

#define EXPECT_EQ(a, b, msg) \
    do { \
        ++total_tests; \
        if (!((a) == (b))) { \
            std::cerr << "EXPECT_EQ FAILED: " << msg \
                      << " (expected " << (a) << " == " << (b) << ") (line " << __LINE__ << ")\n"; \
            ++failed_tests; \
        } \
    } while (0)

#define ASSERT_TRUE(cond, msg) \
    do { \
        ++total_tests; \
        if (!(cond)) { \
            std::cerr << "ASSERT_TRUE FAILED: " << msg << " (line " << __LINE__ << ")\n"; \
        } \
    } while (0)

// Utility to create and free a prev_row buffer
static unsigned char* alloc_prev_row(int size, unsigned char fill_byte)
{
    unsigned char* buf = new unsigned char[size];
    std::fill(buf, buf + size, fill_byte);
    return buf;
}

// --------------------------- Test Cases ------------------------------------------------ //

// Test 1: Fast path (row_number < num_rows) should increment and return immediately.
// Verifies that no side effects occur on intervening state.
void test_fast_path_increments_row()
{
    // Prepare a mock PNG instance
    MockPNG png;
    png.row_number = 0;
    png.num_rows = 5;
    png.interlaced = 0;        // non-interlaced to skip interlacing block
    png.width = 10;
    png.height = 8;
    png.rowbytes = 12;
    png.transformations = 0;
    png.iwidth = 0;
    png.pass = 0;

    // Prev_row is used by the function; ensure it's distinct so we can detect no change
    std::vector<unsigned char> prev_storage(13, 0xAB); // arbitrary non-zero pattern
    png.prev_row = prev_storage.data();

    // Capture state before
    int before_row = png.row_number;
    int before_len = png.num_rows;
    unsigned char before_first = png.prev_row[0];

    // Call focal function
    png_read_push_finish_row(&png);

    // Validate: row_number should have incremented by 1; num_rows unchanged
    EXPECT_EQ(png.row_number, before_row + 1, "row_number should increment by 1");
    EXPECT_EQ(png.num_rows, before_len, "num_rows should remain unchanged");
    // Prev_row should remain untouched (since we took fast path)
    EXPECT_EQ(png.prev_row[0], before_first, "prev_row should be untouched on fast path");
}

// Test 2: End-of-rows with interlacing disabled (no interlacing path taken when interlaced==0).
// Ensures no crash and proper state when we hit the end of rows without interlacing.
void test_end_of_rows_no_interlacing()
{
    MockPNG png;
    png.row_number = 4;
    png.num_rows = 5;
    png.interlaced = 0;        // interlacing disabled
    png.width = 8;
    png.height = 10;
    png.rowbytes = 12;
    png.transformations = 0;
    png.iwidth = 0;
    png.pass = 0;

    unsigned char* dummy = alloc_prev_row(13, 0x55);
    png.prev_row = dummy;

    // Pre-call: row_number = 4; after increment it will be 5 which equals num_rows
    png_read_push_finish_row(&png);

    // Validate: row_number should now be 5; since interlacing is disabled, no further changes
    EXPECT_EQ(png.row_number, 5, "row_number should reach num_rows when interlacing is off");
    // prev_row should remain as initial (not cleared)
    EXPECT_EQ(png.prev_row[0], 0x55, "prev_row should remain unchanged when interlacing is off");

    delete[] dummy;
}

// Test 3: Interlaced path with a single pass calculation (pass=1) and no early break.
// Verifies that the function resets prev_row and computes iwidth/num_rows as expected.
void test_interlaced_single_pass_initialization()
{
    MockPNG png;
    // Start at end of rows to trigger interlacing path
    png.row_number = 5;
    png.num_rows = 5;
    png.interlaced = 1;
    png.width = 10;     // ensures width >= 5 so pass 1 isn't auto-incremented
    png.height = 10;
    png.rowbytes = 12;
    png.transformations = 0; // do not enter the PNG_INTERLACE break
    png.iwidth = 0;
    png.pass = 0;

    // Prev_row has non-zero data to verify it is zeroed
    unsigned char* buffer = alloc_prev_row(13, 0xAA);
    png.prev_row = buffer;

    // Call focal function
    png_read_push_finish_row(&png);

    // Validate: row_number reset to 0, prev_row zeroed, pass advanced to 1, iwidth computed (>0)
    EXPECT_EQ(png.row_number, 0, "After interlacing start, row_number should reset to 0");
    // prev_row should be zeroed up to rowbytes
    bool all_zero = true;
    for (int i = 0; i <= png.rowbytes; ++i) {
        if (png.prev_row[i] != 0) { all_zero = false; break; }
    }
    EXPECT_TRUE(all_zero, "prev_row should be memset to 0 up to rowbytes inclusive");

    EXPECT_EQ(png.pass, 1, "pass should be incremented to 1 on first interlacing step");
    EXPECT_GT(png.iwidth, 0, "iwidth should be computed and > 0 for first pass");
    EXPECT_EQ(png.num_rows, 5, "num_rows should be recomputed and non-zero after first pass");

    delete[] buffer;
}

// --------------------------- Main Entry Point for Tests --------------------------- //

int main() {
    std::cout << "Running png_read_push_finish_row unit tests (self-contained harness)\n";

    test_fast_path_increments_row();
    test_end_of_rows_no_interlacing();
    test_interlaced_single_pass_initialization();

    std::cout << "Tests completed. Total: " << total_tests
              << ", Failures: " << failed_tests << "\n";

    // Return non-zero if any test failed for convenient CI signaling
    return (failed_tests == 0) ? 0 : 1;
}