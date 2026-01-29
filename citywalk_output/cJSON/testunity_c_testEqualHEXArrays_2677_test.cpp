// Test suite: Focused validation for the focal method testEqualHEXArrays
// This suite exercises the HEX array equality assertions used by the focal test.
// It is designed for C++11 compilation, without using GTest, and relies on Unity.
// The tests cover both passing scenarios and expected-failure branches via Unity's
// EXPECT_ABORT_BEGIN / VERIFY_FAILS_END constructs.
//
// Step 1 (Candidate Keywords):
// - UNITY_UINT32, p0, p1, p2, p3
// - TEST_ASSERT_EQUAL_HEX_ARRAY, TEST_ASSERT_EQUAL_HEX32_ARRAY
// - Hex-array equality semantics, with lengths 1, 4, 3
// - Explicit positive and negative test cases for equality of arrays
// - Macros for abortable tests: EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
// - Static linking of Unity requires a minimal Putchar spy setup (stubbed here)

/* Domain knowledge notes (embedded as comments for clarity)
   - Import dependencies: include unity.h (and necessary standard headers)
   - Test both true and false branches of each predicate at least once
   - Static members in the focal file are not directly tested here; we rely on Unity
     to validate behavior through macros
   - Use only standard library and Unity-provided methods
   - Use non-terminating assertions where possible to maximize coverage
   - Avoid private/internal API access; tests operate on public Unity API
   - For tests that must abort, use EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
   - If using C++, ensure correct linkage when including C headers (extern "C" wrapper)
*/

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

#ifdef __cplusplus
}
#endif

// Forward declarations for test wrappers (to be used by the Unity runner)
void test_equal_hex_arrays_basic(void);
void test_not_equal_hex_arrays_last_element(void);
void test_not_equal_hex_arrays_first_element(void);
void test_not_equal_hex_arrays_third_element(void);

// Minimal Putchar spy stubs to satisfy Unity abort-helpers (no-ops for test purposes)
void startPutcharSpy(void) { /* no-op stub for test harness */ }
void endPutcharSpy(void)   { /* no-op stub for test harness */ }
char* getBufferPutcharSpy(void) { return nullptr; }

// Empty setUp/tearDown to satisfy Unity's expectations (no per-test state needed)
void setUp(void)  { }
void tearDown(void) { }

// Test 1: Validate that the exact same arrays are reported equal across
// multiple length parameters and between different-typed macros.
// This mirrors the original focal method's positive path.
void test_equal_hex_arrays_basic(void)
{
    // Candidate Keywords mapping:
    // - p0, p1, p2, p3 are UNITY_UINT32 arrays
    // - p0 == p0 for length 1
    // - p0 == p0 for length 4
    // - p0 == p1 for length 4 (identical content across both arrays)
    // - p0 == p2 for length 3 (first three elements equal)
    // - p0 == p3 for length 1 (first element equal)
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p2[] = {1, 8, 987, 2};
    UNITY_UINT32 p3[] = {1, 500, 600, 700};

    // Verify equality with varying lengths and macros
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p0, 1);       // first element equality
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p0, 4);       // entire array equality (4 elements)
    TEST_ASSERT_EQUAL_HEX_ARRAY(p0, p1, 4);       // identical arrays
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p2, 3);     // first 3 elements equal
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p3, 1);     // only first element checked
}

// Test 2: Negative case - last element mismatch should abort the test
// to validate that the failure path is properly detected by Unity.
void test_not_equal_hex_arrays_last_element(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 987, 65131u}; // last element differs

    // Expect the assertion to fail and abort the test, which we catch
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Test 3: Negative case - first element mismatch should abort the test
void test_not_equal_hex_arrays_first_element(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {2, 8, 987, 65132u}; // first element differs

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Test 4: Negative case - third element mismatch should abort the test
void test_not_equal_hex_arrays_third_element(void)
{
    UNITY_UINT32 p0[] = {1, 8, 987, 65132u};
    UNITY_UINT32 p1[] = {1, 8, 986, 65132u}; // third element differs (987 vs 986)

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_HEX32_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
}

// Main runner: executes the focused test suite
int main(void)
{
    // Initialize Unity test framework
    UNITY_BEGIN();

    // Run the focused tests for testEqualHEXArrays and related negative tests
    RUN_TEST(test_equal_hex_arrays_basic);
    RUN_TEST(test_not_equal_hex_arrays_last_element);
    RUN_TEST(test_not_equal_hex_arrays_first_element);
    RUN_TEST(test_not_equal_hex_arrays_third_element);

    // Finish Unity and return result
    return UNITY_END();
}