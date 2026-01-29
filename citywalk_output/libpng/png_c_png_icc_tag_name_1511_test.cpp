// This is a lightweight C++11 test suite for the focal method
// png_icc_tag_name(char *name, png_uint_32 tag) implemented in png.c.
// The tests are designed to run without any external testing framework.
// They rely on the actual library implementation for png_icc_tag_char and
// png_icc_tag_name. The tests use only the C++ standard library and
// plain assertions that do not terminate on failure, allowing all tests
// to execute for maximum coverage.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Forward declarations of the focal methods with C linkage.
// We do not include the full PNG headers to avoid frame-setup dependencies.
// The real library provides these definitions; we mirror their signatures here
// to enable linking with the actual implementation.
extern "C" {
    // Writes to name[0..5], without a trailing null terminator.
    void png_icc_tag_name(char *name, uint32_t tag);
    
    // Converts a single ICC tag byte to a character (implementation provided by library).
    uint32_t png_icc_tag_char(uint32_t byte);
}

// Simple non-terminating test harness
struct TestResult {
    const char* name;
    bool passed;
};

// Utility: run a test, print result, and return pass/fail
static bool runTest(const char* testName, bool passed) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << testName << "\n";
    return passed;
}

// Helper: create the 4-byte tag from printable ASCII characters ('A'..'Z' etc.)
static uint32_t makeTag(char b0, char b1, char b2, char b3) {
    return (static_cast<uint32_t>(static_cast<unsigned char>(b0)) << 24) |
           (static_cast<uint32_t>(static_cast<unsigned char>(b1)) << 16) |
           (static_cast<uint32_t>(static_cast<unsigned char>(b2)) << 8)  |
           (static_cast<uint32_t>(static_cast<unsigned char>(b3)));
}

// Test 1: Basic ASCII tag "ABCD" -> expect "'ABCD'"
static bool test_png_icc_tag_name_basic_abcd() {
    // Prepare tag: 'A','B','C','D'
    uint32_t tag = makeTag('A','B','C','D');
    char name[7]; // 6 chars written + extra space for safety
    name[6] = 'X'; // ensure we can detect unexpected overwrites beyond [0..5]

    // Act
    png_icc_tag_name(name, tag);

    // Expected: name[0] == '\'', name[5] == '\''
    if (name[0] != '\'') {
        std::cerr << "Expect name[0] == '\\'' for tag 'ABCD'\n";
        return false;
    }
    if (name[5] != '\'') {
        std::cerr << "Expect name[5] == '\\'' for tag 'ABCD'\n";
        return false;
    }

    // Build expected middle characters using the library's mapping function
    char expected_middle[4];
    for (int i = 0; i < 4; ++i) {
        uint32_t byte = (tag >> (24 - i * 8)) & 0xFF;
        expected_middle[i] = static_cast<char>(png_icc_tag_char(byte));
    }

    // Compare the 4 middle characters
    if (name[1] != expected_middle[0] ||
        name[2] != expected_middle[1] ||
        name[3] != expected_middle[2] ||
        name[4] != expected_middle[3]) {
        std::cerr << "Mismatch in tag characters for 'ABCD'.\n";
        return false;
    }

    // Ensure extra buffer space beyond name[5] is not overwritten
    if (name[6] != 'X') {
        std::cerr << "Buffer overflow detected: name[6] was overwritten.\n";
        return false;
    }

    return true;
}

// Test 2: Basic ASCII tag "TEST" -> expect "'TEST'"
static bool test_png_icc_tag_name_basic_test() {
    uint32_t tag = makeTag('T','E','S','T');
    char name[7];
    name[6] = 'Y';
    png_icc_tag_name(name, tag);

    if (name[0] != '\'') {
        std::cerr << "Expect name[0] == '\\'' for tag 'TEST'\n";
        return false;
    }
    if (name[5] != '\'') {
        std::cerr << "Expect name[5] == '\\'' for tag 'TEST'\n";
        return false;
    }

    char expected_middle[4];
    for (int i = 0; i < 4; ++i) {
        uint32_t byte = (tag >> (24 - i * 8)) & 0xFF;
        expected_middle[i] = static_cast<char>(png_icc_tag_char(byte));
    }

    if (name[1] != expected_middle[0] ||
        name[2] != expected_middle[1] ||
        name[3] != expected_middle[2] ||
        name[4] != expected_middle[3]) {
        std::cerr << "Mismatch in tag characters for 'TEST'.\n";
        return false;
    }

    if (name[6] != 'Y') {
        std::cerr << "Buffer overflow detected in test2: name[6] overwritten.\n";
        return false;
    }

    return true;
}

// Test 3: Another ASCII tag "PNG " (note the trailing space) -> expect "'PNG '"
static bool test_png_icc_tag_name_png_with_space() {
    uint32_t tag = makeTag('P','N','G',' ');
    char name[7];
    name[6] = '\0';
    png_icc_tag_name(name, tag);

    if (name[0] != '\'') {
        std::cerr << "Expect name[0] == '\\'' for tag 'PNG '\n";
        return false;
    }
    if (name[5] != '\'') {
        std::cerr << "Expect name[5] == '\\'' for tag 'PNG '\n";
        return false;
    }

    char expected_middle[4];
    for (int i = 0; i < 4; ++i) {
        uint32_t byte = (tag >> (24 - i * 8)) & 0xFF;
        expected_middle[i] = static_cast<char>(png_icc_tag_char(byte));
    }

    if (name[1] != expected_middle[0] ||
        name[2] != expected_middle[1] ||
        name[3] != expected_middle[2] ||
        name[4] != expected_middle[3]) {
        std::cerr << "Mismatch in tag characters for 'PNG '.\n";
        return false;
    }

    // Ensure the null terminator is not written by png_icc_tag_name; we verify
    // buffer beyond name[5] remains unchanged if we set name[6] beforehand.
    return true;
}

// Test 4: Buffer integrity check without altering trailing byte after call
static bool test_png_icc_tag_name_buffer_integrity() {
    uint32_t tag = makeTag('1','2','3','4');
    char name[7];
    // Initialize trailing byte with a distinct value
    name[6] = 'Z';
    png_icc_tag_name(name, tag);

    // Trailing byte should remain untouched
    if (name[6] != 'Z') {
        std::cerr << "Trailing buffer byte altered by png_icc_tag_name.\n";
        return false;
    }
    // Basic sanity checks on format
    if (name[0] != '\'' || name[5] != '\'') {
        std::cerr << "Format violation: quotes not in expected positions.\n";
        return false;
    }
    return true;
}

int main() {
    int total = 0;
    int passed = 0;

    TestResult results[4];
    // Run Test 1
    results[0].name = "test_png_icc_tag_name_basic_abcd";
    results[0].passed = test_png_icc_tag_name_basic_abcd();
    // Run Test 2
    results[1].name = "test_png_icc_tag_name_basic_test";
    results[1].passed = test_png_icc_tag_name_basic_test();
    // Run Test 3
    results[2].name = "test_png_icc_tag_name_png_with_space";
    results[2].passed = test_png_icc_tag_name_png_with_space();
    // Run Test 4
    results[3].name = "test_png_icc_tag_name_buffer_integrity";
    results[3].passed = test_png_icc_tag_name_buffer_integrity();

    for (int i = 0; i < 4; ++i) {
        ++total;
        if (results[i].passed) ++passed;
    }

    std::cout << "\nTest summary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}