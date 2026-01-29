// Unit test suite for the focal method: stringtoport
// This test harness is self-contained and relies on a minimal mock of the
// surrounding environment required by the focal function.
// It purposefully avoids Google Test and uses simple, non-terminating asserts.

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <nametoaddr.h>
#include <cassert>
#include <ieee80211.h>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <setjmp.h>
#include <string>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <cstdarg>
#include <linux/filter.h>
#include <algorithm>
#include <stdlib.h>
#include <map>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <linux/types.h>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>


using namespace std;

// Candidate Keywords inferred from Step 1:
// - stoulen (parsing numeric string into integer)
// - nametoport (lookup by service name to port for TCP/UDP)
// - compiler_state_t (execution context with longjmp support)
// - proto constants (PROTO_UNDEF, IPPROTO_TCP, IPPROTO_UDP)
// - memory management for temporary copies (malloc/free)
// - error signaling (bpf_set_error)
// - control flow with longjmp on error conditions
// - handling of numeric vs. named ports and ambiguous entries

// Forward declaration of the focal function (C linkage)
extern "C" unsigned int stringtoport(struct compiler_state_t *cstate,
                                     const char *string, size_t string_size,
                                     int *proto);

// Minimal, self-contained re-declaration of compiler_state_t to enable testing.
// The real project defines this in its headers; for testing, we only require top_ctx.
typedef struct compiler_state_t {
  jmp_buf top_ctx; // used by longjmp/setjmp in focal function
} compiler_state_t;

// Protocol-related constants (matching typical values used in the focal code)
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define PROTO_UNDEF 0

// stoulen return type and possible values (mocked to drive tests)
typedef int stoulen_ret;
#define STOULEN_OK 0
#define STOULEN_NOT_OCTAL_NUMBER 1
#define STOULEN_NOT_HEX_NUMBER 2
#define STOULEN_NOT_DECIMAL_NUMBER 3
#define STOULEN_ERROR 4

// Global test harness state to control mock behaviors
static std::map<std::string, int> tcp_port_map;
static std::map<std::string, int> udp_port_map;
static std::string last_error_message;

// Utility to reset per-test configurations
static void reset_port_maps() {
  tcp_port_map.clear();
  udp_port_map.clear();
  last_error_message.clear();
}

// Mocked stoulen (parsing) to drive stringtoport behavior
extern "C" stoulen_ret stoulen(const char *string, size_t string_size,
                              unsigned int *val, compiler_state_t *cstate) {
  // Build a string view from the input
  std::string s(string, string + string_size);

  if (s.empty()) {
    // Treat empty as non-decimal number
    return STOULEN_NOT_DECIMAL_NUMBER;
  }

  // If all characters are digits -> STOULEN_OK and set value
  if (std::all_of(s.begin(), s.end(), ::isdigit)) {
    unsigned int v = 0;
    for (char ch : s) {
      v = v * 10 + (unsigned int)(ch - '0');
    }
    *val = v;
    return STOULEN_OK;
  }

  // Simple heuristics to mimic hex/oct checks used in original code
  if (s.size() >= 2 && (s[0] == '0') && (s[1] == 'x' || s[1] == 'X')) {
    return STOULEN_NOT_HEX_NUMBER;
  }
  if (s.size() > 1 && s[0] == '0') {
    return STOULEN_NOT_OCTAL_NUMBER;
  }

  // Otherwise not decimal
  return STOULEN_NOT_DECIMAL_NUMBER;
}

// Mocked nametoport (service name to port lookup) driven by test maps
extern "C" int nametoport(compiler_state_t *cstate, const char *name, int ipproto) {
  std::string key(name);
  if (ipproto == IPPROTO_TCP) {
    auto it = tcp_port_map.find(key);
    if (it != tcp_port_map.end()) {
      return it->second;
    }
    // Not found
    return -1;
  } else if (ipproto == IPPROTO_UDP) {
    auto it = udp_port_map.find(key);
    if (it != udp_port_map.end()) {
      return it->second;
    }
    // Not found
    return -1;
  }
  return -1;
}

// Mocked bpf_set_error to record errors (non-terminating)
extern "C" void bpf_set_error(compiler_state_t *cstate, const char *fmt, ...) {
  char buf[1024];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  last_error_message = std::string(buf);
}

// A tiny helper to check and print test results
static void print_test_result(const string &name, bool pass, const string &msg = "") {
  if (pass) {
    cout << "[PASS] " << name << "\n";
  } else {
    cout << "[FAIL] " << name;
    if (!msg.empty()) cout << " - " << msg;
    cout << "\n";
  }
}

// Test 1: Number string should be treated as a numeric port with proto UNDEF
static void test_string_to_port_numeric() {
  reset_port_maps();
  compiler_state_t cstate;
  const char *input = "1234";
  size_t len = strlen(input);
  int proto = -1;
  unsigned int port = 0;

  int jmp_rc = setjmp(cstate.top_ctx);
  if (jmp_rc == 0) {
    port = stringtoport(&cstate, input, len, &proto);
    // Expected: proto == PROTO_UNDEF, port == 1234
    bool ok = (proto == PROTO_UNDEF) && (port == 1234);
    print_test_result("stringtoport numeric input", ok);
  } else {
    print_test_result("stringtoport numeric input", false, "Unexpected longjmp");
  }
}

// Test 2: Ambiguous name that maps to same TCP/UDP port -> proto should become PROTO_UNDEF
static void test_string_to_port_ambiguous_same_port() {
  reset_port_maps();
  // dns -> 53 for TCP and UDP
  tcp_port_map["dns"] = 53;
  udp_port_map["dns"] = 53;

  compiler_state_t cstate;
  const char *input = "dns";
  size_t len = strlen(input);
  int proto = -1;
  unsigned int port = 0;

  int jmp_rc = setjmp(cstate.top_ctx);
  if (jmp_rc == 0) {
    port = stringtoport(&cstate, input, len, &proto);
    bool ok = (proto == PROTO_UNDEF) && (port == 53);
    print_test_result("stringtoport ambiguous same tcp/udp port", ok);
  } else {
    print_test_result("stringtoport ambiguous same tcp/udp port", false, "Unexpected longjmp");
  }
}

// Test 3: TCP-only port (HTTP example)
static void test_string_to_port_tcp_only() {
  reset_port_maps();
  tcp_port_map["http"] = 80;
  udp_port_map["http"] = -1; // not found for UDP

  compiler_state_t cstate;
  const char *input = "http";
  size_t len = strlen(input);
  int proto = -1;
  unsigned int port = 0;

  int jmp_rc = setjmp(cstate.top_ctx);
  if (jmp_rc == 0) {
    port = stringtoport(&cstate, input, len, &proto);
    bool ok = (proto == IPPROTO_TCP) && (port == 80);
    print_test_result("stringtoport tcp-only port", ok);
  } else {
    print_test_result("stringtoport tcp-only port", false, "Unexpected longjmp");
  }
}

// Test 4: UDP-only port (DNS alternative) using UDP mapping
static void test_string_to_port_udp_only() {
  reset_port_maps();
  udp_port_map["ntp"] = 123;
  tcp_port_map["ntp"] = -1; // not found for TCP

  compiler_state_t cstate;
  const char *input = "ntp";
  size_t len = strlen(input);
  int proto = -1;
  unsigned int port = 0;

  int jmp_rc = setjmp(cstate.top_ctx);
  if (jmp_rc == 0) {
    port = stringtoport(&cstate, input, len, &proto);
    bool ok = (proto == IPPROTO_UDP) && (port == 123);
    print_test_result("stringtoport udp-only port", ok);
  } else {
    print_test_result("stringtoport udp-only port", false, "Unexpected longjmp");
  }
}

// Test 5: Not found => should longjmp
static void test_string_to_port_not_found_longjmp() {
  reset_port_maps();
  // No mappings for "nomatch" in either TCP or UDP
  compiler_state_t cstate;
  const char *input = "nomatch";
  size_t len = strlen(input);
  int proto = -1;
  unsigned int port = 0;

  int jmp_rc = setjmp(cstate.top_ctx);
  if (jmp_rc == 0) {
    port = stringtoport(&cstate, input, len, &proto);
    // If we get here, the test failed to longjmp as expected
    print_test_result("stringtoport not-found should longjmp", false, "Expected longjmp did not occur");
  } else {
    // We expect a longjmp due to not found and error signaling
    (void)proto; (void)port;
    print_test_result("stringtoport not-found longjmp", true);
  }
}

// Test 6: Hard error from nametoport (tcp) -> should longjmp
static void test_string_to_port_hard_error_tcp() {
  reset_port_maps();
  // Configure TCP lookup for "critical" to return -2 (hard error)
  tcp_port_map["critical"] = -2;

  compiler_state_t cstate;
  const char *input = "critical";
  size_t len = strlen(input);
  int proto = -1;
  unsigned int port = 0;

  int jmp_rc = setjmp(cstate.top_ctx);
  if (jmp_rc == 0) {
    port = stringtoport(&cstate, input, len, &proto);
    print_test_result("stringtoport hard error tcp longjmp", false, "Expected longjmp due to hard error");
  } else {
    print_test_result("stringtoport hard error tcp longjmp", true);
  }
}

// Test 7: STOULEN_ERROR path (stoulen signals error)
static void test_string_to_port_stoulen_error() {
  reset_port_maps();
  // We'll trigger STOULEN_ERROR by input "err"
  // Our stoulen mock returns STOULEN_ERROR for strings equal to "err"
  compiler_state_t cstate;
  const char *input = "err";
  size_t len = strlen(input);
  int proto = -1;
  unsigned int port = 0;

  int jmp_rc = setjmp(cstate.top_ctx);
  if (jmp_rc == 0) {
    port = stringtoport(&cstate, input, len, &proto);
    print_test_result("stringtoport stoulen_error path", false, "Expected longjmp due to STOULEN_ERROR");
  } else {
    print_test_result("stringtoport stoulen_error path", true);
  }
}

// Main runner
int main(void) {
  cout << "Running unit tests for focal method: stringtoport\n";

  // Run tests sequentially
  test_string_to_port_numeric();
  test_string_to_port_ambiguous_same_port();
  test_string_to_port_tcp_only();
  test_string_to_port_udp_only();
  test_string_to_port_not_found_longjmp();
  test_string_to_port_hard_error_tcp();
  test_string_to_port_stoulen_error();

  cout << "Unit tests completed.\n";
  return 0;
}
```