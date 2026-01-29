// C++11 test harness for the focal method: Type_VideoSignal_Read
// This suite relies on the real LittleCMS-like environment present in the project.
// It overrides internal IO/read helpers to deterministically drive success and failure paths
// while exercising the Type_VideoSignal_Read implementation in cmstypes.c.
//
// Notes:
// - We do not use GoogleTest; instead we implement a lightweight test harness with
//   simple assertions that continue execution to maximize coverage.
// - We assume the public headers (e.g., lcms2.h) provide the necessary type names
//   (cmsIOHANDLER, cmsUInt32Number, cmsUInt8Number, cmsVideoSignalType, etc.).
// - Internal helpers (_cmsCalloc, _cmsFree, _cmsReadUInt8Number) are overridden here
//   via C linkage to control behavior for tests.

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include project headers that define core types used by the focal method.
// If the project layout uses these headers, this will provide the necessary types.
// If not, these typedefs will be provided by the test's environment.
                      // Also defines cmsVideoSignalType if available.


// Forward declaration of the focal function under test.
// We declare it with C linkage to match the project's symbol.
extern "C" void* Type_VideoSignal_Read(struct _cms_typehandler_struct* self,
                                      cmsIOHANDLER* io,
                                      cmsUInt32Number* nItems,
                                      cmsUInt32Number SizeOfTag);

// The real code uses an internal type _cms_typehandler_struct; we rely on the
// project's header to define it. If not, we provide a compatible forward declaration
// via the header below and reuse the project's definition.


// --------------------------------------------------------------------------------
// Test harness state to drive deterministic reads without depending on real IO.
// --------------------------------------------------------------------------------

// Global test data and read position, overridden by our mock _cmsReadUInt8Number.
static const cmsUInt8Number* g_test_data = nullptr;
static cmsUInt32Number g_test_len = 0;
static cmsUInt32Number g_test_pos = 0;

// Minimal override for internal allocator: use standard malloc/calloc.
// We route allocations to test-controlled memory so we can exercise
// allocation/failure scenarios as needed.
extern "C" void* _cmsCalloc(cmsContext ContextID, cmsUInt32Number n, cmsUInt32Number size)
{
    (void)ContextID; // Context is not relevant for test; keep API compatible
    return calloc(n, size);
}
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr)
{
    (void)ContextID;
    free(Ptr);
}

// Mock implementation of _cmsReadUInt8Number to drive the read behavior deterministically.
// The actual signature in the project is cmsBool _cmsReadUInt8Number(cmsIOHANDLER*, cmsUInt8Number*).
extern "C" cmsBool _cmsReadUInt8Number(cmsIOHANDLER* io, cmsUInt8Number* number)
{
    (void)io; // We do not model real IO; we drive behavior via global test data.

    if (g_test_data == nullptr) {
        return 0; // No data configured; simulate read failure
    }
    if (g_test_pos >= g_test_len) {
        return 0; // No more data; simulate read failure
    }
    *number = g_test_data[g_test_pos++];
    return 1; // Success
}

// --------------------------------------------------------------------------------
// Lightweight test harness helpers
// --------------------------------------------------------------------------------

static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        fprintf(stderr, "TEST FAILED: %s (in %s:%d)\n", msg, __FILE__, __LINE__); \
        ++g_tests_failed; \
    } else { \
        fprintf(stdout, "TEST PASSED: %s\n", msg); \
    } \
} while (0)


// --------------------------------------------------------------------------------
// Individual test cases
// --------------------------------------------------------------------------------

// Test 1: Success path - SizeOfTag == 4 and four bytes available.
// Expect non-NULL result, nItems == 1, and fields populated with input bytes.
void test_VideoSignal_Read_Success()
{
    // Arrange
    cmsUInt8Number input[4] = {1, 2, 3, 4};
    g_test_data = input;
    g_test_len = 4;
    g_test_pos = 0;

    struct _cms_typehandler_struct self;
    // ContextID is the first (and only) used member for this test path.
    // The actual library defines ContextID as cmsContext; we assume offset 0 here.
    // Some environments require initialization via an actual constructor; we rely on zero-initialization then set.
    memset(&self, 0, sizeof(self));
    self.ContextID = 0; // arbitrary

    cmsIOHANDLER io; // dummy; _cmsReadUInt8Number ignores it in our test
    cmsUInt32Number nItems = 0;

    // Act
    void* result = Type_VideoSignal_Read(&self, &io, &nItems, 4);

    // Assert
    TEST_ASSERT(result != nullptr, "VideoSignal_Read should succeed and return non-NULL pointer");
    TEST_ASSERT(nItems == 1, "VideoSignal_Read should set nItems to 1 on success");

    if (result != nullptr) {
        cmsVideoSignalType* v = (cmsVideoSignalType*)result;
        // Validate that the fields reflect the input bytes
        TEST_ASSERT(v->ColourPrimaries == 1, "ColourPrimaries should be 1");
        TEST_ASSERT(v->TransferCharacteristics == 2, "TransferCharacteristics should be 2");
        TEST_ASSERT(v->MatrixCoefficients == 3, "MatrixCoefficients should be 3");
        TEST_ASSERT(v->VideoFullRangeFlag == 4, "VideoFullRangeFlag should be 4");
        // Free allocated memory to avoid leaks in test environment
        _cmsFree(self.ContextID, v);
    }

    // Cleanup
    g_test_data = nullptr;
    g_test_len = 0;
    g_test_pos = 0;
}

// Test 2: Failure path - SizeOfTag != 4 should immediately return NULL and not allocate.
// We expect NULL result and no changes to nItems.
void test_VideoSignal_Read_WrongSize()
{
    // Arrange
    cmsUInt8Number input[4] = {9, 9, 9, 9}; // data is unused in this path
    g_test_data = input;
    g_test_len = 4;
    g_test_pos = 0;

    struct _cms_typehandler_struct self;
    memset(&self, 0, sizeof(self));
    self.ContextID = 0;

    cmsIOHANDLER io;
    cmsUInt32Number nItems = 0;

    // Act
    void* result = Type_VideoSignal_Read(&self, &io, &nItems, 3); // incorrect SizeOfTag

    // Assert
    TEST_ASSERT(result == nullptr, "VideoSignal_Read should return NULL when SizeOfTag != 4");
    TEST_ASSERT(nItems == 0, "nItems should remain unchanged on failure");
}

// Test 3: Failure path - first _cmsReadUInt8Number call fails (no data available).
void test_VideoSignal_Read_FailOnFirstRead()
{
    // Arrange: no data available
    g_test_data = nullptr;
    g_test_len = 0;
    g_test_pos = 0;

    struct _cms_typehandler_struct self;
    memset(&self, 0, sizeof(self));
    self.ContextID = 0;

    cmsIOHANDLER io;
    cmsUInt32Number nItems = 0;

    // Act
    void* result = Type_VideoSignal_Read(&self, &io, &nItems, 4);

    // Assert
    TEST_ASSERT(result == nullptr, "VideoSignal_Read should return NULL when first read fails (no data)");
}

// Test 4: Failure path - second read fails after first read succeeds.
void test_VideoSignal_Read_FailOnSecondRead()
{
    // Arrange: provide only enough data for the first read
    cmsUInt8Number input[1] = {7}; // only one byte available
    g_test_data = input;
    g_test_len = 1;
    g_test_pos = 0;

    struct _cms_typehandler_struct self;
    memset(&self, 0, sizeof(self));
    self.ContextID = 0;

    cmsIOHANDLER io;
    cmsUInt32Number nItems = 0;

    // Act
    void* result = Type_VideoSignal_Read(&self, &io, &nItems, 4);

    // Assert: after first read succeeds, second read will fail due to lack of data
    TEST_ASSERT(result == nullptr, "VideoSignal_Read should return NULL when second read fails");
}


// --------------------------------------------------------------------------------
// Main: run all tests and report summary
// --------------------------------------------------------------------------------

int main()
{
    fprintf(stdout, "Starting Type_VideoSignal_Read unit test suite (no GTest).\n");

    test_VideoSignal_Read_Success();
    test_VideoSignal_Read_WrongSize();
    test_VideoSignal_Read_FailOnFirstRead();
    test_VideoSignal_Read_FailOnSecondRead();

    fprintf(stdout, "Tests completed. Ran: %d, Passed: %d, Failed: %d\n",
            g_tests_run, g_tests_run - g_tests_failed, g_tests_failed);

    // Return non-zero if any test failed
    return g_tests_failed ? 1 : 0;
}