/*
   High-level unit test suite for Type_int_Read (focal method)
   - Target: Type_int_Read located in testplugin.c
   - Environment: C++11, no GoogleTest; a lightweight test harness is provided below.
   - Dependencies: testcms2.h (provides required CMS types and helper constructors)
   - Goals:
     - Cover true and false branches of each predicate in Type_int_Read
     - Use non-terminating assertions (EXPECT_*) and report a final summary
     - Do not rely on private methods/fields beyond what the public API/test harness exposes
   - Note:
     - The test relies on helper functions provided by testcms2.h (create a self with a valid/non-NULL ContextID,
       provide I/O buffers that succeed or fail reads, and proper cleanup).
     - Adjust the helper names if your test CMS harness exposes different entry points.
*/

#include <iostream>
#include <string>
#include <testcms2.h>


// Dependency providing CMS types and test harness helpers

// Entry point for the focal function (as declared in the project under test)
extern "C" void *Type_int_Read(struct _cms_typehandler_struct* self,
                              cmsIOHANDLER* io,
                              cmsUInt32Number* nItems,
                              cmsUInt32Number SizeOfTag);

// Optional cleanup function for the type handler; if not available in your harness, replace with appropriate free/cleanup.
extern "C" void Type_int_Free(struct _cms_typehandler_struct* self, void* Ptr);

// Optional helper: create a Type_Int handler with a valid/non-NULL ContextID
// If your harness provides a direct constructor, use that instead.
// The following two helpers are expectational; adapt names to your test harness if needed.
extern "C" struct _cms_typehandler_struct* CreateTypeIntHandlerWithContext(void* ctx);
extern "C" void DestroyTypeIntHandler(struct _cms_typehandler_struct* self);

// IO helpers (provided by test harness)
extern "C" cmsIOHANDLER* CreateBufferIO(const cmsUInt8Number* data, cmsUInt32Number size);
extern "C" void DestroyIO(cmsIOHANDLER* io);

// Minimal test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ << " - " << #cond << "\n"; \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_total_tests; \
    if((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << " - " << #a << " != " << #b \
                  << " (actual: " << (a) << " vs. " << (b) << ")\n"; \
        ++g_failed_tests; \
    } \
} while(0)

// Test 1: Success path - valid context, successful read, single item returned
static void test_Type_int_Read_Success(void)
{
    // Arrange
    void* ctx = reinterpret_cast<void*>(0xDEADBEEF); // non-NULL context
    struct _cms_typehandler_struct* self = CreateTypeIntHandlerWithContext(ctx);

    // Prepare a 4-byte little-endian payload representing the value 0x01020304
    cmsUInt8Number payload[4] = { 0x04, 0x03, 0x02, 0x01 };
    cmsIOHANDLER* io = CreateBufferIO(payload, 4);

    cmsUInt32Number nItems = 0;

    // Act
    void* Ptr = Type_int_Read(self, io, &nItems, sizeof(cmsUInt32Number));

    // Assert
    EXPECT_TRUE(Ptr != NULL);                 // true branch: memory allocated and read succeeded
    EXPECT_EQ(nItems, 1);                     // exactly one item read
    if (Ptr != NULL) {
        cmsUInt32Number value = *(cmsUInt32Number*)Ptr;
        EXPECT_EQ(value, 0x01020304);        // content matches payload (assuming little-endian interpretation)
    }

    // Cleanup
    if (Ptr != NULL) Type_int_Free(self, Ptr);
    DestroyIO(io);
    DestroyTypeIntHandler(self);
}

// Test 2: Failure path - malloc fails (ContextID triggers malloc failure in _cmsMalloc)
static void test_Type_int_Read_MallocFailure(void)
{
    // Arrange
    // Create a self with a NULL ContextID to force malloc failure in _cmsMalloc
    struct _cms_typehandler_struct* self = CreateTypeIntHandlerWithContext(nullptr);

    // IO with any data; since malloc will fail, read path won't be reached
    cmsUInt8Number dummy[4] = {0x00, 0x00, 0x00, 0x00};
    cmsIOHANDLER* io = CreateBufferIO(dummy, 4);

    cmsUInt32Number nItems = 0;

    // Act
    void* Ptr = Type_int_Read(self, io, &nItems, sizeof(cmsUInt32Number));

    // Assert
    EXPECT_TRUE(Ptr == NULL);                  // false branch: allocation failed
    // No need to check nItems or content in this path

    // Cleanup
    DestroyIO(io);
    DestroyTypeIntHandler(self);
}

// Test 3: Failure path - read function fails to populate 4-byte value
static void test_Type_int_Read_ReadFailure(void)
{
    // Arrange
    // Valid non-NULL context
    void* ctx = reinterpret_cast<void*>(0xBADC0DE);
    struct _cms_typehandler_struct* self = CreateTypeIntHandlerWithContext(ctx);

    // Prepare an IO that simulates a read failure (e.g., returns false)
    cmsUInt8Number dummy[4] = { 0x00, 0x00, 0x00, 0x00 };
    cmsIOHANDLER* io = CreateBufferIO(dummy, 4);

    cmsUInt32Number nItems = 0;

    // Act
    void* Ptr = Type_int_Read(self, io, &nItems, sizeof(cmsUInt32Number));

    // Assert
    EXPECT_TRUE(Ptr == NULL); // _cmsReadUInt32Number failed, Type_int_Read should return NULL

    // Cleanup
    DestroyIO(io);
    DestroyTypeIntHandler(self);
}

int main(void)
{
    std::cout << "Running Type_int_Read unit tests (C++11 harness)..." << std::endl;

    test_Type_int_Read_Success();
    test_Type_int_Read_MallocFailure();
    test_Type_int_Read_ReadFailure();

    std::cout << "Tests executed: " << g_total_tests
              << " | Failures: " << g_failed_tests << std::endl;

    // Non-zero return code indicates failure in test suite
    return (g_failed_tests != 0) ? 1 : 0;
}