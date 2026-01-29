// Test suite for Type_S15Fixed16_Free in cmstypes.c
// This test uses a minimal mock of _cmsFree to verify that Type_S15Fixed16_Free
// correctly calls _cmsFree with the expected ContextID and Ptr values.
// The actual CMS type structure is opaque in the test; we only rely on the first
// member being ContextID (as used by Type_S15Fixed16_Free).

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Forward declaration of the focal function from the C source (cmstypes.c)
extern "C" void Type_S15Fixed16_Free(struct _cms_typehandler_struct* self, void* Ptr);

// We mock _cmsFree to capture its parameters for verification.
// Provide C linkage to match the symbol expected by the linked C code.
extern "C" void _cmsFree(void* ContextID, void* Ptr);

// Minimal replica of the _cms_typehandler_struct layout sufficient for tests.
// We place ContextID as the first member to align with how the function uses it.
struct _cms_typehandler_struct {
    void* ContextID;
    // The rest of the real structure is opaque to these tests.
};

// Global storage to capture _cmsFree invocation parameters
static void* g_last_free_ContextID = nullptr;
static void* g_last_free_Ptr = nullptr;

// Mock implementation of _cmsFree to record the arguments passed by Type_S15Fixed16_Free
extern "C" void _cmsFree(void* ContextID, void* Ptr)
{
    g_last_free_ContextID = ContextID;
    g_last_free_Ptr = Ptr;
}

// Simple test harness
static int g_failures = 0;
static void report_test(const char* name, bool passed)
{
    if (passed) {
        std::cout << "[OK]   " << name << "\n";
    } else {
        std::cout << "[FAILED] " << name << "\n";
        ++g_failures;
    }
}

// Test 1: Basic scenario - ensure ContextID and Ptr are passed through to _cmsFree
void test_basic_free_passes_context_and_ptr()
{
    g_last_free_ContextID = nullptr;
    g_last_free_Ptr = nullptr;

    // Prepare a fake self with a known ContextID and a distinct Ptr
    _cms_typehandler_struct self;
    void* expected_ctx = reinterpret_cast<void*>(0x11111111);
    void* expected_ptr = reinterpret_cast<void*>(0x22222222);
    self.ContextID = expected_ctx;

    // Call the focal function
    Type_S15Fixed16_Free(&self, expected_ptr);

    // Verify that _cmsFree was invoked with the exact values
    bool ok = (g_last_free_ContextID == expected_ctx) && (g_last_free_Ptr == expected_ptr);
    report_test("test_basic_free_passes_context_and_ptr", ok);
}

// Test 2: Null Ptr - ensure passing a NULL Ptr works and is forwarded
void test_free_with_null_ptr()
{
    g_last_free_ContextID = nullptr;
    g_last_free_Ptr = reinterpret_cast<void*>(0xDEADBEEF); // ensure it's overwritten

    _cms_typehandler_struct self;
    void* expected_ctx = reinterpret_cast<void*>(0x33333333);
    void* expected_ptr = nullptr; // NULL
    self.ContextID = expected_ctx;

    Type_S15Fixed16_Free(&self, expected_ptr);

    bool ok = (g_last_free_ContextID == expected_ctx) && (g_last_free_Ptr == expected_ptr);
    report_test("test_free_with_null_ptr", ok);
}

// Test 3: Null ContextID - ensure a NULL ContextID is handled (still forwarded)
void test_free_with_null_context()
{
    g_last_free_ContextID = reinterpret_cast<void*>(0xAAAAAAAA);
    g_last_free_Ptr = reinterpret_cast<void*>(0xBBBBBBBB);

    _cms_typehandler_struct self;
    void* expected_ctx = nullptr; // NULL context
    void* expected_ptr = reinterpret_cast<void*>(0xCCCCCCCC);
    self.ContextID = expected_ctx;

    Type_S15Fixed16_Free(&self, expected_ptr);

    bool ok = (g_last_free_ContextID == expected_ctx) && (g_last_free_Ptr == expected_ptr);
    report_test("test_free_with_null_context", ok);
}

// Test 4: Multiple invocations with different values ensure updates are reflected
void test_multiple_invocations_update_values()
{
    // First call
    g_last_free_ContextID = nullptr;
    g_last_free_Ptr = nullptr;

    _cms_typehandler_struct self1;
    void* ctx1 = reinterpret_cast<void*>(0x10101010);
    void* p1   = reinterpret_cast<void*>(0x20202020);
    self1.ContextID = ctx1;
    Type_S15Fixed16_Free(&self1, p1);

    bool ok1 = (g_last_free_ContextID == ctx1) && (g_last_free_Ptr == p1);
    report_test("test_multiple_invocations_update_values_step1", ok1);

    // Second call with different values
    g_last_free_ContextID = nullptr;
    g_last_free_Ptr = nullptr;

    _cms_typehandler_struct self2;
    void* ctx2 = reinterpret_cast<void*>(0x30303030);
    void* p2   = reinterpret_cast<void*>(0x40404040);
    self2.ContextID = ctx2;
    Type_S15Fixed16_Free(&self2, p2);

    bool ok2 = (g_last_free_ContextID == ctx2) && (g_last_free_Ptr == p2);
    report_test("test_multiple_invocations_update_values_step2", ok2);
}

// Main test runner
int main()
{
    // Run tests
    test_basic_free_passes_context_and_ptr();
    test_free_with_null_ptr();
    test_free_with_null_context();
    test_multiple_invocations_update_values();

    // Summary
    if (g_failures == 0) {
        std::cout << "\nAll tests passed.\n";
        return 0;
    } else {
        std::cout << "\nTests failed: " << g_failures << "\n";
        return 1;
    }
}