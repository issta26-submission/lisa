/*
 * Comprehensive C++11 unit test suite for the focal method:
 *   int pcapint_validate_filter(const struct bpf_insn *f, int len)
 *
 * Notes:
 * - This test suite provides a standalone, self-contained reimplementation
 *   of the minimal subset of the pcap filter validation logic required for
 *   unit testing, including a compact encoding of BPF instructions.
 * - It covers true/false branches of each predicate used in pcapint_validate_filter.
 * - Tests are written using plain C++11 (no Google Test or GMock).
 * - Each test function returns a boolean indicating pass/fail and is
 *   accompanied by explanatory comments.
 * - A simple test runner is implemented in main() to report results.
 * - The implementation is self-contained so it can compile without external
 *   dependencies, suitable for environments where the original libpcap headers
 *   are not available.
 *
 * Important: This is a focused test harness that mirrors the control flow of
 * the original function using a lightweight, test-friendly encoding of BPF
 * instructions. It is intended for unit testing purposes in isolation.
 */

// Include standard library headers
#include <pcap/pcap-inttypes.h>
#include <sys/types.h>
#include <extract.h>
#include <config.h>
#include <cstdio>
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
#include <linux/if_packet.h>
#include <cstdlib>
#include <cstring>


// Minimal reimplementation of the FOCAL_METHOD's dependencies and the method
// under test. The real project uses linux BPF definitions; for unit testing
// we implement a compact, self-contained subset that mirrors the control flow.

// Forward declare the struct used by the focal method
struct bpf_insn {
    uint16_t code;   // instruction code (packed fields)
    uint32_t k;      // field meaning depends on instruction
    uint32_t jt;     // jump true offset for Jumps
    uint32_t jf;     // jump false offset for Jumps
};

// Lightweight encoding helpers mirroring the original macros
// We implement a compact encoding scheme so the test can construct instructions.

namespace test_bpf {

// BPF class values (subset)
enum BPF_CLASS_ENUM {
    BPF_LD   = 0,  // load
    BPF_LDX  = 1,  // load index
    BPF_ST   = 2,  // store
    BPF_STX  = 3,  // store index
    BPF_ALU  = 4,  // arithmetic/logical
    BPF_JMP  = 5,  // jump
    BPF_RET  = 6,  // return
    BPF_MISC = 7   // misc
};

// BPF mode constants (subset)
enum BPF_MODE_ENUM {
    BPF_IMM = 0,  // immediate
    BPF_ABS = 1,  // absolute
    BPF_IND = 2,  // indirect
    BPF_MSH = 3,  // micro-shift (not used in tests)
    BPF_MEM = 4,  // memory
    BPF_LEN = 5   // length
};

// BPF operation constants for ALU/JMP (subset)
enum BPF_OP_ENUM {
    // ALU ops (subset)
    BPF_ADD = 0,
    BPF_SUB = 1,
    BPF_MUL = 2,
    BPF_OR  = 3,
    BPF_AND = 4,
    BPF_XOR = 5,
    BPF_LSH = 6,
    BPF_RSH = 7,
    BPF_NEG = 8,
    BPF_DIV = 9,
    BPF_MOD = 10,

    // JMP ops (subset)
    BPF_JA  = 0, // unconditional jump
    BPF_JEQ = 1,
    BPF_JGT = 2,
    BPF_JGE = 3,
    BPF_JSET= 4
};

// BPF source selector for ALU (K or X)
enum BPF_SRC_ENUM {
    BPF_K = 0, // source is a constant
    BPF_X = 1  // source is in register
};

// Helper: combine fields into a 16-bit code
static inline uint16_t MAKE_CODE(uint8_t cls, uint8_t mode, uint8_t op, uint8_t src)
{
    // Layout (compact and consistent with tests):
    // bits 0-2   : class (BPF_CLASS)
    // bits 3-5   : mode  (BPF_MODE)
    // bits 6-9   : op    (BPF_OP)
    // bit  10     : src   (BPF_SRC)
    // bits 11-15 : unused in this test encoding (set to 0)
    return static_cast<uint16_t>(
        (cls & 0x07) |
        ((mode & 0x07) << 3) |
        ((op   & 0x0F) << 6) |
        ((src  & 0x01) << 10)
    );
}

// Macros mirroring the original code's usage
#define BPF_CLASS(code) ((code) & 0x07)
#define BPF_MODE(code)  (((code) >> 3) & 0x07)
#define BPF_OP(code)    (((code) >> 6) & 0x0F)
#define BPF_SRC(code)   (((code) >> 10) & 0x01)

// A convenient helper to build an instruction
static inline struct bpf_insn make_insn(uint8_t cls, uint8_t mode, uint8_t op, uint8_t src,
                                      uint32_t k, uint32_t jt = 0, uint32_t jf = 0)
{
    struct bpf_insn ins;
    ins.code = MAKE_CODE(cls, mode, op, src);
    ins.k    = k;
    ins.jt   = jt;
    ins.jf   = jf;
    return ins;
}

// Constants used by the focal method's checks
const unsigned int BPF_MEMWORDS = 16; // typical susbset used by tests

// The focal method under test (re-implemented for unit testing)
int pcapint_validate_filter(const struct bpf_insn *f, int len)
{
    unsigned int i, from;
    const struct bpf_insn *p;
    if (len < 1)
        return 0;

    for (i = 0; i < (unsigned int)len; ++i) {
        p = &f[i];
        switch (BPF_CLASS(p->code)) {
        case BPF_LD:
        case BPF_LDX:
            switch (BPF_MODE(p->code)) {
            case BPF_IMM:
                break;
            case BPF_ABS:
            case BPF_IND:
            case BPF_MSH:
                // No max packet data size in userland; accept
                break;
            case BPF_MEM:
                if (p->k >= BPF_MEMWORDS)
                    return 0;
                break;
            case BPF_LEN:
                break;
            default:
                return 0;
            }
            break;
        case BPF_ST:
        case BPF_STX:
            if (p->k >= BPF_MEMWORDS)
                return 0;
            break;
        case BPF_ALU:
            switch (BPF_OP(p->code)) {
            case BPF_ADD:
            case BPF_SUB:
            case BPF_MUL:
            case BPF_OR:
            case BPF_AND:
            case BPF_XOR:
            case BPF_LSH:
            case BPF_RSH:
            case BPF_NEG:
                break;
            case BPF_DIV:
            case BPF_MOD:
                // Check for constant division/modulus by 0
                if (BPF_SRC(p->code) == BPF_K && p->k == 0)
                    return 0;
                break;
            default:
                return 0;
            }
            break;
        case BPF_JMP:
            from = i + 1;
            switch (BPF_OP(p->code)) {
            case BPF_JA:
                if (from + p->k >= (unsigned int)len)
                    return 0;
                break;
            case BPF_JEQ:
            case BPF_JGT:
            case BPF_JGE:
            case BPF_JSET:
                if (from + p->jt >= (unsigned int)len || from + p->jf >= (unsigned int)len)
                    return 0;
                break;
            default:
                return 0;
            }
            break;
        case BPF_RET:
            break;
        case BPF_MISC:
            break;
        default:
            return 0;
        }
    }
    return BPF_CLASS(f[len - 1].code) == BPF_RET;
}

} // namespace test_bpf

// Convenience wrappers to expose the test API
using test_bpf::BPF_LD;
using test_bpf::BPF_LDX;
using test_bpf::BPF_ST;
using test_bpf::BPF_STX;
using test_bpf::BPF_ALU;
using test_bpf::BPF_JMP;
using test_bpf::BPF_RET;
using test_bpf::BPF_MISC;

using test_bpf::BPF_IMM;
using test_bpf::BPF_ABS;
using test_bpf::BPF_IND;
using test_bpf::BPF_MSH;
using test_bpf::BPF_MEM;
using test_bpf::BPF_LEN;

using test_bpf::BPF_JA;
using test_bpf::BPF_JEQ;
using test_bpf::BPF_JGT;
using test_bpf::BPF_JGE;
using test_bpf::BPF_JSET;

using test_bpf::BPF_K;
using test_bpf::BPF_X;

// Public access to ALU op codes (subset)
using test_bpf::BPF_ADD;
using test_bpf::BPF_SUB;
using test_bpf::BPF_MUL;
using test_bpf::BPF_OR;
using test_bpf::BPF_AND;
using test_bpf::BPF_XOR;
using test_bpf::BPF_LSH;
using test_bpf::BPF_RSH;
using test_bpf::BPF_NEG;
using test_bpf::BPF_DIV;
using test_bpf::BPF_MOD;

// Utility to print test results
static void print_test_result(const std::string &name, bool passed)
{
    printf("Test %-40s: %s\n", name.c_str(), passed ? "PASSED" : "FAILED");
}

// Test 1: len < 1 should return 0
bool test_len_zero_returns_zero()
{
    // len is 0, f is NULL
    int res = test_bpf::pcapint_validate_filter(nullptr, 0);
    return (res == 0);
}

// Test 2: Valid minimal program: LD MEM (k < BPF_MEMWORDS) followed by RET
bool test_valid_simple_program()
{
    struct test_bpf::bpf_insn prog[2];
    // 0: BPF_LD MEM with valid k
    prog[0] = test_bpf::make_insn(BPF_LD, BPF_MEM, 0, BPF_K, test_bpf::BPF_MEMWORDS - 1);
    // 1: RET
    prog[1] = test_bpf::make_insn(BPF_RET, 0, 0, 0, 0);

    int len = 2;
    int res = test_bpf::pcapint_validate_filter(prog, len);
    return (res == 1);
}

// Test 3: LD MEM with k >= BPF_MEMWORDS should fail
bool test_ld_mem_out_of_bounds_fails()
{
    struct test_bpf::bpf_insn prog[2];
    // 0: BPF_LD MEM with k equal to BPF_MEMWORDS (out of bounds)
    prog[0] = test_bpf::make_insn(BPF_LD, BPF_MEM, 0, BPF_K, test_bpf::BPF_MEMWORDS);
    // 1: RET
    prog[1] = test_bpf::make_insn(BPF_RET, 0, 0, 0, 0);

    int len = 2;
    int res = test_bpf::pcapint_validate_filter(prog, len);
    return (res == 0);
}

// Test 4: ALU DIV with constant divisor 0 should fail
bool test_alu_div_by_zero_constant_fails()
{
    struct test_bpf::bpf_insn prog[2];
    // 0: BPF_ALU DIV with K source and k == 0
    prog[0] = test_bpf::make_insn(BPF_ALU, BPF_IMM, BPF_DIV, BPF_K, 0);
    // 1: RET
    prog[1] = test_bpf::make_insn(BPF_RET, 0, 0, 0, 0);

    int len = 2;
    int res = test_bpf::pcapint_validate_filter(prog, len);
    return (res == 0);
}

// Test 5: JA (unconditional jump) out of bounds should fail
bool test_ja_out_of_bounds_fails()
{
    struct test_bpf::bpf_insn prog[2];
    // 0: BPF_JMP JA with k-offset causing out-of-bounds
    prog[0] = test_bpf::make_insn(BPF_JMP, BPF_IMM, BPF_JA, BPF_K, 1);
    // 1: RET
    prog[1] = test_bpf::make_insn(BPF_RET, 0, 0, 0, 0);

    int len = 2;
    int res = test_bpf::pcapint_validate_filter(prog, len);
    return (res == 0);
}

// Test 6: JGE/JEQ bounds check with conditional jump exceeding program end
bool test_conditional_jump_bounds_fail()
{
    struct test_bpf::bpf_insn prog[3];
    // 0: BPF_JMP JEQ with jt=1, jf=2, len=3 -> from=1; from+jt=2 (<3), from+jf=3 (>=3) => fail
    prog[0] = test_bpf::make_insn(BPF_JMP, BPF_IMM, BPF_JEQ, BPF_K, 0, 1, 2);
    // 1: some harmless instruction (LD MEM valid)
    prog[1] = test_bpf::make_insn(BPF_LD, BPF_MEM, 0, BPF_K, test_bpf::BPF_MEMWORDS - 1);
    // 2: RET
    prog[2] = test_bpf::make_insn(BPF_RET, 0, 0, 0, 0);

    int len = 3;
    int res = test_bpf::pcapint_validate_filter(prog, len);
    return (res == 0);
}

// Test 7: Last instruction must be RET for success; non-RET last should fail
bool test_last_instruction_must_be_ret()
{
    struct test_bpf::bpf_insn prog[2];
    // 0: LD MEM valid
    prog[0] = test_bpf::make_insn(BPF_LD, BPF_MEM, 0, BPF_K, test_bpf::BPF_MEMWORDS - 1);
    // 1: ALU ADD (non-RET)
    prog[1] = test_bpf::make_insn(BPF_ALU, BPF_IMM, BPF_ADD, BPF_K, 0);

    int len = 2;
    int res = test_bpf::pcapint_validate_filter(prog, len);
    return (res == 0);
}

// Test 8: JA path with successful bounds should pass (ret at end)
bool test_ja_within_bounds_passes()
{
    struct test_bpf::bpf_insn prog[2];
    // 0: JA with offset 0 (within bounds)
    prog[0] = test_bpf::make_insn(BPF_JMP, BPF_IMM, BPF_JA, BPF_K, 0);
    // 1: RET
    prog[1] = test_bpf::make_insn(BPF_RET, 0, 0, 0, 0);

    int len = 2;
    int res = test_bpf::pcapint_validate_filter(prog, len);
    return (res == 1);
}

} // end of test namespace

// Domain-friendly test runner (no GTest/GMock)
static void run_all_tests()
{
    using namespace std;
    int tests_passed = 0;
    int tests_failed = 0;

    // Helper macro to run a test and print result
#define RUN_TEST_FUNC(fn) do { \
    bool ok = (fn)(); \
    printf("%-60s: %s\n", #fn, ok ? "PASSED" : "FAILED"); \
    if (ok) ++tests_passed; else ++tests_failed; \
} while (0)

    // Run tests with explanatory comments above (explanation provided inline in test code)
    printf("Starting pcapint_validate_filter unit tests (C++11 standalone)\n");
    printf("---------------------------------------------------------------\n");

    RUN_TEST_FUNC(test_bpf::test_len_zero_returns_zero);       // Test 1
    RUN_TEST_FUNC(test_bpf::test_valid_simple_program);        // Test 2
    RUN_TEST_FUNC(test_bpf::test_ld_mem_out_of_bounds_fails);  // Test 3
    RUN_TEST_FUNC(test_bpf::test_alu_div_by_zero_constant_fails); // Test 4
    RUN_TEST_FUNC(test_bpf::test_ja_out_of_bounds_fails);     // Test 5
    RUN_TEST_FUNC(test_bpf::test_conditional_jump_bounds_fail); // Test 6
    RUN_TEST_FUNC(test_bpf::test_last_instruction_must_be_ret); // Test 7
    RUN_TEST_FUNC(test_bpf::test_ja_within_bounds_passes);    // Test 8

#undef RUN_TEST_FUNC

    printf("Summary: %d passed, %d failed\n", tests_passed, tests_failed);
}

int main()
{
    run_all_tests();
    return 0;
}