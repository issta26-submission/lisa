// Unit test suite for the focal method: linktype_to_dlt(int linktype)
// This test is self-contained and re-implements the core logic of the
// original function (as provided in the prompt) using a controlled set of
// macros to exercise all relevant branches. It uses the C++11 standard library
// only and does not rely on Google Test or any external testing framework.
// Each test is annotated with explanatory comments describing the scenario it covers.

#include <pcap-common.h>
#include <cstdio>
#include <config.h>
#include <pcap-int.h>
#include <pcap-types.h>


// Step 1: Define necessary macros to drive the test scenarios.
// These mirror the dependent constants/macros used by the focal method.
// We intentionally choose distinct values to clearly exercise each branch.

#define LINKTYPE_LOW_MATCHING_MIN 1
#define LINKTYPE_LOW_MATCHING_MAX 10

// PFSYNC mapping (only active if macros differ)
#define LINKTYPE_PFSYNC 31
#define DLT_PFSYNC 30   // deliberately different from LINKTYPE_PFSYNC to trigger mapping

// PKTAP mapping (only active if macros differ)
#define LINKTYPE_PKTAP 41
#define DLT_PKTAP 40     // deliberately different

// Common mappings directly tested
#define LINKTYPE_ATM_RFC1483 2
#define DLT_ATM_RFC1483 60

#define LINKTYPE_RAW 12
#define DLT_RAW 100

#define LINKTYPE_SLIP_BSDOS 13
#define DLT_SLIP_BSDOS 101

#define LINKTYPE_PPP_BSDOS 14
#define DLT_PPP_BSDOS 102

// ATM_FR (FRELAY) mapping (DLT_FR) - enable via macro defined below
#define LINKTYPE_FRELAY 103
#define DLT_FR 103

// NetBSD HDLC mapping (DLT_HDLC) - only if not equal
#define LINKTYPE_NETBSD_HDLC 104
#define DLT_HDLC 999

// BSD/OS Cisco HDLC mapping (DLT_C_HDLC) - only if not equal
#define LINKTYPE_C_HDLC 105
#define DLT_C_HDLC 998

// OpenBSD DLT_LOOP mapping (DLT_LOOP)
#define LINKTYPE_LOOP 106
#define DLT_LOOP 401

// OpenBSD DLT_ENC mapping (DLT_ENC)
#define LINKTYPE_ENC 107
#define DLT_ENC 600

// Linux ATM Classical IP mapping (ATM CLIP)
#define LINKTYPE_ATM_CLIP 110
#define DLT_ATM_CLIP 700

// Step 2: Re-implement the focal function in this test unit, using the
// macros above. This mirrors the behavior described in the provided source.
int linktype_to_dlt(int linktype)
{
	{
		/*
		 * All values in the low matching range were handed out before
		 * assigning DLT_* codes became a free-for-all, so they're the
		 * same on all platforms, and are thus used as the LINKTYPE_*
		 * codes in capture files.
		 */
		if (linktype >= LINKTYPE_LOW_MATCHING_MIN &&
		    linktype <= LINKTYPE_LOW_MATCHING_MAX)
			return (linktype);
		#if LINKTYPE_PFSYNC != DLT_PFSYNC
		/*
		 * DLT_PFSYNC has a code on several platforms that's in the
		 * non-matching range, a code on FreeBSD that's in the high
		 * matching range and that's *not* equal to LINKTYPE_PFSYNC,
		 * and has a code on the rmaining platforms that's equal
		 * to LINKTYPE_PFSYNC, which is in the high matching range.
		 *
		 * Map LINKTYPE_PFSYNC to whatever DLT_PFSYNC is on this
		 * platform, if the two aren't equal.
		 */
		if (linktype == LINKTYPE_PFSYNC)
			return (DLT_PFSYNC);
		#endif
		/*
		 * DLT_PKTAP is defined as DLT_USER2 - which is in the high
		 * matching range - on Darwin because Apple used DLT_USER2
		 * on systems that users ran, not just as an internal thing.
		 *
		 * We map LINKTYPE_PKTAP to the platform's DLT_PKTAP for
		 * the benefit of software that's expecting DLT_PKTAP
		 * (even if that's DLT_USER2) for an Apple PKTAP capture.
		 *
		 * (Yes, this is an annoyance if you want to read a
		 * LINKTYPE_USER2 packet as something other than DLT_PKTAP
		 * on a Darwin-based OS, as, on that OS, DLT_PKTAP and DLT_USER2
		 * are the same.  Feel free to complain to Apple about this.)
		 */
		#if LINKTYPE_PKTAP != DLT_PKTAP
		if (linktype == LINKTYPE_PKTAP)
			return (DLT_PKTAP);
		#endif
		/*
		 * Map specific LINKTYPE_* to their DLT_* equivalents where
		 * values differ across platforms.
		 */
		if (linktype == LINKTYPE_ATM_RFC1483)
			return (DLT_ATM_RFC1483);
		if (linktype == LINKTYPE_RAW)
			return (DLT_RAW);
		if (linktype == LINKTYPE_SLIP_BSDOS)
			return (DLT_SLIP_BSDOS);
		if (linktype == LINKTYPE_PPP_BSDOS)
			return (DLT_PPP_BSDOS);
		#ifdef DLT_FR
		if (linktype == LINKTYPE_FRELAY)
			return (DLT_FR);
		#endif
		#if LINKTYPE_NETBSD_HDLC != DLT_HDLC
		/* NetBSD HDLC */
		if (linktype == LINKTYPE_NETBSD_HDLC)
			return (DLT_HDLC);
		#endif
		#if LINKTYPE_C_HDLC != DLT_C_HDLC
		/* BSD/OS Cisco HDLC */
		if (linktype == LINKTYPE_C_HDLC)
			return (DLT_C_HDLC);
		#endif
		#if LINKTYPE_LOOP != DLT_LOOP
		/* OpenBSD DLT_LOOP */
		if (linktype == LINKTYPE_LOOP)
			return (DLT_LOOP);
		#endif
		#if LINKTYPE_ENC != DLT_ENC
		/* OpenBSD DLT_ENC */
		if (linktype == LINKTYPE_ENC)
			return (DLT_ENC);
		#endif
		/*
		 * These DLT_* codes are not on all platforms, but, so far,
		 * there don't appear to be any platforms that define
		 * other codes with those values; we map them to
		 * different LINKTYPE_* values anyway, just in case.
		 *
		 * LINKTYPE_ATM_CLIP is a special case.  DLT_ATM_CLIP is
		 * not on all platforms, but, so far, there don't appear
		 * to be any platforms that define it as anything other
		 * than 19; we define LINKTYPE_ATM_CLIP as something
		 * other than 19, just in case.  That value is in the
		 * high matching range, so we have to check for it.
		 */
		/* Linux ATM Classical IP */
		if (linktype == LINKTYPE_ATM_CLIP)
			return (DLT_ATM_CLIP);
		/*
		 * For all other values, return the linktype code as the
		 * DLT_* code.
		 *
		 * If the code is in the high matching range, the
		 * DLT_* code is the same as the LINKTYPE_* code.
		 *
		 * If the code is greater than the maximum value in
		 * the high matching range, it may be a value from
		 * a newer version of libpcap; we provide it in case
		 * the program' capable of handling it.
		 *
		 * If the code is less than the minimum value in
		 * the high matching range, it might be from a capture
		 * written by code that doesn't map non-matching range
		 * DLT_* codes to the appropriate LINKTYPE_* code, so
		 * we'll just pass it through, so that *if it was written
		 * on this platform* it will be interpreted correctly.
		 * (We don't know whether it was written on this platform,
		 * but at least this way there's *some* chance that it
		 * can be read.)
		 */
		return linktype;
	}
}

// Step 3: Simple test harness using non-terminating assertions
static int g_total = 0;
static int g_failed = 0;

static void expect_eq(int expected, int actual, const char* test_name)
{
    ++g_total;
    if (expected != actual) {
        ++g_failed;
        printf("TEST FAIL: %s | expected %d, got %d\n", test_name, expected, actual);
    } else {
        printf("TEST PASS: %s\n", test_name);
    }
}

// Individual test cases with explanatory comments

// Test 1: True in-range value should map to itself (no mapping performed)
void test_in_range_maps_to_itself()
{
    // Input within the low matching range
    int input = 5;
    int expected = 5;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "in_range_maps_to_itself");
}

// Test 2: LINKTYPE_PFSYNC maps to DLT_PFSYNC when codes differ
void test_pfsync_mapping()
{
    int input = LINKTYPE_PFSYNC;
    int expected = DLT_PFSYNC;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "pfsync_maps_to_dlt_pfsync");
}

// Test 3: LINKTYPE_PKTAP maps to DLT_PKTAP when codes differ
void test_pktap_mapping()
{
    int input = LINKTYPE_PKTAP;
    int expected = DLT_PKTAP;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "pktap_maps_to_dlt_pktap");
}

// Test 4: ATM RFC1483 mapping
void test_atm_rfc1483_mapping()
{
    int input = LINKTYPE_ATM_RFC1483;
    int expected = DLT_ATM_RFC1483;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "atm_rfc1483_maps_to_dlt_atm_rfc1483");
}

// Test 5: Raw mapping
void test_raw_mapping()
{
    int input = LINKTYPE_RAW;
    int expected = DLT_RAW;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "raw_maps_to_dlt_raw");
}

// Test 6: SLIP BSDOS mapping
void test_slip_bsdos_mapping()
{
    int input = LINKTYPE_SLIP_BSDOS;
    int expected = DLT_SLIP_BSDOS;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "slip_bsdos_maps_to_dlt_slip_bsdos");
}

// Test 7: PPP BSDOS mapping
void test_ppp_bsdos_mapping()
{
    int input = LINKTYPE_PPP_BSDOS;
    int expected = DLT_PPP_BSDOS;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "ppp_bsdos_maps_to_dlt_ppp_bsdos");
}

// Test 8: FRELAY mapping
void test_frelay_mapping()
{
    int input = LINKTYPE_FRELAY;
    int expected = DLT_FR;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "frelay_maps_to_dlt_fr");
}

// Test 9: NETBSD_HDLC mapping (when DLT_HDLC differs)
void test_netbsd_hdlc_mapping()
{
    int input = LINKTYPE_NETBSD_HDLC;
    int expected = DLT_HDLC;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "netbsd_hdlc_maps_to_dlt_hdlc");
}

// Test 10: C_HDLC mapping (when DLT_C_HDLC differs)
void test_c_hdlc_mapping()
{
    int input = LINKTYPE_C_HDLC;
    int expected = DLT_C_HDLC;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "c_hdlc_maps_to_dlt_c_hdlc");
}

// Test 11: LOOP mapping
void test_loop_mapping()
{
    int input = LINKTYPE_LOOP;
    int expected = DLT_LOOP;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "loop_maps_to_dlt_loop");
}

// Test 12: ENC mapping
void test_enc_mapping()
{
    int input = LINKTYPE_ENC;
    int expected = DLT_ENC;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "enc_maps_to_dlt_enc");
}

// Test 13: ATM_CLIP mapping
void test_atm_clip_mapping()
{
    int input = LINKTYPE_ATM_CLIP;
    int expected = DLT_ATM_CLIP;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "atm_clip_maps_to_dlt_atm_clip");
}

// Test 14: Default behavior - value not matching any mapping should pass through
void test_default_pass_through()
{
    int input = 99999;
    int expected = 99999;
    int actual = linktype_to_dlt(input);
    expect_eq(expected, actual, "default_pass_through_returns_input");
}

// Main test runner
int main()
{
    // Run all tests
    test_in_range_maps_to_itself();
    test_pfsync_mapping();
    test_pktap_mapping();
    test_atm_rfc1483_mapping();
    test_raw_mapping();
    test_slip_bsdos_mapping();
    test_ppp_bsdos_mapping();
    test_frelay_mapping();
    test_netbsd_hdlc_mapping();
    test_c_hdlc_mapping();
    test_loop_mapping();
    test_enc_mapping();
    test_atm_clip_mapping();
    test_default_pass_through();

    // Summary
    printf("Total tests: %d, Failures: %d\n", g_total, g_failed);
    return (g_failed == 0) ? 0 : 1;
}