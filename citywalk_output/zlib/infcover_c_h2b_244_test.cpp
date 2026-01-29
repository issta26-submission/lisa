// Test suite for the focal method h2b (hex string to byte buffer) from infcover.c
// This test harness is written in C++11 without any GoogleTest framework.
// It directly calls the C implementation (h2b) and validates its behavior.
//
// Step 1 (Program Understanding) - Candidate Keywords extracted from h2b:
// - hex, len, in, re, next, val, delimiter, digits (0-9, a-f, A-F)
// - memory management: malloc, realloc
// - decoding logic: two-digit hex decoding, delimiter handling (non-hex chars between digits)
// - returns: pointer to decoded bytes, length through len, NULL on allocation failure
// - edge behavior: handling of uppercase/lowercase hex, empty input, NULL length pointer
// - domain: C standard library usage, malloc/realloc/free semantics
//
// Step 2 (Unit Test Generation) - Tests cover: basic decoding, case insensitivity,
// delimiter handling, non-hex characters, empty input, NULL length pointer, and
// chained hex sequences with delimiters.
// Step 3 (Test Case Refinement) - Tests attempt to maximize coverage of true/false branches
// and critical paths while keeping tests executable and independent.

#include <cstring>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <inftrees.h>
#include <zlib.h>
#include <stdlib.h>
#include <inflate.h>
#include <cstdlib>


extern "C" // Ensure C linkage for the focal C function
unsigned char* h2b(const char* hex, unsigned* len);

// Helper to print a vector of bytes in hex for debugging
static void print_bytes(const unsigned char* data, size_t len) {
    if (len == 0) {
        std::cout << "(empty)" << std::endl;
        return;
    }
    for (size_t i = 0; i < len; ++i) {
        printf("%02X ", data[i]);
    }
    std::cout << std::endl;
}

// Test 1: Basic decoding of a typical hex string with even length
// Input: "4a3f" -> bytes: 0x4A, 0x3F, len = 2
static void test_basic_even_length() {
    const char* hex = "4a3f";
    std::vector<unsigned char> expected = {0x4a, 0x3f};
    unsigned len = 0;
    unsigned char* out = h2b(hex, &len);
    bool pass = true;

    if (out == NULL) {
        pass = false;
    } else {
        if (len != expected.size()) pass = false;
        else if (std::memcmp(out, expected.data(), len) != 0) pass = false;
    }

    std::cout << "Test 1 - Basic even-length hex: " << (pass ? "PASS" : "FAIL") << std::endl;
    if (!pass) {
        std::cout << "  Expected len=" << expected.size() << " data: ";
        print_bytes(expected.data(), expected.size());
        std::cout << "  Got len=" << len << " data: ";
        print_bytes(out, len);
    }
    if (out) free(out);
}

// Test 2: Basic decoding with uppercase hex letters
// Input: "4A3F" -> bytes: 0x4A, 0x3F, len = 2
static void test_basic_uppercase() {
    const char* hex = "4A3F";
    std::vector<unsigned char> expected = {0x4a, 0x3f};
    unsigned len = 0;
    unsigned char* out = h2b(hex, &len);
    bool pass = true;

    if (out == NULL) pass = false;
    else {
        if (len != expected.size()) pass = false;
        else if (std::memcmp(out, expected.data(), len) != 0) pass = false;
    }

    std::cout << "Test 2 - Uppercase hex: " << (pass ? "PASS" : "FAIL") << std::endl;
    if (!pass) {
        std::cout << "  Expected len=" << expected.size() << " data: ";
        print_bytes(expected.data(), expected.size());
        std::cout << "  Got len=" << len << " data: ";
        print_bytes(out, len);
    }
    if (out) free(out);
}

// Test 3: Delimiter between digits (non-hex char in middle)
// Input: "4:3f" -> according to decoding rules, first two digits "4" delimiter -> 0x00,
// next "3f" -> 0x3F => bytes {0x00, 0x3F}, len = 2
static void test_delimiter_between_digits() {
    const char* hex = "4:3f";
    std::vector<unsigned char> expected = {0x00, 0x3f};
    unsigned len = 0;
    unsigned char* out = h2b(hex, &len);
    bool pass = true;

    if (out == NULL) pass = false;
    else {
        if (len != expected.size()) pass = false;
        else if (std::memcmp(out, expected.data(), len) != 0) pass = false;
    }

    std::cout << "Test 3 - Delimiter between digits: " << (pass ? "PASS" : "FAIL") << std::endl;
    if (!pass) {
        std::cout << "  Expected len=" << expected.size() << " data: ";
        print_bytes(expected.data(), expected.size());
        std::cout << "  Got len=" << len << " data: ";
        print_bytes(out, len);
    }
    if (out) free(out);
}

// Test 4: Non-hex character after first digit should be treated as delimiter causing a 0 byte
// Input: "4G" -> '4' creates 0x14; 'G' is non-hex and triggers delimiter path -> 0x00
static void test_non_hex_as_delimiter() {
    const char* hex = "4G";
    std::vector<unsigned char> expected = {0x00};
    unsigned len = 0;
    unsigned char* out = h2b(hex, &len);
    bool pass = true;

    if (out == NULL) pass = false;
    else {
        if (len != expected.size()) pass = false;
        else if (std::memcmp(out, expected.data(), len) != 0) pass = false;
    }

    std::cout << "Test 4 - Non-hex delimiter handling: " << (pass ? "PASS" : "FAIL") << std::endl;
    if (!pass) {
        std::cout << "  Expected len=" << expected.size() << " data: ";
        print_bytes(expected.data(), expected.size());
        std::cout << "  Got len=" << len << " data: ";
        print_bytes(out, len);
    }
    if (out) free(out);
}

// Test 5: Lowercase hex digits mixed with numbers
// Input: "a1" -> 0xA1, but decoding yields one byte 0x01 due to overflow behavior on finalization
// Expected result: len = 1, byte = 0x01
static void test_lowercase_mixed() {
    const char* hex = "a1";
    std::vector<unsigned char> expected = {0x01};
    unsigned len = 0;
    unsigned char* out = h2b(hex, &len);
    bool pass = true;

    if (out == NULL) pass = false;
    else {
        if (len != expected.size()) pass = false;
        else if (std::memcmp(out, expected.data(), len) != 0) pass = false;
    }

    std::cout << "Test 5 - Lowercase hex mixed: " << (pass ? "PASS" : "FAIL") << std::endl;
    if (!pass) {
        std::cout << "  Expected len=" << expected.size() << " data: ";
        print_bytes(expected.data(), expected.size());
        std::cout << "  Got len=" << len << " data: ";
        print_bytes(out, len);
    }
    if (out) free(out);
}

// Test 6: Simple two-digit zeroes
// Input: "00" -> 0x00, len = 1
static void test_double_zero() {
    const char* hex = "00";
    std::vector<unsigned char> expected = {0x00};
    unsigned len = 0;
    unsigned char* out = h2b(hex, &len);
    bool pass = true;

    if (out == NULL) pass = false;
    else {
        if (len != expected.size()) pass = false;
        else if (std::memcmp(out, expected.data(), len) != 0) pass = false;
    }

    std::cout << "Test 6 - Two zeros -> 0x00: " << (pass ? "PASS" : "FAIL") << std::endl;
    if (!pass) {
        std::cout << "  Expected len=" << expected.size() << " data: ";
        print_bytes(expected.data(), expected.size());
        std::cout << "  Got len=" << len << " data: ";
        print_bytes(out, len);
    }
    if (out) free(out);
}

// Test 7: Empty string input
// Input: "" -> len = 0, no bytes (behavior depends on malloc(0) and realloc with size 0)
// We simply ensure no crash and len is 0 if possible.
static void test_empty_input() {
    const char* hex = "";
    unsigned len = 0;
    unsigned char* out = h2b(hex, &len);
    bool pass = true;
    if (out != NULL) {
        if (len != 0) pass = false;
        // It might still return non-zero length depending on implementation details;
        // but if len==0, there are no bytes to compare.
        free(out);
    } else {
        // If function chose to return NULL, treat as acceptable only if len is zero by contract.
        if (len != 0) pass = false;
    }

    std::cout << "Test 7 - Empty input: " << (pass ? "PASS" : "FAIL") << std::endl;
}

// Test 8: len parameter is NULL (no length returned)
// Input: "4A3F" -> two bytes; ensure function does not crash and returns non-NULL pointer
static void test_null_len_pointer() {
    const char* hex = "4A3F";
    unsigned* len_ptr = NULL;
    unsigned char* out = h2b(hex, len_ptr);
    bool pass = (out != NULL);
    std::cout << "Test 8 - NULL len pointer: " << (pass ? "PASS" : "FAIL") << std::endl;
    if (out) free(out);
}

// Test 9: Delimiter with multiple segments
// Input: "4:3f2a" -> bytes: 0x00, 0x3F, 0x2A, len = 3
static void test_delimiter_chain() {
    const char* hex = "4:3f2a";
    std::vector<unsigned char> expected = {0x00, 0x3f, 0x2a};
    unsigned len = 0;
    unsigned char* out = h2b(hex, &len);
    bool pass = true;

    if (out == NULL) pass = false;
    else {
        if (len != expected.size()) pass = false;
        else if (std::memcmp(out, expected.data(), len) != 0) pass = false;
    }

    std::cout << "Test 9 - Delimiter chain (4:3f2a): " << (pass ? "PASS" : "FAIL") << std::endl;
    if (!pass) {
        std::cout << "  Expected len=" << expected.size() << " data: ";
        print_bytes(expected.data(), expected.size());
        std::cout << "  Got len=" << len << " data: ";
        print_bytes(out, len);
    }
    if (out) free(out);
}

// Main runner to execute all tests
int main(void) {
    std::cout << "Starting test suite for h2b (infcover.c) under C++11" << std::endl;
    test_basic_even_length();
    test_basic_uppercase();
    test_delimiter_between_digits();
    test_non_hex_as_delimiter();
    test_lowercase_mixed();
    test_double_zero();
    test_empty_input();
    test_null_len_pointer();
    test_delimiter_chain();
    std::cout << "Test suite finished." << std::endl;
    return 0;
}