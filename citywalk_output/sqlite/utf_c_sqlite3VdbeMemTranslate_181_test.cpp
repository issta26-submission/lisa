/*
  Unit test suite for sqlite3VdbeMemTranslate (utf.c)

  - Uses the internal SQLite test dependencies available in the project
    (vdbeInt.h, sqliteInt.h) to construct Mem objects and invoke
    sqlite3VdbeMemTranslate.
  - Exercises key branches:
      1) In-place UTF-16 endian swap when both source and destination
         encodings are non-UTF8 (swap bytes, update enc).
      2) UTF-16LE/UTF16BE -> UTF-8 translation path (non-trivial encoding
         conversion), verifying resulting UTF-8 bytes and length.
      3) UTF-16BE/LE -> UTF-8 in another variant (to cover both BE/LE inputs).
  - Uses a small, lightweight test harness (no external test framework required).
  - All tests are self-contained and rely on the real sqlite3VdbeMemTranslate
    implementation provided by the project.

  Notes:
  - This test assumes a build environment where the internal SQLite headers
    (vdbeInt.h, sqliteInt.h) are available to the test translation unit.
  - The Mem structure and related constants (MEM_Str, SQLITE_UTF8, SQLITE_UTF16LE,
    SQLITE_UTF16BE, etc.) are provided by those headers.
  - The test uses malloc/free for input/out buffers consistent with
    the expected SQLite memory management semantics.
*/

#include <assert.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Internal SQLite includes (as per FOCAL_CLASS_DEP)

// Ensure C linkage for the external function under test
extern "C" {
  // The function under test is defined in utf.c
  int sqlite3VdbeMemTranslate(Mem *pMem, u8 desiredEnc);
}

// Simple test harness (non-terminating assertions)
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_PASS(msg) do { std::cout << "[PASS] " << msg << std::endl; ++g_tests_passed; } while(0)
#define TEST_FAIL(msg) do { std::cerr << "[FAIL] " << msg << std::endl; ++g_tests_failed; } while(0)
#define TEST_ASSERT(cond, msg) do { if(!(cond)) TEST_FAIL(msg); else TEST_PASS(msg); } while(0)

// Helper to print a buffer in hex for debugging (optional)
static void print_hex(const unsigned char *buf, int len) {
  for(int i = 0; i < len; ++i) {
    printf("%02X ", buf[i]);
  }
  printf("\n");
}

// Test 1: In-place UTF-16 endian swap (LE <-> BE) when both encodings are non-UTF8
// Input: "AB" encoded in UTF-16LE. Convert to UTF-16BE. Expect byte swap and encoding update.
static void test_swap_utf16_endian_le_to_be() {
  // Prepare Mem
  Mem m;
  memset(&m, 0, sizeof(Mem));
  m.db = 0;                    // No DB object; first assert will be satisfied by this
  m.flags = MEM_Str;           // String data present
  m.enc = SQLITE_UTF16LE;       // Source encoding: UTF-16 LE
  m.n = 4;                       // Two UTF-16 chars: 'A' (0x41 0x00), 'B' (0x42 0x00)

  // Allocate input buffer containing "AB" in UTF-16LE
  unsigned char *buf = (unsigned char *)malloc(4);
  buf[0] = 0x41; buf[1] = 0x00; // 'A'
  buf[2] = 0x42; buf[3] = 0x00; // 'B'
  m.z = (char *)buf;
  m.zMalloc = (char *)buf;
  m.szMalloc = 0;

  // Perform translation to UTF-16BE (swap bytes)
  int rc = sqlite3VdbeMemTranslate(&m, SQLITE_UTF16BE);
  TEST_ASSERT(rc == SQLITE_OK, "sqlite3VdbeMemTranslate should return SQLITE_OK for endian swap");

  // Validate behavior: encoding should be updated to BE and bytes swapped
  TEST_ASSERT(m.enc == SQLITE_UTF16BE, "Encoding should be updated to UTF-16BE after swap");
  unsigned char *out = (unsigned char *)m.z;
  // Expect swapped bytes: [0x00,0x41, 0x00,0x42]
  if(out[0] == 0x00 && out[1] == 0x41 && out[2] == 0x00 && out[3] == 0x42) {
    TEST_PASS("Endian swap produced correct BE representation for 'A','B'");
  } else {
    TEST_FAIL("Endian swap did not produce expected BE bytes for 'A','B'");
    std::cerr << "Got: ";
    print_hex(out, 4);
  }

  // Clean up allocated memory
  free((void *)buf);
}

// Test 2: UTF-16LE/UTF-16BE -> UTF-8 translation path (UTF-16 input to UTF-8 output)
// Input: "AB" encoded in UTF-16LE. Convert to UTF-8. Expect "AB" with terminator.
static void test_utf16le_to_utf8() {
  Mem m;
  memset(&m, 0, sizeof(Mem));
  m.db = 0;
  m.flags = MEM_Str;
  m.enc = SQLITE_UTF16LE;       // Source encoding: UTF-16LE
  m.n = 4;                       // Two UTF-16 chars: 'A','B'
  unsigned char *buf = (unsigned char *)malloc(4);
  buf[0] = 0x41; buf[1] = 0x00; // 'A'
  buf[2] = 0x42; buf[3] = 0x00; // 'B'
  m.z = (char *)buf;
  m.zMalloc = (char *)buf;
  m.szMalloc = 0;

  // Convert to UTF-8
  int rc = sqlite3VdbeMemTranslate(&m, SQLITE_UTF8);
  TEST_ASSERT(rc == SQLITE_OK, "sqlite3VdbeMemTranslate should return SQLITE_OK for UTF-16LE -> UTF-8");

  // Validate: buffer should contain "AB" followed by NUL terminator
  unsigned char *out = (unsigned char *)m.z;
  if(out[0] == 'A' && out[1] == 'B' && out[2] == '\0') {
    TEST_PASS("UTF-16LE -> UTF-8 translation produced 'AB' with terminator");
  } else {
    TEST_FAIL("UTF-16LE -> UTF-8 translation produced unexpected output");
    std::cerr << "Output bytes: ";
    print_hex(out, 3);
  }

  // Clean up
  free((void *)m.z); // m.z now points to newly allocated UTF-8 buffer
  free((void *)buf); // Original buffer (input) is freed
}

// Test 3: UTF-16BE -> UTF-8 translation path (BE input to UTF-8 output)
// Input: "AB" encoded in UTF-16BE. Convert to UTF-8. Expect "AB" with terminator.
static void test_utf16be_to_utf8() {
  Mem m;
  memset(&m, 0, sizeof(Mem));
  m.db = 0;
  m.flags = MEM_Str;
  m.enc = SQLITE_UTF16BE;       // Source encoding: UTF-16BE
  m.n = 4;                       // Two UTF-16 chars: 'A','B'
  unsigned char *buf = (unsigned char *)malloc(4);
  buf[0] = 0x00; buf[1] = 0x41; // 'A' in BE: 00 41
  buf[2] = 0x00; buf[3] = 0x42; // 'B' in BE: 00 42
  m.z = (char *)buf;
  m.zMalloc = (char *)buf;
  m.szMalloc = 0;

  // Convert to UTF-8
  int rc = sqlite3VdbeMemTranslate(&m, SQLITE_UTF8);
  TEST_ASSERT(rc == SQLITE_OK, "sqlite3VdbeMemTranslate should return SQLITE_OK for UTF-16BE -> UTF-8");

  // Validate: buffer should contain "AB" followed by NUL terminator
  unsigned char *out = (unsigned char *)m.z;
  if(out[0] == 'A' && out[1] == 'B' && out[2] == '\0') {
    TEST_PASS("UTF-16BE -> UTF-8 translation produced 'AB' with terminator");
  } else {
    TEST_FAIL("UTF-16BE -> UTF-8 translation produced unexpected output");
    std::cerr << "Output bytes: ";
    print_hex(out, 3);
  }

  // Clean up
  free((void *)m.z); // UTF-8 buffer allocated by translation
  free((void *)buf); // Original BE buffer
}

// Entry point
int main() {
  // Run tests
  test_swap_utf16_endian_le_to_be();
  test_utf16le_to_utf8();
  test_utf16be_to_utf8();

  // Summary
  std::cout << "Tests passed: " << g_tests_passed << ", Tests failed: " << g_tests_failed << std::endl;
  if(g_tests_failed > 0) {
    return 1;
  }
  return 0;
}