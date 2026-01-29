/*
Unit test suite for the focal method:
    void* Type_UInt64_Read(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems, cmsUInt32Number SizeOfTag)

Context:
- We create a minimal test harness that links against the focal function Type_UInt64_Read.
- We mock the dependent runtime functions _cmsCalloc, _cmsFree, and _cmsReadUInt64Number to isolate the unit under test.
- We provide a small fake cmsIOHANDLER type and drive reads from a preloaded buffer.
- We exercise normal path (success), allocation failure, and read failure mid-way to ensure correct cleanup and behavior.
- All tests are self-contained in C++11, no GTest is used. A simple main() runs the tests.
- The Candidate Keywords from Step 1 are reflected in how we mock dependencies and feed inputs:
  - ContextID, _cmsCalloc, _cmsFree, _cmsReadUInt64Number, cmsUInt64Number, cmsUInt32Number, SizeOfTag, nItems, Type_UInt64_Read flow
*/

#include <cstring>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Domain knowledge related typedefs (mirroring the library usage in a test harness)
typedef uint32_t cmsUInt32Number;
typedef uint64_t cmsUInt64Number;
typedef void* cmsContext;
typedef int cmsBool;
typedef struct _cmsIOHANDLER cmsIOHANDLER;

// Forward declaration of the focal method (as used by tests)
extern "C" void* Type_UInt64_Read(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems, cmsUInt32Number SizeOfTag);

// Minimal replica of the type-hander struct used by Type_UInt64_Read
typedef struct _cms_typehandler_struct {
    cmsContext ContextID;
} _cms_typehandler_struct;

// Global test scaffolding to mock library-dependent behavior
static std::vector<uint8_t> g_InputBuffer;
static size_t                 g_ReadIndex = 0;
static int                    g_ReadCallCount = 0;

static bool                   g_ForceAllocFail = false;
static bool                   g_ReadShouldFailOnThirdRead = false;

static void*                  g_LastFreedPtr = nullptr;

/* Forward declarations of mocked dependencies (C linkage to be resolved by linker with cmstypes.c) */
extern "C" {
    void* _cmsCalloc(cmsContext ContextID, cmsUInt32Number Num, cmsUInt32Number Size);
    void  _cmsFree(cmsContext ContextID, void* Ptr);
    cmsBool _cmsReadUInt64Number(cmsIOHANDLER* io, cmsUInt64Number* Number);
}

/* Mock implementation: _cmsCalloc
   - Simulates memory allocation failure when g_ForceAllocFail is set.
   - Returns NULL for zero-sized requests (to reflect library behavior in many C runtimes). */
extern "C" void* _cmsCalloc(cmsContext ContextID, cmsUInt32Number Num, cmsUInt32Number Size) {
    (void)ContextID;
    if (g_ForceAllocFail) {
        return NULL;
    }
    if (Num == 0 || Size == 0) {
        return NULL;
    }
    // Use malloc + zero-initialization (calloc equivalent)
    void* p = std::calloc((size_t)Num, (size_t)Size);
    return p;
}

/* Mock implementation: _cmsFree
   - Frees memory and records the freed pointer for validation. */
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr) {
    (void)ContextID;
    if (Ptr != nullptr) {
        g_LastFreedPtr = Ptr;
        std::free(Ptr);
    } else {
        g_LastFreedPtr = nullptr;
    }
}

/* Mock implementation: _cmsReadUInt64Number
   - Reads a 64-bit little-endian value from g_InputBuffer.
   - Fails on third read if g_ReadShouldFailOnThirdRead is true. */
extern "C" cmsBool _cmsReadUInt64Number(cmsIOHANDLER* io, cmsUInt64Number* Number) {
    (void)io; // io is not used by this minimal mock
    if (g_ReadShouldFailOnThirdRead && g_ReadCallCount == 2) { // zero-based: 0,1,2
        ++g_ReadCallCount;
        return 0;
    }
    if (g_ReadIndex + 8 > g_InputBuffer.size()) {
        return 0;
    }
    cmsUInt64Number val = 0;
    for (int b = 0; b < 8; ++b) {
        val |= (static_cast<cmsUInt64Number>(g_InputBuffer[g_ReadIndex + b]) << (8 * b));
    }
    g_ReadIndex += 8;
    *Number = val;
    ++g_ReadCallCount;
    return 1;
}

/* Helpers to drive tests */

static void setupBufferWithNumbers(const std::vector<cmsUInt64Number>& nums) {
    g_InputBuffer.clear();
    for (cmsUInt64Number v : nums) {
        for (int b = 0; b < 8; ++b) {
            g_InputBuffer.push_back(static_cast<uint8_t>((v >> (8 * b)) & 0xFF));
        }
    }
    g_ReadIndex = 0;
    g_ReadCallCount = 0;
}

static bool test_UInt64_Read_normal_path() {
    // Test that Type_UInt64_Read reads 3 numbers correctly and returns proper nItems
    // Candidate Keywords: ContextID, allocation, reading, array contents
    bool ok = true;

    // Prepare environment
    _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0xABCD; // non-null
    setupBufferWithNumbers({1ULL, 2ULL, 3ULL});

    cmsUInt32Number nItems = 0;
    void* ptr = Type_UInt64_Read(&self, nullptr, &nItems, 3 * sizeof(cmsUInt64Number));

    if (ptr == nullptr) {
        std::cerr << "[Test Normal] FAILED: Type_UInt64_Read returned NULL on valid input.\n";
        ok = false;
    } else if (nItems != 3) {
        std::cerr << "[Test Normal] FAILED: Expected nItems=3, got " << nItems << "\n";
        ok = false;
    } else {
        cmsUInt64Number* arr = reinterpret_cast<cmsUInt64Number*>(ptr);
        if (arr[0] != 1ULL || arr[1] != 2ULL || arr[2] != 3ULL) {
            std::cerr << "[Test Normal] FAILED: Array contents incorrect. Got {"
                      << arr[0] << ", " << arr[1] << ", " << arr[2] << "}\n";
            ok = false;
        }
    }

    if (ptr != nullptr) {
        // Free memory as the test consumer
        _cmsFree(self.ContextID, ptr);
    }

    if (!ok) {
        std::cerr << "[Test Normal] Details: nItems=" << nItems << ", ptr=" << (void*)ptr << "\n";
    } else {
        std::cout << "[Test Normal] PASSED\n";
    }
    return ok;
}

static bool test_UInt64_Read_allocation_failure() {
    // Test that if allocation fails, function returns NULL and doesn't modify nItems (0)
    // Candidate Keywords: allocation failure path
    bool ok = true;

    _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0xBEEF;
    g_ForceAllocFail = true; // force allocation to fail
    setupBufferWithNumbers({10ULL, 20ULL, 30ULL}); // input numbers exist, but allocation will fail

    cmsUInt32Number nItems = 123; // ensure it gets reset to 0 by function
    void* ptr = Type_UInt64_Read(&self, nullptr, &nItems, 3 * sizeof(cmsUInt64Number));

    if (ptr != nullptr) {
        std::cerr << "[Test AllocFail] FAILED: Expected NULL pointer on allocation failure.\n";
        ok = false;
        _cmsFree(self.ContextID, ptr);
    }
    if (nItems != 0) {
        std::cerr << "[Test AllocFail] FAILED: Expected nItems=0 on allocation failure, got " << nItems << "\n";
        ok = false;
    }

    // Reset flag for other tests
    g_ForceAllocFail = false;

    if (ok) {
        std::cout << "[Test AllocFail] PASSED\n";
    }
    return ok;
}

static bool test_UInt64_Read_read_failure_midway() {
    // Test that if _cmsReadUInt64Number fails on a read, Type_UInt64_Read frees allocated memory and returns NULL
    // Candidate Keywords: partial read failure cleanup
    bool ok = true;

    _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0xC0DE;
    setupBufferWithNumbers({7ULL, 8ULL, 9ULL});

    g_ReadShouldFailOnThirdRead = true; // fail on the third read (index 2)
    g_LastFreedPtr = nullptr;
    cmsUInt32Number nItems = 0;
    void* ptr = Type_UInt64_Read(&self, nullptr, &nItems, 3 * sizeof(cmsUInt64Number));

    if (ptr != nullptr) {
        std::cerr << "[Test ReadFail] FAILED: Expected NULL due to read error, got non-null pointer.\n";
        ok = false;
        _cmsFree(self.ContextID, ptr);
    }
    if (nItems != 0) {
        std::cerr << "[Test ReadFail] FAILED: Expected nItems=0 on read failure, got " << nItems << "\n";
        ok = false;
    }
    if (g_LastFreedPtr == nullptr) {
        std::cerr << "[Test ReadFail] FAILED: Expected previously allocated memory to be freed on failure.\n";
        ok = false;
    } else {
        // Reset for cleanliness
        g_LastFreedPtr = nullptr;
    }

    // Clean up test state
    g_ReadShouldFailOnThirdRead = false;

    if (ok) {
        std::cout << "[Test ReadFail] PASSED\n";
    }
    return ok;
}

/* Main harness: run tests and report summary.
   The tests are designed to be non-terminating (i.e., all tests run even if one fails). */
int main() {
    int failures = 0;

    std::cout << "Starting unit tests for Type_UInt64_Read (cmstypes.c) using a minimal test harness.\n";

    if (!test_UInt64_Read_normal_path()) {
        ++failures;
    }
    if (!test_UInt64_Read_allocation_failure()) {
        ++failures;
    }
    if (!test_UInt64_Read_read_failure_midway()) {
        ++failures;
    }

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED.\n";
        return 0;
    } else {
        std::cout << failures << " TEST(S) FAILED.\n";
        return 1;
    }
}

/*
Notes for maintainers:
- The test harness mocks the core memory read/write dependencies that the focal function Type_UInt64_Read relies on
  (_cmsCalloc, _cmsFree, _cmsReadUInt64Number). By controlling these, we validate:
  - Normal successful reads and proper array contents
  - Allocation failure handling with proper cleanup and no memory leak
  - Read failure mid-way ensuring the allocated memory is freed and the function returns NULL

- We purposely avoid any private API exposure and keep tests driven through the public functional entry point.

- If the build environment provides the actual LittleCMS headers and cmstypes.c implementation, link this test
  with the corresponding cmstypes.o or library to execute the tests.
*/