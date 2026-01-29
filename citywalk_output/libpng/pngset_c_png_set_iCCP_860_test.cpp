// C++11 test suite for png_set_iCCP (standalone mock environment, no GTest)
// This file provides a minimal "libpng-like" environment to exercise the focal method.
// It implements a lightweight test harness and multiple test scenarios described below.

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain-specific scaffolding: minimal stand-ins for libpng-like types and helpers

// Public-ish constants (subset used by focal method)
#define PNG_COMPRESSION_TYPE_BASE 0
#define PNG_FREE_ICCP 0x01
#define PNG_INFO_iCCP 0x04  // bit flag for iCCP presence

// Lightweight type definitions to mimic the focal code's usage
typedef struct png_struct_def* png_structrp;
typedef const struct png_struct_def* png_const_structrp;

typedef struct png_info_struct* png_inforp;

typedef char* png_charp;
typedef unsigned char* png_bytep;
typedef const char* png_const_charp;
typedef const unsigned char* png_const_bytep;
typedef uint32_t png_uint_32;
typedef unsigned int png_uint_32_alias; // for internal clarity

// Fake internal "PNG" object state (simplified)
struct png_struct_def {
  // empty; in tests we only need the pointer presence
};

// Info struct with fields used by png_set_iCCP
struct png_info_struct {
  unsigned int iccp_proflen;
  char* iccp_name;
  unsigned char* iccp_profile;
  unsigned int free_me;
  unsigned int valid;
};

// Mockable global state to observe side effects and control flow
static bool test_app_error_called = false;
static const char* test_app_error_last_msg = nullptr;

static bool test_benign_error_called = false;
static const char* test_benign_last_msg = nullptr;

// Allocation control for testing failure branches
static bool test_force_first_alloc_fail = false;
static bool test_force_second_alloc_fail = false;
static int test_allocation_call_counter = 0; // counts calls to png_malloc_warn

static void reset_all_tests_state() {
  test_app_error_called = false;
  test_app_error_last_msg = nullptr;
  test_benign_error_called = false;
  test_benign_last_msg = nullptr;
  test_force_first_alloc_fail = false;
  test_force_second_alloc_fail = false;
  test_allocation_call_counter = 0;
}

// Helper: lightweight "libpng" style functions (mock implementations)

static void png_debug1(int /*level*/, const char* /*fmt*/, const char* /*arg*/) {
  // no-op for testing
}

static void png_app_error(png_const_structrp /*png_ptr*/, const char* /*msg*/) {
  test_app_error_called = true;
  // In a real libpng, a message would be logged; here we simply record that an error occurred
}

static void png_benign_error(png_const_structrp /*png_ptr*/, const char* /*msg*/) {
  test_benign_error_called = true;
  // Just mark that a non-fatal error happened
}

static void* png_malloc_warn(png_const_structrp /*png_ptr*/, size_t size) {
  // Simulate deterministic allocation failures for testing branches
  if (test_force_first_alloc_fail && test_allocation_call_counter == 0) {
    ++test_allocation_call_counter;
    return nullptr;
  }
  if (test_force_second_alloc_fail && test_allocation_call_counter == 1) {
    // second allocation attempt should fail
    ++test_allocation_call_counter;
    return nullptr;
  }
  ++test_allocation_call_counter;
  return std::malloc(size);
}

static void png_free(png_const_structrp /*png_ptr*/, void* ptr) {
  if (ptr) std::free(ptr);
}

static void png_free_data(png_const_structrp /*png_ptr*/, png_inforp info_ptr,
                          int free_me, int /*flags*/) {
  // Only care about PNG_FREE_ICCP path in tests
  if (free_me & PNG_FREE_ICCP) {
    if (info_ptr) {
      if (info_ptr->iccp_name) {
        std::free(info_ptr->iccp_name);
        info_ptr->iccp_name = nullptr;
      }
      if (info_ptr->iccp_profile) {
        std::free(info_ptr->iccp_profile);
        info_ptr->iccp_profile = nullptr;
      }
      info_ptr->iccp_proflen = 0;
    }
  }
}

// C-style cast helper macro (mimics png_voidcast usage)
#define png_voidcast(type, value) reinterpret_cast<type>(value)

// The focal function under test (re-implemented to work with the test harness)
png_const_charp dummy_null = nullptr; // just to avoid unused var warnings

void png_set_iCCP(png_const_structrp png_ptr, png_inforp info_ptr,
    png_const_charp name, int compression_type,
    png_const_bytep profile, png_uint_32 proflen)
{
{
   png_charp new_iccp_name;
   png_bytep new_iccp_profile;
   size_t length;
   png_debug1(1, "in %s storage function", "iCCP");
   if (png_ptr == NULL || info_ptr == NULL || name == NULL || profile == NULL)
      return;
   if (compression_type != PNG_COMPRESSION_TYPE_BASE)
      png_app_error(png_ptr, "Invalid iCCP compression method");
   length = strlen(name)+1;
   new_iccp_name = png_voidcast(png_charp, png_malloc_warn(png_ptr, length));
   if (new_iccp_name == NULL)
   {
      png_benign_error(png_ptr, "Insufficient memory to process iCCP chunk");
      return;
   }
   memcpy(new_iccp_name, name, length);
   new_iccp_profile = png_voidcast(png_bytep,
       png_malloc_warn(png_ptr, proflen));
   if (new_iccp_profile == NULL)
   {
      png_free(png_ptr, new_iccp_name);
      png_benign_error(png_ptr,
          "Insufficient memory to process iCCP profile");
      return;
   }
   memcpy(new_iccp_profile, profile, proflen);
   png_free_data(png_ptr, info_ptr, PNG_FREE_ICCP, 0);
   info_ptr->iccp_proflen = proflen;
   info_ptr->iccp_name = new_iccp_name;
   info_ptr->iccp_profile = new_iccp_profile;
   info_ptr->free_me |= PNG_FREE_ICCP;
   info_ptr->valid |= PNG_INFO_iCCP;
}
}

// End of focal function

// Simple test harness: assertion helpers and test runners
static int tests_run = 0;
static int tests_passed = 0;

#define ASSERT(cond, msg) do { \
  tests_run++; \
  if (cond) { tests_passed++; } else { \
    std::cout << "[FAIL] " << msg << std::endl; \
  } \
} while(0)

#define ASSERT_TRUE(cond, msg) ASSERT((cond), msg)

// Test 1: Null argument handling
bool test_png_set_iCCP_null_arguments() {
  reset_all_tests_state();

  png_struct_def png;
  png_structrp png_ptr = &png;
  png_info_struct info;
  std::memset(&info, 0, sizeof(info));

  const char* name = "sRGB";
  const unsigned char profile_data[] = {0x01, 0x02, 0x03};
  png_uint_32 proflen = sizeof(profile_data);

  // Case 1: png_ptr is NULL
  png_set_iCCP(nullptr, &info, name, PNG_COMPRESSION_TYPE_BASE, profile_data, proflen);
  // info_ptr should remain unchanged
  ASSERT_TRUE(info.iccp_name == nullptr && info.iccp_profile == nullptr && info.iccp_proflen == 0, "Null png_ptr should not modify info_ptr");

  // Case 2: info_ptr is NULL
  png_set_iCCP(png_ptr, nullptr, name, PNG_COMPRESSION_TYPE_BASE, profile_data, proflen);
  // No crash, nothing should happen

  // Case 3: name is NULL
  png_set_iCCP(png_ptr, &info, nullptr, PNG_COMPRESSION_TYPE_BASE, profile_data, proflen);
  ASSERT_TRUE(info.iccp_name == nullptr && info.iccp_profile == nullptr, "Null name should not set any iCCP data");

  // Case 4: profile is NULL
  png_set_iCCP(png_ptr, &info, name, PNG_COMPRESSION_TYPE_BASE, nullptr, proflen);
  ASSERT_TRUE(info.iccp_name == nullptr && info.iccp_profile == nullptr, "Null profile should not set any iCCP data");

  return true;
}

// Test 2: Invalid compression method triggers app error
bool test_png_set_iCCP_invalid_compression() {
  reset_all_tests_state();

  png_struct_def png;
  png_structrp png_ptr = &png;
  png_info_struct info;
  std::memset(&info, 0, sizeof(info));

  const char* name = "sRGB";
  const unsigned char profile_data[] = {0xAA, 0xBB};
  png_uint_32 proflen = sizeof(profile_data);

  png_set_iCCP(png_ptr, &info, name, 1234, profile_data, proflen);

  ASSERT_TRUE(test_app_error_called, "Invalid compression should trigger png_app_error");
  // Ensure no data allocation/dependent changes occurred
  ASSERT_TRUE(info.iccp_name == nullptr && info.iccp_profile == nullptr && info.iccp_proflen == 0, "No iCCP data should be stored on error");
  return true;
}

// Test 3: First allocation failure (name buffer) short-circuits
bool test_png_set_iCCP_first_allocation_fail() {
  reset_all_tests_state();

  png_struct_def png;
  png_structrp png_ptr = &png;
  png_info_struct info;
  std::memset(&info, 0, sizeof(info));

  const char* name = "iCCP_name";
  const unsigned char profile_data[] = {0x10, 0x20, 0x30};
  png_uint_32 proflen = sizeof(profile_data);

  test_force_first_alloc_fail = true;
  test_allocation_call_counter = 0;

  png_set_iCCP(png_ptr, &info, name, PNG_COMPRESSION_TYPE_BASE, profile_data, proflen);

  // Expect no iCCP data set and a benign error
  ASSERT_TRUE(test_benign_error_called, "First allocation fail should trigger benign error");
  ASSERT_TRUE(info.iccp_name == nullptr && info.iccp_profile == nullptr && info.iccp_proflen == 0, "No iCCP data should be stored on alloc failure");
  return true;
}

// Test 4: Second allocation failure (profile buffer) short-circuits
bool test_png_set_iCCP_second_allocation_fail() {
  reset_all_tests_state();

  png_struct_def png;
  png_structrp png_ptr = &png;
  png_info_struct info;
  std::memset(&info, 0, sizeof(info));

  const char* name = "iCCP_name";
  const unsigned char profile_data[] = {0xDE, 0xAD, 0xBE, 0xEF};
  png_uint_32 proflen = sizeof(profile_data);

  test_force_first_alloc_fail = false;
  test_force_second_alloc_fail = true;
  test_allocation_call_counter = 0;

  png_set_iCCP(png_ptr, &info, name, PNG_COMPRESSION_TYPE_BASE, profile_data, proflen);

  // Expect benign error and no iCCP data
  // The benign error should be triggered due to second allocation fail
  // (We implemented benign_error for allocation failure)
  // After execution, iccp fields should still be NULL/zero
  ASSERT_TRUE(info.iccp_name == nullptr && info.iccp_profile == nullptr && info.iccp_proflen == 0, "Second allocation fail should not leave partial iCCP data");
  return true;
}

// Test 5: Successful iCCP storage path stores data and flags correctly
bool test_png_set_iCCP_success_path() {
  reset_all_tests_state();

  png_struct_def png;
  png_structrp png_ptr = &png;
  png_info_struct info;
  std::memset(&info, 0, sizeof(info));

  const char* name = "TestProfileName";
  const unsigned char profile_data[] = {0x01, 0x02, 0x03, 0x04, 0x05};
  png_uint_32 proflen = sizeof(profile_data);

  png_set_iCCP(png_ptr, &info, name, PNG_COMPRESSION_TYPE_BASE, profile_data, proflen);

  // Verify that data was stored and flags set
  ASSERT_TRUE(info.iccp_name != nullptr, "iCCP name should be allocated");
  ASSERT_TRUE(std::strcmp(info.iccp_name, name) == 0, "Stored iCCP name should match input");

  ASSERT_TRUE(info.iccp_profile != nullptr, "iCCP profile should be allocated");
  ASSERT_TRUE(info.iccp_proflen == proflen, "iCCP profile length should match input");
  ASSERT_TRUE(info.valid & PNG_INFO_iCCP, "PNG_INFO_iCCP flag should be set");
  ASSERT_TRUE(info.free_me & PNG_FREE_ICCP, "PNG_FREE_ICCP flag should be set");
  // Content check
  ASSERT_TRUE(std::memcmp(info.iccp_profile, profile_data, proflen) == 0, "Stored iCCP profile content should match input");

  return true;
}

// Test 6: Null name or profile after valid inputs should still be guarded (no crash)
bool test_png_set_iCCP_nulls_after_valid_input() {
  reset_all_tests_state();

  png_struct_def png;
  png_structrp png_ptr = &png;
  png_info_struct info;
  std::memset(&info, 0, sizeof(info));

  const char* name = "Name";
  const unsigned char profile_data[] = {0x11, 0x22};
  png_uint_32 proflen = sizeof(profile_data);

  // First, do a successful call
  png_set_iCCP(png_ptr, &info, name, PNG_COMPRESSION_TYPE_BASE, profile_data, proflen);

  // Then call with NULL name
  png_set_iCCP(png_ptr, &info, nullptr, PNG_COMPRESSION_TYPE_BASE, profile_data, proflen);
  // Now ensure still no crash and internal state is unchanged by the invalid call
  ASSERT_TRUE(info.iccp_name != nullptr && info.iccp_profile != nullptr, "State should persist after invalid subsequent call");
  return true;
}

// Test runner and main
int main() {
  std::cout << "Starting png_set_iCCP unit tests (standalone, no GTest)..." << std::endl;

  // Run tests
  test_png_set_iCCP_null_arguments();
  test_png_set_iCCP_invalid_compression();
  test_png_set_iCCP_first_allocation_fail();
  test_png_set_iCCP_second_allocation_fail();
  test_png_set_iCCP_success_path();
  test_png_set_iCCP_nulls_after_valid_input();

  // Simple aggregated result
  int total = tests_run;
  int passed = tests_passed;

  std::cout << "Test results: " << passed << " / " << total << " tests passed." << std::endl;

  // Exit code based on pass/fail
  return (passed == total) ? 0 : 1;
}