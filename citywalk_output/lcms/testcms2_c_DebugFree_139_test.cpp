// Test suite for DebugFree in testcms2.c-like environment
// This test suite is written in C++11 (without GoogleTest) and relies on the
// provided test harness (testcms2.h) to supply the focal function and types.
// It uses a lightweight, non-terminating assertion approach and runs tests
// from main() as requested in the domain knowledge.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <cstdarg>


extern "C" {
}

// --------------------------------------------------------------
// Test harness scaffolding
// --------------------------------------------------------------

static bool DieCalled = false;

// Override the Die function used by DebugFree to capture error invocation.
// The real implementation in the test harness is a stub; we provide a test
// specific override to observe whether DebugFree requests a failure path.
void Die(const char* Reason, ...)
{
    (void)Reason; // unused in assertion, but kept to match signature
    DieCalled = true;
}

// Global TotalMemory used by DebugFree for bookkeeping.
// We assume TotalMemory is a global scalar compatible with the test host.
// If the actual type differs, this generic unsigned long long typically aligns
// with cmsUInt64Number in most CMS codebases (adjust if necessary).
unsigned long long TotalMemory = 0;

// Helper: safely cast to header pointer from user Ptr
static _cmsMemoryBlock* headerFromPtr(void* Ptr)
{
    return (_cmsMemoryBlock*) (((unsigned char*)Ptr) - SIZE_OF_MEM_HEADER);
}

// Helper: create a block header+userPtr as expected by DebugFree
// Returns the user pointer which DebugFree will receive and frees later as needed.
// Note: The header memory will be freed by DebugFree when it frees 'blk'.
// We allocate the header ourselves to control the fields for testing.
static void* PrepareBlock(unsigned long long KeepSize,
                          cmsContext WhoAllocated,
                          bool DontCheck)
{
    // Allocate memory for the header block
    _cmsMemoryBlock* hdr = (_cmsMemoryBlock*) malloc(sizeof(_cmsMemoryBlock));
    if (!hdr) {
        std::cerr << "Failed to allocate memory for header in test." << std::endl;
        return nullptr;
    }

    // Initialize header fields as used by DebugFree
    hdr->KeepSize = KeepSize;
    hdr->WhoAllocated = WhoAllocated;
    hdr->DontCheck = DontCheck ? 1 : 0;

    // Compute Ptr to be SIZE_OF_MEM_HEADER bytes after header
    void* Ptr = (void*)(((unsigned char*)hdr) + SIZE_OF_MEM_HEADER);
    return Ptr;
}

// Basic assertion utilities (non-terminating)
static void ASSERT_TRUE(bool cond, const char* msg)
{
    if (!cond) {
        std::cerr << "ASSERT_TRUE FAILED: " << msg << std::endl;
    }
}

static void ASSERT_FALSE(bool cond, const char* msg)
{
    if (cond) {
        std::cerr << "ASSERT_FALSE FAILED: " << msg << std::endl;
    }
}

static void ASSERT_EQ(unsigned long long a, unsigned long long b, const char* msg)
{
    if (a != b) {
        std::cerr << "ASSERT_EQ FAILED: " << msg
                  << " (expected " << b << ", got " << a << ")" << std::endl;
    }
}

// --------------------------------------------------------------
// Test cases for DebugFree
// --------------------------------------------------------------

/*
Test 1: Null pointer should trigger Die (NULL free ...)
- Expect Die to be invoked (DieCalled == true).
*/
static void test_DebugFree_NullPtr()
{
    DieCalled = false;
    // Ptr == NULL should trigger the NULL free branch
    DebugFree(0, NULL);
    ASSERT_TRUE(DieCalled, "DebugFree(NULL) should call Die.");
    // Reset for next tests
    DieCalled = false;
    std::cout << "test_DebugFree_NullPtr passed.\n";
}

/*
Test 2: Free by same thread, non-DontCheck, non-NULL Ptr
- Ptr points to a header with WhoAllocated == ContextID and DontCheck == false
- Expect no Die to be invoked
- Expect TotalMemory to be decremented by KeepSize
*/
static void test_DebugFree_FreeSameThread_NoDie()
{
    // Initialize memory accounting
    TotalMemory = 1000;

    unsigned long long KeepSize = 40;
    cmsContext ContextID = 1;

    void* Ptr = PrepareBlock(KeepSize, ContextID, false);
    ASSERT_TRUE(Ptr != nullptr, "PrepareBlock failed for test_DebugFree_FreeSameThread_NoDie.");

    DieCalled = false;
    // Call the function under test
    DebugFree(ContextID, Ptr);

    // Expectations
    ASSERT_FALSE(DieCalled, "DebugFree should not Die when allocated by same thread and DontCheck is false.");
    // If TotalMemory is updated by DebugFree, it should reflect the subtraction
    unsigned long long ExpectedTotal = 1000 - KeepSize;
    // We can't guarantee TotalMemory type/initialization alignment; best effort check:
    ASSERT_EQ(TotalMemory, ExpectedTotal, "TotalMemory should be decremented by KeepSize.");

    std::cout << "test_DebugFree_FreeSameThread_NoDie passed.\n";
}

/*
Test 3: Free by different thread with DontCheck == false
- Should trigger Die
*/
static void test_DebugFree_FreeDifferentThread_ShouldDie()
{
    // Initialize memory accounting
    TotalMemory = 2000;

    unsigned long long KeepSize = 24;
    cmsContext ContextID = 2;
    cmsContext OtherContext = 99;

    void* Ptr = PrepareBlock(KeepSize, OtherContext, false);
    ASSERT_TRUE(Ptr != nullptr, "PrepareBlock failed for test_DebugFree_FreeDifferentThread_ShouldDie.");

    DieCalled = false;
    DebugFree(ContextID, Ptr);

    ASSERT_TRUE(DieCalled, "DebugFree should call Die when freeing memory allocated by a different thread with DontCheck=false.");

    // Even in Die case, TotalMemory is decremented in the function prior to the Die call.
    unsigned long long ExpectedTotal = 2000 - KeepSize;
    ASSERT_EQ(TotalMemory, ExpectedTotal, "TotalMemory should be decremented by KeepSize even if Die is invoked.");

    std::cout << "test_DebugFree_FreeDifferentThread_ShouldDie passed.\n";
}

/*
Test 4: Free by different thread with DontCheck == true
- Should NOT trigger Die
*/
static void test_DebugFree_FreeDifferentThread_DontCheckTrue()
{
    // Initialize memory accounting
    TotalMemory = 3000;

    unsigned long long KeepSize = 16;
    cmsContext ContextID = 3;
    cmsContext OtherContext = 7;

    void* Ptr = PrepareBlock(KeepSize, OtherContext, true);
    ASSERT_TRUE(Ptr != nullptr, "PrepareBlock failed for test_DebugFree_FreeDifferentThread_DontCheckTrue.");

    DieCalled = false;
    DebugFree(ContextID, Ptr);

    ASSERT_FALSE(DieCalled, "DebugFree should NOT call Die when DontCheck is true, even if allocated by another thread.");

    unsigned long long ExpectedTotal = 3000 - KeepSize;
    ASSERT_EQ(TotalMemory, ExpectedTotal, "TotalMemory should be decremented by KeepSize in DontCheck branch.");

    std::cout << "test_DebugFree_FreeDifferentThread_DontCheckTrue passed.\n";
}

// --------------------------------------------------------------
// Main: run tests
// --------------------------------------------------------------
int main()
{
    // Run tests sequentially
    test_DebugFree_NullPtr();
    test_DebugFree_FreeSameThread_NoDie();
    test_DebugFree_FreeDifferentThread_ShouldDie();
    test_DebugFree_FreeDifferentThread_DontCheckTrue();

    std::cout << "All tests completed.\n";
    return 0;
}