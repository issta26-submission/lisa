// Lightweight C++11 test suite for the focal function LLVMFuzzerTestOneInput
// This test suite intentionally avoids GoogleTest and uses a small, non-terminating assertion mechanism.
// It exercises various branches of the fuzzer function by constructing crafted inputs.

#include <../cJSON.h>
#include <cstddef>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <cstdint>


// Forward-declare the focal C function with C linkage
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size);

// Simple non-terminating assertion helpers
static int g_test_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            ++g_test_failures; \
            std::cerr << "[EXPECT_FAIL] " << (msg) << "  (in " << __FILE__ << ":" << __LINE__ << ")\n"; \
        } \
    } while(0)

#define RUN_TEST(test_func) \
    do { \
        test_func(); \
    } while(0)

// Helper: build input buffer for the fuzzer
// Flags: minify (data[0]), require_termination (data[1]), formatted (data[2]), buffered (data[3])
// Offset = 4; JSON payload follows; input must end with a null terminator when used for valid tests
static std::vector<uint8_t> buildInput(char flagMinify, char flagRequireTermination, char flagFormatted, char flagBuffered, const std::string& jsonPayload, bool endWithNull = true) {
    std::vector<uint8_t> input;
    input.push_back(flagMinify);
    input.push_back(flagRequireTermination);
    input.push_back(flagFormatted);
    input.push_back(flagBuffered);
    // Append JSON payload (offset region is the first 4 bytes, not part of the JSON payload)
    input.insert(input.end(), jsonPayload.begin(), jsonPayload.end());
    // Ensure a terminal 0 if requested
    if(endWithNull) input.push_back('\0');
    return input;
}

// Test 1: size <= offset should immediately return (0)
void test_size_too_small() {
    // size == offset (4) should early-exit
    std::vector<uint8_t> input = { '0','0','0','0' }; // exactly 4 bytes
    int ret = LLVMFuzzerTestOneInput(input.data(), input.size());
    EXPECT_TRUE(ret == 0, "Expected 0 when size <= offset (early exit)");
}

// Test 2: last byte not '\0' should early-return
void test_last_byte_not_null() {
    // size > offset but last byte != '\0'
    // Flags: all zeros; JSON payload is "{}"
    std::string json = "{}";
    std::vector<uint8_t> input = buildInput('0','0','0','0', json, false); // no trailing 0
    int ret = LLVMFuzzerTestOneInput(input.data(), input.size());
    EXPECT_TRUE(ret == 0, "Expected 0 when data[size-1] != '\\0'");
}

// Test 3: All four flags are '0' (minify=0, require_termination=0, formatted=0, buffered=0) with valid JSON
void test_flags_all_zero_valid_json() {
    // Valid simple JSON; ensure the fuzzer parses and prints unformatted/unminified
    std::string json = "{\"a\":1}";
    std::vector<uint8_t> input = buildInput('0','0','0','0', json, true);
    int ret = LLVMFuzzerTestOneInput(input.data(), input.size());
    EXPECT_TRUE(ret == 0, "Parse with flags 0000 should succeed and not crash");
}

// Test 4: All flags '1' (minify=1, require_termination=1, formatted=1, buffered=1) with valid JSON
void test_flags_all_one_buffered_minified_and_formatted() {
    // Large-ish JSON to exercise buffered printing and minification
    std::string json = "{\"a\":1,\"b\":2,\"c\":3}";
    std::vector<uint8_t> input = buildInput('1','1','1','1', json, true);
    int ret = LLVMFuzzerTestOneInput(input.data(), input.size());
    EXPECT_TRUE(ret == 0, "Parse -> buffered print (formatted) -> minify path should not crash");
}

// Test 5: Unbuffered, unformatted path (buffered=0, formatted=0) with valid JSON
void test_unbuffered_unformatted_path() {
    std::string json = "{\"x\":123}";
    std::vector<uint8_t> input = buildInput('0','0','0','0', json, true);
    int ret = LLVMFuzzerTestOneInput(input.data(), input.size());
    EXPECT_TRUE(ret == 0, "Unbuffered, unformatted path should succeed");
}

// Test 6: Include require_termination = 1 with valid JSON to exercise termination branch
void test_require_termination_true() {
    // require_termination=1 should be exercised; JSON is valid
    std::string json = "{\"term\":1}";
    std::vector<uint8_t> input = buildInput('0','1','0','0', json, true);
    int ret = LLVMFuzzerTestOneInput(input.data(), input.size());
    EXPECT_TRUE(ret == 0, "Require termination path should succeed with valid JSON");
}

// Runner that executes all tests
void runAllTests() {
    test_size_too_small();
    test_last_byte_not_null();
    test_flags_all_zero_valid_json();
    test_flags_all_one_buffered_minified_and_formatted();
    test_unbuffered_unformatted_path();
    test_require_termination_true();

    if(g_test_failures == 0) {
        std::cout << "[TEST_PASS] All tests passed.\n";
    } else {
        std::cout << "[TEST_FAIL] " << g_test_failures << " test(s) failed.\n";
    }
}

int main() {
    // Execute test suite
    runAllTests();
    return g_test_failures != 0 ? 1 : 0;
}