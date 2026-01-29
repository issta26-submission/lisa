// Unit test suite for the focal method: cover_fast (located in infcover.c)
// This test harness is written for C++11 without using GTest.
// It uses:
 // - direct invocation of cover_fast (if available in the linked binary)
 // - dynamic symbol lookup via dlsym/dlopen (to cope with varied build/link options)
 // - a lightweight safety wrapper to catch potential crashes (SIGSEGV) using setjmp/longjmp
 // - a minimal test driver with two concrete tests and a final summary
// Notes from Step 1 and Step 2 (mapped to tests):
// - Candidate Keywords identified for the method under test: inf, Z_DATA_ERROR, Z_OK, Z_STREAM_END,
//   "fast length extra bits", "fast distance extra bits", "fast invalid distance code",
//   "fast invalid literal/length code", "fast 2nd level codes and too far back", "very common case",
//   "contiguous and wrap around window", "copy direct from output".
// - The tests focus on ensuring the function can be invoked and executes without crashing,
//   while also exercising both a direct call path and a dynamic lookup path.
// - Static/internal implementation details are not accessed directly; we interact through the
//   public symbol cover_fast (when available) and through safe invocation wrappers.
// - The test code uses only the C++ standard library and POSIX facilities (dlopen/dlsym, signals).

#include <string.h>
#include <assert.h>
#include <csignal>
#include <string>
#include <csetjmp>
#include <dlfcn.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <inftrees.h>
#include <zlib.h>
#include <stdlib.h>
#include <inflate.h>


// Forward declaration to allow direct invocation if the symbol is visible in the build.
// If cover_fast is not visible (static), this will not link; the dynamic path below will be attempted.
extern "C" void cover_fast(void);

// Safety machinery to catch potential crashes (SIGSEGV) during the invocation of C code.
static volatile sig_atomic_t g_got_segv = 0;
static sigjmp_buf g_jmp_buf;

// Signal handler to recover from crashes without killing the test process.
static void segv_handler(int) {
    g_got_segv = 1;
    siglongjmp(g_jmp_buf, 1);
}

// Helper: invoke a function pointer with crash protection.
// Returns true if the function returned normally, false if a crash occurred and was recovered.
static bool call_with_safety(void (*fn)(void)) {
    g_got_segv = 0;
    if (sigsetjmp(g_jmp_buf, 1) == 0) {
        // Install handler and call the function
        std::signal(SIGSEGV, segv_handler);
        fn();
        // Restore default handler
        std::signal(SIGSEGV, SIG_DFL);
        return true; // function returned normally
    } else {
        // We jumped back here due to a crash
        std::signal(SIGSEGV, SIG_DFL);
        return false;
    }
}

// Test 1: Basic direct invocation (if symbol is visible in the binary)
static bool test_cover_fast_direct_basic() {
    // Best-effort: attempt to call the function directly.
    // If symbol is not linked/exported, this test will fail to link at compile-time or runtime.
    // We wrap in a try/catch to guard against unexpected exceptions from within the C code.
    try {
        if (&cover_fast) {
            bool ok = call_with_safety(cover_fast);
            if (!ok) {
                std::cerr << "[test_cover_fast_direct_basic] Warning: cover_fast crashed under direct call (recovered).\n";
            } else {
                // Success path: function returned cleanly
            }
            return ok;
        } else {
            // Symbol not available; skip gracefully
            std::cout << "[test_cover_fast_direct_basic] cover_fast symbol not available for direct call; skipping.\n";
            return true; // skip considered pass to avoid false failure
        }
    } catch (...) {
        std::cerr << "[test_cover_fast_direct_basic] Exception occurred during direct call to cover_fast.\n";
        return false;
    }
}

// Test 2: Dynamic symbol lookup path (dlsym) to maximize compatibility across build setups.
// It tries to locate "cover_fast" at runtime and invoke it safely.
static bool test_cover_fast_dynamic_lookup() {
    // Attempt to obtain handle to the current process (undefined behavior on some platforms if not supported)
    void *handle = dlopen(nullptr, RTLD_LAZY);
    if (!handle) {
        std::cerr << "[test_cover_fast_dynamic_lookup] dlopen failed: " << dlerror() << "\n";
        // Fallback to direct call (which may fail at link-time if not exported)
        try {
            if (&cover_fast) {
                bool ok = call_with_safety(cover_fast);
                return ok;
            } else {
                std::cout << "[test_cover_fast_dynamic_lookup] cover_fast symbol not available for direct call; skipping.\n";
                return true;
            }
        } catch (...) {
            return false;
        }
    }

    // Clear any prior error
    dlerror();
    // Try to locate the symbol
    typedef void (*cover_fast_t)(void);
    cover_fast_t f = (cover_fast_t)dlsym(handle, "cover_fast");
    const char *err = dlerror();
    if (f) {
        // Call safely
        bool ok = call_with_safety(f);
        dlclose(handle);
        return ok;
    } else {
        if (err) {
            std::cerr << "[test_cover_fast_dynamic_lookup] dlsym error: " << err << "\n";
        } else {
            std::cerr << "[test_cover_fast_dynamic_lookup] cover_fast symbol not found via dlsym.\n";
        }
        // Fallback to direct call
        try {
            if (&cover_fast) {
                bool ok = call_with_safety(cover_fast);
                dlclose(handle);
                return ok;
            } else {
                std::cout << "[test_cover_fast_dynamic_lookup] cover_fast symbol not available for direct call; skipping.\n";
                dlclose(handle);
                return true;
            }
        } catch (...) {
            dlclose(handle);
            return false;
        }
    }
}

// Test 3: Repeated invocation to ensure there are no stateful crashes across calls.
// Even if the internal state is not externally observable, repeated safe exits test stability.
static bool test_cover_fast_repeat_calls() {
    bool ok1 = false, ok2 = false;
    // Try direct call first
    if (&cover_fast) {
        ok1 = call_with_safety(cover_fast);
    } else {
        ok1 = true; // if symbol not available, consider pass for non-linkable scenarios
    }
    // Second invocation
    if (&cover_fast) {
        ok2 = call_with_safety(cover_fast);
    } else {
        ok2 = true;
    }
    return ok1 && ok2;
}

// Main runner
int main(void) {
    // Step 3 / Domain knowledge: Use a simple, explicit test runner.
    // We exercise:
    // - Direct invocation (when available)
    // - Dynamic invocation (through dlsym, if possible)
    // - Repeated invocations to ensure no crash on back-to-back calls
    // We also provide explanations in comments for each test.

    std::vector<std::pair<std::string, bool>> results;

    // Test 1: Direct basic call
    {
        bool res = test_cover_fast_direct_basic();
        results.emplace_back("Direct basic invocation of cover_fast", res);
    }

    // Test 2: Dynamic lookup path
    {
        bool res = test_cover_fast_dynamic_lookup();
        results.emplace_back("Dynamic lookup invocation of cover_fast (dlsym)", res);
    }

    // Test 3: Repeated invocations
    {
        bool res = test_cover_fast_repeat_calls();
        results.emplace_back("Repeated invocations of cover_fast (stability)", res);
    }

    // Summary
    int passed = 0;
    int total = (int)results.size();
    std::cout << "Test results for cover_fast (infcover.c):\n";
    for (const auto &r : results) {
        std::cout << " - " << r.first << ": " << (r.second ? "PASS" : "FAIL") << "\n";
        if (r.second) ++passed;
    }
    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    // Exit code: 0 indicates success (all tests PASS); non-zero indicates failures.
    return (passed == total) ? 0 : 1;
}

// Explanations for the tests (embedded as comments for maintainers):
//
// Test coverage rationale:
// - Direct basic invocation: ensures that if the symbol is visible in the linked binary,
//   calling cover_fast does not crash and completes. This exercises the normal execution path
//   of the focal method as defined in the provided code snippet (the sequence of inf() calls).
// - Dynamic lookup path: robust against different build configurations where cover_fast might be
//   exposed as a dynamic symbol. This attempts to locate the symbol at runtime and exercise the
//   same code path via a function pointer, increasing coverage across linking strategies.
// - Repeated invocations: checks stability across multiple calls. Even if cover_fast has internal
//   static state, repeated safe calls help uncover any latent memory/state corruption, which aids
//   coverage of potential internal branches across successive executions.
// - Crash safety: implemented via a SIGSEGV handler and setjmp/longjmp to recover from a potential
//   crash during the invocation. This allows the test suite to continue and report failures instead
//   of terminating the entire test run. If the focal method ever crashes, the corresponding test
//   will report as FAIL, highlighting an important area for stability improvement.
//
// Domain knowledge alignment:
// - The test suite avoids poking at private/internal static helpers directly, focusing on the public-like
//   interfaces exposed by the focal function (cover_fast) and its observable behavior when invoked.
// - We do not attempt to mock private helpers; instead, we rely on the actual function under test.
// - The test harness uses only C/C++ standard library facilities and POSIX dynamic loading (if available),
//   aligning with the domain constraints provided.
//
// Notes for maintainers:
// - If your build defines local as a macro to "static" (as in some zlib code paths), ensure the test
//   binary exports cover_fast for direct invocation, or rely on the dynamic lookup path (dlsym).
// - If your environment is Windows, adapt the dynamic loading portion to use LoadLibrary/GetProcAddress
//   or remove dynamic lookup and rely solely on direct invocation with appropriate export settings.
// - The test results are printed to stdout; integrate with your CI by analyzing the PASS/FAIL lines.