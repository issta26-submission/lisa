// C++11 single-file unit test suite for the focal method putShortMSB
// Note: This test is designed to be self-contained and does not depend on GTest.
// It provides a minimal fake environment to exercise the logic of putShortMSB
// (two consecutive writes: high byte first, then low byte) by routing
// writes through a test-provided put_byte.
// The test suite focuses on verifying correct MSB/LSB extraction and write order.

#include <string>
#include <deflate.h>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <vector>


// Domain knowledge compatibility notes (informational, not used by the compiler):
// - We emulate the deflate_state buffer writing via a small, test-only definition.
// - putShortMSB writes two bytes: high byte (b>>8) then low byte (b&0xff).
// - We ensure behavior under normal and overflow conditions (to cover branches).

// Lightweight typedefs to mirror the real types used in the focal function
typedef unsigned char Byte;
typedef unsigned int  uInt;

// Lightweight fake deflate_state suitable for testing the focal function.
// It tracks an output buffer, its capacity, and the current write position.
struct deflate_state {
    Byte*  output;   // pointer to an output buffer
    size_t size;     // capacity of the output buffer
    size_t pos;      // current write position (number of bytes written)
    bool   overrun;  // flag set if a write would exceed capacity
};

// Global test buffer to be used by tests
static Byte g_testBuffer[256];

// Forward declaration of the test-friendly put_byte used by putShortMSB.
// In the real project, put_byte is provided by the production code.
// Here we provide a test-friendly version to capture the writes.
static void put_byte(deflate_state* s, Byte c);

// The focal method under test, implemented here in test-friendly form.
// It mirrors: local void putShortMSB(deflate_state *s, uInt b) { put_byte(s, (Byte)(b >> 8)); put_byte(s, (Byte)(b & 0xff)); }
static void putShortMSB(deflate_state* s, uInt b) {
    put_byte(s, (Byte)(b >> 8));
    put_byte(s, (Byte)(b & 0xff));
}

// Test helper: implement put_byte to write into the test buffer with bounds checking.
static void put_byte(deflate_state* s, Byte c) {
    if (s == nullptr) return;
    if (s->pos < s->size) {
        s->output[s->pos++] = c;
    } else {
        s->overrun = true;
        // In the real code, behavior on overrun would be undefined or handled differently.
        // Here we simply mark the overrun flag and skip writing beyond capacity.
    }
}

// Minimal test framework
struct TestFramework {
    int total{0};
    int failed{0};
    using TestFn = std::function<void()>;
    std::vector<std::string> failures;

    void run(const std::string& name, TestFn fn) {
        ++total;
        try {
            fn();
        } catch (const std::exception& e) {
            ++failed;
            failures.push_back(name + " threw: " + e.what());
        } catch (...) {
            ++failed;
            failures.push_back(name + " threw an unknown exception");
        }
    }

    void report() const {
        std::cout << "\nTest results: " << total << " test(s) run, "
                  << failed << " failure(s).\n";
        for (const auto& f : failures) {
            std::cout << "  - " << f << "\n";
        }
        if (failed == 0) {
            std::cout << "All tests passed.\n";
        }
    }
};

// Helper to reset a deflate_state for a fresh test
static deflate_state make_state(size_t capacity) {
    deflate_state s;
    s.output = g_testBuffer;
    s.size   = capacity;
    s.pos    = 0;
    s.overrun = false;
    // Ensure buffer is zeroed for deterministic comparison (optional)
    for (size_t i = 0; i < capacity; ++i) g_testBuffer[i] = 0;
    return s;
}

// Test 1: Basic correctness - high byte then low byte are written in big-endian order
// This verifies that putShortMSB writes two bytes: high-order then low-order.
static void test_putShortMSB_writes_high_then_low_basic() {
    // Arrange
    deflate_state s = make_state(256);
    uint16_t value = 0x1234; // High byte = 0x12, Low byte = 0x34

    // Act
    putShortMSB(&s, value);

    // Assert
    if (s.pos != 2) {
        throw std::runtime_error("Expected pos to be 2 after putShortMSB call");
    }
    if (s.output[0] != 0x12) {
        throw std::runtime_error("Expected first byte to be 0x12 (MSB)");
    }
    if (s.output[1] != 0x34) {
        throw std::runtime_error("Expected second byte to be 0x34 (LSB)");
    }
}

// Test 2: Boundary check - zero value should produce two zero bytes in order
static void test_putShortMSB_zero_value_produces_two_zero_bytes() {
    deflate_state s = make_state(256);
    uint16_t value = 0x0000;

    putShortMSB(&s, value);

    if (s.pos != 2) {
        throw std::runtime_error("Expected pos to be 2 after putShortMSB for zero value");
    }
    if (s.output[0] != 0x00 || s.output[1] != 0x00) {
        throw std::runtime_error("Expected both bytes to be 0x00 for zero value");
    }
}

// Test 3: Large value order verification - ensures MSB is placed before LSB
static void test_putShortMSB_order_for_various_values() {
    deflate_state s = make_state(256);
    unsigned int v1 = 0xABCD;
    putShortMSB(&s, v1); // Expect 0xAB, 0xCD

    if (s.pos != 2 || s.output[0] != 0xAB || s.output[1] != 0xCD) {
        throw std::runtime_error("Order test failed for 0xABCD (expected 0xAB 0xCD)");
    }

    // Reset only the position for a second value check
    s.pos = 0;
    s.overrun = false;
    unsigned int v2 = 0x1234;
    putShortMSB(&s, v2); // Expect 0x12, 0x34

    if (s.pos != 2 || s.output[0] != 0x12 || s.output[1] != 0x34) {
        throw std::runtime_error("Order test failed for 0x1234 (expected 0x12 0x34)");
    }
}

// Test 4: Overflow handling - ensure behavior when buffer capacity is exhausted
static void test_putShortMSB_overflow_sets_overrun_and_writes_as_possible() {
    // Capacity exactly 1, so second write should trigger overrun flag
    deflate_state s = make_state(1);
    putShortMSB(&s, 0x1234); // First write should succeed, second should cause overrun

    // After first write, pos == 1; second write cannot occur
    if (s.pos != 1) {
        throw std::runtime_error("Expected pos to be 1 after first write on overflow test");
    }
    if (!s.overrun) {
        throw std::runtime_error("Expected overrun flag to be set due to insufficient space for second byte");
    }
    // The first byte should be 0x12, and the second byte would be unswritten (buffer size 1)
    if (s.output[0] != 0x12) {
        throw std::runtime_error("Expected first byte to be 0x12 before overflow");
    }
}

// Test 5: Multiple consecutive calls - verify sequential writes accumulate correctly
static void test_putShortMSB_multiple_consecutive_calls() {
    deflate_state s = make_state(256);
    putShortMSB(&s, 0x1111); // Expect 0x11, 0x11
    putShortMSB(&s, 0x2222); // Expect 0x22, 0x22

    if (s.pos != 4) {
        throw std::runtime_error("Expected pos to be 4 after two consecutive putShortMSB calls");
    }
    if (s.output[0] != 0x11 || s.output[1] != 0x11 ||
        s.output[2] != 0x22 || s.output[3] != 0x22) {
        throw std::runtime_error("Unexpected byte sequence after two consecutive putShortMSB calls");
    }
}

int main() {
    TestFramework tf;

    // Register and run tests with explanatory comments for maintainers
    tf.run("putShortMSB_writes_high_then_low_basic", test_putShortMSB_writes_high_then_low_basic);
    tf.run("putShortMSB_zero_value_produces_two_zero_bytes", test_putShortMSB_zero_value_produces_two_zero_bytes);
    tf.run("putShortMSB_order_for_various_values", test_putShortMSB_order_for_various_values);
    tf.run("putShortMSB_overflow_sets_overrun_and_writes_as_possible", test_putShortMSB_overflow_sets_overrun_and_writes_as_possible);
    tf.run("putShortMSB_multiple_consecutive_calls", test_putShortMSB_multiple_consecutive_calls);

    tf.report();
    // Exit code 0 if all tests pass; non-zero if any test fails
    return (tf.failed == 0) ? 0 : 1;
}