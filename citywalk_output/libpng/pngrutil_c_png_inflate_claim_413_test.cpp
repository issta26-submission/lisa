// Test suite for the focal method png_inflate_claim (C libpng-like environment)
// This test is written in C++11, without Google Test, and uses a lightweight
// testing harness with non-terminating assertions to maximize coverage.
// The tests rely on a mocked minimal environment for the png structures and
// zlib interaction to drive and observe internal behavior of png_inflate_claim.
//
// Important: These tests assume the real function png_inflate_claim is linkable
// from the project/library under test. The accompanying stubs in this file
// shadow the zlib/libpng related calls to keep tests self-contained in this
// translation unit.

#include <cassert>
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>


// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            fprintf(stderr, "EXPECT_TRUE failed: %s:%d\n", __FILE__, __LINE__); \
            ++g_failed_tests; \
        } \
    } while (0)

#define EXPECT_EQ(a, b) \
    do { \
        ++g_total_tests; \
        if (!((a) == (b))) { \
            fprintf(stderr, "EXPECT_EQ failed: %s:%d (expected %ld, actual %ld)\n", __FILE__, __LINE__, static_cast<long>(a), static_cast<long>(b)); \
            ++g_failed_tests; \
        } \
    } while (0)

#define EXPECT_NEQ(a, b) \
    do { \
        ++g_total_tests; \
        if ((a) == (b)) { \
            fprintf(stderr, "EXPECT_NEQ failed: %s:%d (value %ld should differ)\n", __FILE__, __LINE__, static_cast<long>(a)); \
            ++g_failed_tests; \
        } \
    } while (0)


// ---------------------------------------------------------------------------
// Minimal mocked environment to support png_inflate_claim
// We provide simplified stubs for the types and functions referenced by the
// focal function, so tests can compile and link against the real png_inflate_claim.
// The stubs do not emulate full PNG decoding; they merely enable control and
// observation of the function's control flow and side effects.

// A minimal zlib-like stream structure used by the libpng code
typedef struct z_stream_s {
    void* next_in;
    unsigned int avail_in;
    void* next_out;
    unsigned int avail_out;
} z_stream;

// zlib's z_stream pointer alias (as used by libpng)
typedef z_stream* z_streamp;

// Basic png-like structure used by the focal function
typedef struct png_struct_def {
    unsigned int zowner;
    int flags;
    unsigned int options;
    int zstream_start;
    z_stream zstream;
} png_struct_def, *png_structrp, *png_structrt;
typedef const png_struct_def* png_const_structrp;

// Public constants used by the focal code (mirroring typical libpng definitions)
#define PNG_FLAG_ZSTREAM_INITIALIZED 0x01
#define PNG_OPTION_ON 1
#define PNG_MAXIMUM_INFLATE_WINDOW 0  // for test path

// Global flags to observe unit-test behavior of the stubs
static bool g_chunk_warning_called = false;
static bool g_chunk_error_called = false;

// A tiny safe-concatenate function used by the focal path (via png_safecat)
extern "C" void png_safecat(char* dest, size_t destSize, int add, const char* suffix)
{
    if (dest == nullptr || suffix == nullptr || destSize == 0)
        return;
    size_t cur = std::strlen(dest);
    size_t maxAppend = (destSize > cur) ? destSize - cur - 1 : 0;
    if (maxAppend > 0)
    {
        std::strncat(dest, suffix, maxAppend);
    }
}

// Mimic PNG_STRING_FROM_CHUNK macro usage by providing a minimal stand-in
// The actual content isn't important for the unit test as we only check side effects.
extern "C" void PNG_STRING_FROM_CHUNK(char* dest, unsigned int zowner)
{
    (void)dest; (void)zowner;
    // No-op for the test harness; the real macro would fill a 4-char chunk name.
}

// Stubbed zlib inflate API (shadow the real zlib calls for deterministic tests)
extern "C" int inflateInit2(z_streamp strm, int window_bits) { (void)strm; (void)window_bits; return 0; }
extern "C" int inflateReset2(z_streamp strm, int window_bits) { (void)strm; (void)window_bits; return 0; }
extern "C" int inflateInit(z_streamp strm) { (void)strm; return 0; }
extern "C" int inflateReset(z_streamp strm) { (void)strm; return 0; }
extern "C" int inflateValidate(z_streamp strm, int dummy) { (void)strm; (void)dummy; return 0; }

// channeled error/warning handlers expected by the focal function
extern "C" void png_chunk_warning(png_structrp png_ptr, const char* msg)
{
    (void)png_ptr;
    (void)msg;
    g_chunk_warning_called = true;
}
extern "C" void png_chunk_error(png_structrp png_ptr, const char* msg)
{
    (void)png_ptr;
    (void)msg;
    g_chunk_error_called = true;
}
extern "C" void png_zstream_error(png_structrp png_ptr, int ret)
{
    (void)png_ptr;
    (void)ret;
    // No-op for test; we only observe side effects from other branches.
}

// The function under test is assumed to be linked from the library under test
extern "C" int png_inflate_claim(png_structrp png_ptr, unsigned int owner);

// ---------------------------------------------------------------------------
// Test case 1: zowner is non-zero -> ensure the function handles the zowner check
// and that the post-condition aligns with the expected behavior (either
// zowner becomes 0 in release builds or remains unchanged otherwise).
// We verify that exactly one of the two branches has been triggered via side effects.
bool test_inflate_claim_zowner_nonzero_branch()
{
    // Reset test environment
    g_chunk_warning_called = false;
    g_chunk_error_called = false;

    // Prepare a png_struct with non-zero zowner
    png_struct_def s;
    std::memset(&s, 0, sizeof(s));
    s.zowner = 0xabcdef01u;  // non-zero initial owner
    s.flags = 0;
    s.options = 0;            // ensure the 'window_bits' selection is deterministic (path depends on compile-time macro)
    s.zstream_start = -1;

    // Call the focal function
    int ret = png_inflate_claim(&s, 0x1234u);

    // Observations:
    // - If built as a release build, s.zowner should be reset to 0 and a warning should be issued.
    // - If not, s.zowner should be left as the original owner (or equal to the input owner).
    bool zowner_is_zero = (s.zowner == 0u);
    bool zowner_unchanged = (s.zowner == 0xabcdef01u);
    bool branch_triggered = g_chunk_warning_called || g_chunk_error_called;

    // We expect one of the two branches to have run.
    EXPECT_TRUE(branch_triggered);
    // We expect either the zowner was cleared or remained unchanged, but not an arbitrary value
    EXPECT_TRUE(zowner_is_zero || zowner_unchanged);

    // Return success only if both expectations hold
    return branch_triggered && (zowner_is_zero || zowner_unchanged);
}

// ---------------------------------------------------------------------------
// Test case 2: zowner is zero -> force the zlib inflate initialization path and
// verify that upon successful initialization, the owner is recorded (zowner = owner)
// and the zstream_initialized flag is set.
bool test_inflate_claim_zowner_zero_path()
{
    // Reset state
    g_chunk_warning_called = false;
    g_chunk_error_called = false;

    png_struct_def s;
    std::memset(&s, 0, sizeof(s));
    s.zowner = 0u;        // zero to bypass the zowner-warning branch
    s.flags = 0;
    s.options = 0;         // default options, enabling inflateInit path
    s.zstream_start = -1;

    unsigned int owner = 0xDEADBEEFu;

    int ret = png_inflate_claim(&s, owner);

    // Observations:
    // - Expect that inflate initialization was performed and succeeded (our stub returns 0)
    // - Expect that zowner is set to the passed owner on success
    bool zowner_set = (s.zowner == owner);
    bool zstream_initialized = (s.flags & PNG_FLAG_ZSTREAM_INITIALIZED) != 0;

    EXPECT_TRUE(ret >= 0); // basic sanity check
    EXPECT_TRUE(zowner_set);
    EXPECT_TRUE(zstream_initialized);

    return zowner_set && zstream_initialized;
}

// ---------------------------------------------------------------------------
// Main: run all tests and report results.
// Per instructions, tests are invoked from main (no gtest).
int main()
{
    bool t1 = test_inflate_claim_zowner_nonzero_branch();
    bool t2 = test_inflate_claim_zowner_zero_path();

    if (!t1) {
        fprintf(stderr, "Test 1 (zowner non-zero) failed.\n");
    }
    if (!t2) {
        fprintf(stderr, "Test 2 (zowner zero path) failed.\n");
    }

    int total = g_total_tests;
    int failed = g_failed_tests;
    printf("Test results: %d total, %d failed.\n", total, failed);
    return (failed == 0) ? 0 : 1;
}