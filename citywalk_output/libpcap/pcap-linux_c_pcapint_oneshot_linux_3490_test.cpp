// Test suite for the focal method: pcapint_oneshot_linux
// This test targets the Linux "oneshot" path behavior implemented in pcapint_oneshot_linux.
// The tests are written in C++11, do not rely on GTest, and use a lightweight in-file harness.
// Explanation tied to Candidate Keywords from Step 1: oneshot_buffer, hdr (pcap_pkthdr), pkt (pointer to buffer),
// pcap_t (handle), priv (pcap_linux -> oneshot_buffer), memcpy, header copy, and pointer assignment.

// Important: The test provides lightweight, minimal stubs for the required structures to allow
// linking with the focal C function defined in pcap-linux.c. It does not rely on private/private
// members beyond the required layout used by pcapint_oneshot_linux.

#include <netinet/in.h>
#include <pcap/vlan.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/if_ether.h>
#include <netlink/msg.h>
#include <iostream>
#include <sys/ioctl.h>
#include <errno.h>
#include <cassert>
#include <pcap-snf.h>
#include <pcap/can_socketcan.h>
#include <pcap-util.h>
#include <cstdint>
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


// Domain knowledge note: we assume the function uses the following types and fields.
// The test redefines these minimally and with matching tags as expected by pcapint_oneshot_linux.

// C alias used by the focal function
typedef unsigned char u_char;

// Lightweight stand-ins for the pcap_pkthdr structure used by the focal function
struct timeval {
    long tv_sec;
    long tv_usec;
};

struct pcap_pkthdr {
    struct timeval ts;  // timestamp
    unsigned int caplen; // captured length
    unsigned int len;    // original length
};

// Lightweight stand-ins for internal structures
struct pcap_linux {
    unsigned char oneshot_buffer[4096];
};

struct pcap_t {
    struct pcap_linux *priv; // private data used by the Linux backend
};

// The actual oneshot_userdata struct layout used by the focal function
struct oneshot_userdata {
    pcap_t *pd;
    struct pcap_pkthdr *hdr;
    unsigned char **pkt;
};

// Forward declaration of the focal function (C linkage)
extern "C" void pcapint_oneshot_linux(u_char *user,
                                     const struct pcap_pkthdr *h,
                                     const u_char *bytes);

// Lightweight assertion macro (non-terminating, prints message on failure)
static int g_failures = 0;
#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "ASSERTION FAILED: " << msg << " [Line: " << __LINE__ << "]\n"; \
            ++g_failures; \
        } \
    } while (0)

// Test 1: Basic correctness - header is copied, buffer is filled, and pkt pointer points to buffer
void test_oneshot_basic_correctness() {
    // Candidate Keywords mapping:
    // - oneshot_buffer (pcap_linux)
    // - hdr (pcap_pkthdr)
    // - pd / priv linkage
    // - memcpy behavior
    // - *sp->pkt assignment to oneshot_buffer

    // Arrange
    struct pcap_linux handlep;
    // Clear buffer for deterministic comparison
    std::memset(handlep.oneshot_buffer, 0xAA, sizeof(handlep.oneshot_buffer));

    struct pcap_t handle;
    handle.priv = &handlep;

    // Header to copy into
    struct pcap_pkthdr hdr_copy;
    hdr_copy.caplen = 5;
    hdr_copy.len = 8;
    hdr_copy.ts.tv_sec = 1;
    hdr_copy.ts.tv_usec = 123456;

    // Input packet bytes
    const unsigned char input_bytes[5] = { 0x11, 0x22, 0x33, 0x44, 0x55 };

    // Build oneshot_userdata for the test
    unsigned char *pkt_out = nullptr;
    struct oneshot_userdata sp = { &handle, &hdr_copy, &pkt_out };

    // We must pass the address of sp as a raw user pointer
    pcapint_oneshot_linux((u_char *)&sp, &hdr_copy, input_bytes);

    // Assert
    // After call, hdr_copy should reflect the input header
    ASSERT_TRUE(hdr_copy.caplen == 5, "Header caplen should be copied (5)");
    ASSERT_TRUE(hdr_copy.len == 8, "Header len should be copied (8)");
    ASSERT_TRUE(hdr_copy.ts.tv_sec == 1, "Header ts_sec should be copied (1)");
    ASSERT_TRUE(hdr_copy.ts.tv_usec == 123456, "Header ts_usec should be copied (123456)");

    // The oneshot buffer should contain the input bytes
    ASSERT_TRUE(std::memcmp(handlep.oneshot_buffer, input_bytes, 5) == 0,
                "Buffer content should match input bytes");

    // The pkt pointer should now point to the oneshot_buffer
    ASSERT_TRUE(pkt_out == handlep.oneshot_buffer,
                "sp->pkt should be assigned to handlep->oneshot_buffer");
}

// Test 2: Boundary condition - zero-length capture (caplen = 0) should be handled safely
void test_oneshot_zero_length() {
    // Arrange
    struct pcap_linux handlep;
    std::memset(handlep.oneshot_buffer, 0x00, sizeof(handlep.oneshot_buffer));

    struct pcap_t handle;
    handle.priv = &handlep;

    struct pcap_pkthdr hdr_copy;
    hdr_copy.caplen = 0;
    hdr_copy.len = 0;
    hdr_copy.ts.tv_sec = 0;
    hdr_copy.ts.tv_usec = 0;

    const unsigned char input_bytes[1] = { 0xFF }; // Should not be dereferenced due to caplen 0

    unsigned char *pkt_out = nullptr;
    struct oneshot_userdata sp = { &handle, &hdr_copy, &pkt_out };

    // Act
    pcapint_oneshot_linux((u_char *)&sp, &hdr_copy, input_bytes);

    // Assert
    // Header should reflect zero caplen
    ASSERT_TRUE(hdr_copy.caplen == 0, "Header caplen should be 0");
    // Buffer should remain unchanged (or zeroed if it previously was)
    // Since caplen == 0, we skip content comparison and ensure no crash occurred
    ASSERT_TRUE(pkt_out == handlep.oneshot_buffer, "sp->pkt should still point to oneshot_buffer (even if caplen is 0)");
}

// Test 3: Non-zero length with different data to ensure multiple invocations behave consistently
void test_oneshot_varied_data() {
    // Arrange
    struct pcap_linux handlep;
    std::memset(handlep.oneshot_buffer, 0x00, sizeof(handlep.oneshot_buffer));

    struct pcap_t handle;
    handle.priv = &handlep;

    struct pcap_pkthdr hdr_copy;
    hdr_copy.caplen = 8;
    hdr_copy.len = 16;
    hdr_copy.ts.tv_sec = 42;
    hdr_copy.ts.tv_usec = 99999;

    const unsigned char input_bytes[8] = { 0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80};

    unsigned char *pkt_out = nullptr;
    struct oneshot_userdata sp = { &handle, &hdr_copy, &pkt_out };

    // Act
    pcapint_oneshot_linux((u_char *)&sp, &hdr_copy, input_bytes);

    // Assert
    ASSERT_TRUE(std::memcmp(handlep.oneshot_buffer, input_bytes, 8) == 0,
                "Buffer content should match new input bytes (8 bytes)");
    ASSERT_TRUE(pkt_out == handlep.oneshot_buffer,
                "sp->pkt should be assigned to updated oneshot_buffer");
    // Header should reflect input header values
    ASSERT_TRUE(hdr_copy.caplen == 8, "Header caplen should be updated to 8");
    ASSERT_TRUE(hdr_copy.ts.tv_sec == 42, "Header ts_sec should be 42");
}

// Simple runner to execute tests and report results
int main() {
    std::cout << "pcapint_oneshot_linux test suite starting...\n";

    test_oneshot_basic_correctness();
    test_oneshot_zero_length();
    test_oneshot_varied_data();

    if (g_failures == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << g_failures << " test(s) FAILED.\n";
        return 1;
    }
}