// Test suite for cmsBool MyMtxLock(cmsContext id, void* mtx)
// Focus: verify immediate acquisition and blocking behavior using a real Windows mutex.
// The tests are designed to be run in a Windows environment (Win32 API).
// This file should be compiled together with the focal implementation (testthread.cpp).

#include <windows.h>
#include <iostream>
#include <cstdio>
#include <lcms2_plugin.h>


// Include types used by the focal method (cmsBool, cmsContext, etc.)
// The focal class dependencies indicate inclusion of lcms2_plugin.h.

// Forward declaration of the focal function under test
// Note: The function is expected with C linkage; ensure proper linkage in the build.
extern "C" cmsBool MyMtxLock(cmsContext id, void* mtx);

// Global shared state for test 2 (blocking scenario)
static volatile int g_test2_result = 0; // 0 = not finished, 1 = success

// Test helper: owner thread acquires the mutex and holds it for a short duration
// to simulate a locked state that blocks the MyMtxLock call from another thread.
DWORD WINAPI owner_lock(LPVOID param)
{
    HANDLE mtx = (HANDLE)param;
    // Acquire the mutex (becomes owned by this thread)
    WaitForSingleObject(mtx, INFINITE);
    // Hold the mutex briefly to simulate work
    Sleep(600);
    // Release the mutex to allow other threads to proceed
    ReleaseMutex(mtx);
    return 0;
}

// Test helper: tester thread calls the focal function MyMtxLock and records result
DWORD WINAPI tester_lock(LPVOID param)
{
    HANDLE mtx = (HANDLE)param;
    cmsBool res = MyMtxLock(NULL, mtx);
    g_test2_result = (res != 0) ? 1 : 0;
    return 0;
}

// Simple non-terminating assertion macro (print-only on failure)
#define ASSERT_NT(cond, msg) do { if(!(cond)) { std::cerr << "ASSERT FAILED: " << (msg) << "\n"; } } while(0)

// Test 1: Immediate acquire
// - Create an unlocked mutex and call MyMtxLock.
// - MyMtxLock should return quickly (non-blocking path) and yield TRUE.
bool test_MyMtxLock_immediate_acquire()
{
    // Candidate Keywords: WaitForSingleObject, HANDLE, mutex, cmsContext, cmsBool, TRUE
    HANDLE mtx = CreateMutex(NULL, FALSE, NULL);
    if (mtx == NULL) {
        std::cerr << "CreateMutex failed in test_MyMtxLock_immediate_acquire\n";
        return false;
    }

    cmsBool r = MyMtxLock(NULL, (void*)mtx);
    bool ok = (r != 0);

    CloseHandle(mtx);
    // Explanatory comment:
    // This test ensures that when the mutex is initially unlocked, the
    // WaitForSingleObject inside MyMtxLock returns immediately and the
    // function returns TRUE.
    return ok;
}

// Test 2: Blocking behavior
// - Create a mutex, spawn an owner thread that acquires and holds it.
// - Spawn a tester thread that calls MyMtxLock on the same mutex (should block).
// - Release the mutex from the owner thread and verify tester completes with TRUE.
bool test_MyMtxLock_blocks_until_released()
{
    // Candidate Keywords: WaitForSingleObject, HANDLE, mtx, cmsContext, cmsBool
    HANDLE mtx = CreateMutex(NULL, FALSE, NULL);
    if (mtx == NULL) {
        std::cerr << "CreateMutex failed in test_MyMtxLock_blocks_until_released\n";
        return false;
    }

    // Start owner thread that will acquire the mutex
    DWORD tid_owner;
    HANDLE hOwner = CreateThread(NULL, 0, owner_lock, (LPVOID)mtx, 0, &tid_owner);
    if (hOwner == NULL) {
        std::cerr << "CreateThread (owner) failed\n";
        CloseHandle(mtx);
        return false;
    }

    // Start tester thread that will call MyMtxLock (should block until owner releases)
    g_test2_result = 0;
    DWORD tid_tester;
    HANDLE hTester = CreateThread(NULL, 0, tester_lock, (LPVOID)mtx, 0, &tid_tester);
    if (hTester == NULL) {
        std::cerr << "CreateThread (tester) failed\n";
        // Cleanup
        WaitForSingleObject(hOwner, INFINITE);
        CloseHandle(hOwner);
        CloseHandle(mtx);
        return false;
    }

    // Give tester a moment to start and block on the mutex
    // Ensure tester is blocked by giving some time before releasing mutex
    // (Owner will release after 600ms, so tester should complete after that)
    Sleep(50);

    // Wait for tester to finish with a reasonable timeout
    DWORD waitStat = WaitForSingleObject(hTester, 2000);
    // Wait for owner to finish and cleanup
    WaitForSingleObject(hOwner, INFINITE);

    // Cleanup handles
    CloseHandle(hTester);
    CloseHandle(hOwner);
    CloseHandle(mtx);

    // Explanatory comment:
    // If MyMtxLock correctly waits on the provided HANDLE until it is signaled,
    // the tester thread should complete and g_test2_result should be 1.
    bool passed = (waitStat == WAIT_OBJECT_0) && (g_test2_result == 1);
    return passed;
}

// Minimal test harness (no gtest). Call test methods from main as guided by <DOMAIN_KNOWLEDGE>.
// Reports results and exits with non-zero code on failure.
int main()
{
    int tests_run = 0;
    int tests_failed = 0;

    std::cout << "Running unit tests for cmsBool MyMtxLock...\n";

    // Test 1: Immediate acquire
    bool t1 = test_MyMtxLock_immediate_acquire();
    tests_run++;
    if (t1) {
        std::cout << "[PASS] test_MyMtxLock_immediate_acquire\n";
    } else {
        std::cout << "[FAIL] test_MyMtxLock_immediate_acquire\n";
        tests_failed++;
    }

    // Test 2: Blocking behavior
    bool t2 = test_MyMtxLock_blocks_until_released();
    tests_run++;
    if (t2) {
        std::cout << "[PASS] test_MyMtxLock_blocks_until_released\n";
    } else {
        std::cout << "[FAIL] test_MyMtxLock_blocks_until_released\n";
        tests_failed++;
    }

    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << "\n";

    // Return non-zero if any test failed (as a simple pass/fail signal)
    return (tests_failed != 0) ? 1 : 0;
}