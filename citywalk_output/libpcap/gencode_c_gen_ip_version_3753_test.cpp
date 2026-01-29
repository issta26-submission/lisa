// Unit test suite for the focal method gen_ip_version in gencode.c
// This test suite does not use GTest. It relies on providing C-linkage
// stubs for dependent functions (gen_mcmp and bpf_error) so that
// gen_ip_version can be exercised in isolation.
//
// The tests cover:
// - ver = 4: ensures gen_mcmp is invoked with ver << 4 as v and 0xf0 as mask.
// - ver = 6: same as above with ver = 6.
// - invalid ver (e.g., 5): ensures bpf_error is invoked with proper arguments.
//
// Notes:
// - We provide lightweight test assertions that do not terminate the test
//   runner on failure, to maximize coverage.
// - The test runner calls test functions from main, as required when GTest isn't allowed.

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
#include <cstdint>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <cstdarg>
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


// Forward-declare types used by the focal function's signature.
// We provide minimal stand-ins here so that this test file can compile
// and link against gencode.c (which is compiled with its own definitions).
typedef struct compiler_state_t compiler_state_t;
typedef int e_offrel;
typedef unsigned char uint8_t;

// Declare the focal function as declared in gencode.c.
extern "C" int gen_ip_version(compiler_state_t *cstate, e_offrel offrel, const uint8_t ver);

// Provide C-callable stubs for the dependent functions used by gen_ip_version.
extern "C" int gen_mcmp(compiler_state_t *cstate, e_offrel offrel,
                       unsigned int offset, unsigned int size,
                       uint32_t v, uint32_t mask);

extern "C" void bpf_error(compiler_state_t *cstate, const char *fmt, ...);

// ---------------------------------------------------------
// Test scaffolding to capture calls and assertions
// ---------------------------------------------------------

// Global state captured by the stubs
static bool g_mcmp_called = false;
static unsigned int g_mcmp_offset = 0;
static unsigned int g_mcmp_size = 0;
static uint32_t g_mcmp_v = 0;
static uint32_t g_mcmp_mask = 0;

static bool g_error_called = false;
static const char* g_error_func = nullptr;
static const char* g_error_key = nullptr;
static int g_error_val = 0;

// Reset helper
static void reset_test_state() {
    g_mcmp_called = false;
    g_mcmp_offset = 0;
    g_mcmp_size = 0;
    g_mcmp_v = 0;
    g_mcmp_mask = 0;
    g_error_called = false;
    g_error_func = nullptr;
    g_error_key = nullptr;
    g_error_val = 0;
}

// Stubs implementing the behavior to observe gen_ip_version interactions

extern "C" int gen_mcmp(compiler_state_t *cstate, e_offrel offrel,
                       unsigned int offset, unsigned int size,
                       uint32_t v, uint32_t mask)
{
    // Capture arguments for verification
    g_mcmp_called = true;
    g_mcmp_offset = offset;
    g_mcmp_size = size;
    g_mcmp_v = v;
    g_mcmp_mask = mask;
    // Return a sentinel value so tests can distinguish behavior
    return 999; // arbitrary value chosen to differentiate from error paths
}

extern "C" void bpf_error(compiler_state_t *cstate, const char *fmt, ...)
{
    // Capture the format string usage and the varargs following fmt
    g_error_called = true;

    // Parse varargs: fmt, then func name, then "ver", then ver value (int)
    va_list ap;
    va_start(ap, fmt);
    const char *func_name = va_arg(ap, const char*);
    const char *key_name  = va_arg(ap, const char*);
    int ver_arg = va_arg(ap, int);
    va_end(ap);

    g_error_func = func_name;
    g_error_key = key_name;
    g_error_val = ver_arg;
}

// Lightweight assertion helpers (non-terminating)
#define TEST_EXPECT_TRUE(x) \
    if(!(x)) { std::cerr << "[TEST] Expectation failed: " #x " at " __FILE__ ":" << __LINE__ << "\n"; }

#define TEST_EXPECT_FALSE(x) TEST_EXPECT_TRUE(!(x))

#define TEST_EXPECT_EQ(a, b) \
    if(!((a) == (b))) { std::cerr << "[TEST] Expectation failed: " #a " == " #b " (" << (a) << " vs " << (b) << ") at " __FILE__ ":" << __LINE__ << "\n"; }

// Public test entry points

// Test 1: ver = 4 should call gen_mcmp with v = 4 << 4 and mask = 0xf0, offset = 0
static void test_ver4_calls_gen_mcmp() {
    reset_test_state();
    // Offrel can be any value; we pass 0 (valid in tests)
    int result = gen_ip_version(nullptr, 0 /* offrel */, 4 /* ver */);

    // Validate that gen_mcmp was invoked with expected parameters
    TEST_EXPECT_TRUE(g_mcmp_called);
    TEST_EXPECT_EQ(g_mcmp_offset, 0u);
    // size corresponds to BPF_B in production code; we don't compare against its literal value
    // Instead verify the computation result encoding in v and mask
    TEST_EXPECT_EQ(g_mcmp_v, 4u << 4);     // ver << 4
    TEST_EXPECT_EQ(g_mcmp_mask, 0xf0u);

    // Also ensure the function returned via gen_mcmp path (our stub returns 999)
    TEST_EXPECT_EQ(result, 999);
}

// Test 2: ver = 6 should call gen_mcmp with v = 6 << 4 and mask = 0xf0
static void test_ver6_calls_gen_mcmp() {
    reset_test_state();
    int result = gen_ip_version(nullptr, 0, 6);

    TEST_EXPECT_TRUE(g_mcmp_called);
    TEST_EXPECT_EQ(g_mcmp_offset, 0u);
    TEST_EXPECT_EQ(g_mcmp_v, 6u << 4); // 96
    TEST_EXPECT_EQ(g_mcmp_mask, 0xf0u);
    TEST_EXPECT_EQ(result, 999);
}

// Test 3: invalid ver (e.g., 5) should trigger bpf_error with proper parameters
static void test_invalid_ver_triggers_bpf_error() {
    reset_test_state();
    // ver = 5, which should go to default branch
    int result = gen_ip_version(nullptr, 0, 5);

    // gen_ip_version is expected to call bpf_error for invalid ver
    TEST_EXPECT_TRUE(g_error_called);
    // The test stubs record function name and key/value
    TEST_EXPECT_TRUE(g_error_func != nullptr);
    std::string fname = g_error_func ? g_error_func : "";
    TEST_EXPECT_TRUE(fname.find("gen_ip_version") != std::string::npos);
    TEST_EXPECT_TRUE(g_error_key != nullptr);
    TEST_EXPECT_TRUE(std::string(g_error_key).find("ver") != std::string::npos);
    TEST_EXPECT_EQ(g_error_val, 5);
    // The invalid path should not have invoked gen_mcmp
    TEST_EXPECT_FALSE(g_mcmp_called);
    // We do not rely on a concrete return value for the invalid path in this test
    (void)result; // suppress unused warning if compiler optimizes away
}

// ---------------------------------------------------------
// Test runner
// ---------------------------------------------------------

static void run_all_tests() {
    std::cout << "Starting tests for gen_ip_version...\n";

    test_ver4_calls_gen_mcmp();
    test_ver6_calls_gen_mcmp();
    test_invalid_ver_triggers_bpf_error();

    std::cout << "Testing complete.\n";
}

// Entrypoint for the test executable
int main() {
    run_all_tests();
    return 0;
}