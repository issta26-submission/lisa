#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cstddef>


// Minimal, self-contained stubs to exercise the focal function logic.
// This test suite is designed to compile with C++11 and not rely on GTest.
// It mocks just enough of the PNG types and constants to validate the control flow
// and CRC update behavior of png_calculate_crc as described in the focal method.

/* Type and macro definitions to mirror the focal function environment */
typedef unsigned char png_byte;
typedef const unsigned char png_const_bytep;
typedef unsigned char png_bytep;
typedef unsigned int png_uint_32;
typedef unsigned long uLong;
typedef unsigned int uInt;
typedef std::size_t size_t;

struct png_struct {
  unsigned int chunk_name;     // Simulated chunk name (with ancillary flag in high bit)
  unsigned int flags;          // Flags controlling CRC behavior
  unsigned int crc;              // Current CRC value (32-bit)
};

typedef png_struct* png_structrp;
typedef const png_struct* png_const_structrp;

/* Macros/flags to drive test scenarios (simplified) */
#define PNG_CHUNK_ANCILLARY(chunk) ((chunk) & 0x80)  // MSB set => ancillary
#define PNG_FLAG_CRC_ANCILLARY_MASK 0x03
#define PNG_FLAG_CRC_ANCILLARY_USE 0x01
#define PNG_FLAG_CRC_ANCILLARY_NOWARN 0x02
#define PNG_FLAG_CRC_CRITICAL_IGNORE 0x04

// Forward declaration for our fake CRC32 (test-specific)
extern "C" uLong crc32(uLong crc, const png_byte* buf, uInt len);

/* Focal method copied/adapted for the isolated test environment.
 * In real code, this would reside in png.c; here we reproduce it to enable unit tests.
 */
extern "C" void png_calculate_crc(png_structrp png_ptr, png_const_bytep ptr, size_t length)
{
   int need_crc = 1;
   if (PNG_CHUNK_ANCILLARY(png_ptr->chunk_name) != 0)
   {
      if ((png_ptr->flags & PNG_FLAG_CRC_ANCILLARY_MASK) ==
          (PNG_FLAG_CRC_ANCILLARY_USE | PNG_FLAG_CRC_ANCILLARY_NOWARN))
         need_crc = 0;
   }
   else /* critical */
   {
      if ((png_ptr->flags & PNG_FLAG_CRC_CRITICAL_IGNORE) != 0)
         need_crc = 0;
   }
   /* 'uLong' is defined in zlib.h as unsigned long; this means that on some
    * systems it is a 64-bit value.  crc32, however, returns 32 bits so the
    * following cast is safe.  'uInt' may be no more than 16 bits, so it is
    * necessary to perform a loop here.
    */
   if (need_crc != 0 && length > 0)
   {
      uLong crc = png_ptr->crc; /* Should never issue a warning */
      do
      {
         uInt safe_length = (uInt)length;
#ifndef __COVERITY__
         if (safe_length == 0)
            safe_length = (uInt)-1; /* evil, but safe */
#endif
         crc = crc32(crc, ptr, safe_length);
         /* The following should never issue compiler warnings; if they do the
          * target system has characteristics that will probably violate other
          * assumptions within the libpng code.
          */
         ptr += safe_length;
         length -= safe_length;
      }
      while (length > 0);
      /* And the following is always safe because the crc is only 32 bits. */
      png_ptr->crc = (png_uint_32)crc;
   }
}

/* Simple, deterministic CRC32 stub for testing purposes.
 * It is not a real CRC32, but is deterministic and sufficient for exercising
 * the control flow and update behavior in the focal method.
 */
extern "C" uLong crc32(uLong crc, const png_byte* buf, uInt len)
{
  // A tiny, deterministic "CRC-like" accumulator for test purposes.
  unsigned long acc = crc;
  acc = acc ^ 0xFFFFFFFFu;
  for (uInt i = 0; i < len; ++i) {
    acc = acc + static_cast<unsigned long>(buf[i]);
  }
  acc = acc ^ static_cast<unsigned long>(len);
  return static_cast<uLong>(acc);
}

/* Helper: initialize a data buffer for tests */
static void fill_buffer(png_byte* dst, const char* s, size_t n)
{
    // Copy exactly n bytes from s; if s shorter, pad with zeros.
    size_t slen = std::strlen(s);
    size_t copy_len = (n < slen) ? n : slen;
    std::memcpy(dst, s, copy_len);
    if (copy_len < n) {
        std::memset(dst + copy_len, 0, n - copy_len);
    }
}

/* ---------------------------- Test Cases ---------------------------- */

// Test 1: Ancillary chunk with mask set to "use + nowarn" -> need_crc should be 0
static bool test_ancillary_mask_causes_no_crc()
{
    png_struct s;
    s.chunk_name = 0x80; // Ancillary flag set (MSB)
    s.flags = PNG_FLAG_CRC_ANCILLARY_USE | PNG_FLAG_CRC_ANCILLARY_NOWARN; // mask matches -> no CRC
    s.crc = 0x12345678u;

    const char data[] = "abcdef"; // length 6
    png_const_bytep p = reinterpret_cast<png_const_bytep>(data);

    png_calculate_crc(&s, p, sizeof(data) - 1 + 1); // length 6

    // Expect no CRC update
    return s.crc == 0x12345678u;
}

// Test 2: Ancillary chunk without the specific mask -> CRC should update
static bool test_ancillary_crc_updated()
{
    png_struct s;
    s.chunk_name = 0x80; // Ancillary
    s.flags = 0x00;      // mask not matching -> should update
    s.crc = 0u;

    const char data[] = "ABCD"; // length 4
    png_const_bytep p = reinterpret_cast<png_const_bytep>(data);

    // Record expected CRC using the same CRC32 stub
    unsigned long expected = crc32(0u, reinterpret_cast<const png_byte*>(data), 4);

    png_calculate_crc(&s, p, 4);

    return s.crc == static_cast<unsigned int>(expected);
}

// Test 3: Critical chunk with IGNORE flag -> CRC should not be calculated
static bool test_critical_ignore_no_crc()
{
    png_struct s;
    s.chunk_name = 0x00; // Critical
    s.flags = PNG_FLAG_CRC_CRITICAL_IGNORE;
    s.crc = 0xDEADBEEFu;

    const char data[] = "XYZ"; // length 3
    png_const_bytep p = reinterpret_cast<png_const_bytep>(data);

    png_calculate_crc(&s, p, 3);

    // CRC should remain unchanged
    return s.crc == 0xDEADBEEFu;
}

// Test 4: Critical chunk without IGNORE -> CRC should update
static bool test_critical_crc_updated()
{
    png_struct s;
    s.chunk_name = 0x00; // Critical
    s.flags = 0x00;      // No ignore
    s.crc = 1u;

    const char data[] = "ABC"; // length 3
    png_const_bytep p = reinterpret_cast<png_const_bytep>(data);

    unsigned long expected = crc32(1u, reinterpret_cast<const png_byte*>(data), 3);

    png_calculate_crc(&s, p, 3);

    return s.crc == static_cast<unsigned int>(expected);
}

// Test 5: Length = 0 should not alter CRC
static bool test_zero_length_no_crc_change()
{
    png_struct s;
    s.chunk_name = 0x00; // Critical
    s.flags = 0x00;
    s.crc = 0xABCD1234u;

    const char data[] = "HELLO"; // length 5, but we pass length 0
    png_const_bytep p = reinterpret_cast<png_const_bytep>(data);

    png_calculate_crc(&s, p, 0);

    return s.crc == 0xABCD1234u;
}

/* ---------------------------- Main Entry ---------------------------- */

int main()
{
    int fail_count = 0;

    std::cout << "Running unit tests for png_calculate_crc (self-contained test harness)..." << std::endl;

    if (!test_ancillary_mask_causes_no_crc()) {
        std::cout << "Test 1 failed: Ancillary with mask should skip CRC calculation." << std::endl;
        ++fail_count;
    } else {
        std::cout << "Test 1 passed." << std::endl;
    }

    if (!test_ancillary_crc_updated()) {
        std::cout << "Test 2 failed: Ancillary without proper mask should update CRC." << std::endl;
        ++fail_count;
    } else {
        std::cout << "Test 2 passed." << std::endl;
    }

    if (!test_critical_ignore_no_crc()) {
        std::cout << "Test 3 failed: Critical with IGNORE flag should skip CRC." << std::endl;
        ++fail_count;
    } else {
        std::cout << "Test 3 passed." << std::endl;
    }

    if (!test_critical_crc_updated()) {
        std::cout << "Test 4 failed: Critical without IGNORE should update CRC." << std::endl;
        ++fail_count;
    } else {
        std::cout << "Test 4 passed." << std::endl;
    }

    if (!test_zero_length_no_crc_change()) {
        std::cout << "Test 5 failed: Zero length should not modify CRC." << std::endl;
        ++fail_count;
    } else {
        std::cout << "Test 5 passed." << std::endl;
    }

    if (fail_count == 0) {
        std::cout << "All tests passed successfully." << std::endl;
        return 0;
    } else {
        std::cout << fail_count << " test(s) failed." << std::endl;
        return 1;
    }
}