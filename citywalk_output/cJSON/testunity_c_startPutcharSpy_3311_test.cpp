// Test suite for focal method: startPutcharSpy
// Target environment: C++11, no GTest, minimal custom test harness
// Focus: verify key observable effects of startPutcharSpy and related spy functionality
// Assumptions based on provided FOCAL_CLASS_DEP and focal method:
// - startPutcharSpy resets an internal index (indexSpyBuffer) to 0
// - startPutcharSpy enables the spy via (putcharSpyEnabled = 1)
// - There is a buffer accessible via getBufferPutcharSpy()
// - putcharSpy(int) records characters into the buffer when the spy is enabled
// - External C APIs are available to link against
// Notes: This test suite uses a lightweight, non-terminating assertion model built on exceptions.

#include <unity.h>
#include <iostream>
#include <stdexcept>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Declare C linkage for the focal APIs
extern "C" {
    void startPutcharSpy(void);
    void endPutcharSpy(void);
    char* getBufferPutcharSpy(void);
    void putcharSpy(int c);

    // Expose internal state for test inspection (as per <FOCAL_CLASS_DEP> behavior)
    // These are assumed to be defined in the C codebase (non-static globals)
    extern int indexSpyBuffer;
    extern int putcharSpyEnabled;
}

// Simple assertion helpers
#define ASSERT_TRUE(cond, msg) \
    do { if (!(cond)) throw std::runtime_error(std::string("ASSERT_TRUE failed: ") + (msg)); } while (0)

#define ASSERT_EQ(expected, actual, msg) \
    do { if ((expected) != (actual)) throw std::runtime_error(std::string("ASSERT_EQ failed: ") + (msg) + \
        " (expected=" + std::to_string(expected) + ", actual=" + std::to_string(actual) + ")"); } while (0)

#define ASSERT_PTR_NOT_NULL(p, msg) \
    do { if ((p) == nullptr) throw std::runtime_error(std::string("ASSERT_PTR_NOT_NULL failed: ") + (msg)); } while (0)

// Test 1: startPutcharSpy initializes internal spy state: index reset to 0 and spy enabled
// Expected: indexSpyBuffer == 0 and putcharSpyEnabled == 1 after call
void test_startPutcharSpy_initializes_state() {
    // Act
    startPutcharSpy();

    // Assert
    ASSERT_EQ(0, indexSpyBuffer, "indexSpyBuffer should be reset to 0 by startPutcharSpy");
    ASSERT_EQ(1, putcharSpyEnabled, "putcharSpyEnabled should be set to 1 by startPutcharSpy");
}

// Test 2: startPutcharSpy resets the internal buffer index when called again after some writes
// This checks that a subsequent call reinitializes the buffer index back to 0
void test_startPutcharSpy_resets_index_on_recall() {
    // First session: perform a write to advance the index
    startPutcharSpy();
    putcharSpy('A');
    int firstIndex = indexSpyBuffer;

    // Second session: re-invoke to reset
    startPutcharSpy();
    int secondIndex = indexSpyBuffer;

    // Assert
    ASSERT_TRUE(firstIndex >= 0, "First index should be non-negative (sanity check)");
    ASSERT_EQ(0, secondIndex, "Second call to startPutcharSpy should reset indexSpyBuffer to 0");
    // If the implementation increments index on writes, firstIndex will be > = 0 and differ from 0
    // We do not require a specific non-zero value for firstIndex; just that reset occurs
}

// Test 3: After enabling the spy, putcharSpy writes to the internal buffer accessible via getBufferPutcharSpy
// Verifies that buffer content matches sequence of writes
void test_startPutcharSpy_buffer_content_after_writes() {
    // Arrange
    startPutcharSpy();

    // Act
    putcharSpy('A');
    putcharSpy('B');
    char* buf = getBufferPutcharSpy();

    // Assert
    ASSERT_PTR_NOT_NULL(buf, "Buffer from getBufferPutcharSpy should not be null after enabling spy");
    ASSERT_EQ('A', buf[0], "First character in buffer should be 'A'");
    ASSERT_EQ('B', buf[1], "Second character in buffer should be 'B'");
}

// Test 4: Re-invoking startPutcharSpy after a prior session overwrites the new session's first buffer entry
// This validates that a new session starts fresh in the buffer (index reset implies new write sequence)
void test_startPutcharSpy_overwrites_buffer_on_restart() {
    // Start first session and write a character
    startPutcharSpy();
    putcharSpy('X');

    // Restart spy to reset buffer
    startPutcharSpy();
    // Write a new character in the new session
    putcharSpy('Y');

    // Read buffer and validate only the new session's first character is observed
    char* buf = getBufferPutcharSpy();
    ASSERT_PTR_NOT_NULL(buf, "Buffer should be non-null after restart and write");
    ASSERT_EQ('Y', buf[0], "After restarting spy, buffer should reflect the new session's first write ('Y')");
}

// Test 5: End spy should be callable without crashing and should not affect subsequent writes unexpectedly
void test_endPutcharSpy_no_crash_and_continued_writes() {
    // Begin session
    startPutcharSpy();
    putcharSpy('D');
    // End spy (disable/cleanup)
    endPutcharSpy();

    // Attempt to write after end; behavior depends on endPutcharSpy implementation
    // We simply ensure that the call does not crash
    putcharSpy('E');

    // If the spy remains disabled after endPutcharSpy, buffer may or may not contain 'E'
    // We only assert that buffer pointer remains valid
    char* buf = getBufferPutcharSpy();
    ASSERT_PTR_NOT_NULL(buf, "Buffer should remain non-null after endPutcharSpy and subsequent writes");
}

// Simple test harness
typedef void (*TestFn)();
struct TestCase {
    const char* name;
    TestFn func;
};

// List of tests
TestCase tests[] = {
    {"test_startPutcharSpy_initializes_state", test_startPutcharSpy_initializes_state},
    {"test_startPutcharSpy_resets_index_on_recall", test_startPutcharSpy_resets_index_on_recall},
    {"test_startPutcharSpy_buffer_content_after_writes", test_startPutcharSpy_buffer_content_after_writes},
    {"test_startPutcharSpy_overwrites_buffer_on_restart", test_startPutcharSpy_overwrites_buffer_on_restart},
    {"test_endPutcharSpy_no_crash_and_continued_writes", test_endPutcharSpy_no_crash_and_continued_writes}
};

// Helper to print test result
static void printResult(const char* name, bool pass, const std::string& detail) {
    if (pass) {
        std::cout << "[PASS] " << name << "\n";
    } else {
        std::cerr << "[FAIL] " << name << ": " << detail << "\n";
    }
}

int main() {
    int total = sizeof(tests) / sizeof(TestCase);
    int failures = 0;

    for (int i = 0; i < total; ++i) {
        const char* testName = tests[i].name;
        try {
            // Run test
            tests[i].func();
            // If no exception, test passed
            printResult(testName, true, "");
        } catch (const std::exception& ex) {
            // Catch assertion failures
            ++failures;
            printResult(testName, false, ex.what());
        } catch (...) {
            ++failures;
            printResult(testName, false, "Unknown exception");
        }
    }

    // Summary
    if (failures == 0) {
        std::cout << "All tests passed (" << total << "/" << total << ").\n";
        return 0;
    } else {
        std::cerr << failures << " test(s) failed out of " << total << ".\n";
        return 1;
    }
}