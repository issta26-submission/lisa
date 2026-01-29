// Minimal, self-contained unit tests for gzfilebuf::sync() without using GoogleTest.
// The tests create a small derived class to control the behavior of is_open(), out_waiting(), and flushbuf()
// to verify the three execution paths inside gzfilebuf::sync().

#include <iostream>
#include <string>
#include <cstdio>
#include <zfstream.h>


// Include the focal header containing gzfilebuf and related classes.

// Lightweight testing helpers (non-terminating assertions)
namespace TestFramework {
    static int tests_run = 0;
    static int tests_passed = 0;

    inline void report_pass(const std::string& name) {
        std::cout << "[PASS] " << name << std::endl;
        ++tests_run;
        ++tests_passed;
    }

    inline void report_fail(const std::string& name, const std::string& reason) {
        std::cerr << "[FAIL] " << name << ": " << reason << std::endl;
        ++tests_run;
    }

    inline void assert_eq(const std::string& name, int expected, int actual) {
        if (expected == actual) {
            report_pass(name);
        } else {
            report_fail(name, "expected " + std::to_string(expected) +
                                  ", got " + std::to_string(actual));
        }
    }

    inline void summary() {
        std::cout << "Tests run: " << tests_run << ", Passed: " << tests_passed
                  << ", Failed: " << (tests_run - tests_passed) << std::endl;
    }
}

// Derived test subclass to control internal behavior of gzfilebuf.
// We assume the base class defines the following virtual hooks (as suggested by typical design):
//   virtual bool is_open();
//   virtual bool out_waiting();
//   virtual int  flushbuf();
class TestGzfilebuf : public gzfilebuf {
public:
    TestGzfilebuf() : openFlag(false), waitingFlag(false), flushVal(0) {}

    // Override virtual hooks to drive test scenarios.
    virtual bool is_open() override { return openFlag; }
    virtual bool out_waiting() override { return waitingFlag; }
    virtual int  flushbuf() override { return flushVal; }

    // Setters to configure test scenarios
    void setOpenFlag(bool v)   { openFlag = v; }
    void setWaitingFlag(bool v) { waitingFlag = v; }
    void setFlushVal(int v)     { flushVal = v; }

private:
    bool openFlag;
    bool waitingFlag;
    int  flushVal;
};

// Test 1: When the stream is not open, sync() should return EOF.
void test_sync_not_open() {
    TestGzfilebuf buf;
    buf.setOpenFlag(false);   // is_open() returns false
    buf.setWaitingFlag(false);
    int result = buf.sync();
    TestFramework::assert_eq("test_sync_not_open (not open -> EOF)", EOF, result);
}

// Test 2: When the stream is open and there is data waiting, sync() should call flushbuf()
// and return the value from flushbuf().
void test_sync_open_and_out_waiting() {
    TestGzfilebuf buf;
    buf.setOpenFlag(true);      // is_open() returns true
    buf.setWaitingFlag(true);   // out_waiting() returns true
    buf.setFlushVal(123);       // flushbuf() returns 123
    int result = buf.sync();
    TestFramework::assert_eq("test_sync_open_and_out_waiting (flushbuf value)", 123, result);
}

// Test 3: When the stream is open but there is no data waiting, sync() should return 0.
void test_sync_open_no_out_waiting() {
    TestGzfilebuf buf;
    buf.setOpenFlag(true);      // is_open() returns true
    buf.setWaitingFlag(false);  // out_waiting() returns false
    buf.setFlushVal(999);       // Should not be used
    int result = buf.sync();
    TestFramework::assert_eq("test_sync_open_no_out_waiting (no flush -> 0)", 0, result);
}

// Main function to run tests
int main() {
    // Run the focused tests for gzfilebuf::sync()
    test_sync_not_open();
    test_sync_open_and_out_waiting();
    test_sync_open_no_out_waiting();

    // Summary
    TestFramework::summary();
    return (TestFramework::tests_run == TestFramework::tests_passed) ? 0 : 1;
}