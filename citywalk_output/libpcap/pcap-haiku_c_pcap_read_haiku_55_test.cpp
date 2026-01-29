// High-quality C++11 unit tests for the focal function pcap_read_haiku
// This test suite provides a self-contained environment to exercise
// the pcap_read_haiku function under Haiku-like conditions without GTest.
// It uses real UDP sockets to simulate incoming packets and a minimal,
// test-friendly pcap_t/pcap_haiku layout to align with the function's usage.
// The tests are designed to be portable to C++11 compilers and rely only
// on the C standard library plus the system's sockets API when available.
//
// Notes:
// - This file defines a minimal compatible surface for pcap_t, pcap_pkthdr,
//   and related structures to allow linking with the existing pcap-haiku.c
//   implementation included in the build.
// - It uses simple, non-terminating assertions to maximize code coverage.
// - A small test harness is provided (no external test framework).
// - The tests cover: break_loop path, would-block path, and a successful read-path.

#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <cstdlib>
#include <stdint.h>
#include <cstdio>
#include <OS.h>
#include <cstdint>
#include <net/if_dl.h>
#include <fcntl.h>
#include <cstring>
#include <net/if_media.h>
#include <vector>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_types.h>
#include <functional>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pcap-int.h>
#include <sys/sockio.h>
#include <string.h>
#include <config.h>
#include <sys/utsname.h>


// Forward declaration of the focal C function to be tested.
// The actual implementation is in pcap-haiku.c.
extern "C" int pcap_read_haiku(struct pcap_t* handle, int maxPackets,
                               void (*callback)(unsigned char*, const struct pcap_pkthdr*, const unsigned char*),
                               unsigned char* userdata);

// Lightweight re-declaration of types and constants to mirror usage in pcap-haiku.c
// This is a minimal surface; the real pcap_t in the target project may differ.
// We rely on field names used by pcap_read_haiku.
#define PCAP_ERRBUF_SIZE 256
#define PCAP_ERROR -1
#define PCAP_ERROR_BREAK -2

typedef unsigned char u_char;
typedef uint32_t bpf_u_int32;

// Minimal timeval and pcap_pkthdr definitions used by the test callback
struct timeval {
    long tv_sec;
    long tv_usec;
};

struct pcap_pkthdr {
    struct timeval ts;
    bpf_u_int32 caplen;
    bpf_u_int32 len;
};

// Callback type used by pcap_read_haiku
typedef void (*pcap_handler)(u_char*, const struct pcap_pkthdr*, const u_char*);

// Minimal pcap_t layout matching the fields touched by pcap_read_haiku.
// This layout is a best-effort to align with the expectations of pcap-haiku.c
// in this test harness. Any mismatch with the real pcap_t layout is a potential
// source of undefined behavior; the tests are designed to keep this surface
// consistent with the function's usage.
struct pcap_t {
    u_char* buffer;
    int fd;
    unsigned int bufsize;
    int break_loop;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct {
        void* bf_insns; // non-NULL would imply filter invocation; we keep NULL in tests
    } fcode;
    unsigned int snapshot;
    void* priv; // private Haiku-specific data (pcap_haiku*)
};

// Haiku private data mock used by the test to satisfy handle->priv->stat.ps_recv, etc.
struct pcap_haiku {
    struct {
        int ps_recv;
        int ps_drop;
    } stat;
};

// Global state to capture callback invocations for assertion
struct CallbackCapture {
    bool called;
    struct pcap_pkthdr header;
    std::vector<unsigned char> data;
    unsigned char* userdata;
    CallbackCapture() : called(false), userdata(nullptr) {}
} g_cb;

// Deterministic time for testing: emulate real_time_clock_usecs()
typedef long long bigtime_t;
extern "C" bigtime_t real_time_clock_usecs() {
    // Fixed timestamp: 1234 seconds + 567890 microseconds
    return 1234LL * 1000000LL + 567890LL;
}

// Optional: provide a stub for errno-based message formatting used by pcap_read_haiku
extern "C" void pcapint_fmt_errmsg_for_errno(char* errbuf, size_t errbuf_size, int errnoVal,
                                             const char* /*prefix*/) {
    // Very small, deterministic message
    if (errbuf && errbuf_size > 0) {
        snprintf(errbuf, errbuf_size, "errno=%d", errnoVal);
        // Ensure null-termination
        if (errbuf_size > 0) errbuf[errbuf_size - 1] = '\0';
    }
}

// Helper: simple non-terminating assertion
static int g_failures = 0;
#define ASSERT(cond, msg) do { if (!(cond)) { \
    std::cerr << "ASSERTION FAILED: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    ++g_failures; } } while(0)

// Helper: reset callback capture
static void reset_capture() {
    g_cb.called = false;
    g_cb.header = struct pcap_pkthdr();
    g_cb.data.clear();
    g_cb.userdata = nullptr;
}

// Implementation of the test callback matching pcap_handler signature
static void test_callback(unsigned char* userdata, const struct pcap_pkthdr* header,
                          const unsigned char* data) {
    g_cb.called = true;
    g_cb.header = *header;
    if (data) {
        g_cb.data.assign(data, data + header->caplen);
    } else {
        g_cb.data.clear();
    }
    g_cb.userdata = userdata;
}

// Utility: set non-blocking on a socket
static bool set_non_blocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return false;
    return (fcntl(fd, F_SETFL, flags | O_NONBLOCK) != -1);
}

// Test 1: Break path: if handle->break_loop is set, function should return PCAP_ERROR_BREAK
static void test_break_path() {
    // Prepare a minimal handle
    struct pcap_t handle;
    memset(&handle, 0, sizeof(handle));
    handle.buffer = new unsigned char[512];
    handle.fd = -1; // no real socket; path should take the break early
    handle.bufsize = 256;
    handle.break_loop = 1; // trigger break condition

    // Prepare dummy priv/pool
    struct pcap_haiku haiku;
    haiku.stat.ps_recv = 0;
    haiku.stat.ps_drop = 0;
    haiku.priv = nullptr;
    handle.priv = &haiku;
    handle.fcode.bf_insns = nullptr;
    handle.snapshot = 512;

    // Reset
    reset_capture();
    // Call
    int result = pcap_read_haiku(&handle, 1, test_callback, nullptr);

    // Validate
    ASSERT(result == PCAP_ERROR_BREAK, "pcap_read_haiku should return PCAP_ERROR_BREAK when break_loop set");
    ASSERT(handle.break_loop == 0, "break_loop should be reset to 0 after break");
    delete[] handle.buffer;
}

// Test 2: Would-block path: when no data is available and the socket is non-blocking,
// recvfrom should return -1 with errno == EWOULDBLOCK, and method should return 0.
static void test_would_block_path() {
    // Create a UDP server socket and bind to an ephemeral port
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT(server_fd >= 0, "Failed to create UDP socket for would-block test");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0; // ephemeral port

    int r = bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    ASSERT(r == 0, "bind failed for would-block test");

    // Retrieve the assigned port
    socklen_t addr_len = sizeof(addr);
    r = getsockname(server_fd, (struct sockaddr*)&addr, &addr_len);
    ASSERT(r == 0, "getsockname failed");
    int assigned_port = ntohs(addr.sin_port);

    // Make the server socket non-blocking
    set_non_blocking(server_fd);

    // Prepare the Haiku-like handle pointing to server_fd
    struct pcap_t handle;
    memset(&handle, 0, sizeof(handle));
    handle.buffer = new unsigned char[512];
    handle.fd = server_fd;
    handle.bufsize = 256;
    handle.break_loop = 0;

    struct pcap_haiku haiku;
    haiku.stat.ps_recv = 0;
    haiku.stat.ps_drop = 0;
    haiku.priv = &haiku;
    handle.priv = &haiku;
    handle.fcode.bf_insns = nullptr;
    handle.snapshot = 512;

    // No data sent; the non-blocking socket should return would-block
    reset_capture();
    int result = pcap_read_haiku(&handle, 1, test_callback, nullptr);

    // Validate
    // Expect 0 as there is no packet available
    ASSERT(result == 0, "pcap_read_haiku should return 0 when no packets are available (would block)");
    delete[] handle.buffer;
    close(server_fd);
}

// Test 3: Successful read path: send a UDP datagram to the server socket and verify callback
static void test_successful_read_path() {
    // Create a UDP server socket and bind to an ephemeral port
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT(server_fd >= 0, "Failed to create UDP socket for successful read test");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = 0; // ephemeral
    int r = bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    ASSERT(r == 0, "bind failed for successful read test");

    // Get the assigned port
    socklen_t addr_len = sizeof(addr);
    r = getsockname(server_fd, (struct sockaddr*)&addr, &addr_len);
    ASSERT(r == 0, "getsockname failed for successful read test");
    int assigned_port = ntohs(addr.sin_port);

    // Make server non-blocking
    set_non_blocking(server_fd);

    // Prepare a sender socket to push data into server_fd
    int sender_fd = socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT(sender_fd >= 0, "sender socket creation failed");

    // Data to send
    const char payload[] = "HELLO";
    r = sendto(sender_fd, payload, sizeof(payload) - 1, 0,
               (struct sockaddr*)&addr, sizeof(addr));
    ASSERT(r == (int)(sizeof(payload) - 1), "sendto did not send all payload");

    // Prepare pcap-like handle
    struct pcap_t handle;
    memset(&handle, 0, sizeof(handle));
    handle.buffer = new unsigned char[512];
    handle.fd = server_fd;
    handle.bufsize = 256;
    handle.break_loop = 0;

    struct pcap_haiku haiku;
    haiku.stat.ps_recv = 0;
    haiku.stat.ps_drop = 0;
    haiku.priv = &haiku;
    handle.priv = &haiku;
    handle.fcode.bf_insns = nullptr;
    handle.snapshot = 1024;

    // Prepare a capture callback to validate header and payload
    reset_capture();
    // Use a wrapper to capture the callback data
    g_cb = CallbackCapture();
    g_cb.called = false;
    g_cb.userdata = nullptr;

    // The test callback will fill g_cb via test_callback
    // Bind the wrapper to our test
    // call pcap_read_haiku; it should read the datagram from the socket
    int result = pcap_read_haiku(&handle, 1, test_callback, nullptr);

    // Validate
    ASSERT(result == 1, "pcap_read_haiku should return 1 on successful packet read");
    ASSERT(g_cb.called, "Callback should be invoked on successful packet read");
    ASSERT(g_cb.header.caplen == (bpf_u_int32)(sizeof(payload) - 1),
           "Captured length should match payload size");
    ASSERT(g_cb.header.len == (bpf_u_int32)(sizeof(payload) - 1),
           "Reported packet length should match payload size");
    // Verify timestamp conversion roughly (non-deterministic in real-time environments, but we used a deterministic clock)
    // We simply ensure the ts fields are populated
    ASSERT(g_cb.header.ts.tv_sec >= 0, "Timestamp seconds should be non-negative");
    ASSERT(g_cb.header.ts.tv_usec >= 0, "Timestamp microseconds should be non-negative");
    // Validate data integrity
    ASSERT(g_cb.data.size() == (size_t)g_cb.header.caplen,
           "Callback should pass correct captured data length");
    if (!g_cb.data.empty()) {
        ASSERT(memcmp(g_cb.data.data(), payload, g_cb.header.caplen) == 0,
               "Payload data should match what was sent");
    }

    // Cleanup
    delete[] handle.buffer;
    close(server_fd);
    close(sender_fd);
}

// Main: run tests and report summary
int main() {
    fprintf(stdout, "pcap_read_haiku unit test suite starting...\n");
    // Run tests
    test_break_path();
    test_would_block_path();
    test_successful_read_path();

    if (g_failures == 0) {
        fprintf(stdout, "ALL tests PASSED\n");
        return 0;
    } else {
        fprintf(stderr, "Tests FAILED: %d failure(s)\n", g_failures);
        return 1;
    }
}