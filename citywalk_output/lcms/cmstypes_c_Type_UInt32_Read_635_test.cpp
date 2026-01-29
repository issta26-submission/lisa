// A lightweight, self-contained C++11 test harness for the focal function:
// Type_UInt32_Read in cmstypes.c
// This test uses a minimal, hand-written shim environment to mock internal
// CMS memory/I/O helpers and the cmsIOHANDLER interface so we can exercise
// the focal function without pulling in the entire lcCMS test framework.
// The goal is to validate the core behavior: successful reads, allocation/free
// handling, and failure paths, with non-terminating assertions.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


using namespace std;

// Domain-specific lightweight type aliases to mirror the focal code's usage.
typedef void* cmsContext;
typedef unsigned int cmsUInt32Number;

// Forward-declare the focal function prototype to allow linkage with the real
// implementation from cmstypes.c when compiling this test alongside the
// library. We assume C linkage for the library symbols.
extern "C" {
    // Minimal stubs / prototypes required by the focal function.
    typedef struct _cms_typehandler_struct cms_typehandler_struct;
    typedef struct _cmsIOHANDLER cmsIOHANDLER;

    // Real library functions that Type_UInt32_Read will call.
    void* Type_UInt32_Read(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, cmsUInt32Number* nItems, cmsUInt32Number SizeOfTag);

    // Internal memory and IO helpers (we provide our own shims via linkage).
    void* _cmsCalloc(cmsContext ContextID, cmsUInt32Number Count, cmsUInt32Number Size);
    void  _cmsFree(cmsContext ContextID, void* Ptr);
    int   _cmsReadUInt32Number(cmsIOHANDLER* io, cmsUInt32Number* value);
}

// Local shim types matching the library's usage but safe for testing.
typedef struct _cms_typehandler_struct {
    cmsContext ContextID;
} cms_typehandler_struct;

struct _cmsIOHANDLER {
    // We don't rely on real lcMS fields; the test shim will reinterpret this
    // pointer as a Mock IO context.
    void* reserved;
};

// Global test-state to observe allocation/deallocation and control IO behavior.
struct MockIO {
    cmsUInt32Number* nums; // input numbers to supply
    size_t count;          // number of elements in nums
    size_t pos;            // current read index
};

// Test state for different scenarios
static MockIO g_currentMockIO;
static bool g_failAllocNextCall = false;
static size_t g_allocatedCount = 0;
static size_t g_freedCount = 0;

// Public shim implementations (C linkage) to be linked with the focal function.

// Shim for calloc used by Type_UInt32_Read.
// We count allocations to verify matching frees and optionally simulate OOM.
extern "C" void* _cmsCalloc(cmsContext ContextID, cmsUInt32Number Count, cmsUInt32Number Size) {
    if (g_failAllocNextCall) {
        g_failAllocNextCall = false; // reset flag for subsequent calls
        return NULL;
    }
    void* p = calloc(Count, Size);
    if (p != NULL) {
        g_allocatedCount += (size_t)Count * (size_t)Size;
    }
    return p;
}

// Shim for free used by Type_UInt32_Read's success and failure paths.
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr) {
    if (Ptr != NULL) {
        // We can't easily know the exact original allocation size here, but
        // we can count deallocations as a proxy for correctness.
        g_freedCount += 1;
        free(Ptr);
    }
}

// Shim for reading a 32-bit number from the provided CMS IO handler.
// We reinterpret the cmsIOHANDLER* to our MockIO object.
extern "C" int _cmsReadUInt32Number(cmsIOHANDLER* io, cmsUInt32Number* value) {
    if (io == NULL) return 0;
    MockIO* m = (MockIO*)io; // Our test uses the CMS IO pointer as MockIO*
    if (m->pos < m->count) {
        *value = m->nums[m->pos];
        m->pos += 1;
        return 1; // success
    }
    return 0; // simulate end-of-stream / failure
}

// Helper function to initialize a MockIO as the io pointer
static void init_mock_io(MockIO& m, cmsUInt32Number* nums, size_t count) {
    m.nums = nums;
    m.count = count;
    m.pos = 0;
}

// Lightweight assertion utilities (non-terminating)
static int g_testFailures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAILED] " << __FUNCTION__ << ": " << msg << "\n"; \
        ++g_testFailures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if (!((a) == (b))) { \
        std::cerr << "[FAILED] " << __FUNCTION__ << ": " << msg \
                  << " (expected " << (b) << ", got " << (a) << ")\n"; \
        ++g_testFailures; \
    } \
} while(0)

// Externally-linkable test entry that will be invoked by our main() below.
extern "C" {

// Test 1: Normal path - Type_UInt32_Read reads 3 numbers successfully.
void test_Type_UInt32_Read_Success() {
    // Prepare focal object
    cms_typehandler_struct self;
    self.ContextID = nullptr;

    // Prepare IO with 3 numbers
    cmsUInt32Number input[] = { 10, 20, 30 };
    init_mock_io(g_currentMockIO, input, 3);
    cmsIOHANDLER io;
    io.reserved = &g_currentMockIO;

    // Call the focal function
    cmsUInt32Number nItems = 0;
    void* result = Type_UInt32_Read(&self, &io, &nItems, 12); // 3 * 4 bytes

    // Verify outcomes
    EXPECT_TRUE(result != NULL, "Expected non-NULL result array on success");
    cmsUInt32Number* data = (cmsUInt32Number*)result;
    EXPECT_EQ(nItems, 3, "nItems should be 3 after reading 3 elements");
    if (result != NULL) {
        EXPECT_EQ(data[0], 10, "First element should be 10");
        EXPECT_EQ(data[1], 20, "Second element should be 20");
        EXPECT_EQ(data[2], 30, "Third element should be 30");
    }

    // Free memory through the library path
    if (result) _cmsFree(self.ContextID, result);
}

// Test 2: Failure path during read - ensure NULL return and proper free path.
void test_Type_UInt32_Read_FailureDuringRead() {
    // Reset memory accounting
    g_allocatedCount = 0;
    g_freedCount = 0;
    g_failAllocNextCall = false;

    // Prepare focal object
    cms_typehandler_struct self;
    self.ContextID = nullptr;

    // Prepare IO with 2 numbers; simulate failure on the second ReadUInt32Number call
    cmsUInt32Number input[] = { 5, 0 }; // second value would be read, but we force failure on second read
    init_mock_io(g_currentMockIO, input, 2);

    // We'll implement a fault by placing a custom fail flag that our _cmsReadUInt32Number
    // doesn't use yet; easiest is to force a failure by making the IO fail on second call
    // by artificially advancing the MockIO pos inside the shim on a specific condition.
    // Since our shim uses a simple counter, we simulate failure by configuring input and then
    // modifying the IO state before call. Here we instead rely on the test pattern:
    // We'll set up input such that the library will attempt to read more than available, causing
    // a failure on the second read.

    cmsIOHANDLER io;
    io.reserved = &g_currentMockIO;

    // Call the focal function
    cmsUInt32Number nItems = 0;
    void* result = Type_UInt32_Read(&self, &io, &nItems, 12); // 3 elements attempt

    // We expect NULL due to read failure
    EXPECT_TRUE(result == NULL, "Expected NULL result on read failure");
    // Ensure that if any allocation happened, it was freed
    if (result) _cmsFree(self.ContextID, result);
    // We also expect that the allocated/free accounting is in a consistent state (no leaks)
    // Note: In this test, since a failure causes no successful allocation, allocatedCount should be 0
    EXPECT_EQ(g_allocatedCount, g_freedCount, "Allocation and free counts should match on failure");
}

// Test 3: Allocation failure path - simulate _cmsCalloc returning NULL
void test_Type_UInt32_Read_AllocFailure() {
    // Prepare focal object
    cms_typehandler_struct self;
    self.ContextID = nullptr;

    // Configure allocator to fail on next allocation
    g_failAllocNextCall = true;

    // Prepare IO with some data
    cmsUInt32Number input[] = { 1, 2, 3 };
    init_mock_io(g_currentMockIO, input, 3);

    cmsIOHANDLER io;
    io.reserved = &g_currentMockIO;

    cmsUInt32Number nItems = 0;
    void* result = Type_UInt32_Read(&self, &io, &nItems, 12);

    EXPECT_TRUE(result == NULL, "Expected NULL result when allocation fails");
    // No memory should be allocated or freed
    EXPECT_EQ(g_allocatedCount, 0, "No allocation should occur on allocator failure");
    EXPECT_EQ(g_freedCount, 0, "No memory should be freed on allocator failure");
}

} // extern "C"

// Test runner
int main() {
    cout << "Running Type_UInt32_Read unit tests (lightweight harness)..." << endl;
    // Call tests directly (no test framework)
    test_Type_UInt32_Read_Success();
    test_Type_UInt32_Read_FailureDuringRead();
    test_Type_UInt32_Read_AllocFailure();

    if (g_testFailures == 0) {
        cout << "All tests completed. No assertion failures detected." << endl;
        return 0;
    } else {
        cout << g_testFailures << " test failure(s) detected." << endl;
        return 1;
    }
}