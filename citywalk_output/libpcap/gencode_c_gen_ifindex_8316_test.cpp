// Lightweight C++11 test harness for the focal function gen_ifindex in gencode.c
// The tests are designed to be self-contained and do not rely on Google Test.
// They use simple runtime assertions and a small set of mock functions to verify
// the branching behavior of gen_ifindex.
// Notes:
// - The test includes gencode.c directly in an extern "C" region to ensure C linkage.
// - Mocks intercept dependencies: gen_cmp, require_basic_bpf_extensions, fail_kw_on_dlt.
// - We exercise both the DLT_LINUX_SLL2 branch and the default Linux branch (non-SLL2).
// - This harness assumes a Linux-like environment where __linux__ is defined.

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <nametoaddr.h>
#include <ieee80211.h>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <cstdio>
#include <setjmp.h>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <gencode.c>
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


// Forward declaration of the compiler_state_t type will occur once gencode.c is included.
// We declare a minimal interface here so that test code can instantiate the object after inclusion.
extern "C" {
  struct compiler_state_t;
  // Prototypes for mocks that gencode.c expects. The actual signatures inside gencode.c
  // use compiler_state_t*, but we keep the mocks tolerant by using void* for the state.
  void *gen_cmp(void *cstate, int offrel, unsigned int offset, unsigned int size, unsigned int v);
  void require_basic_bpf_extensions(void *cstate, const char *kw);
  void fail_kw_on_dlt(void *cstate, const char *kw);
  // Forward declare gen_ifindex so we can call it from C++ test code.
  void *gen_ifindex(struct compiler_state_t *cstate, int ifindex);
}

// Global state captured by mocks for assertions
static struct {
  void *cstate;
  int offrel;
  unsigned offset;
  unsigned size;
  unsigned v;
} g_last_call = {0, 0, 0, 0, 0};

static int g_require_called = 0;
static int g_fail_called = 0;

// Simple dummy buffer to return from mock gen_cmp (non-NULL to indicate success)
static unsigned char g_dummy_buffer[8] = {0};

// Mock implementations (C linkage)
extern "C" {

// Intercept calls to gen_cmp and record the parameters for verification.
void *gen_cmp(void *cstate, int offrel, unsigned int offset, unsigned int size, unsigned int v) {
  g_last_call.cstate = cstate;
  g_last_call.offrel = offrel;
  g_last_call.offset = offset;
  g_last_call.size = size;
  g_last_call.v = v;
  // Return a non-NULL pointer to simulate a successful match pattern creation.
  return (void*)g_dummy_buffer;
}

// Intercept Linux BPF extension check calls.
void require_basic_bpf_extensions(void *cstate, const char *kw) {
  (void)cstate;
  (void)kw;
  g_require_called = 1;
}

// Intercept error-on-dlt keyword usage for non-Linux scenarios.
void fail_kw_on_dlt(void *cstate, const char *kw) {
  (void)cstate;
  (void)kw;
  g_fail_called = 1;
}
}

// Include the focal C source under test with C linkage.
extern "C" {
}

// Test 1: Ensure true branch for DLT_LINUX_SLL2 is taken and gen_cmp is invoked with offset 4
bool test_linux_sll2_path() {
  // Prepare a compiler_state_t instance (layout provided by gencode.c)
  struct compiler_state_t cstate;
  std::memset(&cstate, 0, sizeof(cstate));
  // Force the Linux SLL2 path
  cstate.linktype = DLT_LINUX_SLL2;

  int test_ifindex = 12345;

  // Call the focal function
  void *ret = gen_ifindex(&cstate, test_ifindex);

  // Validate the return is non-NULL (gen_cmp should have been called)
  if (ret == NULL) {
    std::fprintf(stderr, "test_linux_sll2_path: gen_ifindex returned NULL unexpectedly\n");
    return false;
  }

  // Validate that gen_cmp was invoked with expected parameters for DLT_LINUX_SLL2 path
  if (g_last_call.cstate != &cstate) {
    std::fprintf(stderr, "test_linux_sll2_path: gen_cmp called with incorrect cstate\n");
    return false;
  }
  if (g_last_call.offset != 4) {
    std::fprintf(stderr, "test_linux_sll2_path: gen_cmp offset expected 4, got %u\n", g_last_call.offset);
    return false;
  }
  if (g_last_call.v != (unsigned)test_ifindex) {
    std::fprintf(stderr, "test_linux_sll2_path: gen_cmp v (ifindex) mismatch: expected %u, got %u\n", (unsigned)test_ifindex, g_last_call.v);
    return false;
  }

  // Path validated
  return true;
}

// Test 2: Ensure default Linux path executes require_basic_bpf_extensions and then gen_cmp
bool test_non_linux_path() {
  // Prepare a compiler_state_t instance with non-Linux linktype
  struct compiler_state_t cstate;
  std::memset(&cstate, 0, sizeof(cstate));
  // Use a value other than DLT_LINUX_SLL2 to trigger the default path
  cstate.linktype = 9999;

  int test_ifindex = 54321;

  // Reset mock state
  g_require_called = 0;
  g_last_call.cstate = nullptr;
  g_last_call.offset = 0;
  g_last_call.v = 0;

  void *ret = gen_ifindex(&cstate, test_ifindex);

  // Expect a non-NULL return (gen_cmp should be invoked after require_basic_bpf_extensions)
  if (ret == NULL) {
    std::fprintf(stderr, "test_non_linux_path: gen_ifindex returned NULL unexpectedly\n");
    return false;
  }

  // Ensure require_basic_bpf_extensions was called
  if (g_require_called == 0) {
    std::fprintf(stderr, "test_non_linux_path: require_basic_bpf_extensions was not called\n");
    return false;
  }

  // Ensure the state passed to gen_cmp belongs to our cstate and that a call occurred
  if (g_last_call.cstate != &cstate) {
    std::fprintf(stderr, "test_non_linux_path: gen_cmp called with incorrect cstate\n");
    return false;
  }

  // In this test, we don't fix on the exact SKF_AD_OFF value, but ensure a call happened
  // and that the function did not crash or return NULL.
  return true;
}

int main() {
  bool ok1 = test_linux_sll2_path();
  bool ok2 = test_non_linux_path();

  if (ok1 && ok2) {
    std::puts("All tests passed.");
    return 0;
  } else {
    std::fprintf(stderr, "One or more tests failed. ok1=%d ok2=%d\n", ok1, ok2);
    return 1;
  }
}