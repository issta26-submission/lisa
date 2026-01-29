// A self-contained C++11 unit test suite for the focal method count_zero_samples.
// The test suite reproduces minimal dependencies from the provided FOCAL_CLASS_DEP
// and defines a lightweight environment to validate the behavior of count_zero_samples.

#include <vector>
#include <string.h>
#include <time.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <functional>
#include <cmath>
#include <stdio.h>
#include <png.h>


// -----------------------------------------------------------------------------
// Minimal type/shim definitions to mirror the libpng-like environment used by
// count_zero_samples. This keeps the tests self-contained and compilable under
// C++11 without external dependencies.
// -----------------------------------------------------------------------------

typedef void* png_structp;
typedef struct png_row_info png_row_infop;
typedef unsigned char* png_bytep;

struct png_row_info {
    unsigned int width;       // width of the row
    unsigned int rowbytes;    // number of bytes in row (not strictly used in tests)
    unsigned int color_type;  // color type (0, 2, 3, 4, etc.)
    unsigned int bit_depth;   // bit depth per sample
    unsigned int channels;    // number of channels (1-4) (e.g., 3 for RGB)
};

// Global accumulator analogous to the static variable in the original file.
int zero_samples = 0;

// Mocked error handler to emulate libpng error behavior used in the focal method.
// It throws an exception so tests can verify error paths without terminating
// the test runner.
void png_error(png_structp /*png_ptr*/, const char* message)
{
    throw std::runtime_error(message);
}

// The focal method under test (reproduced in a self-contained form for unit testing).
void count_zero_samples(png_structp png_ptr, png_row_infop row_info, png_bytep data)
{
    png_bytep dp = data;
    // The callback should always receive correct parameters.
    if (png_ptr == NULL)
        png_error(png_ptr, "count_zero_samples: bad png_ptr");
    if (row_info == NULL)
        png_error(png_ptr, "count_zero_samples: bad row info");
    if (data == NULL)
        png_error(png_ptr, "count_zero_samples: bad data");

    // Contents of row_info:
    //  png_uint_32 width      width of row
    //  png_uint_32 rowbytes   number of bytes in row
    //  png_byte color_type    color type of pixels
    //  png_byte bit_depth     bit depth of samples
    //  png_byte channels      number of channels (1-4)
    //  png_byte pixel_depth   bits per pixel (depth*channels)

    // Counts the number of zero samples (or zero pixels if color_type is 3)
    if (row_info->color_type == 0 || row_info->color_type == 3)
    {
        int pos = 0;
        unsigned int n, nstop;
        for (n = 0, nstop = row_info->width; n < nstop; n++)
        {
            if (row_info->bit_depth == 1)
            {
                if (((*dp << pos++) & 0x80) == 0)
                    zero_samples++;
                if (pos == 8)
                {
                    pos = 0;
                    dp++;
                }
            }
            if (row_info->bit_depth == 2)
            {
                if (((*dp << (pos += 2)) & 0xc0) == 0)
                    zero_samples++;
                if (pos == 8)
                {
                    pos = 0;
                    dp++;
                }
            }
            if (row_info->bit_depth == 4)
            {
                if (((*dp << (pos += 4)) & 0xf0) == 0)
                    zero_samples++;
                if (pos == 8)
                {
                    pos = 0;
                    dp++;
                }
            }
            if (row_info->bit_depth == 8)
                if (*dp++ == 0)
                    zero_samples++;
            if (row_info->bit_depth == 16)
            {
                if ((*dp | *(dp+1)) == 0)
                    zero_samples++;
                dp += 2;
            }
        }
    }
    else /* Other color types */
    {
        unsigned int n, nstop;
        int channel;
        int color_channels = row_info->channels;
        if (row_info->color_type > 3)
            color_channels--;
        for (n = 0, nstop = row_info->width; n < nstop; n++)
        {
            for (channel = 0; channel < color_channels; channel++)
            {
                if (row_info->bit_depth == 8)
                    if (*dp++ == 0)
                        zero_samples++;
                if (row_info->bit_depth == 16)
                {
                    if ((*dp | *(dp+1)) == 0)
                        zero_samples++;
                    dp += 2;
                }
            }
            if (row_info->color_type > 3)
            {
                dp++;
                if (row_info->bit_depth == 16)
                    dp++;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Lightweight, self-contained test harness (no GTest, no Google Mock).
// It uses simple bool-based test results with descriptive comments for each
// test case as requested in the prompt.
// -----------------------------------------------------------------------------

// Utility to reset the global zero_samples counter before each test
void reset_zero_samples() { zero_samples = 0; }

// Individual test cases

// Test 1: Ensure that passing a NULL png_ptr triggers an error.
bool test_null_png_ptr_error()
{
    reset_zero_samples();
    png_row_infop ri = new png_row_info;
    ri->width = 4;
    ri->rowbytes = 4;
    ri->color_type = 0;
    ri->bit_depth = 1;
    ri->channels = 1;
    unsigned char data_buf[1] = {0x00};

    bool caught = false;
    try {
        count_zero_samples(nullptr, ri, data_buf);
    } catch (const std::exception& /*e*/) {
        caught = true;
    }

    delete ri;
    return caught;
}

// Test 2: Ensure that passing a NULL row_info triggers an error.
bool test_null_row_info_error()
{
    reset_zero_samples();
    png_structp sptr = reinterpret_cast<png_structp>(0x1);
    unsigned char data_buf[1] = {0x00};

    bool caught = false;
    try {
        count_zero_samples(sptr, nullptr, data_buf);
    } catch (const std::exception& /*e*/) {
        caught = true;
    }
    return caught;
}

// Test 3: Ensure that passing a NULL data buffer triggers an error.
bool test_null_data_error()
{
    reset_zero_samples();
    png_row_infop ri = new png_row_info;
    ri->width = 4;
    ri->rowbytes = 4;
    ri->color_type = 0;
    ri->bit_depth = 1;
    ri->channels = 1;

    bool caught = false;
    try {
        count_zero_samples(reinterpret_cast<png_structp>(0x1), ri, nullptr);
    } catch (const std::exception& /*e*/) {
        caught = true;
    }

    delete ri;
    return caught;
}

// Test 4: color_type 0 (grayscale), bit_depth 1, width 4, all zeros => 4 zero samples
bool test_color_type0_bitdepth1_all_zero()
{
    reset_zero_samples();
    png_row_infop ri = new png_row_info;
    ri->width = 4;
    ri->rowbytes = 1;
    ri->color_type = 0;
    ri->bit_depth = 1;
    ri->channels = 1;

    unsigned char data_buf[1] = { 0x00 };

    count_zero_samples(reinterpret_cast<png_structp>(0x1), ri, data_buf);

    bool ok = (zero_samples == 4);
    delete ri;
    return ok;
}

// Test 5: color_type 0 (grayscale), bit_depth 2, width 4, all zeros => 4 zero samples
bool test_color_type0_bitdepth2_all_zero()
{
    reset_zero_samples();
    png_row_infop ri = new png_row_info;
    ri->width = 4;
    ri->rowbytes = 1;
    ri->color_type = 0;
    ri->bit_depth = 2;
    ri->channels = 1;

    unsigned char data_buf[1] = { 0x00 };

    count_zero_samples(reinterpret_cast<png_structp>(0x1), ri, data_buf);

    bool ok = (zero_samples == 4);
    delete ri;
    return ok;
}

// Test 6: color_type 0 (grayscale), bit_depth 4, width 2, all zeros => 2 zero samples
bool test_color_type0_bitdepth4_all_zero()
{
    reset_zero_samples();
    png_row_infop ri = new png_row_info;
    ri->width = 2;
    ri->rowbytes = 1;
    ri->color_type = 0;
    ri->bit_depth = 4;
    ri->channels = 1;

    unsigned char data_buf[1] = { 0x00 };

    count_zero_samples(reinterpret_cast<png_structp>(0x1), ri, data_buf);

    bool ok = (zero_samples == 2);
    delete ri;
    return ok;
}

// Test 7: color_type 0 (grayscale), bit_depth 8, width 3, all zeros => 3 zero samples
bool test_color_type0_bitdepth8_all_zero()
{
    reset_zero_samples();
    png_row_infop ri = new png_row_info;
    ri->width = 3;
    ri->rowbytes = 3;
    ri->color_type = 0;
    ri->bit_depth = 8;
    ri->channels = 1;

    unsigned char data_buf[3] = { 0x00, 0x00, 0x00 };

    count_zero_samples(reinterpret_cast<png_structp>(0x1), ri, data_buf);

    bool ok = (zero_samples == 3);
    delete ri;
    return ok;
}

// Test 8: color_type 0 (grayscale), bit_depth 16, width 2, all zeros => 2 zero samples
bool test_color_type0_bitdepth16_all_zero()
{
    reset_zero_samples();
    png_row_infop ri = new png_row_info;
    ri->width = 2;
    ri->rowbytes = 4;
    ri->color_type = 0;
    ri->bit_depth = 16;
    ri->channels = 1;

    unsigned char data_buf[4] = { 0x00, 0x00, 0x00, 0x00 };

    count_zero_samples(reinterpret_cast<png_structp>(0x1), ri, data_buf);

    bool ok = (zero_samples == 2);
    delete ri;
    return ok;
}

// Test 9: color_type 2 (truecolor), bit_depth 8, width 3, all zeros => 9 zero samples
bool test_color_type2_bitdepth8_all_zero()
{
    reset_zero_samples();
    png_row_infop ri = new png_row_info;
    ri->width = 3;
    ri->rowbytes = 9;
    ri->color_type = 2;
    ri->bit_depth = 8;
    ri->channels = 3; // RGB

    unsigned char data_buf[9] = {0,0,0, 0,0,0, 0,0,0};

    count_zero_samples(reinterpret_cast<png_structp>(0x1), ri, data_buf);

    bool ok = (zero_samples == 9);
    delete ri;
    return ok;
}

// Test 10: color_type 2 (truecolor), bit_depth 16, width 2, all zeros => 6 zero samples
bool test_color_type2_bitdepth16_all_zero()
{
    reset_zero_samples();
    png_row_infop ri = new png_row_info;
    ri->width = 2;
    ri->rowbytes = 12;
    ri->color_type = 2;
    ri->bit_depth = 16;
    ri->channels = 3; // RGB

    // 6 samples * 2 bytes each = 12 bytes
    unsigned char data_buf[12] = {0,0, 0,0, 0,0,
                                 0,0, 0,0, 0,0};

    count_zero_samples(reinterpret_cast<png_structp>(0x1), ri, data_buf);

    bool ok = (zero_samples == 6);
    delete ri;
    return ok;
}

// Test 11: color_type > 3 path (e.g., 4), bit_depth 8, width 1, channels 2
// Expect zero_samples to count only the non-alpha channel (first byte) and not the extra ones.
bool test_color_type_greater_than_3_path_bitdepth8()
{
    reset_zero_samples();
    png_row_infop ri = new png_row_info;
    ri->width = 1;
    ri->rowbytes = 3;
    ri->color_type = 4; // > 3
    ri->bit_depth = 8;
    ri->channels = 2;   // one color sample + one alpha

    unsigned char data_buf[3] = { 0x00, 0x01, 0x00 }; // first sample zero, alpha non-zero

    count_zero_samples(reinterpret_cast<png_structp>(0x1), ri, data_buf);

    bool ok = (zero_samples == 1);
    delete ri;
    return ok;
}

// -----------------------------------------------------------------------------
// Test runner
// -----------------------------------------------------------------------------

int main()
{
    struct TestCase {
        const char* name;
        std::function<bool()> func;
    };

    std::vector<TestCase> tests = {
        {"test_null_png_ptr_error", test_null_png_ptr_error},
        {"test_null_row_info_error", test_null_row_info_error},
        {"test_null_data_error", test_null_data_error},
        {"test_color_type0_bitdepth1_all_zero", test_color_type0_bitdepth1_all_zero},
        {"test_color_type0_bitdepth2_all_zero", test_color_type0_bitdepth2_all_zero},
        {"test_color_type0_bitdepth4_all_zero", test_color_type0_bitdepth4_all_zero},
        {"test_color_type0_bitdepth8_all_zero", test_color_type0_bitdepth8_all_zero},
        {"test_color_type0_bitdepth16_all_zero", test_color_type0_bitdepth16_all_zero},
        {"test_color_type2_bitdepth8_all_zero", test_color_type2_bitdepth8_all_zero},
        {"test_color_type2_bitdepth16_all_zero", test_color_type2_bitdepth16_all_zero},
        {"test_color_type_greater_than_3_path_bitdepth8", test_color_type_greater_than_3_path_bitdepth8}
    };

    int passed = 0;
    for (const auto& t : tests)
    {
        bool result = false;
        try {
            result = t.func();
        } catch (...) {
            result = false;
        }
        std::cout << "[TEST] " << t.name << " : " << (result ? "PASSED" : "FAILED") << "\n";
        if (result) ++passed;
    }
    std::cout << "\nSummary: " << passed << " / " << tests.size() << " tests passed.\n";

    return (passed == (int)tests.size()) ? 0 : 1;
}