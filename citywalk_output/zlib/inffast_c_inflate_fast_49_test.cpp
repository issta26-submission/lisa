/*
This test suite targets the inflate_fast path indirectly by exercising
zlib's inflate() behavior through end-to-end (deflate -> inflate) round trips
and by introducing controlled failure scenarios. While the internal
inflate_fast function in inffast.c is not directly callable via a public API,
rounded tests ensure the same functional path is executed under realistic data
conditions and that key branches within the inflation logic are exercised.

Step-by-step mapping to the requested steps:
- Step 1 (Program Understanding): Candidate keywords representative of the focal method's core dependencies
  - inflate_fast, z_streamp, lencode, distcode, window, hold, bits, end-of-block
  - distance/length decoding, literal/length handling, window copying, end-of-block
  - input/output buffering: next_in, avail_in, next_out, avail_out
  - error handling: BAD state, invalid distance code, invalid literal/length code
  - state machine: inflate_state, dmax (when INFLATE_STRICT is defined)
- Step 2 (Unit Test Generation): We build integration-style tests around inflate-like behavior
  - Use real zlib deflate/inflate to exercise the inflate path (which uses inflate_fast internally in the library)
  - Test true/false branches via normal correctness tests and a controlled failure
  - Cover end-of-block and distance-based copying implicitly through representative data
- Step 3 (Test Case Refinement): Use C++11 standard library, no GoogleTest, and a small in-code test harness
  - Validate data integrity for round-trip scenarios
  - Validate proper error on deliberately corrupted input
  - Ensure tests are executable under a C++11 compiler with zlib available

Notes:
- The tests rely on the zlib public API (deflate/inflate) to trigger the internal inflate_fast code path.
- No private methods/fields are accessed directly; tests operate through the public API.
- The test harness is self-contained and uses a minimal assertion mechanism suitable for C++11.
- Compile with zlib linked (e.g., -lz). For example: g++ -std=c++11 -O2 test_inflate_fast.cpp -lz -o test_inflate_fast

*/

// Candidate Keywords (from Step 1) for reference within comments:
// inflate_fast, z_streamp, lencode, distcode, window, hold, bits, end-of-block, literal/length,
// distance/base, op, len, dist, beg, out, in, last, beg, end, wsize, whave, wnext, window,
// invalid distance, invalid distance code, invalid literal/length code, TYPE, BAD,  INFLATE_STRICT,
// INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR, s/0x20..0x7f literal/

#include <cstring>
#include <cassert>
#include <string>
#include <iostream>
#include <vector>
#include <inftrees.h>
#include <zlib.h>
#include <inffast.h>
#include <zutil.h>
#include <inflate.h>



// Simple test harness: basic assertions with descriptive output
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { \
    std::cerr << "ASSERT FAILED: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
    return false; } } while(0)

#define TEST_PASS(msg) do { std::cout << "[PASS] " << (msg) << std::endl; } while(0)
#define TEST_FAIL(msg) do { std::cerr << "[FAIL] " << (msg) << std::endl; } while(0)

// Helper: compress input using zlib deflate
static bool compress_data(const std::string& input, std::vector<unsigned char>& out) {
    z_stream zs;
    std::memset(&zs, 0, sizeof(zs));
    int ret = deflateInit(&zs, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) return false;

    zs.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    zs.avail_in = static_cast<uInt>(input.size());

    const size_t CHUNK = 16384;
    std::vector<unsigned char> buffer;
    buffer.resize(CHUNK);

    do {
        zs.next_out = buffer.data();
        zs.avail_out = static_cast<uInt>(buffer.size());

        ret = deflate(&zs, (zs.avail_in == 0) ? Z_FINISH : Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR) {
            deflateEnd(&zs);
            return false;
        }

        size_t have = buffer.size() - zs.avail_out;
        if (have > 0) {
            out.insert(out.end(), buffer.begin(), buffer.begin() + have);
        }
    } while (ret != Z_STREAM_END);

    deflateEnd(&zs);
    return true;
}

// Helper: decompress input using zlib inflate
static bool decompress_data(const std::vector<unsigned char>& in, std::string& out) {
    z_stream zs;
    std::memset(&zs, 0, sizeof(zs));
    int ret = inflateInit(&zs);
    if (ret != Z_OK) return false;

    zs.next_in = const_cast<Bytef*>(reinterpret_cast<const Bytef*>(in.data()));
    zs.avail_in = static_cast<uInt>(in.size());

    const size_t CHUNK = 16384;
    std::vector<unsigned char> buffer;
    buffer.resize(CHUNK);

    std::string result;
    do {
        zs.next_out = buffer.data();
        zs.avail_out = static_cast<uInt>(buffer.size());

        ret = inflate(&zs, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&zs);
            return false;
        }

        size_t have = buffer.size() - zs.avail_out;
        if (have > 0) {
            result.append(reinterpret_cast<char*>(buffer.data()), have);
        }
    } while (ret != Z_STREAM_END);

    inflateEnd(&zs);
    out.swap(result);
    return true;
}

// Test 1: Round-trip with a moderately long string to ensure normal inflate path
static bool test_roundtrip_basic() {
    // Slightly long text to ensure multiple blocks in compression
    const std::string input =
        "The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog. "
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut "
        "labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
        "nisi ut aliquip ex ea commodo consequat.";

    std::vector<unsigned char> compressed;
    if (!compress_data(input, compressed)) {
        TEST_FAIL("Compression failed in test_roundtrip_basic");
        return false;
    }

    std::string output;
    if (!decompress_data(compressed, output)) {
        TEST_FAIL("Decompression failed in test_roundtrip_basic");
        return false;
    }

    ASSERT_TRUE(output == input, "Round-trip basic: decompressed data matches original");
    TEST_PASS("Round-trip basic: basic compression-decompression works and inflate path is exercised");
    return true;
}

// Test 2: Round-trip with repeating pattern to enlarge the probability of distance-copy paths
static bool test_roundtrip_repetition() {
    const std::string input = "ABCABCABCABCABCABCABCABCABCABC"; // high repetition to trigger distanceCOPY behavior
    std::vector<unsigned char> compressed;
    if (!compress_data(input, compressed)) {
        TEST_FAIL("Compression failed in test_roundtrip_repetition");
        return false;
    }

    std::string output;
    if (!decompress_data(compressed, output)) {
        TEST_FAIL("Decompression failed in test_roundtrip_repetition");
        return false;
    }

    ASSERT_TRUE(output == input, "Round-trip repetition: decompressed data matches original");
    TEST_PASS("Round-trip repetition: repetition triggers distance copying scenarios in inflate path");
    return true;
}

// Test 3: Edge case with empty input
static bool test_roundtrip_empty() {
    const std::string input = "";
    std::vector<unsigned char> compressed;
    if (!compress_data(input, compressed)) {
        TEST_FAIL("Compression failed in test_roundtrip_empty");
        return false;
    }

    std::string output;
    if (!decompress_data(compressed, output)) {
        TEST_FAIL("Decompression failed in test_roundtrip_empty");
        return false;
    }

    ASSERT_TRUE(output == input, "Round-trip empty: empty input round-trips correctly");
    TEST_PASS("Round-trip empty: handles zero-length input without issues");
    return true;
}

// Test 4: Invalid/corrupted compressed data should produce a decompression error
static bool test_invalid_data_error() {
    const std::string input = "Sample data for invalid test";
    std::vector<unsigned char> compressed;
    if (!compress_data(input, compressed)) {
        TEST_FAIL("Compression failed in test_invalid_data_error (setup)");
        return false;
    }

    // Corrupt a byte in the compressed stream to simulate invalid data
    if (!compressed.empty()) {
        compressed[compressed.size() / 2] ^= 0xFF; // flip some bits to invalidate data
    }

    std::string output;
    bool ok = decompress_data(compressed, output);
    // We expect failure due to corruption
    if (ok) {
        TEST_FAIL("Decompression unexpectedly succeeded on corrupted data in test_invalid_data_error");
        return false;
    } else {
        TEST_PASS("Invalid data test: decompression failed as expected on corrupted input");
        return true;
    }
}

// Helper to run all tests
static bool run_all_tests() {
    bool all_ok = true;
    all_ok &= test_roundtrip_basic();
    all_ok &= test_roundtrip_repetition();
    all_ok &= test_roundtrip_empty();
    all_ok &= test_invalid_data_error();
    return all_ok;
}

// Entry point
int main() {
    std::cout << " inflate_fast test suite (via end-to-end inflate path) - C++11\n";
    std::cout << "Note: Tests rely on zlib public APIs (deflate/inflate) to exercise inflate_fast internally.\n";

    bool ok = run_all_tests();
    if (ok) {
        std::cout << "[ALL TESTS PASSED] inflate_fast via inflate path exercised across scenarios." << std::endl;
        return 0;
    } else {
        std::cerr << "[SOME TESTS FAILED] See above for details." << std::endl;
        return 1;
    }
}