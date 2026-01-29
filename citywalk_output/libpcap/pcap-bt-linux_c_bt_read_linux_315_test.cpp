// Unit test suite for bt_read_linux from pcap-bt-linux.c
// Target: C++11, no Google Test, minimal test harness using direct assertions.
// We create a small fake environment to exercise key branches of bt_read_linux
// (break path, non-blocking path, and a path where a callback is invoked for data).
// The tests rely on a custom fake recvmsg (interposed at link-time) to avoid real I/O.
//
// Important: This test provides a self-contained environment and mocks only the
// necessary pieces to exercise bt_read_linux in isolation. It is designed to be
// compiled and linked together with pcap-bt-linux.c (the focal method source).

#include <pcap/bluetooth.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
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


// Forward declare the focal function to be tested (C linkage)
extern "C" {
    // Minimal compatible prototypes for the test harness
    typedef unsigned char u_char;

    // Forward declare pcap_t-like type used by bt_read_linux (library layout)
    typedef struct pcap_t pcap_t;
    typedef void (*pcap_handler)(u_char *user, const struct pcap_pkthdr *h, const u_char *bytes);

    struct timeval { long tv_sec; long tv_usec; };
    typedef uint32_t bpf_u_int32;
    struct pcap_pkthdr { struct timeval ts; bpf_u_int32 caplen; bpf_u_int32 len; };

    typedef struct {
        uint32_t direction;
    } pcap_bluetooth_h4_header;

    // Fallback declaration of the function under test
    int bt_read_linux(pcap_t *handle, int max_packets, pcap_handler callback, u_char *user);
}

// Test constants (match typical libpcap internal constants)
#ifndef BT_CTRL_SIZE
#define BT_CTRL_SIZE 8
#endif
#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif
#define HCI_CMSG_DIR    1
#define HCI_CMSG_TSTAMP 2
#define PCAP_D_IN  0
#define PCAP_D_OUT 1
#define PCAP_ERROR -1
#define PCAP_ERROR_BREAK -2

// Minimal pcap_t-like layout used by the focal function.
// We layout fields in the same order used by bt_read_linux to ensure proper access.
struct pcap_t {
    unsigned char* buffer;
    int fd;
    int break_loop;
    unsigned int snapshot;
    int direction;
    struct { void* bf_insns; } fcode;
    char errbuf[PCAP_ERRBUF_SIZE];
};

// Typedefs and mocks for test helpers
static int g_cb_invocations = 0;
static struct pcap_pkthdr g_last_pkthdr;
static unsigned char* g_last_cb_bytes = nullptr;
static pcap_handler g_cb_ptr = nullptr;

// Simple callback matching the library signature
static void test_callback(unsigned char* user, const struct pcap_pkthdr* h, const unsigned char* bytes)
{
    (void)user;
    g_cb_invocations++;
    if (h) g_last_pkthdr = *h;
    g_last_cb_bytes = const_cast<unsigned char*>(bytes);
}

// Mock of pcapint_fmt_errmsg_for_errno to simulate error reporting path.
// We mirror the expected signature: void pcapint_fmt_errmsg_for_errno(char*, size_t, int, const char*, ...)
extern "C" void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t errbuf_size, int errno_value, const char* fmt, ...)
{
    (void)fmt;
    // Simple deterministic message for testing
    const char* msg = "mocked errno";
    if (errbuf && errbuf_size > 0) {
        size_t len = strlen(msg);
        if (len >= errbuf_size) len = errbuf_size - 1;
        memcpy(errbuf, msg, len);
        errbuf[len] = '\0';
    }
    (void)errno_value; // unused further
}

// Mocked recvmsg to drive the focal function without real I/O.
// We interpose the symbol at link-time to replace the system call within the test binary.
#ifdef __cplusplus
extern "C" {
#endif

// A simple enum to select test scenarios for the fake recvmsg
enum RecvMsgScenario {
    SCEN_BREAK, // not used (break_loop short-circuits before recvmsg)
    SCEN_AGAIN, // simulate EAGAIN / EWOULDBLOCK (non-blocking success path)
    SCEN_ERROR, // simulate some other errno (to exercise error handling)
    SCEN_CB       // simulate a data path with a valid payload and cmsghdrs
};

static int g_recv_scenario = SCEN_CB;

// Internal helper to place cmsghdrs for test SCEN_CB
static void fill_fake_control_messages(struct msghdr* msg)
{
    // We place two cmsghdr blocks in the control buffer: one for HCI_CMSG_DIR (in value)
    // and one for HCI_CMSG_TSTAMP (ts). We rely on standard cmsghdr layout.
    unsigned char* ctrl = (unsigned char*)msg->msg_control;
    if (!ctrl) return;

    struct cmsghdr* cm1 = (struct cmsghdr*)ctrl;
    cm1->cmsg_len = sizeof(struct cmsghdr) + sizeof(int);
    cm1->cmsg_level = 0;
    cm1->cmsg_type = HCI_CMSG_DIR;
    int in = 1;
    memcpy((unsigned char*)cm1 + sizeof(struct cmsghdr), &in, sizeof(in));

    unsigned char* cm2p = ctrl + cm1->cmsg_len;
    struct cmsghdr* cm2 = (struct cmsghdr*)cm2p;
    cm2->cmsg_len = sizeof(struct cmsghdr) + sizeof(struct timeval);
    cm2->cmsg_level = 0;
    cm2->cmsg_type = HCI_CMSG_TSTAMP;
    struct timeval tv; tv.tv_sec = 0; tv.tv_usec = 0;
    memcpy((unsigned char*)cm2 + sizeof(struct cmsghdr), &tv, sizeof(tv));
}

// The fake recvmsg implementation
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags)
{
    (void)sockfd;
    // Trace scenario (some minimal behavior)
    switch (g_recv_scenario) {
        case SCEN_AGAIN:
        case SCEN_ERROR: {
            // Simulate a failure with errno set appropriately
            errno = (g_recv_scenario == SCEN_AGAIN) ? EAGAIN : EINVAL;
            return -1;
        }
        case SCEN_CB: {
            // Fill the iovec data region with known payload
            if (msg && msg->msg_iov && msg->msg_iov[0].iov_base) {
                unsigned char* data = (unsigned char*)msg->msg_iov[0].iov_base;
                // 4-byte payload
                data[0] = 0xAA; data[1] = 0xBB; data[2] = 0xCC; data[3] = 0xDD;
            }
            // Prepare two cmsghdr blocks in the control buffer
            fill_fake_control_messages(msg);
            // Return a non-zero length to simulate received data
            return 4;
        }
        default:
            return -1;
    }
}

#ifdef __cplusplus
}
#endif

// End of recvmsg mock

// Prototypes for bt_read_linux from the focal source.
// We declare the minimal signature compatible with the test harness.
extern "C" int bt_read_linux(pcap_t *handle, int max_packets, pcap_handler callback, u_char *user);

#ifdef __cplusplus
extern "C" {
#endif
// Type-safe wrappers for test setup
static void reset_test_context()
{
    g_cb_invocations = 0;
    g_last_cb_bytes = nullptr;
    g_cb_ptr = nullptr;
    memset(&g_last_pkthdr, 0, sizeof(g_last_pkthdr));
}

static void setup_callback(pcap_handler cb)
{
    g_cb_ptr = cb;
}
#ifdef __cplusplus
}
#endif

// Test 1: Break path should return PCAP_ERROR_BREAK and reset break_loop
void test_break_loop_path()
{
    reset_test_context();

    // Prepare a fake handle
    const size_t BUF_SIZE = 4096;
    unsigned char* buf = (unsigned char*)malloc(BUF_SIZE);
    assert(buf != nullptr);

    pcap_t handle;
    handle.buffer = buf;
    handle.fd = -1;
    handle.break_loop = 1;      // trigger break on first check
    handle.snapshot = 64;
    handle.direction = PCAP_D_IN;
    handle.fcode.bf_insns = NULL;
    memset(handle.errbuf, 0, sizeof(handle.errbuf));

    // No callback needed for break path
    int ret = bt_read_linux(&handle, 1, test_callback, nullptr);

    // Validate early-break path
    assert(ret == PCAP_ERROR_BREAK);
    // Ensure break_loop was reset
    assert(handle.break_loop == 0);

    free(buf);
}

// Test 2: Non-blocking path (EAGAIN) should return 0
void test_nonblocking_eagain_path()
{
    reset_test_context();

    // Prepare a fake handle
    const size_t BUF_SIZE = 4096;
    unsigned char* buf = (unsigned char*)malloc(BUF_SIZE);
    assert(buf != nullptr);

    pcap_t handle;
    handle.buffer = buf;
    handle.fd = -1;
    handle.break_loop = 0;
    handle.snapshot = 64;
    handle.direction = PCAP_D_IN;
    handle.fcode.bf_insns = NULL;
    memset(handle.errbuf, 0, sizeof(handle.errbuf));

    // Set fake recvmsg to simulate EAGAIN
    g_recv_scenario = SCEN_AGAIN;

    int ret = bt_read_linux(&handle, 1, test_callback, nullptr);

    // Expect non-blocking path: return 0
    assert(ret == 0);

    free(buf);
}

// Test 3: Error path (errno other than EAGAIN/EWOULDBLOCK) should return PCAP_ERROR
void test_error_path_other_errno()
{
    reset_test_context();

    // Prepare a fake handle
    const size_t BUF_SIZE = 4096;
    unsigned char* buf = (unsigned char*)malloc(BUF_SIZE);
    assert(buf != nullptr);

    pcap_t handle;
    handle.buffer = buf;
    handle.fd = -1;
    handle.break_loop = 0;
    handle.snapshot = 64;
    handle.direction = PCAP_D_IN;
    handle.fcode.bf_insns = NULL;
    memset(handle.errbuf, 0, sizeof(handle.errbuf));

    // Set fake recvmsg to simulate a non-EAGAIN error
    g_recv_scenario = SCEN_ERROR;

    int ret = bt_read_linux(&handle, 1, test_callback, nullptr);

    // Expect error return code
    assert(ret == PCAP_ERROR);

    // Ensure the error buffer was touched (our mock sets a fixed message)
    assert(strlen(handle.errbuf) > 0);

    free(buf);
}

// Test 4: Data path: verify callback is invoked and caplen includes header size
void test_data_path_triggers_callback()
{
    reset_test_context();

    // Prepare a bigger buffer to host control messages and payload
    const size_t BUF_SIZE = 4096;
    unsigned char* buf = (unsigned char*)malloc(BUF_SIZE);
    assert(buf != nullptr);

    pcap_t handle;
    handle.buffer = buf;
    handle.fd = -1;
    handle.break_loop = 0;
    handle.snapshot = 8;  // small payload size
    handle.direction = PCAP_D_IN;
    handle.fcode.bf_insns = NULL;
    memset(handle.errbuf, 0, sizeof(handle.errbuf));

    // Use the callback to verify invocation and data length
    g_cb_invocations = 0;
    g_last_pkthdr = {};
    g_last_cb_bytes = nullptr;
    g_cb_ptr = test_callback;

    // Prepare scenario for callback path
    g_recv_scenario = SCEN_CB;

    // Call the focal function
    int ret = bt_read_linux(&handle, 1, test_callback, nullptr);

    // We expect the callback to be invoked exactly once
    assert(ret == 1);
    assert(g_cb_invocations == 1);
    // The caplen should be payload length (4) + Bluetooth header size (sizeof(pcap_bluetooth_h4_header))
    // Our test header is defined as 4 bytes
    const unsigned int header_size = sizeof(pcap_bluetooth_h4_header);
    // The code adds 4 bytes (ret) to caplen and then adds header_size
    unsigned int expected_caplen = 4 + header_size;
    assert(g_last_pkthdr.caplen == expected_caplen);

    // The payload bytes passed to the callback should match what we placed in recvmsg
    // We placed 4 payload bytes: AA BB CC DD
    if (g_last_cb_bytes) {
        assert(g_last_cb_bytes[0] == 0xAA);
        assert(g_last_cb_bytes[1] == 0xBB);
        assert(g_last_cb_bytes[2] == 0xCC);
        assert(g_last_cb_bytes[3] == 0xDD);
    } else {
        // If the callback didn't get bytes, force failure
        assert(false && "Callback did not receive data bytes");
    }

    free(buf);
}

// Main entry: run all tests
int main()
{
    // Run tests in a simple sequential manner
    test_break_loop_path();
    printf("Test 1 (break path) passed.\n");

    test_nonblocking_eagain_path();
    printf("Test 2 (non-blocking EAGAIN path) passed.\n");

    test_error_path_other_errno();
    printf("Test 3 (error path) passed.\n");

    test_data_path_triggers_callback();
    printf("Test 4 (data path with callback) passed.\n");

    printf("All tests completed successfully.\n");
    return 0;
}