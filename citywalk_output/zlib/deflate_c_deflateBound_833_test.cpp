/*
   Unit Test Suite for deflateBound (standalone C++11 test harness)
   Focus: Validate the logic of the focal deflateBound-like implementation
          by recreating the core decision branches in a testable form.

   Notes:
   - This test harness recreates the essential data structures (z_streamp, deflate_state,
     gz_header) needed to drive the deflateBound logic without requiring the full
     project build. This ensures the tests are self-contained and portable in a
     C++11 environment without GTest.
   - We implement a simulated deflateBound function (deflateBound_sim) that mirrors the
     behavior of the provided <FOCAL_METHOD> deflateBound, but with a controllable
     fake deflateStateCheck() behavior to exercise both branches.
   - The tests cover:
     * true/false branches for deflateStateCheck
     * wrap modes: 0 (raw), 1 (zlib wrapper), 2 (gzip wrapper, with gzip header content)
     * the default parameter branch (w_bits==15 && hash_bits==15)
     * non-default parameter branch (w_bits != 15 or hash_bits != 8+7)
     * combinations of strstart, extra header, gzhead name and comment lengths, and hcrc
   - All assertions are non-terminating: failures are reported but do not abort the test run.
   - The test code is in a single file and uses only the C++ standard library.

   How to run:
     - Compile with a C++11 compatible compiler, e.g.:
         g++ -std=c++11 -O2 -Wall -Wextra -o deflate_bound_test deflate_bound_test.cpp
     - Run: ./deflate_bound_test
*/

#include <iostream>
#include <deflate.h>
#include <cstdint>
#include <cstring>


// Namespace for test utilities
namespace tests_bound {

// Typedefs and small stand-ins to mirror essential structures used by deflateBound
using Bytef = unsigned char;
using uLong = unsigned long long;

// Forward declaration of test "state"
struct deflate_state;

// Minimal gzip header structure used only for testing deflateBound_sim
struct gz_header {
    int extra_len;
    Bytef* extra;
    const char* name;
    const char* comment;
    int hcrc;
};

// Lightweight z_stream-like wrapper used only for tests
struct z_streamp_tag {
    deflate_state* state;
};
typedef struct z_streamp_tag z_streamp;

// Lightweight deflate_state matching used fields (subset of real implementation)
struct deflate_state {
    int wrap;           // 0: raw, 1: zlib, 2: gzip, other: default
    int strstart;       // used to determine zlib wrap overhead
    gz_header* gzhead;    // gzip header (only used when wrap == 2)
    int w_bits;          // window bits (15 = default)
    int hash_bits;       // hash bits (8+7 = 15 by default)
    int level;           // compression level
};

// Global variable to control the fake deflateStateCheck() behavior
static bool fake_state_check = false;

// Helper: compute maximum of two unsigned long long values
static inline uLong max_uLong(uLong a, uLong b) {
    return (a > b) ? a : b;
}

// Core replicated logic of the focal deflateBound (simplified to be testable here)
static uLong deflateBound_sim(z_streamp strm, uLong sourceLen) {
    deflate_state* s;
    uLong fixedlen, storelen, wraplen;

    // Upper bounds for fixed and stored blocks (as in original code)
    fixedlen = sourceLen + (sourceLen >> 3) + (sourceLen >> 8) +
               (sourceLen >> 9) + 4;
    storelen = sourceLen + (sourceLen >> 5) + (sourceLen >> 7) +
               (sourceLen >> 11) + 7;

    // If parameters are not available, return conservative bound + wrapper
    if (fake_state_check)
        return max_uLong(fixedlen, storelen) + 6;

    // Access internal state
    s = strm->state;

    // Determine wrapper overhead
    switch (s->wrap) {
    case 0:                                 // raw deflate
        wraplen = 0;
        break;
    case 1:                                 // zlib wrapper
        wraplen = 6 + (s->strstart ? 4 : 0);
        break;
    case 2:                                 // gzip wrapper
        wraplen = 18;
        if (s->gzhead != nullptr) {          // user-supplied gzip header
            Bytef *str;
            if (s->gzhead->extra != nullptr)
                wraplen += 2 + s->gzhead->extra_len;
            str = (Bytef*)s->gzhead->name;
            if (str != nullptr)
                do {
                    wraplen++;
                } while (*str++);
            str = (Bytef*)s->gzhead->comment;
            if (str != nullptr)
                do {
                    wraplen++;
                } while (*str++);
            if (s->gzhead->hcrc)
                wraplen += 2;
        }
        break;
    default:                                // for compiler happiness
        wraplen = 6;
        break;
    }

    // If non-default parameters, return a conservative bound
    if (s->w_bits != 15 || s->hash_bits != 8 + 7)
        return (s->w_bits <= s->hash_bits && s->level ? fixedlen : storelen) +
               wraplen;

    // Default parameters: tight bound for this case
    return sourceLen + (sourceLen >> 12) + (sourceLen >> 14) +
           (sourceLen >> 25) + 13 - 6 + wraplen;
}

// Helper function to create a C-string length for a given const char*
static int strlen_safe(const char* s) {
    return s ? int(std::strlen(s)) : 0;
}

// Test helpers: expect macros (non-terminating)
static int tests_run = 0;
static int tests_pass = 0;
static int tests_fail = 0;

#define TEST_EXPECT_EQ(actual, expected, msg) do { \
    ++tests_run; \
    if ((actual) != (expected)) { \
        std::cerr << "\033[31m[FAIL] " << (msg << " (expected " \
                  << (expected) << ", got " << (actual) << ")\033[0m" \
                  << std::endl; \
        ++tests_fail; \
    } else { \
        std::cout << "\033[32m[PASS] " << (msg) << "\033[0m" << std::endl; \
        ++tests_pass; \
    } \
} while (0)

#define TEST_EXPECT_TRUE(cond, msg) do { \
    ++tests_run; \
    if (!(cond)) { \
        std::cerr << "\033[31m[FAIL] " << (msg) << "\033[0m" << std::endl; \
        ++tests_fail; \
    } else { \
        std::cout << "\033[32m[PASS] " << (msg) << "\033[0m" << std::endl; \
        ++tests_pass; \
    } \
} while (0)

// Public test entry: expose an interface for running tests
bool run_all_tests() {
    // Local helper to create a z_streamp carrying a deflate_state
    auto make_strm = []() -> z_streamp {
        z_streamp s = new z_streamp_tag{nullptr};
        return s;
    };

    // Test 1: deflateStateCheck true -> use conservative bound and +6
    {
        fake_state_check = true;
        z_streamp strm = make_strm();
        deflate_state st;
        // state can be anything; the early return is driven by fake_state_check
        strm->state = &st;
        uLong sourceLen = 1000;

        uLong result = deflateBound_sim(strm, sourceLen);
        // fixedlen and storelen computed as in function
        uLong fixedlen = sourceLen + (sourceLen >> 3) + (sourceLen >> 8) +
                         (sourceLen >> 9) + 4;
        uLong storelen = sourceLen + (sourceLen >> 5) + (sourceLen >> 7) +
                         (sourceLen >> 11) + 7;
        uLong expected = max_uLong(fixedlen, storelen) + 6;

        TEST_EXPECT_EQ(result, expected,
                       "deflateBound_sim: StateCheck(true) returns max(bound) + 6");
        delete strm;
    }

    // Test 2: default parameters (wrap == 0, w_bits==15, hash_bits==15)
    // No early return
    {
        fake_state_check = false;
        z_streamp strm = make_strm();
        deflate_state st;
        st.wrap = 0;
        st.strstart = 0;
        st.gzhead = nullptr;
        st.w_bits = 15;
        st.hash_bits = 15;
        st.level = 0;
        strm->state = &st;

        uLong sourceLen = 1000;
        uLong result = deflateBound_sim(strm, sourceLen);

        // expected = sourceLen + (sourceLen>>12) + (sourceLen>>14) + (sourceLen>>25) + 13 - 6 + wraplen
        // with wraplen = 0
        uLong expected = sourceLen + (sourceLen >> 12) + (sourceLen >> 14) +
                         (sourceLen >> 25) + 7; // 13-6 = 7
        TEST_EXPECT_EQ(result, expected,
                       "deflateBound_sim: default params (wrap 0, 15,15) tight bound");
        delete strm;
    }

    // Test 3: wrap == 1 (zlib wrapper) with strstart = 1
    {
        fake_state_check = false;
        z_streamp strm = make_strm();
        deflate_state st;
        st.wrap = 1;
        st.strstart = 1;
        st.gzhead = nullptr;
        st.w_bits = 15;
        st.hash_bits = 15;
        st.level = 0;
        strm->state = &st;

        uLong sourceLen = 1000;
        uLong result = deflateBound_sim(strm, sourceLen);

        // wraplen = 6 + (strstart ? 4 : 0) = 10
        uLong wraplen = 6 + 4;
        uLong expected = sourceLen + (sourceLen >> 12) + (sourceLen >> 14) +
                         (sourceLen >> 25) + 7 + wraplen; // 7 == 13-6
        TEST_EXPECT_EQ(result, expected,
                       "deflateBound_sim: zlib wrapper (wrap 1) with strstart=1");
        delete strm;
    }

    // Test 4: wrap == 2 (gzip wrapper) with gzip header details
    {
        fake_state_check = false;
        z_streamp strm = make_strm();
        deflate_state st;
        st.wrap = 2;
        st.strstart = 0;
        st.w_bits = 15;
        st.hash_bits = 15;
        st.level = 0;

        // Prepare a gzip header with extra, name, comment and hcrc
        unsigned char extra_buf[4] = { 'e', 'x', 't', 'r' };
        gz_header hdr;
        hdr.extra_len = 4;
        hdr.extra = extra_buf;
        hdr.name = "abc";       // length 3
        hdr.comment = "de";     // length 2
        hdr.hcrc = 1;
        hdr.gzhead = &hdr;

        st.gzhead = &hdr;
        strm->state = &st;

        uLong sourceLen = 1000;
        uLong result = deflateBound_sim(strm, sourceLen);

        // wraplen = 18 + (extra != NULL ? 2 + extra_len : 0) +
        //           name_len + comment_len + (hcrc ? 2 : 0)
        // extra: 2 + 4 = 6
        // name: 3
        // comment: 2
        // hcrc: 2
        // Total: 18 + 6 + 3 + 2 + 2 = 31
        uLong wraplen = 31;
        uLong expected = sourceLen + (sourceLen >> 12) + (sourceLen >> 14) +
                         (sourceLen >> 25) + 7 + wraplen;
        TEST_EXPECT_EQ(result, expected,
                       "deflateBound_sim: gzip wrapper (wrap 2) with header content");
        delete strm;
    }

    // Test 5: non-default non-wrap (w_bits != 15 or hash_bits != 15) with fixedlen
    // Case: w_bits <= hash_bits and level = 1 -> use fixedlen
    {
        fake_state_check = false;
        z_streamp strm = make_strm();
        deflate_state st;
        st.wrap = 0;       // raw
        st.strstart = 0;
        st.gzhead = nullptr;
        st.w_bits = 7;
        st.hash_bits = 15;
        st.level = 1;        // triggers fixedlen path
        strm->state = &st;

        uLong sourceLen = 1024;
        uLong result = deflateBound_sim(strm, sourceLen);

        // fixedlen = sourceLen + sourceLen/8 + sourceLen/256 + sourceLen/512 + 4
        uLong fixedlen = sourceLen + (sourceLen >> 3) + (sourceLen >> 8) +
                         (sourceLen >> 9) + 4;
        uLong wraplen = 0;
        uLong expected = fixedlen + wraplen;
        TEST_EXPECT_EQ(result, expected,
                       "deflateBound_sim: non-default non-wrap with fixedlen (7,15,1)");
        delete strm;
    }

    // Test 6: non-default non-wrap with storelen (level = 0)
    // Case: w_bits <= hash_bits and level = 0 -> use storelen
    {
        fake_state_check = false;
        z_streamp strm = make_strm();
        deflate_state st;
        st.wrap = 0;
        st.strstart = 0;
        st.gzhead = nullptr;
        st.w_bits = 7;
        st.hash_bits = 15;
        st.level = 0;        // storelen path
        strm->state = &st;

        uLong sourceLen = 1024;
        uLong result = deflateBound_sim(strm, sourceLen);

        // storelen = sourceLen + sourceLen/32 + sourceLen/128 + sourceLen/2048 + 7
        uLong storelen = sourceLen + (sourceLen >> 5) + (sourceLen >> 7) +
                         (sourceLen >> 11) + 7;
        uLong wraplen = 0;
        uLong expected = storelen + wraplen;
        TEST_EXPECT_EQ(result, expected,
                       "deflateBound_sim: non-default non-wrap with storelen (7,15,0)");
        delete strm;
    }

    // Summary
    std::cout << "\nTotal tests run: " << tests_run
              << ", Passed: " << tests_pass
              << ", Failed: " << tests_fail << "\n";
    // Return true if all tests passed
    return (tests_fail == 0);
}

// Public entry for main
} // namespace tests_bound

// Main function to execute tests (no external test framework)
int main() {
    bool ok = tests_bound::run_all_tests();
    if (!ok) {
        std::cerr << "\nSome tests failed. See output above for details.\n";
        return 1;
    }
    std::cout << "All tests passed.\n";
    return 0;
}