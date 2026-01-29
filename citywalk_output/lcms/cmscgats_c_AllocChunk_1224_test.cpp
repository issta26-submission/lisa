/*
Unit Test Suite for AllocChunk in cmscgats.c
- Target: AllocChunk(cmsIT8* it8, cmsUInt32Number size)
- Environment: C++11, no GoogleTest. Tests are executed from main().
- Approach: Use real library types and functions. Create minimal cmsIT8 instances
  with required fields to exercise AllocChunk; verify both block reuse and new block
  allocation paths. Non-terminating assertions (log on failure but continue) are used
  to maximize code coverage.
- Notes:
  - This test relies on the internal layout of cmsIT8 as referenced by the provided
    focal class dependencies (Allocator.BlockSize, Allocator.Used, Allocator.Block).
  - The test does not depend on private methods; it exercises AllocChunk through its
    public-looking interface and the actual AllocBigBlock backend.
  - The tests should be compiled and linked together with the library sources that define
    AllocChunk and AllocBigBlock (e.g., cmscgats.c and the rest of LittleCMS).
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>


// Include internal headers to access cmsIT8 and AllocChunk signature.
// Using extern "C" to ensure proper linkage for C API when compiling as C++.
extern "C" {
void* AllocChunk(cmsIT8* it8, cmsUInt32Number size); // Provided by cmscgats.c
}

// Lightweight non-terminating test framework (log on failure, continue)
static int g_testFailures = 0;
static int g_testsRun = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "TEST FAILURE: " << (msg) << " [" << __FILE__ << ":" << __LINE__ << "]\n"; \
            ++g_testFailures; \
        } \
        ++g_testsRun; \
    } while(0)

// Helper to create a minimal cmsIT8 instance with a zeroed allocator structure
static cmsIT8* CreateCmsIT8() {
    cmsIT8* it8 = (cmsIT8*)calloc(1, sizeof(cmsIT8));
    if (it8 == nullptr) return nullptr;
    // Initialize allocator sub-structure fields that AllocChunk touches.
    // The exact layout is assumed from the provided dependencies:
    // it8->Allocator.BlockSize, it8->Allocator.Used, it8->Allocator.Block
    // Ensure they start in a known state.
    it8->Allocator.Block = nullptr;
    it8->Allocator.BlockSize = 0;
    it8->Allocator.Used = 0;
    return it8;
}

// Helper to create a raw memory block for simulating an initial Block
static cmsUInt8Number* AllocateRawBlock(std::size_t sz) {
    return (cmsUInt8Number*)std::malloc(sz);
}

// Test 1: AllocChunk with size = 0 should return NULL (no allocation performed)
static void testAllocChunk_ZeroSize() {
    cmsIT8* it8 = CreateCmsIT8();
    TEST_ASSERT(it8 != nullptr, "Failed to allocate cmsIT8 for testAllocChunk_ZeroSize");

    void* res = AllocChunk(it8, 0);

    TEST_ASSERT(res == NULL, "AllocChunk should return NULL when requested size is 0");

    // Clean up any memory that might have been allocated ( AllocChunk returns NULL so nothing allocated )
    std::free(it8);
}

// Test 2: First allocation (BlockSize == 0) should allocate a new big block (20KB) and
//      set Used to the aligned size, and return pointer to the start of the new block.
static void testAllocChunk_FirstBlockAllocation() {
    cmsIT8* it8 = CreateCmsIT8();
    TEST_ASSERT(it8 != nullptr, "Failed to allocate cmsIT8 for testAllocChunk_FirstBlockAllocation");

    // Prepare an empty allocator state
    it8->Allocator.Block = nullptr;
    it8->Allocator.BlockSize = 0;
    it8->Allocator.Used = 0;

    cmsUInt32Number requested = 100; // multiple of 4 to avoid alignment surprises
    void* p = AllocChunk(it8, requested);

    // Expectations
    TEST_ASSERT(p != NULL, "AllocChunk should return a non-NULL pointer on first allocation");
    TEST_ASSERT(it8->Allocator.Block != NULL, "Allocator.Block should be non-NULL after first allocation");
    TEST_ASSERT(it8->Allocator.BlockSize == 20 * 1024, "BlockSize should be set to 20*1024 on first allocation");
    // Alignment for 100 is likely 100 (since 100 % 4 == 0). Used should reflect aligned size.
    TEST_ASSERT(it8->Allocator.Used == requested, "Used should equal the requested size after first allocation (no extra alignment assumed for multiples of 4)");
    TEST_ASSERT(p == (void*)it8->Allocator.Block, "Returned pointer should be the start of the allocated block (Block + 0)");

    std::free(it8); // Note: If AllocBigBlock allocated memory that is not freed here, it's acceptable for unit test scope.
}

// Test 3: Reuse existing block when enough Free space is available
// Setup a block of 1024 with 256 already used, request 128. Should not allocate a new block.
static void testAllocChunk_SufficientFree() {
    cmsIT8* it8 = CreateCmsIT8();
    TEST_ASSERT(it8 != nullptr, "Failed to allocate cmsIT8 for testAllocChunk_SufficientFree");

    // Prepare an existing block
    cmsUInt8Number* existing = AllocateRawBlock(1024);
    TEST_ASSERT(existing != nullptr, "Failed to allocate existing memory block for testAllocChunk_SufficientFree");

    it8->Allocator.Block = existing;
    it8->Allocator.BlockSize = 1024;
    it8->Allocator.Used = 256;

    cmsUInt32Number requested = 128;
    void* p = AllocChunk(it8, requested);

    // Expectations
    // pointer should be at Block + 256
    TEST_ASSERT(p == (void*)(existing + 256), "Returned pointer should point to Block + previous Used (256)");
    TEST_ASSERT(it8->Allocator.Block == existing, "Block should remain the same when there is enough free space");
    TEST_ASSERT(it8->Allocator.BlockSize == 1024, "BlockSize should remain unchanged when reusing existing block");
    TEST_ASSERT(it8->Allocator.Used == 256 + requested, "Used should increase by the aligned size of the request");

    std::free(it8); // Note: persistent allocated area by AllocChunk is not freed here to keep test self-contained.
}

// Test 4: When Free is insufficient, AllocChunk should allocate a new block by doubling size
// and then allocate from the new block.
static void testAllocChunk_BlockSwitchWhenFull() {
    cmsIT8* it8 = CreateCmsIT8();
    TEST_ASSERT(it8 != nullptr, "Failed to allocate cmsIT8 for testAllocChunk_BlockSwitchWhenFull");

    // Prepare an existing small block to trigger a switch
    cmsUInt8Number* oldBlock = AllocateRawBlock(256);
    TEST_ASSERT(oldBlock != nullptr, "Failed to allocate old block for testAllocChunk_BlockSwitchWhenFull");

    it8->Allocator.Block = oldBlock;
    it8->Allocator.BlockSize = 256;
    it8->Allocator.Used = 128;

    cmsUInt32Number requested = 320; // > Free (128)
    void* p = AllocChunk(it8, requested);

    // Expectations
    TEST_ASSERT(p != NULL, "AllocChunk should return a non-NULL pointer after allocating a new block");
    TEST_ASSERT(it8->Allocator.Block != oldBlock, "Block should have switched to a new allocation");
    TEST_ASSERT(it8->Allocator.BlockSize == 512, "BlockSize should double to 512 after switching blocks");
    TEST_ASSERT(it8->Allocator.Used == requested, "Used should reflect the allocated size in the new block");
    TEST_ASSERT(p == (void*)it8->Allocator.Block, "Returned pointer should be the start of the new block (since Used starts at 0)");

    std::free(it8); // Cleanup
}

// Main entry: run all tests
int main() {
    std::cout << "Running AllocChunk unit tests (CMS CGATS):\n";

    testAllocChunk_ZeroSize();
    testAllocChunk_FirstBlockAllocation();
    testAllocChunk_SufficientFree();
    testAllocChunk_BlockSwitchWhenFull();

    if (g_testFailures == 0) {
        std::cout << "All tests passed (" << g_testsRun << " tests).\n";
        return 0;
    } else {
        std::cerr << g_testFailures << " test(s) failed out of " << g_testsRun << ".\n";
        return 1;
    }
}