/*
  Test Suite for CRE2-based pattern matching functionality (mirroring
  the focal main() behavior from test-matching.c).

  This C++11 test harness exercises the following scenarios:
  - Single simple match using POSIX syntax
  - Regex with two capturing groups and retrieval of ranges
  - Literal mode matching (no regex interpretation)
  - Named capturing groups and extraction of named groups

  Notes:
  - No GTest; a lightweight in-process test harness is used.
  - Non-terminating assertions are implemented to collect all failures
    within each test and print them, rather than aborting on the first failure.
  - All CRE2 resources are properly released in both success and failure paths.
  - The tests rely solely on the CRE2 C API, wrapped in a C++11 file.
  - The code is intended to be compiled with a C++11 compiler and linked
    against the CRE2 library.

  How to run (example):
  - Ensure CRE2 library/header is available.
  - Compile: g++ -std=c++11 -o test_match test_match_suite.cpp -lcre2
  - Run: ./test_match

  The tests are implemented inside this single translation unit.
*/

#include <stdlib.h>
#include <cre2.h>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <cre2-test.h>
#include <vector>
#include <iostream>
#include <cstdio>


extern "C" {
}

/* Helper to convert CRE2 string to std::string safely */
static std::string to_std_string(const cre2_string_t& s) {
    return std::string(s.data, s.length);
}

/* Lightweight test harness with non-terminating assertion collection */
struct TestResult {
    std::string name;
    std::vector<std::string> errors;
    explicit TestResult(const char* n) : name(n) {}
    bool ok() const { return errors.empty(); }
};

static void report_result(const TestResult& r) {
    if (r.ok()) {
        std::printf("[OK] %s\n", r.name.c_str());
    } else {
        std::fprintf(stderr, "[FAIL] %s\n", r.name.c_str());
        for (const auto& e : r.errors) {
            std::fprintf(stderr, "  - %s\n", e.c_str());
        }
    }
}

/* Test 1: Single match (pattern = "ciao"), text = "ciao" */
static TestResult test_single_match() {
    TestResult tr("test_single_match: single match with simple pattern");

    // Local lambda to report non-fatal errors
    auto fail = [&](const std::string& msg) {
        tr.errors.push_back(msg);
    };

    cre2_regexp_t* rex = nullptr;
    cre2_options_t* opt = nullptr;

    const char* pattern = "ciao";
    opt = cre2_opt_new();
    if (!opt) {
        fail("Failed to allocate cre2_options_t");
        // cleanup if possible
        rex = nullptr;
        tr.errors.push_back("Allocation failure: cre2_opt_new returned null");
        return tr;
    }
    cre2_opt_set_posix_syntax(opt, 1);
    rex = cre2_new(pattern, std::strlen(pattern), opt);
    if (!rex) {
        fail("Failed to create regexp with cre2_new");
        cre2_opt_delete(opt);
        return tr;
    }
    if (cre2_error_code(rex)) {
        fail("cre2 reported error code after rex creation");
        cre2_delete(rex);
        cre2_opt_delete(opt);
        return tr;
    }

    cre2_string_t match;
    int nmatch = 1;
    int e;
    const char* text = "ciao";
    int text_len = std::strlen(text);

    e = cre2_match(rex, text, text_len, 0, text_len, CRE2_UNANCHORED, &match, nmatch);
    if (1 != e) {
        fail("Expected match return value 1 for single match test");
    } else {
        std::string got = std::string(match.data, match.length);
        if (got != "ciao") {
            fail("Expected matched text to be 'ciao' for single match");
        }
    }

    cre2_delete(rex);
    cre2_opt_delete(opt);

    return tr;
}

/* Test 2: Two groups (pattern = "(ciao) (hello)") and extraction of ranges */
static TestResult test_two_groups() {
    TestResult tr("test_two_groups: two capturing groups extraction");

    auto fail = [&](const std::string& msg) {
        tr.errors.push_back(msg);
    };

    cre2_regexp_t* rex = nullptr;
    cre2_options_t* opt = nullptr;
    const char* pattern = "(ciao) (hello)";
    opt = cre2_opt_new();
    if (!opt) {
        fail("Failed to allocate cre2_options_t");
        return tr;
    }
    rex = cre2_new(pattern, std::strlen(pattern), opt);
    if (!rex) {
        fail("Failed to create regexp for two groups");
        cre2_opt_delete(opt);
        return tr;
    }
    if (cre2_error_code(rex)) {
        fail("cre2 reported error code after rex creation (two groups)");
        cre2_delete(rex);
        cre2_opt_delete(opt);
        return tr;
    }

    int nmatch = 3;
    cre2_string_t strings[3];
    cre2_range_t ranges[3];
    int e;
    const char* text = "ciao hello";
    int text_len = std::strlen(text);

    e = cre2_match(rex, text, text_len, 0, text_len, CRE2_UNANCHORED, strings, nmatch);
    if (1 != e) {
        fail("Expected single full match with two groups (e != 1)");
        cre2_delete(rex);
        cre2_opt_delete(opt);
        return tr;
    }

    cre2_strings_to_ranges(text, ranges, strings, nmatch);

    // Full match: ranges[0]
    std::string full = std::string(text + ranges[0].start, ranges[0].past - ranges[0].start);
    if (full != "ciao hello") {
        fail("Full match text mismatch: expected 'ciao hello'");
    }

    // First group: ranges[1]
    std::string g1 = std::string(text + ranges[1].start, ranges[1].past - ranges[1].start);
    if (g1 != "ciao") {
        fail("First group mismatch: expected 'ciao'");
    }

    // Second group: ranges[2]
    std::string g2 = std::string(text + ranges[2].start, ranges[2].past - ranges[2].start);
    if (g2 != "hello") {
        fail("Second group mismatch: expected 'hello'");
    }

    cre2_delete(rex);
    cre2_opt_delete(opt);
    return tr;
}

/* Test 3: Literal option enabled (pattern treated as literal) */
static TestResult test_literal_option() {
    TestResult tr("test_literal_option: literal matching mode");

    auto fail = [&](const std::string& msg) {
        tr.errors.push_back(msg);
    };

    cre2_regexp_t* rex = nullptr;
    cre2_options_t* opt = nullptr;

    const char* pattern = "(ciao) (hello)";
    opt = cre2_opt_new();
    if (!opt) {
        fail("Failed to allocate cre2_options_t (literal test)");
        return tr;
    }

    cre2_opt_set_literal(opt, 1);
    rex = cre2_new(pattern, std::strlen(pattern), opt);
    if (!rex) {
        fail("Failed to create regexp for literal test");
        cre2_opt_delete(opt);
        return tr;
    }
    if (cre2_error_code(rex)) {
        fail("cre2 reported error code after rex creation (literal test)");
        cre2_delete(rex);
        cre2_opt_delete(opt);
        return tr;
    }

    // With literal option, the text "(ciao) (hello)" should match literally.
    int nmatch = 0;
    int e;
    const char* text = "(ciao) (hello)";
    int text_len = std::strlen(text);

    e = cre2_match(rex, text, text_len, 0, text_len, CRE2_UNANCHORED, NULL, nmatch);
    if (0 == e) {
        fail("Expected a non-zero match value with literal option enabled");
    }

    cre2_delete(rex);
    cre2_opt_delete(opt);
    return tr;
}

/* Test 4: Named capturing groups (S and D) extraction */
static TestResult test_named_groups() {
    TestResult tr("test_named_groups: named capturing groups retrieval");

    auto fail = [&](const std::string& msg) {
        tr.errors.push_back(msg);
    };

    cre2_regexp_t* rex = nullptr;
    cre2_options_t* opt = nullptr;

    const char* pattern = "from (?P<S>.*) to (?P<D>.*)";
    opt = cre2_opt_new();
    if (!opt) {
        fail("Failed to allocate cre2_options_t (named groups test)");
        return tr;
    }

    rex = cre2_new(pattern, std::strlen(pattern), opt);
    if (!rex) {
        fail("Failed to create regexp for named groups test");
        cre2_opt_delete(opt);
        return tr;
    }
    if (cre2_error_code(rex)) {
        fail("cre2 reported error code after rex creation (named groups)");
        cre2_delete(rex);
        cre2_opt_delete(opt);
        return tr;
    }

    int nmatch = cre2_num_capturing_groups(rex) + 1;
    cre2_string_t strings[nmatch];
    int e;
    const char* text = "from Montreal, Canada to Lausanne, Switzerland";
    int text_len = std::strlen(text);

    e = cre2_match(rex, text, text_len, 0, text_len, CRE2_UNANCHORED, strings, nmatch);
    if (0 == e) {
        fail("Named groups match failed (e == 0)");
        cre2_delete(rex);
        cre2_opt_delete(opt);
        return tr;
    }

    int SIndex = cre2_find_named_capturing_groups(rex, "S");
    int DIndex = cre2_find_named_capturing_groups(rex, "D");
    if (SIndex < 0) {
        fail("Named group 'S' not found");
    } else {
        std::string s = to_std_string(strings[SIndex]);
        if (s != "Montreal, Canada") {
            fail("Named group S content mismatch: expected 'Montreal, Canada'");
        }
    }
    if (DIndex < 0) {
        fail("Named group 'D' not found");
    } else {
        std::string d = to_std_string(strings[DIndex]);
        if (d != "Lausanne, Switzerland") {
            fail("Named group D content mismatch: expected 'Lausanne, Switzerland'");
        }
    }

    cre2_delete(rex);
    cre2_opt_delete(opt);
    return tr;
}

int main(void) {
    // Run all tests; do not exit on first failure to maximize coverage (non-terminating).
    TestResult r1 = test_single_match();
    TestResult r2 = test_two_groups();
    TestResult r3 = test_literal_option();
    TestResult r4 = test_named_groups();

    report_result(r1);
    report_result(r2);
    report_result(r3);
    report_result(r4);

    bool all_ok = r1.ok() && r2.ok() && r3.ok() && r4.ok();
    if (all_ok) {
        std::printf("All CRE2-focused tests PASSED.\n");
        return EXIT_SUCCESS;
    } else {
        std::fprintf(stderr, "Some CRE2-focused tests FAILED.\n");
        return EXIT_FAILURE;
    }
}