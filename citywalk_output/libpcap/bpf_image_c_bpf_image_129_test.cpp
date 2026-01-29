// Unit Test Suite for the focal method: bpf_image (in bpf_image.c)
// This test suite is written in C++11 without using Google Test.
// It relies on the Linux BPF headers to obtain the correct BPF_* constants.
// The tests focus on exercising the formatting logic of bpf_image by constructing
// various BPF instruction encodings and validating the produced string content
// using substring assertions (non-terminating) to maximize coverage.
//
// Notes:
// - The struct bpf_insn layout is assumed to be compatible with the Linux kernel
//   definition:
//     struct bpf_insn { uint16_t code; uint8_t jt; uint8_t jf; int32_t k; };
// - The function bpf_image has external C linkage and is defined in bpf_image.c
//   along with any helpers like bpf_print_abs_load_operand.
// - We avoid private field access and rely on public API only.
// - We use dynamic string checks (std::string::find) to validate behavior across
//   multiple possible macro value configurations.

#include <string.h>
#include <linux/types.h>
#include <cstdint>
#include <linux/filter.h>
#include <os-proto.h>
#include <thread-local.h>
#include <stdio.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <string>
#include <iostream>
#include <pcap-types.h>


#ifdef __linux__
#endif

extern "C" {
    // Declaration of the focal function from the C source file.
    // We assume the C function uses the Linux kernel BPF struct layout.
    const char *bpf_image(const struct /* bpf_insn */ void *p, int n); // generic proxy signature
}

// Since the exact definition of struct bpf_insn is provided by the Linux header
// (which is included in bpf_image.c during build), we mirror a compatible version here
// for test construction. The actual layout must match the kernel's to ensure correctness.
struct bpf_insn {
    uint16_t code;
    uint8_t jt;
    uint8_t jf;
    int32_t k;
};

// Helper: safe substring check for test assertions
static bool contains(const std::string &haystack, const std::string &needle) {
    return haystack.find(needle) != std::string::npos;
}

// Helper: print test result
static void report_result(const std::string &name, bool pass) {
    std::cerr << (pass ? "[PASS] " : "[FAIL] ") << name << std::endl;
}

// Test 1: Default/Unimplemented path (unimp) should be produced for an unknown code
static bool test_default_unimp() {
    bpf_insn p;
    p.code = 0; // not matching any explicit case
    p.jt = 0;
    p.jf = 0;
    p.k = 0;

    const char *img = bpf_image(reinterpret_cast<const struct bpf_insn*>(&p), 0);
    if (!img) return false;

    std::string s(img);
    // Expect the operation name to be "unimp" and a hex encoding of code present
    bool ok_op = contains(s, "unimp");
    bool ok_hex = contains(s, "0x0"); // code printed as 0x%x in default path
    bool ok_prefix = !s.empty() && s[0] == '(';

    return ok_op && ok_hex && ok_prefix;
}

// Test 2: BPF_RET|BPF_K should print "ret" with a specific immediate operand
static bool test_ret_k_path() {
    bpf_insn p;
    // Use kernel-provided macros to form the exact code for RET with K
#ifdef __linux__
    p.code = static_cast<uint16_t>(BPF_RET | BPF_K);
#else
    // Fallback: if macros aren't available, skip this test gracefully
    // (returning false signals test not applicable in non-Linux env)
    return false;
#endif
    p.jt = 0;
    p.jf = 0;
    p.k = 42;

    const char *img = bpf_image(reinterpret_cast<const struct bpf_insn*>(&p), 5);
    if (!img) return false;

    std::string s(img);
    bool ok_op = contains(s, "ret");
    bool ok_operand = contains(s, "#42"); // immediate
    return ok_op && ok_operand;
}

// Test 3: BPF_RET|BPF_A path should produce "ret" with no operand
static bool test_ret_a_path() {
    bpf_insn p;
#ifdef __linux__
    p.code = static_cast<uint16_t>(BPF_RET | BPF_A);
#else
    return false;
#endif
    p.jt = 0;
    p.jf = 0;
    p.k = 0;

    const char *img = bpf_image(reinterpret_cast<const struct bpf_insn*>(&p), 10);
    if (!img) return false;

    std::string s(img);
    bool ok_op = contains(s, "ret");
    // Expect no operand for RET with A
    bool no_operand = (s.find("#") == std::string::npos);
    return ok_op && no_operand;
}

// Test 4: BPF_LD|BPF_IMM should print "ld" with an immediate operand
static bool test_ld_imm_path() {
    bpf_insn p;
#ifdef __linux__
    p.code = static_cast<uint16_t>(BPF_LD | BPF_IMM);
#else
    return false;
#endif
    p.jt = 0;
    p.jf = 0;
    p.k = 0xABCD;

    const char *img = bpf_image(reinterpret_cast<const struct bpf_insn*>(&p), 1);
    if (!img) return false;

    std::string s(img);
    bool ok_op = contains(s, "ld");
    bool has_operand = contains(s, "#0xabc"); // hex formatting as lower-case
    // Depending on width formatting, the exact hex may be lower-case
    bool ok_hex_variant = has_operand || contains(s, "#0xabcd");
    return ok_op && ok_hex_variant;
}

// Test 5: BPF_JMP|BPF_JA should print jump target with jt/jf fields
static bool test_jmp_ja_path() {
    bpf_insn p;
#ifdef __linux__
    p.code = static_cast<uint16_t>(BPF_JMP | BPF_JA);
#else
    return false;
#endif
    p.jt = 2;
    p.jf = 3;
    // The target is computed as n + 1 + p->k
    int n = 7;
    p.k = 3; // target: 7 + 1 + 3 = 11

    const char *img = bpf_image(reinterpret_cast<const struct bpf_insn*>(&p), n);
    if (!img) return false;

    std::string s(img);
    bool ok_op = contains(s, "ja");
    bool ok_jt_jf = contains(s, "jt 11") || contains(s, "jt\t11") || contains(s, "jf 12") || contains(s, "jf\t12");
    // We check for presence of "ja" and either a numeric jt or jf target
    return ok_op && ok_jt_jf;
}

// Test 6: BPF_LD|BPF_W|BPF_ABS should print "ld" with an absolute operand
static bool test_ld_abs_path() {
    bpf_insn p;
#ifdef __linux__
    p.code = static_cast<uint16_t>(BPF_LD | BPF_W | BPF_ABS);
#else
    return false;
#endif
    p.jt = 0;
    p.jf = 0;
    p.k = 0;

    const char *img = bpf_image(reinterpret_cast<const struct bpf_insn*>(&p), 0);
    if (!img) return false;

    std::string s(img);
    bool ok_op = contains(s, "ld");      // operation name
    bool has_operand = s.find("[") != std::string::npos || s.find("M[") != std::string::npos ||
                       s.find("pkt") != std::string::npos || s.find("abs") != std::string::npos;
    // We can't assert exact operand text, but we expect some load-operand representation
    return ok_op && has_operand;
}

// Main: Run all tests and report results
int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const std::string &name, bool result) {
        total++;
        if (result) {
            passed++;
        }
        report_result(name, result);
    };

    // Execute tests (logically independent)
    run("test_default_unimp", test_default_unimp());
    run("test_ret_k_path", test_ret_k_path());
    run("test_ret_a_path", test_ret_a_path());
    run("test_ld_imm_path", test_ld_imm_path());
    run("test_jmp_ja_path", test_jmp_ja_path());
    run("test_ld_abs_path", test_ld_abs_path());

    // Summary
    std::cerr << "Tests passed: " << passed << "/" << total << std::endl;
    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}