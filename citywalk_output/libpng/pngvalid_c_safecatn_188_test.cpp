// C++11 test suite for the focal function safecatn from pngvalid.c
// This test suite uses a small self-contained harness (no GoogleTest) and
// relies on the external C functions provided by the project under test.
// We assume the C linkage for safecat, safecatn, and safecatd is available.

#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Ensure C linkage for the functions defined in the C source file.
extern "C" {
    // Prototypes of the focal method and its related safe-string utilities.
    // We only rely on safecatn for unit testing here, but safecat is invoked by safecatn.
    // The actual implementations are expected to be provided when linking the project.
    char* safecat(char* buffer, size_t bufsize, size_t pos, const char* cat);
    char* safecatn(char* buffer, size_t bufsize, size_t pos, int n);
    char* safecatd(char* buffer, size_t bufsize, size_t pos, double d, int precision);
}

// Simple non-terminating test harness
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Helper to run a test and report result while continuing on errors.
static void RUN(const std::string& test_name, const std::function<bool()>& test_func) {
    bool ok = test_func();
    if (ok) {
        ++g_tests_passed;
        std::cout << "[PASSED] " << test_name << std::endl;
    } else {
        ++g_tests_failed;
        std::cout << "[FAILED] " << test_name << std::endl;
    }
}

// Test 1: Basic append at position 0 with a positive integer n
// Purpose: safecatn converts n to string and delegates to safecat. When buffer is empty
// and pos is 0, the resulting content should begin with the decimal representation of n.
static bool test_safecatn_basic_append_zero() {
    // Prepare a clean buffer
    char buffer[64];
    std::memset(buffer, 0, sizeof(buffer));

    int n = 42;
    char* res = safecatn(buffer, sizeof(buffer), 0, n);
    if (res == nullptr) {
        std::cerr << "test_safecatn_basic_append_zero: safecatn returned NULL" << std::endl;
        return false;
    }
    // Expect buffer to start with "42"
    const std::string expected = std::to_string(n);
    if (std::strncmp(buffer, expected.c_str(), expected.size()) != 0) {
        std::cerr << "test_safecatn_basic_append_zero: buffer does not start with expected '"
                  << expected << "'; buffer starts with '"
                  << (buffer[0] ? buffer : "") << "'" << std::endl;
        return false;
    }
    // Also verify the return pointer equals the buffer (common convention)
    if (res != buffer) {
        std::cerr << "test_safecatn_basic_append_zero: return value is not equal to buffer" << std::endl;
        return false;
    }
    return true;
}

// Test 2: Append at a non-zero position after pre-filled content
// Purpose: Ensure safecatn writes starting at the given pos, leaving earlier content intact.
static bool test_safecatn_append_with_existing() {
    // Pre-fill buffer with "ABC" and a null terminator
    char buffer[64];
    std::memset(buffer, 0, sizeof(buffer));
    std::strcpy(buffer, "ABC");

    // Append number at pos = 3 (after "ABC")
    int n = 7;
    char* res = safecatn(buffer, sizeof(buffer), 3, n);
    if (res == nullptr) {
        std::cerr << "test_safecatn_append_with_existing: safecatn returned NULL" << std::endl;
        return false;
    }

    // Expect buffer to be "ABC7" at the start
    const char* expected = "ABC7";
    if (std::strncmp(buffer, expected, std::strlen(expected)) != 0) {
        std::cerr << "test_safecatn_append_with_existing: buffer does not match expected '"
                  << expected << "'" << std::endl;
        return false;
    }
    if (res != buffer) {
        std::cerr << "test_safecatn_append_with_existing: return value is not equal to buffer" << std::endl;
        return false;
    }
    return true;
}

// Test 3: Negative integer n
// Purpose: safecatn should handle negative numbers by converting to a string like "-N".
static bool test_safecatn_negative_number() {
    char buffer[64];
    std::memset(buffer, 0, sizeof(buffer));

    int n = -9;
    char* res = safecatn(buffer, sizeof(buffer), 0, n);
    if (res == nullptr) {
        std::cerr << "test_safecatn_negative_number: safecatn returned NULL" << std::endl;
        return false;
    }

    const std::string expected = std::to_string(n); // "-9"
    if (std::strncmp(buffer, expected.c_str(), expected.size()) != 0) {
        std::cerr << "test_safecatn_negative_number: buffer does not start with '" << expected << "'" << std::endl;
        return false;
    }
    if (res != buffer) {
        std::cerr << "test_safecatn_negative_number: return value is not equal to buffer" << std::endl;
        return false;
    }
    return true;
}

// Test 4: Overflow scenario (not enough space in buffer to hold the numeric string)
// Purpose: safecatn should fail gracefully and return NULL when there isn't enough space.
static bool test_safecatn_overflow() {
    // bufsize intentionally small to force overflow
    char buffer[4];
    // initialize to zeros
    std::memset(buffer, 0, sizeof(buffer));
    // Try to write a number with representation longer than available space
    int n = 12345; // "12345" length 5
    char* res = safecatn(buffer, sizeof(buffer), 0, n);
    if (res != nullptr) {
        std::cerr << "test_safecatn_overflow: expected NULL due to overflow, got non-NULL" << std::endl;
        return false;
    }
    // On overflow, buffer content should remain unchanged (all zeros or initial state)
    // We simply verify buffer[0] is still '\0' or unchanged
    if (buffer[0] != '\0') {
        // If anything was written, that's unexpected for this overflow path
        std::cerr << "test_safecatn_overflow: buffer was modified on overflow" << std::endl;
        return false;
    }
    return true;
}

// Test 5: pos out of range (pos >= bufsize) should fail gracefully
static bool test_safecatn_pos_out_of_range() {
    char buffer[16];
    std::memset(buffer, 0, sizeof(buffer));
    // Place a small content to ensure we can detect non-modification on failure
    std::strcpy(buffer, "init");

    // Set pos to bufsize to simulate out-of-range position
    char* res = safecatn(buffer, sizeof(buffer), 16, 1); // pos equals bufsize
    if (res != nullptr) {
        std::cerr << "test_safecatn_pos_out_of_range: expected NULL for pos out of range" << std::endl;
        return false;
    }

    // Ensure original content remains intact
    if (std::strcmp(buffer, "init") != 0) {
        std::cerr << "test_safecatn_pos_out_of_range: buffer content modified on failure" << std::endl;
        return false;
    }
    return true;
}

int main() {
    // Run the focused unit tests for safecatn
    std::cout << "Starting safecatn unit tests (C++ harness, no GTest):" << std::endl;

    RUN("test_safecatn_basic_append_zero", test_safecatn_basic_append_zero);
    RUN("test_safecatn_append_with_existing", test_safecatn_append_with_existing);
    RUN("test_safecatn_negative_number", test_safecatn_negative_number);
    RUN("test_safecatn_overflow", test_safecatn_overflow);
    RUN("test_safecatn_pos_out_of_range", test_safecatn_pos_out_of_range);

    std::cout << "Summary: Passed " << g_tests_passed << ", Failed " << g_tests_failed << std::endl;
    // Return non-zero if any test failed, to signal issues clearly.
    return (g_tests_failed == 0) ? 0 : 1;
}