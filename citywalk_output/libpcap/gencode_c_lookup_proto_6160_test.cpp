// Test suite for the focal method: lookup_proto in gencode.c
// This test suite is self-contained (no GoogleTest) and uses a minimal
// in-house test harness that records failures but continues execution.

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <nametoaddr.h>
#include <ieee80211.h>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <setjmp.h>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <linux/filter.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <linux/types.h>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>
#include <cstring>


// Include the project's interfaces to gain access to necessary types and the focal method.
// The test assumes the project provides gencode.h (or an equivalent header) exposing
// compiler_state_t, struct qual, and the lookup_proto function and related enums/macros.
extern "C" {
}

// Simple non-terminating test harness
static int g_test_failures = 0;

// Helper macro to print test results with location info
#define TEST_PASS(msg) \
    do { std::cout << "[PASS] " << __FILE__ << ":" << __LINE__ << " - " << msg << "\n"; } while(0)

#define TEST_FAIL(msg) \
    do { std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " - " << msg << "\n"; ++g_test_failures; } while(0)

#define TEST_ASSERT_EQUAL(expected, actual, msg) \
    do { if ((expected) == (actual)) { TEST_PASS(msg); } else { TEST_FAIL(msg); } } while(0)

#define TEST_ASSERT_GE0(actual, msg) \
    do { if ((actual) >= 0) { TEST_PASS(msg); } else { TEST_FAIL(msg); } } while(0)

// Test 1: ISO: "esis" -> ISO9542_ESIS
void test_lookup_proto_iso_esis() {
    struct qual q;
    q.proto = Q_ISO;
    q.addr = 0; // addr is not used in this path for "esis"
    int v = lookup_proto(nullptr, "esis", q);
    int expected = ISO9542_ESIS;
    TEST_ASSERT_EQUAL(expected, v, "lookup_proto with Q_ISO and name 'esis' should return ISO9542_ESIS");
}

// Test 2: ISO: "isis" -> ISO10589_ISIS
void test_lookup_proto_iso_isis() {
    struct qual q;
    q.proto = Q_ISO;
    q.addr = 0;
    int v = lookup_proto(nullptr, "isis", q);
    int expected = ISO10589_ISIS;
    TEST_ASSERT_EQUAL(expected, v, "lookup_proto with Q_ISO and name 'isis' should return ISO10589_ISIS");
}

// Test 3: ISO: "clnp" -> ISO8473_CLNP
void test_lookup_proto_iso_clnp() {
    struct qual q;
    q.proto = Q_ISO;
    q.addr = 0;
    int v = lookup_proto(nullptr, "clnp", q);
    int expected = ISO8473_CLNP;
    TEST_ASSERT_EQUAL(expected, v, "lookup_proto with Q_ISO and name 'clnp' should return ISO8473_CLNP");
}

// Test 4: Q_IP path with a common protocol name like "tcp" should yield a valid value
void test_lookup_proto_qip_tcp() {
    struct qual q;
    q.proto = Q_IP;
    q.addr = 0;
    int v = lookup_proto(nullptr, "tcp", q);
    TEST_ASSERT_GE0(v, "lookup_proto with Q_IP and name 'tcp' should yield a non-negative protocol id");
}

// Test 5: Q_IPV6 path with a common protocol name like "tcp" should yield a valid value
void test_lookup_proto_qipv6_tcp() {
    struct qual q;
    q.proto = Q_IPV6;
    q.addr = 0;
    int v = lookup_proto(nullptr, "tcp", q);
    TEST_ASSERT_GE0(v, "lookup_proto with Q_IPV6 and name 'tcp' should yield a non-negative protocol id");
}

// Test 6: Q_DEFAULT path with a common protocol name like "udp" should yield a valid value
void test_lookup_proto_qdefault_udp() {
    struct qual q;
    q.proto = Q_DEFAULT;
    q.addr = 0;
    int v = lookup_proto(nullptr, "udp", q);
    TEST_ASSERT_GE0(v, "lookup_proto with Q_DEFAULT and name 'udp' should yield a non-negative protocol id");
}

// Main function: run all tests and report the overall result
int main() {
    std::cout << "Starting lookup_proto test suite...\n";

    test_lookup_proto_iso_esis();
    test_lookup_proto_iso_isis();
    test_lookup_proto_iso_clnp();
    test_lookup_proto_qip_tcp();
    test_lookup_proto_qipv6_tcp();
    test_lookup_proto_qdefault_udp();

    if (g_test_failures == 0) {
        std::cout << "[SUMMARY] All tests passed.\n";
        return 0;
    } else {
        std::cerr << "[SUMMARY] " << g_test_failures << " test(s) failed.\n";
        return 1;
    }
}