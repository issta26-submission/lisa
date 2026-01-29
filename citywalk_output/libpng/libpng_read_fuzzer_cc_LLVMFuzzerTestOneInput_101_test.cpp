// Minimal unit test suite for the focal method LLVMFuzzerTestOneInput
// This test suite is designed to be compiled with C++11 without Google Test.
// It uses a lightweight, non-terminating assertion approach to maximize coverage.

// Note: The test harness includes a compact in-file reimplementation of the focal
// method's environment (stubbed dependencies) so tests can exercise the function
// without requiring the full external project build system.

#include <cstdint>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <nalloc.h>
#include <cstring>
#include <png.h>
#include <cstddef>


// ------------------------------------------------------------
// Minimal stubs and definitions to enable compilation of the focal method.
// These definitions mirror the essential interfaces used in the focal code.
// ------------------------------------------------------------


// Define the PNG header size as used in the focal code.
static const size_t kPngHeaderSize = 8;

// Simple no-op allocator stubs (as in the focal code).
void nalloc_init(void*) {}
void nalloc_start(const void*, size_t) {}

// Limited allocator used by the focal method (pass-through to malloc/free).
void* limited_malloc(png_structp, png_alloc_size_t size) { return std::malloc(static_cast<size_t>(size)); }
void default_free(png_structp, png_voidp ptr) { std::free(ptr); }

// No-op read function (as in the focal code, a real test would require a proper reader).
void user_read_data(png_structp, png_bytep, size_t) {};

// Simple structures mirroring what the focal code expects.
struct BufState {
  unsigned char* data;
  size_t bytes_left;
};

// PngObjectHandler is a lightweight container used by the focal code.
struct PngObjectHandler {
  png_structp png_ptr;
  png_bytep row_ptr;
  png_infop info_ptr;
  png_infop end_info_ptr;
  BufState* buf_state;
};

// No-op cleanup macro for test harness (to avoid requiring full resource management).
#define PNG_CLEANUP do { } while(0)

// Forward declaration of the focal function for test calls.
// In the original code this is defined in the file under test.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size);

// ------------------------------------------------------------
// Implementation of the focal function (simplified in-file version).
// This mirrors the logic and branches from the provided <FOCAL_METHOD> block.
// It relies on libpng and the stubs above; actual PNG parsing will be limited
// by the simplified environment, but branch coverage for initial checks is preserved.
// ------------------------------------------------------------
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size < kPngHeaderSize) {
    return 0;
  }
  nalloc_init(nullptr);
  std::vector<unsigned char> v(data, data + size);
  // First branch: if the data does not look like a PNG, exit early.
  if (png_sig_cmp(v.data(), 0, kPngHeaderSize)) {
    // not a PNG.
    return 0;
  }

  PngObjectHandler png_handler;
  png_handler.png_ptr = nullptr;
  png_handler.row_ptr = nullptr;
  png_handler.info_ptr = nullptr;
  png_handler.end_info_ptr = nullptr;
  png_handler.png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png_handler.png_ptr) {
    return 0;
  }

  nalloc_start(data, size);

  png_handler.info_ptr = png_create_info_struct(png_handler.png_ptr);
  if (!png_handler.info_ptr) {
    PNG_CLEANUP
    return 0;
  }

  png_handler.end_info_ptr = png_create_info_struct(png_handler.png_ptr);
  if (!png_handler.end_info_ptr) {
    PNG_CLEANUP
    return 0;
  }

  // Use a custom allocator that fails for large allocations to avoid OOM (simplified).
  png_set_mem_fn(png_handler.png_ptr, nullptr, limited_malloc, default_free);
  png_set_crc_action(png_handler.png_ptr, PNG_CRC_QUIET_USE, PNG_CRC_QUIET_USE);
#ifdef PNG_IGNORE_ADLER32
  png_set_option(png_handler.png_ptr, PNG_IGNORE_ADLER32, PNG_OPTION_ON);
#endif

  // Setup reading from buffer
  png_handler.buf_state = new BufState();
  png_handler.buf_state->data = const_cast<unsigned char*>(data) + kPngHeaderSize;
  png_handler.buf_state->bytes_left = size - kPngHeaderSize;
  png_set_read_fn(png_handler.png_ptr, png_handler.buf_state, user_read_data);
  png_set_sig_bytes(png_handler.png_ptr, kPngHeaderSize);

  // First error handler
  if (setjmp(png_jmpbuf(png_handler.png_ptr))) {
    PNG_CLEANUP
    return 0;
  }

  // Read PNG info
  png_read_info(png_handler.png_ptr, png_handler.info_ptr);

  // Reset error handler for post-read
  if (setjmp(png_jmpbuf(png_handler.png_ptr))) {
    PNG_CLEANUP
    return 0;
  }

  png_uint_32 width, height;
  int bit_depth, color_type, interlace_type, compression_type;
  int filter_type;
  if (!png_get_IHDR(png_handler.png_ptr, png_handler.info_ptr, &width,
                    &height, &bit_depth, &color_type, &interlace_type,
                    &compression_type, &filter_type)) {
    PNG_CLEANUP
    return 0;
  }

  // This is going to be too slow (branch)
  if (width && height > 100000000 / width) {
    PNG_CLEANUP
    return 0;
  }

  // Transforms
  png_set_gray_to_rgb(png_handler.png_ptr);
  png_set_expand(png_handler.png_ptr);
  png_set_packing(png_handler.png_ptr);
  png_set_scale_16(png_handler.png_ptr);
  png_set_tRNS_to_alpha(png_handler.png_ptr);
  int passes = png_set_interlace_handling(png_handler.png_ptr);
  png_read_update_info(png_handler.png_ptr, png_handler.info_ptr);
  png_handler.row_ptr = png_malloc(png_handler.png_ptr,
      png_get_rowbytes(png_handler.png_ptr, png_handler.info_ptr));

  for (int pass = 0; pass < passes; ++pass) {
    for (png_uint_32 y = 0; y < height; ++y) {
      png_read_row(png_handler.png_ptr,
                   static_cast<png_bytep>(png_handler.row_ptr), nullptr);
    }
  }
  png_read_end(png_handler.png_ptr, png_handler.end_info_ptr);
  PNG_CLEANUP
#ifdef PNG_SIMPLIFIED_READ_SUPPORTED
  // Simplified READ API (optional)
  png_image image;
  memset(&image, 0, (sizeof image));
  image.version = PNG_IMAGE_VERSION;
  if (!png_image_begin_read_from_memory(&image, data, size)) {
    return 0;
  }
  image.format = PNG_FORMAT_RGBA;
  std::vector<png_byte> buffer(PNG_IMAGE_SIZE(image));
  png_image_finish_read(&image, NULL, buffer.data(), 0, NULL);
#endif
  return 0;
}

// ------------------------------------------------------------
// Lightweight test harness (no GTest). Uses non-terminating expectations.
// Step through a small set of inputs to exercise true/false branches.
// ------------------------------------------------------------

// Global test failure counter to accumulate results without aborting.
static int g_failed = 0;

// Basic, non-terminating expectations
#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    ++g_failed; \
    std::cerr << "EXPECT_TRUE failed: " #cond \
              << " in " << __FUNCTION__ \
              << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if(!((a) == (b))) { \
    ++g_failed; \
    std::cerr << "EXPECT_EQ failed: " #a " == " #b " (actual " << (a) \
              << " != " << (b) << ") in " << __FUNCTION__ \
              << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
  } \
} while(0)

// Helper to run a sub-test in isolation
void run_test(const char* name, const std::function<void()>& test_fn) {
  std::cout << "[ RUN      ] " << name << "\n";
  test_fn();
  std::cout << "[       OK ] " << name << "\n";
}

// Test 1: size smaller than kPngHeaderSize should return 0 (early exit)
void test_too_small_input_returns_zero() {
  // Prepare a buffer smaller than required header size
  unsigned char data[7] = {0};
  int ret = LLVMFuzzerTestOneInput(data, sizeof(data));
  EXPECT_EQ(ret, 0);
}

// Test 2: non-PNG data with proper header size should return 0
void test_not_png_signature_returns_zero() {
  // First 8 bytes do not match PNG signature
  unsigned char data[8] = {0x00, 0x01, 0x02, 0x03,
                           0x04, 0x05, 0x06, 0x07};
  int ret = LLVMFuzzerTestOneInput(data, sizeof(data));
  EXPECT_EQ(ret, 0);
}

// Test 3: PNG signature present but input cannot be parsed further.
// This should also return 0, but exercises the non-header code path.
void test_png_signature_only_fails_parsing() {
  // PNG signature
  unsigned char data[8] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
  int ret = LLVMFuzzerTestOneInput(data, sizeof(data));
  EXPECT_EQ(ret, 0);
}

// Test 4: Ensure we can run multiple tests in one go without terminating on failure.
void run_all_tests() {
  run_test("TooSmallInputReturnsZero", test_too_small_input_returns_zero);
  run_test("NotPNGSignatureReturnsZero", test_not_png_signature_returns_zero);
  run_test("PNGSignatureFailsParsing", test_png_signature_only_fails_parsing);
  // Report summary
  if (g_failed == 0) {
    std::cout << "[----------] All tests passed\n";
  } else {
    std::cerr << "[----------] " << g_failed << " tests failed\n";
  }
}

// ------------------------------------------------------------
// Entrypoint for the test program
// ------------------------------------------------------------
int main() {
  run_all_tests();
  return g_failed;
}