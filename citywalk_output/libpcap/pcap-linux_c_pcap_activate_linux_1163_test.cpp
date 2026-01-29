// A self-contained C++11 test harness for the focal C function: pcap_activate_linux
// The test harness provides minimal, controllable stubs for the dependencies
// used inside pcap_activate_linux so we can exercise multiple branches without
// requiring the full libpcap runtime.
//
// Notes:
// - This file compiles as a C++11 program but implements C interfaces where needed.
// - We do not rely on external testing frameworks; tests are invoked from main().
// - Each test case prints its result and includes comments explaining the intent.
// - The actual focal function is implemented in extern "C" to avoid C++ name mangling.

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
#include <cstdlib>
#include <pcap/can_socketcan.h>
#include <pcap-util.h>
#include <dirent.h>
#include <netlink/genl/genl.h>
#include <linux/netlink.h>
#include <fcntl.h>
#include <cstring>
#include <diag-control.h>
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


// -------------------------
// Domain constants & basic types
// -------------------------

#define PCAP_ERRBUF_SIZE 256
#define PCAP_ERROR -1
#define PCAP_ERROR_NO_SUCH_DEVICE -2
#define PCAP_WARNING_PROMISC_NOTSUP 1
#define MAXIMUM_SNAPLEN 262144

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef EFD_NONBLOCK
#define EFD_NONBLOCK 0x8000 // arbitrary nonzero flag for our test stub
#endif

// Forward declare for type compatibility
struct pcap_pkthdr;

// Typedef for the pcap_handler used by mmap read paths (we'll keep it opaque for testing)
typedef void (*pcap_handler)(void*, struct pcap_pkthdr*);

// -------------------------
// Minimal userland data structures to mirror the focal code requirements
// -------------------------

typedef struct pcap_linux {
    char* device;
    int timeout;
    int sysfs_dropped;
    int ifindex;
    int tp_version;
    int poll_breakloop_fd;
} pcap_linux;

typedef struct pcap_t {
    void* priv; // pointer to pcap_linux
    struct {
        const char* device;
        int promisc;
        int timeout;
        int nonblock;
    } opt;
    int snapshot;
    char errbuf[PCAP_ERRBUF_SIZE];
    int fd;

    // Activation-time function pointers (types kept generic for testing)
    void* inject_op;
    void* setfilter_op;
    void* setdirection_op;
    void* set_datalink_op;
    void* setnonblock_op;
    void* getnonblock_op;
    void* cleanup_op;
    void* stats_op;
    void* breakloop_op;
    void* read_op;
    int selectable_fd;
} pcap_t;

// We provide a minimal, opaque pcap_pkthdr to satisfy the declaration
struct pcap_pkthdr {
    int dummy;
};

// ---------------
// Prototypes for dependencies (to be provided by our test harness)
// ---------------
static int setup_socket(pcap_t *handle, int is_any_device);
static int linux_if_drops(const char *if_name);
static int eventfd(int initval, int flags);
static int iface_bind(int fd, int ifindex, char *ebuf, int protocol);
static int pcap_protocol(pcap_t *handle);
static void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t size, int errnum, const char *str);
static void pcap_cleanup_linux(pcap_t *handle);
static int setup_mmapped(pcap_t *handle);

// Integers to control behavior of stubs (configurable per test)
static int setup_socket_ret = 0;
static int linux_if_drops_ret = 0;
static int eventfd_ret = 3;
static int iface_bind_ret = 0;
static int setup_mmapped_ret = 0;
static int missing_strict_tp = 0; // not used, but reserved for future

// For test purposes: provide a simple, fake mmap/read function symbol
static void pcap_read_linux_mmap_v2(pcap_t *handle, int max_packets, pcap_handler callback, void *user) { (void)handle; (void)max_packets; (void)callback; (void)user; }

// ---------------
// Forward declaration of the focal function (the one under test)
// ---------------
extern "C" int pcap_activate_linux(pcap_t *handle);

// ---------------
// Implementations of the dependencies (stubs) that the focal function calls
// ---------------
static int setup_socket(pcap_t *handle, int is_any_device) {
    (void)is_any_device; // unused for deterministic testing in this harness
    return setup_socket_ret;
}

static int linux_if_drops(const char *if_name) {
    (void)if_name;
    return linux_if_drops_ret;
}

static int eventfd(int initval, int flags) {
    (void)initval; (void)flags;
    // Simulate failure when value is -1
    if (eventfd_ret == -1) return -1;
    return eventfd_ret;
}

static int iface_bind(int fd, int ifindex, char *ebuf, int protocol) {
    (void)fd; (void)ifindex; (void)ebuf; (void)protocol;
    return iface_bind_ret;
}

static int pcap_protocol(pcap_t *handle) {
    (void)handle;
    // Return a dummy protocol
    return 0;
}

static void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t size, int errnum, const char *str) {
    // Minimalistic error message helper; we clear the buffer to indicate quiet error.
    (void)errnum;
    (void)str;
    if (errbuf && size > 0) errbuf[0] = '\0';
}

static void pcap_cleanup_linux(pcap_t *handle) {
    // Free any allocated device string in priv struct
    if (handle && handle->priv) {
        pcap_linux *priv = (pcap_linux*)handle->priv;
        if (priv->device) {
            free(priv->device);
            priv->device = NULL;
        }
        free(priv);
        handle->priv = NULL;
    }
}

static int setup_mmapped(pcap_t *handle) {
    (void)handle;
    return setup_mmapped_ret;
}

// -------------------------
// The focal function (copied/adapted from the provided snippet)
// -------------------------

extern "C" int pcap_activate_linux(pcap_t *handle)
{
{
	struct pcap_linux *handlep = handle->priv;
	const char	*device;
	int		is_any_device;
	struct ifreq	ifr;
	int		status;
	int		ret;
	device = handle->opt.device;
	/*
	 * Start out assuming no warnings.
	 */
	status = 0;
	/*
	 * Make sure the name we were handed will fit into the ioctls we
	 * might perform on the device; if not, return a "No such device"
	 * indication, as the Linux kernel shouldn't support creating a device whose name won't fit into those ioctls.
	 *
	 * "Will fit" means "will fit, complete with a null terminator",
	 * so if the length, which does *not* include the null terminator,
	 * is greater than *or equal to* the size of the field into which
	 * we'll be copying it, that won't fit.
	 */
	if (strlen(device) >= sizeof(ifr.ifr_name)) {
		/*
		 * There's nothing more to say, so clear the error
		 * message.
		 */
		handle->errbuf[0] = '\0';
		status = PCAP_ERROR_NO_SUCH_DEVICE;
		goto fail;
	}
	/*
	 * Turn a negative snapshot value (invalid), a snapshot value of
	 * 0 (unspecified), or a value bigger than the normal maximum
	 * value, into the maximum allowed value.
	 *
	 * If some application really *needs* a bigger snapshot
	 * length, we should just increase MAXIMUM_SNAPLEN.
	 */
	if (handle->snapshot <= 0 || handle->snapshot > MAXIMUM_SNAPLEN)
		handle->snapshot = MAXIMUM_SNAPLEN;
	handlep->device	= strdup(device);
	if (handlep->device == NULL) {
		pcapint_fmt_errmsg_for_errno(handle->errbuf, PCAP_ERRBUF_SIZE,
		    errno, "strdup");
		status = PCAP_ERROR;
		goto fail;
	}
	/*
	 * The "any" device is a special device which causes us not
	 * to bind to a particular device and thus to look at all
	 * devices.
	 */
	is_any_device = (strcmp(device, "any") == 0);
	if (is_any_device) {
		if (handle->opt.promisc) {
			handle->opt.promisc = 0;
			/* Just a warning. */
			snprintf(handle->errbuf, PCAP_ERRBUF_SIZE,
			    "Promiscuous mode not supported on the \"any\" device");
			status = PCAP_WARNING_PROMISC_NOTSUP;
		}
	}
	/* copy timeout value */
	handlep->timeout = handle->opt.timeout;
	/*
	 * If we're in promiscuous mode, then we probably want
	 * to see when the interface drops packets too, so get an
	 * initial count from
	 * /sys/class/net/{if_name}/statistics/rx_{missed,fifo}_errors
	 */
	if (handle->opt.promisc)
		handlep->sysfs_dropped = linux_if_drops(handlep->device);
	/*
	 * If the "any" device is specified, try to open a SOCK_DGRAM.
	 * Otherwise, open a SOCK_RAW.
	 */
	ret = setup_socket(handle, is_any_device);
	if (ret < 0) {
		/*
		 * Fatal error; the return value is the error code,
		 * and handle->errbuf has been set to an appropriate
		 * error message.
		 */
		status = ret;
		goto fail;
	}
	if (ret > 0) {
		/*
		 * We got a warning; return that, as handle->errbuf
		 * might have been overwritten by this warning.
		 */
		status = ret;
	}
	/*
	 * Success (possibly with a warning).
	 *
	 * First, try to allocate an event FD for breakloop, if
	 * we're not going to start in non-blocking mode.
	 */
	if (!handle->opt.nonblock) {
		handlep->poll_breakloop_fd = eventfd(0, EFD_NONBLOCK);
		if (handlep->poll_breakloop_fd == -1) {
			/*
			 * Failed.
			 */
			pcapint_fmt_errmsg_for_errno(handle->errbuf,
			    PCAP_ERRBUF_SIZE, errno, "could not open eventfd");
			status = PCAP_ERROR;
			goto fail;
		}
	}
	/*
	 * Succeeded.
	 * Try to set up memory-mapped access.
	 */
	ret = setup_mmapped(handle);
	if (ret < 0) {
		/*
		 * We failed to set up to use it, or the
		 * kernel supports it, but we failed to
		 * enable it.  The return value is the
		 * error status to return and, if it's
		 * PCAP_ERROR, handle->errbuf contains
		 * the error message.
		 */
		status = ret;
		goto fail;
	}
	if (ret > 0) {
		/*
		 * We got a warning; return that, as handle->errbuf
		 * might have been overwritten by this warning.
		 */
		status = ret;
	}
	/*
	 * We succeeded.  status has been set to the status to return,
	 * which might be 0, or might be a PCAP_WARNING_ value.
	 */
	/*
	 * Now that we have activated the mmap ring, we can
	 * set the correct protocol.
	 */
	if ((ret = iface_bind(handle->fd, handlep->ifindex,
	    handle->errbuf, pcap_protocol(handle))) != 0) {
		status = ret;
		goto fail;
	}
	handle->inject_op = pcap_inject_linux;
	handle->setfilter_op = pcap_setfilter_linux;
	handle->setdirection_op = pcap_setdirection_linux;
	handle->set_datalink_op = pcap_set_datalink_linux;
	handle->setnonblock_op = pcap_setnonblock_linux;
	handle->getnonblock_op = pcap_getnonblock_linux;
	handle->cleanup_op = pcap_cleanup_linux;
	handle->stats_op = pcap_stats_linux;
	handle->breakloop_op = pcap_breakloop_linux;
	switch (handlep->tp_version) {
	case TPACKET_V2:
		handle->read_op = pcap_read_linux_mmap_v2;
		break;
#ifdef HAVE_TPACKET3
	case TPACKET_V3:
		handle->read_op = pcap_read_linux_mmap_v3;
		break;
#endif
	}
	handle->oneshot_callback = pcapint_oneshot_linux;
	handle->selectable_fd = handle->fd;
	return status;
fail:
	pcap_cleanup_linux(handle);
	return status;
}
pcap_set_datalink_linux(pcap_t *handle, int dlt)
{{};
linux_check_direction(const pcap_t *handle, const struct sockaddr_ll *sll)
{{};
device_still_exists(pcap_t *handle)
{{};
pcap_inject_linux(pcap_t *handle, const void *buf, int size)
{{};
pcap_stats_linux(pcap_t *handle, struct pcap_stat *stats)
{{};
// ... truncated rest of the original file for testing purposes ...
}
 // End of focal function

// -------------------------
// Test harness main
// -------------------------

// Helper: simple assertion with message
#define ASSERT(cond, msg) do { if (!(cond)) { \
    std::cerr << "ASSERTION FAILED: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } else { \
    std::cout << "PASS: " << (msg) << "\n"; \
    } \
} while (0)

static void reset_globals_for_test() {
    setup_socket_ret = 0;
    linux_if_drops_ret = 0;
    eventfd_ret = 3;
    iface_bind_ret = 0;
    setup_mmapped_ret = 0;
}

static pcap_t* build_handle(const char* device) {
    // Allocate handle and its private data
    pcap_t* handle = (pcap_t*)calloc(1, sizeof(pcap_t));
    if (!handle) return nullptr;
    handle->priv = calloc(1, sizeof(pcap_linux));
    if (!handle->priv) {
        free(handle);
        return nullptr;
    }
    ((pcap_linux*)handle->priv)->device = NULL;

    handle->opt.device = device;
    handle->opt.promisc = 0;
    handle->opt.timeout = 1000;
    handle->opt.nonblock = 0;
    handle->snapshot = MAXIMUM_SNAPLEN; // default to max
    // initialize other fields to avoid undefined behavior
    handle->fd = 42;
    return handle;
}

static void free_handle(pcap_t* handle) {
    if (!handle) return;
    if (handle->priv) {
        pcap_linux* priv = (pcap_linux*)handle->priv;
        if (priv->device) free(priv->device);
        free(priv);
    }
    free(handle);
}

// Test 1: device name too long triggers PCAP_ERROR_NO_SUCH_DEVICE and clears errbuf
static void test_long_device_name() {
    std::cout << "Test 1: Long device name triggers PCAP_ERROR_NO_SUCH_DEVICE\n";
    reset_globals_for_test();
    // Create a device string longer than ifr_name
    char longDev[32];
    for (int i = 0; i < 31; ++i) longDev[i] = 'a';
    longDev[31] = '\0';
    pcap_t* h = build_handle(longDev);
    if (!h) { std::cerr << "Failed to allocate handle\n"; return; }

    int status = pcap_activate_linux(h);

    std::cout << "Status: " << status << "\n";
    ASSERT(status == PCAP_ERROR_NO_SUCH_DEVICE, "Expected PCAP_ERROR_NO_SUCH_DEVICE");
    if (h && h->errbuf) {
        ASSERT(h->errbuf[0] == '\0', "errbuf should be cleared on NO_SUCH_DEVICE");
    } else {
        std::cout << "NOTE: No errbuf available to inspect\n";
    }
    free_handle(h);
}

// Test 2: snapshot normalization to MAXIMUM_SNAPLEN happens when 0
static void test_snapshot_normalization() {
    std::cout << "Test 2: Snapshot normalization to MAXIMUM_SNAPLEN when 0\n";
    reset_globals_for_test();
    pcap_t* h = build_handle("eth0");
    if (!h) { std::cerr << "Failed to allocate handle\n"; return; }

    h->snapshot = 0; // invalid, should be corrected
    int status = pcap_activate_linux(h);

    // Expect success (status 0) and snapshot corrected
    std::cout << "Status: " << status << "\n";
    if (h && h->priv) {
        pcap_linux* priv = (pcap_linux*)h->priv;
        std::cout << "snapshot after activation: " << h->snapshot << "\n";
        ASSERT(h->snapshot == MAXIMUM_SNAPLEN, "snapshot should be MAXIMUM_SNAPLEN");
        ASSERT(priv->device != NULL && strcmp(priv->device, "eth0") == 0, "device copied to priv struct");
    } else {
        std::cout << "Test setup failed: missing priv\n";
    }
    free_handle(h);
}

// Test 3: any-device with promisc set yields a warning and promisc is cleared
static void test_any_device_promisc_warning() {
    std::cout << "Test 3: 'any' device with promisc yields warning and clears promisc\n";
    reset_globals_for_test();
    pcap_t* h = build_handle("any");
    if (!h) { std::cerr << "Failed to allocate handle\n"; return; }

    h->opt.promisc = 1;
    int status = pcap_activate_linux(h);

    std::cout << "Status: " << status << "\n";
    ASSERT(status == PCAP_WARNING_PROMISC_NOTSUP, "Expected PCAP_WARNING_PROMISC_NOTSUP");
    if (h) {
        // promisc should be cleared
        ASSERT(h->opt.promisc == 0, "Promiscuous mode should be disabled for 'any' device");
        // errbuf should contain the warning message
        if (strlen(h->errbuf) > 0) {
            // Basic content check
            const char* expected = "Promiscuous mode not supported on the \"any\" device";
            if (strstr(h->errbuf, "Promiscuous mode not supported") == NULL) {
                std::cout << "Warning text not exactly as expected. errbuf: " << h->errbuf << "\n";
            } else {
                std::cout << "Warning text present as expected.\n";
            }
        } else {
            std::cout << "Warning errbuf is empty unexpectedly.\n";
        }
    }
    free_handle(h);
}

// Test 4: setup_socket returns negative => fail with that status
static void test_setup_socket_negative() {
    std::cout << "Test 4: setup_socket returns negative => propagation as failure\n";
    reset_globals_for_test();
    setup_socket_ret = -1;
    pcap_t* h = build_handle("eth0");
    if (!h) { std::cerr << "Failed to allocate handle\n"; return; }

    int status = pcap_activate_linux(h);

    std::cout << "Status: " << status << "\n";
    ASSERT(status == -1, "Expected negative status propagated from setup_socket");
    free_handle(h);
}

// Test 5: eventfd failure while not in nonblock mode
static void test_eventfd_failure() {
    std::cout << "Test 5: eventfd failure when not in nonblock mode\n";
    reset_globals_for_test();
    eventfd_ret = -1;
    pcap_t* h = build_handle("eth0");
    if (!h) { std::cerr << "Failed to allocate handle\n"; return; }

    int status = pcap_activate_linux(h);

    std::cout << "Status: " << status << "\n";
    ASSERT(status == PCAP_ERROR, "Expected PCAP_ERROR due to eventfd failure");
    free_handle(h);
}

// Test 6: setup_mmapped failure
static void test_setup_mmapped_failure() {
    std::cout << "Test 6: setup_mmapped returns negative => failure\n";
    reset_globals_for_test();
    setup_mmapped_ret = -1;
    pcap_t* h = build_handle("eth0");
    if (!h) { std::cerr << "Failed to allocate handle\n"; return; }

    int status = pcap_activate_linux(h);
    std::cout << "Status: " << status << "\n";
    ASSERT(status == -1, "Expected -1 from setup_mmapped failure path");
    free_handle(h);
}

// Test 7: iface_bind failure
static void test_iface_bind_failure() {
    std::cout << "Test 7: iface_bind failure propagates status\n";
    reset_globals_for_test();
    iface_bind_ret = 7; // non-zero -> failure
    pcap_t* h = build_handle("eth0");
    if (!h) { std::cerr << "Failed to allocate handle\n"; return; }

    int status = pcap_activate_linux(h);
    std::cout << "Status: " << status << "\n";
    ASSERT(status == 7, "Expected iface_bind to return non-zero status propagated");
    free_handle(h);
}

// Test 8: success path ensures function pointers are set, and read_op is mapped
static void test_success_path_read_op_mapping() {
    std::cout << "Test 8: success path maps read_op to pcap_read_linux_mmap_v2 for tp_version=TPACKET_V2\n";
    reset_globals_for_test();
    pcap_t* h = build_handle("eth0");
    if (!h) { std::cerr << "Failed to allocate handle\n"; return; }

    // Ensure normal path continues
    setup_socket_ret = 0;
    linux_if_drops_ret = 0;
    eventfd_ret = 3;
    iface_bind_ret = 0;
    setup_mmapped_ret = 0;

    // Provide tp_version in the private struct
    if (h->priv) {
        ((pcap_linux*)h->priv)->tp_version = 2; // TPACKET_V2
        // Also give an ifindex for iface_bind usage in test (not used by stub)
        ((pcap_linux*)h->priv)->ifindex = 1;
    }

    int status = pcap_activate_linux(h);
    std::cout << "Status: " << status << "\n";
    ASSERT(status == 0, "Activation should succeed");
    if (h->read_op) {
        void* expected = (void*)pcap_read_linux_mmap_v2;
        // Compare as raw pointers (cast to void*)
for (size_t i = 0; i < 1; ++i) { /* no-op loop to keep structure similar to multi-path checks */ }
        if (h->read_op == expected) {
            std::cout << "read_op correctly mapped to pcap_read_linux_mmap_v2\n";
        } else {
            std::cout << "read_op was not mapped to pcap_read_linux_mmap_v2\n";
        }
    } else {
        std::cout << "read_op is null; expected mapping to pcap_read_linux_mmap_v2\n";
    }

    free_handle(h);
}

// ---------------
// Main: run tests
// ---------------
int main() {
    std::cout << "Starting pcap_activate_linux focused tests (C++11 harness)\n";

    test_long_device_name();
    std::cout << "-------------------------------------\n";

    test_snapshot_normalization();
    std::cout << "-------------------------------------\n";

    test_any_device_promisc_warning();
    std::cout << "-------------------------------------\n";

    test_setup_socket_negative();
    std::cout << "-------------------------------------\n";

    test_eventfd_failure();
    std::cout << "-------------------------------------\n";

    test_setup_mmapped_failure();
    std::cout << "-------------------------------------\n";

    test_iface_bind_failure();
    std::cout << "-------------------------------------\n";

    test_success_path_read_op_mapping();
    std::cout << "-------------------------------------\n";

    std::cout << "Tests completed.\n";
    return 0;
}