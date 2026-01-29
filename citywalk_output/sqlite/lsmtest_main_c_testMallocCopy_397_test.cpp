// Test suite for the focal method: testMallocCopy
// Step 1 (Program Understanding) and Candidate Keywords:
// - Core dependencies: testMalloc, memcpy, pCopy, nByte, pRet, 8-byte header used by testMalloc to store the allocated size, memory layout, zero-init behavior of testMalloc, memory freeing strategy via base pointer.
// - Focus: verify that testMallocCopy correctly allocates memory via testMalloc(nByte),
//   copies exactly nByte bytes from pCopy into the newly allocated region, preserves
//   a zero-initialized header tail, and returns a distinct allocation (not aliasing pCopy).
// - Expected behaviors to cover: data copy accuracy, header value equals nByte, trailing bytes zeroed,
//   non-overlapping source/destination, handling of zero-byte copy, and large copies.
// - Note: This test suite is written in C++11 and links against the C implementations of testMalloc and testMallocCopy
//   provided in lsmtest_main.c (extern "C" linkage used for compatibility).

#include <lsmtest.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <cstring>
#include <sys/resource.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Forward declarations for the focal C functions (from the project under test)
extern "C" {
  void *testMalloc(int n);
  void *testMallocCopy(void *pCopy, int nByte);
}

// Lightweight, non-terminating test harness (no GTest). Collects failures and prints diagnostic messages.
// This aligns with Domain Knowledge guidance to use non-terminating assertions and a simple main() driver.
static int g_testCount = 0;
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
  ++g_testCount; \
  if(!(cond)) { \
    std::cerr << "[TEST_FAIL] " << __FILE__ << ":" << __LINE__ << ": " << (msg) << "\n"; \
    ++g_failures; \
  } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
  ++g_testCount; \
  auto _aa = (a); \
  auto _bb = (b); \
  if(!(_aa == _bb)) { \
    std::cerr << "[TEST_FAIL] " << __FILE__ << ":" << __LINE__ << ": " << (msg) \
              << " expected " << _bb << " got " << _aa << "\n"; \
    ++g_failures; \
  } \
} while(0)

using namespace std;

// Helper to safely free allocations returned by testMallocCopy' partner testMalloc
static void freeMallocBlock(void *pRet) {
  if(pRet){
    unsigned char *base = (unsigned char*)pRet - 8;
    free(base);
  }
}

/*
Test 1: BasicCopy verifies that testMallocCopy correctly copies a non-empty source buffer.
- Purpose: ensure the bytes copied into the newly allocated region match the source data.
- Additional checks:
  - The header (8 bytes before the returned pointer) stores the allocated size equal to nByte.
  - The 8-byte header region is zero-initialized by testMalloc and remains untouched by testMallocCopy.
  - The tail region (bytes after the copied data up to the end of the allocation) remains zero.
  - The returned pointer is distinct from the source pointer.
*/
static void testMallocCopy_basicCopy() {
  const int nByte = 10;
  unsigned char src[nByte];
  for(int i = 0; i < nByte; ++i) src[i] = (unsigned char)(i + 1);

  void *pRet = testMallocCopy((void*)src, nByte);
  EXPECT_TRUE(pRet != nullptr, "testMallocCopy returned null");

  unsigned char *dst = (unsigned char*)pRet;

  // Validate copied data
  for(int i = 0; i < nByte; ++i){
    EXPECT_EQ((int)dst[i], (int)src[i], "Copied byte mismatch at position i");
  }

  // Validate header stores nByte (the allocation size for the copied region)
  int headerVal = *(int*)(((unsigned char*)pRet) - 8);
  EXPECT_EQ(headerVal, nByte, "Header should store allocation size equals nByte");

  // Validate tail region remains zero (8 bytes at the end)
  for(int i = 0; i < 8; ++i){
    EXPECT_EQ((int)dst[nByte + i], 0, "Tail region not zero-initialized");
  }

  // Validate pCopy and pRet are distinct
  EXPECT_TRUE(src != pRet, "Source and destination should be distinct pointers");

  // Cleanup
  freeMallocBlock(pRet);
}

/*
Test 2: Zero-length copy ensures that a copy of 0 bytes does not dereference pCopy
and still returns a valid allocation with a header value of 0 and zero tail.
- Purpose: exercise the boundary nByte == 0 path.
- Note: We pass a small dummy buffer to guarantee a valid source pointer for 0-byte copy.
*/
static void testMallocCopy_zeroLength() {
  unsigned char dummy[1] = {0};
  void *pRet = testMallocCopy((void*)dummy, 0);
  EXPECT_TRUE(pRet != nullptr, "testMallocCopy with nByte=0 returned null");

  unsigned char *dst = (unsigned char*)pRet;
  int headerVal = *(int*)(((unsigned char*)pRet) - 8);
  EXPECT_EQ(headerVal, 0, "Header should store 0 for nByte=0");

  // All bytes in the allocated 0+8 region should be zero
  for(int i = 0; i < 8; ++i){
    EXPECT_EQ((int)dst[i], 0, "Memory should remain zero for 0-byte copy");
  }

  freeMallocBlock(pRet);
}

/*
Test 3: Large-copy verifies correctness for a reasonably large data size
and that memory layout invariants still hold for bigger allocations.
- Purpose: ensure no overflow or miscopy occurs across larger buffers.
*/
static void testMallocCopy_largeCopy() {
  const int nByte = 256;
  unsigned char src[nByte];
  for(int i = 0; i < nByte; ++i) src[i] = (unsigned char)(i * 3 + 7);

  void *pRet = testMallocCopy((void*)src, nByte);
  EXPECT_TRUE(pRet != nullptr, "testMallocCopy large copy returned null");

  unsigned char *dst = (unsigned char*)pRet;
  for(int i = 0; i < nByte; ++i){
    EXPECT_EQ((int)dst[i], (int)src[i], "Large copy mismatch at position i");
  }

  int headerVal = *(int*)(((unsigned char*)pRet) - 8);
  EXPECT_EQ(headerVal, nByte, "Header should store allocation size equals nByte for large copy");

  // Tail bytes should be zero (8 bytes at end)
  for(int i = 0; i < 8; ++i){
    EXPECT_EQ((int)dst[nByte + i], 0, "Tail region not zero-initialized for large copy");
  }

  freeMallocBlock(pRet);
}

/*
Test 4: Copy from a source that includes varied patterns to ensure no sign extension or corruption occurs.
- Purpose: validate integrity of bytes with non-trivial patterns.
*/
static void testMallocCopy_patternCopy() {
  const int nByte = 64;
  unsigned char src[nByte];
  for(int i = 0; i < nByte; ++i){
    src[i] = (unsigned char)((i * 17) & 0xFF);
  }

  void *pRet = testMallocCopy((void*)src, nByte);
  EXPECT_TRUE(pRet != nullptr, "Pattern copy: returned null");

  unsigned char *dst = (unsigned char*)pRet;
  for(int i = 0; i < nByte; ++i){
    EXPECT_EQ((int)dst[i], (int)src[i], "Pattern copy mismatch at position i");
  }

  int headerVal = *(int*)(((unsigned char*)pRet) - 8);
  EXPECT_EQ(headerVal, nByte, "Header should store nByte for pattern copy");

  for(int i = 0; i < 8; ++i){
    EXPECT_EQ((int)dst[nByte + i], 0, "Tail region not zero-initialized for pattern copy");
  }

  freeMallocBlock(pRet);
}

/*
Main driver: runs all tests and reports aggregate results.
According to Domain Knowledge, we should expose tests via main() and avoid external test runners.
*/
int main() {
  std::cout << "Running testMallocCopy unit tests (C++11 harness)..." << std::endl;

  testMallocCopy_basicCopy();
  testMallocCopy_zeroLength();
  testMallocCopy_largeCopy();
  testMallocCopy_patternCopy();

  if(g_failures){
    std::cerr << "Total assertions: " << g_testCount
              << ", Failures: " << g_failures << std::endl;
    return 1;
  }else{
    std::cout << "All tests passed. Total assertions: " << g_testCount << std::endl;
    return 0;
  }
}