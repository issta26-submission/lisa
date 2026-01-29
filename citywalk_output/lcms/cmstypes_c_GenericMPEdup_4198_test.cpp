// Test suite for GenericMPEdup (focal method) using a lightweight custom harness.
// This test uses a stub for cmsStageDup to isolate GenericMPEdup's behavior
// without requiring the full CMS pipeline creation logic.
// The goal is to ensure GenericMPEdup delegates correctly to cmsStageDup
// and preserves pointer semantics (non-null Ptr yields a non-null dup, null Ptr yields null).

#include <vector>
#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Forward declaration of the focal function as it exists in cmstypes.c.
// We rely on the actual implementation linked from cmstypes.c during test.
extern "C" void* GenericMPEdup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// Stubbed cmsStageDup to intercept and verify the behavior of GenericMPEdup
// without needing the full upstream cmsStageDup implementation.
// We declare it with C linkage to match the library's symbol.
extern "C" cmsStage* cmsStageDup(cmsStage* Stage) {
    // The test will define the expected behavior for non-null and null inputs.
    extern cmsStage* g_last_input_stage; // set by the test code to capture the input.

    g_last_input_stage = Stage;
    if (Stage == nullptr) return nullptr;
    // Return a stable sentinel object to verify that GenericMPEdup returns the dup.
    extern cmsStage* g_sentinel_ptr;
    return g_sentinel_ptr;
}

// Global hooks to observe the interaction between GenericMPEdup and cmsStageDup
static cmsStage sentinelStage;          // Sentinel stage instance for dup results
static cmsStage* g_sentinel_ptr = &sentinelStage; // Pointer returned by cmsStageDup when Ptr is non-null
static cmsStage* g_last_input_stage = nullptr;    // Tracks the last Stage passed to cmsStageDup

// Lightweight non-terminating test helpers (domain-knowledge aligned)
static int g_failures = 0;
#define CHECK(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "CHECK FAILED: " << (msg) << " [File: " << __FILE__ << ", Line: " << __LINE__ << "]\n"; \
            ++g_failures; \
        } \
    } while(0)

int main() {
    // Test 1: Non-null Ptr should result in the dup pointer being returned and Ptr being passed to cmsStageDup
    {
        // Arrange
        static cmsStage testStage; // a dummy stage instance
        g_last_input_stage = nullptr;

        // Act
        void* result = GenericMPEdup(nullptr, &testStage, 123);

        // Assert
        CHECK(result == static_cast<void*>(g_sentinel_ptr),
              "GenericMPEdup should return the result of cmsStageDup for non-null Ptr");
        CHECK(g_last_input_stage == &testStage,
              "GenericMPEdup should pass the original Ptr to cmsStageDup");
    }

    // Test 2: Null Ptr should propagate as NULL without attempting to dereference
    {
        // Arrange
        g_last_input_stage = nullptr;

        // Act
        void* result = GenericMPEdup(nullptr, nullptr, 0);

        // Assert
        CHECK(result == nullptr,
              "GenericMPEdup should return NULL when Ptr is NULL");
        CHECK(g_last_input_stage == nullptr,
              "cmsStageDup should be called with NULL when Ptr is NULL");
    }

    // Test 3: Non-null Ptr with a non-null self (to ensure self is unused and Ptr is still forwarded)
    {
        // Arrange
        static cmsStage testStage;
        g_last_input_stage = nullptr;
        static struct _cms_typehandler_struct dummySelf; // Use a valid-looking self pointer

        // Act
        void* result = GenericMPEdup(&dummySelf, &testStage, 42);

        // Assert
        CHECK(result == static_cast<void*>(g_sentinel_ptr),
              "GenericMPEdup should ignore 'self' parameter and forward Ptr to cmsStageDup");
        CHECK(g_last_input_stage == &testStage,
              "cmsStageDup should receive the correct Ptr even when self is provided");
    }

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed for GenericMPEdup (custom harness).\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed for GenericMPEdup (custom harness).\n";
        return 1;
    }
}