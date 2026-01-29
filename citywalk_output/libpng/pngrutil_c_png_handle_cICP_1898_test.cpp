// Test suite for png_handle_cICP (pngrutil.c) using a lightweight, framework-free approach.
// This test uses manual mocks for libpng internals to exercise the focal method's logic
// without requiring the actual libpng library in the test environment.
// The tests are designed to be compiled with a C++11 compiler and linked against the
// production object file that contains png_handle_cICP.

// Notes:
// - Tests focus on the observable side effects of png_handle_cICP:
//   * When png_crc_finish returns 0, png_set_cICP should be invoked with the bytes read from png_crc_read.
//   * When png_crc_finish returns non-zero, png_set_cICP should NOT be invoked.
// - We implement minimal C linkage mocks for the required libpng functions so that
//   the focal function can call them during the test.
// - The test does not terminate on assertion failures; instead, it reports failures and exits
//   with a non-zero status code if any test fails.

#include <cstdint>
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>


// Forward declaration of the focal function from the library under test.
// We declare the signature with opaque pointer types for compatibility with the mocked environment.
extern "C" int png_handle_cICP(void* png_ptr, void* info_ptr, std::uint32_t length);

// Type aliases used by the focal function (simplified in test environment)
typedef void* png_structrp;
typedef void* png_inforp;
typedef std::uint32_t png_uint_32;
typedef unsigned char png_byte;
typedef unsigned char* png_bytep;

// Global state for mocks to verify behavior
static bool g_cICP_called = false;
static png_byte g_cICP_a = 0, g_cICP_b = 0, g_cICP_c = 0, g_cICP_d = 0;
static int g_finish_return = 0;
static int g_has_chunk_return = 0; // Not relied upon in tests, but provided for completeness
static int g_crc_read_invoked = 0;  // number of times png_crc_read was invoked (for sanity)


// Mocks: provide C-linkage to interpose libpng calls made by png_handle_cICP
extern "C" {

// libpng-like hooks used by png_handle_cICP
void png_debug(int /*level*/, const char* /*msg*/) {
  // No-op for tests
}

void png_crc_read(png_structrp /*png_ptr*/, png_bytep buf, png_uint_32 /*length*/) {
  // Supply deterministic 4-byte data regardless of length requested
  // This simulates: buf = [0x11, 0x22, 0x33, 0x44]
  buf[0] = 0x11;
  buf[1] = 0x22;
  buf[2] = 0x33;
  buf[3] = 0x44;
  g_crc_read_invoked++;
}

int png_crc_finish(png_structrp /*png_ptr*/, png_uint_32 /*skip*/) {
  return g_finish_return; // 0 indicates success in our test scenario
}

void png_set_cICP(png_structrp /*png_ptr*/, png_inforp /*info_ptr*/,
                  png_byte a, png_byte b, png_byte c, png_byte d) {
  // Record the values passed to the setter to verify correct propagation
  g_cICP_called = true;
  g_cICP_a = a;
  g_cICP_b = b;
  g_cICP_c = c;
  g_cICP_d = d;
}

int png_has_chunk(png_structrp /*png_ptr*/, int /*chunk*/) {
  return g_has_chunk_return;
}

} // extern "C"

// Helper: simple test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_PASS(...) do { printf("PASS: "); printf(__VA_ARGS__); printf("\n"); } while(0)
#define TEST_FAIL(...) do { printf("FAIL: "); printf(__VA_ARGS__); printf("\n"); g_failed_tests++; } while(0)
#define RUN_TEST(NAME) do { g_total_tests++; NAME(); } while(0)

// Test 1: Successful cICP handling path
void test_png_handle_cICP_success() {
  // Arrange
  g_cICP_called = false;
  g_cICP_a = g_cICP_b = g_cICP_c = g_cICP_d = 0;
  g_finish_return = 0;       // png_crc_finish will indicate success
  g_has_chunk_return = 0;      // simulate missing mDCV chunk (branch may be inert anyway)

  // Act
  int ret = png_handle_cICP(nullptr, nullptr, 4);

  // Assert
  if (!g_cICP_called) {
    TEST_FAIL("png_handle_cICP did not call png_set_cICP on success path.");
  } else if (g_cICP_a != 0x11 || g_cICP_b != 0x22 || g_cICP_c != 0x33 || g_cICP_d != 0x44) {
    TEST_FAIL("png_set_cICP received incorrect parameters: expected [11 22 33 44], got ["
              << std::hex << static_cast<int>(g_cICP_a) << " "
              << static_cast<int>(g_cICP_b) << " "
              << static_cast<int>(g_cICP_c) << " "
              << static_cast<int>(g_cICP_d) << "]" << std::dec);
  } else {
    TEST_PASS("png_handle_cICP success path invoked png_set_cICP with correct arguments.");
  }

  // We do not rely on the exact return value; the observable behavior is the setter call.
  // Additionally check that at least png_crc_read was invoked to fetch 4 bytes.
  // The exact number of times might depend on implementation; ensure it happened at least once.
  if (g_crc_read_invoked == 0) {
    TEST_FAIL("png_crc_read was not invoked to populate the 4-byte buffer.");
  }
}

// Test 2: Failure path where crc_finish signals an error
void test_png_handle_cICP_failure() {
  // Arrange
  g_cICP_called = false;
  g_cICP_a = g_cICP_b = g_cICP_c = g_cICP_d = 0;
  g_finish_return = 1;         // Non-zero indicates an error to png_crc_finish
  g_has_chunk_return = 0;

  // Act
  int ret = png_handle_cICP(nullptr, nullptr, 4);

  // Assert
  if (g_cICP_called) {
    TEST_FAIL("png_handle_cICP should not call png_set_cICP when finish indicates error.");
  } else {
    TEST_PASS("png_handle_cICP correctly avoided setting cICP on error from png_crc_finish.");
  }
  // We avoid asserting on the exact return value, focusing on side-effects to verify control flow.
}

// Driver
int main() {
  printf("Starting tests for png_handle_cICP (pngrutil.c)\n");

  RUN_TEST(test_png_handle_cICP_success);
  RUN_TEST(test_png_handle_cICP_failure);

  printf("Tests completed: %d total, %d failed\n", g_total_tests, g_failed_tests);
  return g_failed_tests != 0 ? 1 : 0;
}