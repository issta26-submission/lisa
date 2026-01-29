// C++11 unit test suite for the focal method png_handle_tIME (as found in pngrutil.c)
// This test harness provides minimal, self-contained re-implementations of the
// required libpng dependencies to validate the core logic of png_handle_tIME
// without requiring the full libpng build. It uses a lightweight, non-GTest
// assertion framework suitable for environments where GoogleTest is not allowed.

#include <cstdint>
#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>


// Domain-specific notes (from problem description):
// - We focus on the core dependent components: PNG_HAVE_IDAT, PNG_AFTER_IDAT,
//   png_crc_read, png_crc_finish, png_get_uint_16, png_set_tIME, png_debug,
//   PNG_UNUSED.
// - We simulate with minimal stand-ins for types like png_struct, png_infor, png_time.
// - We cover true/false branches for conditions and ensure static behavior is handled.
// - We avoid private methods; expose only what is necessary for test execution.

// Lightweight test framework (non-terminating assertions)
static int g_test_failures = 0;

static void report_failure(const char* file, int line, const char* message) {
    std::fprintf(stderr, "TEST FAILURE: %s:%d: %s\n", file, line, message);
    ++g_test_failures;
}

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        report_failure(__FILE__, __LINE__, #cond); \
    } \
} while(0)

#define EXPECT_EQ_INT(a,b) do { \
    if((int)(a) != (int)(b)) { \
        char _buf[128]; std::snprintf(_buf, sizeof(_buf), "Expected %d, got %d", (int)(b), (int)(a)); \
        report_failure(__FILE__, __LINE__, _buf); \
    } \
} while(0)

#define EXPECT_EQ_UINT(a,b) do { \
    if((unsigned)(a) != (unsigned)(b)) { \
        char _buf[128]; std::snprintf(_buf, sizeof(_buf), "Expected %u, got %u", (unsigned)(b), (unsigned)(a)); \
        report_failure(__FILE__, __LINE__, _buf); \
    } \
} while(0)

#define EXPECT_TIME_EQ(actual, expected) do { \
    EXPECT_EQ_INT((actual).year, (expected).year); \
    EXPECT_EQ_INT((actual).month, (expected).month); \
    EXPECT_EQ_INT((actual).day, (expected).day); \
    EXPECT_EQ_INT((actual).hour, (expected).hour); \
    EXPECT_EQ_INT((actual).minute, (expected).minute); \
    EXPECT_EQ_INT((int)(actual).second, (int)(expected).second); \
} while(0)

#define RUN_TEST(test_func) do { \
    std::cout << "Running " << #test_func << "...\n"; \
    g_test_failures = 0; test_func(); \
    if(g_test_failures == 0) { std::cout << "[PASS] " << #test_func << "\n"; } \
    else { std::cout << "[FAIL] " << #test_func << " with " << g_test_failures << " failures\n"; } \
} while(0)

// Minimal stubs and data structures to support png_handle_tIME

// Type aliases and constants (from libpng style)
typedef unsigned char png_byte;
typedef unsigned int png_uint_32;
typedef const unsigned char* png_const_bytep;
typedef unsigned char* png_bytep;

#define PNG_HAVE_IDAT 0x01
#define PNG_AFTER_IDAT 0x02
#define PNG_UNUSED(x) (void)(x)

// Mock of a tiny png_time structure used by png_handle_tIME
struct png_time {
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
};

// Forward declarations of the function under test's dependencies
struct png_struct {
    unsigned int mode;
};

typedef struct png_struct png_struct;
typedef png_struct* png_structrp;
typedef void* png_inforp;

// Global test state to simulate libpng internal behavior
static unsigned char g_fake_crc_bytes[16];
static size_t g_crc_read_pos = 0;
static int g_png_crc_finish_result = 0;
static png_time g_last_mod_time;
static int g_time_set = 0;

// Helpers used by the focal function
static void png_debug(int level, const char* msg) {
    (void)level; (void)msg;
    // no-op for testing
}

static unsigned int png_get_uint_16(png_const_bytep buf) {
    // Big-endian interpretation as libpng uses network order
    return (static_cast<unsigned int>(buf[0]) << 8) | static_cast<unsigned int>(buf[1]);
}

static void png_set_tIME(png_structrp png_ptr, png_inforp /*info_ptr*/, const png_time* mod_time) {
    // Capture the mod_time into a global location for assertion
    (void)png_ptr; (void)/*info_ptr*/;
    g_last_mod_time = *mod_time;
    g_time_set = 1;
}

static void set_fake_crc_bytes(const unsigned char* src, size_t len) {
    if (len > sizeof(g_fake_crc_bytes)) len = sizeof(g_fake_crc_bytes);
    std::memcpy(g_fake_crc_bytes, src, len);
    g_crc_read_pos = 0;
}

static void reset_test_state() {
    g_crc_read_pos = 0;
    std::memset(g_fake_crc_bytes, 0, sizeof(g_fake_crc_bytes));
    g_png_crc_finish_result = 0;
    g_time_set = 0;
    std::memset(&g_last_mod_time, 0, sizeof(g_last_mod_time));
}

// Mock implementations of libpng-like interfaces used by the focal function

static void png_crc_read(png_structrp png_ptr, png_bytep buf, png_uint_32 length) {
    (void)png_ptr;
    // Copy from the fake CRC bytes into buf
    for (png_uint_32 i = 0; i < length; ++i) {
        buf[i] = g_fake_crc_bytes[g_crc_read_pos + i];
    }
    g_crc_read_pos += length;
}

static int png_crc_finish(png_structrp png_ptr, int /*skip*/) {
    (void)png_ptr;
    return g_png_crc_finish_result;
}

// The focal method under test (reproduced in this test harness)
static const int handled_ok = 1;
static const int handled_error = 0;

static int png_handle_tIME(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{
{
   png_byte buf[7];
   png_time mod_time;
   png_debug(1, "in png_handle_tIME");
   /* TODO: what is this doing here?  It should be happened in pngread.c and
    * pngpread.c, although it could be moved to png_handle_chunk below and
    * thereby avoid some code duplication.
    */
   if ((png_ptr->mode & PNG_HAVE_IDAT) != 0)
      png_ptr->mode |= PNG_AFTER_IDAT;
   png_crc_read(png_ptr, buf, 7);
   if (png_crc_finish(png_ptr, 0) != 0)
      return handled_error;
   mod_time.second = buf[6];
   mod_time.minute = buf[5];
   mod_time.hour = buf[4];
   mod_time.day = buf[3];
   mod_time.month = buf[2];
   mod_time.year = png_get_uint_16(buf);
   png_set_tIME(png_ptr, info_ptr, &mod_time);
   return handled_ok;
   PNG_UNUSED(length)
}
}

// Unit Tests

// Test 1: Successful parsing - verifies time extraction and mode flag manipulation
static void test_png_handle_tIME_success_sets_time_and_mode() {
    reset_test_state();

    // Prepare a test PNG struct with IDAT present
    png_struct test_ptr;
    test_ptr.mode = PNG_HAVE_IDAT;
    png_inforp test_info = nullptr;

    // Prepare 7-byte time data (big-endian year, then month, day, hour, minute, second)
    // year = 0x07D0 = 2000
    unsigned char time_buf[7] = {
        0x07, 0xD0, // year = 2000
        0x04,       // month = 4
        0x1A,       // day = 26
        0x0F,       // hour = 15
        0x2A,       // minute = 42
        0x3B        // second = 59
    };

    set_fake_crc_bytes(time_buf, 7);
    g_png_crc_finish_result = 0; // success

    int result = png_handle_tIME(&test_ptr, test_info, 7);

    // Results
    EXPECT_TRUE(result == handled_ok);
    EXPECT_TRUE(g_time_set != 0);

    png_time expected = {2000, 4, 26, 15, 42, 59};
    EXPECT_TIME_EQ(g_last_mod_time, expected);

    // Mode should have IDAT and AFTER_IDAT bits set
    EXPECT_TRUE((test_ptr.mode & PNG_HAVE_IDAT) != 0);
    EXPECT_TRUE((test_ptr.mode & PNG_AFTER_IDAT) != 0);
}

// Test 2: CRC finish non-zero should yield an error and not set time
static void test_png_handle_tIME_crc_failure_returns_error() {
    reset_test_state();

    // Prepare a test PNG struct with IDAT present
    png_struct test_ptr;
    test_ptr.mode = PNG_HAVE_IDAT;
    png_inforp test_info = nullptr;

    // Prepare 7-byte time data
    unsigned char time_buf[7] = {
        0x07, 0xD0, // year = 2000
        0x04,       // month = 4
        0x1A,       // day = 26
        0x0F,       // hour = 15
        0x2A,       // minute = 42
        0x3B        // second = 59
    };

    set_fake_crc_bytes(time_buf, 7);
    g_png_crc_finish_result = 1; // simulate CRC error

    int result = png_handle_tIME(&test_ptr, test_info, 7);

    // Results
    EXPECT_TRUE(result == handled_error);
    // Ensure time was not set due to error
    EXPECT_TRUE(g_time_set == 0);
}

// Test 3: When PNG_HAVE_IDAT is not set, AFTER_IDAT should not be toggled
static void test_png_handle_tIME_without_idat_no_after_idat() {
    reset_test_state();

    // Prepare a test PNG struct without IDAT
    png_struct test_ptr;
    test_ptr.mode = 0; // no PNG_HAVE_IDAT

    png_inforp test_info = nullptr;

    // Prepare 7-byte time data
    unsigned char time_buf[7] = {
        0x07, 0xD0, // year = 2000
        0x04,       // month = 4
        0x1A,       // day = 26
        0x0F,       // hour = 15
        0x2A,       // minute = 42
        0x3B        // second = 59
    };

    set_fake_crc_bytes(time_buf, 7);
    g_png_crc_finish_result = 0; // success

    int result = png_handle_tIME(&test_ptr, test_info, 7);

    // Results
    EXPECT_TRUE(result == handled_ok);
    EXPECT_TRUE(g_time_set != 0);

    png_time expected = {2000, 4, 26, 15, 42, 59};
    EXPECT_TIME_EQ(g_last_mod_time, expected);

    // BEFORE: mode had no IDAT; AFTER: ensure AFTER_IDAT not set
    EXPECT_TRUE((test_ptr.mode & PNG_AFTER_IDAT) == 0);
    // IDAT bit should still be absent
    EXPECT_TRUE((test_ptr.mode & PNG_HAVE_IDAT) == 0);
}

// Main entry: run all tests
int main() {
    std::cout << "Starting png_handle_tIME unit tests (C++11, no GTest)...\n";

    RUN_TEST(test_png_handle_tIME_success_sets_time_and_mode);
    RUN_TEST(test_png_handle_tIME_crc_failure_returns_error);
    RUN_TEST(test_png_handle_tIME_without_idat_no_after_idat);

    if (g_test_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Total failures: " << g_test_failures << "\n";
        return 1;
    }
}