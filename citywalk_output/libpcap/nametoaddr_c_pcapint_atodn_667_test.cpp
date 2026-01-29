// Test suite for pcapint_atodn function (from nametoaddr.c)
// - No external testing framework is used.
// - Tests are designed to cover true/false branches of the function's state machine.
// - C functions are invoked via extern "C" to avoid name mangling in C++11.
// - All tests are self-contained and run from main().

#include <string.h>
#include <pcap/namedb.h>
#include <errno.h>
#include <diag-control.h>
#include <nametoaddr.h>
#include <cstdint>
#include <vector>
#include <gencode.h>
#include <os-proto.h>
#include <thread-local.h>
#include <stdio.h>
#include <cstdio>
#include <stdlib.h>
#include <ethertype.h>
#include <config.h>
#include <pcap-int.h>
#include <string>
#include <llc.h>


// Forward declare the C function under test.
// We assume the function is available from the project build (nametoaddr.c).
extern "C" int pcapint_atodn(const char *s, uint16_t *addr);

// Simple, non-terminating test harness to allow execution to continue
// after a failed assertion and collect a final summary.
struct TestCase {
    const char* input;
    bool expectSuccess;
    uint16_t expectedAddr; // Only valid if expectSuccess == true
    const char* description;
};

// Helper to print a test result line
static void print_test_result(int idx, bool passed, const TestCase& tc, const uint16_t actualAddr, int actualRes) {
    if (passed) {
        printf("[PASS] Test %d: %s (input='%s')\n", idx, tc.description, tc.input);
    } else {
        if (tc.expectSuccess) {
            printf("[FAIL] Test %d: %s (input='%s') - expected success with addr=0x%04x, got res=%d, addr=0x%04x\n",
                   idx, tc.description, tc.input, tc.expectedAddr, actualRes, actualAddr);
        } else {
            printf("[FAIL] Test %d: %s (input='%s') - expected failure, got res=%d\n",
                   idx, tc.description, tc.input, actualRes);
        }
    }
}

int main() {
    // Domain knowledge notes:
    // AREASHIFT = 10, AREAMASK = 0176000 (octal) -> AREAMASK >> AREASHIFT = 63
    // NODEMASK = 01777 (octal) -> 1023
    // Valid form: "<area>.<node>", where area is 0..63 and node is 0..1023
    // The function returns 1 on success and writes into *addr the value
    // (area << AREASHIFT) | node. It returns 0 on any invalid input.

    std::vector<TestCase> tests = {
        // Valid basic inputs
        {"1.2", true,  (uint16_t)((1u << 10) | 2u), "basic valid small numbers"},
        // Boundary values
        {"63.1023", true, (uint16_t)((63u << 10) | 1023u), "maximum area and maximum node"},
        // Minimum values
        {"0.0", true, 0u, "minimum area and node"},
        // Another valid boundary
        {"0.63", true, (uint16_t)(0u << 10 | 63u), "zero area with non-zero node"},
        {"63.0", true, (uint16_t)(63u << 10 | 0u), "max area with zero node"},

        // Invalid inputs to cover false branches
        {"", false, 0u, "empty string (invalid)"},
        {"a", false, 0u, "non-digit first character (invalid)"},
        {"1", false, 0u, "missing DOT and node (invalid)"},
        {"1.", false, 0u, "dot with no node (invalid)"},
        {"64.1", false, 0u, "area overflow beyond 63 (invalid)"},
        {"1.1024", false, 0u, "node overflow beyond 1023 (invalid)"},
        {"1.2.3", false, 0u, "multiple dots (invalid)"},
        {"1a2.3", false, 0u, "non-digit within area (invalid)"},
        {"01.2", false, 0u, "leading zero area but multi-digit; treated as valid in parsing, but keeping test strict"},

        // Optional: additional coverage for node boundary with leading zeros in node
        {"1.0002", false, 0u, "node with leading zeros exceeding valid interpretation (invalid)"},
    };

    int failures = 0;
    for (size_t i = 0; i < tests.size(); ++i) {
        const TestCase& tc = tests[i];
        uint16_t addr = 0;
        int res = pcapint_atodn(tc.input, &addr);

        bool pass = false;
        if (tc.expectSuccess) {
            pass = (res == 1) && (addr == tc.expectedAddr);
        } else {
            pass = (res == 0);
        }

        print_test_result((int)i, pass, tc, addr, res);

        if (!pass) ++failures;
    }

    if (failures == 0) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("%d test(s) failed.\n", failures);
        return 1;
    }
}