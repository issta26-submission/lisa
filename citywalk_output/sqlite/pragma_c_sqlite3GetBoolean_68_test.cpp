// Unit test suite for sqlite3GetBoolean (pragma.c) using a lightweight, non-GTest C++11 harness.
// This test suite does not rely on Google Test and is designed to be linked with the
// production C source file(s) that provide the focal function.
// Note: The production code defines sqlite3GetBoolean as: u8 sqlite3GetBoolean(const char *z, u8 dflt)
// and internally relies on a static getSafetyLevel function. This test treats sqlite3GetBoolean
// as a black-box public API and focuses on its observable behavior.

#include <vector>
#include <iostream>
#include <string>
#include <sqliteInt.h>
#include <pragma.h>


// Declaration of the focal function from the production C code.
// We use extern "C" to prevent C++ name mangling.
extern "C" unsigned char sqlite3GetBoolean(const char *z, unsigned char dflt);

// Global counter to accumulate test failures without terminating the test run.
static int g_failures = 0;

// Lightweight test result logger that does not terminate the test run on failure.
// It prints PASS/FAIL messages to std::cerr for visibility and increments the global counter on failure.
static void logResult(const std::string &testName, bool pass) {
    if (pass) {
        std::cerr << "[PASS] " << testName << std::endl;
    } else {
        std::cerr << "[FAIL] " << testName << std::endl;
        ++g_failures;
    }
}

// -----------------------------------------------------------------------------
// Test 1: Binary nature of the boolean result
// Intent: For a variety of inputs z and for different default values dflt (0 and 1),
// the function sqlite3GetBoolean must return a value that is either 0 or 1
// (i.e., a proper boolean value after the != 0 comparison inside the function).
// This checks that the observable return type is consistently binary.
// -----------------------------------------------------------------------------
static void test_BinaryOutput_IsBoolean() {
    std::vector<std::string> inputs = {
        "", "0", "1", "true", "false", "yes", "no", "abcdef", "12345"
    };

    for (const auto &s : inputs) {
        unsigned char r0 = sqlite3GetBoolean(s.c_str(), 0);
        bool ok0 = (r0 == 0 || r0 == 1);
        logResult("sqlite3GetBoolean(\"" + s + "\", 0) returns 0/1", ok0);

        unsigned char r1 = sqlite3GetBoolean(s.c_str(), 1);
        bool ok1 = (r1 == 0 || r1 == 1);
        logResult("sqlite3GetBoolean(\"" + s + "\", 1) returns 0/1", ok1);
    }
}

// -----------------------------------------------------------------------------
// Test 2: Determinism / consistency
// Intent: For a fixed input z and fixed dflt, repeated invocations should yield
// the same boolean value. This guards against non-deterministic behavior due to
// internal state or undefined behavior in static helpers.
// -----------------------------------------------------------------------------
static void test_DeterminismForConstantInput() {
    const std::string z = "determinism-test";
    unsigned char initial = sqlite3GetBoolean(z.c_str(), 0);

    // Repeat several times to ensure stability.
    bool deterministic = true;
    for (int i = 0; i < 5; ++i) {
        unsigned char next = sqlite3GetBoolean(z.c_str(), 0);
        if (next != initial) {
            deterministic = false;
            break;
        }
    }

    logResult("sqlite3GetBoolean(\"determinism-test\", 0) is deterministic across calls", deterministic);
}

// -----------------------------------------------------------------------------
// Test 3: Handling of long inputs (no crashes, reasonable behavior)
// Intent: Ensure that very long input strings do not crash the function and that
// the return value remains a boolean (0 or 1). This tests basic robustness.
// -----------------------------------------------------------------------------
static void test_LongInput_NoCrash() {
    std::string longInput(4096, 'x'); // 4KB input
    unsigned char r = sqlite3GetBoolean(longInput.c_str(), 0);
    bool ok = (r == 0 || r == 1);
    logResult("sqlite3GetBoolean(long_input, 0) handles long inputs without crash and returns 0/1", ok);
}

// -----------------------------------------------------------------------------
// Test 4: Basic sanity check across multiple inputs and dflt values
// Intent: Exercise several edge-case strings to increase coverage of the decision
// path inside the primitive predicate (getSafetyLevel != 0) by ensuring that
// the function remains stable across common inputs.
// -----------------------------------------------------------------------------
static void test_MultipleInputsSanity() {
    const std::vector<std::string> inputs = {
        "", " ", "\t", "0", "1", "true", "false", "random text with spaces", "1234567890"
    };

    for (const auto &s : inputs) {
        unsigned char r0 = sqlite3GetBoolean(s.c_str(), 0);
        bool ok0 = (r0 == 0 || r0 == 1);
        logResult("sqlite3GetBoolean(\"" + s + "\", 0) sanity check returns 0/1", ok0);

        unsigned char r1 = sqlite3GetBoolean(s.c_str(), 1);
        bool ok1 = (r1 == 0 || r1 == 1);
        logResult("sqlite3GetBoolean(\"" + s + "\", 1) sanity check returns 0/1", ok1);
    }
}

// -----------------------------------------------------------------------------
// Main test runner
// -----------------------------------------------------------------------------
int main() {
    // Execute test cases
    test_BinaryOutput_IsBoolean();
    test_DeterminismForConstantInput();
    test_LongInput_NoCrash();
    test_MultipleInputsSanity();

    // Summary
    std::cerr << "Total failures: " << g_failures << std::endl;
    // Return non-zero if any test failed
    return g_failures;
}