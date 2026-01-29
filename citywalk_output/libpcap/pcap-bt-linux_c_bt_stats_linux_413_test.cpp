/*
Unit test suite for bt_stats_linux in pcap-bt-linux.c

Assumptions and build notes (to be adapted by the user environment):
- Build with C++11, no GoogleTest. A simple main-based test runner is used.
- The test uses a linker wrap of the ioctl symbol to intercept calls to ioctl
  via GCC/ld flag: -Wl,--wrap=ioctl
- The test provides a minimal user-space fake pcap_t and related types to
  exercise the focal function bt_stats_linux. It is intended to be used in a
  project where the real types (pcap_t, pcap_stat, pcap_bt, etc.) are defined
  by the project headers. If your environment uses the project-provided headers,
  include them instead of the local stubs below and remove the local stub types.
- The test focuses on the logic inside bt_stats_linux:
  a) successful population of ps_recv, ps_drop, ps_ifdrop
  b) handling of EINTR retry loop
  c) handling of a non-EINTR ioctl failure
- When compiled with -Wl,--wrap=ioctl, the wrapper __wrap_ioctl will shadow all
  calls to ioctl in the object under test.

To build (example, adapt to your environment):
g++ -std=c++11 test_bt_stats_linux.cpp -o test_bt_stats_linux \
    -Wl,--wrap=ioctl -lpcap

If you have project headers for pcap-bt-linux, include them instead of the
local stubs below.

This file contains the actual tests and explanatory comments for each case.
*/

#include <pcap/bluetooth.h>
#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <errno.h>
#include <cassert>
#include <cerrno>
#include <fcntl.h>
#include <diag-control.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <pcap-int.h>
#include <string.h>
#include <pcap-bt-linux.h>
#include <config.h>
#include <cstring>


// Provide a size for PCAP_ERRBUF_SIZE if not available from headers.
#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif

// Minimal stubbed types (replace with real project headers if available)
struct pcap_bt {
    int dev_id;
};

struct pcap_stat {
    unsigned int ps_recv;
    unsigned int ps_drop;
    unsigned int ps_ifdrop;
};

// Opaque pcap_t shape for test. In the real project this is defined in the project headers.
// We provide just enough structure (priv, fd, errbuf) for bt_stats_linux to access.
typedef struct pcap_t {
    void *priv;
    int fd;
    char errbuf[PCAP_ERRBUF_SIZE];
} pcap_t;

// Prototype of the focal function (as provided by the project, with C linkage)
extern "C" int bt_stats_linux(pcap_t *handle, struct pcap_stat *stats);

// Forward declaration of a Bluetooth dev-info structure from <bluetooth/hci.h>
// We re-declare minimal fields here for the test to interact with our ioctl mock.
extern "C" {
struct hci_dev_info;
struct hci_dev_stats {
    unsigned int evt_rx;
    unsigned int acl_rx;
    unsigned int sco_rx;
    unsigned int cmd_tx;
    unsigned int acl_tx;
    unsigned int sco_tx;
    unsigned int err_rx;
    unsigned int err_tx;
};
struct hci_dev_info {
    int dev_id;
    struct hci_dev_stats stat;
};
};

static IoctlMode g_ioctl_mode = IOCTL_OK; // default mode

// Enum for ioctl behavior control in tests
enum IoctlMode {
    IOCTL_OK,            // succeed on HCIGETDEVINFO
    IOCTL_EINTR_ONCE,      // first EINTR, then succeed
    IOCTL_EINTR_REPEAT,    // first EINTR, then EINTR again (simulate persistent EINTR)
    IOCTL_FAIL              // fail with non-EINTR errno
};

// C linkage wrapper to override ioctl calls during tests
extern "C" int __wrap_ioctl(int fd, unsigned long request, void *argp)
{
    // Only intercept HCIGETDEVINFO requests; otherwise, return success
    // GetHCIGETDEVINFO is defined in <bluetooth/hci.h> typically as a numeric constant.
    // We assume the environment provides this symbol. If not, set a fallback value.
    const unsigned long HCIGETDEVINFO = /* platform-specific; if not defined, provide a fallback */ 0x80000000;

    if (request != HCIGETDEVINFO) {
        // Not our target request; return success to avoid affecting other calls
        return 0;
    }

    struct hci_dev_info *dev_info = static_cast<struct hci_dev_info *>(argp);

    // Behavior control
    switch (g_ioctl_mode) {
        case IOCTL_OK:
            // Fill in device info with deterministic values
            dev_info->dev_id = 0;
            dev_info->stat.evt_rx = 1;
            dev_info->stat.acl_rx = 2;
            dev_info->stat.sco_rx = 3;
            dev_info->stat.cmd_tx = 4;
            dev_info->stat.acl_tx = 5;
            dev_info->stat.sco_tx = 6;
            dev_info->stat.err_rx = 7;
            dev_info->stat.err_tx = 8;
            return 0;

        case IOCTL_EINTR_ONCE: {
            // Implement EINTR once, then succeed on the second call
            static bool did_intr = false;
            if (!did_intr) {
                did_intr = true;
                errno = EINTR;
                return -1;
            }
            // On second call, succeed with the same data as IOCTL_OK
            dev_info->dev_id = 0;
            dev_info->stat.evt_rx = 1;
            dev_info->stat.acl_rx = 2;
            dev_info->stat.sco_rx = 3;
            dev_info->stat.cmd_tx = 4;
            dev_info->stat.acl_tx = 5;
            dev_info->stat.sco_tx = 6;
            dev_info->stat.err_rx = 7;
            dev_info->stat.err_tx = 8;
            return 0;
        }

        case IOCTL_EINTR_REPEAT: {
            // Always report EINTR to simulate a failing ioctl
            errno = EINTR;
            return -1;
        }

        case IOCTL_FAIL:
        default:
            // Non-EINTR failure
            errno = EIO;
            return -1;
    }
}

// Helpers to construct a fake pcap_t handle used by bt_stats_linux
static pcap_t* create_fake_handle(int dev_id)
{
    // Create a fake handle with the minimal fields BT_STATS expects
    pcap_t *h = reinterpret_cast<pcap_t*>(malloc(sizeof(pcap_t)));
    if (!h) return nullptr;
    h->priv = malloc(sizeof(struct pcap_bt));
    if (!h->priv) {
        free(h);
        return nullptr;
    }
    static_cast<struct pcap_bt*>(h->priv)->dev_id = dev_id;
    h->fd = 0; // value unused by our wrapper
    std::memset(h->errbuf, 0, PCAP_ERRBUF_SIZE);
    return h;
}

static void destroy_fake_handle(pcap_t *h)
{
    if (!h) return;
    free(h->priv);
    free(h);
}

// Test 1: Successful stats retrieval
bool test_success_case() {
    std::cout << "TEST 1: bt_stats_linux success path\n";

    // Configure behavior
    g_ioctl_mode = IOCTL_OK;

    // Create a fake handle
    pcap_t *handle = create_fake_handle(42);
    if (!handle) {
        std::cerr << "Failed to create fake handle\n";
        return false;
    }

    // Call the focal function
    struct pcap_stat stats;
    std::memset(&stats, 0, sizeof(stats));
    int ret = bt_stats_linux(handle, &stats);

    // Expected sums from the wrapper:
    // ps_recv = evt_rx + acl_rx + sco_rx + cmd_tx + acl_tx + sco_tx
    // = 1 + 2 + 3 + 4 + 5 + 6 = 21
    // ps_drop = err_rx + err_tx = 7 + 8 = 15
    const unsigned int expected_recv = 21;
    const unsigned int expected_drop = 15;
    bool ok = true;

    if (ret != 0) {
        std::cerr << "BT_STATS_LINUX returned non-zero: " << ret << "\n";
        ok = false;
    }
    if (stats.ps_recv != expected_recv) {
        std::cerr << "ps_recv mismatch: got " << stats.ps_recv
                  << " expected " << expected_recv << "\n";
        ok = false;
    }
    if (stats.ps_drop != expected_drop) {
        std::cerr << "ps_drop mismatch: got " << stats.ps_drop
                  << " expected " << expected_drop << "\n";
        ok = false;
    }
    if (stats.ps_ifdrop != 0) {
        std::cerr << "ps_ifdrop non-zero: got " << stats.ps_ifdrop << "\n";
        ok = false;
    }

    destroy_fake_handle(handle);
    return ok;
}

// Test 2: EINTR path is handled and retried
bool test_eintr_once_path() {
    std::cout << "TEST 2: bt_stats_linux EINTR retry path (once)\n";

    // Configure behavior: first EINTR, then success
    g_ioctl_mode = IOCTL_EINTR_ONCE;

    // Create a fake handle
    pcap_t *handle = create_fake_handle(43);
    if (!handle) {
        std::cerr << "Failed to create fake handle\n";
        return false;
    }

    struct pcap_stat stats;
    std::memset(&stats, 0, sizeof(stats));

    int ret = bt_stats_linux(handle, &stats);
    // After EINTR once, the second call should succeed with the same values as IOCTL_OK
    const unsigned int expected_recv = 21; // 1+2+3+4+5+6
    const unsigned int expected_drop = 15; // 7+8

    bool ok = true;
    if (ret != 0) {
        std::cerr << "BT_STATS_LINUX did not retry after EINTR as expected; ret=" << ret << "\n";
        ok = false;
    }
    if (stats.ps_recv != expected_recv) {
        std::cerr << "ps_recv mismatch after EINTR retry: got " << stats.ps_recv
                  << " expected " << expected_recv << "\n";
        ok = false;
    }
    if (stats.ps_drop != expected_drop) {
        std::cerr << "ps_drop mismatch after EINTR retry: got " << stats.ps_drop
                  << " expected " << expected_drop << "\n";
        ok = false;
    }
    if (stats.ps_ifdrop != 0) {
        std::cerr << "ps_ifdrop non-zero after EINTR retry: got " << stats.ps_ifdrop << "\n";
        ok = false;
    }

    destroy_fake_handle(handle);
    return ok;
}

// Test 3: Non-EINTR failure path
bool test_failure_path() {
    std::cout << "TEST 3: bt_stats_linux non-EINTR failure path\n";

    // Configure behavior: immediate failure
    g_ioctl_mode = IOCTL_FAIL;

    // Create a fake handle
    pcap_t *handle = create_fake_handle(44);
    if (!handle) {
        std::cerr << "Failed to create fake handle\n";
        return false;
    }

    struct pcap_stat stats;
    std::memset(&stats, 0, sizeof(stats));

    int ret = bt_stats_linux(handle, &stats);

    bool ok = true;
    if (ret >= 0) {
        std::cerr << "Expected failure (-1) but got ret=" << ret << "\n";
        ok = false;
    }

    destroy_fake_handle(handle);
    return ok;
}

int main() {
    // Simple, non-terminating test runner
    int failures = 0;

    if (!test_success_case()) {
        std::cerr << "Test 1 FAILED\n";
        ++failures;
    } else {
        std::cout << "Test 1 PASSED\n";
    }

    if (!test_eintr_once_path()) {
        std::cerr << "Test 2 FAILED\n";
        ++failures;
    } else {
        std::cout << "Test 2 PASSED\n";
    }

    if (!test_failure_path()) {
        std::cerr << "Test 3 FAILED\n";
        ++failures;
    } else {
        std::cout << "Test 3 PASSED\n";
    }

    if (failures > 0) {
        std::cout << "OVERALL RESULT: " << failures << " test(s) failed.\n";
        return 1;
    } else {
        std::cout << "OVERALL RESULT: ALL TESTS PASSED.\n";
        return 0;
    }
}

/*
Notes for integration:
- The test uses a simple manual test harness and a wrapper around ioctl to simulate
  the kernel behavior. It relies on the bt_stats_linux implementation to access
  members of pcap_t and the nested hci_dev_info structure.
- If your environment provides the project headers for pcap-bt-linux (pcap-int.h,
  pcap/bt-linux or similar), prefer including those headers and removing the local
  stub definitions for pcap_t and pcap_bt above.
- Build the tests with proper include paths for Bluetooth headers and PCAP headers.
- The -Wl,--wrap=ioctl linker option is required for intercepting ioctl in bt_stats_linux.
  Example: g++ -std=c++11 test_bt_stats_linux.cpp -o test_bt_stats_linux -Wl,--wrap=ioctl -lpcap
*/