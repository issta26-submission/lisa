// C++11 test suite for the focal deflateGetDictionary function (no GTest used)
// This test suite exercises various paths of deflateGetDictionary by constructing
// a minimal zlib-like environment using the project's provided headers.
// The tests are designed to be executed from the project's test executable's main().

#include <cstring>
#include <deflate.h>
#include <iostream>
#include <vector>
#include <cstdlib>


// Import C headers from the project (deflate.c implements deflateGetDictionary)
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT_EQ(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        std::cerr << "TEST_FAIL: " << msg << " | Expected: " << (expected) \
                  << ", Actual: " << (actual) << std::endl; \
        ++g_tests_failed; \
    } else { \
        ++g_tests_passed; \
    } \
} while (0)

#define TEST_ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "TEST_FAIL: " << msg << std::endl; \
        ++g_tests_failed; \
    } else { \
        ++g_tests_passed; \
    } \
} while (0)

////////////////////////////////////////////////////////////////
// Helper utilities and test state constructors
////////////////////////////////////////////////////////////////

// Helper: allocate a z_stream_s and a deflate_state with required fields.
// This relies on the project's internal struct layout as exposed by deflate.h.
// We populate only fields used by deflateGetDictionary:
//   - strstart, lookahead, w_size, window
//   - strm->state points to a deflate_state with the above fields set.
static z_streamp make_strm_with_state(unsigned w_size,
                                      unsigned strstart,
                                      unsigned lookahead,
                                      std::vector<unsigned char>& window_buf)
{
    // Allocate z_stream_s from C (structure layout assumed by the project)
    z_streamp strm = (z_streamp)std::malloc(sizeof(z_stream_s));
    if (!strm) return nullptr;
    std::memset(strm, 0, sizeof(z_stream_s));

    // Allocate and initialize internal deflate_state
    deflate_state *s = (deflate_state*)std::malloc(sizeof(deflate_state));
    if (!s) {
        std::free(strm);
        return nullptr;
    }
    std::memset(s, 0, sizeof(deflate_state));

    // Setup window buffer (the test will provide a large window to test boundary cases)
    if (window_buf.empty()) {
        // If caller didn't provide a window buffer, allocate a default small one
        window_buf.resize(w_size * 2);
        for (size_t i = 0; i < window_buf.size(); ++i) window_buf[i] = static_cast<unsigned char>(i & 0xFF);
    }
    s->window = window_buf.data();
    s->w_size = w_size;
    s->strstart = strstart;
    s->lookahead = lookahead;

    strm->state = s;
    return strm;
}

// Cleanup helper
static void destroy_strm(z_streamp strm)
{
    if (!strm) return;
    if (strm->state) {
        std::free(strm->state);
    }
    std::free(strm);
}

////////////////////////////////////////////////////////////////
// Test 1: Null pointer for strm should yield Z_STREAM_ERROR
////////////////////////////////////////////////////////////////

static bool test_deflateGetDictionary_null_strm()
{
    // Case: deflateStateCheck(strm) should fail for NULL strm
    // We expect deflateGetDictionary(NULL, nonNullDict, &len) to return Z_STREAM_ERROR
    unsigned char dictBuf[16];
    unsigned dictLen = 0;

    int ret = deflateGetDictionary(nullptr, dictBuf, &dictLen);

    // Z_STREAM_ERROR macro should be defined by the project (zlib-like)
    TEST_ASSERT_EQ(Z_STREAM_ERROR, ret, "deflateGetDictionary should return Z_STREAM_ERROR for NULL strm");
    return true;
}

////////////////////////////////////////////////////////////////
// Test 2: Basic copy path verifies correct dictionary copy and length
////////////////////////////////////////////////////////////////

static bool test_deflateGetDictionary_basic_copy()
{
    // Arrange: valid strm and state, non-null dictionary, dictLength provided
    const unsigned w_size = 1024;
    unsigned strstart = 100;
    unsigned lookahead = 50; // len = 150 (<= w_size)

    std::vector<unsigned char> window(2 * w_size); // 2*w_size for safety
    for (unsigned i = 0; i < window.size(); ++i) window[i] = static_cast<unsigned char>(i & 0xFF);

    z_streamp strm = make_strm_with_state(w_size, strstart, lookahead, window);
    if (!strm) {
        std::cerr << "Failed to allocate test strm/state" << std::endl;
        ++g_tests_failed;
        return false;
    }

    // Prepare dictionary buffer to receive data
    const unsigned len_expected = strstart + lookahead; // 150
    std::vector<unsigned char> dict(len_expected);
    unsigned dictLen = 0;

    int ret = deflateGetDictionary(strm, dict.data(), &dictLen);

    // Expected results
    TEST_ASSERT_EQ(Z_OK, ret, "deflateGetDictionary should return Z_OK for valid strm");
    TEST_ASSERT_EQ(len_expected, dictLen, "dictLength should equal computed len (150)");
    // Expected copied data: window[start .. start+len-1] where start = strstart + lookahead - len
    unsigned startIndex = strstart + lookahead - len_expected; // 0
    for (unsigned i = 0; i < dictLen; ++i) {
        TEST_ASSERT_EQ(window[startIndex + i], dict[i],
                       "Dictionary contents should match window data");
    }

    destroy_strm(strm);
    return true;
}

////////////////////////////////////////////////////////////////
// Test 3: dictLength is NULL; ensure no attempt to write length but copy if possible
////////////////////////////////////////////////////////////////

static bool test_deflateGetDictionary_null_dictLength()
{
    // Arrange: valid strm and state, non-null dictionary, but dictLength is NULL
    const unsigned w_size = 512;
    unsigned strstart = 50;
    unsigned lookahead = 30; // len = 80

    std::vector<unsigned char> window(2 * w_size);
    for (unsigned i = 0; i < window.size(); ++i) window[i] = static_cast<unsigned char>(237 - (i & 0xFF));

    z_streamp strm = make_strm_with_state(w_size, strstart, lookahead, window);
    if (!strm) {
        std::cerr << "Failed to allocate test strm/state" << std::endl;
        ++g_tests_failed;
        return false;
    }

    std::vector<unsigned char> dict(80);
    // dictLength is NULL to test that path
    int ret = deflateGetDictionary(strm, dict.data(), nullptr);

    TEST_ASSERT_EQ(Z_OK, ret, "deflateGetDictionary should return Z_OK even when dictLength is NULL");
    // Verify contents copied
    unsigned startIndex = strstart + lookahead - 80; // 50+30-80 = 0
    for (unsigned i = 0; i < 80; ++i) {
        TEST_ASSERT_EQ(window[startIndex + i], dict[i], "Dictionary contents should match window data when dictLength is NULL");
    }

    destroy_strm(strm);
    return true;
}

////////////////////////////////////////////////////////////////
// Test 4: dictionary is NULL; ensure dictLength is still set (if non-NULL) and no copy is attempted
////////////////////////////////////////////////////////////////

static bool test_deflateGetDictionary_null_dictionary()
{
    // Arrange: valid strm and state, dictionary is NULL, dictLength provided
    const unsigned w_size = 256;
    unsigned strstart = 20;
    unsigned lookahead = 60; // len = 80

    std::vector<unsigned char> window(2 * w_size);
    for (unsigned i = 0; i < window.size(); ++i) window[i] = static_cast<unsigned char>(i * 3);

    z_streamp strm = make_strm_with_state(w_size, strstart, lookahead, window);
    if (!strm) {
        std::cerr << "Failed to allocate test strm/state" << std::endl;
        ++g_tests_failed;
        return false;
    }

    unsigned dictLen = 0;
    int ret = deflateGetDictionary(strm, nullptr, &dictLen);

    TEST_ASSERT_EQ(Z_OK, ret, "deflateGetDictionary should return Z_OK when dictionary is NULL");
    TEST_ASSERT_EQ(strstart + lookahead, dictLen, "dictLength should equal computed len (80) even if dictionary is NULL");

    destroy_strm(strm);
    return true;
}

////////////////////////////////////////////////////////////////
// Test 5: len computation exceeds w_size (caps to w_size) and uses 2*w_size window safely
////////////////////////////////////////////////////////////////

static bool test_deflateGetDictionary_len_cap()
{
    // Arrange: ensure len > w_size and verify cap to w_size with correct start index
    const unsigned w_size = 1024;
    unsigned strstart = 900; // near end
    unsigned lookahead = 200; // strstart + lookahead = 1100 > w_size => cap to w_size (1024)

    std::vector<unsigned char> window(2 * w_size);
    // Fill with a recognizable pattern
    for (unsigned i = 0; i < window.size(); ++i) window[i] = static_cast<unsigned char>((i * 7) & 0xFF);

    z_streamp strm = make_strm_with_state(w_size, strstart, lookahead, window);
    if (!strm) {
        std::cerr << "Failed to allocate test strm/state" << std::endl;
        ++g_tests_failed;
        return false;
    }

    const unsigned expected_len = w_size; // cap
    std::vector<unsigned char> dict(expected_len);
    unsigned dictLen = 0;

    int ret = deflateGetDictionary(strm, dict.data(), &dictLen);

    TEST_ASSERT_EQ(Z_OK, ret, "deflateGetDictionary should return Z_OK for cap case");
    TEST_ASSERT_EQ(expected_len, dictLen, "dictLength should be capped to w_size");

    // startIndex = strstart + lookahead - len = 900 + 200 - 1024 = 76
    unsigned startIndex = strstart + lookahead - expected_len;
    for (unsigned i = 0; i < dictLen; ++i) {
        TEST_ASSERT_EQ(window[startIndex + i], dict[i],
                       "Dictionary data should be window[startIndex .. startIndex+len-1]");
    }

    destroy_strm(strm);
    return true;
}

////////////////////////////////////////////////////////////////
// Main: run all tests
////////////////////////////////////////////////////////////////

int main()
{
    std::cout << "Running deflateGetDictionary unit tests (C++11, non-GTest)..." << std::endl;

    test_deflateGetDictionary_null_strm();
    test_deflateGetDictionary_basic_copy();
    test_deflateGetDictionary_null_dictLength();
    test_deflateGetDictionary_null_dictionary();
    test_deflateGetDictionary_len_cap();

    std::cout << "Tests completed. Passed: " << g_tests_passed << ", Failed: " << g_tests_failed << std::endl;
    // Return a non-zero exit code if any test failed
    return g_tests_failed ? 1 : 0;
}