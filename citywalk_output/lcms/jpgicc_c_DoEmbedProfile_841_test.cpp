// Test suite for DoEmbedProfile in jpgicc.c
// Purpose: exercise DoEmbedProfile() behavior across typical file scenarios
// - Non-existent file (path does not exist) to cover the f == NULL branch in a safe manner
// - Empty ICC profile file to cover size == 0 branch
// - Small ICC profile file to cover normal path with a few bytes
// Notes:
// - This test harness assumes the project can be compiled with C++11 and linked
//   against the same codebase that provides DoEmbedProfile and its dependencies.
// - We intentionally do not rely on any testing framework (GTest/GMock) per instructions.
// - We use a minimal portable test harness with non-terminating checks (i.e., tests always run).

#include <cstring>
#include <iccjpeg.h>
#include <cstdio>
#include <jpeglib.h>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include <utils.h>


// Declaration of the focal function under test (C linkage)
extern "C" void DoEmbedProfile(const char* ProfileFile);

// Helper: simple non-terminating assertion-like mechanism
static bool passIf(bool cond, const char* testName, const char* detail) {
    if (cond) {
        std::cout << "[PASS] " << testName;
        if (detail) std::cout << " - " << detail;
        std::cout << std::endl;
        return true;
    } else {
        std::cout << "[FAIL] " << testName;
        if (detail) std::cout << " - " << detail;
        std::cout << std::endl;
        return false;
    }
}

// Utility: remove a file if it exists (best-effort)
static void safeRemove(const char* path) {
    if (path == nullptr) return;
    std::remove(path);
}

// Test 1: Non-existent file should cause DoEmbedProfile to return without crashing.
// Rationale: This exercises the "if (f == NULL) return;" branch by providing a path that does not exist.
static bool test_DoEmbedProfile_NonExistentFile() {
    const char* path = "test_profile_nonexistent.icc";

    // Ensure the file truly does not exist (best effort)
    safeRemove(path);

    // Call the focal function; if it crashes, the test would fail by terminating the process.
    DoEmbedProfile(path);

    // If no crash occurred, we consider this test a pass.
    // We cannot introspect internal state here due to the function's void return.
    return true;
}

// Test 2: Empty file should be handled without errors.
// Rationale: This covers the path where size == 0, EmbedLen == 0, and an ICC buffer of length 0 is embedded.
static bool test_DoEmbedProfile_EmptyFile() {
    const char* path = "test_profile_empty.icc";

    // Create an empty file
    safeRemove(path);
    {
        FILE* f = std::fopen(path, "wb");
        if (f) {
            // Write nothing to keep the file size 0
            std::fclose(f);
        } else {
            // If we fail to create the file, consider test inconclusive
            return false;
        }
    }

    // Invoke DoEmbedProfile on the empty file
    DoEmbedProfile(path);

    // Cleanup
    safeRemove(path);

    return true;
}

// Test 3: Small file should be handled correctly.
// Rationale: This exercises the normal path with EmbedLen > 0 and ensures no crash.
static bool test_DoEmbedProfile_SmallFile() {
    const char* path = "test_profile_small.icc";

    // Create a small ICC-like profile file with a few bytes
    safeRemove(path);
    {
        FILE* f = std::fopen(path, "wb");
        if (!f) return false;
        const unsigned char data[] = {0x01, 0x02, 0x03, 0x04}; // small payload
        std::fwrite(data, 1, sizeof(data), f);
        std::fclose(f);
    }

    // Invoke DoEmbedProfile on the small file
    DoEmbedProfile(path);

    // Cleanup
    safeRemove(path);

    return true;
}

// Test 4 (optional extension): multiple consecutive calls with different inputs.
// Rationale: ensures no state leakage between calls across typical use-cases.
static bool test_DoEmbedProfile_MultipleInputs() {
    const char* paths[] = {
        "test_profile_multiple_1.icc",
        "test_profile_multiple_2.icc"
    };
    // Create two small files
    for (const char* p : paths) {
        safeRemove(p);
        FILE* f = std::fopen(p, "wb");
        if (!f) continue;
        const unsigned char data[] = {0xAA, 0xBB, 0xCC};
        std::fwrite(data, 1, sizeof(data), f);
        std::fclose(f);
    }

    // Call DoEmbedProfile for each
    for (const char* p : paths) {
        DoEmbedProfile(p);
    }

    // Cleanup
    for (const char* p : paths) {
        safeRemove(p);
    }

    // If all calls completed without crashing, treat as pass
    return true;
}

int main() {
    std::vector<std::string> testNames = {
        "DoEmbedProfile_NonExistentFile",
        "DoEmbedProfile_EmptyFile",
        "DoEmbedProfile_SmallFile",
        "DoEmbedProfile_MultipleInputs"
    };

    int total = 0;
    int passed = 0;

    // Run Test 1
    {
        const bool ok = test_DoEmbedProfile_NonExistentFile();
        total++;
        if (ok) passed++;
        // Explanatory note: verifies graceful handling of non-existing input path.
        (void) ok;
    }

    // Run Test 2
    {
        const bool ok = test_DoEmbedProfile_EmptyFile();
        total++;
        if (ok) passed++;
        // Explanatory note: ensures zero-length ICC buffers are handled.
        (void) ok;
    }

    // Run Test 3
    {
        const bool ok = test_DoEmbedProfile_SmallFile();
        total++;
        if (ok) passed++;
        // Explanatory note: exercises normal embedding path with small payload.
        (void) ok;
    }

    // Run Test 4
    {
        const bool ok = test_DoEmbedProfile_MultipleInputs();
        total++;
        if (ok) passed++;
        // Explanatory note: checks that sequential calls do not crash and complete.
        (void) ok;
    }

    // Summary
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Exit code 0 if all tests pass; non-zero otherwise
    return (passed == total) ? 0 : 1;
}