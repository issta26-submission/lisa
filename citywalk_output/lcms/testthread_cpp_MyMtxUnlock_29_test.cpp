#include <windows.h>
#include <iostream>
#include <lcms2_plugin.h>


// Declaration of the focal function under test.
// The real implementation is provided in the project under test.
// We declare with C linkage to match potential linkage expectations.
extern "C" void MyMtxUnlock(cmsContext id, void* mtx);

/*
  Lightweight, non-terminating test harness (no GTest) for the focal method:
  void MyMtxUnlock(cmsContext id, void* mtx)

  The goal is to exercise mutex unlock behavior via the Windows API:
  - When the current thread owns the mutex, unlocking should allow immediate re-acquisition.
  - When the current thread does not own the mutex, unlocking should fail (ERROR_NOT_OWNER).
  - Unlocking with an invalid handle should fail with ERROR_INVALID_HANDLE.

  All tests are designed to be robust in a multi-threaded environment and do not
  abort on failure; instead they report results and proceed to maximize code coverage.
*/

// Helper to indicate a test result
static void logTestResult(const char* testName, bool passed, const char* extra = nullptr)
{
    if (passed) {
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cout << "[FAIL] " << testName;
        if (extra) std::cout << " - " << extra;
        std::cout << "\n";
    }
}

// Test 1: When the mutex is owned by the current thread, MyMtxUnlock should unlock,
// allowing the thread to immediately reacquire it (using a zero-timeout wait).
bool test_owned_unlock_allows_reacquire() {
    const char* testName = "test_owned_unlock_allows_reacquire";
    bool ok = true;

    // Create a unnamed mutex
    HANDLE h = CreateMutex(NULL, FALSE, NULL);
    if (h == NULL) {
        logTestResult(testName, false, "CreateMutex failed");
        return false;
    }

    // Acquire the mutex (ownership by this thread)
    DWORD rc = WaitForSingleObject(h, INFINITE);
    if (rc != WAIT_OBJECT_0) {
        logTestResult(testName, false, "Failed to acquire mutex initially");
        CloseHandle(h);
        return false;
    }

    // Unlock using the focal function
    MyMtxUnlock(NULL, (void*)h);

    // After unlocking, this thread should be able to reacquire immediately (zero timeout)
    rc = WaitForSingleObject(h, 0);
    if (rc != WAIT_OBJECT_0) {
        logTestResult(testName, false, "Unlock did not allow immediate reacquire with 0 timeout");
        ok = false;
    } else {
        // Reacquired; release to leave the mutex unowned again
        ReleaseMutex(h);
    }

    CloseHandle(h);
    logTestResult(testName, ok);
    return ok;
}

// Test 2: If the current thread does not own the mutex, MyMtxUnlock should fail
// and set the last error to ERROR_NOT_OWNER.
bool test_not_owned_unlock_sets_last_error() {
    const char* testName = "test_not_owned_unlock_sets_last_error";
    bool ok = true;

    HANDLE h = CreateMutex(NULL, FALSE, NULL);
    if (h == NULL) {
        logTestResult(testName, false, "CreateMutex failed");
        return false;
    }

    // Ensure last error is cleared before test
    SetLastError(0);
    // Do NOT acquire the mutex; attempt unlock from a thread that doesn't own it
    MyMtxUnlock(NULL, (void*)h);

    DWORD gle = GetLastError();
    if (gle != ERROR_NOT_OWNER) {
        logTestResult(testName, false, ("Expected ERROR_NOT_OWNER, got " + std::to_string(gle)).c_str());
        ok = false;
    }

    CloseHandle(h);
    logTestResult(testName, ok);
    return ok;
}

// Test 3: Double unlock from a thread that does not own the mutex should again yield
// ERROR_NOT_OWNER. This validates stability of error reporting across repeated calls.
bool test_not_owner_double_unlock_sets_error() {
    const char* testName = "test_not_owner_double_unlock_sets_error";
    bool ok = true;

    HANDLE h = CreateMutex(NULL, FALSE, NULL);
    if (h == NULL) {
        logTestResult(testName, false, "CreateMutex failed");
        return false;
    }

    // First unlock attempt (not owner)
    SetLastError(0);
    MyMtxUnlock(NULL, (void*)h);
    DWORD gle1 = GetLastError();
    if (gle1 != ERROR_NOT_OWNER) {
        logTestResult(testName, false, "First unlock not owner did not report ERROR_NOT_OWNER");
        ok = false;
    }

    // Second unlock attempt (still not owner)
    SetLastError(0);
    MyMtxUnlock(NULL, (void*)h);
    DWORD gle2 = GetLastError();
    if (gle2 != ERROR_NOT_OWNER) {
        logTestResult(testName, false, "Second unlock not owner did not report ERROR_NOT_OWNER");
        ok = false;
    }

    CloseHandle(h);
    logTestResult(testName, ok);
    return ok;
}

// Test 4: Unlocking with an invalid handle should fail with ERROR_INVALID_HANDLE.
bool test_unlock_invalid_handle_sets_error() {
    const char* testName = "test_unlock_invalid_handle_sets_error";
    bool ok = true;

    // Use an obviously invalid handle value
    void* invalidHandle = reinterpret_cast<void*>(0xDEADBEEF);

    SetLastError(0);
    MyMtxUnlock(NULL, invalidHandle);
    DWORD gle = GetLastError();
    if (gle != ERROR_INVALID_HANDLE) {
        logTestResult(testName, false, ("Expected ERROR_INVALID_HANDLE, got " + std::to_string(gle)).c_str());
        ok = false;
    }

    logTestResult(testName, ok);
    return ok;
}

// Entry point: execute all tests and report a final summary.
// Per the domain knowledge instruction, tests should be invoked from main when not using GTest.
int main() {
    bool all_passed =
        test_owned_unlock_allows_reacquire() &&
        test_not_owned_unlock_sets_last_error() &&
        test_not_owner_double_unlock_sets_error() &&
        test_unlock_invalid_handle_sets_error();

    if (all_passed) {
        std::cout << "[SUMMARY] All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "[SUMMARY] Some tests FAILED.\n";
        return 1;
    }
}