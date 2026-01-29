/*
Step 1 & 2 Notes (embedded as comments for context)
- Focal method under test: unity_free(void* mem)
  - Behavior:
    1) If mem == NULL, return immediately.
    2) Else call isOverrun(mem), then release_memory(mem).
    3) If overrun was detected, report a failure via UNITY_TEST_FAIL(Unity.CurrentTestLineNumber, "Buffer overrun detected during free()").
- Key dependencies (as seen in the focal class):
  - isOverrun(void*) [static in Unity's file, used by unity_free]
  - release_memory(void*) [static in Unity's file, used by unity_free]
  - unity_malloc(size_t) and UnityTest framework facilities (UnityMain, RUN_TEST, etc.)
- Domain knowledge considerations:
  - We’ll test the NULL branch explicitly; we’ll also exercise a non-NULL path by allocating memory with Unity’s allocator and freeing it, ensuring no crash or unexpected fail is triggered under nominal conditions.
  - Since isOverrun is an internal static function, deterministic control over its return value isn’t exposed from tests outside the Unity fixture file. We therefore rely on the non-NULL path to verify that the function runs without immediate error, leaving overrun-specific behavior to the internal Unity test harness.
- This test file uses the Unity framework (unity_fixture.h) and calls into unity_free/unity_malloc via extern "C" linkage from C++, as the project under test is C code.

The following test code is designed to be compiled with C++11 and linked against the provided Unity fixture (unity_fixture.c/.h) with no GTest usage.
*/

#include <unity_internals.h>
#include <cstddef>
#include <cstring>
#include <stdlib.h>
#include <string.h>
#include <unity_fixture.h>


extern "C" {
    // Declarations of the C functions provided by the Unity fixture
    void unity_free(void* mem);
    void* unity_malloc(size_t size);
    int UnityMain(int argc, const char* argv[], void (*runAllTests)(void));
}


// Forward declare the test runner to be used by UnityMain
static void runAllTests(void);

/*
Test Group: UnityFreeTests
- Verifies behavior of unity_free() in two scenarios:
  1) NULL pointer input -> should return immediately without side effects or failures.
  2) Non-NULL pointer input -> allocate memory via Unity's allocator, free it, and ensure no immediate failure occurs.
Note: Overrun-specific behavior is exercised by Unity's internal mechanisms; we validate that the non-NULL path executes without causing a test failure in normal conditions.
*/

TEST_GROUP(UnityFreeTests);

TEST_SETUP(UnityFreeTests)
{
    // No setup required for these tests
}

TEST_TEAR_DOWN(UnityFreeTests)
{
    // No teardown required for these tests
}

/*
Test: NullPointerReturnsDoesNotCrash
- Purpose: Ensure that passing NULL to unity_free(mem) causes an early return
  without performing any further operations or triggering failures.
- Rationale: Matches the first branch of the predicate: if (mem == NULL) { return; }
*/
TEST(UnityFreeTests, NullPointerReturnsDoesNotCrash)
{
    unity_free(nullptr);   // Act
    // If we reach this point, no crash occurred and the function returned early.
    TEST_ASSERT_TRUE(true);  // Explicit assertion to mark test as passed
}

/*
Test: FreeNonNullPointerMemoryAllocatedAndFreed
- Purpose: Allocate a block using Unity's allocator and free it via unity_free(mem).
- Rationale: Exercises the non-NULL path, which internally calls isOverrun(mem) and release_memory(mem).
  Under nominal conditions, this should complete without triggering a UNITY_TEST_FAIL.
*/
TEST(UnityFreeTests, FreeNonNullPointerMemoryAllocatedAndFreed)
{
    void* mem = unity_malloc(32);
    TEST_ASSERT_NOT_NULL(mem);

    // Initialize memory to a recognizable pattern (helps ensure memory is writable)
    std::memset(mem, 0xAB, 32);

    // Act
    unity_free(mem);

    // If Unity did not flag a failure for the non-NULL path, the test passes.
    TEST_ASSERT_TRUE(true);
}

/*
Test runner: registers all tests in this group
*/
static void runAllTests(void)
{
    RUN_TEST(UnityFreeTests, NullPointerReturnsDoesNotCrash);
    RUN_TEST(UnityFreeTests, FreeNonNullPointerMemoryAllocatedAndFreed);
}

/*
Main entry point that delegates to Unity's test runner
*/
int main(int argc, const char* argv[])
{
    // Cast argv to the const-char* array expected by UnityMain
    return UnityMain(argc, argv, runAllTests);
}