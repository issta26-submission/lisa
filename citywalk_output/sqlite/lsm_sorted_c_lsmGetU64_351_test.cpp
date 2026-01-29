// High-quality C++11 unit tests for the focal method: lsmGetU64
// This test suite is designed to be compiled alongside the C source
// in lsm_sorted.c and linked appropriately (no GoogleTest used).

/*
Step 1 insights (for test planning):
- Focal method: u64 lsmGetU64(u8 *aOut)
- Purpose: Assemble a 64-bit unsigned integer from 8 bytes in big-endian order.
- Core dependent components (Candidate Keywords):
  - aOut[0] .. aOut[7], 8-byte input
  - Shifts: 56, 48, 40, 32, 24, 16, 8, 0
  - Result type: u64
  - Data types: u8 (byte), u64 (64-bit value)
- Behavior: return (aOut[0] << 56) | (aOut[1] << 48) | ... | aOut[7]
- Edge cases: all zeros, all ones (0xFF), mixed bytes, single non-zero byte, high bit set, etc.
*/

/*
Step 2 and 3: Unit test generation and refinement
- Tests cover:
  - All-zero input -> 0
  - Example sequence [0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08] -> 0x0102030405060708
  - All-ones input -> 0xFFFFFFFFFFFFFFFF
  - Mixed pattern to validate endianness and correct bit placement
  - High-bit set (0x80 at MSB) -> 0x8000000000000000
- Test harness uses non-terminating assertions (collects failures but continues)
- Static functions in the C file are not directly accessed; we test only the focal function.
- We implement a tiny test framework (EXPECT_EQ) to accumulate failures
*/

#include <iomanip>
#include <iostream>
#include <cstdint>


// Type aliases matching the codebase's style used in lsm_sorted.c
using u8  = unsigned char;
using u64 = unsigned long long;

// Linkage to the focal C function.
// Ensure C linkage to avoid name mangling when linking with C code.
extern "C" {
  // The actual signature in lsm_sorted.c is:
  // u64 lsmGetU64(u8 *aOut);
  // We declare it here so the C++ test can call it.
  u64 lsmGetU64(u8 *aOut);
}

// Lightweight test harness: non-terminating assertions
static int gFailures = 0;
#define EXPECT_EQ(A, B) do {                                  \
    auto _a = (A);                                             \
    auto _b = (B);                                             \
    if(!(_a == _b)) {                                         \
        std::cerr << "EXPECT_EQ failed: " #A " == " #B "  ->  "  \
                  << "actual=" << static_cast<unsigned long long>(_a) \
                  << ", expected=" << static_cast<unsigned long long>(_b) \
                  << "  at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++gFailures;                                           \
    }                                                          \
} while(0)

static void runTest_AllZeros() {
  // Candidate Keywords: aOut[0..7], 8 bytes, zeroed input
  u8 a[8] = {0,0,0,0,0,0,0,0};
  u64 val = lsmGetU64(a);
  EXPECT_EQ(val, 0ULL);
}

static void runTest_SequentialBytes() {
  // Candidate Keywords: big-endian assembly, test known sequence
  // Input: [0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08]
  u8 a[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
  // Expected: 0x0102030405060708
  u64 expected = 0x0102030405060708ULL;
  u64 val = lsmGetU64(a);
  EXPECT_EQ(val, expected);
}

static void runTest_AllFF() {
  // Candidate Keywords: all bytes 0xFF
  u8 a[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
  u64 val = lsmGetU64(a);
  EXPECT_EQ(val, 0xFFFFFFFFFFFFFFFFULL);
}

static void runTest_MixedPattern() {
  // Candidate Keywords: mixed bytes to validate endianness
  // Input: [0xAA,0xBB,0xCC,0xDD,0xEE,0x11,0x22,0x33]
  u8 a[8] = {0xAA,0xBB,0xCC,0xDD,0xEE,0x11,0x22,0x33};
  // Expected: 0xAABBCCDDEE112233
  u64 expected = 0xAABBCCDDEE112233ULL;
  u64 val = lsmGetU64(a);
  EXPECT_EQ(val, expected);
}

static void runTest_HighBitMSB() {
  // Candidate Keywords: MSB set (0x80)
  // Input: [0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
  u8 a[8] = {0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  // Expected: 0x8000000000000000
  u64 expected = 0x8000000000000000ULL;
  u64 val = lsmGetU64(a);
  EXPECT_EQ(val, expected);
}

int main() {
  // Run all tests
  runTest_AllZeros();
  runTest_SequentialBytes();
  runTest_AllFF();
  runTest_MixedPattern();
  runTest_HighBitMSB();

  // Summary
  if(gFailures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cout << gFailures << " TEST(S) FAILED" << std::endl;
    return gFailures;
  }
}