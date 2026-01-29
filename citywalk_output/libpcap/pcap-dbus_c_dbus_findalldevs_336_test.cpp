// Test harness for the focal method dbus_findalldevs
// This standalone test mirrors the control flow of the original function
// and injects a mock for pcapint_add_dev to enable deterministic testing
// of both true/false branches without requiring the entire pcap build system.
//
// Notes:
// - The test provides a minimal pcap_if_list_t type and the necessary
//   PCAP_IF_CONNECTION_STATUS_NOT_APPLICABLE macro to compile.
// - A C linkage function pcapint_add_dev is provided as a stub that can be
//   driven by a global test vector to simulate success or failure.
// - The test suite covers: both additions succeed, first addition fails,
//   second addition fails, and verifies the side effects (which devices were added).

#include <string.h>
#include <vector>
#include <time.h>
#include <sys/time.h>
#include <dbus/dbus.h>
#include <config.h>
#include <pcap-dbus.h>
#include <pcap-int.h>
#include <cstring>
#include <string>
#include <iostream>


// Lightweight stand-in for the real C type used by the focal function
typedef struct pcap_if_list {
    // Intentionally empty: the test only needs a pointer to pass through
} pcap_if_list_t;

// The focal code uses this macro; we define a compatible placeholder.
#define PCAP_IF_CONNECTION_STATUS_NOT_APPLICABLE 0

// Forward declaration for the mock that will be provided in this test file.
extern "C" void* pcapint_add_dev(pcap_if_list_t *devlistp,
                                const char *name,
                                int conn,
                                const char *description,
                                char *err_str);

// The focal function under test (re-implemented here for a self-contained test).
// It mirrors the exact logic shown in the <FOCAL_METHOD> section.
extern "C" int dbus_findalldevs(pcap_if_list_t *devlistp, char *err_str)
{
    // The notion of connected/disconnected isn't used for these DBus devices.
    if (pcapint_add_dev(devlistp,
                        "dbus-system",
                        PCAP_IF_CONNECTION_STATUS_NOT_APPLICABLE,
                        "D-Bus system bus",
                        err_str) == NULL)
        return -1;
    if (pcapint_add_dev(devlistp,
                        "dbus-session",
                        PCAP_IF_CONNECTION_STATUS_NOT_APPLICABLE,
                        "D-Bus session bus",
                        err_str) == NULL)
        return -1;
    return 0;
}

// Global test control: each entry determines whether the next call to
// pcapint_add_dev should succeed (true) or fail (false).
static std::vector<bool> g_pcapint_add_dev_results;  // LIFO-like: consume from front
static std::vector<std::string> g_added_device_names; // record device names added (for assertion)

// Helper to reset test state before each case
static void reset_test_state() {
    g_pcapint_add_dev_results.clear();
    g_added_device_names.clear();
}

// Injected: mock implementation of pcapint_add_dev
extern "C" void* pcapint_add_dev(pcap_if_list_t *devlistp,
                                const char *name,
                                int conn,
                                const char *description,
                                char *err_str)
{
    // If no explicit test outcome is configured, default to success.
    bool should_succeed = true;
    if (!g_pcapint_add_dev_results.empty()) {
        should_succeed = g_pcapint_add_dev_results.front();
        g_pcapint_add_dev_results.erase(g_pcapint_add_dev_results.begin());
    }

    if (should_succeed) {
        // Record the device name to validate test behavior
        if (name) {
            g_added_device_names.push_back(std::string(name));
        }
        // Return a non-NULL pointer to indicate success
        return (void*)name;
    } else {
        // On failure, optionally populate an error string
        if (err_str) {
            const char *mock_err = "mocked add_dev failure";
            std::strcpy(err_str, mock_err);
        }
        return NULL;
    }
}

// Simple assertion helper to report failures without terminating tests prematurely
#define ASSERT_EQ(actual, expected, message)                                   \
    do {                                                                         \
        if ((actual) != (expected)) {                                           \
            std::cerr << "ASSERTION FAILED: " << (message) << " | got: "        \
                      << (actual) << ", expected: " << (expected) << std::endl; \
            add_failure(message);                                               \
        } else {                                                                \
            std::cout << "PASS: " << (message) << std::endl;                   \
        }                                                                       \
    } while (0)

static int g_failure_count = 0;
static void add_failure(const std::string& msg) { ++g_failure_count; }

// Convenience macro to wrap a test case execution
#define RUN_TEST(test_func) test_func()

// Test 1: both additions succeed -> dbus_findalldevs returns 0 and records both devices
static void test_both_adds_succeed() {
    reset_test_state();
    // Configure: first add succeeds, second add succeeds
    g_pcapint_add_dev_results = { true, true };

    // Prepare test inputs
    pcap_if_list_t devlist;
    char err_buf[256] = {0};

    // Execute
    int result = dbus_findalldevs(&devlist, err_buf);

    // Assertions
    ASSERT_EQ(result, 0, "dbus_findalldevs should return 0 when both adds succeed");
    // Verify that both devices were recorded in order
    ASSERT_EQ((int)g_added_device_names.size(), 2, "Two devices should be added when both succeeds");
    if (g_added_device_names.size() == 2) {
        ASSERT_EQ(g_added_device_names[0], std::string("dbus-system"), "First added device should be dbus-system");
        ASSERT_EQ(g_added_device_names[1], std::string("dbus-session"), "Second added device should be dbus-session");
    }
}

// Test 2: first addition fails -> dbus_findalldevs returns -1 and no second device is attempted
static void test_first_add_fails() {
    reset_test_state();
    // Configure: first add fails
    g_pcapint_add_dev_results = { false };

    pcap_if_list_t devlist;
    char err_buf[256] = {0};

    int result = dbus_findalldevs(&devlist, err_buf);

    ASSERT_EQ(result, -1, "dbus_findalldevs should return -1 when first add_dev fails");
    // No device should be recorded
    ASSERT_EQ((int)g_added_device_names.size(), 0, "No devices should be added when first add fails");
}

// Test 3: first add succeeds, second add fails -> dbus_findalldevs returns -1
static void test_second_add_fails() {
    reset_test_state();
    // Configure: first add succeeds, second add fails
    g_pcapint_add_dev_results = { true, false };

    pcap_if_list_t devlist;
    char err_buf[256] = {0};

    int result = dbus_findalldevs(&devlist, err_buf);

    ASSERT_EQ(result, -1, "dbus_findalldevs should return -1 when second add_dev fails");
    // Only the first device should be recorded
    ASSERT_EQ((int)g_added_device_names.size(), 1, "Only the first device should be added when second add fails");
    if (g_added_device_names.size() == 1) {
        ASSERT_EQ(g_added_device_names[0], std::string("dbus-system"), "First added device should be dbus-system");
    }
}

// Entry point: runs the test suite and reports summary
int main() {
    // Run tests
    RUN_TEST(test_both_adds_succeed);
    RUN_TEST(test_first_add_fails);
    RUN_TEST(test_second_add_fails);

    // Summary
    if (g_failure_count == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failure_count << " test(s) FAILED" << std::endl;
        return 1;
    }
}