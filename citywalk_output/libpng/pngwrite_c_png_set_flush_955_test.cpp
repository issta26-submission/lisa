// Lightweight C++11 unit test suite for the focal method: png_set_flush
// This test suite provides a minimal testing framework (no GTest) and a
// self-contained mock of the necessary png structures to validate the logic
// of png_set_flush as defined in the provided snippet.
// Notes:
// - Tests cover NULL pointer handling, positive nrows, and negative nrows cases.
// - A small png_debug mock is used to verify that the debug message is issued.
// - All tests are non-terminating (they run to completion even if some checks fail).

#include <string>
#include <iostream>
#include <pngpriv.h>


// Minimal representation of the libpng types used by the focal method
typedef unsigned int png_uint_32;

// Forward declaration of the debug function used by the focal method.
// We provide a simple stub to capture debug messages for assertions.
void png_debug(int level, const char* message);

// Lightweight mock of the png_struct used by the focal method.
// Only the members required for the tests are included.
struct png_struct {
    png_uint_32 flush_dist;
};

// Pointer-to-struct type alias used by the focal method signature.
// This mirrors the expected "png_structrp" usage (pointer to png_struct).
typedef png_struct* png_structrp;

// Prototype of the focal method under test (as implemented in the test harness).
void png_set_flush(png_structrp png_ptr, int nrows);

// Global variables to capture debug information for assertions.
static int g_last_debug_level = 0;
static std::string g_last_debug_message;

// Debug function stub to capture and inspect debug messages.
void png_debug(int level, const char* message) {
    g_last_debug_level = level;
    g_last_debug_message = (message != nullptr) ? message : "";
}

// Implementation of the focal method under test (copied from the prompt).
void png_set_flush(png_structrp png_ptr, int nrows)
{
{
   png_debug(1, "in png_set_flush");
   if (png_ptr == NULL)
      return;
   png_ptr->flush_dist = (nrows < 0 ? 0 : (png_uint_32)nrows);
}
}

// Lightweight test harness
static int g_total_tests = 0;
static int g_passed_tests = 0;
static int g_failed_tests = 0;

// Non-terminating assertion helper
void expect(bool condition, const char* message) {
    ++g_total_tests;
    if (condition) {
        ++g_passed_tests;
        std::cout << "[PASS] " << message << std::endl;
    } else {
        ++g_failed_tests;
        std::cerr << "[FAIL] " << message << std::endl;
    }
}

// Test 1: Null pointer should not modify any existing state and should trigger a debug
// The function should return immediately when png_ptr is NULL, but the call to png_debug
// occurs before the NULL check according to the focal method.
void test_png_set_flush_nullptr() {
    // Arrange
    png_struct s;
    s.flush_dist = 123; // initial state
    g_last_debug_level = 0;
    g_last_debug_message.clear();

    // Act
    png_set_flush(nullptr, 10);

    // Assert
    expect(s.flush_dist == 123, "Null ptr: flush_dist remains unchanged");
    expect(g_last_debug_level == 1, "Null ptr: debug level is 1");
    expect(g_last_debug_message == "in png_set_flush", "Null ptr: debug message captured");
}

// Test 2: Positive nrows should set flush_dist to the provided value
void test_png_set_flush_positive() {
    // Arrange
    png_struct s;
    s.flush_dist = 0;
    g_last_debug_level = 0;
    g_last_debug_message.clear();

    // Act
    png_set_flush(&s, 5);

    // Assert
    expect(s.flush_dist == 5, "Positive nrows: flush_dist equals 5");
    expect(g_last_debug_level == 1, "Positive nrows: debug level is 1");
    expect(g_last_debug_message == "in png_set_flush", "Positive nrows: debug message captured");
}

// Test 3: Negative nrows should reset flush_dist to 0
void test_png_set_flush_negative() {
    // Arrange
    png_struct s;
    s.flush_dist = 42; // initial non-zero
    g_last_debug_level = 0;
    g_last_debug_message.clear();

    // Act
    png_set_flush(&s, -3);

    // Assert
    expect(s.flush_dist == 0, "Negative nrows: flush_dist is set to 0");
    expect(g_last_debug_level == 1, "Negative nrows: debug level is 1");
    expect(g_last_debug_message == "in png_set_flush", "Negative nrows: debug message captured");
}

// Test 4: Zero nrows should set flush_dist to 0
void test_png_set_flush_zero() {
    // Arrange
    png_struct s;
    s.flush_dist = 7;
    g_last_debug_level = 0;
    g_last_debug_message.clear();

    // Act
    png_set_flush(&s, 0);

    // Assert
    expect(s.flush_dist == 0, "Zero nrows: flush_dist is set to 0");
    expect(g_last_debug_level == 1, "Zero nrows: debug level is 1");
    expect(g_last_debug_message == "in png_set_flush", "Zero nrows: debug message captured");
}

// Test 5: Multiple consecutive calls update flush_dist accordingly
void test_png_set_flush_multiple_calls() {
    // Arrange
    png_struct s;
    s.flush_dist = 0;
    g_last_debug_level = 0;
    g_last_debug_message.clear();

    // Act & Assert: first call
    png_set_flush(&s, 3);
    expect(s.flush_dist == 3, "Multiple calls: first set to 3");
    // Act & Assert: second call
    png_set_flush(&s, -1);
    expect(s.flush_dist == 0, "Multiple calls: second set to 0 (negative)");
    // Act & Assert: third call
    png_set_flush(&s, 10);
    expect(s.flush_dist == 10, "Multiple calls: third set to 10");
}

// Run all tests with a short summary
void run_all_tests() {
    test_png_set_flush_nullptr();
    test_png_set_flush_positive();
    test_png_set_flush_negative();
    test_png_set_flush_zero();
    test_png_set_flush_multiple_calls();

    std::cout << "\nTest Summary: "
              << g_passed_tests << " passed, "
              << g_failed_tests << " failed, "
              << g_total_tests << " total."
              << std::endl;
}

int main() {
    run_all_tests();
    return (g_failed_tests == 0) ? 0 : 1;
}