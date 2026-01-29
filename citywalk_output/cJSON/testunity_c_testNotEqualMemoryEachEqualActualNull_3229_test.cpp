/*
  Candidate Keywords summary (Step 1):
  - TEST_ASSERT_EACH_EQUAL_MEMORY
  - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
  - NULL
  - memory size (sizeof(type))
  - element count (4, 1, 0, etc.)
  - int, char data types
  - p0, p1 pointer usage
  - memory comparison semantics (content vs address)
  - Unity testing macros and test harness

  This file extends coverage around the focal testNotEqualMemoryEachEqualActualNull
  scenario by introducing additional type variations and edge cases to maximize
  execution paths for the underlying Unity TEST_ASSERT_EACH_EQUAL_MEMORY macro
  without depending on the GTest framework.
*/

#include <stddef.h>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Include Unity headers in a C linkage context for compatibility with C code.
// This file is intended to be compiled as C++11 compatible code, but uses the
// Unity C framework macros.
extern "C" {
}

// Test 1: NotEqualMemoryEachEqualActualNull with int array (4 elements) and NULL actual.
// Purpose: Ensure that when the actual memory pointer is NULL while a real memory
//          block is provided, the assertion fails (aborts) as expected.
void testNotEqualMemoryEachEqualActualNull_Int4(void)
{
    int p0[] = {1, 8, 987, -2};
    int* p1 = NULL;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(p0, p1, sizeof(int), 4);
    VERIFY_FAILS_END
}

// Test 2: NotEqualMemoryEachEqualActualNull with char array (4 elements) and NULL actual.
// Purpose: Same as Test 1 but for a different data type to ensure type-agnostic behavior.
void testNotEqualMemoryEachEqualActualNull_Char4(void)
{
    char p0[] = {'a', 'b', 'c', 'd'};
    char* p1 = NULL;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(p0, p1, sizeof(char), 4);
    VERIFY_FAILS_END
}

// Test 3: NotEqualMemoryEachEqualActualNull with a single int element and NULL actual.
// Purpose: Validate behavior for the smallest non-zero memory block.
void testNotEqualMemoryEachEqualActualNull_IntSingle(void)
{
    int p0 = 12345;
    int* p1 = NULL;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(&p0, p1, sizeof(int), 1);
    VERIFY_FAILS_END
}

// Test 4: Zero-length memory comparison with NULL actual should NOT abort.
// Purpose: Ensure that a zero-length comparison bypasses memory checks correctly.
void testNotEqualMemoryEachEqualActualNull_ZeroLength(void)
{
    int p0[] = {1, 2, 3};
    int* p1 = NULL;
    // No abort expected since length is zero.
    TEST_ASSERT_EACH_EQUAL_MEMORY(p0, p1, sizeof(int), 0);
}

// Lightweight main to run the above tests independently.
// Note: If integrated into an existing test runner, this main can be omitted
// to avoid multiple mains during linking.
int main(void)
{
    UnityBegin("testNotEqualMemoryEachEqualActualNull_Extensions");
    RUN_TEST(testNotEqualMemoryEachEqualActualNull_Int4);
    RUN_TEST(testNotEqualMemoryEachEqualActualNull_Char4);
    RUN_TEST(testNotEqualMemoryEachEqualActualNull_IntSingle);
    RUN_TEST(testNotEqualMemoryEachEqualActualNull_ZeroLength);
    return UnityEnd();
}