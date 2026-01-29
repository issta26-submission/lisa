// Test suite for the focal method: ss7kw
// This test suite is designed as a lightweight, non-GTest C++11 test harness.
// It relies on the project's gencode.h declarations for the ss7kw function
// and the related MTP keyword macros (e.g., M_FISU, MH_MSU, etc.).
//
// Notes:
// - This test assumes that ss7kw returns a C-string (const char*) corresponding
//   to the mapped keyword token (e.g., "fisu", "lssu", ...), or NULL when the
//   input id is out of range.
// - We perform non-terminating assertions: tests continue after a failure to
//   maximize code coverage.
// - We explicitly test all known keyword mappings and an out-of-range case.
// - The tests use only the C++ standard library and the project's public header.

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
#include <string>
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
#include <limits>
#include <cstring>


// Import dependencies and declarations from the focal project.
// This header is expected to declare: const char* ss7kw(unsigned id);
// and the MTP keyword macros (M_FISU, M_LSSU, M_MSU, MH_FISU, MH_LSSU, MH_MSU,
// M_SIO, M_OPC, M_DPC, M_SLS, MH_SIO, MH_OPC, MH_DPC, MH_SLS).

// Candidate Keywords (Step 1: extracted from the focal method)
//
// Core components used by ss7kw (represented as tokens indices):
// - M_FISU,  M_LSSU,  M_MSU
// - MH_FISU, MH_LSSU, MH_MSU
// - M_SIO,   M_OPC,   M_DPC,   M_SLS
// - MH_SIO,  MH_OPC,  MH_DPC,  MH_SLS
//
// Expected mapping strings (as defined in the tokens array inside ss7kw):
// - "fisu", "lssu", "msu"
// - "hfisu", "hlssu", "hmsu"
// - "sio", "opc", "dpc", "sls"
// - "hsio", "hopc", "hdpc", "hsls"

static int g_failures = 0;
static int g_tests_run = 0;

// Lightweight non-terminating assertion helpers
#define LOG_FAIL(msg) do { std::cerr << "TEST FAIL: " << (msg) << std::endl; } while (0)
#define EXPECT_TRUE(cond, msg) do { if (!(cond)) { ++g_failures; LOG_FAIL(msg); } } while (0)
#define EXPECT_NOT_NULL(p, msg) do { if ((p) == nullptr) { ++g_failures; LOG_FAIL(msg); } } while (0)
#define EXPECT_STR_EQ(res, expected, msg) do { if ((res) == nullptr) { ++g_failures; LOG_FAIL(std::string("NULL pointer where string expected: ").append(msg)); } else { if (std::strcmp((res), (expected)) != 0) { ++g_failures; LOG_FAIL(std::string(msg).append(" got '").append(res).append("' expected '").append(expected).append("'")); } } } while (0)
#define EXPECT_STR_CONTAINS(res, substr, msg) do { if ((res) == nullptr) { ++g_failures; LOG_FAIL(std::string("NULL pointer: ").append(msg)); } else { std::string s(res); if (s.find(substr) == std::string::npos) { ++g_failures; LOG_FAIL(std::string(msg).append(" string '").append(res).append("' does not contain '").append(substr).append("'")); } } } while (0)

// Test 1: Validate mappings for all known SS7 keywords
void test_ss7kw_mappings() {
    struct Mapping {
        unsigned id;
        const char* expected;
        const char* name;
    } mappings[] = {
        { M_FISU,  "fisu", "M_FISU" },
        { M_LSSU,  "lssu", "M_LSSU" },
        { M_MSU,   "msu",  "M_MSU"  },
        { MH_FISU, "hfisu","MH_FISU" },
        { MH_LSSU, "hlssu","MH_LSSU" },
        { MH_MSU,  "hmsu", "MH_MSU"  },
        { M_SIO,   "sio",  "M_SIO"  },
        { M_OPC,   "opc",  "M_OPC"  },
        { M_DPC,   "dpc",  "M_DPC"  },
        { M_SLS,   "sls",  "M_SLS"  },
        { MH_SIO,  "hsio", "MH_SIO" },
        { MH_OPC,  "hopc", "MH_OPC" },
        { MH_DPC,  "hdpc", "MH_DPC" },
        { MH_SLS,  "hsls", "MH_SLS" }
    };

    const size_t count = sizeof(mappings) / sizeof(mappings[0]);
    ++g_tests_run;
    for (size_t i = 0; i < count; ++i) {
        const Mapping& m = mappings[i];
        const char* res = ss7kw(m.id);
        // Expect non-null and that the returned string contains the expected token.
        bool ok = (res != nullptr);
        if (ok) {
            std::string s(res);
            if (s.find(m.expected) == std::string::npos) {
                ok = false;
            }
        }
        if (!ok) {
            ++g_failures;
            LOG_FAIL("test_ss7kw_mappings: id=" + std::to_string(m.id) +
                     " (" + m.name + ") did not map to expected token '" +
                     m.expected + "'. Returned: " +
                     (res ? std::string(res) : "NULL"));
        }
    }
}

// Test 2: Out-of-range id should return NULL (defensive check)
void test_ss7kw_out_of_range() {
    ++g_tests_run;
    unsigned out_of_range = std::numeric_limits<unsigned>::max();
    const char* res = ss7kw(out_of_range);
    EXPECT_TRUE(res == nullptr, "ss7kw(out_of_range) should return NULL");
    if (res != nullptr) {
        LOG_FAIL("test_ss7kw_out_of_range: expected NULL, got: " + std::string(res));
    }
}

// Test 3: Consistency check across multiple calls for the same id
void test_ss7kw_consistency() {
    struct Mapping {
        unsigned id;
        const char* expected;
        const char* name;
    } mappings[] = {
        { M_FISU,  "fisu", "M_FISU" },
        { M_LSSU,  "lssu", "M_LSSU" },
        { M_MSU,   "msu",  "M_MSU"  },
        { MH_FISU, "hfisu","MH_FISU" },
        { MH_LSSU, "hlssu","MH_LSSU" },
        { MH_MSU,  "hmsu", "MH_MSU"  },
        { M_SIO,   "sio",  "M_SIO"  },
        { M_OPC,   "opc",  "M_OPC"  },
        { M_DPC,   "dpc",  "M_DPC"  },
        { M_SLS,   "sls",  "M_SLS"  },
        { MH_SIO,  "hsio", "MH_SIO" },
        { MH_OPC,  "hopc", "MH_OPC" },
        { MH_DPC,  "hdpc", "MH_DPC" },
        { MH_SLS,  "hsls", "MH_SLS" }
    };

    const size_t count = sizeof(mappings) / sizeof(mappings[0]);
    ++g_tests_run;
    for (size_t i = 0; i < count; ++i) {
        const Mapping& m = mappings[i];
        const char* first = ss7kw(m.id);
        const char* second = ss7kw(m.id);
        if (first == nullptr || second == nullptr) {
            ++g_failures;
            LOG_FAIL("test_ss7kw_consistency: id=" + std::to_string(m.id) +
                     " (" + m.name + ") returned NULL on one of the calls");
            continue;
        }
        // Content equivalence check
        if (std::string(first) != std::string(second)) {
            ++g_failures;
            LOG_FAIL("test_ss7kw_consistency: id=" + std::to_string(m.id) +
                     " (" + m.name + ") has inconsistent outputs: '"
                     + std::string(first) + "' vs '" + std::string(second) + "'");
        }
    }
}

// Runner
int main() {
    std::cout << "Running ss7kw test suite (non-terminating assertions)" << std::endl;

    test_ss7kw_mappings();
    test_ss7kw_out_of_range();
    test_ss7kw_consistency();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_failures << std::endl;
    if (g_failures > 0) {
        std::cout << "Some tests FAILED. See logs above for details." << std::endl;
        return 1;
    }
    std::cout << "All tests PASSED." << std::endl;
    return 0;
}