// Test suite for dlt_to_linktype in pcap-common.c
// This test harness uses a lightweight, non-GTest approach suitable for C++11.
// It exercises multiple branches of the dlt_to_linktype function by leveraging
// the platform-dependent DLT_* and LINKTYPE_* macros.
// The tests are designed to compile and run in environments where the pcap
// headers provide the expected macros. Where macros are not defined on a
// given platform, the corresponding tests are skipped via preprocessor guards.

#include <pcap-common.h>
#include <cstdio>
#include <config.h>
#include <pcap-int.h>
#include <iostream>
#include <pcap-types.h>


// Expose the C function under test
extern "C" int dlt_to_linktype(int);

// Lightweight test harness (non-terminating assertions)
static int g_total_asserts = 0;
static int g_failed_asserts = 0;

#define EXPECT_EQ(a, b) do {                           \
    ++g_total_asserts;                                   \
    if ((a) != (b)) {                                    \
        std::cerr << "EXPECT_EQ FAILED: " #a " (" << (a) << ") != " #b " (" << (b) \
                  << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failed_asserts;                              \
    }                                                     \
} while (0)

#define EXPECT_TRUE(cond) do {                          \
    ++g_total_asserts;                                   \
    if (!(cond)) {                                       \
        std::cerr << "EXPECT_TRUE FAILED: " #cond " at " << __FILE__ << ":" << __LINE__ \
                  << std::endl;                          \
        ++g_failed_asserts;                              \
    }                                                    \
} while (0)

// Test 1: Values in the low matching range should map to themselves
static void test_low_matching_range() {
#ifdef DLT_LOW_MATCHING_MIN
    int start = DLT_LOW_MATCHING_MIN;
    int end = DLT_LOW_MATCHING_MAX;
    for (int v = start; v <= end; ++v) {
        int res = dlt_to_linktype(v);
        EXPECT_EQ(res, v);
    }
#else
    // Test not applicable on this platform; skip with no effect.
    (void)0;
#endif
}

// Test 2: Values in the high matching range should map to themselves
static void test_high_matching_range() {
#ifdef DLT_HIGH_MATCHING_MIN
    int start = DLT_HIGH_MATCHING_MIN;
    int end = DLT_HIGH_MATCHING_MAX;
    for (int v = start; v <= end; ++v) {
        int res = dlt_to_linktype(v);
        EXPECT_EQ(res, v);
    }
#else
    // Test not applicable on this platform; skip with no effect.
    (void)0;
#endif
}

// Test 3: DLT_PFSYNC special case (if macro values differ, map to LINKTYPE_PFSYNC)
static void test_pfsync_mapping() {
#if defined(DLT_PFSYNC) && defined(LINKTYPE_PFSYNC)
#if DLT_PFSYNC != LINKTYPE_PFSYNC
    int res = dlt_to_linktype(DLT_PFSYNC);
    EXPECT_EQ(res, LINKTYPE_PFSYNC);
#endif
#else
    // Skip if not applicable
    (void)0;
#endif
}

// Test 4: DLT_PKTAP special case (if macro values differ, map to LINKTYPE_PKTAP)
static void test_pktap_mapping() {
#if defined(DLT_PKTAP) && defined(LINKTYPE_PKTAP)
#if DLT_PKTAP != LINKTYPE_PKTAP
    int res = dlt_to_linktype(DLT_PKTAP);
    EXPECT_EQ(res, LINKTYPE_PKTAP);
#endif
#else
    // Skip if not applicable
    (void)0;
#endif
}

// Test 5: DLT_ATM_RFC1483 maps to LINKTYPE_ATM_RFC1483
static void test_atm_rfc1483_mapping() {
#ifdef DLT_ATM_RFC1483
    int res = dlt_to_linktype(DLT_ATM_RFC1483);
    EXPECT_EQ(res, LINKTYPE_ATM_RFC1483);
#else
    (void)0;
#endif
}

// Test 6: DLT_RAW maps to LINKTYPE_RAW
static void test_raw_mapping() {
#ifdef DLT_RAW
    int res = dlt_to_linktype(DLT_RAW);
    EXPECT_EQ(res, LINKTYPE_RAW);
#else
    (void)0;
#endif
}

// Test 7: DLT_SLIP_BSDOS maps to LINKTYPE_SLIP_BSDOS
static void test_slip_bsdos_mapping() {
#ifdef DLT_SLIP_BSDOS
    int res = dlt_to_linktype(DLT_SLIP_BSDOS);
    EXPECT_EQ(res, LINKTYPE_SLIP_BSDOS);
#else
    (void)0;
#endif
}

// Test 8: DLT_PPP_BSDOS maps to LINKTYPE_PPP_BSDOS
static void test_ppp_bsdos_mapping() {
#ifdef DLT_PPP_BSDOS
    int res = dlt_to_linktype(DLT_PPP_BSDOS);
    EXPECT_EQ(res, LINKTYPE_PPP_BSDOS);
#else
    (void)0;
#endif
}

// Test 9: DLT_FR (BSD/OS Frame Relay) maps to LINKTYPE_FRELAY
static void test_frelay_mapping() {
#ifdef DLT_FR
    int res = dlt_to_linktype(DLT_FR);
    EXPECT_EQ(res, LINKTYPE_FRELAY);
#else
    (void)0;
#endif
}

// Test 10: Platform-dependent HDLC mapping to NETBSD_HDLC
static void test_hdlc_mapping() {
#if defined(DLT_HDLC) && defined(LINKTYPE_NETBSD_HDLC)
#if DLT_HDLC != LINKTYPE_NETBSD_HDLC
    int res = dlt_to_linktype(DLT_HDLC);
    EXPECT_EQ(res, LINKTYPE_NETBSD_HDLC);
#else
    // If equal, compiled branch not taken; skip
    (void)0;
#endif
#else
    (void)0;
#endif
}

// Test 11: Platform-dependent C_HDLC mapping
static void test_c_hdlc_mapping() {
#if defined(DLT_C_HDLC) && defined(LINKTYPE_C_HDLC)
#if DLT_C_HDLC != LINKTYPE_C_HDLC
    int res = dlt_to_linktype(DLT_C_HDLC);
    EXPECT_EQ(res, LINKTYPE_C_HDLC);
#else
    (void)0;
#endif
#else
    (void)0;
#endif
}

// Test 12: DLT_LOOP maps to LINKTYPE_LOOP (OpenBSD HDLC - conditional)
static void test_loop_mapping() {
#if defined(DLT_LOOP) && defined(LINKTYPE_LOOP)
#if DLT_LOOP != LINKTYPE_LOOP
    int res = dlt_to_linktype(DLT_LOOP);
    EXPECT_EQ(res, LINKTYPE_LOOP);
#else
    (void)0;
#endif
#else
    (void)0;
#endif
}

// Test 13: DLT_ENC maps to LINKTYPE_ENC (OpenBSD ENC)
static void test_enc_mapping() {
#if defined(DLT_ENC) && defined(LINKTYPE_ENC)
#if DLT_ENC != LINKTYPE_ENC
    int res = dlt_to_linktype(DLT_ENC);
    EXPECT_EQ(res, LINKTYPE_ENC);
#else
    (void)0;
#endif
#else
    (void)0;
#endif
}

// Test 14: DLT_ATM_CLIP maps to LINKTYPE_ATM_CLIP
static void test_atm_clip_mapping() {
#ifdef DLT_ATM_CLIP
    int res = dlt_to_linktype(DLT_ATM_CLIP);
    EXPECT_EQ(res, LINKTYPE_ATM_CLIP);
#else
    (void)0;
#endif
}

// Test 15: Non-matching oddball DLTs map to themselves
static void test_other_oddball_mappings() {
#ifdef DLT_REDBACK_SMARTEDGE
    int res = dlt_to_linktype(DLT_REDBACK_SMARTEDGE);
    EXPECT_EQ(res, DLT_REDBACK_SMARTEDGE);
#else
    (void)0;
#endif

#ifdef DLT_PPP_SERIAL
    int res2 = dlt_to_linktype(DLT_PPP_SERIAL);
    EXPECT_EQ(res2, DLT_PPP_SERIAL);
#else
    (void)0;
#endif

#ifdef DLT_PPP_ETHER
    int res3 = dlt_to_linktype(DLT_PPP_ETHER);
    EXPECT_EQ(res3, DLT_PPP_ETHER);
#else
    (void)0;
#endif

#ifdef DLT_SYMANTEC_FIREWALL
    int res4 = dlt_to_linktype(DLT_SYMANTEC_FIREWALL);
    EXPECT_EQ(res4, DLT_SYMANTEC_FIREWALL);
#else
    (void)0;
#endif
}

// Test 16: Unknown DLT value returns -1 (error)
static void test_unknown_value_returns_minus_one() {
    int res = dlt_to_linktype(-1);
    EXPECT_EQ(res, -1);

    // Also test a large arbitrary value unlikely to map
    res = dlt_to_linktype(1234567);
    EXPECT_EQ(res, -1);
}

// Run all tests and report results
static void run_all_tests() {
    std::cout << "Starting tests for dlt_to_linktype...\n";

    test_low_matching_range();
    test_high_matching_range();
    test_pfsync_mapping();
    test_pktap_mapping();
    test_atm_rfc1483_mapping();
    test_raw_mapping();
    test_slip_bsdos_mapping();
    test_ppp_bsdos_mapping();
    test_frelay_mapping();
    test_hdlc_mapping();
    test_c_hdlc_mapping();
    test_loop_mapping();
    test_enc_mapping();
    test_atm_clip_mapping();
    test_other_oddball_mappings();
    test_unknown_value_returns_minus_one();

    std::cout << "Test run complete. Total assertions: "
              << g_total_asserts << ", Failures: " << g_failed_asserts << std::endl;
}

int main() {
    run_all_tests();
    // Return non-zero if any test failed to aid automated scripts
    return (g_failed_asserts == 0) ? 0 : 1;
}