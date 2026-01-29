/*
Unit Test Suite for the focal method Type_DateTime_Dup
Context:
- The focal method duplicates a memory block sized sizeof(struct tm) using _cmsDupMem.
- The test harness below provides a minimal, self-contained environment to exercise
  Type_DateTime_Dup without requiring the entire cms LCMS3/C/C++ project.
- We mock _cmsDupMem to verify:
  - The size passed to the duplication is exactly sizeof(struct tm).
  - The ContextID passed through is preserved.
  - The content of the duplicated memory matches the source when Ptr is valid.
  - Behavior when Ptr is NULL (results in zero-initialized memory, as per our mock).
- The tests are non-terminating (they log failures but continue), and we provide
  a simple test bootstrap via main().
*/

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <lcms2_internal.h>
#include <ctime>


// Domain-specific typedefs and macros (minimal stubs for testing)
typedef void* cmsContext;
typedef unsigned int cmsUInt32Number;
#define cmsUNUSED_PARAMETER(x) (void)(x)

// Fwd declaration of the focal struct used by the method under test
struct _cms_typehandler_struct {
    cmsContext ContextID;
};

// Global bookkeeping for the mock _cmsDupMem to enable assertions in tests
void* g_last_contextID = nullptr;
size_t g_last_size = 0;

// Mock implementation of _cmsDupMem used by Type_DateTime_Dup
extern "C" void* _cmsDupMem(cmsContext ContextID, const void* Ptr, size_t Size)
{
    // Record the context and size for assertions in tests
    g_last_contextID = ContextID;
    g_last_size = Size;

    // Allocate and copy if Ptr is valid; otherwise zero-initialize
    void* dst = malloc(Size);
    if (!dst) return nullptr;
    if (Ptr) {
        memcpy(dst, Ptr, Size);
    } else {
        memset(dst, 0, Size);
    }
    return dst;
}

// The focal method under test (reproduced here for a self-contained test)
void* Type_DateTime_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n)
{
{
    return _cmsDupMem(self ->ContextID, Ptr, sizeof(struct tm));
    cmsUNUSED_PARAMETER(n);
}
}

// Helper: simple test framework (non-terminating, lightweight)
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        fprintf(stderr, "Test failure: %s:%d: EXPECT_TRUE(%s) failed\n", __FILE__, __LINE__, #cond); \
        ++g_tests_failed; \
    } else { \
        ++g_tests_passed; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    ++g_tests_run; \
    if(!((a) == (b))) { \
        fprintf(stderr, "Test failure: %s:%d: EXPECT_EQ(%s, %s) failed (values: %ld vs %ld)\n", __FILE__, __LINE__, #a, #b, (long)(a), (long)(b)); \
        ++g_tests_failed; \
    } else { \
        ++g_tests_passed; \
    } \
} while(0)


// Test 1: Normal copy path
// - Prepare a valid tm struct and duplicate it. Expect exact byte-for-byte copy.
// - Also verify that the last_contextID and last_size reflect the call parameters.
void test_Type_DateTime_Dup_normal_copy()
{
    // Arrange
    struct _cms_typehandler_struct handler;
    handler.ContextID = (void*)0x12345678; // distinct context
    struct tm source;
    memset(&source, 0, sizeof(struct tm));
    source.tm_sec  = 12;
    source.tm_min  = 34;
    source.tm_hour = 5;
    source.tm_mday = 15;
    source.tm_mon  = 6;   // July (0-based in struct tm)
    source.tm_year = 121; // 2021
    // Act
    void* dup = Type_DateTime_Dup(&handler, &source, 99);
    // Assert
    EXPECT_TRUE(dup != nullptr);
    const struct tm* dup_tm = static_cast<const struct tm*>(dup);
    EXPECT_TRUE(memcmp(dup_tm, &source, sizeof(struct tm)) == 0);
    // Cleanup
    free(dup);
    // Verify that the mock recorded the expected context and size
    EXPECT_TRUE(g_last_contextID == handler.ContextID);
    EXPECT_EQ(g_last_size, sizeof(struct tm));
}

// Test 2: Ptr == NULL should yield zero-initialized memory of size sizeof(struct tm)
void test_Type_DateTime_Dup_null_ptr()
{
    // Arrange
    struct _cms_typehandler_struct handler;
    handler.ContextID = (void*)0xDEADBEEF;
    struct tm source;
    memset(&source, 0, sizeof(struct tm)); // source content is irrelevant here
    // Act
    void* dup = Type_DateTime_Dup(&handler, nullptr, 7);
    // Assert
    EXPECT_TRUE(dup != nullptr);
    const struct tm* dup_tm = static_cast<const struct tm*>(dup);
    // Expect all fields zero since Ptr was NULL and our mock zeros the memory
    struct tm zeros;
    memset(&zeros, 0, sizeof(struct tm));
    EXPECT_TRUE(memcmp(dup_tm, &zeros, sizeof(struct tm)) == 0);
    // Cleanup
    free(dup);
    // Verify context and size tracking
    EXPECT_TRUE(g_last_contextID == handler.ContextID);
    EXPECT_EQ(g_last_size, sizeof(struct tm));
}

// Test 3: ContextID can be NULL
void test_Type_DateTime_Dup_null_context()
{
    // Arrange
    struct _cms_typehandler_struct handler;
    handler.ContextID = nullptr;
    struct tm source;
    memset(&source, 0, sizeof(struct tm));
    source.tm_sec = 1;
    // Act
    void* dup = Type_DateTime_Dup(&handler, &source, 0);
    // Assert
    EXPECT_TRUE(dup != nullptr);
    const struct tm* dup_tm = static_cast<const struct tm*>(dup);
    EXPECT_TRUE(memcmp(dup_tm, &source, sizeof(struct tm)) == 0);
    // Cleanup
    free(dup);
    // Verify that ContextID was recorded as NULL
    EXPECT_TRUE(g_last_contextID == nullptr);
    EXPECT_EQ(g_last_size, sizeof(struct tm));
}

// Main: run all tests and report summary
int main()
{
    fprintf(stdout, "Starting Unit Tests for Type_DateTime_Dup (self-contained harness)\n");

    test_Type_DateTime_Dup_normal_copy();
    test_Type_DateTime_Dup_null_ptr();
    test_Type_DateTime_Dup_null_context();

    fprintf(stdout, "Tests run: %d, Passed: %d, Failed: %d\n",
            g_tests_run, g_tests_passed, g_tests_failed);

    if (g_tests_failed > 0) {
        fprintf(stderr, "Some tests FAILED. See log above for details.\n");
        return EXIT_FAILURE;
    }
    fprintf(stdout, "All tests PASSED.\n");
    return EXIT_SUCCESS;
}