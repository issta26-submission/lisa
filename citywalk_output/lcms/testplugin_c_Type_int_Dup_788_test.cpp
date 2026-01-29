// Minimal C++11 unit test suite for the focal method: Type_int_Dup
// This test suite provides lightweight EXPECT-like assertions and a tiny test harness
// to validate the behavior of Type_int_Dup by supplying a mock _cmsDupMem implementation.
// The goal is to cover typical success and failure paths without relying on a full test framework.

#include <cstring>
#include <cstddef>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>


// Domain-specific typedefs and types (aligned with expected C signatures)
typedef unsigned int cmsUInt32Number;

// Forward declaration of the focal C struct and function to be tested.
// The actual implementation of Type_int_Dup is in testplugin.c (C source).
struct _cms_typehandler_struct;

// Prototype of the focal function under test (C linkage)
extern "C" void* Type_int_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// Mocked dependency: _cmsDupMem that Type_int_Dup delegates to.
// The test harness provides this to control behavior and observe interactions.
extern "C" void* _cmsDupMem(void* ContextID, const void* Ptr, size_t Size);

// Lightweight test harness state
static int g_failures = 0;                 // number of failed assertions
static bool g_simulateFailure = false;     // toggles failure path in mock allocator
static void* g_lastContextID = nullptr;     // captures the ContextID passed to _cmsDupMem

// Minimal forward declaration of the helper struct used by Type_int_Dup
// We declare the layout here to allow test code to prepare a concrete instance
struct _cms_typehandler_struct {
    void* ContextID;
};

// Custom assertion helpers (non-terminating)
#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed on " << __FUNCTION__ \
                  << " at line " << __LINE__ << ": " #cond "\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_NULL(ptr) do { \
    if ((ptr) != nullptr) { \
        std::cerr << "EXPECT_NULL failed on " << __FUNCTION__ \
                  << " at line " << __LINE__ << " : pointer is non-null\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_NOT_NULL(ptr) do { \
    if ((ptr) == nullptr) { \
        std::cerr << "EXPECT_NOT_NULL failed on " << __FUNCTION__ \
                  << " at line " << __LINE__ << " : pointer is null\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_EQ_UINT(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ_UINT failed on " << __FUNCTION__ \
                  << " at line " << __LINE__ << ": " #a " != " #b " (" << (a) << " != " << (b) << ")\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_EQ_PTR(a, b) do { \
    if ((void*)(a) != (void*)(b)) { \
        std::cerr << "EXPECT_EQ_PTR failed on " << __FUNCTION__ \
                  << " at line " << __LINE__ << ": " #a " != " #b "\n"; \
        ++g_failures; \
    } \
} while (0)

// Mock implementation of _cmsDupMem to simulate duplication behavior.
// It records the ContextID for verification, optionally simulates failure,
// and duplicates the given memory region.
extern "C" void* _cmsDupMem(void* ContextID, const void* Ptr, size_t Size)
{
    // Record the context observed by the focal method
    g_lastContextID = ContextID;

    // Simulate a failure path if requested
    if (g_simulateFailure) {
        return nullptr;
    }

    // Perform a straightforward memory copy of the requested Size
    void* dst = std::malloc(Size);
    if (!dst) {
        return nullptr;
    }
    if (Ptr && Size > 0) {
        std::memcpy(dst, Ptr, Size);
    }
    return dst;
}

// Helper to run a test function and print a summary
static void RunTest(void (*testFunc)(), const char* name)
{
    // Reset per-test state if needed
    (void)name; // unused if debug printing is extended later
    testFunc();
}

// Test 1: Normal duplication path
// - Verifies that Type_int_Dup duplicates the input array correctly
// - Ensures the ContextID is forwarded to _cmsDupMem
// - Verifies the returned memory contents match the source
void test_Type_int_Dup_Normal()
{
    // Explanation:
    // Create a focal self with a non-null ContextID, prepare a source array,
    // invoke Type_int_Dup, then validate that the returned memory contains the
    // same values and that the ContextID observed by _cmsDupMem matches.
    g_simulateFailure = false;
    g_lastContextID = nullptr;

    struct _cms_typehandler_struct self;
    void* expectedContext = reinterpret_cast<void*>(0xABCD1234);
    self.ContextID = expectedContext;

    cmsUInt32Number src[4] = { 1, 2, 3, 4 }; // 4 elements
    cmsUInt32Number n = 4;

    void* dup = Type_int_Dup(&self, src, n);

    // Assertions
    EXPECT_NOT_NULL(dup);
    if (dup != nullptr) {
        cmsUInt32Number* duparr = static_cast<cmsUInt32Number*>(dup);
        for (cmsUInt32Number i = 0; i < n; ++i) {
            EXPECT_EQ_UINT(duparr[i], src[i]);
        }
        // Free allocated memory
        std::free(dup);
    }

    // Context forwarding check
    EXPECT_TRUE(g_lastContextID == expectedContext);
}

// Test 2: Failure path for the underlying allocator
// - Simulates _cmsDupMem failure and validates that Type_int_Dup propagates nullptr
void test_Type_int_Dup_Failure()
{
    g_simulateFailure = true;

    struct _cms_typehandler_struct self;
    self.ContextID = reinterpret_cast<void*>(0xDEADBEAF);

    cmsUInt32Number src[3] = { 10, 20, 30 };
    cmsUInt32Number n = 3;

    void* dup = Type_int_Dup(&self, src, n);

    // Expect a null pointer on failure
    EXPECT_NULL(dup);

    // Reset flag for subsequent tests
    g_simulateFailure = false;
}

// Test 3: Large duplication path to ensure robustness
// - Duplicates a larger dataset to exercise memory copy logic
void test_Type_int_Dup_Large()
{
    g_simulateFailure = false;
    g_lastContextID = nullptr;

    struct _cms_typehandler_struct self;
    self.ContextID = reinterpret_cast<void*>(0x12345678);

    const cmsUInt32Number n = 256;
    cmsUInt32Number src[n];
    for (cmsUInt32Number i = 0; i < n; ++i) {
        src[i] = static_cast<cmsUInt32Number>(i);
    }

    void* dup = Type_int_Dup(&self, src, n);

    EXPECT_NOT_NULL(dup);
    if (dup != nullptr) {
        cmsUInt32Number* duparr = static_cast<cmsUInt32Number*>(dup);
        for (cmsUInt32Number i = 0; i < n; ++i) {
            EXPECT_EQ_UINT(duparr[i], src[i]);
        }
        std::free(dup);
    }

    // Context forwarding check for large dup
    EXPECT_TRUE(g_lastContextID == self.ContextID);
}

// Entry point
int main()
{
    std::cout << "Running test_Type_int_Dup_Normal...\n";
    test_Type_int_Dup_Normal();

    std::cout << "Running test_Type_int_Dup_Failure...\n";
    test_Type_int_Dup_Failure();

    std::cout << "Running test_Type_int_Dup_Large...\n";
    test_Type_int_Dup_Large();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED (" << 3 << " tests).\n";
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED.\n";
        return g_failures;
    }
}