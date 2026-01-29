// Test suite for the focal method testMallocInstall(lsm_env* pEnv)
// This test is written for C++11 without GTest, using a lightweight in-process harness.
// It exercises the installation path and validates the post-conditions established by testMallocInstall.
// Explanatory comments are provided for each test case.
//
// How this test works in short:
// - Provides a minimal, mock environment (lsm_env) with required callbacks.
// - Supplies concrete implementations for memory/locking primitives (tmLsmMalloc, tmLsmEnvMalloc, etc.).
// - Invokes testMallocInstall and then asserts on the resulting state (pEnv, pGlobal, mutex, etc.).
// - Prints a readable result summary without terminating the process on non-fatal assertion.
// - Keeps memory clean by releasing any allocated mock mutex handles at the end.

#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <lsm.h>
#include <cassert>


// Include the project header to obtain definitions for lsm_env, TmGlobal, LsmMutex, etc.
extern "C" {
}

// Forward declaration of the focal method under test (as it resides in the focal code path).
extern "C" void testMallocInstall(lsm_env *pEnv);

// Lightweight test harness helpers
#define TEST_PASS true
#define TEST_FAIL false

struct TestResult {
    const char* testName;
    bool passed;
    const char* message;
};

// Global storage for basic memory mocks to facilitate simple asserts
static void* g_mutexHandle = nullptr;

// --- Minimal C wrappers to enable testMallocInstall to operate in a controlled manner ---

// Realistic malloc wrappers (external linkage to resolve from testMallocInstall)
extern "C" void *tmLsmMalloc(int n) {
    return malloc(n);
}
extern "C" void *tmLsmRealloc(void *p, int n) {
    return realloc(p, n);
}
extern "C" void tmLsmFree(void *p) {
    free(p);
}

// Environment-aware wrappers (used by testMallocInstall to rebind env allocators)
extern "C" void *tmLsmEnvMalloc(lsm_env *pEnv, size_t n) {
    (void)pEnv;
    return malloc(n);
}
extern "C" void *tmLsmEnvRealloc(lsm_env *pEnv, void *p, size_t n) {
    (void)pEnv;
    return realloc(p, n);
}
extern "C" void tmLsmEnvFree(lsm_env *pEnv, void *p) {
    (void)pEnv;
    free(p);
}

// Mutex handling stubs (locker hooks used by testMallocInstall)
static void tmLsmMutexEnter(TmGlobal *pTm) { (void)pTm; }
static void tmLsmMutexLeave(TmGlobal *pTm) { (void)pTm; }
static void tmLsmMutexDel(TmGlobal *pTm) { (void)pTm; }

// A small, deterministic mutex-allocator used by the tests
extern "C" void lsm_myMutexStatic(lsm_env *pEnv, int type, void **ppMutex) {
    (void)pEnv;
    (void)type;
    // Allocate a tiny dummy handle to represent a mutex
    *ppMutex = malloc(16);
    g_mutexHandle = *ppMutex;
}

// Additional stubs to satisfy potential internal references within the focal code path
extern "C" void *tmMalloc(TmGlobal *pTm, int n) {
    (void)pTm;
    return malloc(n);
}
extern "C" void tmFree(TmGlobal *pTm, void *p) {
    (void)pTm;
    free(p);
}
extern "C" void *tmRealloc(TmGlobal *pTm, void *p, int n) {
    (void)pTm;
    return realloc(p, n);
}
extern "C" void tmEnterMutex(TmGlobal *pTm) { if (pTm && pTm->xEnterMutex) pTm->xEnterMutex(pTm); }
extern "C" void tmLeaveMutex(TmGlobal *pTm) { if (pTm && pTm->xLeaveMutex) pTm->xLeaveMutex(pTm); }

// Utility to perform a single-test run with a given environment
static TestResult run_basic_install_test() {
    TestResult result;
    result.testName = "testMallocInstall_basic_install";

    // Prepare a clean environment (as required by the focal function's first assertion)
    lsm_env env;
    std::memset(&env, 0, sizeof(env));
    // Precondition: pMemCtx must be 0
    env.pMemCtx = 0;

    // Pre-bind dummy allocator callbacks (they will be overridden by testMallocInstall)
    env.xMalloc = &tmLsmEnvMalloc;
    env.xRealloc = &tmLsmEnvRealloc;
    env.xFree = &tmLsmEnvFree;

    // Mutex facility provider
    env.xMutexStatic = &lsm_myMutexStatic;

    // Call the focal function under test
    testMallocInstall(&env);

    // Post-conditions:
    // 1) pMemCtx should be non-null and point to a valid TmGlobal
    if (env.pMemCtx == nullptr) {
        result.passed = TEST_FAIL;
        result.message = "FAILED: env.pMemCtx is null after testMallocInstall.";
        return result;
    }

    TmGlobal *pGlobal = (TmGlobal *)env.pMemCtx;
    if (pGlobal == nullptr) {
        result.passed = TEST_FAIL;
        result.message = "FAILED: env.pMemCtx does not point to a valid TmGlobal.";
        return result;
    }

    // 2) Confirm that the library-selected allocator hooks are wired correctly
    if (pGlobal->xMalloc != tmLsmMalloc) {
        result.passed = TEST_FAIL;
        result.message = "FAILED: pGlobal->xMalloc is not tmLsmMalloc as expected.";
        return result;
    }
    if (pGlobal->xRealloc != tmLsmRealloc) {
        result.passed = TEST_FAIL;
        result.message = "FAILED: pGlobal->xRealloc is not tmLsmRealloc as expected.";
        return result;
    }
    if (pGlobal->xFree != tmLsmFree) {
        result.passed = TEST_FAIL;
        result.message = "FAILED: pGlobal->xFree is not tmLsmFree as expected.";
        return result;
    }

    // 3) Validate that the environment now uses the "environment" allocators
    if (env.xMalloc != tmLsmEnvMalloc) {
        result.passed = TEST_FAIL;
        result.message = "FAILED: env.xMalloc was not updated to tmLsmEnvMalloc.";
        return result;
    }
    if (env.xRealloc != tmLsmEnvRealloc) {
        result.passed = TEST_FAIL;
        result.message = "FAILED: env.xRealloc was not updated to tmLsmEnvRealloc.";
        return result;
    }
    if (env.xFree != tmLsmEnvFree) {
        result.passed = TEST_FAIL;
        result.message = "FAILED: env.xFree was not updated to tmLsmEnvFree.";
        return result;
    }

    // 4) Basic structural checks on the mutex wiring (non-nullness and function pointers)
    if (pGlobal->pMutex == nullptr) {
        result.passed = TEST_FAIL;
        result.message = "FAILED: pGlobal->pMutex is null after installation.";
        return result;
    }
    if (pGlobal->xEnterMutex != tmLsmMutexEnter) {
        result.passed = TEST_FAIL;
        result.message = "FAILED: pGlobal->xEnterMutex is not tmLsmMutexEnter.";
        return result;
    }
    if (pGlobal->xLeaveMutex != tmLsmMutexLeave) {
        result.passed = TEST_FAIL;
        result.message = "FAILED: pGlobal->xLeaveMutex is not tmLsmMutexLeave.";
        return result;
    }
    if (pGlobal->xDelMutex != tmLsmMutexDel) {
        result.passed = TEST_FAIL;
        result.message = "FAILED: pGlobal->xDelMutex is not tmLsmMutexDel.";
        return result;
    }

    // All basic checks passed
    result.passed = TEST_PASS;
    result.message = "PASSED: testMallocInstall_basic_install completed successfully.";
    return result;
}

// Dummy signatures to satisfy potential linking in the focal code path (no-ops, as above)
extern "C" void tmLsmMutexEnter(TmGlobal *pTm) { (void)pTm; }
extern "C" void tmLsmMutexLeave(TmGlobal *pTm) { (void)pTm; }
extern "C" void tmLsmMutexDel(TmGlobal *pTm) { (void)pTm; }

// Entry point
int main() {
    // Run the basic test case for testMallocInstall
    TestResult res = run_basic_install_test();

    // Print a concise result with an explanation
    if (res.passed) {
        std::cout << "[OK] " << res.testName << ": " << res.message << std::endl;
        return 0;
    } else {
        std::cerr << "[ERR] " << res.testName << ": " << res.message << std::endl;
        return 1;
    }
}