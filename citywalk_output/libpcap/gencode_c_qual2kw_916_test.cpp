/*
Unit test suite for the focal method:
  qual2kw(const char *kind, const unsigned id, const char *tokens[],
          const size_t size)

Notes:
- This is a C function compiled in C, tested from C++11 test harness.
- We avoid GTest; a small custom test runner is implemented that logs
  pass/fail but does not terminate on first failure (non-terminating assertions).
- Tests focus on:
  - True path: return tokens[id] when id<size and tokens[id] != NULL.
  - False path: return "<invalid kind id>" when id >= size or tokens[id] == NULL.
  - Buffer rotation behavior of the internal static thread_local buffers
    by performing consecutive invalid calls and ensuring distinct strings.
  - Static/local state usage does not affect direct return value for valid paths.

Compilation note:
- Link against the implementation file that defines qual2kw (gencode.c in the project).

*/

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
#include <string>
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


extern "C" const char* qual2kw(const char *kind, const unsigned id, const char *tokens[],
                               const size_t size);

// Simple non-terminating assertion helper
static bool assert_eq(const std::string& test_desc, const std::string& got, const std::string& expected) {
    if (got == expected) {
        std::cout << "PASS: " << test_desc << "\n";
        return true;
    } else {
        std::cout << "FAIL: " << test_desc << " | Expected: \"" << expected
                  << "\" | Got: \"" << got << "\"\n";
        return false;
    }
}

// Test 1: In-range id with non-null token should return the exact token pointer
// This verifies the true-branch: if (id < size && tokens[id]) return tokens[id];
static bool test_in_range_returns_token() {
    const char* tokens[] = { "alpha", "beta", "gamma", "delta" };
    const size_t size = 4;

    const char* res = qual2kw("kind", 1, tokens, size);
    // Expect pointer equality with tokens[1]
    return res == tokens[1];
}

// Test 2: In-range id with NULL token should take the invalid path
// This verifies the false-branch when tokens[id] is NULL
// Expected: "<invalid kind id>"
static bool test_in_range_with_null_token() {
    const char* tokens[] = { "alpha", nullptr, "gamma", "delta" };
    const size_t size = 4;

    const char* res = qual2kw("kind", 1, tokens, size);
    std::string got(res ? res : "");
    std::string expected("<invalid kind 1>");
    return got == expected;
}

// Test 3: Out-of-range id should take the invalid path
// Expected: "<invalid kind id>"
static bool test_out_of_range_id() {
    const char* tokens[] = { "alpha", "beta", "gamma", "delta" };
    const size_t size = 4;

    const char* res = qual2kw("kind", 1000, tokens, size);
    std::string got(res ? res : "");
    std::string expected("<invalid kind 1000>");
    return got == expected;
}

// Test 4: Buffer rotation (static thread_local buffers) should yield distinct strings
// Perform multiple invalid calls with different ids to ensure distinct invalid messages
static bool test_buffer_rotation_distinct_invalids() {
    const char* tokens[] = { "alpha", "beta", "gamma", "delta" }; // valid tokens, but we'll use invalid IDs
    const size_t size = 4;

    // Four consecutive invalid calls with increasing IDs
    const char* r1 = qual2kw("kind", 99, tokens, size);
    const char* r2 = qual2kw("kind", 100, tokens, size);
    const char* r3 = qual2kw("kind", 101, tokens, size);
    const char* r4 = qual2kw("kind", 102, tokens, size);

    // Copy to std::string to avoid mutation by subsequent calls
    std::string s1(r1 ? r1 : "");
    std::string s2(r2 ? r2 : "");
    std::string s3(r3 ? r3 : "");
    std::string s4(r4 ? r4 : "");

    bool ok = (s1 == "<invalid kind 99>") &&
              (s2 == "<invalid kind 100>") &&
              (s3 == "<invalid kind 101>") &&
              (s4 == "<invalid kind 102>");
    if (!ok) {
        std::cout << "FAIL: test_buffer_rotation_distinct_invalids | Got: "
                  << "[" << s1 << "], [" << s2 << "], [" << s3 << "], [" << s4 << "]\n";
    } else {
        std::cout << "PASS: test_buffer_rotation_distinct_invalids\n";
    }
    return ok;
}

// Test 5: After invalid calls, a valid in-range call should still return the correct token
static bool test_valid_after_invalid_calls_unaffected() {
    const char* tokens[] = { "alpha", "beta", "gamma", "delta" };
    const size_t size = 4;

    // Trigger some invalid calls to exercise the internal buffer rotation
    qual2kw("kind", 555, tokens, size);
    qual2kw("kind", 666, tokens, size);

    // Now perform a valid call
    const char* res = qual2kw("kind", 2, tokens, size);
    return res == tokens[2];
}

int main() {
    int total = 0;
    int failed = 0;

    // Run tests
    bool t1 = test_in_range_returns_token();
    total++;
    if (!t1) failed++;

    bool t2 = test_in_range_with_null_token();
    total++;
    if (!t2) failed++;

    bool t3 = test_out_of_range_id();
    total++;
    if (!t3) failed++;

    bool t4 = test_buffer_rotation_distinct_invalids();
    total++;
    if (!t4) failed++;

    bool t5 = test_valid_after_invalid_calls_unaffected();
    total++;
    if (!t5) failed++;

    // Summary
    std::cout << "Summary: " << (total - failed) << " passed, " << failed << " failed\n";
    return failed ? 1 : 0;
}