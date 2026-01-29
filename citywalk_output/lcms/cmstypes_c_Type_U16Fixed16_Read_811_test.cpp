// This is a standalone C++11 test harness for the focal function Type_U16Fixed16_Read.
// It provides minimal stubs for the production dependencies so the function can be exercised
// in isolation without relying on the full cms libraries.
// The tests cover typical success paths, boundary cases, and failure paths.
// Target: C++11 (no GTest). A lightweight in-file test framework is used.

#include <cstdio>
#include <cstdint>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cmath>


// Domain knowledge and test strategy notes (embedded as comments for traceability):
// - Candidate Keywords: Type_U16Fixed16_Read, _cmsReadUInt32Number, _cmsCalloc, _cmsFree,
//   cmsIOHANDLER, cmsUInt32Number, cmsFloat64Number, cmsContext, _cms_typehandler_struct
// - The test interacts with a minimal, in-source reimplementation of the dependencies:
//   - _cmsReadUInt32Number(io, &v): reads next 32-bit value from io's buffer
//   - _cmsCalloc(ContextID, Count, Size): allocates memory (handles Count == 0 gracefully)
//   - _cmsFree(ContextID, Ptr): frees memory
// - Type_U16Fixed16_Read reads Count = SizeOfTag / sizeof(uint32) 32-bit numbers,
//   converts each to double by dividing by 65536.0, and returns an allocated array of doubles.
// - The test uses a fake cmsIOHANDLER with a simple in-memory buffer and a pointer index.

// Typedefs to mirror the production types used by the focal function
typedef uint32_t cmsUInt32Number;
typedef double   cmsFloat64Number;
typedef void*    cmsContext;

// Minimal in-source type to satisfy the focal function's signature
struct _cms_typehandler_struct {
    cmsContext ContextID;
};

// Minimal in-source IO handler used by the focal function
typedef struct _cmsIOHANDLER {
    cmsUInt32Number* data;  // pointer to in-memory 32-bit values
    size_t count;             // total number of 32-bit values available
    size_t pos;               // current read position (index into data)
} cmsIOHANDLER;

// Forward declarations of internal helpers used by Type_U16Fixed16_Read (re-implemented for test)
static void*  _cmsCalloc(cmsContext ContextID, cmsUInt32Number Count, cmsUInt32Number Size);
static void   _cmsFree(cmsContext ContextID, void* ptr);
static bool   _cmsReadUInt32Number(cmsIOHANDLER* io, cmsUInt32Number* v);

// Implementation of the focal method under test (adapted for test in a single translation unit)
extern "C" void *Type_U16Fixed16_Read(struct _cms_typehandler_struct* self,
                                    cmsIOHANDLER* io,
                                    cmsUInt32Number* nItems,
                                    cmsUInt32Number SizeOfTag)
{
{
    cmsFloat64Number*  array_double;
    cmsUInt32Number v;
    cmsUInt32Number i, n;
    *nItems = 0;
    n = SizeOfTag / sizeof(cmsUInt32Number);
    array_double = (cmsFloat64Number*) _cmsCalloc(self ->ContextID, n, sizeof(cmsFloat64Number));
    if (array_double == NULL) return NULL;
    for (i=0; i < n; i++) {
        if (!_cmsReadUInt32Number(io, &v)) {
            _cmsFree(self ->ContextID, (void*) array_double);
            return NULL;
        }
        // Convert to cmsFloat64Number
        array_double[i] =  (cmsFloat64Number) (v / 65536.0);
    }
    *nItems = n;
    return (void*) array_double;
}
}

// Minimal implementations of the internal helpers (test harness)
static void* _cmsCalloc(cmsContext ContextID, cmsUInt32Number Count, cmsUInt32Number Size)
{
    // If Count == 0, allocate a tiny non-null block to avoid null-pointer issues in zero-sized cases.
    if (Count == 0) {
        // allocate at least 1 element to return non-null in zero-element scenarios
        return std::calloc(1, (size_t)Size ? (size_t)Size : 1);
    }
    return std::calloc((size_t)Count, (size_t)Size);
}

static void _cmsFree(cmsContext ContextID, void* ptr)
{
    if (ptr != nullptr) {
        std::free(ptr);
    }
}

static bool _cmsReadUInt32Number(cmsIOHANDLER* io, cmsUInt32Number* v)
{
    if (io == nullptr) return false;
    if (io->pos >= io->count) return false;
    *v = io->data[io->pos++];
    return true;
}

// Lightweight test framework (no external libraries)
static int g_total = 0;
static int g_failed = 0;

#define ASSERT_TRUE(cond) \
    do { \
        ++g_total; \
        if (!(cond)) { \
            ++g_failed; \
            std::fprintf(stderr, "Assertion failed: %s, at %s:%d\n", #cond, __FILE__, __LINE__); \
        } \
    } while (0)

#define ASSERT_EQ(val1, val2) \
    do { \
        ++g_total; \
        if (!((val1) == (val2))) { \
            ++g_failed; \
            std::fprintf(stderr, "Assertion failed: %s == %s (actual: %lld, expected: %lld), at %s:%d\n", #val1, #val2, (long long)(val1), (long long)(val2), __FILE__, __LINE__); \
        } \
    } while (0)

#define ASSERT_PTR_NOT_NULL(p) \
    do { \
        ++g_total; \
        if ((p) == nullptr) { \
            ++g_failed; \
            std::fprintf(stderr, "Assertion failed: %s is not null, at %s:%d\n", #p, __FILE__, __LINE__); \
        } \
    } while (0)

#define ASSERT_PTR_NULL(p) \
    do { \
        ++g_total; \
        if ((p) != nullptr) { \
            ++g_failed; \
            std::fprintf(stderr, "Assertion failed: %s is null, at %s:%d\n", #p, __FILE__, __LINE__); \
        } \
    } while (0)


// Test 1: Basic read of 4 U16Fixed16 values -> expect [0.0, 1.0, 2.0, 3.0]
static void test_U16Fixed16_Read_basic4()
{
    // Candidate Keywords: Type_U16Fixed16_Read, memory allocation, in-memory IO
    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0x01;

    uint32_t data[] = { 0, 65536, 131072, 196608 }; // 0, 1, 2, 3 after division by 65536
    cmsIOHANDLER io;
    io.data = data;
    io.count = 4;
    io.pos = 0;

    cmsUInt32Number nItems = 0;
    void* result = Type_U16Fixed16_Read(&self, &io, &nItems, sizeof(data));

    ASSERT_PTR_NOT_NULL(result);
    ASSERT_EQ(nItems, (cmsUInt32Number)4);

    cmsFloat64Number* arr = (cmsFloat64Number*) result;
    ASSERT_EQ(arr[0], 0.0);
    ASSERT_EQ(arr[1], 1.0);
    ASSERT_EQ(arr[2], 2.0);
    ASSERT_EQ(arr[3], 3.0);

    // Free the allocated memory
    _cmsFree(self.ContextID, result);
}

// Test 2: Incomplete read should fail and return NULL
static void test_U16Fixed16_Read_incomplete()
{
    // Candidate Keywords: Type_U16Fixed16_Read, failure path, cleanup on error
    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0x02;

    uint32_t data[] = { 0, 65536 }; // Only 2 values available; need 4 for 4 items
    cmsIOHANDLER io;
    io.data = data;
    io.count = 2;
    io.pos = 0;

    cmsUInt32Number nItems = 0;
    void* result = Type_U16Fixed16_Read(&self, &io, &nItems, sizeof(data) * 2); // SizeOfTag intentionally large enough for 2 elements

    // Expect NULL due to insufficient data
    ASSERT_PTR_NULL(result);
    // nItems should remain 0 on failure
    ASSERT_EQ(nItems, (cmsUInt32Number)0);
}

// Test 3: Zero-sized input (SizeOfTag == 0) should return non-null pointer with nItems == 0
static void test_U16Fixed16_Read_zero_items()
{
    // Candidate Keywords: Type_U16Fixed16_Read, zero-length handling
    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0x03;

    cmsUInt32Number nItems = 0;
    cmsIOHANDLER io;
    io.data = nullptr;
    io.count = 0;
    io.pos = 0;

    cmsUInt32Number sz = 0; // SizeOfTag = 0
    void* result = Type_U16Fixed16_Read(&self, &io, &nItems, sz);

    ASSERT_PTR_NOT_NULL(result);
    ASSERT_EQ(nItems, (cmsUInt32Number)0);

    // Free the allocated memory (even though it's zero items, a non-null pointer may be returned)
    _cmsFree(self.ContextID, result);
}

// Test 4: Read 5 items with non-multiple-of-4 impact (SizeOfTag corresponds to 5 items)
static void test_U16Fixed16_Read_five_items()
{
    // Candidate Keywords: Type_U16Fixed16_Read, 5-item case
    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0x04;

    uint32_t data[] = { 0, 65536, 131072, 196608, 262144 }; // 0..4
    cmsIOHANDLER io;
    io.data = data;
    io.count = 5;
    io.pos = 0;

    cmsUInt32Number nItems = 0;
    void* result = Type_U16Fixed16_Read(&self, &io, &nItems, sizeof(data)); // 5 * 4 = 20

    ASSERT_PTR_NOT_NULL(result);
    ASSERT_EQ(nItems, (cmsUInt32Number)5);

    cmsFloat64Number* arr = (cmsFloat64Number*) result;
    for (int i = 0; i < 5; ++i) {
        ASSERT_EQ(arr[i], (cmsFloat64Number)i);
    }

    _cmsFree(self.ContextID, result);
}

// Main function: run all tests
int main()
{
    // Run tests in order
    test_U16Fixed16_Read_basic4();
    test_U16Fixed16_Read_incomplete();
    test_U16Fixed16_Read_zero_items();
    test_U16Fixed16_Read_five_items();

    // Summary
    std::cout << "Total tests run: " << g_total << "\n";
    std::cout << "Total failures:  " << g_failed << "\n";

    // Exit code 0 on success, 1 if any test failed
    return (g_failed == 0) ? 0 : 1;
}