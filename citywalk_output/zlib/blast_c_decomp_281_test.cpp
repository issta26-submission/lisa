// Unit tests for the focal method decomp (via blast) in blast.c
// Note: This test suite is written for C++11, without Google Test.
// It exercises branches by feeding crafted input to blast() that
// trigger specific return codes from decomp: -1 (lit > 1) and -2 (dict out of range).

#include <cstring>
#include <stddef.h>
#include <blast.h>
#include <setjmp.h>
#include <cstdint>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>


extern "C" {
}

// Global buffers and state for the mock input/output
static unsigned char test_input_buf[256];
static unsigned test_input_size = 0;
static unsigned test_input_pos = 0;

// Mock input function: provides one byte at a time from test_input_buf
static unsigned in_func(void* how, unsigned char** buf) {
    if (test_input_pos >= test_input_size) {
        *buf = nullptr;
        return 0; // no more data
    }
    // Provide a pointer to the current unread byte
    *buf = test_input_buf + test_input_pos;
    // Consume exactly one byte per call to reliably feed bits()
    test_input_pos++;
    return 1;
}

// Mock output function: simply consumes the output; always succeed
static int out_func(void* how, unsigned char* buf, unsigned len) {
    // In a real test, we could collect output for verification.
    // Here, we discard it to keep tests focused on return codes.
    (void)buf; (void)len;
    return 0;
}

// Helper to run blast() with given input bytes and return ret value.
// Exposes left/in pointers to satisfy blast() API.
static int run_blast_with_input(const unsigned char* input, unsigned input_len) {
    // Copy input into the test buffer
    std::memcpy(test_input_buf, input, input_len);
    test_input_size = input_len;
    test_input_pos = 0;

    unsigned left = 0;
    unsigned char *inptr = nullptr;

    // Call into the focal method via blast()
    int ret = blast(in_func, nullptr, out_func, nullptr, &left, &inptr);

    // Optional: print debugging info
    // std::cout << "blast() returned: " << ret << ", left=" << left << "\n";

    return ret;
}

// Test 1: Ensure -1 is returned when the first 8 bits (lit) are > 1.
// We feed a single byte 0x02 as lit, which should cause early return -1.
static bool test_case_lit_gt1() {
    // Input: lit = 0x02 (binary 00000010) -> lit > 1 triggers -1
    unsigned char input[] = { 0x02 };
    int ret = run_blast_with_input(input, sizeof(input));

    if (ret != -1) {
        std::cerr << "[TEST FAILED] test_case_lit_gt1: expected -1, got " << ret << "\n";
        return false;
    }
    std::cout << "[TEST PASSED] test_case_lit_gt1: correctly returned -1 when lit > 1.\n";
    return true;
}

// Test 2: Ensure -2 is returned when dict is out of the allowed range [4..6].
// We feed lit = 0x00 (valid), then dict = 0x07 (out of range) to trigger -2.
static bool test_case_dict_out_of_range() {
    // Input: lit = 0x00, dict = 0x07
    unsigned char input[] = { 0x00, 0x07 };
    int ret = run_blast_with_input(input, sizeof(input));

    if (ret != -2) {
        std::cerr << "[TEST FAILED] test_case_dict_out_of_range: expected -2, got " << ret << "\n";
        return false;
    }
    std::cout << "[TEST PASSED] test_case_dict_out_of_range: correctly returned -2 for dict out of range.\n";
    return true;
}

int main() {
    bool all_passed = true;

    std::cout << "Starting blast decomp unit tests (C++11)...\n";

    // Run Test 1
    if (!test_case_lit_gt1()) all_passed = false;

    // Run Test 2
    if (!test_case_dict_out_of_range()) all_passed = false;

    if (all_passed) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED.\n";
        return 1;
    }
}