// Test suite for the focal method: bpf_filter
// Purpose: Verify that bpf_filter forwards its arguments to pcapint_filter
//          and returns the same value produced by pcapint_filter.
// Approach: Provide a minimal C/C++ bridge in a single test file.
//           We supply a C-safe stub for pcapint_filter that records invocation
//           and returns a value derived from the BPF instruction(s) to enable
//           deterministic assertions without relying on the full PCAP/BPF engine.
//
// Notes:
// - This test is designed to be compiled with C++11 (no GTest required).
// - The struct bpf_insn layout is defined here to match typical Linux/bpf layout.
// - We deliberately keep tests lightweight and exercise multiple program inputs
//   to exercise forward-declaration and return-path behavior of bpf_filter.

#include <pcap/pcap-inttypes.h>
#include <sys/types.h>
#include <extract.h>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <sys/param.h>
#include <diag-control.h>
#include <vector>
#include <linux/filter.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <pcap-types.h>
#include <linux/types.h>
#include <sys/time.h>
#include <cstddef>
#include <linux/if_packet.h>
#include <config.h>


// Lightweight typedefs to mirror the project's expected types
using u_int = unsigned int;
using u_char = unsigned char;

// Minimal struct matching the common BPF instruction layout used by libpcap/libbpf
// This mirrors typical definitions used by BPF interfaces.
struct bpf_insn {
    uint16_t code; // operation code
    uint8_t  jt;   // jump true offset
    uint8_t  jf;   // jump false offset
    uint32_t k;    // generalized argument
};

// Forward declarations of the focal function and its dependency.
// The actual project provides these in external headers; here we declare them
// to enable testing in a self-contained manner.
extern "C" u_int bpf_filter(const struct bpf_insn *pc, const u_char *p, u_int wirelen,
                            u_int buflen);

// Declare the dependency that bpf_filter calls. In a real environment this would
// be provided by the PCAP library. In this test harness, we provide a local stub.
extern "C" u_int pcapint_filter(const struct bpf_insn *pc, const u_char *p,
                                u_int wirelen, u_int buflen);

// Global counter to verify that the dependency is invoked by bpf_filter.
static int g_call_count = 0;

// Stub for pcapint_filter to instrument calls and return a deterministic value.
// We increment the call counter and return the first instruction's k value
// (if available) to provide a predictable result for assertions.
extern "C" u_int pcapint_filter(const struct bpf_insn *pc, const u_char *p,
                                u_int wirelen, u_int buflen)
{
    (void)p; // unused in this stub, but kept to match signature
    (void)wirelen;
    (void)buflen;

    ++g_call_count;
    if (pc != nullptr) {
        return pc[0].k; // deterministic value derived from the first instruction
    }
    return 0;
}

// Simple test harness to collect and report results without GTest.
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

static void printSummary(const std::vector<TestResult>& results) {
    int total = (int)results.size();
    int passed = 0;
    for (const auto &r : results) if (r.passed) ++passed;
    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed.\n";
    for (const auto &r : results) {
        std::cout << (r.passed ? "[PASS] " : "[FAIL] ") << r.name << " - " << r.message << "\n";
    }
}

static TestResult makeResult(const std::string &name, bool passed, const std::string &message) {
    return TestResult{name, passed, message};
}

// Test 1: Forwarding single-instruction program. Expect bpf_filter to return k=7
// and to have invoked pcapint_filter once.
static TestResult test_bpf_filter_forwards_single_instruction() {
    // Reset invocation counter for isolated test
    g_call_count = 0;

    // Program: single instruction RET with k = 7
    // BPF_RET | BPF_K is typically encoded as code 0x06
    struct bpf_insn prog[] = { {0x06, 0, 0, 7} };
    unsigned char data[] = { 10, 20, 30, 40, 50 }; // arbitrary payload
    unsigned int wirelen = sizeof(data);
    unsigned int buflen = sizeof(data);

    u_int result = bpf_filter(prog, data, wirelen, buflen);

    bool ok = (g_call_count == 1) && (result == 7);
    std::string msg = ok ? "bpf_filter forwarded to pcapint_filter and returned expected value." :
                           "Mismatch: bpf_filter did not forward correctly or returned unexpected value.";

    return makeResult("test_bpf_filter_forwards_single_instruction", ok, msg);
}

// Test 2: Forwarding with a different k value to ensure consistency across calls.
// Expect bpf_filter to return k from the first instruction and to have invoked the
// dependency exactly once.
static TestResult test_bpf_filter_forwards_second_value() {
    g_call_count = 0;

    // Program: single instruction RET with k = 13
    struct bpf_insn prog[] = { {0x06, 0, 0, 13} };
    unsigned char data[] = { 1, 2, 3, 4 };
    unsigned int wirelen = sizeof(data);
    unsigned int buflen = sizeof(data);

    u_int result = bpf_filter(prog, data, wirelen, buflen);

    bool ok = (g_call_count == 1) && (result == 13);
    std::string msg = ok ? "Second value forwarded correctly." : "Mismatch on second value.";

    return makeResult("test_bpf_filter_forwards_second_value", ok, msg);
}

// Test 3: Forwarding with multiple instructions.
// The stub uses the first instruction's k as the return value; ensure still covers
// the case where more than one BPF instruction is supplied.
static TestResult test_bpf_filter_forwards_multiple_instructions() {
    g_call_count = 0;

    // Program: two instructions; first k = 13, second k = 99 (second should be ignored by the stub)
    struct bpf_insn prog[] = {
        {0x06, 0, 0, 13},
        {0x06, 0, 0, 99}
    };
    unsigned char data[] = { 9, 8, 7, 6 };
    unsigned int wirelen = sizeof(data);
    unsigned int buflen = sizeof(data);

    u_int result = bpf_filter(prog, data, wirelen, buflen);

    bool ok = (g_call_count == 1) && (result == 13);
    std::string msg = ok ? "Multiple-instruction program forwarded with first k value." :
                           "Mismatch with multiple-instruction program.";

    return makeResult("test_bpf_filter_forwards_multiple_instructions", ok, msg);
}

// Entry point: run all tests and print results.
int main() {
    std::vector<TestResult> results;

    results.push_back(test_bpf_filter_forwards_single_instruction());
    results.push_back(test_bpf_filter_forwards_second_value());
    results.push_back(test_bpf_filter_forwards_multiple_instructions());

    printSummary(results);
    // Return non-zero if any test failed to aid integration with CI systems
    bool anyFailed = false;
    for (const auto &r : results) if (!r.passed) { anyFailed = true; break; }
    return anyFailed ? 1 : 0;
}