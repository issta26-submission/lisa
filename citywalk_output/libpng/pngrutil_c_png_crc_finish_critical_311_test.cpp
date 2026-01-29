// Minimal C++11 unit test suite for the focal C function png_crc_finish_critical
// This test harness provides a lightweight, GTest-free environment with
// non-terminating assertions to maximize code coverage for the function under test.
// It uses a small, self-contained mock of the png internals required by the focal method.

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain: Provide minimal C-like environment for the focal function.
// We implement only the dependencies the function uses.

extern "C" {

// Typedefs to mimic png types in a compact form
typedef unsigned int png_uint_32;
typedef unsigned char png_byte;

// Forward declaration of the focal function
struct png_struct_tag;
typedef struct png_struct_tag png_struct;
typedef png_struct* png_structrp;
typedef png_structrp png_structrp_t; // alias for readability

// Lightweight, self-contained definitions to support the focal function.
// Configurable constants and flags (bitfields mimic libpng behavior)
#define PNG_INFLATE_BUF_SIZE 256
#define PNG_FLAG_CRC_CRITICAL_IGNORE 0x01
#define PNG_FLAG_CRC_ANCILLARY_NOWARN 0x02
#define PNG_FLAG_CRC_CRITICAL_USE 0x04

// Ancillary chunk naming stub (used by the test to toggle ancillary behavior)
#define ANCILLARY_CHUNK_NAME 0x1
// Macro to pretend "chunk is ancillary" for a given name.
// The real libpng macro checks chunk type; we provide a minimal, controllable stub.
#define PNG_CHUNK_ANCILLARY(name) ((name) == ANCILLARY_CHUNK_NAME ? 1 : 0)

// Minimal internal png_struct with only fields used by the focal function
struct png_struct_tag {
    unsigned int flags;       // bitmask of PNG_FLAG_* flags
    png_uint_32 chunk_name;     // name/id of current chunk
};

// Internal counters and state used by the test harness to observe behavior
static int g_crc_read_calls = 0;
static int g_crc_error_result = 0;
static int g_warning_calls = 0;
static int g_error_calls = 0;
static std::string g_last_warning_msg;
static std::string g_last_error_msg;

// Reset all monitoring state between tests
static void reset_monitors() {
    g_crc_read_calls = 0;
    g_crc_error_result = 0;
    g_warning_calls = 0;
    g_error_calls = 0;
    g_last_warning_msg.clear();
    g_last_error_msg.clear();
}

// Mock implementations of png_* helpers used by the focal function

// Simulate reading a CRC-protected buffer from the stream.
// We only count how many times this would be invoked to verify loop behavior.
void png_crc_read(png_structrp png_ptr, png_byte* buf, png_uint_32 length) {
    (void)png_ptr;
    (void)buf;
    (void)length;
    ++g_crc_read_calls;
}

// Simulate CRC check. Returns non-zero to indicate error when desired by tests.
int png_crc_error(png_structrp png_ptr, int handle_as_ancillary) {
    (void)handle_as_ancillary;
    // Return the pre-configured test value to trigger CRC error handling paths
    return g_crc_error_result;
}

// Simulate a warning emission when CRC errors occur
void png_chunk_warning(png_structrp png_ptr, const char* msg) {
    (void)png_ptr;
    (void)msg;
    ++g_warning_calls;
    g_last_warning_msg = "CRC error"; // consistent with the focal code
}

// Simulate an error emission when CRC errors occur
void png_chunk_error(png_structrp png_ptr, const char* msg) {
    (void)png_ptr;
    (void)msg;
    ++g_error_calls;
    g_last_error_msg = "CRC error"; // consistent with the focal code
}

// The focal function under test (reproduced in this test file for isolation)
int png_crc_finish_critical(png_structrp png_ptr, png_uint_32 skip,
      int handle_as_ancillary)
{
{
   /* The size of the local buffer for inflate is a good guess as to a
    * reasonable size to use for buffering reads from the application.
    */
   while (skip > 0)
   {
      png_uint_32 len;
      png_byte tmpbuf[PNG_INFLATE_BUF_SIZE];
      len = (sizeof tmpbuf);
      if (len > skip)
         len = skip;
      skip -= len;
      png_crc_read(png_ptr, tmpbuf, len);
   }
   /* If 'handle_as_ancillary' has been requested and this is a critical chunk
    * but PNG_FLAG_CRC_CRITICAL_IGNORE was set then png_read_crc did not, in
    * fact, calculate the CRC so the ANCILLARY settings should not be used
    * instead.
    */
   if (handle_as_ancillary &&
       (png_ptr->flags & PNG_FLAG_CRC_CRITICAL_IGNORE) != 0)
      handle_as_ancillary = 0;
   /* TODO: this might be more comprehensible if png_crc_error was inlined here.
    */
   if (png_crc_error(png_ptr, handle_as_ancillary) != 0)
   {
      /* See above for the explanation of how the flags work. */
      if (handle_as_ancillary || PNG_CHUNK_ANCILLARY(png_ptr->chunk_name) != 0 ?
          (png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_NOWARN) == 0 :
          (png_ptr->flags & PNG_FLAG_CRC_CRITICAL_USE) != 0)
         png_chunk_warning(png_ptr, "CRC error");
      else
         png_chunk_error(png_ptr, "CRC error");
      return 1;
   }
   return 0;
}
}

// End of focal function

} // extern "C"

// Lightweight test framework (non-terminating, to maximize coverage)
static void expect_true(bool cond, const char* test_desc) {
    if (!cond) {
        std::cerr << "[FAIL] " << test_desc << "\n";
    } else {
        // For successful expectations, no output to avoid noise
    }
}

static void expect_int_eq(int a, int b, const char* test_desc) {
    if (a != b) {
        std::cerr << "[FAIL] " << test_desc << " (Expected " << b << ", got " << a << ")\n";
    }
}

// Test 1: No skip and no CRC error -> function should return 0 and not call png_crc_read
void test_no_skip_no_error() {
    reset_monitors();
    png_struct png_ptr;
    png_ptr.flags = 0;
    png_ptr.chunk_name = 0;

    // No CRC error
    g_crc_error_result = 0;

    int ret = png_crc_finish_critical(&png_ptr, 0, 0);

    expect_true(ret == 0, "test_no_skip_no_error: return value should be 0 when no error");
    expect_int_eq(g_crc_read_calls, 0, "test_no_skip_no_error: no reads should occur when skip == 0");
}

// Test 2: Non-zero skip should trigger multiple png_crc_read calls and still no error
void test_skip_multiple_calls() {
    reset_monitors();
    png_struct png_ptr;
    png_ptr.flags = 0;
    png_ptr.chunk_name = 0;

    g_crc_error_result = 0;
    const unsigned int skip_value = 700; // > 2 * PNG_INFLATE_BUF_SIZE to ensure multiple iterations

    int ret = png_crc_finish_critical(&png_ptr, skip_value, 0);

    int expected_calls = (skip_value + (PNG_INFLATE_BUF_SIZE - 1)) / PNG_INFLATE_BUF_SIZE;
    expect_true(ret == 0, "test_skip_multiple_calls: return 0 when no CRC error");
    expect_int_eq(g_crc_read_calls, expected_calls, "test_skip_multiple_calls: number of reads should equal ceil(skip/BUF)");
}

// Test 3: CRC error occurs with handle_as_ancillary = 1 -> should trigger a warning
void test_crc_error_warn_with_handle_ancillary() {
    reset_monitors();
    png_struct png_ptr;
    png_ptr.flags = 0; // no NOWARN, etc.
    png_ptr.chunk_name = ANCILLARY_CHUNK_NAME; // ancillary chunk

    g_crc_error_result = 1;
    int ret = png_crc_finish_critical(&png_ptr, 123, 1); // non-zero skip; any value OK

    expect_true(ret == 1, "test_crc_error_warn_with_handle_ancillary: should return 1 on CRC error");
    expect_true(g_warning_calls == 1, "test_crc_error_warn_with_handle_ancillary: should trigger a warning");
}

// Test 4: CRC error occurs for non-ancillary chunk with no NOWARN and no CRITICAL_USE -> should call error
void test_crc_error_error_not_ancillary_no_flags() {
    reset_monitors();
    png_struct png_ptr;
    png_ptr.flags = 0; // no flags
    png_ptr.chunk_name = 0xDEADBEEF; // non-ancillary

    g_crc_error_result = 1;
    int ret = png_crc_finish_critical(&png_ptr, 128, 0);

    expect_true(ret == 1, "test_crc_error_error_not_ancillary_no_flags: CRC error should cause return 1");
    expect_true(g_error_calls == 1, "test_crc_error_error_not_ancillary_no_flags: should emit error when not ancillary and no flags");
}

// Test 5: CRC error occurs for non-ancillary chunk with CRITICAL_USE flag set -> should warn
void test_crc_error_warn_with_critical_use() {
    reset_monitors();
    png_struct png_ptr;
    png_ptr.flags = PNG_FLAG_CRC_CRITICAL_USE; // enable critical use to trigger warning path
    png_ptr.chunk_name = 0xCAFEBABE; // non-ancillary

    g_crc_error_result = 1;
    int ret = png_crc_finish_critical(&png_ptr, 64, 0);

    expect_true(ret == 1, "test_crc_error_warn_with_critical_use: CRC error should cause return 1");
    expect_true(g_warning_calls == 1, "test_crc_error_warn_with_critical_use: should emit warning when not ancillary and CRITICAL_USE set");
}

int main() {
    std::cout << "Running unit tests for png_crc_finish_critical (mocked environment)\n";

    test_no_skip_no_error();
    test_skip_multiple_calls();
    test_crc_error_warn_with_handle_ancillary();
    test_crc_error_error_not_ancillary_no_flags();
    test_crc_error_warn_with_critical_use();

    std::cout << "Tests completed.\n";
    return 0;
}