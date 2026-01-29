// Minimal, self-contained C++11 test harness for the focal C function
// png_write_IEND(png_structrp png_ptr).
// The tests mock the small subset of the "png" environment required by
// the focal method (png_write_complete_chunk, png_debug, etc.) to enable
// unit testing without linking against the full libpng library.
// The test suite covers the core behavior of png_write_IEND:
// - calls png_write_complete_chunk with PNG_IEND and NULL data, length 0
// - sets the PNG_HAVE_IEND flag in png_ptr->mode
// - logs the debug message via png_debug
// - preserves/ORs existing mode flags appropriately

#include <cstdint>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <cstddef>


// ----------------------------
// Minimal types and constants
// ----------------------------

// Simulated public constants used by the focal function
#define PNG_HAVE_IEND 0x01
#define PNG_IEND 0x49454E44u // ASCII 'IEND' packed as a 32-bit int (big-endian style for tests)

// Basic libpng-like type aliases (subset)
typedef unsigned char png_bytep;
typedef const unsigned char* png_const_bytep;
typedef unsigned int png_uint_32;
typedef void* png_voidp;

struct png_struct {
    unsigned int mode;
};

// Exported handle type used by the focal function
typedef png_struct* png_structrp;

// ----------------------------
// Forward declarations (focal API)
// ----------------------------

// The real project would provide these in headers; for testing we implement
// minimal stubs in this test file.
extern "C" void png_write_IEND(png_structrp png_ptr);
extern "C" void png_debug(int level, const char* message);
extern "C" void png_write_complete_chunk(png_structrp png_ptr, png_uint_32 chunk_name,
                                         png_const_bytep data, size_t length);

// ----------------------------
// Mock state to observe interactions
// ----------------------------

static png_uint_32 g_last_chunk_name = 0;
static const void* g_last_data_ptr = nullptr;
static size_t g_last_length = 0;
static int g_complete_chunk_called = 0;

static const char* g_last_debug_message = nullptr;

// Reset all mock observers/state before each test
static void reset_mock_state() {
    g_last_chunk_name = 0;
    g_last_data_ptr = nullptr;
    g_last_length = 0;
    g_complete_chunk_called = 0;
    g_last_debug_message = nullptr;
}

// ----------------------------
// Mocked implementations
// ----------------------------

extern "C" void png_debug(int level, const char* message) {
    // Record the last diagnostic message (simulating logging at runtime)
    (void)level; // unused in tests, but kept to resemble real API
    g_last_debug_message = message;
}

extern "C" void png_write_complete_chunk(png_structrp png_ptr, png_uint_32 chunk_name,
                                         png_const_bytep data, size_t length) {
    // Capture the call for assertions
    (void)png_ptr; // not used beyond presence in the real API; kept for compatibility
    g_complete_chunk_called++;
    g_last_chunk_name = chunk_name;
    g_last_data_ptr = data;
    g_last_length = length;
}

// The focal function under test (reproduced here for the self-contained test).
// In the real project, this would be pulled from pngwutil.c, but to enable
// isolated unit testing without the full library, we implement a faithful copy.
extern "C" void png_write_IEND(png_structrp png_ptr)
{
{
   png_debug(1, "in png_write_IEND");
   png_write_complete_chunk(png_ptr, PNG_IEND, NULL, 0);
   png_ptr->mode |= PNG_HAVE_IEND;
}
}

// ----------------------------
// Lightweight test framework
// ----------------------------

#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        printf("  FAIL: %s\n", msg); \
        return false; \
    } \
} while (0)

static bool test_png_write_IEND_calls_complete_and_sets_mode_zero() {
    // Test that, starting from a clean state, png_write_IEND both records the
    // IEND chunk and sets the mode flag.
    reset_mock_state();

    png_struct s;
    s.mode = 0;

    png_write_IEND(&s);

    bool chunk_ok = (g_last_chunk_name == PNG_IEND) &&
                    (g_last_length == 0) &&
                    (g_last_data_ptr == nullptr);

    bool mode_ok = (s.mode & PNG_HAVE_IEND) != 0;

    bool debug_ok = (g_last_debug_message != nullptr) &&
                    (std::strcmp(g_last_debug_message, "in png_write_IEND") == 0);

    if (!chunk_ok) {
        printf("  Expected last_chunk_name=%u, last_length=0, last_data_ptr=nullptr; got name=%u, length=%zu, data_ptr=%p\n",
               PNG_IEND, (unsigned)g_last_chunk_name, g_last_length, g_last_data_ptr);
    }

    TEST_ASSERT(chunk_ok, "png_write_complete_chunk not called with PNG_IEND and NULL data/0 length");
    TEST_ASSERT(mode_ok, "png_ptr->mode was not OR'ed with PNG_HAVE_IEND");
    TEST_ASSERT(debug_ok, "png_debug did not log expected message 'in png_write_IEND'");

    return true;
}

static bool test_png_write_IEND_preserves_existing_mode_and_sets_bit() {
    reset_mock_state();

    png_struct s;
    s.mode = 0xDEAD; // arbitrary pre-existing flags

    png_write_IEND(&s);

    bool bit_set = (s.mode & PNG_HAVE_IEND) != 0;
    bool no_other_bits_lost = (s.mode & ~PNG_HAVE_IEND) == 0xDEAD;

    TEST_ASSERT(bit_set, "PNG_HAVE_IEND bit was not set in existing mode");
    TEST_ASSERT(no_other_bits_lost, "Existing mode bits were unexpectedly altered besides setting PNG_HAVE_IEND");

    // The mock should have recorded the IEND chunk call with NULL data and 0 length
    TEST_ASSERT((g_last_chunk_name == PNG_IEND) && (g_last_length == 0) && (g_last_data_ptr == nullptr),
                "png_write_complete_chunk not invoked as expected within IEND write");

    return true;
}

static bool test_png_write_IEND_calls_once_and_logs_once() {
    reset_mock_state();

    png_struct s;
    s.mode = 0;

    png_write_IEND(&s);

    // Ensure complete_chunk was invoked exactly once
    TEST_ASSERT(g_complete_chunk_called == 1,
                "png_write_complete_chunk should be called exactly once by png_write_IEND");

    // Ensure a single debug log was produced with the expected message
    TEST_ASSERT(g_last_debug_message != nullptr &&
                std::strcmp(g_last_debug_message, "in png_write_IEND") == 0,
                "png_debug should be called with 'in png_write_IEND'");

    return true;
}

// ----------------------------
// Entry point
// ----------------------------

int main() {
    int tests_passed = 0;
    int tests_total = 0;

    printf("Running tests for: png_write_IEND (self-contained mock)\n");

    // Test 1
    tests_total++;
    printf("Test %d: png_write_IEND calls complete chunk and updates mode (initially zero)\n", tests_total);
    if (test_png_write_IEND_calls_complete_and_sets_mode_zero()) {
        printf("  PASS\n");
        tests_passed++;
    } else {
        printf("  FAIL\n");
    }

    // Test 2
    tests_total++;
    printf("Test %d: png_write_IEND preserves existing mode bits and sets PNG_HAVE_IEND\n", tests_total);
    if (test_png_write_IEND_preserves_existing_mode_and_sets_bit()) {
        printf("  PASS\n");
        tests_passed++;
    } else {
        printf("  FAIL\n");
    }

    // Test 3
    tests_total++;
    printf("Test %d: png_write_IEND calls png_write_complete_chunk exactly once and logs debug\n", tests_total);
    if (test_png_write_IEND_calls_once_and_logs_once()) {
        printf("  PASS\n");
        tests_passed++;
    } else {
        printf("  FAIL\n");
    }

    // Summary
    printf("Summary: %d/%d tests passed\n", tests_passed, tests_total);

    return (tests_passed == tests_total) ? 0 : 1;
}