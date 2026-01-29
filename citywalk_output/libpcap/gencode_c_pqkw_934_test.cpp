/*
Auto-generated C++11 unit test harness for the focal function pqkw (located in gencode.c).

Notes:
- The test harness is designed to be compiled as C++11 and linked against the C source
  implementing pqkw (gencode.c). The C source uses C99 designated initializers and
  a C interface, so we declare the symbol with C linkage: extern "C" const char*
  pqkw(unsigned id);
- We avoid any GTest usage as requested. A lightweight in-house test runner is provided
  that prints per-test results and a final summary.
- Domain assumptions:
  - The function pqkw maps a numeric id to a keyword string via an internal tokens[] array.
  - Based on the provided tokens, Q_LINK likely corresponds to id 0, Q_IP to id 1, Q_TCP to id 5,
    Q_UDP to id 6, and Q_CARP to the last token (id ~38). We verify these mappings by asserting
    that the returned string contains the known token for each id.
- This test focuses on true/false coverage of the predicate mappings by validating known-id mappings
  and basic properties (non-empty, consistency, and difference between distinct ids).
- Static members and private/internal details in the focal code are not accessed directly; only the
  pqkw interface is exercised, consistent with Step 3 requirements.

We also include a brief Candidate Keywords section to document the core dependencies observed
in the focal method, as requested in Step 1.

*/

// Candidate Keywords identified from the focal method and its dependencies (Step 1):
// - Tokens mapping: "link", "ip", "arp", "rarp", "sctp", "tcp", "udp", "icmp", "igmp", "igrp",
//   "atalk", "decnet", "lat", "sca", "moprc", "mopdl", "ip6", "icmp6", "ah", "esp", "pim",
//   "vrrp", "aarp", "iso", "esis", "isis", "clnp", "stp", "ipx", "netbeui", "l1", "l2",
//   "iih", "snp", "csnp", "psnp", "lsp", "radio", "carp"
// - Kind string: "proto" (passed to qual2kw)
// - qual2kw: helper function used to translate kind/id/tokens into a final string
// - id: unsigned input that selects an entry in tokens (Q_* enum index)
// - pqkw: focal function under test which returns a const char* (string) via qual2kw
//
// These keywords guide expected mappings for test cases (e.g., id 0 -> "link", id 1 -> "ip", etc.)

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
#include <cstdlib>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <setjmp.h>
#include <string>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <vector>
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


// Extern C prototype of the focal function under test.
// The function is defined in gencode.c (compiled as C).
extern "C" const char* pqkw(const unsigned id);

// Simple in-house test framework (non-terminating assertions)
static void log_test_result(const std::string& name, bool passed, const std::string& details = "") {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << name;
    if (!details.empty()) {
        std::cout << " - " << details;
    }
    std::cout << std::endl;
}

// Helper: call pqkw and return std::string (empty if null)
static std::string call_pqkw(unsigned id) {
    const char* s = pqkw(id);
    if (s == nullptr) {
        return std::string();
    }
    return std::string(s);
}

// Helper: verify that the returned string contains the expected token (case-sensitive)
static bool string_contains(const std::string& haystack, const std::string& needle) {
    return haystack.find(needle) != std::string::npos;
}

// Mapping from id to the expected token based on the provided token table.
// We assume the enumerations start at 0 (0 -> Q_LINK, 1 -> Q_IP, ..., 38 -> Q_CARP).
// This mapping is used only for the test expectations.
static std::string expected_token_for_id(unsigned id) {
    switch (id) {
        case 0: return "link";
        case 1: return "ip";
        case 2: return "arp";
        case 3: return "rarp";
        case 4: return "sctp";
        case 5: return "tcp";
        case 6: return "udp";
        case 7: return "icmp";
        case 8: return "igmp";
        case 9: return "igrp";
        case 10: return "atalk";
        case 11: return "decnet";
        case 12: return "lat";
        case 13: return "sca";
        case 14: return "moprc";
        case 15: return "mopdl";
        case 16: return "ip6";
        case 17: return "icmp6";
        case 18: return "ah";
        case 19: return "esp";
        case 20: return "pim";
        case 21: return "vrrp";
        case 22: return "aarp";
        case 23: return "iso";
        case 24: return "esis";
        case 25: return "isis";
        case 26: return "clnp";
        case 27: return "stp";
        case 28: return "ipx";
        case 29: return "netbeui";
        case 30: return "l1";
        case 31: return "l2";
        case 32: return "iih";
        case 33: return "snp";
        case 34: return "csnp";
        case 35: return "psnp";
        case 36: return "lsp";
        case 37: return "radio";
        case 38: return "carp";
        default: return ""; // unknown id
    }
}

// Test 1: Verify that known ids map to the expected token substrings.
// We check that the returned string contains the exact expected token (lowercase).
static void test_pqkw_known_ids_mapping() {
    // Define a small set of representative ids to validate known mappings.
    const unsigned test_ids[] = {0, 1, 5, 6, 38}; // link, ip, tcp, udp, carp
    const size_t n = sizeof(test_ids) / sizeof(test_ids[0]);
    bool all_passed = true;

    for (size_t i = 0; i < n; ++i) {
        unsigned id = test_ids[i];
        std::string expected = expected_token_for_id(id);
        std::string value = call_pqkw(id);

        // If pqkw returns null/empty, that's a clear failure for a known id
        bool ok = !value.empty() && string_contains(value, expected);
        if (!ok) {
            all_passed = false;
        }
        std::string details;
        if (value.empty()) {
            details = "pqkw(" + std::to_string(id) + ") returned empty string";
        } else {
            details = "expected token '" + expected + "' not found in '" + value + "'";
        }
        log_test_result("pqkw_known_id_mapping_id_" + std::to_string(id),
                        ok, details);
    }

    // Report overall status (optional additional detail)
    if (all_passed) {
        log_test_result("pqkw_known_ids_mapping", true);
    } else {
        log_test_result("pqkw_known_ids_mapping", false, "One or more known id mappings failed");
    }
}

// Test 2: Consistency check for the same id across multiple calls.
static void test_pqkw_consistency_same_id() {
    unsigned id = 0; // using id 0 as a representative known id (expected 'link')
    std::string first = call_pqkw(id);
    std::string second = call_pqkw(id);
    bool ok = (!first.empty()) && (first == second);
    std::string details;
    if (first.empty()) {
        details = "first call pqkw(0) returned empty string";
    } else if (first != second) {
        details = "inconsistent results between calls: '" + first + "' vs '" + second + "'";
    }
    log_test_result("pqkw_consistency_same_id_0", ok, details);
}

// Test 3: Distinct ids should produce distinct outputs (most of the time).
static void test_pqkw_distinct_ids_produce_distinct_results() {
    unsigned id_a = 0; // expected 'link'
    unsigned id_b = 1; // expected 'ip'
    std::string val_a = call_pqkw(id_a);
    std::string val_b = call_pqkw(id_b);
    bool ok = (!(val_a.empty()) && !(val_b.empty()) && (val_a != val_b));
    std::string details;
    if (val_a.empty() || val_b.empty()) {
        details = "One of the calls returned an empty string (val_a: '" +
                  val_a + "', val_b: '" + val_b + "')";
    } else if (val_a == val_b) {
        details = "Both ids produced identical outputs: '" + val_a + "'";
    }
    log_test_result("pqkw_distinct_ids_produce_distinct_results_0_vs_1", ok, details);
}

// Test 4: Non-empty output for a broader sample of ids (sanity check).
static void test_pqkw_nonempty_for_selected_ids() {
    const unsigned test_ids[] = {0, 1, 5, 6, 38};
    bool all_nonempty = true;
    for (unsigned id : test_ids) {
        std::string v = call_pqkw(id);
        if (v.empty()) {
            all_nonempty = false;
            log_test_result("pqkw_nonempty_for_id_" + std::to_string(id), false,
                            "empty result for id " + std::to_string(id));
        } else {
            log_test_result("pqkw_nonempty_for_id_" + std::to_string(id), true);
        }
    }
    if (all_nonempty) {
        log_test_result("pqkw_nonempty_for_selected_ids", true);
    } else {
        log_test_result("pqkw_nonempty_for_selected_ids", false, "One or more ids returned empty string");
    }
}

int main() {
    // Run the test suite (Step 2/3 coverage)
    std::cout << "Starting pqkw test suite (C++11 harness for pqkw)..." << std::endl;

    test_pqkw_known_ids_mapping();
    test_pqkw_consistency_same_id();
    test_pqkw_distinct_ids_produce_distinct_results();
    test_pqkw_nonempty_for_selected_ids();

    std::cout << "pqkw test suite completed." << std::endl;
    return 0;
}