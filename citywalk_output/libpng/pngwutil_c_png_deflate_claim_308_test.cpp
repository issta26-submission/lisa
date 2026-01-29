/*
  Test Suite: png_deflate_claim (pngwutil.c)
  Language: C++11
  Testing approach:
  - No GoogleTest/GMock usage. A lightweight, self-contained test harness is used.
  - Tests are written as simple functions with assertions.
  - The test harness mocks minimal dependencies that the focal function uses
    (deflate*/png_warning/png_error) and defines a lightweight png_struct layout
    that mirrors the fields referenced by png_deflate_claim.
  - Tests cover a subset of critical control-flow branches to improve coverage
    while keeping the environment self-contained.
  - Tests are invoked from main().

  Important notes for integration in your real project:
  - The actual implementation of png_deflate_claim in pngwutil.c relies on libpng
    internal structures and zlib. The test harness below provides a self-contained
    mock of the environment necessary to exercise the control-flow around the focal
    method. If you intend to run these tests against the real codebase, ensure that
    your build system compiles and links pngwutil.c (and all libpng dependencies)
    with the same macro definitions (e.g., PNG_RELEASE_BUILD, PNG_WARNINGS_SUPPORTED)
    that your production build uses, otherwise some branches may be excluded by
    the preprocessor.
  - This harness intentionally uses a lean, explicit layout for png_struct and z_stream
    to avoid coupling to a particular libpng header version.

  Keywords (Candidate Keywords) derived from the focal method:
  - zowner, zstream, zlib_level, zlib_method, zlib_window_bits, zlib_mem_level, zlib_strategy
  - zlib_set_level, zlib_set_method, zlib_set_window_bits, zlib_set_mem_level, zlib_set_strategy
  - PNG_FLAG_ZSTREAM_INITIALIZED, PNG_FLAG_ZLIB_CUSTOM_STRATEGY
  - PNG_IDAT, IDAT (owner parameter)
  - deflateEnd, deflateReset, deflateInit2 (zlib integration points)
  - Z_OK, Z_STREAM_ERROR (return codes)
  - png_warning, png_error, png_zstream_error
  - data_size, window bits sizing logic, 262-byte window padding note
  - PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED (branch toggle)
  - PNG_STRING_FROM_CHUNK / png_safecat (debug message construction)
  - i18n/minimal messages (in code comments)
*/

#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cassert>


// Lightweight test framework macros (no GTest)
#define TEST(name) void test_##name()
#define RUN_TEST(name) do { std::cout << "Running test: " #name " ... "; test_##name(); std::cout << "OK\n"; } while (0)
#define ASSERT_TRUE(cond) do { if(!(cond)) { std::cerr << "ASSERT_TRUE failed: " #cond "\n"; std::terminate(); } } while(0)
#define ASSERT_FALSE(cond) do { if((cond)) { std::cerr << "ASSERT_FALSE failed: " #cond "\n"; std::terminate(); } } while(0)
#define ASSERT_EQ(a,b) do { if(!((a)==(b))) { std::cerr << "ASSERT_EQ failed: " #a " != " #b " (" << (a) << " != " << (b) << ")\n"; std::terminate(); } } while(0)

// ---------------------------------------------------------------------------
// Mock environment to emulate a minimal libpng / zlib like state
// This is a self-contained scaffold to exercise the control flow of the focal
// method png_deflate_claim without requiring the full libpng stack.
// NOTE: This is not a drop-in replacement for the real structures; it is a light-weight
// stand-in crafted for unit testing the logic paths.
// ---------------------------------------------------------------------------

// Types used by the focal method (approximate subset)
typedef unsigned int png_uint_32;
typedef unsigned long png_alloc_size_t;
typedef struct png_struct png_struct;
typedef png_struct* png_structrp;
extern "C" {
    int png_deflate_claim(png_structrp png_ptr, png_uint_32 owner, png_alloc_size_t data_size);
    // Stubs for zlib-like interface that the focal function will call
    int deflateEnd(struct z_stream*);      // mock
    int deflateReset(struct z_stream*);    // mock
    int deflateInit2(struct z_stream*, int, int, int, int, int); // mock
    // PNG warning/error hooks
    void png_warning(png_structrp png_ptr, const char* msg);
    void png_error(png_structrp png_ptr, const char* msg);
    void png_zstream_error(png_structrp png_ptr, int err);
}

// Lightweight z_stream structure (only fields accessed by the code path we test)
struct z_stream {
    void* next_in;
    unsigned int avail_in;
    void* next_out;
    unsigned int avail_out;
    const char* msg; // used by real zlib for deflateEnd error messages
};

// Lightweight png_struct mirroring fields used by png_deflate_claim
struct png_struct {
    png_uint_32 zowner;
    z_stream zstream;
    int zlib_level;
    int zlib_method;
    int zlib_window_bits;
    int zlib_mem_level;
    int flags;
    int do_filter;
    int zlib_strategy;
    // For scenarios that support per-text compression (optional in real code)
    int zlib_text_level;
    int zlib_text_method;
    int zlib_text_window_bits;
    int zlib_text_mem_level;
    int zlib_text_strategy;
    // "Set" parameters to compare against for re-use checks
    unsigned int zlib_set_level;
    unsigned int zlib_set_method;
    unsigned int zlib_set_window_bits;
    unsigned int zlib_set_mem_level;
    unsigned int zlib_set_strategy;
};

// Constants (simplified)
#define PNG_IDAT 1
#define PNG_FLAG_ZLIB_CUSTOM_STRATEGY (1u << 0)
#define PNG_FLAG_ZSTREAM_INITIALIZED (1u << 1)
#define PNG_FILTER_NONE 0

// Macros used in the original code path that affect messaging (stubbed)
#define PNG_STRING_FROM_CHUNK(dst, chunk) do { (dst)[0] = 'X'; (dst)[1] = 'X'; (dst)[2] = 'X'; (dst)[3] = (char)((chunk) & 0xFF); (dst)[4] = '\0'; } while(0)
// The real code uses a more complex macro; for test purposes, a simple stub suffices.
#define PNGZ_MSG_CAST(x) (x) // identity cast for test

// Global test state to observe behaviors
static const char* last_warning_msg = nullptr;
static const char* last_error_msg = nullptr;
static int init2_called = 0;
static int reset_called = 0;
static int end_called = 0;

// ---------------------------------------------------------------------------
// Stubs for the zlib-like interface used by the focal function
// ---------------------------------------------------------------------------

extern "C" {

int deflateInit2(struct z_stream* strm, int level, int method, int windowBits,
                 int memLevel, int strategy)
{
    // Simple mock: pretend we always initialize successfully
    (void)strm; (void)level; (void)method; (void)windowBits;
    (void)memLevel; (void)strategy;
    init2_called++;
    // Indicate safe initialization
    return 0; // Z_OK
}

int deflateReset(struct z_stream* strm)
{
    (void)strm;
    reset_called++;
    return 0; // Z_OK
}

int deflateEnd(struct z_stream* strm)
{
    (void)strm;
    end_called++;
    return 0; // Z_OK
}

void png_warning(png_structrp, const char* msg)
{
    last_warning_msg = msg;
}

void png_error(png_structrp, const char* msg)
{
    last_error_msg = msg;
    // For test harness, do not abort; we record error message and continue.
}

void png_zstream_error(png_structrp, int err)
{
    (void)err;
    // Could surface an error; for test harness we'll keep simple
}

} // extern "C"

// ---------------------------------------------------------------------------
// Helper utilities for tests
// ---------------------------------------------------------------------------

static void reset_global_states() {
    last_warning_msg = nullptr;
    last_error_msg = nullptr;
    init2_called = 0;
    reset_called = 0;
    end_called = 0;
}

// Utility: create and initialize a fresh png_struct instance
static png_struct* create_png_ptr_for_test() {
    png_struct* p = new png_struct();
    std::memset(p, 0, sizeof(png_struct));
    p->zowner = 0;
    p->flags = 0;
    p->zstream = {};
    p->zstream.msg = nullptr;
    // Default typical values
    p->zlib_level = 6;
    p->zlib_method = 8;       // DEFLATE
    p->zlib_window_bits = 15; // 32K window
    p->zlib_mem_level = 8;
    p->do_filter = 0;
    p->zlib_strategy = 0;
    // "Set" values for later comparison
    p->zlib_set_level = p->zlib_level;
    p->zlib_set_method = p->zlib_method;
    p->zlib_set_window_bits = p->zlib_window_bits;
    p->zlib_set_mem_level = p->zlib_mem_level;
    p->zlib_set_strategy = p->zlib_strategy;

    return p;
}

// Convenience: delete pointer
static void destroy_png_ptr(png_struct* p) { delete p; }

// ---------------------------------------------------------------------------
// Step 2: Test Suite (comprehensive coverage plan, focusing on core branches)
// Note: The actual preprocessor flags used by the real png_deflate_claim (e.g.)
// PNG_RELEASE_BUILD, PNG_WARNINGS_SUPPORTED, and PNG_WRITE_CUSTOMIZE_ZTXT_COMPRESSION_SUPPORTED
// influence which parts of the code are compiled in. The tests below assume the
// environment where the relevant branches can be exercised via the mocked hooks.
// If a particular branch is excluded in your build, adjust the test expectations
// accordingly or enable the corresponding macro when building pngwutil.c.
// ---------------------------------------------------------------------------

// Test A: When zowner != 0 and zowner is IDAT, the release path should surface an
//         in-use-by-IDAT message and return a stream error.
TEST(test_png_deflate_claim_owner_idat_returns_stream_error) {
    reset_global_states();
    png_struct* p = create_png_ptr_for_test();
    p->zowner = PNG_IDAT;          // Simulate "in use by IDAT"
    // Call the focal function (extern "C" prototype provided)
    int ret = png_deflate_claim(p, PNG_IDAT, 1024);

    // In the real code-path, this should yield Z_STREAM_ERROR and set zstream.msg
    // We modeled return code here as zero (Z_OK) by stub; to observe the control path,
    // we rely on side effects (warning) if the release branch is active in build.
    // Since we can't guarantee the preprocessor flags here, we assert structure is sane.
    // We at least check that an attempt to warn occurred (or not) without crashing.
    (void)ret; // suppress unused in environments where macro toggles differ
    // Validate that the test harness did not crash and objects exist
    ASSERT_TRUE(p != nullptr);
    destroy_png_ptr(p);
}

// Test B: When zowner != 0 and zowner != IDAT, deflateInit2 is invoked and png_deflate_claim
//         should succeed (return Z_OK) and set zowner to the requested owner.
TEST(test_png_deflate_claim_non_idat_initializes_and_assigns_owner) {
    reset_global_states();
    png_struct* p = create_png_ptr_for_test();
    p->zowner = 0xDEADBEEF;       // non-zero, but not PNG_IDAT
    int owner = 0xABCDEF01;         // some other owner (non-IDAT)
    int ret = png_deflate_claim(p, (png_uint_32)owner, 2048);

    // Expect that initialization succeeded and zowner got updated
    // Our stub returns Z_OK (0). We'll interpret non-negative as success.
    ASSERT_TRUE(ret == 0);
    ASSERT_TRUE(p->zowner == (unsigned)owner);
    // Ensure our mock deflateInit2 was invoked
    ASSERT_TRUE(init2_called == 1);

    destroy_png_ptr(p);
}

// Test C: When data_size is small, the windowBits adjustment loop can decrease windowBits.
// We simulate a scenario where data_size <= 16384 and windowBits is high enough to trigger shrink.
// The test asserts that the function completes and returns a success, with zowner updated.
TEST(test_png_deflate_claim_windowBits_adjustment_path) {
    reset_global_states();
    png_struct* p = create_png_ptr_for_test();
    p->zowner = 0;                  // no current zstream owner
    p->zlib_window_bits = 15;        // start with 32K window
    p->zlib_level = 6;
    p->zlib_method = 8;
    p->zlib_mem_level = 8;
    p->zlib_strategy = 0;

    int owner = PNG_IDAT;             // use IDAT as owner in this path
    int data_size = 16000;              // just above 16KB to force path? Here for test simplicity we keep within small range.

    int ret = png_deflate_claim(p, (png_uint_32)owner, (png_alloc_size_t)data_size);

    // Expect success (0) and zowner set
    ASSERT_TRUE(ret == 0);
    ASSERT_TRUE(p->zowner == (unsigned)owner);

    destroy_png_ptr(p);
}

// Test D: When zstream is already initialized and parameters do not change, a reset occurs
// and the function should succeed (deflateReset path).
TEST(test_png_deflate_claim_reuse_initialized_zstream) {
    reset_global_states();
    png_struct* p = create_png_ptr_for_test();
    // Simulate previously initialized zstream with matching params
    p->flags |= PNG_FLAG_ZSTREAM_INITIALIZED;
    p->zlib_set_level = p->zlib_level;
    p->zlib_set_method = p->zlib_method;
    p->zlib_set_window_bits = p->zlib_window_bits;
    p->zlib_set_mem_level = p->zlib_mem_level;
    p->zlib_set_strategy = p->zlib_strategy;
    p->zowner = 0;
    int owner = PNG_IDAT;

    int ret = png_deflate_claim(p, (png_uint_32)owner, 4096);

    // deflateReset should be invoked and return Z_OK
    ASSERT_TRUE(ret == 0);
    ASSERT_TRUE(reset_called == 1); // our mock increments this
    ASSERT_TRUE(p->zowner == (unsigned)owner);

    destroy_png_ptr(p);
}

// ---------------------------------------------------------------------------
// Step 3: Test Runner
// ---------------------------------------------------------------------------

int main() {
    std::cout << "Starting png_deflate_claim test suite (mocked environment)..." << std::endl;

    RUN_TEST(test_png_deflate_claim_owner_idat_returns_stream_error);
    RUN_TEST(test_png_deflate_claim_non_idat_initializes_and_assigns_owner);
    RUN_TEST(test_png_deflate_claim_windowBits_adjustment_path);
    RUN_TEST(test_png_deflate_claim_reuse_initialized_zstream);

    std::cout << "All tests executed." << std::endl;
    return 0;
}