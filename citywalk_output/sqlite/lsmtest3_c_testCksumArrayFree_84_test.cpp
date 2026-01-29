/*
  Unit test suite for the focal method testCksumArrayFree(CksumDb *p)
  - Target: Ensure basic correctness and safe behavior when azCksum is NULL and non-NULL.
  - Approach: No Google Test; a lightweight C++11 test harness is used that calls the C function directly.
  - Notes: This test suite relies on the C declarations provided in "lsmtest.h".
*/

/* Include standard headers for memory management and I/O */
#include <cstdlib>
#include <cstdio>
#include <lsmtest.h>
#include <cstring>


/* Include project headers to obtain CksumDb type and the focal function prototype */

extern "C" void testCksumArrayFree(CksumDb *p); // Focal method (defined in C)

static int g_failures = 0; // Global counter for test failures

// Lightweight assertion helpers (non-terminating)
#define ASSERTPRINT(...) do { fprintf(stderr, __VA_ARGS__); } while(0)
#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    ASSERTPRINT("EXPECT_TRUE failed: %s (in %s:%d)\n", #cond, __FILE__, __LINE__); \
    ++g_failures; \
  } \
} while(0)
#define EXPECT_NOT_NULL(ptr) do { \
  if((ptr) == nullptr) { \
    ASSERTPRINT("EXPECT_NOT_NULL failed: %s is null (in %s:%d)\n", #ptr, __FILE__, __LINE__); \
    ++g_failures; \
  } \
} while(0)

/*
  Test 1: Normal usage
  - Allocate a CksumDb object on the heap.
  - Allocate azCksum and fill with some bytes.
  - Call testCksumArrayFree(p) and ensure no crash occurs.
  - This exercises the typical non-null azCksum path.
*/
static void testCksumArrayFree_Normal()
{
  // Allocate the focal structure
  CksumDb *p = (CksumDb*)malloc(sizeof(CksumDb));
  EXPECT_NOT_NULL(p);
  if(p){
    // Initialize to a known state
    memset(p, 0xAA, sizeof(CksumDb));

    // Allocate azCksum to simulate a typical use-case
    p->azCksum = (char*)malloc(32);
    EXPECT_NOT_NULL(p->azCksum);
    if(p->azCksum){
      memset(p->azCksum, 0xBB, 32);
    }

    // Invoke the focal function
    testCksumArrayFree(p);

    // After this call, both azCksum and p are freed by the focal function.
    // We do not access freed memory to avoid undefined behavior.
  }
}

/*
  Test 2: azCksum is NULL
  - Allocate a CksumDb object on the heap.
  - Explicitly set azCksum to NULL to exercise the null-path for the azCksum free.
  - Call testCksumArrayFree(p) and ensure no crash occurs.
  - This covers the true/false nature of the azCksum being NULL.
*/
static void testCksumArrayFree_NullAz()
{
  // Allocate the focal structure
  CksumDb *p = (CksumDb*)malloc(sizeof(CksumDb));
  EXPECT_NOT_NULL(p);
  if(p){
    // Initialize to a known state and set azCksum to NULL
    memset(p, 0, sizeof(CksumDb));
    p->azCksum = NULL;

    // Invoke the focal function
    testCksumArrayFree(p);

    // As before, memory has been freed inside the focal function.
  }
}

/*
  Entry point: Run all tests and report summary.
  - The tests are invoked from main as required when gtest is not used.
  - We ensure the program continues execution even if some expectations fail.
*/
int main()
{
  // Run tests
  testCksumArrayFree_Normal();
  testCksumArrayFree_NullAz();

  // Report results
  if(g_failures > 0){
    std::printf("TEST SUMMARY: %d failure(s) detected.\n", g_failures);
  } else {
    std::printf("TEST SUMMARY: All tests passed.\n");
  }

  // Return number of failures to integrate with simple CI tooling if desired
  return g_failures;
}