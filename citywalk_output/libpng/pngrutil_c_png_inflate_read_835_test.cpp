// Self-contained C++11 unit test suite for the focal method png_inflate_read
// This test harness provides a minimal, self-contained environment to exercise
// the control flow of the png_inflate_read logic (as provided in the prompt).
// It does not rely on GTest. Instead, it uses simple bool-returning tests
// and a tiny test runner with explanatory comments for each test case.
//
// Note: This is a focused, self-contained test harness. It mocks only the
// essential pieces needed by the focal method (png_inflate_read) to enable
// execution in a standalone C++11 environment.

#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cassert>


// Forward declarations for the test-scaffolded implementation of the focal method.
// We implement a self-contained, minimal replica of the environment needed by
// png_inflate_read (types, constants, and dependencies) to enable unit testing.

typedef unsigned int uInt;
typedef unsigned int png_uint_32;
typedef unsigned char png_bytep;
typedef png_bytep png_const_bytep;
typedef unsigned long png_alloc_size_t;
typedef struct png_struct png_struct;
typedef png_struct* png_structrp;
typedef png_uint_32* png_uint_32p;

// Constants used by the focal method (subset for testing)
const int Z_OK = 0;
const int Z_FINISH = 1;
const int Z_SYNC_FLUSH = 2;
const int Z_NO_FLUSH = 3;
const int Z_STREAM_ERROR = -1;

const unsigned int ZLIB_IO_MAX = 4096;

// PNGZ message macro (as used by the focal code)
#define PNGZ_MSG_CAST(x) (x)

// Public types used by the focal function (minimal subset)
struct PNG_Zstream {
    png_bytep next_out;
    unsigned int avail_out;
    png_bytep next_in;
    unsigned int avail_in;
    const char* msg; // error message pointer (simplified)
};

struct png_struct {
    void* zowner;
    void* chunk_name;
    PNG_Zstream zstream;
    int inflate_calls; // internal counter used by fake inflate
};

// Typedefs matching typical libpng naming (simplified)
typedef struct png_struct png_struct;
typedef png_struct* png_structrp;

// Globals used by the test harness to track calls
static int g_crc_read_calls = 0;
static int g_crc_last_length = 0;

// Read buffer helper for tests
static unsigned char g_read_buffer_storage[256];

// Stub: png_crc_read (mock)
static void png_crc_read(png_structrp png_ptr, png_bytep buf, unsigned int length)
{
    // In a real libpng test, this would validate/consume CRC bytes.
    // Here we simply record that a read occurred.
    (void)buf; // not used in test; we only track invocation count and length
    ++g_crc_read_calls;
    g_crc_last_length = static_cast<int>(length);
}

// Stub: png_zstream_error (mock)
static void png_zstream_error(png_structrp png_ptr, int ret)
{
    // Minimal behavior for test: store a message if an error occurred.
    (void)png_ptr;
    (void)ret;
    // No-op for the test harness beyond preventing unused warnings
}

// Forward declare the fake inflater and wire macro
static int fake_png_inflate(png_structrp png_ptr, int flush);
#define PNG_INFLATE(png_ptr, flush) fake_png_inflate((png_ptr), (flush))

// Fake inflater: deterministic behavior to keep tests finite and predictable
static int fake_png_inflate(png_structrp png_ptr, int flush)
{
    // Simple deterministic behavior to avoid infinite loops in tests.
    // - On first call, if flush == Z_NO_FLUSH, allocate some output space and return Z_OK.
    // - On subsequent call, release the remaining output space and return Z_OK.
    // - If flush indicates Z_FINISH, return Z_FINISH to cover the finish path.
    if (png_ptr == nullptr) return Z_OK;

    if (png_ptr->inflate_calls == 0) {
        // First call: provide a small amount of output (simulate produced data)
        // The focal loop will use avail_out as a signal for produced bytes.
        png_ptr->zstream.avail_out = (flush == Z_NO_FLUSH) ? 1U : 0U;
    } else {
        // After the first call, simulate completion of output in the next step.
        png_ptr->zstream.avail_out = 0;
    }

    // Increment internal call counter
    ++(png_ptr->inflate_calls);

    // If the caller requested finish, honor it
    if (flush == Z_FINISH) {
        return Z_FINISH;
    }

    // Normal inflate behavior simulated as OK
    return Z_OK;
}

// Implementation of the focal method (self-contained replica for testing)
static int png_inflate_read(png_structrp png_ptr, png_bytep read_buffer, uInt read_size,
    png_uint_32p chunk_bytes, png_bytep next_out, png_alloc_size_t *out_size,
    int finish)
{
{
   if (png_ptr->zowner == png_ptr->chunk_name)
   {
      int ret;
      /* next_in and avail_in must have been initialized by the caller. */
      png_ptr->zstream.next_out = next_out;
      png_ptr->zstream.avail_out = 0; /* set in the loop */
      do
      {
         if (png_ptr->zstream.avail_in == 0)
         {
            if (read_size > *chunk_bytes)
               read_size = (uInt)*chunk_bytes;
            *chunk_bytes -= read_size;
            if (read_size > 0)
               png_crc_read(png_ptr, read_buffer, read_size);
            png_ptr->zstream.next_in = read_buffer;
            png_ptr->zstream.avail_in = read_size;
         }
         if (png_ptr->zstream.avail_out == 0)
         {
            uInt avail = ZLIB_IO_MAX;
            if (avail > *out_size)
               avail = (uInt)*out_size;
            *out_size -= avail;
            png_ptr->zstream.avail_out = avail;
         }
         /* Use Z_SYNC_FLUSH when there is no more chunk data to ensure that all
          * the available output is produced; this allows reading of truncated
          * streams.
          */
         ret = PNG_INFLATE(png_ptr, *chunk_bytes > 0 ?
             Z_NO_FLUSH : (finish ? Z_FINISH : Z_SYNC_FLUSH));
      }
      while (ret == Z_OK && (*out_size > 0 || png_ptr->zstream.avail_out > 0));
      *out_size += png_ptr->zstream.avail_out;
      png_ptr->zstream.avail_out = 0; /* Should not be required, but is safe */
      /* Ensure the error message pointer is always set: */
      png_zstream_error(png_ptr, ret);
      return ret;
   }
   else
   {
      png_ptr->zstream.msg = PNGZ_MSG_CAST("zstream unclaimed");
      return Z_STREAM_ERROR;
   }
}
}

// Helper: initialize a png_ptr with sane defaults for tests
static void init_png_ptr(png_struct* p)
{
    p->zowner = reinterpret_cast<void*>(0x1);
    p->chunk_name = reinterpret_cast<void*>(0x1); // same as zowner for "owned" path
    p->zstream.next_out = nullptr;
    p->zstream.avail_out = 0;
    p->zstream.next_in = nullptr;
    p->zstream.avail_in = 0;
    p->zstream.msg = nullptr;
    p->inflate_calls = 0;
}

// Test 1: zstream unclaimed should yield Z_STREAM_ERROR and set message
static bool test_unclaimed_zstream()
{
    // Prepare a png_ptr where zowner != chunk_name
    png_struct_ptr:
    png_struct* ptr = new png_struct;
    init_png_ptr(ptr);

    // Deliberately mismatch zowner and chunk_name
    ptr->zowner = reinterpret_cast<void*>(0xDEAD);
    ptr->chunk_name = reinterpret_cast<void*>(0xBEEF);

    // Buffers and state for call
    unsigned char read_buf[16] = {0};
    png_uint_32 chunk_bytes = 5;
    unsigned int out_size_val = 4;
    unsigned char out_buf[32] = {0};
    png_bytep next_out = out_buf;
    png_alloc_size_t* out_size = &out_size_val;

    int ret = png_inflate_read(ptr, read_buf, 1, &chunk_bytes, next_out, out_size, 0);

    bool ok = (ret == Z_STREAM_ERROR) && (ptr->zstream.msg != nullptr)
              && (std::strcmp(ptr->zstream.msg, "zstream unclaimed") == 0);

    delete ptr;
    if (!ok) {
        std::cerr << "test_unclaimed_zstream: Expected Z_STREAM_ERROR and zstream unclaimed message.\n";
    }
    return ok;
}

// Test 2: Normal path should call png_crc_read and return Z_OK (tiny, finite loop)
static bool test_normal_path_calls_crc_and_returns_ok()
{
    // Owned zstream path
    png_struct* ptr = new png_struct;
    init_png_ptr(ptr);

    // Ensure zowner == chunk_name to take the first branch
    ptr->zowner = reinterpret_cast<void*>(0xABC);
    ptr->chunk_name = reinterpret_cast<void*>(0xABC);

    // Prepare buffers
    unsigned char read_buf[16] = {0};
    unsigned int read_size = 1;  // simulate small chunk
    png_uint_32 chunk_bytes = 2; // enough data to trigger at least one read
    unsigned int out_size_val = 2; // small output cap
    unsigned char out_buf[16] = {0};
    png_bytep next_out = out_buf;

    png_alloc_size_t* out_size = &out_size_val;

    int ret = png_inflate_read(ptr, read_buf, read_size, &chunk_bytes, next_out, out_size, 0);

    // Expectation: function should complete (ret == Z_OK or Z_FINISH) and crc_read() should have been invoked
    bool ok = (ret == Z_OK) && (g_crc_read_calls > 0);
    if (!ok) {
        std::cerr << "test_normal_path_calls_crc_and_returns_ok: Expected ret==Z_OK and CRC read to be invoked.\n";
    }

    delete ptr;
    // Reset global CRC read counter for other tests
    g_crc_read_calls = 0;
    g_crc_last_length = 0;
    return ok;
}

// Test 3: Finish path should return Z_FINISH when finish flag is set and chunk_bytes is exhausted
static bool test_finish_path_returns_finish()
{
    // Owned zstream path
    png_struct* ptr = new png_struct;
    init_png_ptr(ptr);

    // Owned to ensure first branch
    ptr->zowner = reinterpret_cast<void*>(0x1);
    ptr->chunk_name = reinterpret_cast<void*>(0x1);

    // chunk_bytes 0 so that finish path is chosen (Z_FINISH if finish != 0)
    png_uint_32 chunk_bytes = 0;
    unsigned int read_size = 1;
    unsigned int out_size_val = 0;
    unsigned char out_buf[16] = {0};
    png_bytep next_out = out_buf;
    unsigned char read_buf[16] = {0};

    png_alloc_size_t* out_size = &out_size_val;

    int ret = png_inflate_read(ptr, read_buf, read_size, &chunk_bytes, next_out, out_size, 1); // finish = true

    bool ok = (ret == Z_FINISH);
    if (!ok) {
        std::cerr << "test_finish_path_returns_finish: Expected ret==Z_FINISH when finish is requested.\n";
    }

    delete ptr;
    // Reset global CRC read counter for cleanliness
    g_crc_read_calls = 0;
    g_crc_last_length = 0;
    return ok;
}

// Tiny test runner
int main()
{
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool result){
        ++total;
        if (result) {
            ++passed;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    // Run tests with explanatory output
    run("test_unclaimed_zstream", test_unclaimed_zstream());
    run("test_normal_path_calls_crc_and_returns_ok", test_normal_path_calls_crc_and_returns_ok());
    run("test_finish_path_returns_finish", test_finish_path_returns_finish());

    std::cout << "Tests passed: " << passed << " / " << total << "\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}