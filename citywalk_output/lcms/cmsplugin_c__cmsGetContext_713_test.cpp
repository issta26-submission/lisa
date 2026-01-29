// Unit test suite for the focal method _cmsGetContext
// This test suite is designed to be compiled with a C++11 compiler without gtest.
// It uses a small in-file harness to exercise the _cmsGetContext behavior
// (NULL context -> globalContext, context present in pool -> that context,
// context not present -> globalContext). It also exercises traversal for
// multiple pool entries to ensure correct matching.
//
// Notes:
// - The test re-creates a minimal internal environment (struct _cmsContext_struct,
//   globalContext, pool head, mutex) needed by the focal method.
// - The tests are self-contained and do not rely on any external units.
// - Non-terminating assertions are used (failures are logged, execution continues).

#include <cstring>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>
#include <sstream>


// BEGIN Minimal reproducer of the internal definitions used by the focal method

// Typedef used by the focal function
typedef void* cmsContext;

// Internal context structure used by the pool
struct _cmsContext_struct {
    struct _cmsContext_struct* Next;
    // Additional fields are not required for the unit tests
};

// Global (singleton) context and pool state (test-scoped)
static struct _cmsContext_struct globalContext;
static struct _cmsContext_struct* _cmsContextPoolHead = nullptr;
static void* _cmsContextPoolHeadMutex = nullptr;

// Stubbed synchronization primitives (no-ops for unit testing)
static void InitContextMutex(void) { /* no-op in test environment */ }
static void _cmsEnterCriticalSectionPrimitive(void* /*mutex*/) { /* no-op in test environment */ }
static void _cmsLeaveCriticalSectionPrimitive(void* /*mutex*/) { /* no-op in test environment */ }

// The focal method under test (copied from the provided snippet for a self-contained test)
struct _cmsContext_struct* _cmsGetContext(cmsContext ContextID)
{
{
    struct _cmsContext_struct* id = (struct _cmsContext_struct*) ContextID;
    struct _cmsContext_struct* ctx;
    // On 0, use global settings
    if (id == NULL) 
        return &globalContext;
    InitContextMutex();
    // Search
    _cmsEnterCriticalSectionPrimitive(&_cmsContextPoolHeadMutex);
    for (ctx = _cmsContextPoolHead;
         ctx != NULL;
         ctx = ctx ->Next) {
            // Found it?
        if (id == ctx)
        {
            _cmsLeaveCriticalSectionPrimitive(&_cmsContextPoolHeadMutex);
            return ctx; // New-style context
        }
    }
    _cmsLeaveCriticalSectionPrimitive(&_cmsContextPoolHeadMutex);
    return &globalContext;
}
}

// END Minimal reproducer

// Test harness infrastructure (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

static void log_failure(const char* file, int line, const std::string& msg) {
    ++g_tests_failed;
    std::cerr << "Test Failure - " << file << ":" << line << " - " << msg << "\n";
}

// Simple non-terminating assertions for pointers and values
#define EXPECT_PTR_EQ(a, b) do { \
    ++g_tests_run; \
    if ((void*)(a) != (void*)(b)) { \
        std::ostringstream oss; \
        oss << "Expected pointer " << #a << " == " << #b \
            << " but got " << static_cast<void*>(a) \
            << " vs " << static_cast<void*>(b); \
        log_failure(__FILE__, __LINE__, oss.str()); \
    } \
} while(false)

#define EXPECT_EQ(a, b) do { \
    ++g_tests_run; \
    if (!((a) == (b))) { \
        std::ostringstream oss; \
        oss << "Expected " << #a << " == " << #b \
            << " but got " << (a) << " vs " << (b); \
        log_failure(__FILE__, __LINE__, oss.str()); \
    } \
} while(false)

static void reset_context_pool() {
    _cmsContextPoolHead = nullptr;
}

// TEST 1: NULL ContextID should return the globalContext
static void test_null_context_returns_global() {
    // Arrange
    reset_context_pool();
    globalContext.Next = nullptr;
    // Act
    struct _cmsContext_struct* result = _cmsGetContext(nullptr);
    // Assert
    EXPECT_PTR_EQ(result, &globalContext);
}

// TEST 2: Context is the sole element in the pool -> should return that element
static void test_context_in_pool_single_node_returns_it() {
    // Arrange
    reset_context_pool();
    static struct _cmsContext_struct nodeA;
    nodeA.Next = nullptr;
    _cmsContextPoolHead = &nodeA;

    // Act
    cmsContext id = &nodeA;
    struct _cmsContext_struct* result = _cmsGetContext(id);

    // Assert
    EXPECT_PTR_EQ(result, &nodeA);
}

// TEST 2b: Context is the first element in a longer pool -> should return first element
static void test_context_in_pool_first_node_returns_first() {
    // Arrange
    reset_context_pool();
    static struct _cmsContext_struct nodeA;
    static struct _cmsContext_struct nodeB;
    nodeA.Next = &nodeB;
    nodeB.Next = nullptr;
    _cmsContextPoolHead = &nodeA;

    // Act
    cmsContext id = &nodeA;
    struct _cmsContext_struct* result = _cmsGetContext(id);

    // Assert
    EXPECT_PTR_EQ(result, &nodeA);
}

// TEST 2c: Context is in the pool but not the first node -> should be found via traversal
static void test_context_in_pool_second_node_returns_second() {
    // Arrange
    reset_context_pool();
    static struct _cmsContext_struct nodeA;
    static struct _cmsContext_struct nodeB;
    nodeA.Next = &nodeB;
    nodeB.Next = nullptr;
    _cmsContextPoolHead = &nodeA;

    // Act
    cmsContext id = &nodeB;
    struct _cmsContext_struct* result = _cmsGetContext(id);

    // Assert
    EXPECT_PTR_EQ(result, &nodeB);
}

// TEST 3: Context not in pool and not NULL -> should return globalContext
static void test_context_not_in_pool_returns_global() {
    // Arrange
    reset_context_pool();
    static struct _cmsContext_struct nodeA;
    static struct _cmsContext_struct nodeB;
    nodeA.Next = &nodeB;
    nodeB.Next = nullptr;
    _cmsContextPoolHead = &nodeA;

    // Create an unrelated context object
    static struct _cmsContext_struct unrelated;
    unrelated.Next = nullptr;

    // Act
    cmsContext id = &unrelated;
    struct _cmsContext_struct* result = _cmsGetContext(id);

    // Assert
    EXPECT_PTR_EQ(result, &globalContext);
}

// TEST 4: NULL context should override any pool state and return globalContext
static void test_null_overrides_pool_returns_global() {
    // Arrange
    reset_context_pool();
    static struct _cmsContext_struct nodeA;
    nodeA.Next = nullptr;
    _cmsContextPoolHead = &nodeA;
    globalContext.Next = nullptr;

    // Act
    struct _cmsContext_struct* result = _cmsGetContext(nullptr);

    // Assert
    EXPECT_PTR_EQ(result, &globalContext);
}

// MAIN: Run all tests
int main() {
    // Run tests
    test_null_context_returns_global();
    test_context_in_pool_single_node_returns_it();
    test_context_in_pool_first_node_returns_first();
    test_context_in_pool_second_node_returns_second();
    test_context_not_in_pool_returns_global();
    test_null_overrides_pool_returns_global();

    // Summary
    std::cout << "Unit Test Summary: " << (g_tests_run - g_tests_failed)
              << " passed, " << g_tests_failed << " failed, "
              << "out of " << g_tests_run << " assertions." << std::endl;

    return (g_tests_failed == 0) ? 0 : 1;
}