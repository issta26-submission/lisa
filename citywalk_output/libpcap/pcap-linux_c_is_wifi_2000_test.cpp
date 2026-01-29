// Minimal C++11 test harness for the C function is_wifi from pcap-linux.c
// This test does not depend on GTest and uses a small, non-terminating assertion style.

#include <netinet/in.h>
#include <pcap/vlan.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/if_ether.h>
#include <netlink/msg.h>
#include <iostream>
#include <sys/ioctl.h>
#include <errno.h>
#include <pcap-snf.h>
#include <pcap/can_socketcan.h>
#include <pcap-util.h>
#include <dirent.h>
#include <netlink/genl/genl.h>
#include <linux/netlink.h>
#include <fcntl.h>
#include <string>
#include <diag-control.h>
#include <vector>
#include <sys/socket.h>
#include <sys/stat.h>
#include <linux/sockios.h>
#include <endian.h>
#include <linux/net_tstamp.h>
#include <linux/filter.h>
#include <poll.h>
#include <netlink/genl/family.h>
#include <limits.h>
#include <stdlib.h>
#include <linux/nl80211.h>
#include <pcap-int.h>
#include <netlink/attr.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/eventfd.h>
#include <linux/types.h>
#include <linux/if_arp.h>
#include <linux/ethtool.h>
#include <netlink/genl/ctrl.h>
#include <linux/if_bonding.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>
#include <sys/utsname.h>


// Declare the C function under test
extern "C" int is_wifi(const char *device);

// Simple in-file test result container
struct TestResult {
    std::string name;
    bool passed;
    std::string details;
};

// Global test results container
static std::vector<TestResult> g_results;

// Record a test result (non-terminating; continues execution)
static void record_result(const std::string& name, bool passed, const std::string& details = "") {
    g_results.push_back({name, passed, details});
}

// Report all test results to stdout
static void report_results() {
    for (const auto& r : g_results) {
        std::cout << (r.passed ? "[PASS] " : "[FAIL] ") << r.name;
        if (!r.details.empty()) {
            std::cout << " - " << r.details;
        }
        std::cout << std::endl;
    }
    // Summary
    int passed = 0;
    for (const auto& r : g_results) if (r.passed) ++passed;
    int total = (int)g_results.size();
    int failed = total - passed;
    std::cout << "Summary: " << passed << " passed, " << failed << " failed, " << total << " total tests.\n";
}

// Test 1: False branch of is_wifi
// Description: Ensure is_wifi returns 0 when the /sys/class/net/<device>/wireless path cannot be created
// This branch is exercised by providing a device name unlikely to exist in sysfs.
static void test_is_wifi_false_branch() {
    // Device name chosen to have extremely low probability of existing in /sys/class/net
    const char* device = "this_device_should_not_exist_for_wifi_test_abcdef12345";

    int result = is_wifi(device);
    bool pass = (result == 0);
    if (!pass) {
        std::string details = "Expected 0, got 1 for device " + std::string(device);
        record_result("test_is_wifi_false_branch", false, details);
    } else {
        record_result("test_is_wifi_false_branch", true);
    }
}

// Test 2: True branch of is_wifi (discovered at runtime)
// Description: Try to locate any wireless interface by scanning /sys/class/net for a device
// that has a wireless subdirectory. If found, verify is_wifi reports 1 for that device.
// If none are found or accessible, skip with a descriptive message (non-fatal).
static void test_is_wifi_true_branch() {
    const char* base = "/sys/class/net";
    DIR* dir = opendir(base);
    if (dir == nullptr) {
        record_result("test_is_wifi_true_branch", true, "skipped: cannot access /sys/class/net");
        return;
    }

    struct dirent* entry;
    bool found = false;
    std::string chosen_dev;

    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type != DT_DIR) continue;
        std::string devname = entry->d_name;
        if (devname == "." || devname == "..") continue;

        std::string wireless_path = std::string(base) + "/" + devname + "/wireless";
        struct stat st;
        if (stat(wireless_path.c_str(), &st) == 0) {
            found = true;
            chosen_dev = devname;
            break;
        }
    }
    closedir(dir);

    if (!found) {
        record_result("test_is_wifi_true_branch", true, "skipped: no wireless interface found in /sys/class/net");
        return;
    }

    int result = is_wifi(chosen_dev.c_str());
    bool pass = (result != 0);
    if (!pass) {
        std::string details = "Expected 1, got " + std::to_string(result) +
                              " for device " + chosen_dev;
        record_result("test_is_wifi_true_branch", false, details);
    } else {
        record_result("test_is_wifi_true_branch", true);
    }
}

// Entry point: run both tests and report
int main() {
    // Run tests
    test_is_wifi_false_branch();
    test_is_wifi_true_branch();

    // Output results
    report_results();

    // Return non-zero if any test failed
    int failed = 0;
    for (const auto& r : g_results) if (!r.passed) ++failed;
    return failed ? 1 : 0;
}