// C++11 unit test suite for the focal method safecat_current_encoding in the
// context of the provided C code (pngvalid.c). This test suite is implemented
// without GTest, using a lightweight, self-contained test harness based on the
// C API exposed by libpng (included via png.h).
// The tests focus on the true/false branch of the encoding_ignored predicate
// and verify that the function behaves predictably in both cases, while also
// ensuring basic correctness of the returned position.

#include <cstddef>
#include <../../png.h>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Import the libpng definitions and declarations.
// The project structure in the focal class uses a relative include path;
// adjust as necessary for your build system.
extern "C" {

    // Prototypes of the functions used by the focal method.
    // These declarations mirror the C signatures used in the production code.
    size_t safecat(char *buffer, size_t bufsize, size_t pos, const char *cat);
    size_t safecat_color_encoding(char *buffer, size_t bufsize, size_t pos,
                                 const color_encoding *e, double encoding_gamma);
    size_t safecat_current_encoding(char *buffer, size_t bufsize, size_t pos,
                                   const png_modifier *pm);
}

// Simple substring check utility.
static bool contains_substring(const char* buffer, const char* substr) {
    if (!buffer || !substr) return false;
    return std::strstr(buffer, substr) != nullptr;
}

// Lightweight test harness: return true if test passes, false otherwise.
static bool test_safecat_current_encoding_overridden_branch() {
    // This test exercises the true-branch of the encoding_ignored check:
    // pm->encoding_ignored == 1 leads to appending "[overridden]".
    color_encoding ce = {};          // Minimal encoding descriptor (contents opaque to test).
    png_modifier pm = {};              // Zero-initialized; we override relevant fields below.
    pm.current_encoding = &ce;
    pm.current_gamma = 0.5;
    pm.encoding_ignored = 1;           // Trigger the true branch.

    char buffer[256];
    buffer[0] = '\0';                  // Start with an empty string buffer.
    size_t pos = 0;

    size_t newpos = safecat_current_encoding(buffer, sizeof(buffer), pos, &pm);

    bool ok = true;
    if (!contains_substring(buffer, "[overridden]")) {
        std::cerr << "FAIL: Overridden marker not found in buffer when encoding_ignored is true.\n";
        ok = false;
    } else {
        // If the marker is present, ensure that something was appended beyond the initial pos.
        if (newpos <= pos) {
            std::cerr << "FAIL: New position not advanced after appending encoding.\n";
            ok = false;
        }
    }

    // Non-fatal: always report test result
    return ok;
}

static bool test_safecat_current_encoding_normal_branch() {
    // This test exercises the false-branch of the encoding_ignored check:
    // pm->encoding_ignored == 0 should NOT append "[overridden]".
    color_encoding ce = {};
    png_modifier pm = {};
    pm.current_encoding = &ce;
    pm.current_gamma = 1.0;
    pm.encoding_ignored = 0;            // Do not trigger the true branch.

    char buffer[256];
    buffer[0] = '\0';
    size_t pos = 0;

    size_t newpos = safecat_current_encoding(buffer, sizeof(buffer), pos, &pm);

    bool ok = true;
    if (contains_substring(buffer, "[overridden]")) {
        std::cerr << "FAIL: Buffer should not contain '[overridden]' when encoding_ignored is false.\n";
        ok = false;
    }

    if (newpos <= pos) {
        std::cerr << "FAIL: New position not advanced when encoding is appended.\n";
        ok = false;
    }

    return ok;
}

static bool test_safecat_current_encoding_nonzero_start_pos() {
    // Ensure that when an initial non-zero position is provided, the function
    // still appends the encoding information starting at or beyond that position.
    color_encoding ce = {};
    png_modifier pm = {};
    pm.current_encoding = &ce;
    pm.current_gamma = 0.75;
    pm.encoding_ignored = 1;            // Trigger the true branch for this test as well.

    char buffer[256];
    // Pre-fill buffer with a known prefix to verify that appending happens after the prefix.
    const char* prefix = "PRE_";
    std::strncpy(buffer, prefix, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    size_t pos = std::strlen(buffer);   // Start right after the prefix.

    size_t newpos = safecat_current_encoding(buffer, sizeof(buffer), pos, &pm);

    bool ok = true;
    if (!contains_substring(buffer, "[overridden]")) {
        std::cerr << "FAIL: Overridden marker not found when starting from non-zero pos with encoding_ignored.\n";
        ok = false;
    }
    if (newpos <= pos) {
        std::cerr << "FAIL: New position did not advance when starting from non-zero pos.\n";
        ok = false;
    }

    // Optional sanity: ensure the prefix remains at the start of the buffer.
    if (std::strncmp(buffer, prefix, std::strlen(prefix)) != 0) {
        std::cerr << "FAIL: Buffer prefix modified unexpectedly.\n";
        ok = false;
    }

    return ok;
}

int main() {
    int total = 0;
    int passed = 0;

    // Run test 1: true branch on encoding_ignored
    ++total;
    if (test_safecat_current_encoding_overridden_branch()) {
        ++passed;
        std::cout << "[PASS] safecat_current_encoding: true-branch (overridden) coverage.\n";
    } else {
        std::cout << "[FAIL] safecat_current_encoding: true-branch (overridden) coverage.\n";
    }

    // Run test 2: false branch on encoding_ignored
    ++total;
    if (test_safecat_current_encoding_normal_branch()) {
        ++passed;
        std::cout << "[PASS] safecat_current_encoding: false-branch (no overridden) coverage.\n";
    } else {
        std::cout << "[FAIL] safecat_current_encoding: false-branch (no overridden) coverage.\n";
    }

    // Run test 3: non-zero starting position handling
    ++total;
    if (test_safecat_current_encoding_nonzero_start_pos()) {
        ++passed;
        std::cout << "[PASS] safecat_current_encoding: non-zero start position handling.\n";
    } else {
        std::cout << "[FAIL] safecat_current_encoding: non-zero start position handling.\n";
    }

    // Summary
    std::cout << "Unit tests finished: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}