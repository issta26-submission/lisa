// Unit test suite for lsm_default_env in lsm_unix.c
// Purpose: verify that the focal method returns the expected environment struct
// and that key properties/initializations behave as intended.
// Notes:
// - This test avoids GTest and uses a lightweight, non-terminating assertion style.
// - We rely on the provided C interface headers (lsmInt.h) and the C linkage of the
//   lsm library. Function pointers in lsm_env are not exhaustively compared to
//   every implementation detail to keep tests robust across platforms.
// - Static singleton behavior is verified by comparing pointer identities across calls.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <functional>
#include <sys/types.h>
#include <lsmInt.h>
#include <assert.h>
#include <cstddef>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <pthread.h>
#include <sys/mman.h>


// Include the library's public interface header which defines lsm_env and prototypes.
extern "C" {
}

// It is assumed that lsm_default_env is declared in the public header.
// If not, declare it explicitly here for the test compilation.
extern "C" lsm_env *lsm_default_env(void);

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void report_test(const std::string &name, bool passed) {
    g_total_tests++;
    if (passed) {
        std::cout << "[PASS] " << name << "\n";
    } else {
        g_failed_tests++;
        std::cerr << "[FAIL] " << name << "\n";
    }
}

int main() {
    // Test 1: Singleton behavior - lsm_default_env should return the same pointer on repeated calls.
    {
        lsm_env *e1 = lsm_default_env();
        lsm_env *e2 = lsm_default_env();
        bool ok = (e1 != nullptr) && (e1 == e2);
        report_test("Singleton: lsm_default_env returns identical pointer on successive calls", ok);
    }

    // Test 2: Basic field initialization checks.
    // - nByte should be sizeof(lsm_env)
    // - iVersion should be 1
    // - pVfsCtx should be 0
    {
        lsm_env *env = lsm_default_env();
        bool ok = true;
        if (env == nullptr) {
            ok = false;
        } else {
            ok &= (static_cast<size_t>(env->nByte) == sizeof(lsm_env));
            ok &= (env->iVersion == 1);
            ok &= (env->pVfsCtx == 0);
        }
        report_test("Basic fields: nByte, iVersion, pVfsCtx initialization", ok);
    }

    // Test 3: Essential function pointers are non-null.
    // We avoid asserting exact identity to internal static functions to maintain portability.
    // Just ensure that the environment is wired with valid handlers for core I/O and memory ops.
    {
        lsm_env *env = lsm_default_env();
        bool ok = true;
        if (env == nullptr) {
            ok = false;
        } else {
            ok &= (env->xFullpath != nullptr);
            ok &= (env->xOpen != nullptr);
            ok &= (env->xRead != nullptr);
            ok &= (env->xWrite != nullptr);
            ok &= (env->xTruncate != nullptr);
            ok &= (env->xSync != nullptr);
            ok &= (env->xSectorSize != nullptr);
            ok &= (env->xRemap != nullptr);
            ok &= (env->xFileid != nullptr);
            ok &= (env->xClose != nullptr);
            ok &= (env->xUnlink != nullptr);
            ok &= (env->xLock != nullptr);
            ok &= (env->xTestLock != nullptr);
            ok &= (env->xShmMap != nullptr);
            ok &= (env->xShmBarrier != nullptr);
            ok &= (env->xShmUnmap != nullptr);
            ok &= (env->pMemCtx == 0); // static expectation from initializer
            ok &= (env->xMalloc != nullptr);
            ok &= (env->xRealloc != nullptr);
            ok &= (env->xFree != nullptr);
            ok &= (env->xSize != nullptr);
            ok &= (env->xSleep != nullptr);
        }
        report_test("Pointer non-null checks for essential operations in lsm_env", ok);
    }

    // Test 4: Static singleton stability check (address equality across calls).
    // Redundant with Test 1, but reinforces the static nature of the environment.
    {
        lsm_env *a = lsm_default_env();
        lsm_env *b = lsm_default_env();
        bool ok = (a == b);
        report_test("Static singleton stability: repeated addresses match", ok);
    }

    // Summary
    std::cout << "\nTest Summary: " << g_total_tests << " tests run, "
              << g_failed_tests << " failures.\n";

    return (g_failed_tests == 0) ? 0 : 1;
}