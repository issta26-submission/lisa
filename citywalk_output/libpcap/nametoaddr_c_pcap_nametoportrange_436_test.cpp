/*
Unit Test Suite for pcap_nametoportrange (nametoaddr.c)

Goals:
- Validate true/false branches of pcap_nametoportrange.
- Cover cases with valid numeric ports and proto consistency/mismatch.
- Cover invalid input scenarios (missing dash, non-parsable parts).
- Use non-terminating assertions to maximize code coverage.
- Compile as C++11, linking against the C function nametoaddr.c.

Notes:
- We assume a POSIX-like environment. IPPROTO_TCP is used to verify TCP-related results.
- Proto undefined cases are treated as proto < 0 (PROTO_UNDEF in the source).
*/

#include <string.h>
#include <pcap/namedb.h>
#include <errno.h>
#include <diag-control.h>
#include <nametoaddr.h>
#include <netinet/in.h>
#include <gencode.h>
#include <os-proto.h>
#include <thread-local.h>
#include <stdio.h>
#include <stdlib.h>
#include <ethertype.h>
#include <config.h>
#include <pcap-int.h>
#include <llc.h>


#ifdef __cplusplus
extern "C" {
#endif

/* Provide the function prototype for linkage with the C code under test */
int pcap_nametoportrange(const char *name, int *port1, int *port2, int *proto);

#ifdef __cplusplus
}
#endif


// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_pass(const char* name) {
    printf("PASS: %s\n", name);
}

static void log_fail(const char* name) {
    printf("FAIL: %s\n", name);
    g_failed_tests++;
}

static void summarize() {
    printf("\nTEST SUMMARY: Total=%d, Failed=%d\n", g_total_tests, g_failed_tests);
}

static void test_good_range_80_90() {
    // True path: valid range "80-90" with matching TCP ports
    int port1 = 0, port2 = 0, proto = 0;
    int ret = pcap_nametoportrange("80-90", &port1, &port2, &proto);
    g_total_tests++;
    if (ret == 1 && port1 == 80 && port2 == 90 && proto == IPPROTO_TCP) {
        log_pass("pcap_nametoportrange: good range 80-90 (TCP expected)");
    } else {
        printf("  got ret=%d port1=%d port2=%d proto=%d\n", ret, port1, port2, proto);
        log_fail("pcap_nametoportrange: good range 80-90 (TCP expected)");
    }
}

static void test_good_range_70_70() {
    // True path: valid range "70-70" with same port on both sides
    int port1 = 0, port2 = 0, proto = 0;
    int ret = pcap_nametoportrange("70-70", &port1, &port2, &proto);
    g_total_tests++;
    if (ret == 1 && port1 == 70 && port2 == 70 && proto == IPPROTO_TCP) {
        log_pass("pcap_nametoportrange: good range 70-70 (TCP expected)");
    } else {
        printf("  got ret=%d port1=%d port2=%d proto=%d\n", ret, port1, port2, proto);
        log_fail("pcap_nametoportrange: good range 70-70 (TCP expected)");
    }
}

static void test_bad_no_dash() {
    // Negative path: input without dash should fail
    int port1 = 0, port2 = 0, proto = 0;
    int ret = pcap_nametoportrange("80", &port1, &port2, &proto);
    g_total_tests++;
    if (ret == 0) {
        log_pass("pcap_nametoportrange: invalid no-dash input correctly rejected");
    } else {
        printf("  got ret=%d port1=%d port2=%d proto=%d\n", ret, port1, port2, proto);
        log_fail("pcap_nametoportrange: invalid no-dash input should be 0");
    }
}

static void test_mismatch_proto_53_53() {
    // Proto mismatch scenario: TCP on first, UDP on second for the same numeric port
    // DNS over TCP/UDP both exist; this helps exercise the PROTO_UNDEF path.
    int port1 = 0, port2 = 0, proto = 0;
    int ret = pcap_nametoportrange("53-53", &port1, &port2, &proto);
    g_total_tests++;
    if (ret == 1 && port1 == 53 && port2 == 53 && proto < 0) {
        log_pass("pcap_nametoportrange: proto mismatch yields PROTO_UNDEF (proto<0)");
    } else {
        printf("  got ret=%d port1=%d port2=%d proto=%d\n", ret, port1, port2, proto);
        log_fail("pcap_nametoportrange: proto mismatch case failed");
    }
}

static void test_bad_first_part() {
    // Bad first service name should fail
    int port1 = 0, port2 = 0, proto = 0;
    int ret = pcap_nametoportrange("foobar-80", &port1, &port2, &proto);
    g_total_tests++;
    if (ret == 0) {
        log_pass("pcap_nametoportrange: invalid first part correctly rejected");
    } else {
        printf("  got ret=%d port1=%d port2=%d proto=%d\n", ret, port1, port2, proto);
        log_fail("pcap_nametoportrange: invalid first part should fail");
    }
}

static void test_bad_second_part() {
    // Bad second part should fail
    int port1 = 0, port2 = 0, proto = 0;
    int ret = pcap_nametoportrange("80-foobar", &port1, &port2, &proto);
    g_total_tests++;
    if (ret == 0) {
        log_pass("pcap_nametoportrange: invalid second part correctly rejected");
    } else {
        printf("  got ret=%d port1=%d port2=%d proto=%d\n", ret, port1, port2, proto);
        log_fail("pcap_nametoportrange: invalid second part should fail");
    }
}

int main() {
    // Run all tests
    test_good_range_80_90();
    test_good_range_70_70();
    test_bad_no_dash();
    test_mismatch_proto_53_53();
    test_bad_first_part();
    test_bad_second_part();

    summarize();
    // Return non-zero if any test failed
    if (g_failed_tests > 0)
        return 1;
    return 0;
}