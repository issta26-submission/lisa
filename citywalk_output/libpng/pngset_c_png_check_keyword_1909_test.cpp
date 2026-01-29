// Lightweight C++11 unit tests for the focal function png_check_keyword
// Note: This test suite provides two variants of the function under test:
// 1) png_check_keyword_warn_impl: simulates the version with PNG_WARNINGS_SUPPORTED
// 2) png_check_keyword_no_warn_impl: simulates the version without warnings
// The tests do not rely on GTest; they use a small in-file test harness with
// non-terminating checks (logs failures and continues).

#include <cassert>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// ------------------------------
// Minimal type definitions to compile the test code
// (stand-ins for the libpng types used by the focal function)
// ------------------------------
typedef unsigned int png_uint_32;
typedef unsigned char png_byte;
typedef void* png_structrp;
typedef const char* png_const_charp;
typedef unsigned char* png_bytep;

// Simple macro stubs to imitate the environment in tests without full libpng
#define PNG_UNUSED(x) (void)(x)
#define PNG_NUMBER_FORMAT_02x 2
#define png_debug(level, msg) (void)(level); (void)(msg)


// ------------------------------
// Warning hooks (only active in the "with warnings" variant)
// ------------------------------
#ifdef PNG_WARNINGS_SUPPORTED

// Global container to collect warnings during tests
static std::vector<std::string> g_warnings;

static void png_warning(void*, const char* msg) {
    g_warnings.push_back(std::string("WARN:") + (msg ? msg : ""));
}

// Simulated warning parameter collection
static void png_warning_parameter(void*, int idx, const char* value) {
    (void)idx; // keep signature close to real API if needed
    g_warnings.push_back(std::string("WARN_PARAM:") + (value ? value : ""));
}
static void png_warning_parameter_signed(void*, int idx, int format, int value) {
    (void)idx; (void)format;
    g_warnings.push_back(std::string("WARN_PARAM_SIGN:") + std::to_string(value));
}
static void png_formatted_warning(void*, void*, const char* msg) {
    g_warnings.push_back(std::string("FORMATTED:") + (msg ? msg : ""));
}

// No-op macro helpers (simplified for test)
#define PNG_WARNING_PARAMETERS(p) (void)(p)
#define PNG_UNUSED1(p1) (void)(p1)

#else

// When not using warnings, provide no-ops
static void png_warning(void*, const char*) { }
static void png_warning_parameter(void*, int, const char*) { }
static void png_warning_parameter_signed(void*, int, int, int) { }
static void png_formatted_warning(void*, void*, const char*) { }
#define PNG_WARNING_PARAMETERS(p) (void)(p)
#define PNG_UNUSED1(p1) (void)(p1)

#endif // PNG_WARNINGS_SUPPORTED


// ------------------------------
// Implementations of the focal function under test
// We provide two variants to test both code paths (with and without warnings).
// The logic mirrors the code in the prompt, focusing on the keyword sanitization.
// ------------------------------

// Variant A: with warnings (simulated). Uses local implementation for test.
static uint32_t png_check_keyword_warn_impl(png_structrp png_ptr, png_const_charp key, png_bytep new_key) {
    png_const_charp orig_key = key;
    uint32_t key_len = 0;
    int bad_character = 0;
    int space = 1;
    png_debug(1, "in png_check_keyword");
    if (key == NULL) {
        *new_key = 0;
        return 0;
    }
    while (*key && key_len < 79) {
        unsigned char ch = (unsigned char)*key++;
        if ((ch > 32 && ch <= 126) || (ch >= 161)) {
            *new_key++ = ch; ++key_len; space = 0;
        } else if (space == 0) {
            *new_key++ = 32; ++key_len; space = 1;
            if (ch != 32)
                bad_character = ch;
        } else if (bad_character == 0)
            bad_character = ch; /* just skip it, record the first error */
    }
    if (key_len > 0 && space != 0) {
        --key_len; --new_key;
        if (bad_character == 0)
            bad_character = 32;
    }
    *new_key = 0;
    if (key_len == 0)
        return 0;
#ifdef PNG_WARNINGS_SUPPORTED
    if (*key != 0)
        png_warning(png_ptr, "keyword truncated");
    else if (bad_character != 0) {
        PNG_WARNING_PARAMETERS(p)
        png_warning_parameter(png_ptr, 1, orig_key);
        png_warning_parameter_signed(png_ptr, 2, PNG_NUMBER_FORMAT_02x, bad_character);
        png_formatted_warning(png_ptr, (void*)0, "keyword \"@1\": bad character '0x@2'");
    }
#else
    PNG_UNUSED(png_ptr)
#endif
    return key_len;
}

// Variant B: without warnings (no PNG_WARNINGS_SUPPORTED). Full logic without warning hooks.
static uint32_t png_check_keyword_no_warn_impl(png_structrp png_ptr, png_const_charp key, png_bytep new_key) {
    uint32_t key_len = 0;
    int bad_character = 0;
    int space = 1;

    if (key == NULL) {
        *new_key = 0;
        return 0;
    }
    while (*key && key_len < 79) {
        unsigned char ch = (unsigned char)*key++;
        if ((ch > 32 && ch <= 126) || (ch >= 161)) {
            *new_key++ = ch; ++key_len; space = 0;
        } else if (space == 0) {
            *new_key++ = 32; ++key_len; space = 1;
            if (ch != 32)
                bad_character = ch;
        } else if (bad_character == 0)
            bad_character = ch;
    }
    if (key_len > 0 && space != 0) {
        --key_len; --new_key;
        if (bad_character == 0)
            bad_character = 32;
    }
    *new_key = 0;
    if (key_len == 0)
        return 0;

    // In no-warn variant, we do not emit any warnings
    PNG_UNUSED1(png_ptr)
    return key_len;
}

// ------------------------------
// Lightweight test harness
// ------------------------------
static int g_failures = 0;

#define CHECK_EQ(expected, actual) do { \
    if ((expected) != (actual)) { \
        ++g_failures; \
        std::cerr << "FAIL at " << __FILE__ << ":" << __LINE__ \
                  << "  expected=" << (expected) << " actual=" << (actual) << "\n"; \
    } \
} while (0)

#define CHECK_STRING_EQ(expected, actual_ptr) do { \
    if (actual_ptr == nullptr) { \
        ++g_failures; \
        std::cerr << "FAIL at " << __FILE__ << ":" << __LINE__ \
                  << "  expected=" << (expected) << " actual=NULL\n"; \
    } else { \
        std::string s_expected = (expected); \
        std::string s_actual((const char*)actual_ptr); \
        if (s_expected != s_actual) { \
            ++g_failures; \
            std::cerr << "FAIL at " << __FILE__ << ":" << __LINE__ \
                      << "  expected=\"" << s_expected << "\" actual=\"" \
                      << s_actual << "\"\n"; \
        } \
    } \
} while (0)

// Helper to create a zero-terminated buffer and fill with input
static void fill(const char* s, size_t len, png_bytep dst) {
    std::memset(dst, 0, len);
    if (s) {
        std::size_t slen = std::strlen(s);
        if (slen > len - 1) slen = len - 1;
        std::memcpy(dst, s, slen);
        dst[slen] = '\0';
    } else {
        dst[0] = '\0';
    }
}

// ------------------------------
// Individual tests (with detailed comments)
// ------------------------------
void test_null_key_no_warn() {
    // Purpose: key == NULL should yield new_key = 0 and return 0
    char out[128];
    out[0] = 'X'; // pre-fill to ensure it's overwritten
    uint32_t r = png_check_keyword_no_warn_impl(nullptr, nullptr, (png_bytep)out);
    CHECK_EQ(0u, r);
    CHECK_EQ(0, out[0]);
}

void test_empty_key_no_warn() {
    // Purpose: empty key string should yield 0, new_key = 0
    char out[128];
    uint32_t r = png_check_keyword_no_warn_impl(nullptr, "", (png_bytep)out);
    CHECK_EQ(0u, r);
    CHECK_EQ(0, out[0]);
}

void test_simple_key_no_warn() {
    // Purpose: a simple alphanumeric keyword should be preserved
    char out[128];
    fill("ABC123", 128, (png_bytep)out);
    uint32_t r = png_check_keyword_no_warn_impl(nullptr, out, (png_bytep)out);
    // After call, out should contain "ABC123" and r == 6
    CHECK_EQ(6u, r);
    CHECK_STRING_EQ("ABC123", out);
}

void test_key_with_spaces_no_warn() {
    // Purpose: spaces should be collapsed to a single space between tokens
    char out[128];
    fill("A B", 128, (png_bytep)out);
    uint32_t r = png_check_keyword_no_warn_impl(nullptr, out, (png_bytep)out);
    CHECK_EQ(3u, r);
    CHECK_STRING_EQ("A B", out);
}

void test_trailing_space_no_warn() {
    // Purpose: trailing space should be trimmed
    char out[128];
    fill("ABC ", 128, (png_bytep)out);
    uint32_t r = png_check_keyword_no_warn_impl(nullptr, out, (png_bytep)out);
    CHECK_EQ(3u, r);
    CHECK_STRING_EQ("ABC", out);
}

void test_bad_character_no_warn() {
    // Purpose: a bad char (tab) between letters should be turned into a space and not crash
    char out[128];
    // Build input "A\tB" as A, tab, B
    const char* in = "A\tB";
    fill(in, 128, (png_bytep)out);
    uint32_t r = png_check_keyword_no_warn_impl(nullptr, out, (png_bytep)out);
    CHECK_EQ(3u, r);
    CHECK_STRING_EQ("A B", out);
}

void test_long_key_no_warn() {
    // Purpose: verify length-limited output (79 chars max)
    // Build a 100-character string of 'A'
    std::string big(100, 'A');
    char inbuf[256];
    std::memset(inbuf, 0, sizeof(inbuf));
    std::memcpy(inbuf, big.c_str(), big.size());
    inbuf[100] = '\0';
    char out[256];
    uint32_t r = png_check_keyword_no_warn_impl(nullptr, inbuf, (png_bytep)out);
    CHECK_EQ(79u, r);
    // Ensure output buffer contains 79 'A's
    std::string s(out, out + 79);
    CHECK_STRING_EQ(std::string(79, 'A'), s);
}

// Warning-enabled tests (collect warnings)
void test_null_key_with_warn() {
#ifdef PNG_WARNINGS_SUPPORTED
    g_warnings.clear();
    char out[128];
    uint32_t r = png_check_keyword_warn_impl(nullptr, nullptr, (png_bytep)out);
    CHECK_EQ(0u, r);
    CHECK_EQ(0, out[0]);
    // Optional: verify that no warnings were produced for NULL key
    if (!g_warnings.empty()) {
        // If warnings were produced unexpectedly, log but continue
        std::cerr << "Warning emitted for NULL key (unexpected): " << g_warnings.front() << "\n";
    }
#endif
}

void test_long_key_warn_truncation() {
#ifdef PNG_WARNINGS_SUPPORTED
    g_warnings.clear();
    // Build a 100-character string of 'A'
    std::string big(100, 'A');
    char inbuf[256];
    std::memset(inbuf, 0, sizeof(inbuf));
    std::memcpy(inbuf, big.c_str(), big.size());
    inbuf[100] = '\0';
    char out[256] = {0};
    uint32_t r = png_check_keyword_warn_impl(nullptr, inbuf, (png_bytep)out);
    // Expect 79 output length
    CHECK_EQ(79u, r);
    // Ensure a warning about truncation was produced
    bool found = false;
    for (const auto& w : g_warnings) {
        if (w.find("keyword truncated") != std::string::npos) { found = true; break; }
    }
    if (!found) {
        std::cerr << "Expected 'keyword truncated' warning not found." << std::endl;
        ++g_failures;
    }
#endif
}

void test_bad_character_warn() {
#ifdef PNG_WARNINGS_SUPPORTED
    g_warnings.clear();
    // Build input "A\tB" to trigger bad_character path
    char inbuf[8] = { 'A', '\t', 'B', '\0' };
    char out[8] = {0};
    uint32_t r = png_check_keyword_warn_impl(nullptr, inbuf, (png_bytep)out);
    // Expect length 3 ("A B")
    CHECK_EQ(3u, r);
    CHECK_STRING_EQ("A B", out);
    // Ensure some warning about bad character is emitted
    bool found_bad = false;
    for (const auto& w : g_warnings) {
        if (w.find("bad character") != std::string::npos) { found_bad = true; break; }
    }
    if (!found_bad) {
        std::cerr << "Expected a bad-character warning not found." << std::endl;
        ++g_failures;
    }
#endif
}

void test_leading_spaces_warn() {
#ifdef PNG_WARNINGS_SUPPORTED
    g_warnings.clear();
    // Input with leading spaces should skip them and take first non-space
    char inbuf[16] = { ' ', ' ', 'A', 'B', '\0' };
    char out[16] = {0};
    uint32_t r = png_check_keyword_warn_impl(nullptr, inbuf, (png_bytep)out);
    // Expect "AB" length 2 if no spaces collected unnecessarily
    CHECK_EQ(2u, r);
    CHECK_STRING_EQ("AB", out);
    // No specific warning expected for this case; ensure no crash
#endif
}

// ------------------------------
// Main: run all tests
// ------------------------------
int main() {
    int total_tests = 0;
    int total_passed = 0;

    // No-warn variant tests
    test_null_key_no_warn(); ++total_tests;
    test_empty_key_no_warn(); ++total_tests;
    test_simple_key_no_warn(); ++total_tests;
    test_key_with_spaces_no_warn(); ++total_tests;
    test_trailing_space_no_warn(); ++total_tests;
    test_bad_character_no_warn(); ++total_tests;
    test_long_key_no_warn(); ++total_tests;

#ifdef PNG_WARNINGS_SUPPORTED
    // Warn variant tests
    test_null_key_with_warn(); ++total_tests;
    test_long_key_warn_truncation(); ++total_tests;
    test_bad_character_warn(); ++total_tests;
    test_leading_spaces_warn(); ++total_tests;
#endif

    // Summarize
    total_passed = total_tests - g_failures;
    std::cout << "png_check_keyword test results: "
              << total_passed << " / " << total_tests << " tests passed." << std::endl;

    if (g_failures != 0) {
        std::cerr << g_failures << " tests failed." << std::endl;
        return 1;
    }
    return 0;
}