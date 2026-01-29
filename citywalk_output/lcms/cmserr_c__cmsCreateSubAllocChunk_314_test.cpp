// Unit test suite for _cmsCreateSubAllocChunk in cmserr.c
// This test is written in C++11 and does not rely on Google Test.
// It uses the project's existing C API (as exposed by cmserr.c) and provides
// minimal C++-side test harness with non-terminating assertions.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Domain knowledge: We'll rely on the actual behavior of _cmsCreateSubAllocChunk.
// We provide C linkage declarations so that the linker can resolve the symbols
// from the C source file cmserr.c (or the library that implements them).

extern "C" {

    // Typedefs expected by the focal method
    typedef void* cmsContext;
    typedef unsigned int cmsUInt32Number;
    typedef unsigned char cmsUInt8Number;

    // The chunk structure as used by _cmsCreateSubAllocChunk.
    // We mirror the internal layout to be able to inspect fields after creation.
    typedef struct _cmsSubAllocator_chunk {
        cmsUInt8Number* Block;
        cmsUInt32Number BlockSize;
        cmsUInt32Number Used;
        struct _cmsSubAllocator_chunk* next;
    } _cmsSubAllocator_chunk;

    // Function under test
    _cmsSubAllocator_chunk* _cmsCreateSubAllocChunk(cmsContext ContextID, cmsUInt32Number Initial);

    // Memory management functions expected by the focal method (provided by the library)
    void* _cmsMallocZero(cmsContext ContextID, cmsUInt32Number size);
    void* _cmsMalloc(cmsContext ContextID, cmsUInt32Number size);
    void  _cmsFree(cmsContext ContextID, void* Ptr);
}


// Simple non-terminating assertion macro.
// It prints a diagnostic message on failure but does not abort the test run.
#define CHECK(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Test Failure: " << (msg) \
                  << " (Line " << __LINE__ << ")" << std::endl; \
        /* Do not return; continue executing to maximize coverage. */ \
        failures++; \
    } else { \
        passes++; \
    } \
} while (0)

// Global counters to summarize test results.
static int passes = 0;
static int failures = 0;

// Helper to print a short summary
static void print_summary(const char* heading) {
    std::cout << heading << " - Passed: " << passes << ", Failed: " << failures << std::endl;
}

// Test 1: Default "Initial = 0" should allocate 20 * 1024 bytes and initialize fields.
static void test_CreateSubAllocChunk_DefaultInitial() {
    std::cout << "Running test: Default Initial (Initial == 0)" << std::endl;
    cmsContext ctx = nullptr;

    // Call the function under test
    _cmsSubAllocator_chunk* chunk = _cmsCreateSubAllocChunk(ctx, 0);

    // Access internal fields to verify correctness
    CHECK(chunk != nullptr, "chunk should not be NULL when Initial == 0");
    if (chunk != nullptr) {
        CHECK(chunk->Block != nullptr, "chunk->Block should not be NULL");
        const cmsUInt32Number expectedSize = 20 * 1024;
        CHECK(chunk->BlockSize == expectedSize, "BlockSize should be 20 * 1024 when Initial == 0");
        CHECK(chunk->Used == 0, "Used should be 0 after initialization");
        CHECK(chunk->next == nullptr, "next should be NULL after initialization");

        // Cleanup allocated resources to avoid memory leaks in tests
        if (chunk->Block) {
            _cmsFree(ctx, chunk->Block);
        }
        _cmsFree(ctx, chunk);
    }

    print_summary("Test 1 (Default Initial)");
}

// Test 2: Non-zero Initial should allocate exactly that many bytes and initialize fields.
static void test_CreateSubAllocChunk_CustomInitial() {
    std::cout << "Running test: Custom Initial (Initial != 0)" << std::endl;
    cmsContext ctx = nullptr;
    const cmsUInt32Number customInitial = 64 * 1024; // 64 KB

    _cmsSubAllocator_chunk* chunk = _cmsCreateSubAllocChunk(ctx, customInitial);

    CHECK(chunk != nullptr, "chunk should not be NULL when Initial != 0");
    if (chunk != nullptr) {
        CHECK(chunk->Block != nullptr, "chunk->Block should not be NULL");
        CHECK(chunk->BlockSize == customInitial, "BlockSize should match the passed Initial value");
        CHECK(chunk->Used == 0, "Used should be 0 after initialization");
        CHECK(chunk->next == nullptr, "next should be NULL after initialization");

        // Cleanup
        if (chunk->Block) {
            _cmsFree(ctx, chunk->Block);
        }
        _cmsFree(ctx, chunk);
    }

    print_summary("Test 2 (Custom Initial)");
}

// Test 3: Creating two chunks with Initial == 0 should allocate distinct blocks.
// This verifies that separate allocations do not alias each other.
static void test_CreateSubAllocChunk_DistinctAllocations() {
    std::cout << "Running test: Distinct allocations for two chunks (Initial == 0)" << std::endl;
    cmsContext ctx = nullptr;

    _cmsSubAllocator_chunk* chunk1 = _cmsCreateSubAllocChunk(ctx, 0);
    _cmsSubAllocator_chunk* chunk2 = _cmsCreateSubAllocChunk(ctx, 0);

    CHECK(chunk1 != nullptr, "First chunk should not be NULL");
    if (chunk1 != nullptr) {
        CHECK(chunk1->Block != nullptr, "First chunk block should not be NULL");
    }

    CHECK(chunk2 != nullptr, "Second chunk should not be NULL");
    if (chunk2 != nullptr) {
        CHECK(chunk2->Block != nullptr, "Second chunk block should not be NULL");
    }

    // If both chunks exist, ensure they are distinct in both the chunk struct and the data block
    if (chunk1 != nullptr && chunk2 != nullptr) {
        CHECK(chunk1 != chunk2, "Two distinct chunks should not be the same pointer");
        if (chunk1->Block != nullptr && chunk2->Block != nullptr) {
            CHECK(chunk1->Block != chunk2->Block, "Block pointers should be distinct between chunks");
        }
    }

    // Cleanup
    if (chunk1) {
        if (chunk1->Block) _cmsFree(ctx, chunk1->Block);
        _cmsFree(ctx, chunk1);
    }
    if (chunk2) {
        if (chunk2->Block) _cmsFree(ctx, chunk2->Block);
        _cmsFree(ctx, chunk2);
    }

    print_summary("Test 3 (Distinct Allocations)");
}

int main() {
    // Run tests
    test_CreateSubAllocChunk_DefaultInitial();
    test_CreateSubAllocChunk_CustomInitial();
    test_CreateSubAllocChunk_DistinctAllocations();

    // Summary
    std::cout << "====================" << std::endl;
    std::cout << "Total tests run: " << (passes + failures) << std::endl;
    print_summary("Final Result");
    // Return non-zero if any test failed to aid automation
    return (failures > 0) ? 1 : 0;
}