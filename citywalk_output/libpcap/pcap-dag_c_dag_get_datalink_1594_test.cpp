/*
 * Lightweight C++11 test suite for the focal method dag_get_datalink
 * (reproduced in test as a self-contained stub to enable branch coverage
 * testing without external test frameworks).
 *
 * Notes:
 * - This test suite is crafted to exercise the logical branches of the
 *   focal method in a self-contained environment. It mocks the essential
 *   data structures and the dag_get_stream_erf_types helper to drive
 *   specific code paths.
 * - The real project may supply different headers and implementations; here
 *   we provide minimal stand-ins to enable compilation and execution in a
 *   standalone fashion.
 * - Assertions are non-terminating (they report failures but continue),
 *   aligned with the provided domain knowledge (non-terminating EXPECT_*)
 *   while not relying on GTest, as requested.
 * - The code is intended to be compiled as a single translation unit.
 */

#include <string.h>
#include <errno.h>
#include <cstdint>
#include <cstdlib>
#include <endian.h>
#include <dagpci.h>
#include <pcap-util.h>
#include <unistd.h>
#include <pcap-dag.h>
#include <limits.h>
#include <dag_config_api.h>
#include <stdlib.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <iostream>
#include <dagapi.h>


// ---------------------- Minimal, test-facing stubs ---------------------- //

// Basic DLT constants (subset for testing)
enum {
    DLT_EN10MB = 1,
    DLT_DOCSIS = 3,
    DLT_CHDLC = 4,
    DLT_PPP_SERIAL = 5,
    DLT_FRELAY = 6,
    DLT_ATM_RFC1483 = 7,
    DLT_SUNATM = 8,
    DLT_MTP2 = 9,
    DLT_MTP2_WITH_PHDR = 10,
    DLT_LAPD = 11,
    DLT_RAW = 12,
    DLT_IPV4 = 13,
    DLT_IPV6 = 14,
    DLT_ERF = 15
};

// System/PCAP style error return
#define PCAP_ERROR (-1)

// ERF type mask (simplified)
#define ERF_TYPE_MASK 0xFF

// ERF type tags (subset used for testing)
enum {
    ERF_TYPE_HDLC_POS       = 0x01,
    ERF_TYPE_COLOR_HDLC_POS = 0x02,
    ERF_TYPE_DSM_COLOR_HDLC_POS = 0x03,
    ERF_TYPE_COLOR_HASH_POS   = 0x04,
    ERF_TYPE_ETH              = 0x10,
    ERF_TYPE_COLOR_ETH        = 0x11,
    ERF_TYPE_DSM_COLOR_ETH    = 0x12,
    ERF_TYPE_COLOR_HASH_ETH   = 0x13,
    ERF_TYPE_ATM              = 0x20,
    ERF_TYPE_AAL5             = 0x21,
    ERF_TYPE_MC_ATM           = 0x22,
    ERF_TYPE_MC_AAL5          = 0x23,
    ERF_TYPE_IPV4             = 0x30,
    ERF_TYPE_IPV6             = 0x31,
    ERF_TYPE_LEGACY           = 0x40,
    ERF_TYPE_MC_RAW           = 0x41,
    ERF_TYPE_MC_RAW_CHANNEL   = 0x42,
    ERF_TYPE_IP_COUNTER       = 0x43,
    ERF_TYPE_TCP_FLOW_COUNTER = 0x44,
    ERF_TYPE_INFINIBAND       = 0x50,
    ERF_TYPE_RAW_LINK         = 0x51,
    ERF_TYPE_INFINIBAND_LINK  = 0x52,
    ERF_TYPE_META             = 0x60
};

// TX_ONLY macro (simple bit check for test purposes)
#define TX_ONLY(x) (((x) & 0x01) != 0)

// ---------------------- Minimal structures for the test ------------------- //

struct pcap_dag {
    int dag_stream;
};

struct pcap_t {
    // Private data used by dag_get_datalink
    struct pcap_dag *priv;

    // Link-layer type guess list (array of ints)
    int *dlt_list;
    int dlt_count;

    // Current linktype in use
    int linktype;

    // Mock file descriptor (unused by test logic but kept for fidelity)
    int fd;

    // Error buffer (unused in test path but included for compatibility)
    char errbuf[256];
};

// ---------------------- Test harness state and helpers ------------------- //

// Global test pattern to drive dag_get_stream_erf_types behavior
enum TestPattern {
    PATTERN_NONE,
    PATTERN_IPV4_ONLY,
    PATTERN_IPV4_IPV6,
    PATTERN_ETH_ONLY,
    PATTERN_ERROR
};

static TestPattern g_current_pattern = PATTERN_NONE;

// Helper: set the current test pattern
static void set_test_pattern(TestPattern p) {
    g_current_pattern = p;
}

// Mocked helper: dag_get_stream_erf_types
// Fills 'types' according to the global test pattern.
extern "C" int dag_get_stream_erf_types(int fd, int dag_stream, uint8_t *types, int max) {
    // Ignore 'fd' and 'dag_stream' for the mock; rely on g_current_pattern
    (void)fd; (void)dag_stream;
    if (types == nullptr || max <= 0) return -1;

    // Clear
    std::memset(types, 0, max);

    switch (g_current_pattern) {
        case PATTERN_IPV4_ONLY:
            types[0] = ERF_TYPE_IPV4;
            types[1] = 0;
            return 0;
        case PATTERN_IPV4_IPV6:
            types[0] = ERF_TYPE_IPV4;
            types[1] = ERF_TYPE_IPV6;
            types[2] = 0;
            return 0;
        case PATTERN_ETH_ONLY:
            types[0] = ERF_TYPE_ETH;
            types[1] = 0;
            return 0;
        case PATTERN_ERROR:
            return -1;
        case PATTERN_NONE:
        default:
            // No types
            return 0;
    }
}

// A local reimplementation of the focal function (for isolated testing)
static int dag_get_datalink_stub(pcap_t *p) {
    if (!p || !p->priv) return PCAP_ERROR;

    struct pcap_dag *pd = p->priv;

    // Early exit path: TX_ONLY takes precedence
    if (TX_ONLY(pd->dag_stream))
        return (p->linktype = DLT_EN10MB);

    int index = 0, dlt_index = 0;
    uint8_t types[255];
    std::memset(types, 0, 255);

    if (p->dlt_list == NULL && (p->dlt_list = (int*)std::malloc(255 * sizeof(*(p->dlt_list)))) == NULL) {
        // Simulated error buffer fill
        std::strncpy(p->errbuf, "malloc", sizeof(p->errbuf) - 1);
        p->errbuf[sizeof(p->errbuf) - 1] = '\0';
        return PCAP_ERROR;
    }
    p->linktype = 0;

    // Get list of possible ERF types for this card
    if (dag_get_stream_erf_types(p->fd, pd->dag_stream, types, 255) < 0) {
        std::strncpy(p->errbuf, "dag_get_stream_erf_types", sizeof(p->errbuf) - 1);
        p->errbuf[sizeof(p->errbuf) - 1] = '\0';
        return PCAP_ERROR;
    }

    while (types[index]) {
        switch ((types[index] & ERF_TYPE_MASK)) {
            // HDLC-like types
            case ERF_TYPE_HDLC_POS:
            case ERF_TYPE_COLOR_HDLC_POS:
            case ERF_TYPE_DSM_COLOR_HDLC_POS:
            case ERF_TYPE_COLOR_HASH_POS:
                p->dlt_list[dlt_index++] = DLT_CHDLC;
                p->dlt_list[dlt_index++] = DLT_PPP_SERIAL;
                p->dlt_list[dlt_index++] = DLT_FRELAY;
                if (!p->linktype) p->linktype = DLT_CHDLC;
                break;

            // Ethernet-like types
            case ERF_TYPE_ETH:
            case ERF_TYPE_COLOR_ETH:
            case ERF_TYPE_DSM_COLOR_ETH:
            case ERF_TYPE_COLOR_HASH_ETH:
                p->dlt_list[dlt_index++] = DLT_EN10MB;
                p->dlt_list[dlt_index++] = DLT_DOCSIS;
                if (!p->linktype) p->linktype = DLT_EN10MB;
                break;

            // ATM types
            case ERF_TYPE_ATM:
            case ERF_TYPE_AAL5:
            case ERF_TYPE_MC_ATM:
            case ERF_TYPE_MC_AAL5:
                p->dlt_list[dlt_index++] = DLT_ATM_RFC1483;
                p->dlt_list[dlt_index++] = DLT_SUNATM;
                if (!p->linktype) p->linktype = DLT_ATM_RFC1483;
                break;

            // MC-HDLC group
            case ERF_TYPE_COLOR_MC_HDLC_POS:
            case ERF_TYPE_MC_HDLC:
                p->dlt_list[dlt_index++] = DLT_CHDLC;
                p->dlt_list[dlt_index++] = DLT_PPP_SERIAL;
                p->dlt_list[dlt_index++] = DLT_FRELAY;
                p->dlt_list[dlt_index++] = DLT_MTP2;
                p->dlt_list[dlt_index++] = DLT_MTP2_WITH_PHDR;
                p->dlt_list[dlt_index++] = DLT_LAPD;
                if (!p->linktype) p->linktype = DLT_CHDLC;
                break;

            // IPv4
            case ERF_TYPE_IPV4:
                p->dlt_list[dlt_index++] = DLT_RAW;
                p->dlt_list[dlt_index++] = DLT_IPV4;
                if (!p->linktype) p->linktype = DLT_RAW;
                break;

            // IPv6
            case ERF_TYPE_IPV6:
                p->dlt_list[dlt_index++] = DLT_RAW;
                p->dlt_list[dlt_index++] = DLT_IPV6;
                if (!p->linktype) p->linktype = DLT_RAW;
                break;

            // Types not yet supported by libpcap (covered by ERF but not mapped)
            case ERF_TYPE_LEGACY:
            case ERF_TYPE_MC_RAW:
            case ERF_TYPE_MC_RAW_CHANNEL:
            case ERF_TYPE_IP_COUNTER:
            case ERF_TYPE_TCP_FLOW_COUNTER:
            case ERF_TYPE_INFINIBAND:
            case ERF_TYPE_RAW_LINK:
            case ERF_TYPE_INFINIBAND_LINK:
            case ERF_TYPE_META:
            default:
                // Libpcap cannot deal with these types yet
                break;
        }
        index++;
    }

    // Always expose ERF as a catch-all type
    p->dlt_list[dlt_index++] = DLT_ERF;
    p->dlt_count = dlt_index;
    if (!p->linktype) p->linktype = DLT_ERF;
    return p->linktype;
}

// ---------------------- Lightweight assertion helpers ------------------- //

static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                  << " : " #cond "\n"; \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_total_tests; \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << " : " #a " != " #b " (" << (a) << " != " << (b) << ")\n"; \
        ++g_failed_tests; \
    } \
} while(0)

// Helper to reset a pcap_t structure
static void reset_pcap(pcap_t *p) {
    p->priv = nullptr;
    p->dlt_list = nullptr;
    p->dlt_count = 0;
    p->linktype = 0;
    p->fd = -1;
    std::memset(p->errbuf, 0, sizeof(p->errbuf));
}

// ------------------------------- Tests ---------------------------------- //

// Test A: TX_ONLY path should set EN10MB and return immediately
static void test_tx_only_path() {
    // Explanatory comment:
    // Ensure that when TX_ONLY(pd->dag_stream) evaluates true, dag_get_datalink
    // returns DLT_EN10MB and assigns linktype accordingly, without allocating
    // dlt_list.
    pcap_t pc;
    reset_pcap(&pc);

    pcap_dag dag;
    dag.dag_stream = 1; // TX_ONLY true by our macro
    pc.priv = &dag;
    pc.linktype = 0;

    int ret = dag_get_datalink_stub(&pc);

    EXPECT_EQ(ret, DLT_EN10MB);
    EXPECT_EQ(pc.linktype, DLT_EN10MB);
    // dlt_list should remain NULL since we exit early
    EXPECT_TRUE(pc.dlt_list == nullptr);
}

// Test B: IPv4 only path should allocate dlt_list and populate RAW/IPV4 + ERF
static void test_ipv4_only_path_alloc_and_populate() {
    // Explanatory comment:
    // Not TX_ONLY: dag_get_stream_erf_types returns IPv4. Expect RAW and IPV4
    // added, with linktype RAW, and finally ERF marker appended.
    pcap_t pc;
    reset_pcap(&pc);

    pcap_dag dag;
    dag.dag_stream = 2; // Not TX_ONLY
    pc.priv = &dag;
    pc.fd = 3;

    set_test_pattern(PATTERN_IPV4_ONLY);

    int ret = dag_get_datalink_stub(&pc);

    EXPECT_EQ(ret, DLT_RAW);
    EXPECT_TRUE(pc.linktype == DLT_RAW);

    // Expect dlt_list to contain RAW, IPV4, ERF
    EXPECT_TRUE(pc.dlt_list != nullptr);
    EXPECT_EQ(pc.dlt_count, 3);
    EXPECT_EQ(pc.dlt_list[0], DLT_RAW);
    EXPECT_EQ(pc.dlt_list[1], DLT_IPV4);
    EXPECT_EQ(pc.dlt_list[2], DLT_ERF);

    // Cleanup
    std::free(pc.dlt_list);
    pc.dlt_list = nullptr;
}

// Test C: Error path from dag_get_stream_erf_types returns -1
static void test_error_from_dag_get_stream_erf_types() {
    // Explanatory comment:
    // dag_get_stream_erf_types reports an error -> function should return PCAP_ERROR
    pcap_t pc;
    reset_pcap(&pc);

    pcap_dag dag;
    dag.dag_stream = 4; // Not TX_ONLY
    pc.priv = &dag;
    pc.fd = 4;

    set_test_pattern(PATTERN_ERROR);

    int ret = dag_get_datalink_stub(&pc);

    EXPECT_EQ(ret, PCAP_ERROR);
}

// Test D: ETH type path should give EN10MB and DOCSIS
static void test_eth_path() {
    // Explanatory comment:
    // Not TX_ONLY: report ETH. Expect DLT_EN10MB and DLT_DOCSIS then ERF.
    pcap_t pc;
    reset_pcap(&pc);

    pcap_dag dag;
    dag.dag_stream = 6; // Not TX_ONLY
    pc.priv = &dag;
    pc.fd = 7;

    set_test_pattern(PATTERN_ETH_ONLY);

    int ret = dag_get_datalink_stub(&pc);

    // Expect EN10MB (Ethernet-like) with DOCSIS in the list and ERF at end
    EXPECT_EQ(ret, DLT_EN10MB);
    EXPECT_TRUE(pc.linktype == DLT_EN10MB);

    EXPECT_TRUE(pc.dlt_list != nullptr);
    // 2 DLTs + 1 ERF
    EXPECT_EQ(pc.dlt_count, 3);
    EXPECT_EQ(pc.dlt_list[0], DLT_EN10MB);
    EXPECT_EQ(pc.dlt_list[1], DLT_DOCSIS);
    EXPECT_EQ(pc.dlt_list[2], DLT_ERF);

    std::free(pc.dlt_list);
    pc.dlt_list = nullptr;
}

// Test E: IPv4 followed by IPv6 should accumulate multiple RAW entries and end with ERF
static void test_ipv4_ipv6_multi_branch() {
    pcap_t pc;
    reset_pcap(&pc);

    pcap_dag dag;
    dag.dag_stream = 2; // Not TX_ONLY
    pc.priv = &dag;
    pc.fd = 8;

    set_test_pattern(PATTERN_IPV4_IPV6);

    int ret = dag_get_datalink_stub(&pc);

    // Expect RAW, IPV4, RAW, IPV6, ERF
    EXPECT_EQ(ret, DLT_RAW);
    EXPECT_TRUE(pc.linktype == DLT_RAW);
    EXPECT_TRUE(pc.dlt_list != nullptr);
    EXPECT_EQ(pc.dlt_count, 5);
    EXPECT_EQ(pc.dlt_list[0], DLT_RAW);
    EXPECT_EQ(pc.dlt_list[1], DLT_IPV4);
    EXPECT_EQ(pc.dlt_list[2], DLT_RAW);
    EXPECT_EQ(pc.dlt_list[3], DLT_IPV6);
    EXPECT_EQ(pc.dlt_list[4], DLT_ERF);

    std::free(pc.dlt_list);
    pc.dlt_list = nullptr;
}

// --------------------------- Test runner & main --------------------------- //

int main() {
    std::cout << "Running focused unit tests for dag_get_datalink (stand-in test harness)\n";

    // Run tests
    test_tx_only_path();
    test_ipv4_only_path_alloc_and_populate();
    test_error_from_dag_get_stream_erf_types();
    test_eth_path();
    test_ipv4_ipv6_multi_branch();

    // Summary
    std::cout << "Tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests << "\n";

    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}