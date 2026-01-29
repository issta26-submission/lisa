/*******************************
 * ATMWK ATM keyword test suite
 * 
 * Step 1 (Program Understanding):
 * - The focal function atmkw(unsigned id) maps ATM keyword identifiers
 *   (A_METAC, A_BCC, A_OAMF4SC, A_OAMF4EC, A_SC, A_ILMIC,
 *    A_OAM, A_OAMF4, A_LANE, A_VPI, A_VCI, A_CONNECTMSG, A_METACONNECT)
 *   to their string representations using a token array and a helper
 *   qual2kw(...).
 * - Core dependent components (candidate keywords) extracted from the focal
 *   class/file scope include the ATM related identifiers and their
 *   corresponding string representations:
 *   "metac", "bcc", "oamf4sc", "oamf4ec", "sc", "ilmic", "oam",
 *   "oamf4", "lane", "vpi", "vci", "connectmsg", "metaconnect"
 * - The tested behavior relies on qual2kw handling of the token array
 *   and should return the exact token string for valid ids.
 *
 * Step 2 (Unit Test Generation):
 * - Generate tests that cover all valid ATM keyword mappings used by atmkw.
 * - Use dependencies via gencode.h to access A_* macros and atmkw.
 * - Do not use Google Test; implement a lightweight C++11 test harness.
 *
 * Step 3 (Test Case Refinement):
 * - Provide clear, maintainable tests that verify true mappings for all
 *   valid IDs. Keep tests self-contained and executable with standard C++
 *   runtime libraries.
 *******************************/

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <iomanip>
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


extern "C" {
    // Import the project's ATM keyword testable function and constants.
    // The gencode.h header defines at least:
    //   const char* atmkw(const unsigned id);
    //   enum/defines: A_METAC, A_BCC, A_OAMF4SC, A_OAMF4EC, A_SC,
    //                   A_ILMIC, A_OAM, A_OAMF4, A_LANE, A_VPI,
    //                   A_VCI, A_CONNECTMSG, A_METACONNECT
    #include "gencode.h"
}

// Prototype for the focal function from the C source (ensures C linkage)
extern "C" const char* atmkw(const unsigned id);

/*
 Candidate Keywords (from Step 1):
 - METAC -> "metac"
 - BCC -> "bcc"
 - OAMF4SC -> "oamf4sc"
 - OAMF4EC -> "oamf4ec"
 - SC -> "sc"
 - ILMIC -> "ilmic"
 - OAM -> "oam"
 - OAMF4 -> "oamf4"
 - LANE -> "lane"
 - VPI -> "vpi"
 - VCI -> "vci"
 - CONNECTMSG -> "connectmsg"
 - METACONNECT -> "metaconnect"
 These are the core dependent components that atmkw maps via the tokens table.
*/

// Simple test harness to verify atmkw mappings.
static bool test_atmkw_all_valid_ids() {
    struct TestCase {
        unsigned id;
        const char* expected;
        const char* name;
    };

    // Map each ATM identifier to its expected string form.
    // These values rely on the designated initializer in atmkw's token table.
    // If any of these A_* macros change, test updates would be required accordingly.
    TestCase cases[] = {
        { A_METAC,     "metac",       "A_METAC -> metac" },
        { A_BCC,        "bcc",          "A_BCC -> bcc" },
        { A_OAMF4SC,    "oamf4sc",      "A_OAMF4SC -> oamf4sc" },
        { A_OAMF4EC,    "oamf4ec",      "A_OAMF4EC -> oamf4ec" },
        { A_SC,          "sc",           "A_SC -> sc" },
        { A_ILMIC,       "ilmic",        "A_ILMIC -> ilmic" },
        { A_OAM,         "oam",          "A_OAM -> oam" },
        { A_OAMF4,       "oamf4",        "A_OAMF4 -> oamf4" },
        { A_LANE,        "lane",         "A_LANE -> lane" },
        { A_VPI,         "vpi",          "A_VPI -> vpi" },
        { A_VCI,         "vci",          "A_VCI -> vci" },
        { A_CONNECTMSG,  "connectmsg",   "A_CONNECTMSG -> connectmsg" },
        { A_METACONNECT, "metaconnect", "A_METACONNECT -> metaconnect" }
    };

    const size_t num_cases = sizeof(cases) / sizeof(cases[0]);
    bool all_pass = true;

    for (size_t i = 0; i < num_cases; ++i) {
        const TestCase& tc = cases[i];
        const char* result = atmkw(tc.id);
        if (result == nullptr) {
            std::cerr << "ATM test failed: atmkw(" << tc.name
                      << ") returned NULL pointer." << std::endl;
            all_pass = false;
            continue;
        }
        if (std::strcmp(result, tc.expected) != 0) {
            std::cerr << "ATM test failed: atmkw(" << tc.name << ") => '"
                      << (result ? result : "NULL") << "', expected '"
                      << tc.expected << "'." << std::endl;
            all_pass = false;
            continue;
        }

        // Optional success message per case (comment out to reduce output)
        // std::cout << "PASS: atmkw(" << tc.name << ") -> \"" << result << "\"\n";
    }

    return all_pass;
}

int main() {
    // Run the ATM keyword tests and report results.
    bool atm_pass = test_atmkw_all_valid_ids();

    std::cout << std::boolalpha;
    std::cout << "ATM keyword test suite: "
              << (atm_pass ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << std::endl;

    return atm_pass ? 0 : 1;
}