// Unit test suite for pcap_activate_hurd (focal method) using a handcrafted harness.
// This test harness is designed for C++11 compilation without GTest and uses
// non-terminating checks to maximize coverage across code branches.

#include <device/net_status.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <config.h>
#include <time.h>
#include <stddef.h>
#include <cstdio>
#include <hurd.h>
#include <device/device.h>
#include <mach.h>
#include <fcntl.h>
#include <net/if_ether.h>
#include <stdlib.h>
#include <device/device_types.h>
#include <pcap-int.h>
#include <string.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>


// Domain-specific constants (simplified for testing)
#define MACH_PORT_NULL 0
#define O_READ 1
#define O_WRITE 2
#define D_READ 4
#define D_WRITE 8
#define ED_NO_SUCH_DEVICE 2
#define EPERM 1

#define PCAP_ERROR (-1)
#define PCAP_ERROR_PERM_DENIED (-2)
#define PCAP_ERROR_NO_SUCH_DEVICE (-3)
#define PCAP_ERRBUF_SIZE 256
#define MAXIMUM_SNAPLEN 65535
#define DLT_EN10MB 1
#define sizeof_net_rcv_msg sizeof(struct net_rcv_msg)

// Forward declarations to mirror the real code surface used by pcap_activate_hurd
struct net_rcv_msg { int dummy; };

typedef int mach_port_t;
typedef int kern_return_t;

// Forward declaration: pcap_t and pcap_hurd (minimal subset for tests)
struct pcap_hurd {
    int mach_dev;
    int rcv_port;
};
struct pcap_t;

// Function pointer types used by pcap_activate_hurd (simplified)
typedef int (*pcap_read_op_t)(pcap_t*, int, void*, unsigned char*);
typedef int (*pcap_inject_op_t)(pcap_t*, const void*, int);
typedef int (*pcap_setfilter_op_t)(pcap_t*, void*);
typedef int (*pcap_stats_op_t)(pcap_t*, void*);

struct pcap_t {
    int snapshot;
    struct { const char* device; } opt;
    pcap_hurd* priv;

    char errbuf[PCAP_ERRBUF_SIZE];
    int bufsize;
    unsigned char* buffer;

    int linktype;

    pcap_read_op_t read_op;
    pcap_inject_op_t inject_op;
    pcap_setfilter_op_t setfilter_op;
    pcap_stats_op_t stats_op;
};

// Prototypes for the focal function and its cleanup function (to be linked in test)
extern "C" int pcap_activate_hurd(pcap_t* p);
extern "C" void pcap_cleanup_hurd(pcap_t* p);

// Forward declarations of stubs (to be implemented via TestHarness)
extern "C" mach_port_t file_name_lookup(const char* device, int flags, int mode);
extern "C" kern_return_t get_privileged_ports(void* a, mach_port_t* master);
extern "C" int device_open(mach_port_t master, int flags, const char* device, int* out);
extern "C" void mach_port_deallocate(mach_port_t task, mach_port_t name);
extern "C" kern_return_t mach_port_allocate(mach_port_t task, int right, mach_port_t* name);

// We'll override malloc to simulate allocation failure when requested.
static struct net_rcv_msg { int dummy; };

// Test harness that controls stub behavior via static members
class TestHarness {
public:
    // Control values for stubs
    static mach_port_t file_name_lookup_result;
    static int get_priv_kr;
    static int device_open_kr_master_non_null;
    static int device_open_kr_master_null;
    static int mach_port_allocate_kr;
    static bool malloc_should_fail;

    // Simple known constants (used by some branches)
    static constexpr int EPERM_const = EPERM;
    static constexpr int ED_NO_SUCH_DEVICE_const = ED_NO_SUCH_DEVICE;
    static constexpr int PCAP_ERROR_const = PCAP_ERROR;

    static void reset() {
        file_name_lookup_result = 0;
        get_priv_kr = 0;
        device_open_kr_master_non_null = 0;
        device_open_kr_master_null = 0;
        mach_port_allocate_kr = 0;
        malloc_should_fail = false;
        // Ensure errbuf is clean for each test
    }
};

mach_port_t TestHarness::file_name_lookup_result = 0;
int TestHarness::get_priv_kr = 0;
int TestHarness::device_open_kr_master_non_null = 0;
int TestHarness::device_open_kr_master_null = 0;
int TestHarness::mach_port_allocate_kr = 0;
bool TestHarness::malloc_should_fail = false;

// Stub implementations (C linkage) driven by TestHarness state
extern "C" mach_port_t file_name_lookup(const char* device, int flags, int mode) {
    (void)device; (void)flags; (void)mode;
    return TestHarness::file_name_lookup_result;
}

extern "C" kern_return_t get_privileged_ports(void* a, mach_port_t* master) {
    (void)a;
    if (TestHarness::get_priv_kr != 0) {
        *master = 0;
        return TestHarness::get_priv_kr; // non-zero => error
    }
    *master = 1; // non-zero to indicate a valid master port
    return 0;
}

extern "C" int device_open(mach_port_t master, int flags, const char* device, int* out) {
    (void)flags; (void)device;
    if (master != MACH_PORT_NULL) {
        *out = 1;
        return TestHarness::device_open_kr_master_non_null;
    } else {
        *out = 1;
        return TestHarness::device_open_kr_master_null;
    }
}

extern "C" void mach_port_deallocate(mach_port_t task, mach_port_t name) {
    (void)task; (void)name;
}

extern "C" kern_return_t mach_port_allocate(mach_port_t task, int right, mach_port_t* name) {
    (void)task; (void)right;
    if (TestHarness::mach_port_allocate_kr != 0) {
        return TestHarness::mach_port_allocate_kr;
    }
    *name = 0xDEADC0DE;
    return 0;
}

// Provide malloc override so tests can simulate allocation failure
static void* test_malloc(size_t size) {
    if (TestHarness::malloc_should_fail) return NULL;
    return std::malloc(size);
}
#undef malloc
#define malloc test_malloc

// Necessary function pointer stubs for pcap_activate_hurd (they are not used in tests)
int pcap_read_hurd(pcap_t* p, int cnt, void* cb, unsigned char* user) { (void)p; (void)cnt; (void)cb; (void)user; return 0; }
int pcap_inject_hurd(pcap_t* p, const void* buf, int size) { (void)p; (void)buf; (void)size; return 0; }
int pcap_setfilter_hurd(pcap_t* p, void* program) { (void)p; (void)program; return 0; }
int pcap_stats_hurd(pcap_t* p, void* ps) { (void)p; (void)ps; return 0; }

// Minimal pcap_cleanup_hurd: releases buffer if allocated
extern "C" void pcap_cleanup_hurd(pcap_t* p) {
    if (p->buffer != NULL) {
        std::free(p->buffer);
        p->buffer = NULL;
        p->bufsize = 0;
    }
    // Do not attempt to free p->priv here; tests don't own it
}

// The focal function (copied/adapted from the provided snippet)
extern "C" int pcap_activate_hurd(pcap_t *p)
{
{
	struct pcap_hurd *ph;
	mach_port_t master;
	kern_return_t kr;
	int ret = PCAP_ERROR;
	ph = p->priv;
	if (p->snapshot <= 0 || p->snapshot > MAXIMUM_SNAPLEN)
		p->snapshot = MAXIMUM_SNAPLEN;
	/* Try devnode first */
	master = file_name_lookup(p->opt.device, O_READ | O_WRITE, 0);
	if (master != MACH_PORT_NULL)
		kr = device_open(master, D_WRITE | D_READ, "eth", &ph->mach_dev);
	else {
		/* If unsuccessful, try Mach device */
		kr = get_privileged_ports(NULL, &master);
		if (kr) {
			pcapint_fmt_errmsg_for_kern_return_t(p->errbuf,
			    PCAP_ERRBUF_SIZE, kr, "get_privileged_ports");
			if (kr == EPERM)
				ret = PCAP_ERROR_PERM_DENIED;
			goto error;
		}
		kr = device_open(master, D_READ | D_WRITE, p->opt.device,
				 &ph->mach_dev);
	}
	mach_port_deallocate(mach_task_self(), master);
	if (kr) {
		pcapint_fmt_errmsg_for_kern_return_t(p->errbuf, PCAP_ERRBUF_SIZE, kr,
		    "device_open");
		if (kr == ED_NO_SUCH_DEVICE) /* not ENODEV */
			ret = PCAP_ERROR_NO_SUCH_DEVICE;
		goto error;
	}
	kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE,
				&ph->rcv_port);
	if (kr) {
		pcapint_fmt_errmsg_for_kern_return_t(p->errbuf, PCAP_ERRBUF_SIZE, kr,
		    "mach_port_allocate");
		goto error;
	}
	p->bufsize = sizeof(struct net_rcv_msg);
	p->buffer = malloc(p->bufsize);
	if (p->buffer == NULL) {
		pcapint_fmt_errmsg_for_errno(p->errbuf, PCAP_ERRBUF_SIZE,
		    errno, "malloc");
		goto error;
	}
	/*
	 * XXX Ethernet only currently
	 *
	 * XXX - does "Ethernet only currently" mean "the only devices
	 * on which the Hurd supports packet capture are Ethernet
	 * devices", or "it supports other devices but makes them
	 * all provide Ethernet headers"?
	 */
	p->linktype = DLT_EN10MB;
	p->read_op = pcap_read_hurd;
	p->inject_op = pcap_inject_hurd;
	p->setfilter_op = pcap_setfilter_hurd;
	p->stats_op = pcap_stats_hurd;
	return 0;
error:
	pcap_cleanup_hurd(p);
	return ret;
}
}

// Additional helper: if compiled separately, provide a dummy function
extern "C" int pcapint_fmt_errmsg_for_kern_return_t(char* errbuf, int size, int kr, const char* func) {
    if (errbuf && size > 0) {
        std::snprintf(errbuf, (size_t)size, "%s: kern_return_t %d", func, kr);
    }
    (void)kr;
    return 0;
}
extern "C" int pcapint_fmt_errmsg_for_errno(char* errbuf, int size, int err, const char* func) {
    if (errbuf && size > 0) {
        std::snprintf(errbuf, (size_t)size, "%s: errno %d", func, err);
    }
    (void)err;
    return 0;
}

// Test helpers
static void expect_true(bool cond, const char* msg, int& failures) {
    if (!cond) {
        std::cerr << "TEST FAIL: " << msg << "\n";
        ++failures;
    } else {
        std::cout << "TEST PASS: " << msg << "\n";
    }
}

// Test case 1: Master path succeeds and snapshot is normalized; verify allocations and fields
void test_master_path_success() {
    TestHarness::reset();
    TestHarness::file_name_lookup_result = 1; // master path taken
    TestHarness::get_priv_kr = 0;
    TestHarness::device_open_kr_master_non_null = 0;
    TestHarness::malloc_should_fail = false;

    pcap_t p;
    struct pcap_hurd ph;
    ph.mach_dev = -1;
    ph.rcv_port = -1;
    p.priv = &ph;
    p.snapshot = -5; // invalid snapshot should be reset
    p.opt.device = "eth0";
    p.errbuf[0] = '\0';
    p.bufsize = 0;
    p.buffer = NULL;
    p.linktype = 0;

    int ret = pcap_activate_hurd(&p);

    bool ok = (ret == 0);
    bool snapshot_ok = (p.snapshot == MAXIMUM_SNAPLEN);
    bool buf_alloc = (p.buffer != NULL);
    bool rcv_port_valid = (ph.rcv_port != 0);
    bool mach_dev_valid = (ph.mach_dev != -1);
    expect_true(ok, "Master path should succeed and return 0");
    expect_true(snapshot_ok, "Snapshot should be normalized to MAXIMUM_SNAPLEN when invalid");
    expect_true(buf_alloc, "Buffer should be allocated when master path succeeds");
    expect_true(rcv_port_valid, "Receive port should be allocated");
    expect_true(mach_dev_valid, "Mach device should be set by device_open");
}

// Test case 2: Privileged ports path returns EPERM -> expect PCAP_ERROR_PERM_DENIED
void test_perm_denied_when_privileged_ports_fails() {
    TestHarness::reset();
    TestHarness::file_name_lookup_result = 0; // devnode not available
    TestHarness::get_priv_kr = EPERM; // EPERM triggers PERM_DENIED
    TestHarness::device_open_kr_master_null = 0;
    TestHarness::malloc_should_fail = false;

    pcap_t p;
    struct pcap_hurd ph;
    ph.mach_dev = -1;
    ph.rcv_port = -1;
    p.priv = &ph;
    p.snapshot = 100;
    p.opt.device = "some_device";
    p.errbuf[0] = '\0';
    p.bufsize = 0;
    p.buffer = NULL;
    p.linktype = 0;

    int ret = pcap_activate_hurd(&p);

    expect_true(ret == PCAP_ERROR_PERM_DENIED, "EPERM from get_privileged_ports should yield PCAP_ERROR_PERM_DENIED");
}

// Test case 3: Privileged ports succeed, but device_open returns ED_NO_SUCH_DEVICE -> expect PCAP_ERROR_NO_SUCH_DEVICE
void test_no_such_device_error_path() {
    TestHarness::reset();
    TestHarness::file_name_lookup_result = 0; // devnode not available
    TestHarness::get_priv_kr = 0; // success
    TestHarness::device_open_kr_master_non_null = ED_NO_SUCH_DEVICE; // simulate failure on device_open when master non-null
    TestHarness::malloc_should_fail = false;

    pcap_t p;
    struct pcap_hurd ph;
    ph.mach_dev = -1;
    ph.rcv_port = -1;
    p.priv = &ph;
    p.snapshot = 60;
    p.opt.device = "eth0";
    p.errbuf[0] = '\0';
    p.bufsize = 0;
    p.buffer = NULL;
    p.linktype = 0;

    int ret = pcap_activate_hurd(&p);

    expect_true(ret == PCAP_ERROR_NO_SUCH_DEVICE, "ED_NO_SUCH_DEVICE from device_open should yield PCAP_ERROR_NO_SUCH_DEVICE");
}

// Test case 4: Memory allocation failure during buffer allocation leads to PCAP_ERROR
void test_malloc_failure_path() {
    TestHarness::reset();
    TestHarness::file_name_lookup_result = 1; // master path
    TestHarness::get_priv_kr = 0;
    TestHarness::device_open_kr_master_non_null = 0;
    TestHarness::malloc_should_fail = true; // force malloc to fail

    pcap_t p;
    struct pcap_hurd ph;
    ph.mach_dev = -1;
    ph.rcv_port = -1;
    p.priv = &ph;
    p.snapshot = 10;
    p.opt.device = "eth0";
    p.errbuf[0] = '\0';
    p.bufsize = 0;
    p.buffer = NULL;
    p.linktype = 0;

    int ret = pcap_activate_hurd(&p);

    expect_true(ret == PCAP_ERROR, "Malloc failure should result in PCAP_ERROR");
}

// Utility: Run all tests
int main() {
    std::cout << "Starting pcap_activate_hurd unit tests...\n";
    test_master_path_success();
    test_perm_denied_when_privileged_ports_fails();
    test_no_such_device_error_path();
    test_malloc_failure_path();
    std::cout << "Tests completed.\n";
    return 0;
}