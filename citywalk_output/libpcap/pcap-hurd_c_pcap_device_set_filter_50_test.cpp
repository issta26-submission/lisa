// Minimal C++11 unit tests for the focal method pcap_device_set_filter
// Note: This test suite uses lightweight hand-written assertions (no GTest/GMock).
// It provides small test doubles for dependent C functions to exercise the focal path
// without requiring the full external Hurld/Mach environment.

// The tests assume a compatible simplified view of the libpcap pcap_t and related types.
// They are designed to compile in a C++11 environment and to be self-contained.

#include <string.h>
#include <errno.h>
#include <device/net_status.h>
#include <net/if_ether.h>
#include <time.h>
#include <iostream>
#include <stddef.h>
#include <stdio.h>
#include <hurd.h>
#include <device/device.h>
#include <cstdio>
#include <stdlib.h>
#include <device/device_types.h>
#include <config.h>
#include <pcap-int.h>
#include <mach.h>
#include <fcntl.h>
#include <cstring>


// -------------------------
// Test scaffolding & mocks
// -------------------------

// Simple assertion helpers (non-terminating)
static int g_tests_failed = 0;
#define ASSERT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "Assertion failed: " #cond " at " __FILE__ ":" << __LINE__ << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

#define ASSERT_EQ(a,b) do { \
    if((a) != (b)) { \
        std::cerr << "Assertion failed: " #a " == " #b " (" << (a) << " != " << (b) << ") at " __FILE__ ":" << __LINE__ << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

#define ASSERT_STR_EQ(a,b) do { \
    if(std::strcmp((a),(b)) != 0) { \
        std::cerr << "Assertion failed: strings differ: \"" << (a) << "\" != \"" << (b) << "\" at " __FILE__ ":" << __LINE__ << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

#define ASSERT_STR_CONTAINS(haystack, needle) do { \
    if(std::strstr((haystack), (needle)) == nullptr) { \
        std::cerr << "Assertion failed: expected to find \"" << (needle) \
                  << "\" in \"" << (haystack) << "\" at " __FILE__ ":" << __LINE__ << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)

// Compatibility macros / constants (simplified for test environment)
typedef int kern_return_t;
typedef int mach_msg_type_number_t;
typedef void* filter_array_t;
#define MACH_MSG_TYPE_MAKE_SEND 0
#define PCAP_ERRBUF_SIZE 256
#define PCAP_ERROR -1

// Minimal local view of the types used by pcap_device_set_filter
typedef struct pcap_hurd {
    int mach_dev;
    int rcv_port;
} pcap_hurd;

typedef struct pcap_t {
    pcap_hurd *priv;
    char errbuf[PCAP_ERRBUF_SIZE];
} pcap_t;

// Forward declaration of focal function (as it would appear when linked with the real C file)
extern "C" int pcap_device_set_filter(pcap_t *p, filter_array_t filter_array,
                                      const mach_msg_type_number_t filter_count);

// Prototypes for mocked dependency functions (to be overridden by test doubles)
extern "C" kern_return_t device_set_filter(int mach_dev, int rcv_port,
                                           int msg_type, int something,
                                           filter_array_t filter_array,
                                           mach_msg_type_number_t filter_count);

extern "C" void pcapint_fmt_errmsg_for_kern_return_t(char *errbuf, size_t buflen,
                                                       kern_return_t kr, const char *msg);

// Global state for mock behavior
static kern_return_t g_next_kr = 0;
static int g_last_dev = -1;
static int g_last_rcv = -1;
static int g_last_msg = -1;
static int g_last_count = -1;

// Mock implementation of device_set_filter to capture inputs and return controlled value
extern "C" kern_return_t device_set_filter(int mach_dev, int rcv_port, int msg_type, int something,
                                           filter_array_t filter_array,
                                           mach_msg_type_number_t filter_count)
{
    g_last_dev = mach_dev;
    g_last_rcv = rcv_port;
    g_last_msg = msg_type;
    g_last_count = (int)filter_count;
    return g_next_kr;
}

// Mock implementation of error formatting used by the focal method
extern "C" void pcapint_fmt_errmsg_for_kern_return_t(char *errbuf, size_t buflen,
                                                       kern_return_t kr, const char *msg)
{
    if (kr == 0) {
        if (buflen > 0) errbuf[0] = '\0';
        return;
    }
    snprintf(errbuf, buflen, "err %d: %s", (int)kr, msg);
}

// ----------------------------------
// Unit Tests for pcap_device_set_filter
// ----------------------------------

/*
 * Test 1: Success path
 * - Simulate device_set_filter returning 0 (KERN_SUCCESS).
 * - Expect pcap_device_set_filter to return 0 and not alter the error buffer.
 * - Also verify that the correct device/port and message type are passed through.
 */
static void test_success_path()
{
    // Arrange
    g_next_kr = 0; // success
    g_last_dev = g_last_rcv = g_last_msg = g_last_count = -1;

    pcap_hurd ph;
    ph.mach_dev = 12345;
    ph.rcv_port = 67890;

    pcap_t p;
    p.priv = &ph;
    const char* original_err = "unchanged";
    std::strcpy(p.errbuf, original_err);

    // Act
    int rc = pcap_device_set_filter(&p, nullptr, 5);

    // Assert
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(g_last_dev, 12345);
    ASSERT_EQ(g_last_rcv, 67890);
    ASSERT_EQ(g_last_msg, MACH_MSG_TYPE_MAKE_SEND);
    ASSERT_EQ(g_last_count, 5);
    ASSERT_STR_EQ(p.errbuf, original_err); // error buffer should remain unchanged on success
}

/*
 * Test 2: Failure path
 * - Simulate device_set_filter returning a non-zero kern_return_t.
 * - Expect pcap_device_set_filter to return PCAP_ERROR and populate errbuf via
 *   pcapint_fmt_errmsg_for_kern_return_t with a message containing "device_set_filter".
 * - Verify that the correct device/port and message type are passed through.
 */
static void test_failure_path()
{
    // Arrange
    g_next_kr = -12345; // non-zero to simulate an error
    g_last_dev = g_last_rcv = g_last_msg = g_last_count = -1;

    pcap_hurd ph;
    ph.mach_dev = 7;
    ph.rcv_port = 8;

    pcap_t p;
    p.priv = &ph;
    char initial_buf[PCAP_ERRBUF_SIZE];
    std::strcpy(initial_buf, "initial");
    std::strcpy(p.errbuf, initial_buf);

    // Act
    int rc = pcap_device_set_filter(&p, nullptr, 3);

    // Assert
    ASSERT_EQ(rc, PCAP_ERROR);
    ASSERT_EQ(g_last_dev, 7);
    ASSERT_EQ(g_last_rcv, 8);
    ASSERT_EQ(g_last_msg, MACH_MSG_TYPE_MAKE_SEND);
    ASSERT_EQ(g_last_count, 3);
    // Error buffer should contain the message we format in the mock
    ASSERT_STR_CONTAINS(p.errbuf, "device_set_filter");
}

/*
 * Test 3: Boundary case with zero filter_count
 * - Ensure that the function handles zero filter_count without crashing and passes it through.
 */
static void test_zero_filter_count()
{
    // Arrange
    g_next_kr = 0; // success
    g_last_dev = g_last_rcv = g_last_msg = g_last_count = -1;

    pcap_hurd ph;
    ph.mach_dev = 42;
    ph.rcv_port = 24;

    pcap_t p;
    p.priv = &ph;
    std::strcpy(p.errbuf, "");

    // Act
    int rc = pcap_device_set_filter(&p, nullptr, 0);

    // Assert
    ASSERT_EQ(rc, 0);
    ASSERT_EQ(g_last_dev, 42);
    ASSERT_EQ(g_last_rcv, 24);
    ASSERT_EQ(g_last_msg, MACH_MSG_TYPE_MAKE_SEND);
    ASSERT_EQ(g_last_count, 0);
    // On success, errbuf should remain unchanged (empty in this test)
    ASSERT_STR_EQ(p.errbuf, "");
}

// -------------------------
// Test runner (no GTest)
// -------------------------

int main()
{
    // Run tests
    test_success_path();
    test_failure_path();
    test_zero_filter_count();

    if (g_tests_failed == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_tests_failed << " test(s) failed." << std::endl;
        return g_tests_failed;
    }
}