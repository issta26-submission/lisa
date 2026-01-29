/*
Unit test suite for the focal method: dlprim in pcap-dlpi.c

Notes:
- The tests are written in C++11 without any external testing framework (no GTest).
- The tests call the C function dlprim via C linkage.
- We rely on system headers to provide the DLPI primitive constants (e.g., DL_INFO_REQ, DL_INFO_ACK, ...).
- The test suite covers all known primitives (true branches) and a default/unknown primitive (false branch).
- The tests are designed to be non-terminating on failure: they report errors and continue executing remaining tests.
- This test file should be compiled and linked with the pcap-dlpi.c implementation (as part of the project).
- Explanatory comments are added for each unit test to clarify purpose and expectations.

To build (example, assuming a POSIX-like environment and that pcap-dlpi.c is part of the build):
g++ -std=c++11 -Wall -Wextra -O0 test_dlprim.cpp pcap-dlpi.c -o test_dlprim

*/

// Candidate Keywords (for Step 1 understanding)
 // Candidate Keywords: DL_INFO_REQ, DL_INFO_ACK, DL_ATTACH_REQ, DL_DETACH_REQ, DL_BIND_REQ,
 // DL_BIND_ACK, DL_UNBIND_REQ, DL_OK_ACK, DL_ERROR_ACK, DL_SUBS_BIND_REQ, DL_SUBS_BIND_ACK,
 // DL_UNITDATA_REQ, DL_UNITDATA_IND, DL_UDERROR_IND, DL_UDQOS_REQ, DL_CONNECT_REQ,
 // DL_CONNECT_IND, DL_CONNECT_RES, DL_CONNECT_CON, DL_TOKEN_REQ, DL_TOKEN_ACK,
 // DL_DISCONNECT_REQ, DL_DISCONNECT_IND, DL_RESET_REQ, DL_RESET_IND, DL_RESET_RES,
 // DL_RESET_CON, unknown primitive

#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <string>
#include <fcntl.h>
#include <vector>
#include <sys/stat.h>
#include <sys/systeminfo.h>
#include <limits.h>
#include <sys/dlpi.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <dlpisubs.h>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <os-proto.h>
#include <sys/bufmod.h>
#include <config.h>
#include <cstring>
#include <sys/stream.h>


extern "C" {
    // Forward declaration of the focal C function.
    // Use unsigned int in place of bpf_u_int32 for compatibility with C linkage.
    // The actual signature in pcap-dlpi.c is: const char* dlprim(char *primbuf, size_t primbufsize, bpf_u_int32 prim)
    const char* dlprim(char *primbuf, size_t primbufsize, unsigned int prim);
}

// Include system DLPI primitives to obtain correct numeric values for testing.
// These macros are defined by system headers on platforms that provide DLPI (e.g., Solaris).
// If not available on the target platform, the build should be adjusted accordingly.

// Typedef to align with the function's expected primitive type in tests.
typedef unsigned int bpf_u_int32;

// Simple, lightweight test harness (non-terminating; prints per-test results)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Utility: run a single test and report result
static void report_test(const std::string& test_name, bool passed, const std::string& details = "")
{
    ++g_total_tests;
    if (passed) {
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        ++g_failed_tests;
        std::cout << "[FAIL] " << test_name;
        if (!details.empty()) {
            std::cout << " - " << details;
        }
        std::cout << std::endl;
    }
}

// Test 1: Verify that each known DLPI primitive maps to its string literal.
// This exercises all true branches of the switch in the focal function.
static void test_dlprim_known_primitives()
{
    // Structure to pair primitive value with its expected string.
    struct Case { unsigned int prim; const char* expected; };

    // Populate cases using actual DLPI constants from system headers.
    // These macros are defined in <sys/dlpi.h> on platforms that support DLPI.
    // If any macro is missing on the target platform, the test should be adjusted accordingly.
    const Case cases[] = {
        { DL_INFO_REQ,        "DL_INFO_REQ" },
        { DL_INFO_ACK,        "DL_INFO_ACK" },
        { DL_ATTACH_REQ,      "DL_ATTACH_REQ" },
        { DL_DETACH_REQ,      "DL_DETACH_REQ" },
        { DL_BIND_REQ,        "DL_BIND_REQ" },
        { DL_BIND_ACK,        "DL_BIND_ACK" },
        { DL_UNBIND_REQ,      "DL_UNBIND_REQ" },
        { DL_OK_ACK,          "DL_OK_ACK" },
        { DL_ERROR_ACK,       "DL_ERROR_ACK" },
        { DL_SUBS_BIND_REQ,    "DL_SUBS_BIND_REQ" },
        { DL_SUBS_BIND_ACK,    "DL_SUBS_BIND_ACK" },
        { DL_UNITDATA_REQ,     "DL_UNITDATA_REQ" },
        { DL_UNITDATA_IND,     "DL_UNITDATA_IND" },
        { DL_UDERROR_IND,      "DL_UDERROR_IND" },
        { DL_UDQOS_REQ,         "DL_UDQOS_REQ" },
        { DL_CONNECT_REQ,       "DL_CONNECT_REQ" },
        { DL_CONNECT_IND,       "DL_CONNECT_IND" },
        { DL_CONNECT_RES,       "DL_CONNECT_RES" },
        { DL_CONNECT_CON,       "DL_CONNECT_CON" },
        { DL_TOKEN_REQ,         "DL_TOKEN_REQ" },
        { DL_TOKEN_ACK,         "DL_TOKEN_ACK" },
        { DL_DISCONNECT_REQ,      "DL_DISCONNECT_REQ" },
        { DL_DISCONNECT_IND,      "DL_DISCONNECT_IND" },
        { DL_RESET_REQ,           "DL_RESET_REQ" },
        { DL_RESET_IND,           "DL_RESET_IND" },
        { DL_RESET_RES,           "DL_RESET_RES" },
        { DL_RESET_CON,           "DL_RESET_CON" }
    };

    const size_t primbufsize = 128;
    char primbuf[primbufsize];

    // Run through all predefined primitives and verify exact string returned.
    for (size_t i = 0; i < sizeof(cases)/sizeof(cases[0]); ++i) {
        g_total_tests++;
        unsigned int prim = cases[i].prim;
        const char* expected = cases[i].expected;

        // The function returns a string literal for known primitives.
        const char* actual = dlprim(primbuf, primbufsize, prim);

        // Check content equality. If the function returns a literal, content matches exactly.
        bool ok = (actual != nullptr) && (std::strcmp(actual, expected) == 0);
        std::string detail;
        if (!ok) {
            detail = "expected '" + std::string(expected) + "', got '";
            // Provide actual content for diagnostics
            if (actual != nullptr) {
                detail += actual;
            } else {
                detail += "nullptr";
            }
            detail += "'";
        }

        report_test("DL prim " + std::to_string(prim) + " corresponds to " + std::string(expected),
                    ok, detail);
    }
}

// Test 2: Unknown primitive should be written into primbuf and returned as the same buffer.
// This exercises the default branch of the switch.
static void test_dlprim_unknown_primitive()
{
    // Use a value that is not defined as any DLPI primitive to hit the default case.
    // 0xFFFFFFFF is a good choice for 32-bit unsigned values.
    const unsigned int unknown_prim = 0xFFFFFFFFu;
    const size_t primbufsize = 128;
    char primbuf[primbufsize];

    // The function should return primbuf and fill it with the formatted unknown primitive string.
    const char* result = dlprim(primbuf, primbufsize, unknown_prim);

    // Expected string is formatted as: "unknown primitive 0xffffffff"
    const char* expected = "unknown primitive 0xffffffff";

    bool ok = (result == primbuf) && (std::strcmp(primbuf, expected) == 0);
    if (!ok) {
        std::string detail;
        detail = "expected buffer content '" + std::string(expected) + "', got '";
        detail += primbuf;
        detail += "'";
        report_test("DL prim unknown primitive default branch", false, detail);
    } else {
        report_test("DL prim unknown primitive default branch", true);
    }
}

// Entry point for running all tests
int main()
{
    std::cout << "Starting unit tests for focal method: dlprim (pcap-dlpi.c)" << std::endl;

    // Step 2: Unit Test Generation
    // Cover true branches for all known primitives
    test_dlprim_known_primitives();

    // Step 3: Test Case Refinement
    // Ensure default/unknown path is exercised
    test_dlprim_unknown_primitive();

    // Summary
    std::cout << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;

    // Exit code semantics: non-zero on failure to aid CI, zero if all pass
    return (g_failed_tests == 0) ? 0 : 1;
}