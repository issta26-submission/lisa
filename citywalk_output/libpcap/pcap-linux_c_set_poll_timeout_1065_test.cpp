// Test suite for the focal method: set_poll_timeout(struct pcap_linux *handlep)
// This test suite is written in C++11, builds on top of the project headers,
// and does not rely on GoogleTest. It uses a lightweight in-process test harness
// that continues execution after assertion failures (non-terminating checks).
//
// Structure and approach:
// - Use the actual function prototype: extern "C" void set_poll_timeout(struct pcap_linux *handlep);
// - Reuse the real struct pcap_linux from pcap-int.h (or equivalent project header).
// - Cover the three main branches of the outer conditional (timeout == 0, timeout > 0, timeout < 0).
// - For the HAVE_TPACKET3 path, provide tests under an #ifdef HAVE_TPACKET3 to allow
//   separate builds that enable/disable TPACKET_V3-specific behavior.
// - Assertions do not terminate the test run; they record failures and continue.
// - Provide explanatory comments for each test to clarify intent and expected behavior.
//
// Notes:
// - The exact semantics for the TPACKET_V3/broken_tpacket_v3 path depend on
//   the underlying has_broken_tpacket_v3() implementation in the C file.
// - Tests assume the project is Linux-oriented and that the relevant headers
//   (e.g., pcap-int.h) are available in the include path.

#include <netinet/in.h>
#include <pcap/vlan.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/if_ether.h>
#include <netlink/msg.h>
#include <iostream>
#include <sys/ioctl.h>
#include <errno.h>
#include <iomanip>
#include <cassert>
#include <pcap-snf.h>
#include <pcap/can_socketcan.h>
#include <pcap-util.h>
#include <dirent.h>
#include <netlink/genl/genl.h>
#include <linux/netlink.h>
#include <fcntl.h>
#include <string>
#include <cstring>
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


// Forward declarations of C API used by the focal method.
extern "C" {
void set_poll_timeout(struct pcap_linux *handlep);
}

// Lightweight test harness: collects failures but continues execution.
static int g_failures = 0;

#define TEST_LOG(msg) do { std::cout << "[TEST] " << msg << "\n"; } while (0)
#define FAIL(msg) do { \
    std::cerr << "FAIL: " << msg << "\n"; \
    ++g_failures; \
} while (0)
#define PASS(msg) do { \
    std::cout << "PASS: " << msg << "\n"; \
} while (0)
#define CHECK_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        FailWithDetail(msg, a, b, #a, #b, __FILE__, __LINE__); \
    } \
} while (0)
#define CHECK_TRUE(cond, msg) do { \
    if (!(cond)) { FailWithDetail(msg, cond, true, "cond", "true", __FILE__, __LINE__); } \
} while (0)
#define CHECK_OK(cond, msg) CHECK_TRUE(cond, msg)

// Helper to print detailed mismatch information without terminating the test.
static void FailWithDetail(const std::string& base_msg,
                           long long actual, long long expected,
                           const char* actual_expr, const char* expected_expr,
                           const char* file, int line)
{
    std::cerr << "ERROR at " << file << ":" << line
              << " - " << base_msg << "\n"
              << "  Expected (" << expected_expr << ") = " << expected
              << " but got (" << actual_expr << ") = " << actual << "\n";
    ++g_failures;
}

// Utility to safely initialize a pcap_linux handle (via its public members).
static void InitHandleForTest(struct pcap_linux *handle)
{
    // Ensure the structure is zeroed to avoid uninitialized fields influencing tests.
    std::memset(handle, 0, sizeof(*handle));

    // Leave fields at sane defaults; test cases will set only the necessary ones.
    // We intentionally do not set poll_timeout here; set_poll_timeout will modify it.
}

// Test 1: Non-TPACKET3 path, timeout == 0 -> poll_timeout should be -1 (block forever)
void test_set_poll_timeout_timeout_zero_no_tpacket3()
{
    TEST_LOG("test_set_poll_timeout_timeout_zero_no_tpacket3: Verify timeout==0 without HAVE_TPACKET3 => poll_timeout == -1");

    struct pcap_linux h;
    InitHandleForTest(&h);

    h.timeout = 0;
    // Ensure no TP version semantics apply when HAVE_TPACKET3 is not defined.
    #ifndef HAVE_TPACKET3
    set_poll_timeout(&h);
    CHECK_EQ(h.poll_timeout, -1, "poll_timeout should be -1 when timeout==0 and no TPACKET3 support");
    #else
    // If built with HAVE_TPACKET3, this test is not applicable; skip gracefully.
    PASS("Skipped test_set_poll_timeout_timeout_zero_no_tpacket3 (HAVE_TPACKET3 defined).");
    #endif
}

// Test 2: Non-TPACKET3 path with timeout > 0
// Verifies that poll_timeout equals the requested timeout.
void test_set_poll_timeout_timeout_positive_no_tpacket3()
{
    TEST_LOG("test_set_poll_timeout_timeout_positive_no_tpacket3: timeout > 0 without HAVE_TPACKET3 -> poll_timeout == timeout");

    struct pcap_linux h;
    InitHandleForTest(&h);

    h.timeout = 5;

    #ifndef HAVE_TPACKET3
    set_poll_timeout(&h);
    CHECK_EQ(h.poll_timeout, 5, "poll_timeout should equal timeout when timeout>0 and no TPACKET3");
    #else
    // If built with HAVE_TPACKET3, this test is not applicable in this variant.
    PASS("Skipped test_set_poll_timeout_timeout_positive_no_tpacket3 (HAVE_TPACKET3 defined).");
    #endif
}

// Test 3: Non-blocking mode (timeout < 0)
void test_set_poll_timeout_non_blocking_no_tpacket3()
{
    TEST_LOG("test_set_poll_timeout_non_blocking_no_tpacket3: timeout < 0 -> poll_timeout == 0 (non-blocking)");

    struct pcap_linux h;
    InitHandleForTest(&h);

    h.timeout = -1;

    #ifndef HAVE_TPACKET3
    set_poll_timeout(&h);
    CHECK_EQ(h.poll_timeout, 0, "poll_timeout should be 0 for non-blocking mode (timeout < 0) without TPACKET3");
    #else
    PASS("Skipped test_set_poll_timeout_non_blocking_no_tpacket3 (HAVE_TPACKET3 defined).");
    #endif
}

// Test 4: TIMEOUT == 0 with HAVE_TPACKET3 defined
// We cannot deterministically force broken_tpacket_v3, but we can verify
// that the code sets poll_timeout to either 1 (special case) or -1 (default path).
void test_set_poll_timeout_timeout_zero_with_tpacket3()
{
    #ifdef HAVE_TPACKET3
    TEST_LOG("test_set_poll_timeout_timeout_zero_with_tpacket3: timeout==0 with TPACKET_V3 -> poll_timeout in {1, -1}");

    struct pcap_linux h;
    InitHandleForTest(&h);

    h.timeout = 0;
    h.tp_version = TPACKET_V3; // Force TPACKET_V3 path
    set_poll_timeout(&h);

    if (h.poll_timeout == 1 || h.poll_timeout == -1) {
        PASS("poll_timeout is either 1 or -1 as expected for tp3 with timeout==0.");
    } else {
        FAIL("poll_timeout should be either 1 or -1 for tp3 timeout==0; got unexpected value.");
    }
    #else
    PASS("Skipped test_set_poll_timeout_timeout_zero_with_tpacket3 (HAVE_TPACKET3 not defined).");
    #endif
}

// Test 5: TIMEOUT > 0 with HAVE_TPACKET3 defined
// Verifies that poll_timeout is either timeout or -1 depending on broken_tpacket_v3 state.
void test_set_poll_timeout_timeout_positive_with_tpacket3()
{
    #ifdef HAVE_TPACKET3
    TEST_LOG("test_set_poll_timeout_timeout_positive_with_tpacket3: timeout>0 with tp3 -> poll_timeout in {timeout, -1}");

    struct pcap_linux h;
    InitHandleForTest(&h);

    h.timeout = 7;
    h.tp_version = TPACKET_V3; // Force TPACKET_V3 path
    set_poll_timeout(&h);

    if (h.poll_timeout == h.timeout || h.poll_timeout == -1) {
        PASS("poll_timeout is either timeout or -1 for tp3 with timeout>0 (depending on broken_tpacket_v3).");
    } else {
        FAIL("poll_timeout should be either timeout or -1 for tp3 with timeout>0; got unexpected value.");
    }
    #else
    PASS("Skipped test_set_poll_timeout_timeout_positive_with_tpacket3 (HAVE_TPACKET3 not defined).");
    #endif
}

// Helper to run all tests
static void RunAllTests()
{
    g_failures = 0;

    test_set_poll_timeout_timeout_zero_no_tpacket3();
    test_set_poll_timeout_timeout_positive_no_tpacket3();
    test_set_poll_timeout_non_blocking_no_tpacket3();

    test_set_poll_timeout_timeout_zero_with_tpacket3();
    test_set_poll_timeout_timeout_positive_with_tpacket3();

    if (g_failures == 0) {
        std::cout << "[RESULT] All tests passed.\n";
    } else {
        std::cerr << "[RESULT] " << g_failures << " test(s) failed.\n";
    }
}

// Entry point
int main()
{
    RunAllTests();
    return (g_failures == 0) ? 0 : 1;
}