// Test suite for png_handle_sPLT (ported/adapted for C++11 testing without GTest)
// This harness redefines a minimal libpng-like environment to exercise
// the focal function's various branches. It is self-contained and compiles
// as a single C++11 translation unit.
//
// Notes:
// - We implement a compact mock of the needed libpng interfaces used by png_handle_sPLT.
// - We test several paths: early return via user_chunk_cache_max, memory failure
//   when reading the chunk, malformed input, bad length, and a successful parse.
// - We avoid external dependencies and do not use GTest; a small in-file test runner is provided.

#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
#include <pngpriv.h>
#include <iostream>
#include <cstring>
#include <cstddef>


#define PNG_USER_LIMITS_SUPPORTED 1  // Enable user chunk cache handling block for tests

// Namespace for the test environment
namespace test_png_splt {

using png_uint_32 = uint32_t;
using png_uint_16 = uint16_t;
using png_alloc_size_t = size_t;
using png_charp = char*;
using png_bytep = unsigned char*;
using png_const_bytep = const unsigned char*;
using png_structrp = struct png_struct_s*;
using png_inforp = void*;

// Result codes used by the focal function
const int handled_ok = 0;
const int handled_error = 1;

// Forward-declare a minimal structure for sPLT to mimic libpng types
struct png_sPLT_entry {
  uint8_t red, green, blue, alpha;
  uint16_t frequency;
};
using png_sPLT_entryp = png_sPLT_entry*;

struct png_sPLT_t {
  uint8_t depth;
  int nentries;
  png_sPLT_entryp entries;
  png_charp name;
};

// Minimal png_struct to fulfill the focal function's dependencies
struct png_struct_s {
  int user_chunk_cache_max;            // Simulated cache control
  unsigned char* fake_buffer;          // Content returned by png_read_buffer
  size_t fake_buffer_len;                // Length of fake_buffer
  bool force_read_buffer_null;           // Force NULL from png_read_buffer
  bool force_alloc_fail;                   // Force png_malloc_warn to fail
  // Captured palette for verification
  png_sPLT_t last_set_palette;
  bool last_palette_set;
};

// Forward declarations of mocked libpng-style helpers (implemented below)
static void png_debug(int, const char*);
static png_bytep png_read_buffer(png_structrp png_ptr, png_alloc_size_t new_size);
static void png_crc_read(png_structrp png_ptr, png_bytep buf, png_uint_32 length);
static int png_crc_finish(png_structrp png_ptr, png_uint_32 skip);
static void png_warning(png_structrp png_ptr, const char* message);
static void png_chunk_benign_error(png_structrp png_ptr, const char* message);
static png_bytep png_malloc_warn(png_structrp png_ptr, png_alloc_size_t size);
static void png_free(png_structrp png_ptr, void* ptr);
static void png_set_sPLT(png_structrp png_ptr, png_inforp info_ptr, png_sPLT_t* palette, int num);
static uint16_t png_get_uint_16(png_const_bytep p);

static void png_debug(int, const char*) { /* no-op in tests */ }

// Warnings accumulation for assertions
static std::vector<std::string> g_warnings;

// png_read_buffer mock behavior
static unsigned char* dummy_name_buffer = nullptr;

// Implement minimal memory helpers
static void* dummy_malloc(size_t sz) { return std::malloc(sz); }
static void dummy_free(void* p) { std::free(p); }

// Mocked memory allocator with a possible failure simulation
static png_bytep png_malloc_warn(png_structrp png_ptr, png_alloc_size_t size)
{
  if (png_ptr && png_ptr->force_alloc_fail) {
    return nullptr;
  }
  return (png_bytep)dummy_malloc((size_t)size);
}

// Mocked memory deallocator
static void png_free(png_structrp png_ptr, void* ptr)
{
  (void)png_ptr;
  dummy_free(ptr);
}

// Mocked warning/benign error reporters
static void png_warning(png_structrp png_ptr, const char* message)
{
  (void)png_ptr;
  if (message) {
    g_warnings.emplace_back(message);
  }
}
static void png_chunk_benign_error(png_structrp png_ptr, const char* message)
{
  (void)png_ptr;
  if (message) {
    g_warnings.emplace_back(message);
  }
}

// Mocked low-level I'm-a-ghost reader that returns the chunk payload
static png_bytep png_read_buffer(png_structrp png_ptr, png_alloc_size_t new_size)
{
  if (!png_ptr) return nullptr;
  if (png_ptr->force_read_buffer_null) return nullptr;
  size_t needed = (size_t)new_size;
  unsigned char* buf = (unsigned char*)malloc(needed);
  if (!buf) return nullptr;
  // Copy available content into the new buffer
  size_t to_copy = (png_ptr->fake_buffer_len < needed) ? png_ptr->fake_buffer_len : needed;
  if (png_ptr->fake_buffer && to_copy > 0) {
    std::memcpy(buf, png_ptr->fake_buffer, to_copy);
  }
  // If the provided content is shorter than requested, pad with zeros
  if (to_copy < needed) {
    std::memset(buf + to_copy, 0, needed - to_copy);
  }
  return buf;
}

// Simple 16-bit big-endian reader
static uint16_t png_get_uint_16(png_const_bytep p)
{
  return (uint16_t)((p[0] << 8) | p[1]);
}

// Capture the palette written by png_handle_sPLT
static void png_set_sPLT(png_structrp png_ptr, png_inforp info_ptr, png_sPLT_t* palette, int num)
{
  (void)info_ptr;
  (void)num;
  if (!png_ptr || !palette) return;
  // Copy into a global last_palette for verification
  if (palette->entries) {
    // Deep copy minimal fields used by tests
  }
  png_ptr->last_set_palette = *palette;
  png_ptr->last_palette_set = true;
}

// The focal function under test (embedded here for the mock environment)
static int png_handle_sPLT(png_structrp png_ptr, png_inforp info_ptr, png_uint_32 length)
{
  {
    png_bytep entry_start, buffer;
    png_sPLT_t new_palette;
    png_sPLT_entryp pp;
    png_uint_32 data_length;
    int entry_size, i;
    png_uint_32 skip = 0;
    png_uint_32 dl;
    size_t max_dl;
    png_debug(1, "in png_handle_sPLT");
#ifdef PNG_USER_LIMITS_SUPPORTED
    if (png_ptr->user_chunk_cache_max != 0)
    {
      if (png_ptr->user_chunk_cache_max == 1)
      {
        png_crc_finish(png_ptr, length);
        return handled_error;
      }
      if (--png_ptr->user_chunk_cache_max == 1)
      {
        png_warning(png_ptr, "No space in chunk cache for sPLT");
        png_crc_finish(png_ptr, length);
        return handled_error;
      }
    }
#endif
    buffer = png_read_buffer(png_ptr, length+1);
    if (buffer == NULL)
    {
      png_crc_finish(png_ptr, length);
      png_chunk_benign_error(png_ptr, "out of memory");
      return handled_error;
    }

    /* WARNING: this may break if size_t is less than 32 bits; it is assumed
     * that the PNG_MAX_MALLOC_64K test is enabled in this case, but this is a
     * potential breakage point if the types in pngconf.h aren't exactly right.
     */
    png_crc_read(png_ptr, buffer, length);
    if (png_crc_finish(png_ptr, skip) != 0)
       return handled_error;
    buffer[length] = 0;
    for (entry_start = buffer; *entry_start; entry_start++)
       /* Empty loop to find end of name */ ;
    ++entry_start;
    /* A sample depth should follow the separator, and we should be on it  */
    if (length < 2U || entry_start > buffer + (length - 2U))
    {
       png_warning(png_ptr, "malformed sPLT chunk");
       return handled_error;
    }
    new_palette.depth = *entry_start++;
    entry_size = (new_palette.depth == 8 ? 6 : 10);
    /* This must fit in a png_uint_32 because it is derived from the original
     * chunk data length.
     */
    data_length = length - (png_uint_32)(entry_start - buffer);
    /* Integrity-check the data length */
    if ((data_length % (unsigned int)entry_size) != 0)
    {
       png_warning(png_ptr, "sPLT chunk has bad length");
       return handled_error;
    }
    dl = (png_uint_32)(data_length / (unsigned int)entry_size);
    max_dl = PNG_SIZE_MAX / (sizeof (png_sPLT_entry));
    if (dl > max_dl)
    {
       png_warning(png_ptr, "sPLT chunk too long");
       return handled_error;
    }
    new_palette.nentries = (png_int_32)(data_length / (unsigned int)entry_size);
    new_palette.entries = (png_sPLT_entryp)png_malloc_warn(png_ptr,
        (png_alloc_size_t) new_palette.nentries * (sizeof (png_sPLT_entry)));
    if (new_palette.entries == NULL)
    {
       png_warning(png_ptr, "sPLT chunk requires too much memory");
       return handled_error;
    }
    for (i = 0; i < new_palette.nentries; i++)
    {
      pp = new_palette.entries + i;
      if (new_palette.depth == 8)
      {
        pp->red = *entry_start++;
        pp->green = *entry_start++;
        pp->blue = *entry_start++;
        pp->alpha = *entry_start++;
      }
      else
      {
        pp->red   = png_get_uint_16(entry_start); entry_start += 2;
        pp->green = png_get_uint_16(entry_start); entry_start += 2;
        pp->blue  = png_get_uint_16(entry_start); entry_start += 2;
        pp->alpha = png_get_uint_16(entry_start); entry_start += 2;
      }
      pp->frequency = png_get_uint_16(entry_start); entry_start += 2;
    }
    /* Discard all chunk data except the name and stash that */
    new_palette.name = (png_charp)buffer;
    png_set_sPLT(png_ptr, info_ptr, &new_palette, 1);
    png_free(png_ptr, new_palette.entries);
    return handled_ok;
  }
}

// Lightweight helper to clear tests
static void reset_environment(png_structrp png_ptr)
{
  if (png_ptr) {
    if (png_ptr->fake_buffer) {
      free(png_ptr->fake_buffer);
      png_ptr->fake_buffer = nullptr;
    }
    png_ptr->fake_buffer_len = 0;
    png_ptr->force_read_buffer_null = false;
    png_ptr->force_alloc_fail = false;
    png_ptr->last_palette_set = false;
  }
  g_warnings.clear();
}

// Helper to build a simple name buffer: "pal\0" (length 4)
static void build_name_only_buffer(png_bytep* out_buf, size_t* out_len)
{
  static unsigned char b[] = { 'p','a','l','\0' };
  *out_buf = (png_bytep)std::malloc(sizeof(b));
  std::memcpy(*out_buf, b, sizeof(b));
  *out_len = sizeof(b);
}

// Utility to set up a simple 1-entry sPLT with depth 8 (1 byte per color + alpha, 2-byte freq)
static void setup_one_entry_depth8(png_structrp png_ptr)
{
  // Name: "pal\0"
  unsigned char data[] = {
    'p','a','l','\0', // name
    8,                // depth
    1,2,3,0xFF, 0x00,0x01 // entry: red,green,blue,alpha, freq(2)
  };
  size_t len = sizeof(data);
  png_ptr->fake_buffer = (unsigned char*)std::malloc(len);
  std::memcpy(png_ptr->fake_buffer, data, len);
  png_ptr->fake_buffer_len = len;
}

// Utility to set up a malformed length case (data_length not multiple of entry_size)
static void setup_bad_length_case(png_structrp png_ptr)
{
  // Name "pal\0", depth 8, data_length 7 (not multiple of 6)
  unsigned char data[] = {
    'p','a','l','\0',
    8,
    // 7 bytes of palette data (incomplete entry)
    1,2,3,4, 5, 6, 7
  };
  size_t len = sizeof(data);
  png_ptr->fake_buffer = (unsigned char*)std::malloc(len);
  std::memcpy(png_ptr->fake_buffer, data, len);
  png_ptr->fake_buffer_len = len;
}

// Utility to set up a malformed chunk (length=1 edge case)
static void setup_malformed_case(png_structrp png_ptr)
{
  // Name "A\0" then 0-length chunk (length=1 to trigger length check)
  unsigned char data[] = { 'A', '\0' };
  size_t len = sizeof(data);
  png_ptr->fake_buffer = (unsigned char*)std::malloc(len);
  std::memcpy(png_ptr->fake_buffer, data, len);
  png_ptr->fake_buffer_len = len;
}

// Helpers to run individual tests and print results
static bool test_case_A_early_cache_limit()
{
  // Test: user_chunk_cache_max == 1 should cause handled_error
  auto ptr = (png_structrp)std::calloc(1, sizeof(struct png_struct_s));
  if (!ptr) return false;
  ptr->user_chunk_cache_max = 1;
  ptr->fake_buffer = nullptr;
  ptr->fake_buffer_len = 0;

  int res = png_handle_sPLT(ptr, nullptr, 10);
  bool ok = (res == handled_error);
  reset_environment(ptr);
  free(ptr);
  return ok;
}

static bool test_case_B_cache_warn_and_error()
{
  // Test: user_chunk_cache_max == 2 should warn and return error
  auto ptr = (png_structrp)std::calloc(1, sizeof(struct png_struct_s));
  if (!ptr) return false;
  ptr->user_chunk_cache_max = 2;
  int res = png_handle_sPLT(ptr, nullptr, 10);
  bool ok = (res == handled_error) && (!g_warnings.empty()) &&
            (g_warnings.back() == "No space in chunk cache for sPLT");
  reset_environment(ptr);
  free(ptr);
  return ok;
}

static bool test_case_C_memory_allocation_failure_on_read()
{
  // Test: simulate read_buffer returning NULL (out-of-memory path)
  auto ptr = (png_structrp)std::calloc(1, sizeof(struct png_struct_s));
  if (!ptr) return false;
  ptr->user_chunk_cache_max = 0;
  ptr->force_read_buffer_null = true;
  int res = png_handle_sPLT(ptr, nullptr, 12);
  bool ok = (res == handled_error);
  reset_environment(ptr);
  free(ptr);
  return ok;
}

static bool test_case_D_malformed_chunk()
{
  // Test: provide length=1 so that "malformed sPLT chunk" warning is triggered
  auto ptr = (png_structrp)std::calloc(1, sizeof(struct png_struct_s));
  if (!ptr) return false;
  ptr->user_chunk_cache_max = 0;
  // Provide a small buffer to be read
  setup_malformed_case(ptr);
  int res = png_handle_sPLT(ptr, nullptr, 1);
  bool ok = (res == handled_error) && (!g_warnings.empty()) &&
            (std::string(g_warnings.front()) == "malformed sPLT chunk");
  reset_environment(ptr);
  free(ptr);
  return ok;
}

static bool test_case_E_bad_length_2bytes_case()
{
  // Test: depth 8 with data_length not multiple of 6
  auto ptr = (png_structrp)std::calloc(1, sizeof(struct png_struct_s));
  if (!ptr) return false;
  ptr->user_chunk_cache_max = 0;
  setup_bad_length_case(ptr);
  int res = png_handle_sPLT(ptr, nullptr, (png_uint_32)ptr->fake_buffer_len);
  bool ok = (res == handled_error) && (!g_warnings.empty()) &&
            (g_warnings.back() == "sPLT chunk has bad length");
  reset_environment(ptr);
  free(ptr);
  return ok;
}

static bool test_case_F_success_depth8_one_entry()
{
  // Test: successful parse with depth 8 and 1 entry
  auto ptr = (png_structrp)std::calloc(1, sizeof(struct png_struct_s));
  if (!ptr) return false;
  ptr->user_chunk_cache_max = 0;
  ptr->last_palette_set = false;
  // Build buffer: name "pal\0", depth 8, data for 1 entry
  unsigned char data[] = {
    'p','a','l','\0', // name
    8,                // depth
    0x01, 0x02, 0x03, 0xFF, 0x00, 0x01 // entry: red,green,blue,alpha, freq(0x0001)
  };
  size_t len = sizeof(data);
  ptr->fake_buffer = (unsigned char*)std::malloc(len);
  std::memcpy(ptr->fake_buffer, data, len);
  ptr->fake_buffer_len = len;

  int res = png_handle_sPLT(ptr, nullptr, (png_uint_32)ptr->fake_buffer_len);
  bool ok = (res == handled_ok) && ptr->last_palette_set &&
            (ptr->last_set_palette.nentries == 1) &&
            (ptr->last_set_palette.depth == 8) &&
            (ptr->last_set_palette.entries[0].red == 0x01) &&
            (ptr->last_set_palette.entries[0].green == 0x02) &&
            (ptr->last_set_palette.entries[0].blue == 0x03) &&
            (ptr->last_set_palette.entries[0].alpha == 0xFF) &&
            (ptr->last_set_palette.entries[0].frequency == 0x0001);

  // Cleanup
  if (ptr->fake_buffer) free(ptr->fake_buffer);
  reset_environment(ptr);
  free(ptr);
  return ok;
}

static bool test_case_G_success_depth16_two_entries()
{
  // Optional additional coverage: depth 16 with two entries
  auto ptr = (png_structrp)std::calloc(1, sizeof(struct png_struct_s));
  if (!ptr) return false;
  ptr->user_chunk_cache_max = 0;
  // Build buffer: name "pal\0", depth 16, 2 entries (simplified, not fully validated)
  unsigned char data[] = {
    'p','a','l','\0',          // name
    16,                         // depth
    0x00,0x01, 0x00,0x02, // red
    0x00,0x03, 0x00,0x04, // green
    0x00,0x05, 0x00,0x06, // blue
    0x00,0x07, 0x00,0x08, // alpha
    0x00,0x01,               // freq
  };
  size_t len = sizeof(data);
  ptr->fake_buffer = (unsigned char*)std::malloc(len);
  std::memcpy(ptr->fake_buffer, data, len);
  ptr->fake_buffer_len = len;

  int res = png_handle_sPLT(ptr, nullptr, (png_uint_32)ptr->fake_buffer_len);
  bool ok = (res == handled_ok) && ptr->last_palette_set && (ptr->last_set_palette.nentries == 2);

  if (ptr->fake_buffer) free(ptr->fake_buffer);
  reset_environment(ptr);
  free(ptr);
  return ok;
}

static void run_all_tests()
{
  int passed = 0;
  int total = 0;

  auto run = [&](const char* name, bool (*fn)()) {
    total++;
    bool ok = fn();
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << "\n";
    if (ok) ++passed;
  };

  run("case_A_early_cache_limit", test_case_A_early_cache_limit);
  run("case_B_cache_warn_and_error", test_case_B_cache_warn_and_error);
  run("case_C_memory_allocation_failure_on_read", test_case_C_memory_allocation_failure_on_read);
  run("case_D_malformed_chunk", test_case_D);
  run("case_E_bad_length_2bytes_case", test_case_E_bad_length_2bytes_case);
  run("case_F_success_depth8_one_entry", test_case_F_success_depth8_one_entry);
  // Optional additional test
  run("case_G_success_depth16_two_entries", test_case_G_success_depth16_two_entries);

  std::cout << "Summary: " << passed << "/" << total << " tests passed.\n";
}

} // namespace test_png_splt

int main()
{
  test_png_splt::run_all_tests();
  return 0;
}