// cms_alloc_alarm_codes_chunk_test.cpp
// Simple C++11 test suite for the focal function _cmsAllocAlarmCodesChunk
// - No GTest; a lightweight in-process test runner is implemented
// - Uses the library's internal structures via cmsCreateContext / cmsDeleteContext
// - Accesses internal fields from _cmsContext_struct (via lcms2_internal.h)
// - Verifies true/false branches by exercising src != NULL and src == NULL paths
// - Uses non-terminating assertions to maximize code execution

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include internal headers to access private types and constants.
// These headers are part of Little CMS (lcms2) and expose internal structures used by _cmsAllocAlarmCodesChunk.

// Focal function declaration (C linkage)
extern "C" void _cmsAllocAlarmCodesChunk(struct _cmsContext_struct* ctx, const struct _cmsContext_struct* src);

// Helper function to print a test result
static void print_test_result(const char* test_name, bool passed) {
    std::cout << "[TEST " << (passed ? "PASSED" : "FAILED") << "] " << test_name << std::endl;
}

// Domain knowledge notes implemented as comments within tests
// Test 1: src != NULL
// - Prepare a source _cmsContext_struct with a non-null chunk at AlarmCodesContext
// - Call _cmsAllocAlarmCodesChunk(ctx, src)
// - Expect: dest chunk equals a duplicate of the source chunk (byte-for-byte)
// - Expect: destination pointer is distinct from source pointer

// Test 2: src == NULL
// - Call _cmsAllocAlarmCodesChunk(ctx, NULL)
// - Then call again with NULL to ensure the static default chunk is duplicated consistently
// - Expect: two successive allocations (dest1 and dest2) have identical contents but different addresses

int main() {
    bool all_ok = true;

    // Test 1: src != NULL path
    {
        // Create two CMS contexts via the public API
        cmsContext ctxPublic = cmsCreateContext(NULL, NULL);
        cmsContext srcPublic = cmsCreateContext(NULL, NULL);

        // Cast to internal structures to access fields (chunks[], MemPool, etc.)
        _cmsContext_struct* ctx  = reinterpret_cast<_cmsContext_struct*>(ctxPublic);
        _cmsContext_struct* src  = reinterpret_cast<_cmsContext_struct*>(srcPublic);

        // Ensure we can access the AlarmCodesContext slot
        // Allocate a source chunk with a known pattern
        size_t chunkSize = sizeof(_cmsAlarmCodesChunkType);
        void* fromPtr = malloc(chunkSize);
        if (fromPtr == NULL) {
            std::cerr << "Failed to allocate memory for source chunk in Test 1." << std::endl;
            print_test_result("Test 1 (src != NULL): allocation failure", false);
            all_ok = false;
        } else {
            // Initialize source chunk memory with a non-default pattern (0xAA)
            memset(fromPtr, 0xAA, chunkSize);
            // Place the prepared chunk into src's AlarmCodesContext slot
            src->chunks[AlarmCodesContext] = fromPtr;

            // Call the focal function
            _cmsAllocAlarmCodesChunk(ctx, src);

            // Destination after allocation
            void* destPtr = ctx->chunks[AlarmCodesContext];

            // Validate: destPtr should be non-null and distinct from fromPtr
            bool cond_ptr_distinct = (destPtr != NULL) && (destPtr != fromPtr);
            if (!cond_ptr_distinct) {
                print_test_result("Test 1: Destination pointer should be allocated and distinct from source", false);
                all_ok = false;
            }

            // Validate: memory contents should be a duplicate of the source chunk
            bool content_matches = (memcmp(destPtr, fromPtr, chunkSize) == 0);
            if (!content_matches) {
                print_test_result("Test 1: Destination content should be a byte-for-byte duplicate of the source", false);
                all_ok = false;
            }

            // Cleanup: free the source chunk memory (destination is managed by CMS context memory pool)
            free(fromPtr);

            // Clean up CMS contexts
            cmsDeleteContext(ctxPublic);
            cmsDeleteContext(srcPublic);

            print_test_result("Test 1 (src != NULL): copy-from-nonnull path", all_ok);
        }
    }

    // If Test 1 failed, we still attempt to run Test 2 to maximize coverage, but mark failures accordingly.
    // To avoid cascading errors due to prior state, we re-create contexts for Test 2 as well.

    // Test 2: src == NULL path
    {
        bool test2_ok = true;

        // Create a fresh context for Test 2
        cmsContext ctxPublic = cmsCreateContext(NULL, NULL);
        _cmsContext_struct* ctx = reinterpret_cast<_cmsContext_struct*>(ctxPublic);

        // First allocation with NULL source
        _cmsAllocAlarmCodesChunk(ctx, NULL);
        void* dest1 = ctx->chunks[AlarmCodesContext];
        size_t chunkSize = sizeof(_cmsAlarmCodesChunkType);

        if (dest1 == NULL) {
            print_test_result("Test 2: First NULL-path allocation returned NULL pointer (unexpected)", false);
            test2_ok = false;
        } else {
            // Second allocation with NULL source to get a new block
            _cmsAllocAlarmCodesChunk(ctx, NULL);
            void* dest2 = ctx->chunks[AlarmCodesContext];

            if (dest2 == NULL) {
                print_test_result("Test 2: Second NULL-path allocation returned NULL pointer (unexpected)", false);
                test2_ok = false;
            } else {
                // Verify that contents of dest1 and dest2 are identical (default static content)
                bool contents_equal = (memcmp(dest1, dest2, chunkSize) == 0);
                bool pointers_distinct = (dest1 != dest2);

                if (!contents_equal) {
                    print_test_result("Test 2: Contents of two NULL-path allocations should be identical (default chunk)", false);
                    test2_ok = false;
                }
                if (!pointers_distinct) {
                    print_test_result("Test 2: NULL-path allocations should produce distinct memory blocks", false);
                    test2_ok = false;
                }

                // Cleanup: we won't free dest2 explicitly; the CMS context cleanup will release memory when the context is deleted.
            }
        }

        cmsDeleteContext(ctxPublic);
        print_test_result("Test 2 (src == NULL): default-path duplication consistency", test2_ok);
        if (test2_ok) {
            // If test2_ok, set global flag as well
            all_ok = all_ok && true;
        } else {
            all_ok = false;
        }
    }

    if (all_ok) {
        std::cout << "[OVERALL] All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "[OVERALL] Some tests failed." << std::endl;
        return 1;
    }
}