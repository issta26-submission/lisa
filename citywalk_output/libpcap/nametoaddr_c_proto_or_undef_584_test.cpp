/*
  C++11 test suite for the focal method proto_or_undef (via public wrapper pcap_nametoeproto)
  This test avoids GTest and uses a lightweight, non-terminating assertion approach.
  It targets behavior exposed by the public API:
    - pcap_nametoeproto("arp"/"ip"/"ip6") should return the corresponding ETHERTYPE values
      as defined in the internal eproto_db[] mapping.
    - pcap_nametoeproto("unknown") should return PROTO_UNDEF.
  The internal static function proto_or_undef is not directly accessible, but its behavior
  is exercised through the public interface.
*/

#include <string.h>
#include <pcap/namedb.h>
#include <errno.h>
#include <diag-control.h>
#include <nametoaddr.h>
#include <gencode.h>
#include <os-proto.h>
#include <thread-local.h>
#include <stdio.h>
#include <stdlib.h>
#include <ethertype.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <iostream>
#include <llc.h>


// Bring C symbols into the C++ test via C linkage
extern "C" {
}

// Expose the internal eproto_db[] for test-driven coverage without changing access specifiers
extern "C" struct eproto eproto_db[];

/* Lightweight test framework (non-terminating assertions)
   - We accumulate failures and report a summary at the end.
*/
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ(a, b, msg) do { \
    ++g_tests_run; \
    if ((a) != (b)) { \
        ++g_tests_failed; \
        std::cerr << "FAIL: " << msg << " | expected: " << (b) << ", got: " << (a) << "\n"; \
    } \
} while(0)

#define EXPECT_NE(a, b, msg) do { \
    ++g_tests_run; \
    if ((a) == (b)) { \
        ++g_tests_failed; \
        std::cerr << "FAIL: " << msg << " | unexpected equality with: " << (a) << "\n"; \
    } \
} while(0)

/* Helper: fetch the p value from the internal db for a given string key.
   Returns -1 if not found (shouldn't happen for known keys in the test).
*/
static int lookup_db_proto(const char *key)
{
    for (int i = 0; eproto_db[i].s != NULL; ++i) {
        if (strcmp(eproto_db[i].s, key) == 0) {
            return (int)eproto_db[i].p;
        }
    }
    return -1; // not found
}

/* Test 1: arp maps to the database value via pcap_nametoeproto.
   This validates the true-branch behavior of proto_or_undef when a match exists.
*/
static void test_proto_lookup_arp_matches_db()
{
    int result = pcap_nametoeproto("arp");
    int expected = lookup_db_proto("arp");
    EXPECT_EQ(result, expected, "pcap_nametoeproto('arp') should equal db value for 'arp'");
}

/* Test 2: ip maps to the database value via pcap_nametoeproto.
   Confirms another true-branch match.
*/
static void test_proto_lookup_ip_matches_db()
{
    int result = pcap_nametoeproto("ip");
    int expected = lookup_db_proto("ip");
    EXPECT_EQ(result, expected, "pcap_nametoeproto('ip') should equal db value for 'ip'");
}

/* Test 3: ip6 maps to the database value via pcap_nametoeproto.
   Confirms another true-branch match for IPv6.
*/
static void test_proto_lookup_ip6_matches_db()
{
    int result = pcap_nametoeproto("ip6");
    int expected = lookup_db_proto("ip6");
    EXPECT_EQ(result, expected, "pcap_nametoeproto('ip6') should equal db value for 'ip6'");
}

/* Test 4: Unknown name should yield PROTO_UNDEF (false-branch of proto_or_undef).
*/
static void test_proto_lookup_unknown_yields_undef()
{
    int result = pcap_nametoeproto("notaproto");
    EXPECT_EQ(result, PROTO_UNDEF, "pcap_nametoeproto('notaproto') should yield PROTO_UNDEF");
}

/* Test 5: Case sensitivity check; exact string match required.
   proto_or_undef uses strcmp (case-sensitive), so "ARP" should not match "arp".
*/
static void test_proto_lookup_case_sensitivity()
{
    int result_lower = pcap_nametoeproto("arp");
    int result_upper = pcap_nametoeproto("ARP"); // should be undefined
    EXPECT_EQ(result_lower, lookup_db_proto("arp"), "pcap_nametoeproto('arp') should match db value");
    EXPECT_EQ(result_upper, PROTO_UNDEF, "pcap_nametoeproto('ARP') should yield PROTO_UNDEF due to case-sensitivity");
}

/* Run all tests and report final status. */
static void run_all_tests()
{
    test_proto_lookup_arp_matches_db();
    test_proto_lookup_ip_matches_db();
    test_proto_lookup_ip6_matches_db();
    test_proto_lookup_unknown_yields_undef();
    test_proto_lookup_case_sensitivity();

    std::cout << "Tests run: " << g_tests_run << "\n";
    if (g_tests_failed == 0)
        std::cout << "ALL TESTS PASSED\n";
    else
        std::cout << "TESTS FAILED: " << g_tests_failed << "\n";
}

int main()
{
    run_all_tests();
    return g_tests_failed; // non-zero if any test failed
}