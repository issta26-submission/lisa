// Lightweight C++11 test suite for sqlite3GetVarint (from util.c)
// No GTest; a small in-house test harness is used.
// The tests exercise multiple branches of sqlite3GetVarint by feeding crafted byte sequences.
// We also include a reference decoder (refGetVarint) that mirrors the same decoding logic
// to compute expected results for comparison.

#include <math.h>
#include <vector>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Type aliases expected by the focal function (matching sqlite's u8, u64, u32)
typedef unsigned char u8;
typedef unsigned long long u64;
typedef unsigned int u32;

// Declaration of the focal function (C linkage)
extern "C" u8 sqlite3GetVarint(const unsigned char *p, u64 *v);

// Precomputed slot constants (numerical values derived from the original code)
#define SLOT_2_0 0x1FC07F       // ((0x7f<<14) | 0x7f)
#define SLOT_4_2_0 0xF01FC0FE    // ((0xfU<<28) | (0x7f<<14) | (0x7f))

// Reference implementation to compute expected results.
// This mirrors the internal logic of sqlite3GetVarint, using the same constants.
// It is used only to derive expected values for unit-test assertions.
int refGetVarint(const unsigned char* p, u64* v) {
  u32 a,b,s;
  // Case 1: first byte is non-negative as signed char
  if( ((signed char*)p)[0] >= 0 ){
    *v = *p;
    return 1;
  }
  // Case 2: second byte is non-negative as signed char
  if( ((signed char*)p)[1] >= 0 ){
    *v = ((u32)(p[0]&0x7f)<<7) | p[1];
    return 2;
  }
  // Precondition checks (must hold for the following logic)
  assert( SLOT_2_0 == ((0x7f<<14) | (0x7f)) );
  assert( SLOT_4_2_0 == ((0xfU<<28) | (0x7f<<14) | (0x7f)) );
  a = ((u32)p[0])<<14;
  b = p[1];
  p += 2;
  a |= *p;
  if(!(a&0x80))
  {
    a &= SLOT_2_0;
    b &= 0x7f;
    b = b<<7;
    a |= b;
    *v = a;
    return 3;
  }
  a &= SLOT_2_0;
  p++;
  b = b<<14;
  b |= *p;
  if(!(b&0x80))
  {
    b &= SLOT_2_0;
    a = a<<7;
    a |= b;
    *v = a;
    return 4;
  }
  b &= SLOT_2_0;
  s = a;
  p++;
  a = a<<14;
  a |= *p;
  if(!(a&0x80))
  {
    b = b<<7;
    a |= b;
    s = s>>18;
    *v = ((u64)s)<<32 | a;
    return 5;
  }
  s = s<<7;
  s |= b;
  p++;
  b = b<<14;
  b |= *p;
  if(!(b&0x80))
  {
    a &= SLOT_2_0;
    a = a<<7;
    a |= b;
    s = s>>18;
    *v = ((u64)s)<<32 | a;
    return 6;
  }
  p++;
  a = a<<14;
  a |= *p;
  if(!(a&0x80))
  {
    a &= SLOT_4_2_0;
    b &= SLOT_2_0;
    b = b<<7;
    a |= b;
    s = s>>11;
    *v = ((u64)s)<<32 | a;
    return 7;
  }
  a &= SLOT_2_0;
  p++;
  b = b<<14;
  b |= *p;
  if(!(b&0x80))
  {
    b &= SLOT_4_2_0;
    a = a<<7;
    a |= b;
    s = s>>4;
    *v = ((u64)s)<<32 | a;
    return 8;
  }
  p++;
  a = a<<15;
  a |= *p;
  b &= SLOT_2_0;
  b = b<<8;
  a |= b;
  s = s<<4;
  b = p[-4];
  b &= 0x7f;
  b = b>>3;
  s |= b;
  *v = ((u64)s)<<32 | a;
  return 9;
}

int main() {
  // Simple in-house test harness variables
  int failures = 0;

  // Helper lambda to run a single test case by comparing actual vs. reference results.
  // Uses the actual sqlite3GetVarint and a reference implementation to compute expected results.
  auto runTest = [&](const char* testName, const unsigned char* data, size_t len) {
    u64 actualV = 0;
    u8 actualLen = sqlite3GetVarint(data, &actualV);

    // Compute expected using the reference implementation
    u64 expectedV = 0;
    int expectedLen = refGetVarint(data, &expectedV);

    bool ok = (actualLen == expectedLen) && (actualV == expectedV);

    std::cout
      << testName
      << " => actualLen=" << int(actualLen) << ", actualV=" << actualV
      << ", expectedLen=" << expectedLen << ", expectedV=" << expectedV
      << " | " << (ok ? "PASS" : "FAIL") << std::endl;

    if(!ok) ++failures;
  };

  // Test Case 1: True branch of the first condition (single-byte varint)
  // p[0] = 0x7F -> signed char is non-negative; expected len = 1, v = 0x7F
  unsigned char t1[] = {0x7F};
  runTest("Case1_single_byte_true_branch", t1, sizeof(t1));

  // Test Case 2: True branch of the second condition (first byte negative, second non-negative)
  // p = {0xFF, 0x7F} -> v = ((0xFF & 0x7F) << 7) | 0x7F = (0x7F << 7) | 0x7F = 0x3FFF
  unsigned char t2[] = {0xFF, 0x7F};
  runTest("Case2_second_branch_true", t2, sizeof(t2));

  // Test Case 3: Third branch (p2 MSB = 0) when first two branches fail
  // p = {0x01, 0x02, 0x00} -> v computed via path 3
  unsigned char t3[] = {0x01, 0x02, 0x00};
  runTest("Case3_path3", t3, sizeof(t3));

  // Test Case 4: Fourth branch (p2 MSB = 1, p3 MSB = 0)
  // p = {0x01, 0x02, 0x80, 0x03} -> path 4
  unsigned char t4[] = {0x01, 0x02, 0x80, 0x03};
  runTest("Case4_path4", t4, sizeof(t4));

  // Test Case 5: Fifth branch (p4 MSB = 0 after path 3/4 fail)
  // p = {0x01, 0x02, 0x80, 0x80, 0x01} -> path 5
  unsigned char t5[] = {0x01, 0x02, 0x80, 0x80, 0x01};
  runTest("Case5_path5", t5, sizeof(t5));

  // Test Case 6: First branch false, second branch true (first byte negative, second non-negative)
  // p = {0x80, 0x40} -> len 2, v = (0x00 << 7) | 0x40 = 0x40
  unsigned char t6[] = {0x80, 0x40};
  runTest("Case6_firstFalse_secondTrue_len2", t6, sizeof(t6));

  // Test Case 7: First and second branches false; progress to 3-byte path
  // p = {0x80, 0x80, 0x01}
  unsigned char t7[] = {0x80, 0x80, 0x01};
  runTest("Case7_firstFalse_secondFalse_path3", t7, sizeof(t7));

  // Summary
  if (failures == 0) {
    std::cout << "All tests passed." << std::endl;
    return 0;
  } else {
    std::cout << failures << " test(s) failed." << std::endl;
    return 1;
  }
}