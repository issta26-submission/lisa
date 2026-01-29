// C++11 test suite for the C focal function fscan_pnm_magic (pnm2png.c)
// Notes:
// - No GTest is used. A small in-house test harness prints PASS/FAIL per test.
// - Tests exercise true/false branches of the predicate checks inside fscan_pnm_magic.
// - We rely on the actual implementation of fscan_pnm_token for token parsing.
// - Tests use stdio FILE* via tmpfile() to simulate input streams.

#include <string>
#include <limits.h>
#include <functional>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>


// Declare the focal C functions for linking with the C implementation.
// Using extern "C" to avoid C++ name mangling issues.
extern "C" int fscan_pnm_magic(FILE *pnm_file, char *magic_buf, size_t magic_buf_size);
extern "C" int fscan_pnm_token(FILE *pnm_file, char *token_buf, size_t token_buf_size);

// Helper: create a temporary file with provided data (empty data yields an empty file).
static FILE* make_temp_file(const char* data) {
    FILE* f = tmpfile();
    if (!f) return nullptr;
    if (data && data[0] != '\0') {
        size_t len = std::strlen(data);
        if (std::fwrite(data, 1, len, f) != len) {
            std::fclose(f);
            return nullptr;
        }
    }
    // Rewind to start so the focal function can read from the beginning.
    std::rewind(f);
    return f;
}

// Simple in-house test harness: run a test lambda and print PASS/FAIL.
static int failures = 0;
static void run_test(const std::string& name, const std::function<bool()>& test) {
    bool ok = test();
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << "\n";
    if (!ok) ++failures;
}

// Test 1: EOF on input -> fscan_pnm_magic should return 0.
static bool test_fscan_pnm_magic_eof() {
    FILE* f = make_temp_file("");  // Empty file -> immediate EOF
    if (!f) return false;
    char magic_buf[4];
    int ret = fscan_pnm_magic(f, magic_buf, 4);
    std::fclose(f);
    return ret == 0;
}

// Test 2: First character not 'P' -> should return 0.
static bool test_fscan_pnm_magic_not_p() {
    FILE* f = make_temp_file("X1");
    if (!f) return false;
    char magic_buf[4];
    int ret = fscan_pnm_magic(f, magic_buf, 4);
    std::fclose(f);
    return ret == 0;
}

// Test 3: magic_buf_size too small (< 4) -> should return -1.
static bool test_fscan_pnm_magic_small_buf() {
    FILE* f = make_temp_file("P1");
    if (!f) return false;
    char magic_buf[4];
    int ret = fscan_pnm_magic(f, magic_buf, 3); // intentionally too small
    std::fclose(f);
    return ret == -1;
}

// Test 4: Ret from fscan_pnm_token < 1 path (simulate by providing only 'P' and nothing else)
static bool test_fscan_pnm_magic_ret_lt_1() {
    // Provide only 'P' to ensure token read fails (no digit following)
    FILE* f = make_temp_file("P");
    if (!f) return false;
    char magic_buf[4];
    int ret = fscan_pnm_magic(f, magic_buf, 4);
    std::fclose(f);
    // Expecting 0 due to token read failure path (ret < 1)
    return ret == 0;
}

// Test 5: Bad two-character token like "P0" (digit outside '1'..'9') -> should return 0.
static bool test_fscan_pnm_magic_bad_digit() {
    FILE* f = make_temp_file("P0");
    if (!f) return false;
    char magic_buf[4];
    int ret = fscan_pnm_magic(f, magic_buf, 4);
    std::fclose(f);
    return ret == 0;
}

// Test 6: Token longer than two characters (e.g., "P1X") -> magic_buf[2] != '\\0' -> returns 0.
static bool test_fscan_pnm_magic_long_token() {
    FILE* f = make_temp_file("P1X");
    if (!f) return false;
    char magic_buf[4];
    int ret = fscan_pnm_magic(f, magic_buf, 4);
    std::fclose(f);
    return ret == 0;
}

// Test 7: Valid token "P9" (two-char token 'P' + digit '9') -> should return 1.
static bool test_fscan_pnm_magic_valid() {
    FILE* f = make_temp_file("P9");
    if (!f) return false;
    char magic_buf[4];
    int ret = fscan_pnm_magic(f, magic_buf, 4);
    // Optional: verify buffer content matches "P9"
    bool buf_ok = false;
    if (ret == 1) {
        // magic_buf should contain two characters: 'P' and '9', followed by '\0'
        buf_ok = (magic_buf[0] == 'P' && magic_buf[1] == '9' && magic_buf[2] == '\0');
    }
    std::fclose(f);
    return ret == 1 && buf_ok;
}

// Test 8: Valid token with trailing newline after "P5" (ensures tokenizer handles newline)
static bool test_fscan_pnm_magic_valid_with_newline() {
    FILE* f = make_temp_file("P5\n");
    if (!f) return false;
    char magic_buf[4];
    int ret = fscan_pnm_magic(f, magic_buf, 4);
    std::fclose(f);
    // Accept only exact 1 result and the magic string being "P5"
    bool buf_ok = false;
    if (ret == 1) {
        buf_ok = (magic_buf[0] == 'P' && magic_buf[1] == '5' && magic_buf[2] == '\0');
    }
    return ret == 1 && buf_ok;
}

int main() {
    // Run all tests
    run_test("fscan_pnm_magic_EOF_returns_0", test_fscan_pnm_magic_eof);
    run_test("fscan_pnm_magic_not_P_returns_0", test_fscan_pnm_magic_not_p);
    run_test("fscan_pnm_magic_buf_too_small_returns_minus1", test_fscan_pnm_magic_small_buf);
    run_test("fscan_pnm_magic_ret_lt_1_path", test_fscan_pnm_magic_ret_lt_1);
    run_test("fscan_pnm_magic_bad_digit_returns_0", test_fscan_pnm_magic_bad_digit);
    run_test("fscan_pnm_magic_long_token_returns_0", test_fscan_pnm_magic_long_token);
    run_test("fscan_pnm_magic_valid_token_returns_1", test_fscan_pnm_magic_valid);
    run_test("fscan_pnm_magic_valid_with_newline_returns_1", test_fscan_pnm_magic_valid_with_newline);

    // Summary
    std::cout << "\nTest suite completed. Failures: " << failures << "\n";
    return failures != 0 ? 1 : 0;
}