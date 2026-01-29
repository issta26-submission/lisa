// C++11 unit tests for the focal method bt_create in pcap-bt-linux.c
// - No GTest, a lightweight test harness with explicit checks.
// - Tests exercise true/false branches of the predicate logic in bt_create.
// - Uses forward declarations to avoid requiring full pcap types in tests.
// - Notes: This test relies on the BT_IFACE macro defined in the production headers.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <pcap-bt-linux.h>
#include <sys/socket.h>
#include <iostream>
#include <pcap/bluetooth.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <config.h>
#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <pcap-int.h>
#include <cstring>
#include <string>
#include <fcntl.h>
#include <sys/ioctl.h>


// Forward declare the C types/functions to avoid depending on full pcap headers here.
extern "C" {
    struct pcap_t; // forward declaration for the opaque pcap_t type
    // Prototype of the focal function under test (C linkage)
    struct pcap_t* bt_create(const char* device, char* ebuf, int* is_ours);
}

// If the build environment provides BT_IFACE via production headers, include them
// to resolve the macro value for test string construction.
// We guard the include to avoid compilation errors if the header path differs.
#ifdef BT_IFACE
// Intentionally include the header that defines BT_IFACE if available.
// This ensures our test strings can be built using the real interface prefix.
extern "C" {
}
#endif

// Basic test harness utilities
static int g_pass = 0;
static int g_fail = 0;

template <typename T>
static void expect_eq(const T& a, const T& b, const char* msg) {
    if (a == b) {
        ++g_pass;
    } else {
        ++g_fail;
        std::cerr << "FAIL: " << msg << " | Expected: " << b << ", Actual: " << a << std::endl;
    }
}

// Test 1: device string does not start with BT_IFACE prefix -> bt_create should return NULL and is_ours = 0
static bool test_not_bt_iface() {
    char ebuf[256];
    int is_ours = -1;
    // Use a device string that almost certainly does not begin with BT_IFACE
    const char* bad_device = "not_bt_interface";
    struct pcap_t* ret = bt_create(bad_device, ebuf, &is_ours);

    bool ok = (ret == nullptr) && (is_ours == 0);
    if (!ok) {
        std::cerr << "Test not_bt_iface failed: ret=" << ret << ", is_ours=" << is_ours << std::endl;
    }
    return ok;
}

// Test 2: device string starts with BT_IFACE followed by a valid number (e.g., BT_IFACE + '0')
// bt_create should return non-NULL and set is_ours = 1
static bool test_bt_iface_with_number() {
    char ebuf[256];
    int is_ours = -1;

    // Build a device string: BT_IFACE + "0"
    // BT_IFACE is expected to be a string macro (e.g., "bluetooth")
    std::string prefix;
#ifdef BT_IFACE
    prefix = BT_IFACE;
#else
    // Fallback: if BT_IFACE is not defined in the test environment, gracefully skip
    // this test with a safe path that doesn't depend on BT_IFACE value.
    prefix = "bluetooth"; // common default used in many environments
#endif
    std::string dev_str = prefix + "0";

    struct pcap_t* ret = bt_create(dev_str.c_str(), ebuf, &is_ours);

    bool ok = (ret != nullptr) && (is_ours == 1);
    if (!ok) {
        std::cerr << "Test bt_iface_with_number failed: dev_str=" << dev_str
                  << ", ret=" << ret << ", is_ours=" << is_ours << std::endl;
    }
    return ok;
}

// Test 3: device string starts with BT_IFACE followed by a negative number (e.g., BT_IFACE + "-1")
// Expect ret == NULL and is_ours == 0
static bool test_bt_iface_negative_number() {
    char ebuf[256];
    int is_ours = -1;

    std::string prefix;
#ifdef BT_IFACE
    prefix = BT_IFACE;
#else
    prefix = "bluetooth";
#endif
    std::string dev_str = prefix + "-1";

    struct pcap_t* ret = bt_create(dev_str.c_str(), ebuf, &is_ours);

    bool ok = (ret == nullptr) && (is_ours == 0);
    if (!ok) {
        std::cerr << "Test bt_iface_negative_number failed: dev_str=" << dev_str
                  << ", ret=" << ret << ", is_ours=" << is_ours << std::endl;
    }
    return ok;
}

// Test 4: device string starts with BT_IFACE followed by a non-numeric non-empty suffix (e.g., BT_IFACE + 'x')
// Expect ret == NULL and is_ours == 0
static bool test_bt_iface_not_followed_by_number() {
    char ebuf[256];
    int is_ours = -1;

    std::string prefix;
#ifdef BT_IFACE
    prefix = BT_IFACE;
#else
    prefix = "bluetooth";
#endif
    std::string dev_str = prefix + "x";

    struct pcap_t* ret = bt_create(dev_str.c_str(), ebuf, &is_ours);

    bool ok = (ret == nullptr) && (is_ours == 0);
    if (!ok) {
        std::cerr << "Test bt_iface_not_followed_by_number failed: dev_str=" << dev_str
                  << ", ret=" << ret << ", is_ours=" << is_ours << std::endl;
    }
    return ok;
}

// Main driver: run all tests and report summary
int main() {
    std::cout << "Starting bt_create unit tests (C++11)..." << std::endl;

    if (test_not_bt_iface()) {
        ++g_pass;
        std::cout << "PASS: test_not_bt_iface" << std::endl;
    } else {
        ++g_fail;
        std::cerr << "FAIL: test_not_bt_iface" << std::endl;
    }

    if (test_bt_iface_with_number()) {
        ++g_pass;
        std::cout << "PASS: test_bt_iface_with_number" << std::endl;
    } else {
        ++g_fail;
        std::cerr << "FAIL: test_bt_iface_with_number" << std::endl;
    }

    if (test_bt_iface_negative_number()) {
        ++g_pass;
        std::cout << "PASS: test_bt_iface_negative_number" << std::endl;
    } else {
        ++g_fail;
        std::cerr << "FAIL: test_bt_iface_negative_number" << std::endl;
    }

    if (test_bt_iface_not_followed_by_number()) {
        ++g_pass;
        std::cout << "PASS: test_bt_iface_not_followed_by_number" << std::endl;
    } else {
        ++g_fail;
        std::cerr << "FAIL: test_bt_iface_not_followed_by_number" << std::endl;
    }

    std::cout << "Tests completed. Passed: " << g_pass << ", Failed: " << g_fail << std::endl;

    // Return non-zero if any test failed to signal failure to the runner.
    return (g_fail == 0) ? 0 : 1;
}