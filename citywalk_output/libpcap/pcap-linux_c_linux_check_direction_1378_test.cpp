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
#include <cstdint>
#include <dirent.h>
#include <netlink/genl/genl.h>
#include <linux/netlink.h>
#include <fcntl.h>
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


// Minimal re-implementation of the focal function and necessary structures
// to enable self-contained unit tests without depending on the original
// project headers. This is intended for test purposes only.

#define PACKET_OUTGOING 1
#define ARPHRD_CAN 280
#define PCAP_D_OUT 1
#define PCAP_D_IN 2

// Forward declarations matching the usage in the focal function
struct pcap_linux {
    int lo_ifindex;
};

struct pcap_t {
    int direction;
    struct pcap_linux *priv;
};

struct sockaddr_ll {
    unsigned short sll_pkttype;
    int sll_ifindex;
    int sll_hatype;
};

// Re-implementation of linux_check_direction copied from the focal method
static int linux_check_direction(const pcap_t *handle, const struct sockaddr_ll *sll)
{
{
    struct pcap_linux *handlep = handle->priv;
    if (sll->sll_pkttype == PACKET_OUTGOING) {
        /*
         * Outgoing packet.
         * If this is from the loopback device, reject it;
         * we'll see the packet as an incoming packet as well,
         * and we don't want to see it twice.
         */
        if (sll->sll_ifindex == handlep->lo_ifindex)
            return 0;
        /*
         * If this is an outgoing CAN frame, and the user doesn't
         * want only outgoing packets, reject it; CAN devices
         * and drivers, and the CAN stack, always arrange to
         * loop back transmitted packets, so they also appear
         * as incoming packets.  We don't want duplicate packets,
         * and we can't easily distinguish packets looped back
         * by the CAN layer than those received by the CAN layer,
         * so we eliminate this packet instead.
         *
         * We check whether this is a CAN frame by checking whether
         * the device's hardware type is ARPHRD_CAN.
         */
        if (sll->sll_hatype == ARPHRD_CAN &&
             handle->direction != PCAP_D_OUT)
            return 0;
        /*
         * If the user only wants incoming packets, reject it.
         */
        if (handle->direction == PCAP_D_IN)
            return 0;
    } else {
        /*
         * Incoming packet.
         * If the user only wants outgoing packets, reject it.
         */
        if (handle->direction == PCAP_D_OUT)
            return 0;
    }
    return 1;
}
}

// Simple non-terminating test framework (no GTest/GMock)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_ASSERT_EQ(expected, actual, msg) do { \
    g_total_tests++; \
    if ((expected) != (actual)) { \
        ++g_failed_tests; \
        std::cerr << "Test failure: " << (msg) << " | expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
    } \
} while(0)

// Namespace for test cases
namespace linux_direction_tests {

using namespace std;

// Test 1: Outgoing packet from the loopback interface should be rejected (return 0)
void test_outgoing_loopback_rejected()
{
    // Arrange
    pcap_linux pl;
    pl.lo_ifindex = 10;
    pcap_t handle;
    handle.direction = PCAP_D_OUT;        // User wants outgoing only
    handle.priv = &pl;

    sockaddr_ll sll;
    sll.sll_pkttype = PACKET_OUTGOING;   // Outgoing packet
    sll.sll_ifindex = 10;                 // Same as loopback index
    sll.sll_hatype = ARPHRD_CAN;          // CAN type (value shouldn't matter here)

    // Act
    int result = linux_check_direction(&handle, &sll);

    // Assert
    TEST_ASSERT_EQ(0, result, "Outgoing packet from loopback interface should be rejected (0)");
}

// Test 2: Outgoing CAN frame when user does not want only outgoing should be rejected (return 0)
void test_outgoing_can_rejected_when_not_outgoing_only()
{
    // Arrange
    pcap_linux pl;
    pl.lo_ifindex = 5;
    pcap_t handle;
    handle.direction = PCAP_D_IN;           // User wants only incoming packets
    handle.priv = &pl;

    sockaddr_ll sll;
    sll.sll_pkttype = PACKET_OUTGOING;     // Outgoing packet
    sll.sll_ifindex = 6;                    // Different interface
    sll.sll_hatype = ARPHRD_CAN;            // CAN hardware type

    // Act
    int result = linux_check_direction(&handle, &sll);

    // Assert
    TEST_ASSERT_EQ(0, result, "Outgoing CAN frame when only IN is requested should be rejected (0)");
}

// Test 3: Outgoing non-CAN frame with OUT direction should be accepted (return 1)
void test_outgoing_non_can_accepted()
{
    // Arrange
    pcap_linux pl;
    pl.lo_ifindex = 3;
    pcap_t handle;
    handle.direction = PCAP_D_OUT;           // User wants outgoing packets
    handle.priv = &pl;

    sockaddr_ll sll;
    sll.sll_pkttype = PACKET_OUTGOING;     // Outgoing packet
    sll.sll_ifindex = 7;                    // Different interface
    sll.sll_hatype = 999;                   // Some non-CAN hardware

    // Act
    int result = linux_check_direction(&handle, &sll);

    // Assert
    TEST_ASSERT_EQ(1, result, "Outgoing non-CAN packet with OUT direction should be accepted (1)");
}

// Test 4: Incoming packet and direction IN should be accepted (return 1)
void test_incoming_allows_when_in()
{
    // Arrange
    pcap_linux pl;
    pl.lo_ifindex = 11;
    pcap_t handle;
    handle.direction = PCAP_D_IN;            // User wants only IN packets
    handle.priv = &pl;

    sockaddr_ll sll;
    sll.sll_pkttype = 0;                     // Not PACKET_OUTGOING => incoming
    sll.sll_ifindex = 12;                    // Different interface
    sll.sll_hatype = ARPHRD_CAN;             // CAN type (not relevant for incoming path)

    // Act
    int result = linux_check_direction(&handle, &sll);

    // Assert
    TEST_ASSERT_EQ(1, result, "Incoming packet when IN is requested should be accepted (1)");
}

// Test 5: Incoming path but user requests only OUT should be rejected (return 0)
void test_incoming_rejected_when_out_only()
{
    // Arrange
    pcap_linux pl;
    pl.lo_ifindex = 9;
    pcap_t handle;
    handle.direction = PCAP_D_OUT;           // User wants outgoing packets only
    handle.priv = &pl;

    sockaddr_ll sll;
    sll.sll_pkttype = 0;                     // Not PACKET_OUTGOING => incoming
    sll.sll_ifindex = 9;                     // Could be same or different; not relevant here

    // Act
    int result = linux_check_direction(&handle, &sll);

    // Assert
    TEST_ASSERT_EQ(0, result, "Incoming packet when OUT is requested should be rejected (0)");
}

// Test 6: Outgoing CAN frame with OUT direction and non-loopback interface should be accepted (return 1)
void test_outgoing_can_with_out_and_non_loopback()
{
    // Arrange
    pcap_linux pl;
    pl.lo_ifindex = 99;
    pcap_t handle;
    handle.direction = PCAP_D_OUT;           // User wants outgoing packets
    handle.priv = &pl;

    sockaddr_ll sll;
    sll.sll_pkttype = PACKET_OUTGOING;     // Outgoing packet
    sll.sll_ifindex = 100;                  // Different interface (not loopback)
    sll.sll_hatype = ARPHRD_CAN;             // CAN hardware type

    // Act
    int result = linux_check_direction(&handle, &sll);

    // Assert
    TEST_ASSERT_EQ(1, result, "Outgoing CAN frame with OUT direction on non-loopback should be accepted (1)");
}

} // namespace linux_direction_tests

int main() {
    using namespace linux_direction_tests;

    test_outgoing_loopback_rejected();
    test_outgoing_can_rejected_when_not_outgoing_only();
    test_outgoing_non_can_accepted();
    test_incoming_allows_when_in();
    test_incoming_rejected_when_out_only();
    test_outgoing_can_with_out_and_non_loopback();

    int total = g_total_tests;
    int failed = g_failed_tests;

    std::cout << "Test summary: " << (total - failed) << " / " << total << " passed." << std::endl;
    if (failed > 0) {
        std::cout << failed << " test(s) failed." << std::endl;
    }
    return failed ? 1 : 0;
}