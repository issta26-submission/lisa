// Test suite for the focal method dlinfoack(int fd, char *bufp, char *ebuf)
// This unit test is designed for a C/C++11 environment without GoogleTest.
// It relies on providing a test-scoped mock for the external function `recv_ack`
// that dlinfoack delegates to. The goal is to validate:
// - that dlinfoack forwards its arguments to recv_ack (bufp/ebuf/what)
// - that dlinfoack returns whatever value recv_ack returns
// - that the particular constant string used by dlinfoack ("info") is passed to recv_ack
//
// Notes:
// - This test file assumes that pcap-dlpi.c (or its equivalent) is linked with this
//   test and that a symbol `dlinfoack(int, char*, char*)` is available.
// - We provide a C linkage mock for `recv_ack` to intercept calls and verify
//   parameters. If the target build defines recv_ack in a different translation unit,
//   adjust the build so that the mock overrides that symbol (e.g., through linker
//   options or by integrating the mock into the compilation unit that defines
//   dlinfoack).

#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/systeminfo.h>
#include <limits.h>
#include <sys/dlpi.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <dlpisubs.h>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <os-proto.h>
#include <sys/bufmod.h>
#include <config.h>
#include <cstring>
#include <sys/stream.h>


// Forward declarations of functions from the code under test.
// We assume the following C interfaces are available in the build under test.
extern "C" int dlinfoack(int fd, char *bufp, char *ebuf);
extern "C" int recv_ack(int fd, int size, const char *what, char *bufp, char *ebuf, int *uerror);

// --------------------------
// Mock infrastructure
// --------------------------
// Global state used by the mock recv_ack to inspect how dlinfoack calls it.
static int mock_ret_value = 0;
static const char *last_what_seen = nullptr;
static int last_size_seen = 0;
static char *last_bufp_seen = nullptr;
static char *last_ebuf_seen = nullptr;
static int *last_uerror_seen = nullptr;

// A deterministic mock for recv_ack to intercept the call from dlinfoack.
// Provide C linkage so it can override/link with the real implementation in the test target.
extern "C" int recv_ack(int fd, int size, const char *what, char *bufp, char *ebuf, int *uerror)
{
    // Capture arguments for assertions in tests.
    last_size_seen = size;
    last_what_seen = what;
    last_bufp_seen = bufp;
    last_ebuf_seen = ebuf;
    last_uerror_seen = uerror;

    // Return the value controlled by the test to verify propagation.
    return mock_ret_value;
}

// Helper: Reset observed state before each test
static void reset_mock_observables() {
    mock_ret_value = 0;
    last_what_seen = nullptr;
    last_size_seen = 0;
    last_bufp_seen = nullptr;
    last_ebuf_seen = nullptr;
    last_uerror_seen = nullptr;
}

// Utility: Simple assertion with non-terminating behavior (we print errors but continue).
static bool assert_true(bool cond, const char* message)
{
    if (!cond) {
        std::cerr << "ASSERTION FAILED: " << message << std::endl;
        return false;
    }
    return true;
}

// --------------------------
// Test 1: Basic pass-through behavior
// - Verifies that dlinfoack returns exactly what recv_ack returns.
// - Verifies that recv_ack receives: what == "info", and that bufp/ebuf are forwarded.
// --------------------------
static bool test_dlinfoack_basic_pass_through()
{
    reset_mock_observables();

    // Arrange
    mock_ret_value = 12345; // arbitrary sentinel to verify propagation
    char buf[64];
    char ebuf[64];

    // Act
    int ret = dlinfoack(3 /*fd*/, buf, ebuf);

    // Assert
    bool ok = true;
    ok &= assert_true(ret == 12345, "dlinfoack should return the value produced by recv_ack");

    // The code under test uses the exact string "info" for the 'what' parameter.
    ok &= assert_true(last_what_seen != nullptr, "recv_ack 'what' parameter should be set");
    ok &= assert_true(std::strcmp(last_what_seen, "info") == 0,
                      "recv_ack 'what' parameter should be exactly 'info'");

    ok &= assert_true(last_bufp_seen == buf, "recv_ack 'bufp' should be forwarded pointer to buf");
    ok &= assert_true(last_ebuf_seen == ebuf, "recv_ack 'ebuf' should be forwarded pointer to ebuf");
    // The test does not rely on the specific numeric value of 'size' (DL_INFO_ACK_SIZE),
    // only that something is passed through. We ensure it's non-zero.
    ok &= assert_true(last_size_seen != 0, "recv_ack 'size' should be non-zero (DL_INFO_ACK_SIZE)");

    return ok;
}

// --------------------------
// Test 2: Behavior with null ebuf (boundary case)
// - Verifies that even if ebuf is NULL, the call forwards the NULL sentinel.
// - Ensures return value propagation remains intact.
// --------------------------
static bool test_dlinfoack_null_ebuf_handling()
{
    reset_mock_observables();

    mock_ret_value = 77;
    char buf[32];

    // Act
    int ret = dlinfoack(7, buf, nullptr);

    // Assert
    bool ok = true;
    ok &= assert_true(ret == 77, "dlinfoack should propagate return value when recv_ack returns 77");
    ok &= assert_true(last_ebuf_seen == nullptr, "recv_ack 'ebuf' should be forwarded as nullptr when ebuf is NULL");
    ok &= assert_true(last_bufp_seen == buf, "recv_ack 'bufp' should be forwarded pointer to buf");
    ok &= assert_true(std::strcmp(last_what_seen, "info") == 0,
                      "recv_ack 'what' parameter should still be 'info'");

    // We cannot assert about interally how DL_INFO_ACK_SIZE is defined here, but we can ensure
    // the size parameter is non-zero, indicating a valid call path through dlinfoack.
    ok &= assert_true(last_size_seen != 0, "recv_ack 'size' should be non-zero for NULL ebuf case");

    return ok;
}

// --------------------------
// Test 3: Multiple invocations with different return values
// - Ensures that dlinfoack returns whatever recv_ack returns for successive calls.
// - Useful to exercise the wrapper's consistency across invocations.
// --------------------------
static bool test_dlinfoack_multiple_invocations()
{
    reset_mock_observables();

    // First invocation
    mock_ret_value = 1;
    char a[16], b[16];
    int r1 = dlinfoack(1, a, b);

    // Second invocation with a different sentinel
    mock_ret_value = -42;
    char c[8], d[8];
    int r2 = dlinfoack(2, c, d);

    bool ok = true;
    ok &= assert_true(r1 == 1, "First invocation should return the first mock value");
    ok &= assert_true(r2 == -42, "Second invocation should return the second mock value");

    return ok;
}

// --------------------------
// Main
// Executes all tests and reports the overall result.
// --------------------------
int main()
{
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool (*test)()) {
        ++total;
        bool ok = test();
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << name << std::endl;
        } else {
            std::cout << "[FAIL] " << name << std::endl;
        }
        return ok;
    };

    // Execute tests
    bool t1 = run("test_dlinfoack_basic_pass_through", test_dlinfoack_basic_pass_through);
    bool t2 = run("test_dlinfoack_null_ebuf_handling", test_dlinfoack_null_ebuf_handling);
    bool t3 = run("test_dlinfoack_multiple_invocations", test_dlinfoack_multiple_invocations);

    // Summary
    std::cout << "=========================" << std::endl;
    std::cout << "Total tests: " << total << ", Passed: " << passed << ", Failed: "
              << (total - passed) << std::endl;

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}