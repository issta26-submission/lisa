// Test suite for the focal method: Write_Zip64EndOfCentralDirectoryRecord
// This test targets the internal ZIP64 End-Of-Central-Directory Record writer.
// The test uses a self-contained harness, mocks the low level write function,
// and validates the exact byte sequence produced in the output buffer.
//
// Important: This test assumes the repository environment allows compiling the
// zip.c with a lightweight test harness. To enable access to the focal function,
// we override the local macro to non-static by defining 'local' as empty before
// including zip.c. We also mock the low-level writer zip64local_putValue to
// append bytes to a global buffer for verification.

#include <cstring>
#include <string.h>
#include <time.h>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <zip.c>
#include <stdlib.h>
#include <zip.h>


// ---------------------------------------------------------------------------
// Minimal stubbed types and macros to let the included zip.c compile in this TU
// and to provide the necessary types used by the focal method.
// In the real project these would come from zip.h and zlib headers; here we keep
// them minimal and consistent with the test harness.
// ---------------------------------------------------------------------------

// Ensure the helper definitions are provided before including zip.c
// We deliberately keep these definitions small and sufficient for the test.

#ifndef ZIP_OK
#define ZIP_OK 0
#endif

// ZIP64 end of central directory signature (standard value)
#ifndef ZIP64ENDHEADERMAGIC
#define ZIP64ENDHEADERMAGIC (0x06064b50)
#endif

// ZIP64 end of central directory data size (as used by the code)
#ifndef ZIP64ENDDATASIZE
#define ZIP64ENDDATASIZE (44)
#endif

#ifndef uLong
typedef unsigned long uLong;
#endif

#ifndef ZPOS64_T
typedef unsigned long long ZPOS64_T;
#endif

#ifndef voidpf
typedef void* voidpf;
#endif

// Minimal definition for zlib_filefunc64_32_def to satisfy signatures
typedef struct zlib_filefunc64_32_def {
  // content not used by the test, only its type is required
  int dummy;
} zlib_filefunc64_32_def;

// The actual zip64_internal structure is defined in zip.c; we do not redefine it here.
// The test harness will include zip.c into this translation unit and interact with:
// - zip64_internal* zi
// - zi->filestream
// - zi->z_filefunc
// - zi->number_entry
// - zi->add_position_when_writing_offset

// ---------------------------------------------------------------------------
// Prepare to include the focal implementation zip.c
// We want to access the focal static function Write_Zip64EndOfCentralDirectoryRecord.
// To do that, we:
 // 1) Override the 'local' macro to nothing so functions become externally linkable.
 // 2) Provide a minimal mock of zip64local_putValue used by the focal function.
// ---------------------------------------------------------------------------

// Override 'local' to allow external linkage for the function under test.
#define local

// Make sure the output buffer is globally accessible for verification
static std::vector<uint8_t> g_write_buffer;

// Mock implementation of the low-level WriteValue routine used by the focal method.
// It appends the given 'x' value into the global buffer in little-endian order with 'nbByte' bytes.
// The real function in the project would write to a file stream; here we verify the emitted bytes.
extern "C" int zip64local_putValue(const zlib_filefunc64_32_def* pzlib_filefunc_def, voidpf filestream, ZPOS64_T x, int nbByte)
{
  (void)pzlib_filefunc_def;   // unused in test
  (void)filestream;            // unused in test
  // Append little-endian bytes
  for (int i = 0; i < nbByte; ++i) {
    uint8_t byte = static_cast<uint8_t>((x >> (8 * i)) & 0xFF);
    g_write_buffer.push_back(byte);
  }
  // Simulate ZIP_OK
  return ZIP_OK;
}

// Include the focal implementation (zip.c) with our test harness.
// The include should provide the necessary symbols and definitions used by the
// focal method and the mock zip64local_putValue above.

// ---------------------------------------------------------------------------
// Test harness
// ---------------------------------------------------------------------------

// Helper function to compare expected bytes with actual buffer
bool equals(const std::vector<uint8_t>& actual, const std::vector<uint8_t>& expected)
{
  if (actual.size() != expected.size()) return false;
  for (size_t i = 0; i < actual.size(); ++i) {
    if (actual[i] != expected[i]) return false;
  }
  return true;
}

// Helper to print a byte sequence in hex for debugging
void print_hex(const std::vector<uint8_t>& v)
{
  for (size_t i = 0; i < v.size(); ++i) {
    printf("%02X ", static_cast<unsigned int>(v[i]));
  }
  printf("\n");
}

// Test 1: Success path - verifies the exact byte sequence produced by the function
bool test_Write_Zip64EndOfCentralDirectoryRecord_Success()
{
  // Reset global output buffer
  g_write_buffer.clear();

  // Prepare a minimal zip64_internal instance (layout defined in zip.c)
  zip64_internal zi;
  // Zero initialize to known state
  std::memset(&zi, 0, sizeof(zi));

  // Populate fields used by Write_Zip64EndOfCentralDirectoryRecord
  // filestream is not used by our mock writer, but the field exists in the struct
  zi.filestream = nullptr; // not used by mock
  zi.z_filefunc.dummy = 0;  // satisfy potential usage, though unused
  // Set a non-zero number of entries to verify 8-byte writes
  zi.number_entry = 0x1122334455667788ULL;
  // Position where writing offset begins (for pos calculation)
  zi.add_position_when_writing_offset = 0x0000000000001000ULL;

  // Central directory location in the zip file
  // We want pos to be centraldir_pos_inzip - add_position_when_writing_offset
  // Choose centraldir_pos_inzip = add_position + 0x1234, so pos == 0x1234
  ZPOS64_T centraldir_pos_inzip = zi.add_position_when_writing_offset + 0x1234;

  // Central directory size to encode
  uLong size_centraldir = 0x1122334455667788ULL & 0xFFFFFFFFFFFFFFFFULL;

  // Clear buffer and invoke the function under test
  g_write_buffer.clear();
  int err = Write_Zip64EndOfCentralDirectoryRecord(&zi, size_centraldir, centraldir_pos_inzip);

  if (err != ZIP_OK) {
    std::cerr << "Test Success: Unexpected error code: " << err << "\n";
    return false;
  }

  // Build the expected byte sequence step by step
  std::vector<uint8_t> expected;
  // 4 bytes: ZIP64 END HEADER MAGIC (little-endian)
  // Signature: ZIP64ENDHEADERMAGIC = 0x06064b50 -> bytes: 50 4B 06 06
  expected.push_back(0x50);
  expected.push_back(0x4B);
  expected.push_back(0x06);
  expected.push_back(0x06);

  // 8 bytes: Zip64DataSize = 44 (0x2C)
  uint64_t dsize = 44ULL;
  for (int i = 0; i < 8; ++i) {
    expected.push_back(static_cast<uint8_t>((dsize >> (8 * i)) & 0xFF));
  }

  // 2 bytes: Version made by = 45 (0x2D)
  expected.push_back(0x2D);
  expected.push_back(0x00);

  // 2 bytes: Version needed = 45 (0x2D)
  expected.push_back(0x2D);
  expected.push_back(0x00);

  // 4 bytes: Number of this disk = 0
  expected.push_back(0x00);
  expected.push_back(0x00);
  expected.push_back(0x00);
  expected.push_back(0x00);

  // 4 bytes: Number of disk with start of central directory = 0
  expected.push_back(0x00);
  expected.push_back(0x00);
  expected.push_back(0x00);
  expected.push_back(0x00);

  // 8 bytes: Total number of entries in the central dir on this disk = zi.number_entry
  for (int i = 0; i < 8; ++i) {
    expected.push_back(static_cast<uint8_t>((zi.number_entry >> (8 * i)) & 0xFF));
  }

  // 8 bytes: Total number of entries in the central dir = zi.number_entry
  for (int i = 0; i < 8; ++i) {
    expected.push_back(static_cast<uint8_t>((zi.number_entry >> (8 * i)) & 0xFF));
  }

  // 8 bytes: Size of the central directory = size_centraldir
  for (int i = 0; i < 8; ++i) {
    expected.push_back(static_cast<uint8_t>((static_cast<uint64_t>(size_centraldir) >> (8 * i)) & 0xFF));
  }

  // 8 bytes: Offset of start of central directory with respect to the starting disk number
  // pos = centraldir_pos_inzip - zi.add_position_when_writing_offset
  uint64_t pos = static_cast<uint64_t>(centraldir_pos_inzip - zi.add_position_when_writing_offset);
  for (int i = 0; i < 8; ++i) {
    expected.push_back(static_cast<uint8_t>((pos >> (8 * i)) & 0xFF));
  }

  // Compare buffers
  bool ok = equals(g_write_buffer, expected);
  if (!ok) {
    std::cerr << "Test Success: Byte sequence mismatch. Got:\n";
    print_hex(g_write_buffer);
    std::cerr << "Expected:\n";
    print_hex(expected);
  }

  return ok;
}

// Entry point for the test executable
int main()
{
  bool all_passed = true;

  // Run test 1
  if (test_Write_Zip64EndOfCentralDirectoryRecord_Success()) {
    std::cout << "[PASS] test_Write_Zip64EndOfCentralDirectoryRecord_Success\n";
  } else {
    std::cout << "[FAIL] test_Write_Zip64EndOfCentralDirectoryRecord_Success\n";
    all_passed = false;
  }

  // Extend with additional tests (e.g., failure path) if the environment supports it.

  if (all_passed) {
    std::cout << "All tests passed.\n";
    return 0;
  } else {
    std::cerr << "Some tests failed.\n";
    return 1;
  }
}