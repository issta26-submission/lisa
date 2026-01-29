/*
  Unit tests for sqlite3Fts5BufferZero
  - This test suite validates that sqlite3Fts5BufferZero sets the n field of Fts5Buffer to 0.
  - The tests are written for C++11 (no GoogleTest, no GMock). A lightweight test harness is used.
  - The tests assume the presence of the project's C headers (e.g., fts5Int.h) that define Fts5Buffer
    and declare sqlite3Fts5BufferZero. The tests interact with the real implementation.

  Note: Each test includes explanatory comments describing its purpose and the scenario it covers.
*/

#include <limits.h>
#include <iostream>
#include <cstdio>
#include <fts5Int.h>


// Bring in C declarations. The actual project should provide these headers.
// We wrap the C include to ensure correct linkage when compiling as C++.
extern "C" {
}

// If the header path is different in your environment, adjust the include path accordingly.


// Simple non-terminating assertion helper.
// Returns true if condition holds, false otherwise and prints a diagnostic message.
static bool check(bool condition, const char* testName, const char* msg) {
    if(condition) {
        std::cout << "[ PASS ] " << testName << " - " << msg << std::endl;
        return true;
    } else {
        std::cout << "[ FAIL ] " << testName << " - " << msg << std::endl;
        return false;
    }
}


// Test 1: Non-zero input should be zeroed by sqlite3Fts5BufferZero
// Scenario: pBuf.n starts at a non-zero value; after the call, it must be 0.
static bool test_sqlite3Fts5BufferZero_NonZeroInputToZero() {
    // Arrange
    Fts5Buffer buf;
    buf.n = 123; // non-zero initial value

    // Act
    sqlite3Fts5BufferZero(&buf);

    // Assert
    bool ok = (buf.n == 0);
    return ok;
}

// Test 2: Zero input should remain zero after sqlite3Fts5BufferZero
// Scenario: pBuf.n is already 0; the call should leave it as 0.
static bool test_sqlite3Fts5BufferZero_AlreadyZero() {
    // Arrange
    Fts5Buffer buf;
    buf.n = 0; // already zero

    // Act
    sqlite3Fts5BufferZero(&buf);

    // Assert
    bool ok = (buf.n == 0);
    return ok;
}

// Test 3: Negative input should be reset to zero
// Scenario: pBuf.n is negative; after the call, it must be 0.
static bool test_sqlite3Fts5BufferZero_NegativeInput() {
    // Arrange
    Fts5Buffer buf;
    buf.n = -42; // negative

    // Act
    sqlite3Fts5BufferZero(&buf);

    // Assert
    bool ok = (buf.n == 0);
    return ok;
}

// Test 4: Large positive input should be reset to zero
// Scenario: pBuf.n is INT_MAX; after the call, it must be 0.
static bool test_sqlite3Fts5BufferZero_LargeValue() {
    // Arrange
    Fts5Buffer buf;
    buf.n = INT_MAX;

    // Act
    sqlite3Fts5BufferZero(&buf);

    // Assert
    bool ok = (buf.n == 0);
    return ok;
}

// Test 5: Repeated zeroing should be idempotent
// Scenario: Call zero twice; the field should remain 0 after each call.
static bool test_sqlite3Fts5BufferZero_RepeatedCalls() {
    // Arrange
    Fts5Buffer buf;
    buf.n = 77;

    // Act
    sqlite3Fts5BufferZero(&buf);
    bool firstZero = (buf.n == 0);

    sqlite3Fts5BufferZero(&buf);
    bool secondZero = (buf.n == 0);

    // Assert
    return firstZero && secondZero;
}


// Test runner
typedef bool (*TestFunc)();

struct TestCase {
    const char* name;
    TestFunc func;
};

int main() {
    // Collect all test cases
    TestCase tests[] = {
        { "test_sqlite3Fts5BufferZero_NonZeroInputToZero", test_sqlite3Fts5BufferZero_NonZeroInputToZero },
        { "test_sqlite3Fts5BufferZero_AlreadyZero", test_sqlite3Fts5BufferZero_AlreadyZero },
        { "test_sqlite3Fts5BufferZero_NegativeInput", test_sqlite3Fts5BufferZero_NegativeInput },
        { "test_sqlite3Fts5BufferZero_LargeValue", test_sqlite3Fts5BufferZero_LargeValue },
        { "test_sqlite3Fts5BufferZero_RepeatedCalls", test_sqlite3Fts5BufferZero_RepeatedCalls },
    };

    const int nTests = sizeof(tests) / sizeof(TestCase);
    int passed = 0;

    std::cout << "Running sqlite3Fts5BufferZero test suite (C++11, no GTest)..." << std::endl;

    for(int i = 0; i < nTests; ++i) {
        const char* testName = tests[i].name;

        // Each test returns true on success, false on failure.
        bool result = tests[i].func();

        // Provide a concise result, but also add a little context
        if(result) {
            ++passed;
        } else {
            // In case of failure, print an additional diagnostic line.
            // The individual test function already prints the failure reason via check() if integrated.
        }

        // Optional per-test summary can be added here if desired
    }

    int total = nTests;
    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}