// C++11 test suite for the focal method png_handle_eXIf (reproduced in test harness).
// Notes:
// - This test harness provides a self-contained, minimal environment that mimics the
//   relevant portions of the pngrutil-like environment needed by png_handle_eXIf.
// - We intentionally avoid any external test frameworks (GTest, etc.) as requested.
// - Tests are designed to cover true/false branches of key predicates in png_handle_eXIf.
// - Non-terminating assertions are used to maximize code execution and coverage.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Lightweight, self-contained stubs and types to emulate the environment for png_handle_eXIf.
// This is a focused test harness and not a full libpng integration.

using png_uint_32  = uint32_t;
using png_bytep      = unsigned char*;
using png_const_bytep  = const unsigned char*;
using png_structrp   = void*;
using png_inforp      = void*;

// Result indicators mimicking libpng's style
enum { handled_ok = 0, handled_error = 1 };

// Forward declarations of helper functions used by the focal method.
// Implemented below to create a controlled test environment.
static png_bytep png_read_buffer(png_structrp png_ptr, png_uint_32 length);
static void       png_crc_read(png_structrp png_ptr, png_bytep buf, png_uint_32 length);
static int        png_crc_finish(png_structrp png_ptr, png_uint_32 skip);
static void       png_chunk_benign_error(png_structrp png_ptr, const char* err);
static png_uint_32  png_get_uint_32(png_const_bytep buf);
static void       png_set_eXIf_1(png_structrp png_ptr, png_inforp info_ptr,
                               png_uint_32 length, png_bytep data);

// Test context to control behavior of the stubs
struct PngTestCtx {
    // Control flags
    bool simulate_out_of_memory = false;
    bool force_crc_finish_nonzero = false; // if true and skip == 0, return non-zero

    // Header we want to place into the first 4 bytes of the buffer
    uint32_t header_to_write = 0;

    // Buffer returned by png_read_buffer
    unsigned char* mem = nullptr;
    size_t mem_len = 0;

    // Exif data capture (set_eXIf_1 call)
    bool set_eXIf_called = false;
    uint32_t exif_length = 0;
    unsigned char* exif_buffer = nullptr;

    // Error tracking
    const char* last_error = nullptr;
};

// Global test context pointer used by the stubs
static PngTestCtx* g_ctx = nullptr;

// Implementation of the focal method (reproduced for the test harness)
int png_handle_eXIf(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{
    // Core logic copied from the provided snippet, adapted for the harness.
    {
        png_bytep buffer = NULL;
        // In real code, there might be a png_debug call here.
        buffer = png_read_buffer(png_ptr, length);
        if (buffer == NULL)
        {
            png_crc_finish(png_ptr, length);
            png_chunk_benign_error(png_ptr, "out of memory");
            return handled_error;
        }
        png_crc_read(png_ptr, buffer, length);
        if (png_crc_finish(png_ptr, 0) != 0)
            return handled_error;
        {
            png_uint_32 header = png_get_uint_32(buffer);
            if (header != 0x49492A00 && header != 0x4D4D002A)
            {
                png_chunk_benign_error(png_ptr, "invalid");
                return handled_error;
            }
        }
        png_set_eXIf_1(png_ptr, info_ptr, length, buffer);
        return handled_ok;
    }
}

// Stub implementations for test control

static png_bytep png_read_buffer(png_structrp png_ptr, png_uint_32 length)
{
    PngTestCtx* ctx = g_ctx;
    (void)png_ptr; // unused in harness
    if (ctx == nullptr) return nullptr;
    if (ctx->simulate_out_of_memory) return nullptr;

    ctx->mem_len = static_cast<size_t>(length);
    ctx->mem = new unsigned char[length];
    // Fill the first 4 bytes with the requested header value, if length >= 4
    uint32_t header = ctx->header_to_write;
    unsigned char b0 = (header >> 24) & 0xFF;
    unsigned char b1 = (header >> 16) & 0xFF;
    unsigned char b2 = (header >> 8) & 0xFF;
    unsigned char b3 = header & 0xFF;
    if (length >= 4) {
        ctx->mem[0] = b0;
        ctx->mem[1] = b1;
        ctx->mem[2] = b2;
        ctx->mem[3] = b3;
        // Rest of buffer can be zero
        for (size_t i = 4; i < length; ++i) ctx->mem[i] = 0;
    }
    ctx->exif_buffer = ctx->mem; // expose for verification
    return ctx->mem;
}

static void png_crc_read(png_structrp png_ptr, png_bytep buf, png_uint_32 length)
{
    // In harness, we don't need to modify buffer; function exists to mirror API.
    (void)png_ptr; (void)buf; (void)length;
}

static int png_crc_finish(png_structrp png_ptr, png_uint_32 skip)
{
    PngTestCtx* ctx = g_ctx;
    (void)png_ptr; (void)skip;
    if (ctx == nullptr) return 0;
    if (ctx->force_crc_finish_nonzero && skip == 0) return 1;
    return 0;
}

static void png_chunk_benign_error(png_structrp png_ptr, const char* err)
{
    PngTestCtx* ctx = g_ctx;
    (void)png_ptr;
    if (ctx && err) {
        ctx->last_error = err;
    }
}

static png_uint_32 png_get_uint_32(png_const_bytep buf)
{
    // Big-endian interpretation to match PNG header convention
    return (static_cast<uint32_t>(buf[0]) << 24) |
           (static_cast<uint32_t>(buf[1]) << 16) |
           (static_cast<uint32_t>(buf[2]) << 8)  |
            static_cast<uint32_t>(buf[3]);
}

static void png_set_eXIf_1(png_structrp png_ptr, png_inforp info_ptr,
                          png_uint_32 length, png_bytep data)
{
    (void)png_ptr; (void)info_ptr;
    // In harness, record that the setter was invoked and what buffer was used
    PngTestCtx* ctx = g_ctx;
    if (ctx) {
        ctx->set_eXIf_called = true;
        ctx->exif_length = length;
        // Keep a reference to the data buffer
        ctx->exif_buffer = data;
    }
}

// Simple testing framework: non-terminating assertions with summary

static int g_total_failures = 0;
#define T_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << msg << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_total_failures; \
    } \
} while(0)

static void test_success_header_II()
{
    // Test: valid header 0x49492A00 should succeed (handled_ok) and call set_eXIf_1
    PngTestCtx ctx;
    std::memset(&ctx, 0, sizeof(ctx));
    g_ctx = &ctx;

    ctx.header_to_write = 0x49492A00; // "II*"
    ctx.simulate_out_of_memory = false;
    ctx.force_crc_finish_nonzero = false;

    int ret = png_handle_eXIf((png_structrp)&ctx, (png_inforp)nullptr, 4);

    T_ASSERT(ret == handled_ok, "expected handled_ok for header II ('II*')");
    T_ASSERT(ctx.set_eXIf_called, "png_set_eXIf_1 should be called for valid header");
    T_ASSERT(ctx.exif_length == 4, "length passed to png_set_eXIf_1 should equal chunk length");
    T_ASSERT(ctx.exif_buffer == ctx.mem, "buffer passed to png_set_eXIf_1 should be the allocated buffer");
    if (ctx.mem) delete[] ctx.mem;
    g_ctx = nullptr;
}

static void test_success_header_MM()
{
    // Test: valid header 0x4D4D002A should also succeed
    PngTestCtx ctx;
    std::memset(&ctx, 0, sizeof(ctx));
    g_ctx = &ctx;

    ctx.header_to_write = 0x4D4D002A; // "MM*"
    ctx.simulate_out_of_memory = false;
    ctx.force_crc_finish_nonzero = false;

    int ret = png_handle_eXIf((png_structrp)&ctx, (png_inforp)nullptr, 4);

    T_ASSERT(ret == handled_ok, "expected handled_ok for header MM ('MM*')");
    T_ASSERT(ctx.set_eXIf_called, "png_set_eXIf_1 should be called for valid header MM");
    T_ASSERT(ctx.exif_length == 4, "length passed to png_set_eXIf_1 should equal chunk length");
    T_ASSERT(ctx.exif_buffer == ctx.mem, "buffer passed to png_set_eXIf_1 should be the allocated buffer");
    if (ctx.mem) delete[] ctx.mem;
    g_ctx = nullptr;
}

static void test_out_of_memory_path()
{
    // Test: png_read_buffer returns NULL -> handled_error and "out of memory" error
    PngTestCtx ctx;
    std::memset(&ctx, 0, sizeof(ctx));
    g_ctx = &ctx;

    ctx.header_to_write = 0x49492A00;
    ctx.simulate_out_of_memory = true;

    int ret = png_handle_eXIf((png_structrp)&ctx, (png_inforp)nullptr, 4);

    T_ASSERT(ret == handled_error, "expected handled_error on out-of-memory condition");
    T_ASSERT(ctx.last_error != nullptr && std::strcmp(ctx.last_error, "out of memory") == 0,
             "expected 'out of memory' benign error message");
    g_ctx = nullptr;
}

static void test_crc_failure_path()
{
    // Test: CRC finish returns non-zero on finalization -> handled_error
    PngTestCtx ctx;
    std::memset(&ctx, 0, sizeof(ctx));
    g_ctx = &ctx;

    ctx.header_to_write = 0x49492A00;
    ctx.simulate_out_of_memory = false;
    ctx.force_crc_finish_nonzero = true; // cause non-zero return for final crc check

    int ret = png_handle_eXIf((png_structrp)&ctx, (png_inforp)nullptr, 4);

    T_ASSERT(ret == handled_error, "expected handled_error when CRC finish indicates error");
    g_ctx = nullptr;
}

static void test_invalid_header_path()
{
    // Test: header invalid -> handled_error and "invalid" message
    PngTestCtx ctx;
    std::memset(&ctx, 0, sizeof(ctx));
    g_ctx = &ctx;

    ctx.header_to_write = 0x01020304; // invalid header
    ctx.simulate_out_of_memory = false;
    ctx.force_crc_finish_nonzero = false;

    int ret = png_handle_eXIf((png_structrp)&ctx, (png_inforp)nullptr, 4);

    T_ASSERT(ret == handled_error, "expected handled_error for invalid header");
    T_ASSERT(ctx.last_error != nullptr && std::strcmp(ctx.last_error, "invalid") == 0,
             "expected 'invalid' benign error message");
    g_ctx = nullptr;
}

// Runner

int main()
{
    test_success_header_II();
    test_success_header_MM();
    test_out_of_memory_path();
    test_crc_failure_path();
    test_invalid_header_path();

    if (g_total_failures == 0) {
        std::cout << "All tests passed (no assertion failures)." << std::endl;
        return 0;
    } else {
        std::cerr << g_total_failures << " tests failed." << std::endl;
        return 1;
    }
}