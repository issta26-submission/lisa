/*
Candidate Keywords:
- Type_U16Fixed16_Dup: focal method under test
- _cmsDupMem: dependency used to duplicate memory
- cmsUInt32Number: size parameter type
- cmsFloat64Number: element type (double)
- cmsContext and self->ContextID: context propagation mechanism
- _cms_typehandler_struct: structure containing ContextID
- Ptr: input pointer to be duplicated
- n: number of elements to duplicate
- Memory duplication semantics: verify that bytes are copied correctly
- Test harness: non-GTest, standalone main-based test
*/

/* Minimal standalone C++11 test harness for Type_U16Fixed16_Dup
   This test provides lightweight stubs for required CMS types and
   the _cmsDupMem dependency so the focal function can be exercised
   in isolation without the rest of the cmsLibrary sources.

   The tests cover:
   - Basic copy: ensure exact memory copy for non-zero n
   - Zero elements: ensure NULL is returned when n == 0
   - Null source pointer: ensure zero-initialized memory is returned when Ptr == NULL
   - Multiple context IDs: ensure self->ContextID is passed through (no functional effect in stub)
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Domain-specific typedefs (lightweight stubs for testing)
typedef void* cmsContext;
typedef unsigned int cmsUInt32Number;
typedef double cmsFloat64Number;
typedef int cmsBool;

// Focal class/struct used by Type_U16Fixed16_Dup
struct _cms_typehandler_struct {
    cmsContext ContextID;
};

// Forward declaration of the focal function under test
void* Type_U16Fixed16_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// Dependency stub: _cmsDupMem
// Duplicates a memory region of given byte size. If Ptr is NULL, returns a zero-initialized buffer.
void* _cmsDupMem(cmsContext ContextID, const void* Ptr, cmsUInt32Number Size)
{
    (void)ContextID; // Context not used by stub logic, but kept to reflect interface
    if (Size == 0) return NULL;

    void* dst = std::malloc(Size);
    if (!dst) return NULL;

    if (Ptr)
        std::memcpy(dst, Ptr, Size);
    else
        std::memset(dst, 0, Size);

    return dst;
}

// Implementation of the focal method (as provided in the prompt)
void* Type_U16Fixed16_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n)
{
{
    return _cmsDupMem(self ->ContextID, Ptr, n * sizeof(cmsFloat64Number));
}
}

// Simple non-terminating test assertion helpers
static int g_failures = 0;

#define CHECK_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "CHECK FAILED: " << (msg) << " | Condition: " #cond << std::endl; \
            ++g_failures; \
        } \
    } while(0)

#define CHECK_EQ(a, b, msg) \
    do { \
        if(!((a) == (b))) { \
            std::cerr << "CHECK FAILED: " << (msg) \
                      << " | Expected: " << (b) << " Got: " << (a) << std::endl; \
            ++g_failures; \
        } \
    } while(0)

// Test 1: Basic copy of 3 cmsFloat64Number values
void test_Type_U16Fixed16_Dup_BasicCopy()
{
    // Arrange
    cmsFloat64Number src[3] = {1.1, 2.2, 3.3};
    struct _cms_typehandler_struct handler;
    handler.ContextID = (cmsContext)0xDEADBEEF;

    // Act
    void* dupPtr = Type_U16Fixed16_Dup(&handler, src, 3);

    // Assert
    CHECK_TRUE(dupPtr != NULL, "Dup returned non-null pointer for non-zero n");
    cmsFloat64Number* dup = static_cast<cmsFloat64Number*>(dupPtr);
    // Compare memory content exactly
    bool contentMatches = std::memcmp(dup, src, 3 * sizeof(cmsFloat64Number)) == 0;
    CHECK_TRUE(contentMatches, "Duplicated memory contents should match source data");

    // Cleanup
    std::free(dupPtr);
}

// Test 2: Zero elements (n == 0) should return NULL (or equivalent empty allocation)
void test_Type_U16Fixed16_Dup_ZeroElements()
{
    // Arrange
    cmsFloat64Number src[3] = {5.0, 6.0, 7.0};
    struct _cms_typehandler_struct handler;
    handler.ContextID = (cmsContext)0x12345678;

    // Act
    void* dupPtr = Type_U16Fixed16_Dup(&handler, src, 0);

    // Assert
    CHECK_TRUE(dupPtr == NULL, "Dup should return NULL when n == 0");

    // No cleanup needed
}

// Test 3: Ptr == NULL should yield zero-initialized memory of requested size
void test_Type_U16Fixed16_Dup_NullPtr()
{
    // Arrange
    struct _cms_typehandler_struct handler;
    handler.ContextID = (cmsContext)0xCAFEBABE;

    // Act
    void* dupPtr = Type_U16Fixed16_Dup(&handler, NULL, 4); // n=4 elements

    // Assert
    CHECK_TRUE(dupPtr != NULL, "Dup should allocate memory even when Ptr is NULL");
    cmsFloat64Number* dup = static_cast<cmsFloat64Number*>(dupPtr);
    // All bytes should be zeroes (since Ptr == NULL in stub)
    bool allZero = true;
    for (size_t i = 0; i < 4; ++i) {
        if (dup[i] != 0.0) {
            allZero = false;
            break;
        }
    }
    CHECK_TRUE(allZero, "Duplicated memory should be zero-initialized when Ptr is NULL");

    // Cleanup
    std::free(dupPtr);
}

// Test 4: Validate behavior with multiple context IDs (contexts are passed through to _cmsDupMem)
void test_Type_U16Fixed16_Dup_MultipleContexts()
{
    // Arrange
    cmsFloat64Number src[2] = {42.0, 24.0};
    struct _cms_typehandler_struct handler1;
    handler1.ContextID = (cmsContext)0x11111111;
    struct _cms_typehandler_struct handler2;
    handler2.ContextID = (cmsContext)0x22222222;

    // Act
    void* dupPtr1 = Type_U16Fixed16_Dup(&handler1, src, 2);
    void* dupPtr2 = Type_U16Fixed16_Dup(&handler2, src, 2);

    // Assert
    CHECK_TRUE(dupPtr1 != NULL && dupPtr2 != NULL, "Dup should succeed for both contexts");
    cmsFloat64Number* d1 = static_cast<cmsFloat64Number*>(dupPtr1);
    cmsFloat64Number* d2 = static_cast<cmsFloat64Number*>(dupPtr2);
    bool sameContents = (std::memcmp(d1, src, 2 * sizeof(cmsFloat64Number)) == 0) &&
                        (std::memcmp(d2, src, 2 * sizeof(cmsFloat64Number)) == 0);
    CHECK_TRUE(sameContents, "Duplicated memory should match source data for both contexts");

    // Cleanup
    std::free(dupPtr1);
    std::free(dupPtr2);
}

// Simple runner to execute all tests
int main()
{
    std::cout << "Running tests for Type_U16Fixed16_Dup (standalone harness)" << std::endl;

    test_Type_U16Fixed16_Dup_BasicCopy();
    test_Type_U16Fixed16_Dup_ZeroElements();
    test_Type_U16Fixed16_Dup_NullPtr();
    test_Type_U16Fixed16_Dup_MultipleContexts();

    if (g_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "Tests completed with " << g_failures << " failure(s)." << std::endl;
        return 1;
    }
}