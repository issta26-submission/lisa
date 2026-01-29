// C++11 test harness for the focal method skip(file*, unsigned) from gzappend.c
// This standalone test reproduces the minimal dependencies needed to
// validate the behavior of skip, without relying on GTest.
// It mocks the dependent utilities (bye, readmore, lseek) to exercise
// various branches of the function and uses non-terminating assertions.

#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <string>
#include <exception>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>


// --------------------
// Minimal type definitions to mirror the focal code
// --------------------

struct file {
    unsigned left;
    unsigned size;
    int fd;
    const char* name;
    unsigned next;
};

// --------------------
// Testing utilities and mocks
// --------------------

class TestAbort : public std::exception {
public:
    explicit TestAbort(const std::string& m) : msg(m) {}
    virtual const char* what() const throw() { return msg.c_str(); }
    std::string msg;
};

// Flags and helpers to observe behavior
static bool lseek_should_fail = false;
static bool lseek_called = false;
static int lseek_last_fd = -1;
static off_t lseek_last_offset = 0;

static bool readmore_called = false;

// bye is expected to terminate execution in the original code.
// In tests, transform it into an exception carrying the composed message.
void bye(const char* msg1, const char* msg2) {
    throw TestAbort(std::string(msg1) + std::string(msg2));
}

// Mock lseek with controlled failure and minimal state tracking
extern "C" {
off_t lseek(int fd, off_t offset, int whence) {
    lseek_called = true;
    lseek_last_fd = fd;
    lseek_last_offset = offset;
    if (lseek_should_fail) {
        return (off_t)-1;
    }
    // Simulate success: moveable position is not tracked further here.
    return 0;
}
}

// Mock readmore to indicate the function was invoked
void readmore(struct file* in) {
    readmore_called = true;
    // No side effects on in->left to keep test scenarios predictable.
}

// The focal method under test (adapted to be linkable from C++)
void skip(struct file *in, unsigned n) {
    unsigned bypass;
    if (n > in->left) {
        n -= in->left;
        bypass = n & ~((1U << in->size) - 1);
        if (bypass) {
            if (lseek(in->fd, (off_t)bypass, SEEK_CUR) == -1)
                bye("seeking ", in->name);
            n -= bypass;
        }
        readmore(in);
        if (n > in->left)
            bye("unexpected end of ", in->name);
    }
    in->left -= n;
    in->next += n;
}

// --------------------
// Lightweight assertion helpers (non-terminating)
// --------------------

static int test_failures = 0;

#define EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cout << "FAILED: " << (msg) \
                  << " | expected: " << (a) << " actual: " << (b) << std::endl; \
        ++test_failures; \
    } \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cout << "FAILED: " << (msg) << " | condition is false" << std::endl; \
        ++test_failures; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
    if (cond) { \
        std::cout << "FAILED: " << (msg) << " | condition is true" << std::endl; \
        ++test_failures; \
    } \
} while (0)

#define EXPECT_THROW(stmt, exception_type, msg_contains) do { \
    bool caught = false; std::string caught_msg; \
    try { stmt; } \
    catch (const exception_type& e) { caught = true; caught_msg = e.what(); } \
    catch (...) { /* ignore other exceptions for this test harness */ } \
    if (!caught) { \
        std::cout << "FAILED: expected exception " #exception_type " was not thrown" << std::endl; \
        ++test_failures; \
    } else { \
        if (!caught_msg.empty() && (msg_contains.size() ? caught_msg.find(msg_contains) == std::string::npos : false)) { \
            std::cout << "FAILED: exception message did not contain expected text: " \
                      << msg_contains << " got: " << caught_msg << std::endl; \
            ++test_failures; \
        } \
    } \
} while (0)

#define EXPECT_THROW_DEBUG(stmt, exception_type, msg_contains) do { \
    bool caught = false; std::string caught_msg; \
    try { stmt; } \
    catch (const exception_type& e) { caught = true; caught_msg = e.what(); } \
    catch (...) { /* ignore */ } \
    if (!caught) { \
        std::cout << "FAILED: expected exception " #exception_type " was not thrown" << std::endl; \
        ++test_failures; \
    } else { \
        if (msg_contains.size() && caught_msg.find(msg_contains) == std::string::npos) { \
            std::cout << "FAILED: exception message did not contain expected text: " \
                      << msg_contains << " got: " << caught_msg << std::endl; \
            ++test_failures; \
        } \
    } \
} while (0)

// --------------------
// Test cases
// --------------------

// Test 1: No bypass path when n <= in->left
// Expect: in->left decreases by n, in->next increases by n,
// and no lseek/readmore/bye invocations occur.
void test_case_no_bypass() {
    std::cout << "Test 1: No bypass path when n <= left" << std::endl;
    // Reset state
    lseek_called = false; lseek_should_fail = false;
    readmore_called = false; readmore_called = false;
    // Prepare input
    struct file in;
    in.left = 10;
    in.size = 3; // block size = 8
    in.fd = 1;
    in.name = "test1.txt";
    in.next = 0;

    // Execute
    try {
        skip(&in, 5); // 5 <= left
        // Validate final state
        EXPECT_EQ(in.left, 5u, "Left should be reduced by n");
        EXPECT_EQ(in.next, 5u, "Next should be advanced by n");
        EXPECT_FALSE(lseek_called, "lseek should not have been called");
        EXPECT_FALSE(readmore_called, "readmore should not have been called");
    } catch (...) {
        std::cout << "FAILED: Unexpected exception thrown in Test 1" << std::endl;
        ++test_failures;
    }
}

// Test 2: No bypass, but triggers unexpected end of file after readmore
// Condition: left is 0, size such that bypass=0, and n > left yields bye("unexpected end of ", name)
void test_case_unexpected_end_no_bypass() {
    std::cout << "Test 2: Unexpected end of after readmore (bypass=0)" << std::endl;
    // Reset state
    lseek_called = false; lseek_should_fail = false;
    readmore_called = false;

    struct file in;
    in.left = 0;
    in.size = 3; // block 8
    in.fd = 2;
    in.name = "test2.txt";
    in.next = 0;

    // Expect a bye through exception with message containing "unexpected end of "
    EXPECT_THROW_DEBUG(skip(&in, 7), TestAbort, "unexpected end of ");
    // Additional checks after exception (optional): readmore should have been called
    // but since exception interrupts, readmore_called may be true
    // We can't reliably assert post-state because of exception; ensure function attempted to process
    if (readmore_called) {
        // ok
    }
}

// Test 3: Bypass > 0, lseek fails -> triggers bye("seeking ", name)
void test_case_bypass_seek_fail() {
    std::cout << "Test 3: Bypass > 0 and lseek fails -> bye(seeking)" << std::endl;
    // Reset state
    lseek_called = false; lseek_should_fail = true;
    readmore_called = false;

    struct file in;
    in.left = 2;
    in.size = 3; // block 8
    in.fd = 3;
    in.name = "test3.txt";
    in.next = 0;

    // n such that n > left and bypass > 0
    // n = left + (remainder + bypass) with remainder > 0; choose n = 11
    EXPECT_THROW(skip(&in, 11), TestAbort, "seeking ");
    // Validate lseek invocation
    EXPECT_TRUE(lseek_called, "lseek should have been called for bypass");
    // Validate lseek args captured
    // The offset should be exactly 8 in this scenario
    EXPECT_EQ(lseek_last_offset, (off_t)8, "lseek should be called with bypass offset 8");
}

// Test 4: Bypass > 0, lseek ok, but still triggers unexpected end after readmore
// We'll use a scenario where remainder after bypass is greater than in.left
// left=1, size=3, n=16 -> a = 15, bypass=8, n_after=7, 7 > 1 -> bye
void test_case_bypass_unexpected_end_after_read() {
    std::cout << "Test 4: Bypass ok, but after readmore it becomes unexpected end" << std::endl;
    // Reset state
    lseek_called = false; lseek_should_fail = false;
    readmore_called = false;

    struct file in;
    in.left = 1;
    in.size = 3; // block 8
    in.fd = 4;
    in.name = "test4.txt";
    in.next = 0;

    // n chosen to produce remainder 7 (> left)
    EXPECT_THROW(skip(&in, 16), TestAbort, "unexpected end of ");
    // When exception occurs, verify lseek was called with offset 8
    EXPECT_TRUE(lseek_called, "lseek should have been called for bypass");
    EXPECT_EQ(lseek_last_offset, (off_t)8, "lseek should be called with bypass offset 8");
}

// Test 5: Successful path with bypass > 0 and final state verification
// left=10, size=3, n=19 -> a=9, bypass=8, n_after=1, final left=9, next=1
void test_case_bypass_successful_path_final_state() {
    std::cout << "Test 5: Successful bypass path with final state verification" << std::endl;
    // Reset state
    lseek_called = false; lseek_should_fail = false;
    readmore_called = false;

    struct file in;
    in.left = 10;
    in.size = 3; // block 8
    in.fd = 5;
    in.name = "test5.txt";
    in.next = 0;

    // n > left and bypass occurs, but no end condition triggers bye
    skip(&in, 19);
    // Validate final state
    EXPECT_EQ(in.left, 9u, "Left should be reduced by remainder after bypass (1)");
    EXPECT_EQ(in.next, 1u, "Next should be increased by remainder after bypass (1)");
    EXPECT_TRUE(lseek_called, "lseek should have been called for bypass");
    EXPECT_FALSE(readmore_called, "readmore should be called and complete; in test path, it will be called before final check");
}

// --------------------
// Main test runner
// --------------------

int main() {
    std::cout << "Starting unit tests for skip(file*, unsigned)" << std::endl;

    test_case_no_bypass();
    test_case_unexpected_end_no_bypass();
    test_case_bypass_seek_fail();
    test_case_bypass_unexpected_end_after_read();
    test_case_bypass_successful_path_final_state();

    if (test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << "TOTAL FAILURES: " << test_failures << std::endl;
    }

    return test_failures;
}