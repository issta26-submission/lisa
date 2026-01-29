// Test suite for cre2_match (from cre2.cpp) without using GTest.
// This file provides a small, self-contained test harness that exercises
// the cre2_match function with various scenarios to ensure correct behavior.
// It relies on the public cre2 API and does not access private members.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


extern "C" {
}

// Simple lightweight test harness
// Non-terminating assertions: report failures but continue running other tests.
// Each test returns true on success, false on failure.

static bool test_basic_full_match() {
    // Scenario: Simple full match with no capturing groups.
    // Pattern: "abc", Text: "zzabczz" -> match "abc" at positions 2-4
    const char pattern[] = "abc";
    const int plen = (int)strlen(pattern);

    const char text[] = "zzabczz";
    const int text_len = (int)strlen(text);

    // Prepare CRE2 with default options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        std::cerr << "test_basic_full_match: failed to create options\n";
        return false;
    }
    cre2_opt_set_log_errors(opt, 0);

    cre2_regexp_t *re = cre2_new(pattern, plen, opt);
    cre2_opt_delete(opt);
    if (!re) {
        std::cerr << "test_basic_full_match: failed to compile pattern\n";
        return false;
    }

    // We want a single match (the whole pattern)
    const int nmatch = 1;
    cre2_string_t match_buf[nmatch];

    int ret = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, match_buf, nmatch);
    bool ok = true;
    if (ret != 1) {
        std::cerr << "test_basic_full_match: expected match (ret=1), got ret=" << ret << "\n";
        ok = false;
    } else {
        // Validate the matched substring content
        std::string m = std::string(match_buf[0].data, match_buf[0].length);
        if (m != "abc") {
            std::cerr << "test_basic_full_match: expected match 'abc', got '" << m << "'\n";
            ok = false;
        }
    }

    cre2_delete(re);
    return ok;
}

static bool test_capture_groups() {
    // Scenario: Pattern with capturing groups.
    // Pattern: "(ab)(c)", Text: "zzabczz" -> full "abc", group1 "ab", group2 "c"
    const char pattern[] = "(ab)(c)";
    const int plen = (int)strlen(pattern);

    const char text[] = "zzabczz";
    const int text_len = (int)strlen(text);

    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        std::cerr << "test_capture_groups: failed to create options\n";
        return false;
    }
    cre2_opt_set_log_errors(opt, 0);
    cre2_regexp_t *re = cre2_new(pattern, plen, opt);
    cre2_opt_delete(opt);
    if (!re) {
        std::cerr << "test_capture_groups: failed to compile pattern\n";
        return false;
    }

    // We expect 3 capture slots: [0]=whole match, [1]=group1, [2]=group2
    const int nmatch = 3;
    cre2_string_t match_buf[nmatch];

    int ret = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, match_buf, nmatch);
    bool ok = true;
    if (ret != 1) {
        std::cerr << "test_capture_groups: expected match (ret=1), got ret=" << ret << "\n";
        ok = false;
    } else {
        std::string whole(match_buf[0].data, match_buf[0].length);
        std::string g1(match_buf[1].data, match_buf[1].length);
        std::string g2(match_buf[2].data, match_buf[2].length);
        if (whole != "abc") {
            std::cerr << "test_capture_groups: expected whole 'abc', got '" << whole << "'\n";
            ok = false;
        }
        if (g1 != "ab") {
            std::cerr << "test_capture_groups: expected group1 'ab', got '" << g1 << "'\n";
            ok = false;
        }
        if (g2 != "c") {
            std::cerr << "test_capture_groups: expected group2 'c', got '" << g2 << "'\n";
            ok = false;
        }
    }

    cre2_delete(re);
    return ok;
}

static bool test_no_match_due_to_endpos() {
    // Scenario: Pattern exists in string but endpos is too small to include the match.
    // Pattern: "abc", Text: "zzzabc", with endpos = 5 (0..4) -> no full match within range
    const char pattern[] = "abc";
    const int plen = (int)strlen(pattern);

    const char text[] = "zzzabc";
    const int text_len = (int)strlen(text);

    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        std::cerr << "test_no_match_due_to_endpos: failed to create options\n";
        return false;
    }
    cre2_opt_set_log_errors(opt, 0);
    cre2_regexp_t *re = cre2_new(pattern, plen, opt);
    cre2_opt_delete(opt);
    if (!re) {
        std::cerr << "test_no_match_due_to_endpos: failed to compile pattern\n";
        return false;
    }

    const int nmatch = 1;
    cre2_string_t match_buf[nmatch];

    int ret = cre2_match(re, text, text_len, 0, 5, CRE2_UNANCHORED, match_buf, nmatch);
    bool ok = true;
    if (ret != 0) {
        std::cerr << "test_no_match_due_to_endpos: expected no match (ret=0), got ret=" << ret << "\n";
        ok = false;
    }

    cre2_delete(re);
    return ok;
}

static bool test_startpos_match() {
    // Scenario: Match exists when startpos is non-zero.
    // Pattern: "abc", Text: "zzabczz", Start at 2, End at 5 -> "abc" should match
    const char pattern[] = "abc";
    const int plen = (int)strlen(pattern);

    const char text[] = "zzabczz";
    const int text_len = (int)strlen(text);

    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        std::cerr << "test_startpos_match: failed to create options\n";
        return false;
    }
    cre2_opt_set_log_errors(opt, 0);
    cre2_regexp_t *re = cre2_new(pattern, plen, opt);
    cre2_opt_delete(opt);
    if (!re) {
        std::cerr << "test_startpos_match: failed to compile pattern\n";
        return false;
    }

    const int nmatch = 1;
    cre2_string_t match_buf[nmatch];

    int ret = cre2_match(re, text, text_len, 2, 5, CRE2_UNANCHORED, match_buf, nmatch);
    bool ok = true;
    if (ret != 1) {
        std::cerr << "test_startpos_match: expected match (ret=1), got ret=" << ret << "\n";
        ok = false;
    } else {
        std::string m(match_buf[0].data, match_buf[0].length);
        if (m != "abc") {
            std::cerr << "test_startpos_match: expected 'abc', got '" << m << "'\n";
            ok = false;
        }
    }

    cre2_delete(re);
    return ok;
}

static bool test_empty_text_no_match() {
    // Scenario: Empty text should not match a non-empty pattern.
    const char pattern[] = "a";
    const int plen = (int)strlen(pattern);

    const char text[] = "";
    const int text_len = 0;

    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        std::cerr << "test_empty_text_no_match: failed to create options\n";
        return false;
    }
    cre2_opt_set_log_errors(opt, 0);
    cre2_regexp_t *re = cre2_new(pattern, plen, opt);
    cre2_opt_delete(opt);
    if (!re) {
        std::cerr << "test_empty_text_no_match: failed to compile pattern\n";
        return false;
    }

    const int nmatch = 1;
    cre2_string_t match_buf[nmatch];

    int ret = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, match_buf, nmatch);
    bool ok = true;
    if (ret != 0) {
        std::cerr << "test_empty_text_no_match: expected no match (ret=0), got ret=" << ret << "\n";
        ok = false;
    }

    cre2_delete(re);
    return ok;
}

int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool (*test)()) {
        ++total;
        bool res = test();
        if (res) {
            ++passed;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    run("test_basic_full_match", test_basic_full_match);
    run("test_capture_groups", test_capture_groups);
    run("test_no_match_due_to_endpos", test_no_match_due_to_endpos);
    run("test_startpos_match", test_startpos_match);
    run("test_empty_text_no_match", test_empty_text_no_match);

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}