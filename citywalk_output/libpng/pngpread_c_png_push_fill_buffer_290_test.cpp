// Lightweight C++11 unit test suite for the focal function png_push_fill_buffer
// This test suite is self-contained and does not rely on external testing frameworks.
// It uses non-terminating assertions and reports a summary at the end.

#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Minimal re-implementation of the PNG-related types to support testing.
// We mirror only the fields used by the focal method in the provided code.

typedef unsigned char* png_bytep;

// Forward declaration of the struct to mimic the library's typedefs
struct png_struct;
typedef struct png_struct png_struct;
typedef png_struct* png_structp;

// Definition of the minimal png_struct with fields needed by png_push_fill_buffer
struct png_struct {
    size_t save_buffer_size;
    png_bytep save_buffer_ptr;
    size_t buffer_size;
    size_t current_buffer_size;
    png_bytep current_buffer_ptr;
};

// FOCAL_METHOD replicated for the test environment.
// It mirrors the logic exactly as provided in the prompt.
static png_bytep png_push_fill_buffer(png_structp png_ptr, png_bytep buffer, size_t length)
{
{
   png_bytep ptr;
   if (png_ptr == NULL)
      return;
   ptr = buffer;
   if (png_ptr->save_buffer_size != 0)
   {
      size_t save_size;
      if (length < png_ptr->save_buffer_size)
         save_size = length;
      else
         save_size = png_ptr->save_buffer_size;
      memcpy(ptr, png_ptr->save_buffer_ptr, save_size);
      length -= save_size;
      ptr += save_size;
      png_ptr->buffer_size -= save_size;
      png_ptr->save_buffer_size -= save_size;
      png_ptr->save_buffer_ptr += save_size;
   }
   if (length != 0 && png_ptr->current_buffer_size != 0)
   {
      size_t save_size;
      if (length < png_ptr->current_buffer_size)
         save_size = length;
      else
         save_size = png_ptr->current_buffer_size;
      memcpy(ptr, png_ptr->current_buffer_ptr, save_size);
      png_ptr->buffer_size -= save_size;
      png_ptr->current_buffer_size -= save_size;
      png_ptr->current_buffer_ptr += save_size;
   }
}
return buffer;
}

// Lightweight test harness (non-terminating assertions)

static int g_failures = 0;
static std::vector<std::string> g_errors;

// Record a non-fatal failure message
static void record_error(const std::string& msg) {
    g_errors.push_back(msg);
    ++g_failures;
}

// General equality assertion with descriptive message
template<typename A, typename B>
static void assert_eq(A a, B b, const std::string& msg) {
    if (!(a == b)) {
        std::ostringstream oss;
        oss << msg << " (expected " << b << ", got " << a << ")";
        record_error(oss.str());
    }
}

// Compare two memory buffers for exact content
static void assert_mem_eq(const unsigned char* a, const unsigned char* b, size_t n, const std::string& msg) {
    if (std::memcmp(a, b, n) != 0) {
        std::ostringstream oss;
        oss << msg << " - memory differs in first " << n << " bytes";
        record_error(oss.str());
    }
}

// Test 1: Copy only from save buffer when length <= save_buffer_size
// Expected: destination contains the first 'length' bytes from save_buffer_ptr.
// save_buffer_size decreases by length; save_buffer_ptr advances; buffer_size decreases.
static void test_copy_from_save_only_short_length() {
    // Prepare png_ptr with only save buffer populated
    unsigned char save_buf[] = { 10, 20, 30, 40, 50 };
    unsigned char dummy_current[] = {}; // empty

    png_struct png;
    png.save_buffer_size = 5;
    png.save_buffer_ptr = save_buf;
    png.buffer_size = 8;
    png.current_buffer_size = 0;
    png.current_buffer_ptr = nullptr;

    unsigned char dest[3] = { 0, 0, 0 };
    png_bytep buffer = dest;
    size_t length = 3;

    png_push_fill_buffer(&png, buffer, length);

    unsigned char expected[] = { 10, 20, 30 };
    assert_mem_eq(dest, expected, 3, "Test1: dest should contain first 3 bytes from save_buffer_ptr");
    // Check updated internal state
    assert_eq(png.save_buffer_size, size_t(2), "Test1: save_buffer_size should decrease by 3");
    assert_eq(png.buffer_size, size_t(5), "Test1: buffer_size should decrease by 3");
    // save_buffer_ptr advanced by 3 bytes from save_buf[0]
    size_t offset = (size_t)(png.save_buffer_ptr - save_buf);
    assert_eq(offset, size_t(3), "Test1: save_buffer_ptr should advance by 3");
    // current buffer remains untouched
    assert_eq(png.current_buffer_size, size_t(0), "Test1: current_buffer_size should remain 0");
    (void)dummy_current; // unused in this test, silence unused warning
}

// Test 2: Copy from save buffer first, then from current buffer when length exceeds save_buffer_size
// Expected: first 3 bytes from save, next 3 bytes from current_buffer_ptr
static void test_copy_from_both_buffers() {
    unsigned char save_buf[] = { 5, 6, 7 };
    unsigned char cur_buf[]  = { 1, 2, 3, 4 };

    png_struct png;
    png.save_buffer_size = 3;
    png.save_buffer_ptr = save_buf;
    png.buffer_size = 100;
    png.current_buffer_size = 4;
    png.current_buffer_ptr = cur_buf;

    unsigned char dest[6] = { 0, 0, 0, 0, 0, 0 };
    png_bytep buffer = dest;
    size_t length = 6;

    png_push_fill_buffer(&png, buffer, length);

    unsigned char expected[] = { 5, 6, 7, 1, 2, 3 };
    assert_mem_eq(dest, expected, 6, "Test2: dest should contain 3 save bytes followed by 3 current buffer bytes");
    // Validate internal state transitions
    assert_eq(png.save_buffer_size, size_t(0), "Test2: save_buffer_size should be 0 after consuming save buffer");
    assert_eq(png.current_buffer_size, size_t(1), "Test2: current_buffer_size should decrease by 3");
    size_t save_ptr_off = (size_t)(png.save_buffer_ptr - save_buf);
    assert_eq(save_ptr_off, size_t(3), "Test2: save_buffer_ptr advanced by 3");
    size_t cur_ptr_off = (size_t)(png.current_buffer_ptr - cur_buf);
    assert_eq(cur_ptr_off, size_t(3), "Test2: current_buffer_ptr advanced by 3");
    (void)png.buffer_size; // not asserting exact value here
}

// Test 3: Copy only from current buffer when save_buffer_size is 0
static void test_copy_from_current_only() {
    unsigned char cur_buf[] = { 9, 8, 7, 6 };
    png_struct png;
    png.save_buffer_size = 0;
    png.save_buffer_ptr = nullptr;
    png.buffer_size = 50;
    png.current_buffer_size = 4;
    png.current_buffer_ptr = cur_buf;

    unsigned char dest[2] = { 0, 0 };
    png_bytep buffer = dest;
    size_t length = 2;

    png_push_fill_buffer(&png, buffer, length);

    unsigned char expected[] = { 9, 8 };
    assert_mem_eq(dest, expected, 2, "Test3: dest should contain first 2 bytes from current_buffer_ptr");
    assert_eq(png.current_buffer_size, size_t(2), "Test3: current_buffer_size should decrease by 2");
    size_t cur_ptr_off = (size_t)(png.current_buffer_ptr - cur_buf);
    assert_eq(cur_ptr_off, size_t(2), "Test3: current_buffer_ptr advanced by 2");
}

// Test 4: Copy when total available bytes are less than requested length
static void test_exceed_total_buffers() {
    unsigned char save_buf[] = { 7 };
    unsigned char cur_buf[]  = { 8, 9 };

    png_struct png;
    png.save_buffer_size = 1;
    png.save_buffer_ptr = save_buf;
    png.buffer_size = 100;
    png.current_buffer_size = 2;
    png.current_buffer_ptr = cur_buf;

    unsigned char dest[3] = { 0, 0, 0 };
    png_bytep buffer = dest;
    size_t length = 10;

    png_push_fill_buffer(&png, buffer, length);

    unsigned char expected[] = { 7, 8, 9 };
    assert_mem_eq(dest, expected, 3, "Test4: dest should contain available bytes from both buffers in order");
    // After copy, buffers should be exhausted
    assert_eq(png.save_buffer_size, size_t(0), "Test4: save_buffer_size should be 0 after consuming all save bytes");
    assert_eq(png.current_buffer_size, size_t(0), "Test4: current_buffer_size should be 0 after consuming all current bytes");
    size_t cur_ptr_off = (size_t)(png.current_buffer_ptr - cur_buf);
    assert_eq(cur_ptr_off, size_t(2), "Test4: current_buffer_ptr advanced by 2 (end of current buffer)");
}

// Test 5: NULL pointer handling (no crash, but no observable state to assert)
// This verifies that calling with NULL png_ptr is a no-op despite being a void function.
static void test_null_ptr_handling() {
    unsigned char dest[4] = { 1, 2, 3, 4 };
    png_bytep buffer = dest;
    size_t length = 4;

    // This should simply return without dereferencing a NULL pointer
    png_push_fill_buffer(nullptr, buffer, length);
    // No state to verify; this test ensures no crash occurs for NULL input.
}

// Helper to run all tests
static void run_all_tests() {
    test_copy_from_save_only_short_length();
    test_copy_from_both_buffers();
    test_copy_from_current_only();
    test_exceed_total_buffers();
    test_null_ptr_handling();
}

// Entry point
int main() {
    run_all_tests();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "TESTS FAILED: " << g_failures << " failure(s)" << std::endl;
        for (const auto& err : g_errors) {
            std::cout << "  - " << err << std::endl;
        }
        return g_failures;
    }
}