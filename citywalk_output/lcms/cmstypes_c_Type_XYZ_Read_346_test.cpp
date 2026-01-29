// Minimal C++11 test suite for cms Type_XYZ_Read in cmstypes.c
// This test uses function interposition via macro overrides to simulate
// allocations and parsing outcomes without requiring the full CMS IO backend.
// The test avoids terminating assertions and reports all findings.

#include <cstdio>
#include <iostream>
#include <cmstypes.c>
#include <cstdlib>
#include <lcms2_internal.h>


// Test control flags and counters
static int g_malloc_fail = 0;
static int g_read_xyz_fail = 0;
static int g_alloc_count = 0;
static int g_free_count = 0;

// Forward declarations of mocked helpers (signatures must match CMS internals)
static void* mock_cms_malloc_zero(cmsContext ContextID, cmsUInt32Number Size);
static void mock_cms_free(cmsContext ContextID, void* Ptr);
static cmsBool mock_cms_read_xyz_number(cmsIOHANDLER* io, cmsCIEXYZ* xyz);

// Hook macros to redirect internal CMS calls to our mocks
#define _cmsMallocZero(ContextID, Size) mock_cms_malloc_zero(ContextID, Size)
#define _cmsFree(ContextID, Ptr) mock_cms_free(ContextID, Ptr)
#define _cmsReadXYZNumber(io, xyz) mock_cms_read_xyz_number(io, xyz)

// Include the focal module with hooks in place
// The path "cmstypes.c" should be accessible from the include path.

// Implementations of the mocked internal CMS helpers
static void* mock_cms_malloc_zero(cmsContext ContextID, cmsUInt32Number Size)
{
    (void)ContextID; // avoid unused warning for static analysis
    if (g_malloc_fail) {
        return NULL;
    }
    ++g_alloc_count;
    void* p = std::calloc(1, Size);
    return p;
}

static void mock_cms_free(cmsContext ContextID, void* Ptr)
{
    (void)ContextID;
    if (Ptr != NULL) {
        std::free(Ptr);
    }
    ++g_free_count;
}

static cmsBool mock_cms_read_xyz_number(cmsIOHANDLER* io, cmsCIEXYZ* xyz)
{
    (void)io; // io is not used by this mock
    if (g_read_xyz_fail) {
        return cmsFALSE;
    }
    // Do not touch xyz contents; Type_XYZ_Read only uses the return value.
    return cmsTRUE;
}

// Lightweight test assertion macro that prints failures but does not abort tests
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "Test failure: " << (msg) << " (location: " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_tests_failed; \
    } \
} while (0)


// Helper to create a minimal CMS type handler instance for Type_XYZ_Read
static struct _cms_typehandler_struct* create_fake_self()
{
    // Allocate enough space for the focal type
    struct _cms_typehandler_struct* self = (struct _cms_typehandler_struct*) std::malloc(sizeof(struct _cms_typehandler_struct));
    if (self) {
        // The focal function only reads self->ContextID
        // Initialize to a benign value
        // Some CMS builds use ContextID as cmsContext; zero is typically valid
        // The actual field name is ContextID per focal code.
        #ifdef __clang__
        // If compiler warns about uninitialized struct layout, keep simple
        #endif
        // Best effort: initialize the ContextID if the field exists
        // Some builds may have different internal layout; if not, this write will be ignored.
        self->ContextID = (cmsContext)0;
    }
    return self;
}

// Helper to create a dummy IO handler for the tests
static cmsIOHANDLER* create_fake_io()
{
    cmsIOHANDLER* io = (cmsIOHANDLER*) std::malloc(sizeof(cmsIOHANDLER));
    if (io) {
        // Initialize to zeros; the mock ignores content
        std::memset(io, 0, sizeof(cmsIOHANDLER));
    }
    return io;
}

// Test case 1: Normal path - malloc succeeds and _cmsReadXYZNumber succeeds
static void test_xyz_read_success()
{
    g_malloc_fail = 0;
    g_read_xyz_fail = 0;
    g_alloc_count = 0;
    g_free_count = 0;

    struct _cms_typehandler_struct* self = create_fake_self();
    TEST_ASSERT(self != nullptr, "Failed to allocate fake self");

    cmsIOHANDLER* io = create_fake_io();
    TEST_ASSERT(io != nullptr, "Failed to allocate fake IO handler");

    cmsUInt32Number nItems = 0;
    void* res = Type_XYZ_Read(self, io, &nItems, 0);

    TEST_ASSERT(res != nullptr, "Type_XYZ_Read should return non-null on success path");
    TEST_ASSERT(nItems == 1, "Type_XYZ_Read should set nItems to 1 on success");

    // Free the allocated memory using the CMS API (which uses _cmsFree)
    if (res) {
        _cmsFree(self->ContextID, res);
    }

    // Cleanup
    std::free(self);
    std::free(io);

    // Expect exactly one allocation and one corresponding free
    TEST_ASSERT(g_alloc_count == 1, "Expected exactly one allocation on success");
    TEST_ASSERT(g_free_count == 1, "Expected exactly one deallocation after free");
}

// Test case 2: malloc failure path - _cmsMallocZero returns NULL
static void test_xyz_read_malloc_failure()
{
    g_malloc_fail = 1;
    g_read_xyz_fail = 0;
    g_alloc_count = 0;
    g_free_count = 0;

    struct _cms_typehandler_struct* self = create_fake_self();
    TEST_ASSERT(self != nullptr, "Failed to allocate fake self for malloc-failure test");

    cmsIOHANDLER* io = create_fake_io();
    TEST_ASSERT(io != nullptr, "Failed to allocate fake IO handler for malloc-failure test");

    cmsUInt32Number nItems = 0;
    void* res = Type_XYZ_Read(self, io, &nItems, 0);

    TEST_ASSERT(res == nullptr, "Type_XYZ_Read should return NULL when malloc fails");
    // No allocation should have occurred
    TEST_ASSERT(g_alloc_count == 0, "No allocation should occur on malloc failure");
    TEST_ASSERT(g_free_count == 0, "No memory should be freed on malloc failure");

    // Cleanup
    std::free(self);
    std::free(io);
}

// Test case 3: read XYZ number failure path - _cmsReadXYZNumber returns false
static void test_xyz_read_number_failure()
{
    g_malloc_fail = 0;
    g_read_xyz_fail = 1;
    g_alloc_count = 0;
    g_free_count = 0;

    struct _cms_typehandler_struct* self = create_fake_self();
    TEST_ASSERT(self != nullptr, "Failed to allocate fake self for read-failure test");

    cmsIOHANDLER* io = create_fake_io();
    TEST_ASSERT(io != nullptr, "Failed to allocate fake IO handler for read-failure test");

    cmsUInt32Number nItems = 0;
    void* res = Type_XYZ_Read(self, io, &nItems, 0);

    TEST_ASSERT(res == nullptr, "Type_XYZ_Read should return NULL when _cmsReadXYZNumber fails");

    // In this failure path, the function frees the allocated memory before returning NULL
    TEST_ASSERT(g_alloc_count == 1, "One allocation should occur before read failure");
    TEST_ASSERT(g_free_count == 1, "Memory should be freed inside Type_XYZ_Read on failure");

    // Cleanup
    std::free(self);
    std::free(io);
}

// Entry point
int main()
{
    std::cout << "Running Type_XYZ_Read unit tests (no GTest, minimal harness)" << std::endl;

    test_xyz_read_success();
    test_xyz_read_malloc_failure();
    test_xyz_read_number_failure();

    int total = g_tests_run;
    int failed = g_tests_failed;
    int passed = total - failed;

    std::cout << "Tests run: " << total << "\n";
    std::cout << "Passed: " << passed << "\n";
    std::cout << "Failed: " << failed << "\n";

    return (failed == 0) ? 0 : 1;
}