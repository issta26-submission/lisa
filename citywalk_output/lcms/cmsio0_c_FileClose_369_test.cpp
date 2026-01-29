// cmsio0_fileclose_unit_tests.cpp
// A lightweight C++11 test suite for the cmsBool FileClose(cmsIOHANDLER* iohandler) function.
// This test avoids Google Test and uses a minimal self-contained harness to exercise
// the true and false branches of the FileClose function as best as feasible given
// the dependencies of the LCMS project.

// Notes:
// - The test relies on the project's internal types (cmsBool, cmsIOHANDLER, etc.).
// - The true branch is tested with a real FILE* opened via tmpfile() to ensure fclose() succeeds.
// - The false branch is attempted by forcing a scenario where fclose() would fail (closing an
//   already-closed FILE*). This is inherently platform-dependent; the test aims to cover
//   negative behavior without relying on private internals.
// - No GTest is used; a minimal assertion mechanism is provided.
// - Static/internal helpers from the library are not mocked; the test sticks to external API usage.

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Ensure C linkage for LCMS internal API when including the C headers in C++.
extern "C" {
}

// Prototype of the focal function under test. If declared elsewhere, this ensures linkage.
extern cmsBool FileClose(cmsIOHANDLER* iohandler);

// Simple, non-terminating assertion helpers.
// They report failures but do not terminate the test run, to maximize coverage.
static int g_testFailures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "EXPECT_TRUE FAILED: " << (msg) \
                      << "  (Condition: " #cond ")" << std::endl; \
            ++g_testFailures; \
        } \
    } while(false)

#define EXPECT_FALSE(cond, msg) \
    do { \
        if((cond)) { \
            std::cerr << "EXPECT_FALSE FAILED: " << (msg) \
                      << "  (Condition: " #cond ")" << std::endl; \
            ++g_testFailures; \
        } \
    } while(false)

#define PRINT_TEST_HEADER(name) \
    std::cout << "== Running test: " #name " ==\n";

// Helper to create a minimal cmsIOHANDLER instance.
// We only rely on the fields used by FileClose: 'stream' and 'ContextID'.
// The rest of the fields are left unused for this test.
static cmsIOHANDLER* createMinimalIOHANDLER(void* streamPtr) {
    cmsIOHANDLER* ioh = (cmsIOHANDLER*) std::malloc(sizeof(cmsIOHANDLER));
    if (ioh == nullptr) return nullptr;
    // The actual structure in lcms2 typically includes at least 'stream' and 'ContextID'.
    // Initialize with safe defaults; the exact type of ContextID is context-dependent.
    ioh->stream = streamPtr;
    ioh->ContextID = 0; // A neutral context; LCMS should handle this gracefully for tests.
    return ioh;
}

// Test 1: True branch - fclose succeeds and FileClose frees the handler.
static void test_FileClose_Succeeds() {
    PRINT_TEST_HEADER(Test_FileClose_Succeeds);
    // Create a temporary FILE* using tmpfile() for portability and automatic cleanup.
    FILE* tmp = std::tmpfile();
    EXPECT_TRUE(tmp != nullptr, "tmpfile() should succeed to obtain a temporary FILE*.");

    if (tmp == nullptr) {
        // Cannot proceed without a valid FILE*. Report and return.
        return;
    }

    // Build a minimal cmsIOHANDLER pointing to the FILE*.
    cmsIOHANDLER* ioh = createMinimalIOHANDLER((void*)tmp);
    EXPECT_TRUE(ioh != nullptr, "Failed to allocate cmsIOHANDLER for test.");

    if (ioh == nullptr) {
        std::fclose(tmp);
        return;
    }

    // Call the focal function. We expect TRUE on successful close and free.
    cmsBool result = FileClose(ioh);

    EXPECT_TRUE(result == TRUE, "FileClose should return TRUE when fclose succeeds.");

    // If FileClose freed the iohandler, we should not attempt to free it here.
    // Do not call std::free(ioh) to avoid double-free; rely on _cmsFree.

    // Best effort: no further checks since internal freeing semantics are opaque
    // to the test harness. The main criterion is the return value above.
    // Clean up FILE* if still open (on some platforms, FileClose should have closed it).
    // If it's already closed by FileClose, fclose will be no-op or fail gracefully.
    // To be safe, attempt to close only if it's still open (C standard does not provide
    // a portable "is open" check, so we skip further fclose here).

    // If ioh was freed by FileClose, this is a no-op; otherwise, free ourselves to avoid leaks.
    // We deduce freeing happened; avoid freeing to prevent double-free in some LCMS paths.
    (void)tmp; // suppress unused warning in case of unusual linkage
}

// Test 2: False branch - simulate an already-closed stream to provoke a failing fclose.
// Note: This path is platform-dependent; we attempt to close an already-closed FILE* to induce failure.
static void test_FileClose_Fails_When_StreamAlreadyClosed() {
    PRINT_TEST_HEADER(Test_FileClose_Fails_When_StreamAlreadyClosed);

    // Create a temporary file and immediately close it to simulate a closed stream.
    FILE* tmp2 = std::tmpfile();
    EXPECT_TRUE(tmp2 != nullptr, "tmpfile() should succeed for test2.");

    if (tmp2 == nullptr) return;

    // Close the FILE* ourselves to simulate "already closed" state before FileClose.
    int closeDirect = std::fclose(tmp2);
    (void)closeDirect; // Suppress unused warning; we do not assert on this operation.

    // Now create a cmsIOHANDLER whose stream points to this already-closed FILE*.
    cmsIOHANDLER* ioh = createMinimalIOHANDLER((void*)tmp2);
    EXPECT_TRUE(ioh != nullptr, "Failed to allocate cmsIOHANDLER for test2.");

    if (ioh == nullptr) {
        // If allocation failed, we cannot proceed; clean up temp file resource if possible.
        // Note: tmpfile() resources are cleaned up on process exit; no explicit direction here.
        return;
    }

    // Invoke the focal function. We expect FALSE since fclose(tmp2) should fail on a closed stream.
    cmsBool result = FileClose(ioh);
    EXPECT_FALSE(result == TRUE, "FileClose should return FALSE when fclose() would fail on a closed/invalid stream.");

    // Do not free ioh here to avoid potential double-free with library path.
    (void)tmp2;
}

// Main test runner
int main() {
    std::cout << "Starting cmsio0 FileClose unit tests (C++11 harness, no GTest)...\n";

    g_testFailures = 0;

    test_FileClose_Succeeds();
    test_FileClose_Fails_When_StreamAlreadyClosed();

    if (g_testFailures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_testFailures << " test(s) failed.\n";
        return 1;
    }
}