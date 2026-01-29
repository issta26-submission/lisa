// Unit tests for pcap_nametoeproto in nametoaddr.c
// - A lightweight, self-contained test suite (no Google Test or GMock)
// - Uses only C++11 standard library
// - Calls the C function directly via extern "C"
// - Provides non-terminating EXPECT macros to maximize test coverage
// - Main() runs all tests; returns number of failed tests

#include <string.h>
#include <pcap/namedb.h>
#include <errno.h>
#include <diag-control.h>
#include <nametoaddr.h>
#include <cstdint>
#include <cstdlib>
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


// Step 1: Program Understanding (summary for test authors)
// - The focal method is: int pcap_nametoeproto(const char *s)
// - It maps a protocol name string to its EtherType numeric code using the
//   static table 'eproto_db' defined in nametoaddr.c.
// - If the name is found, it returns the corresponding proto (EtherType); otherwise
//   it returns PROTO_UNDEF (an undefined protocol indicator).
// - We will test:
//   1) Known mappings (e.g., "ip" -> ETHERTYPE_IP, "ip6" -> ETHERTYPE_IPV6, "arp" -> ETHERTYPE_ARP)
//   2) Unknown mapping yields a value different from a known valid EtherType (negative branch)
// - This ensures both branches of the predicate inside proto_or_undef are exercised.


// Step 2: Unit Test Generation
// - Expose the C function to C++ via extern "C".
extern "C" int pcap_nametoeproto(const char *s);

// Lightweight test framework (non-terminating assertions)
static int g_passed = 0;
static int g_failed = 0;

#define EXPECT_EQ(expected, actual, msg) \
    do { \
        if (static_cast<int>(actual) == static_cast<int>(expected)) { \
            ++g_passed; \
        } else { \
            ++g_failed; \
            std::cerr << "FAIL: " << msg << " | Expected: 0x" << std::hex << (int)(expected) \
                      << " , Got: 0x" << (int)(actual) << std::dec << std::endl; \
        } \
    } while (0)

#define EXPECT_NE(not_expected, actual, msg) \
    do { \
        if (static_cast<int>(actual) != static_cast<int>(not_expected)) { \
            ++g_passed; \
        } else { \
            ++g_failed; \
            std::cerr << "FAIL: " << msg << " | Did not expect: 0x" \
                      << std::hex << (int)(not_expected) << std::dec << std::endl; \
        } \
    } while (0)


// Step 3: Test Case Refinement
// - Tests chosen to cover true and false branches of the predicate inside
//   proto_or_undef by verifying known mappings and an unknown mapping.
// - The chosen known mappings align with common EtherType values:
//   IP = 0x0800, IPv6 = 0x86DD, ARP = 0x0806
// - Note: EtherType numeric values are standard and should be stable across platforms.


// Test 1: Known mapping for "ip" should return ETHERTYPE_IP (0x0800)
void test_nametoeproto_ip() {
    int res = pcap_nametoeproto("ip");
    EXPECT_EQ(0x0800, res, "pcap_nametoeproto('ip') should map to EtherType IP (0x0800)");
}

// Test 2: Known mapping for "ip6" should return ETHERTYPE_IPV6 (0x86DD)
void test_nametoeproto_ip6() {
    int res = pcap_nametoeproto("ip6");
    EXPECT_EQ(0x86DD, res, "pcap_nametoeproto('ip6') should map to EtherType IPv6 (0x86DD)");
}

// Test 3: Known mapping for "arp" should return ETHERTYPE_ARP (0x0806)
void test_nametoeproto_arp() {
    int res = pcap_nametoeproto("arp");
    EXPECT_EQ(0x0806, res, "pcap_nametoeproto('arp') should map to EtherType ARP (0x0806)");
}

// Test 4: Unknown mapping should not accidentally map to IP EtherType
void test_nametoeproto_unknown() {
    int res = pcap_nametoeproto("this_proto_name_should_not_exist_12345");
    // Ensure negative/undefined mapping behavior: not equal to IP EtherType
    EXPECT_NE(0x0800, res, "pcap_nametoeproto(unknown) should not map to IP EtherType (0x0800)");
}


// Main function: run all tests from Step 2/3
int main() {
    std::cout << "Starting tests for pcap_nametoeproto...\n";

    // Execute tests
    test_nametoeproto_ip();
    test_nametoeproto_ip6();
    test_nametoeproto_arp();
    test_nametoeproto_unknown();

    // Summary
    int total = g_passed + g_failed;
    std::cout << "Tests completed. Passed: " << g_passed
              << ", Failed: " << g_failed
              << ", Total: " << total << "\n";

    // Return number of failed tests as exit code for quick integration
    return g_failed;
}