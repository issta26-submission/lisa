// Test suite for the focal method make_image (reproduced here for testability in a single TU).
// This suite is designed to exercise both branches (fill_rgb true/false),
// verify memory layout, alignment, stride computation, and per-line padding behavior.
// The implementation here mirrors the provided FOCAL_METHOD and its dependency behavior.

#include <cstring>
#include <lcms2_fast_float.h>
#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <cstdint>


// Domain-specific type aliases (as used by the focal method)
typedef uint32_t cmsUInt32Number;
typedef uint8_t  cmsUInt8Number;
typedef int      cmsBool;

// Helper macro to mimic alignment behavior from the focal method
static inline cmsUInt32Number ALIGNED_SIZE(cmsUInt32Number align, cmsUInt32Number size)
{
    // Align size up to the next multiple of 'align' (e.g., 16-byte alignment)
    return (size + align - 1) & ~(align - 1);
}

// Minimal Fail stub to mirror the original behavior (no longjmp/exit in tests)
static int g_failures_seen = 0;
static void Fail(const char* frm, ...)
{
    // In tests, just increment a counter to acknowledge a failure path was reached.
    (void)frm; // unused in tests, but kept to resemble original signature
    ++g_failures_seen;
}

// The focal method under test (reproduced locally for a self-contained test suite)
void* make_image(size_t size_x, size_t size_y, cmsBool fill_rgb, cmsUInt32Number* stride_x)
{
{
    cmsUInt32Number size_x_aligned = ALIGNED_SIZE(16, static_cast<cmsUInt32Number>(size_x));
    cmsUInt32Number line_size_in_bytes = size_x_aligned * sizeof(cmsUInt32Number); // RGBA
    cmsUInt8Number* ptr_image = (cmsUInt8Number*) std::calloc(size_y, line_size_in_bytes);
    
    if (ptr_image == NULL) Fail("Couldn't allocate memory for image");
    if (fill_rgb)
    {
        size_t line;
        for (line = 0; line < size_y; line++)
        {
            cmsUInt32Number* ptr_line = (cmsUInt32Number*)(ptr_image + line_size_in_bytes * line);          
            cmsUInt32Number argb = 0;
            int col;
            for (col = 0; col < (int)size_x; col++)
                *ptr_line++ = argb++;
                
        }
    }
    *stride_x = line_size_in_bytes;
    return (void*) ptr_image;
}
}

// Simple test harness and assertions (no external testing framework)
static int g_total_tests = 0;
static int g_total_failures = 0;

static void report_failure(const std::string& test_name, const std::string& msg)
{
    std::cerr << "[FAIL] " << test_name << ": " << msg << "\n";
    ++g_total_failures;
}

static void report_success(const std::string& test_name)
{
    std::cout << "[PASS] " << test_name << "\n";
}

// Test 1: fill_rgb = false
// - Verifies that memory is zero-initialized due to calloc
// - Verifies correct stride calculation (aligned width * sizeof(uint32))
// - Verifies per-line padding beyond size_x is zero
static void test_make_image_fill_false()
{
    const std::string test_name = "test_make_image_fill_false";
    ++g_total_tests;

    size_t size_x = 5; // not multiple of 16 to exercise alignment
    size_t size_y = 2;

    cmsUInt32Number stride = 0;
    void* img = make_image(size_x, size_y, 0 /*fill_rgb*/, &stride);
    if (img == nullptr) {
        report_failure(test_name, "Returned NULL image pointer");
        return;
    }

    cmsUInt32Number size_x_aligned = ALIGNED_SIZE(16, static_cast<cmsUInt32Number>(size_x));
    cmsUInt32Number line_size_in_bytes = size_x_aligned * sizeof(cmsUInt32Number);

    if (stride != line_size_in_bytes)
    {
        report_failure(test_name, "stride_x does not equal computed line size in bytes");
        free(img);
        return;
    }

    cmsUInt32Number* data = static_cast<cmsUInt32Number*>(img);

    bool ok = true;

    // Each line's first size_x entries should be zero
    for (size_t line = 0; line < size_y && ok; ++line)
    {
        for (size_t col = 0; col < size_x; ++col)
        {
            cmsUInt32Number val = data[line * size_x_aligned + col];
            if (val != 0U)
            {
                ok = false;
                report_failure(test_name, "Expected zero in all data when fill_rgb is false");
                break;
            }
        }
        // Padding region after size_x should also be zero (calloc'd)
        for (size_t col = size_x; col < size_x_aligned; ++col)
        {
            cmsUInt32Number val = data[line * size_x_aligned + col];
            if (val != 0U)
            {
                ok = false;
                report_failure(test_name, "Expected zero in padding region due to calloc");
                break;
            }
        }
    }

    free(img);
    if (ok) report_success(test_name);
}

// Test 2: fill_rgb = true
// - Verifies first line contains 0,1,2,...,size_x-1 for the first line's data
// - Verifies padding region remains zero
static void test_make_image_fill_true()
{
    const std::string test_name = "test_make_image_fill_true";
    ++g_total_tests;

    size_t size_x = 5;
    size_t size_y = 2;

    cmsUInt32Number stride = 0;
    void* img = make_image(size_x, size_y, 1 /*fill_rgb*/, &stride);
    if (img == nullptr) {
        report_failure(test_name, "Returned NULL image pointer");
        return;
    }

    cmsUInt32Number size_x_aligned = ALIGNED_SIZE(16, static_cast<cmsUInt32Number>(size_x));
    cmsUInt32Number line_size_in_bytes = size_x_aligned * sizeof(cmsUInt32Number);

    if (stride != line_size_in_bytes)
    {
        report_failure(test_name, "stride_x does not equal computed line size in bytes");
        free(img);
        return;
    }

    cmsUInt32Number* data = static_cast<cmsUInt32Number*>(img);

    bool ok = true;

    // Each line should contain 0..size_x-1 for the first size_x elements
    for (size_t line = 0; line < size_y && ok; ++line)
    {
        for (size_t col = 0; col < size_x; ++col)
        {
            cmsUInt32Number val = data[line * size_x_aligned + col];
            if (val != (cmsUInt32Number)col)
            {
                ok = false;
                report_failure(test_name, "Expected argb sequence 0..size_x-1 on each line");
                break;
            }
        }
        // Padding region after size_x should be zero (calloc'd)
        for (size_t col = size_x; col < size_x_aligned; ++col)
        {
            cmsUInt32Number val = data[line * size_x_aligned + col];
            if (val != 0U)
            {
                ok = false;
                report_failure(test_name, "Expected zero in padding region after filled data");
                break;
            }
        }
    }

    free(img);
    if (ok) report_success(test_name);
}

// Test 3: Alignment edge case with size_x not multiple of 16
// - Ensures the first line has correct data 0..size_x-1 and that alignment padding is zero
static void test_make_image_alignment_edge()
{
    const std::string test_name = "test_make_image_alignment_edge";
    ++g_total_tests;

    size_t size_x = 7; // not a multiple of 16
    size_t size_y = 1;

    cmsUInt32Number stride = 0;
    void* img = make_image(size_x, size_y, 1 /*fill_rgb*/, &stride);
    if (img == nullptr) {
        report_failure(test_name, "Returned NULL image pointer");
        return;
    }

    cmsUInt32Number size_x_aligned = ALIGNED_SIZE(16, static_cast<cmsUInt32Number>(size_x));
    cmsUInt32Number line_size_in_bytes = size_x_aligned * sizeof(cmsUInt32Number);

    if (stride != line_size_in_bytes)
    {
        report_failure(test_name, "stride_x does not equal computed line size in bytes (alignment edge)");
        free(img);
        return;
    }

    cmsUInt32Number* data = static_cast<cmsUInt32Number*>(img);

    bool ok = true;

    // Check first line contents
    for (size_t col = 0; col < size_x; ++col)
    {
        cmsUInt32Number val = data[col];
        if (val != (cmsUInt32Number)col)
        {
            ok = false;
            report_failure(test_name, "First line data should be 0..size_x-1 for fill_rgb");
            break;
        }
    }
    // Padding region should be zero
    for (size_t col = size_x; col < size_x_aligned; ++col)
    {
        cmsUInt32Number val = data[col];
        if (val != 0U)
        {
            ok = false;
            report_failure(test_name, "Padding in alignment edge case should be zero");
            break;
        }
    }

    free(img);
    if (ok) report_success(test_name);
}

// Test 4: Multiple lines, ensure padding across lines remains zero
static void test_make_image_multiple_lines_padding()
{
    const std::string test_name = "test_make_image_multiple_lines_padding";
    ++g_total_tests;

    size_t size_x = 6;
    size_t size_y = 3;

    cmsUInt32Number stride = 0;
    void* img = make_image(size_x, size_y, 0 /*fill_rgb*/, &stride);
    if (img == nullptr) {
        report_failure(test_name, "Returned NULL image pointer");
        return;
    }

    cmsUInt32Number size_x_aligned = ALIGNED_SIZE(16, static_cast<cmsUInt32Number>(size_x));
    cmsUInt32Number line_size_in_bytes = size_x_aligned * sizeof(cmsUInt32Number);

    if (stride != line_size_in_bytes)
    {
        report_failure(test_name, "stride_x does not equal computed line size in bytes (multi-line)");
        free(img);
        return;
    }

    cmsUInt32Number* data = static_cast<cmsUInt32Number*>(img);
    bool ok = true;

    for (size_t line = 0; line < size_y && ok; ++line)
    {
        // All data is zero (calloc) when fill_rgb is false
        for (size_t col = 0; col < size_x; ++col)
        {
            cmsUInt32Number val = data[line * size_x_aligned + col];
            if (val != 0U)
            {
                ok = false;
                report_failure(test_name, "Expected zero data for fill_rgb=false across lines");
                break;
            }
        }
        // Padding region
        for (size_t col = size_x; col < size_x_aligned; ++col)
        {
            cmsUInt32Number val = data[line * size_x_aligned + col];
            if (val != 0U)
            {
                ok = false;
                report_failure(test_name, "Padding should be zero across lines when fill_rgb=false");
                break;
            }
        }
    }

    free(img);
    if (ok) report_success(test_name);
}

// Test 5: Ensure multiple allocations produce distinct memory blocks
static void test_make_image_distinct_allocations()
{
    const std::string test_name = "test_make_image_distinct_allocations";
    ++g_total_tests;

    size_t size_x = 8;
    size_t size_y = 2;

    cmsUInt32Number stride1 = 0;
    void* img1 = make_image(size_x, size_y, 0, &stride1);

    cmsUInt32Number stride2 = 0;
    void* img2 = make_image(size_x, size_y, 0, &stride2);

    if (img1 == nullptr || img2 == nullptr)
    {
        report_failure(test_name, "One of the allocations returned NULL");
        if (img1) free(img1);
        if (img2) free(img2);
        return;
    }

    if (img1 == img2)
    {
        report_failure(test_name, "Two independent allocations returned the same pointer (memory overlap)");
    }
    else
    {
        report_success(test_name);
    }

    free(img1);
    free(img2);
}

// Main: run all tests
int main(void)
{
    // Run tests
    test_make_image_fill_false();
    test_make_image_fill_true();
    test_make_image_alignment_edge();
    test_make_image_multiple_lines_padding();
    test_make_image_distinct_allocations();

    std::cout << "Total tests: " << g_total_tests << "\n";
    std::cout << "Failures: " << g_total_failures << "\n";

    // Return non-zero if any test failed
    return (g_total_failures > 0) ? 1 : 0;
}