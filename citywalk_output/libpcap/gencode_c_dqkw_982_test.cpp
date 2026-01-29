// Test suite for the focal method dqkw in gencode.c
// This test is written in C++11 without GoogleTest (no external GT frameworks).
// It uses a lightweight, non-terminating assertion mechanism to maximize
// code coverage while continuing test execution after failures.
// The tests rely on the project-provided headers (gencode.h) and the C
// function dqkw with its associated macros (Q_SRC, Q_DST, etc.).

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


#ifdef __cplusplus
extern "C" {
#endif
// Include the project's header to obtain declarations like dqkw and Q_* macros.
// This header is expected to declare: dqkw(const unsigned id), Q_SRC, Q_DST, Q_OR, Q_AND,
// Q_ADDR1, Q_ADDR2, Q_ADDR3, Q_ADDR4, Q_RA, Q_TA, and any other necessary entities.
#ifdef __cplusplus
}
#endif

// Lightweight non-terminating assertion macro.
// It prints a message on failure but does not abort the program, allowing all
// test cases to run for maximum coverage.
#define NP_ASSERT(cond, msg)                                     \
    do {                                                         \
        if (!(cond)) {                                           \
            std::cerr << "[FAIL] " << msg << std::endl;         \
            ++gFailures;                                        \
        } else {                                                 \
            ++gPasses;                                          \
        }                                                        \
    } while (0)

static int gPasses = 0;
static int gFailures = 0;

// Helper: check if 'hay' contains 'needle' as a substring.
static bool contains(const char* hay, const char* needle) {
    if (hay == nullptr || needle == nullptr) return false;
    return std::strstr(hay, needle) != nullptr;
}

// Entrypoint
int main() {
    // Describe each test case: id to test, and the expected substring that should
    // be present in the return string of dqkw for that id.
    // The token strings used inside dqkw are:
    //   Q_SRC  -> "src"
    //   Q_DST  -> "dst"
    //   Q_OR   -> "src or dst"
    //   Q_AND  -> "src and dst"
    //   Q_ADDR1-> "addr1"
    //   Q_ADDR2-> "addr2"
    //   Q_ADDR3-> "addr3"
    //   Q_ADDR4-> "addr4"
    //   Q_RA   -> "ra"
    //   Q_TA   -> "ta"
    //
    // We verify by checking that the returned string contains the corresponding
    // substring. This approach tolerates variations in formatting that may be
    // introduced by qual2kw (e.g., prefixes like "dir:", etc.) while ensuring
    // correct mapping for each id.
    struct TestCase {
        unsigned id;
        const char* expectedSubstring;
        const char* description;
    };

    // Build test cases using the project's macros. If any macro is not defined in
    // the including environment, compilation will fail here, which is expected
    // for a real project test harness.
    std::vector<TestCase> tests = {
        { Q_SRC,  "src",  "Verify Q_SRC maps to 'src' token" },
        { Q_DST,  "dst",  "Verify Q_DST maps to 'dst' token" },
        { Q_OR,   "src or dst", "Verify Q_OR maps to 'src or dst' token" },
        { Q_AND,  "src and dst", "Verify Q_AND maps to 'src and dst' token" },
        { Q_ADDR1,"addr1", "Verify Q_ADDR1 maps to 'addr1' token" },
        { Q_ADDR2,"addr2", "Verify Q_ADDR2 maps to 'addr2' token" },
        { Q_ADDR3,"addr3", "Verify Q_ADDR3 maps to 'addr3' token" },
        { Q_ADDR4,"addr4", "Verify Q_ADDR4 maps to 'addr4' token" },
        { Q_RA,   "ra",   "Verify Q_RA maps to 'ra' token" },
        { Q_TA,   "ta",   "Verify Q_TA maps to 'ta' token" }
    };

    // Run tests
    for (const auto& tc : tests) {
        const char* result = dqkw(tc.id);
        // Null-check: dqkw should always return a pointer to a (non-null) string
        NP_ASSERT(result != nullptr, std::string("dqkw returned null for id=").append(std::to_string(tc.id)).c_str());

        // If non-null, verify that the expected substring is present.
        bool ok = (result != nullptr) && contains(result, tc.expectedSubstring);
        NP_ASSERT(ok, std::string("dqkw(").append(std::to_string(tc.id))
                       .append(") did not contain expected substring '")
                       .append(tc.expectedSubstring).append("'. Got: '")
                       .append(result ? result : "null").append("'").c_str());
        // Note: We do not terminate on first failure to maximize coverage.
        // The descriptive messages help locate which cases fail.
    }

    // Summary
    std::cout << "------------------------------" << std::endl;
    std::cout << "Unit test summary for dqkw" << std::endl;
    std::cout << "Passed: " << gPasses << std::endl;
    std::cout << "Failed: " << gFailures << std::endl;
    std::cout << "------------------------------" << std::endl;

    // Exit with non-zero if any test failed to signal issues in CI
    return (gFailures == 0) ? 0 : 1;
}