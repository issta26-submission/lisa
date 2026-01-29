// Self-contained C++11 unit tests for the focal method png_read_finish_IDAT
// This test suite mimics the dependencies (png_struct, constants, and dependent
// functions) to validate the control flow of png_read_finish_IDAT without a real
// libpng build. It intentionally uses simple asserts and a lightweight test harness
// since GTest is not allowed per instructions.

// NOTE: This file is a standalone test harness and includes a minimal in-memory
// emulation of the necessary libpng structures and functions used by png_read_finish_IDAT.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstddef>


// ------------------------------------------------------------
// Minimal emulation of required types and constants
// ------------------------------------------------------------

typedef unsigned char* png_bytep;
typedef const unsigned char* png_const_bytep;
typedef uint32_t png_uint_32;
typedef size_t png_alloc_size_t;

enum {
    PNG_FLAG_ZSTREAM_ENDED = 0x01,
    PNG_AFTER_IDAT        = 0x02
};

static const uint32_t PNG_IDAT = 1; // owner identifier (simplified)

// Forward declaration for the struct used by the focal function
struct png_struct;

// Pointer-to-struct typedefs to mirror libpng naming
typedef png_struct* png_structrp;
typedef png_structrp png_structrp_alias; // alias for clarity if needed

// Pixel/stream sub-structure used by libpng (simplified)
struct png_zstream_s {
    void* next_out;
    void* next_in;
    uint32_t avail_in;
    uint32_t avail_out;
};

// Core png_struct (simplified for testing the focal method)
struct png_struct {
    uint32_t flags;     // internal flags (e.g., PNG_FLAG_ZSTREAM_ENDED)
    uint32_t mode;      // mode/status bits (e.g., PNG_AFTER_IDAT)
    uint32_t zowner;    // owner of the zlib stream (e.g., png_IDAT)
    uint32_t idat_size; // remaining/end bytes for IDAT (passed to crc_finish)
    png_zstream_s zstream; // zlib stream descriptor
};

// ------------------------------------------------------------
// Declarations of focal-dependent functions (to be implemented in this test)
// We provide simplified in-file implementations to exercise png_read_finish_IDAT logic
// without requiring the entirety of libpng.

// Prototypes matching the usage inside png_read_finish_IDAT
void png_read_IDAT_data(png_structrp png_ptr, png_bytep output, png_alloc_size_t avail_out);
void png_crc_finish(png_structrp png_ptr, uint32_t skip);

// The focal method under test (copied/translated from the snippet)
void png_read_finish_IDAT(png_structrp png_ptr)
{
    {
        // We don't need any more data and the stream should have ended, however the
        // LZ end code may actually not have been processed.  In this case we must
        // read it otherwise stray unread IDAT data or, more likely, an IDAT chunk
        // may still remain to be consumed.
        if ((png_ptr->flags & PNG_FLAG_ZSTREAM_ENDED) == 0)
        {
            // The NULL causes png_read_IDAT_data to swallow any remaining bytes in
            // the compressed stream, but the stream may be damaged too, so even after
            // this call we may need to terminate the zstream ownership.
            png_read_IDAT_data(png_ptr, NULL, 0);
            png_ptr->zstream.next_out = NULL; /* safety */
            /* Now clear everything out for safety; the following may not have been
             * done.
             */
            if ((png_ptr->flags & PNG_FLAG_ZSTREAM_ENDED) == 0)
            {
                png_ptr->mode |= PNG_AFTER_IDAT;
                png_ptr->flags |= PNG_FLAG_ZSTREAM_ENDED;
            }
        }
        /* If the zstream has not been released do it now *and* terminate the reading
         * of the final IDAT chunk.
         */
        if (png_ptr->zowner == PNG_IDAT)
        {
            /* Always do this; the pointers otherwise point into the read buffer. */
            png_ptr->zstream.next_in = NULL;
            png_ptr->zstream.avail_in = 0;
            /* Now we no longer own the zstream. */
            png_ptr->zowner = 0;
            /* The slightly weird semantics of the sequential IDAT reading is that we
             * are always in or at the end of an IDAT chunk, so we always need to do a
             * crc_finish here.  If idat_size is non-zero we also need to read the
             * spurious bytes at the end of the chunk now.
             */
            (void)png_crc_finish(png_ptr, png_ptr->idat_size);
        }
    }
}

// ------------------------------------------------------------
// Lightweight test harness
// ------------------------------------------------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Per-test controllable behavior for the fake png_read_IDAT_data
static int g_read_idat_data_should_set_flag = 0;
// Counters for assertions inside tests
static int g_read_idat_data_calls = 0;
static int g_crc_finish_calls = 0;
static uint32_t g_last_crc_skip = 0;

// Simple expectation helper (non-terminating: records failure but continues)
void expect(bool pred, const char* desc)
{
    ++g_total_tests;
    if (!pred) {
        ++g_failed_tests;
        std::cerr << "TEST FAILURE: " << desc << std::endl;
    }
}

// Fake implementation of png_read_IDAT_data used by the focal method
void png_read_IDAT_data(png_structrp png_ptr, png_bytep /*output*/, png_alloc_size_t /*avail_out*/)
{
    ++g_read_idat_data_calls;
    if (g_read_idat_data_should_set_flag) {
        png_ptr->flags |= PNG_FLAG_ZSTREAM_ENDED;
    }
    // In a real scenario, this would swallow remaining data. Here we only
    // simulate the side effect controlled by the test (set or not set the flag).
}

// Fake implementation of png_crc_finish used by the focal method
void png_crc_finish(png_structrp /*png_ptr*/, uint32_t skip)
{
    ++g_crc_finish_calls;
    g_last_crc_skip = skip;
}

// ------------------------------------------------------------
// Test cases
// ------------------------------------------------------------

// Helper to init a png_struct with default plausible values
void init_png_struct(png_struct &p)
{
    p.flags = 0;
    p.mode = 0;
    p.zowner = 0;
    p.idat_size = 0;
    p.zstream.next_out = nullptr;
    p.zstream.next_in  = nullptr;
    p.zstream.avail_in = 0;
    p.zstream.avail_out = 0;
}

// Test Case A1: ZSTREAM_ENDED initially 0, and png_read_IDAT_data does NOT set ZSTREAM_ENDED.
// Expected: first block processes, inner if runs, mode gets PNG_AFTER_IDAT, ZSTREAM_ENDED set,
// then zowner==png_IDAT path triggers crc_finish. next_out becomes NULL, next_in NULL, zowner reset.
void test_case_A1()
{
    png_struct s;
    init_png_struct(s);
    s.flags = 0;                // ZSTREAM_ENDED not set
    s.zowner = PNG_IDAT;
    s.idat_size = 123;
    s.zstream.next_out = reinterpret_cast<void*>(0x1234);
    s.zstream.next_in  = reinterpret_cast<void*>(0x5678);
    s.zstream.avail_in = 42;
    s.zstream.avail_out = 64;

    g_read_idat_data_calls = 0;
    g_crc_finish_calls = 0;
    g_last_crc_skip = 0;
    g_read_idat_data_should_set_flag = 0; // ensure inner if runs

    png_read_finish_IDAT(&s);

    expect(s.zstream.next_out == nullptr, "A1: next_out should be NULL after png_read_finish_IDAT");
    expect((s.mode & PNG_AFTER_IDAT) != 0, "A1: mode should include PNG_AFTER_IDAT after finish_IDAT");
    expect((s.flags & PNG_FLAG_ZSTREAM_ENDED) != 0, "A1: PNG_FLAG_ZSTREAM_ENDED should be set");
    expect(s.zowner == 0, "A1: zowner should be reset to 0");
    expect(s.zstream.next_in == nullptr, "A1: next_in should be NULL");
    expect(g_crc_finish_calls == 1, "A1: crc_finish should be called once");
    expect(g_last_crc_skip == s.idat_size, "A1: crc_finish skip should equal idat_size");
}

// Test Case A2: ZSTREAM_ENDED initially 0, but png_read_IDAT_data sets ZSTREAM_ENDED to 1.
// Expected: first block executes png_read_IDAT_data (flag set), inner if false (no PNG_AFTER_IDAT),
// nonetheless the second block runs and crc_finish is still called. next_out is NULL; zowner reset.
void test_case_A2()
{
    png_struct s;
    init_png_struct(s);
    s.flags = 0;                // ZSTREAM_ENDED not set initially
    s.zowner = PNG_IDAT;
    s.idat_size = 55;
    s.zstream.next_out = reinterpret_cast<void*>(0xABCD);
    s.zstream.next_in  = reinterpret_cast<void*>(0xDEAD);
    s.zstream.avail_in = 7;
    s.zstream.avail_out = 8;

    g_read_idat_data_calls = 0;
    g_crc_finish_calls = 0;
    g_last_crc_skip = 0;
    g_read_idat_data_should_set_flag = 1; // cause to set ZSTREAM_ENDED in the first block

    png_read_finish_IDAT(&s);

    // next_out should still be NULL because it's set inside the first block
    expect(s.zstream.next_out == nullptr, "A2: next_out should be NULL after finish_IDAT");
    // mode should not gain PNG_AFTER_IDAT since inner if didn't run
    expect((s.mode & PNG_AFTER_IDAT) == 0, "A2: mode should NOT include PNG_AFTER_IDAT when inner condition skipped");
    // ZSTREAM_ENDED is now set by the fake png_read_IDAT_data
    expect((s.flags & PNG_FLAG_ZSTREAM_ENDED) != 0, "A2: PNG_FLAG_ZSTREAM_ENDED should be set by read_IDAT_data");
    expect(s.zowner == 0, "A2: zowner should be reset to 0");
    expect(s.zstream.next_in == nullptr, "A2: next_in should be NULL");
    expect(g_crc_finish_calls == 1, "A2: crc_finish should be called once");
    expect(g_last_crc_skip == s.idat_size, "A2: crc_finish skip should equal idat_size");
}

// Test Case D: zowner is not PNG_IDAT; ensure crc_finish is not invoked.
// Expected: first block may set next_out NULL, but the second block does not call crc_finish.
void test_case_D()
{
    png_struct s;
    init_png_struct(s);
    s.flags = 0;
    s.zowner = 0xFF;            // not PNG_IDAT
    s.idat_size = 9;
    s.zstream.next_out = reinterpret_cast<void*>(0xFEDC);
    s.zstream.next_in  = reinterpret_cast<void*>(0xBEAD);
    s.zstream.avail_in = 3;
    s.zstream.avail_out = 4;

    g_read_idat_data_calls = 0;
    g_crc_finish_calls = 0;
    g_last_crc_skip = 0;
    g_read_idat_data_should_set_flag = 0; // first block will run and set next_out NULL

    png_read_finish_IDAT(&s);

    // next_out should be NULL due to first block
    expect(s.zstream.next_out == nullptr, "D: next_out should be NULL after finish_IDAT");
    // zowner should remain unchanged since it was not PNG_IDAT
    expect(s.zowner != 0, "D: zowner should remain non-zero (not reset) when not PNG_IDAT");
    // next_in should remain unchanged
    expect(s.zstream.next_in != nullptr, "D: next_in should remain unchanged when not releasing zstream");
    // crc_finish should not be called because zowner != PNG_IDAT
    expect(g_crc_finish_calls == 0, "D: crc_finish should not be called when zowner != PNG_IDAT");
}

// ------------------------------------------------------------
// Main function to run tests
// ------------------------------------------------------------
int main()
{
    std::cout << "Running unit tests for png_read_finish_IDAT (stand-alone harness)" << std::endl;

    test_case_A1();
    test_case_A2();
    test_case_D();

    // Summary
    std::cout << "Total tests executed: " << g_total_tests << std::endl;
    std::cout << "Tests failed: " << g_failed_tests << std::endl;

    if (g_failed_tests != 0) {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    }
    std::cout << "All tests PASSED." << std::endl;
    return 0;
}