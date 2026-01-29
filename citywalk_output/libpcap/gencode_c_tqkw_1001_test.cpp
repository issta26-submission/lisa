// Test suite for tqkw function from gencode.c
// This test is designed to be compiled with a C/C++11 toolchain and linked
// against the project that provides gencode.c/gencode.h.
// No Google Test is used; a lightweight custom test harness is provided.

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <utility>
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


// Step 1 & 2: Import the focal library (gencode) to access tqkw and related enums.
// The header is expected to declare Q_HOST, Q_NET, Q_PORT, Q_GATEWAY, Q_PROTO,
// Q_PROTOCHAIN, Q_PORTRANGE, and the function tqkw(unsigned).
#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Lightweight test assertion helpers (non-terminating)
static int g_failures = 0;

#define EXPECT_STR_EQ(actual, expected) do { \
    if ((actual) == NULL || std::strcmp((actual), (expected)) != 0) { \
        std::cerr << "Expectation failed: got '" \
                  << (actual ? (actual) : "NULL") \
                  << "'; expected '" << (expected) << "'.\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_NULL(actual) do { \
    if ((actual) != NULL) { \
        std::cerr << "Expectation failed: got non-NULL value '" \
                  << (actual) << "'; expected NULL.\n"; \
        ++g_failures; \
    } \
} while(0)

// Explanatory comments for each unit test
// Test 1: Validate that tqkw maps known IDs to their correct keyword strings.
// This covers the true branch for each designated initializer in the tokens array.
static void test_tqkw_known_mappings() {
    // Candidate mappings derived from the provided tokens[]
    struct KnownMapping {
        unsigned id;
        const char* expected;
    };

    // Ensure these IDs exist in the included header; they are used as designators in the tokens[] map.
    // The expected strings come directly from the tokens mapping in FOCAL_METHOD.
    KnownMapping mappings[] = {
        { Q_HOST,       "host" },
        { Q_NET,        "net" },
        { Q_PORT,       "port" },
        { Q_GATEWAY,    "gateway" },
        { Q_PROTO,      "proto" },
        { Q_PROTOCHAIN, "protochain" },
        { Q_PORTRANGE,   "portrange" },
    };

    for (const auto &m : mappings) {
        const char *res = tqkw(m.id);
        // For valid IDs, we expect a non-NULL pointer equal to the token string.
        EXPECT_STR_EQ(res, m.expected);
    }
}

// Test 2: Validate that tqkw handles an invalid ID gracefully (returns NULL or equivalent).
// This exercises the false branch where the ID is out-of-range for the tokens array.
static void test_tqkw_invalid_id() {
    // Choose a value that is highly unlikely to be a valid Q_* ID.
    // The exact value is not important; it should be outside the tokens[] range.
    unsigned invalid_id = 0xFFFF; // large value outside typical range
    const char *res = tqkw(invalid_id);
    EXPECT_NULL(res);
}

// Step 3: Test runner to execute the tests and report results.
int main() {
    std::cout << "Running tqkw unit tests...\n";

    // Run individual tests
    test_tqkw_known_mappings();
    test_tqkw_invalid_id();

    // Summary
    if (g_failures == 0) {
        std::cout << "ALL tqkw tests PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " tqkw test(s) FAILED\n";
        return g_failures;
    }
}