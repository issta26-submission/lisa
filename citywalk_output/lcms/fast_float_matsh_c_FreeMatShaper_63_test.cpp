// Unit tests for FreeMatShaper in fast_float_matsh.c
// - NoGoogleTest: custom lightweight test harness with non-terminating assertions
// - Targets: FreeMatShaper(ContextID, Data) behavior for NULL and non-NULL Data
// - Mocks: provide a local _cmsFree implementation to verify correct invocation

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fast_float_internal.h>


// Minimal type aliases to allow compilation against the focal function.
// These typedefs mirror what the focal code expects to interact with.
typedef void* cmsContext;

// Forward declaration of the focal function under test (C linkage)
extern "C" void FreeMatShaper(cmsContext ContextID, void* Data);

// Mock hook for the internal _cmsFree used by FreeMatShaper.
// We provide a C-linkage implementation to intercept calls and record arguments.
static int g_FREE_CALL_COUNT = 0;
static void* g_LAST_FREE_PTR = nullptr;
static void* g_LAST_FREE_CONTEXT = nullptr;

// Dummy implementation of _cmsFree to capture invocation details.
// The real library would free memory; here we record the call for assertions.
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr)
{
    ++g_FREE_CALL_COUNT;
    g_LAST_FREE_CONTEXT = ContextID;
    g_LAST_FREE_PTR = Ptr;
}

// Lightweight structure used to emulate VXMatShaperFloatData layout.
// We only care about the first field (real_ptr) which the focal function uses.
struct DummyVXMatShaperFloatData {
    void* real_ptr; // expected to be the first/primary field accessed by FreeMatShaper
};

// Simple test framework (non-terminating assertions)
static int gTestFailures = 0;

#define TEST_EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { fprintf(stderr, "TEST FAILED: %s\n", msg); ++gTestFailures; } } while(0)

#define TEST_EXPECT_EQ_PTR(a, b, msg) \
    do { if((void*)(a) != (void*)(b)) { fprintf(stderr, "TEST FAILED: %s (expected %p, got %p)\n", msg, (void*)(b), (void*)(a)); ++gTestFailures; } } while(0)

#define TEST_EXPECT_EQ_INT(a, b, msg) \
    do { if((a) != (b)) { fprintf(stderr, "TEST FAILED: %s (expected %d, got %d)\n", msg, (int)(b), (int)(a)); ++gTestFailures; } } while(0)

// Reset global mocks before each test
static void reset_mocks()
{
    g_FREE_CALL_COUNT = 0;
    g_LAST_FREE_PTR = nullptr;
    g_LAST_FREE_CONTEXT = nullptr;
}

// Unit Test 1: NULL Data should not trigger a free
// Rationale: The function guards with if (d != NULL) before calling _cmsFree.
static void test_FreeMatShaper_NullData_NoFree()
{
    reset_mocks();
    // Call with NULL Data
    FreeMatShaper((cmsContext)0xDEADBEEF, nullptr);

    // Verify: no free should have occurred
    TEST_EXPECT_EQ_INT(g_FREE_CALL_COUNT, 0, "_FreeMatShaper should not call _cmsFree when Data is NULL");
}

// Unit Test 2: Non-NULL Data should free the underlying real_ptr
// Rationale: Ensure the correct pointer from d->real_ptr is passed to _cmsFree with the given context.
static void test_FreeMatShaper_ValidData_FreesPointer()
{
    reset_mocks();

    DummyVXMatShaperFloatData d;
    d.real_ptr = (void*)0xCAFEBABE;

    FreeMatShaper((cmsContext)0x1234, &d);

    TEST_EXPECT_EQ_INT(g_FREE_CALL_COUNT, 1, "_cmsFree should be called exactly once for valid Data");
    TEST_EXPECT_EQ_PTR(g_LAST_FREE_PTR, d.real_ptr, "_FreeMatShaper should pass d->real_ptr to _cmsFree");
    TEST_EXPECT_TRUE(g_LAST_FREE_CONTEXT == (void*)0x1234, "_FreeMatShaper should pass the correct ContextID to _cmsFree");
}

// Unit Test 3: Data with NULL real_ptr should still call _cmsFree with NULL pointer
// Rationale: The function only guards on Data != NULL, not the value of real_ptr.
static void test_FreeMatShaper_DataWithNullRealPtr_FreeNullPointer()
{
    reset_mocks();

    DummyVXMatShaperFloatData d;
    d.real_ptr = nullptr;

    FreeMatShaper((cmsContext)0xABCD, &d);

    TEST_EXPECT_EQ_INT(g_FREE_CALL_COUNT, 1, "_cmsFree should be invoked even if d->real_ptr is NULL");
    TEST_EXPECT_EQ_PTR(g_LAST_FREE_PTR, nullptr, "_FreeMatShaper should pass NULL as pointer when real_ptr is NULL");
    TEST_EXPECT_TRUE(g_LAST_FREE_CONTEXT == (void*)0xABCD, "_FreeMatShaper should pass the correct ContextID to _cmsFree even if real_ptr is NULL");
}

// Unit Test 4 (optional coverage): Multiple consecutive calls to FreeMatShaper
// Rationale: Validate that the mock captures multiple invocations and correct state per call.
static void test_FreeMatShaper_MultipleCalls()
{
    reset_mocks();

    DummyVXMatShaperFloatData a;
    a.real_ptr = (void*)0x1001;
    FreeMatShaper((cmsContext)0x01, &a);

    TEST_EXPECT_EQ_INT(g_FREE_CALL_COUNT, 1, "First call should increment free count to 1");
    TEST_EXPECT_EQ_PTR(g_LAST_FREE_PTR, a.real_ptr, "First call: freed pointer should match a.real_ptr");
    TEST_EXPECT_TRUE(g_LAST_FREE_CONTEXT == (void*)0x01, "First call: context should match 0x01");

    DummyVXMatShaperFloatData b;
    b.real_ptr = (void*)0x2002;
    FreeMatShaper((cmsContext)0x02, &b);

    TEST_EXPECT_EQ_INT(g_FREE_CALL_COUNT, 2, "Second call should increment free count to 2");
    TEST_EXPECT_EQ_PTR(g_LAST_FREE_PTR, b.real_ptr, "Second call: freed pointer should match b.real_ptr");
    TEST_EXPECT_TRUE(g_LAST_FREE_CONTEXT == (void*)0x02, "Second call: context should match 0x02");
}

// Main: run all tests and report summary
int main()
{
    printf("Starting FreeMatShaper unit tests...\n");

    test_FreeMatShaper_NullData_NoFree();
    test_FreeMatShaper_ValidData_FreesPointer();
    test_FreeMatShaper_DataWithNullRealPtr_FreeNullPointer();
    test_FreeMatShaper_MultipleCalls();

    if (gTestFailures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        fprintf(stderr, "TOTAL FAILURES: %d\n", gTestFailures);
        return 1;
    }
}