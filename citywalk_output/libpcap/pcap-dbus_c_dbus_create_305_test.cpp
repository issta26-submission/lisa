/*
Unit test suite for the focal method: dbus_create (in pcap-dbus.c)
- Target environment: C++11, no Google Test (GTest) usage
- Approach: Lightweight in-house test harness with non-terminating assertions
- Strategy:
  - Validate the false branch (early return) when device is not one of:
      "dbus-system", "dbus-session", or prefixed with "dbus://"
  - Validate the true branch (activation path) when device matches any accepted form
  - Since internal pcap_t structure fields are not publicly accessible, tests focus on
    function outcomes (pointer NULL vs non-NULL) and the is_ours flag
- Assumptions:
  - The project under test provides a C interface dbus_create with signature:
      pcap_t* dbus_create(const char *device, char *ebuf, int *is_ours)
  - The test program is linked with the appropriate libpcap (or equivalent) build
    to satisfy PCAP_CREATE_COMMON and related internals when a valid device is supplied.
- Notes:
  - We declare the C prototype with C linkage to call from C++11 test code.
  - The test harness uses simple continue-on-failure assertions to maximize coverage.
  - Each test case includes explanatory comments describing the scenario and expectations.
*/

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <dbus/dbus.h>
#include <config.h>
#include <pcap-dbus.h>
#include <pcap-int.h>
#include <cstring>
#include <iostream>


// Forward declare the C API for dbus_create with C linkage
extern "C" {
    // libpcap typically defines "typedef struct pcap pcap_t;" in its headers.
    // We mirror that declaration here to allow pointer usage without requiring the
    // full definition in this test file.
    typedef struct pcap pcap_t;

    // Focal method under test
    pcap_t* dbus_create(const char *device, char *ebuf, int *is_ours);
}

// Simple in-house test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_passed_tests = 0;

#define TEST_PASS(msg) do { std::cout << "[PASS] " << msg << std::endl; g_passed_tests++; } while(0)
#define TEST_FAIL(msg) do { std::cerr << "[FAIL] " << msg << std::endl; } while(0)
#define TEST_ASSERT(cond, msg) do { \
    ++g_total_tests; \
    if (cond) { TEST_PASS(msg); g_passed_tests++; } \
    else { TEST_FAIL(msg); } \
} while(0)

// Test 1: Invalid device string should trigger the early return path
// - Expected: NULL pointer and is_ours == 0
void test_invalid_device_returns_null_and_not_ours()
{
    // Domain knowledge: any device not in { "dbus-system", "dbus-session" } and not starting
    // with "dbus://" should be treated as not ours.
    const char *device = "random-device";
    char ebuf[256];
    int is_ours = -1;

    pcap_t *p = dbus_create(device, ebuf, &is_ours);

    // Assertions
    TEST_ASSERT(p == nullptr, "dbus_create should return NULL for invalid device");
    TEST_ASSERT(is_ours == 0, "dbus_create should set is_ours to 0 for invalid device");
}

// Test 2: Valid device "dbus-system" should trigger the activation path
// - Expected: Non-NULL pointer and is_ours == 1
void test_dbus_system_device_returns_non_null_and_ours()
{
    const char *device = "dbus-system";
    char ebuf[256];
    int is_ours = 0;

    pcap_t *p = dbus_create(device, ebuf, &is_ours);

    // Assertions
    TEST_ASSERT(p != nullptr, "dbus_create should return non-NULL for 'dbus-system'");
    TEST_ASSERT(is_ours == 1, "dbus_create should set is_ours to 1 for 'dbus-system'");
}

// Test 3: Valid device "dbus-session" should trigger the activation path
// - Expected: Non-NULL pointer and is_ours == 1
void test_dbus_session_device_returns_non_null_and_ours()
{
    const char *device = "dbus-session";
    char ebuf[256];
    int is_ours = 0;

    pcap_t *p = dbus_create(device, ebuf, &is_ours);

    // Assertions
    TEST_ASSERT(p != nullptr, "dbus_create should return non-NULL for 'dbus-session'");
    TEST_ASSERT(is_ours == 1, "dbus_create should set is_ours to 1 for 'dbus-session'");
}

// Test 4: Valid device with dbus:// prefix should trigger the activation path
// - Expected: Non-NULL pointer and is_ours == 1
void test_dbus_prefix_device_returns_non_null_and_ours()
{
    const char *device = "dbus://example/service";
    char ebuf[256];
    int is_ours = 0;

    pcap_t *p = dbus_create(device, ebuf, &is_ours);

    // Assertions
    TEST_ASSERT(p != nullptr, "dbus_create should return non-NULL for 'dbus://' prefixed device");
    TEST_ASSERT(is_ours == 1, "dbus_create should set is_ours to 1 for 'dbus://' prefixed device");
}

// Entry point for running the test suite
int main()
{
    std::cout << "Starting tests for dbus_create (pcap-dbus.c) ..." << std::endl;

    test_invalid_device_returns_null_and_not_ours();
    test_dbus_system_device_returns_non_null_and_ours();
    test_dbus_session_device_returns_non_null_and_ours();
    test_dbus_prefix_device_returns_non_null_and_ours();

    std::cout << "Test summary: " << g_passed_tests << " / " << g_total_tests
              << " tests passed." << std::endl;

    // Return non-zero if any test failed
    return (g_passed_tests == g_total_tests) ? 0 : 1;
}