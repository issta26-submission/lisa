// Unit test suite for inflateInit2_ (C code) using C++11, without GoogleTest.
// The tests exercise various execution paths of inflateInit2_ by invoking it through
// C linkage from C++ test harness and validating return codes and internal state.
//
// Assumptions:
// - The environment provides zlib headers (zlib.h) and library linkage (-lz).
// - inflateInit2_ is available to link against (as declared with C linkage).
// - We avoid terminating assertions; instead, tests return a boolean and log
//   descriptive messages for failures to maximize code coverages.
//
// Note: This test suite is self-contained and can be compiled with a C++11 capable compiler.
// It relies on the public zlib API and internal behavior of inflateInit2_ as shown in the provided focal method.

#include <cstring>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <inftrees.h>
#include <zlib.h>
#include <inffast.h>
#include <zutil.h>
#include <sstream>
#include <inflate.h>



// Declare the focal function with C linkage to avoid name mangling when linking from C++
extern "C" int inflateInit2_(z_streamp strm, int windowBits,
                          const char *version, int stream_size);

// Lightweight test harness (non-terminating assertions)
static int gTotalTests = 0;
static int gFailedTests = 0;
static std::vector<std::string> gFailures;

static void logFailure(const std::string& testName, const std::string& detail) {
    std::ostringstream oss;
    oss << testName << " FAILED: " << detail;
    gFailures.push_back(oss.str());
    std::cerr << oss.str() << std::endl;
}

static void recordPass(const std::string& testName) {
    std::ostringstream oss;
    oss << testName << " PASSED";
    std::cout << oss.str() << std::endl;
}

// Helper to safely end inflate to release any allocated resources from zlib
static void safeInflateEnd(z_streamp strm) {
    if (strm != nullptr && strm->state != nullptr) {
        // inflateEnd is provided by zlib; ignore errors in cleanup
        inflateEnd(strm);
        // Reset for safety
        strm->state = nullptr;
    }
}

// Test 1: Basic successful initialization with valid params
static bool test_inflateInit2_basic_success() {
    const char* testName = "test_inflateInit2_basic_success";

    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    int ret = inflateInit2_(&strm, 15, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        logFailure(testName, "expected Z_OK, got " + std::to_string(ret));
        gTotalTests++;
        gFailedTests++;
        return false;
    }
    if (strm.state == nullptr) {
        logFailure(testName, "state should be allocated but is NULL");
        gTotalTests++;
        gFailedTests++;
        return false;
    }

    // Cleanup
    safeInflateEnd(&strm);
    recordPass(testName);
    gTotalTests++;
    return true;
}

// Test 2: Null version pointer should yield Z_VERSION_ERROR
static bool test_inflateInit2_null_version() {
    const char* testName = "test_inflateInit2_null_version";

    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    int ret = inflateInit2_(&strm, 15, Z_NULL, (int)sizeof(z_stream));
    if (ret != Z_VERSION_ERROR) {
        logFailure(testName, "expected Z_VERSION_ERROR, got " + std::to_string(ret));
        gTotalTests++;
        gFailedTests++;
        return false;
    }
    gTotalTests++;
    gFailedTests++;
    return false;
}

// Test 3: Wrong stream_size should yield Z_VERSION_ERROR
static bool test_inflateInit2_wrong_stream_size() {
    const char* testName = "test_inflateInit2_wrong_stream_size";

    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    int ret = inflateInit2_(&strm, 15, ZLIB_VERSION, (int)sizeof(z_stream) - 1);
    if (ret != Z_VERSION_ERROR) {
        logFailure(testName, "expected Z_VERSION_ERROR, got " + std::to_string(ret));
        gTotalTests++;
        gFailedTests++;
        return false;
    }
    gTotalTests++;
    gFailedTests++;
    return false;
}

// Test 4: Null strm pointer should yield Z_STREAM_ERROR
static bool test_inflateInit2_null_stream() {
    const char* testName = "test_inflateInit2_null_stream";

    int ret = inflateInit2_(Z_NULL, 15, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_STREAM_ERROR) {
        logFailure(testName, "expected Z_STREAM_ERROR, got " + std::to_string(ret));
        gTotalTests++;
        gFailedTests++;
        return false;
    }
    gTotalTests++;
    gFailedTests++;
    return false;
}

// Test 5: Version mismatch first character should yield Z_VERSION_ERROR
static bool test_inflateInit2_version_mismatch() {
    const char* testName = "test_inflateInit2_version_mismatch";

    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    // Intentionally wrong first character; ZLIB_VERSION typically starts with a digit (e.g., "1.")
    int ret = inflateInit2_(&strm, 15, "X", (int)sizeof(z_stream));
    if (ret != Z_VERSION_ERROR) {
        logFailure(testName, "expected Z_VERSION_ERROR, got " + std::to_string(ret));
        gTotalTests++;
        gFailedTests++;
        return false;
    }
    gTotalTests++;
    gFailedTests++;
    return false;
}

// Test 6: Negative windowBits with magnitude that yields invalid width (-5) -> Z_STREAM_ERROR
static bool test_inflateInit2_negative_windowbits_invalid() {
    const char* testName = "test_inflateInit2_negative_windowbits_invalid";

    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    // -5 translates to windowBits = 5 after negation, which is invalid (width < 8)
    int ret = inflateInit2_(&strm, -5, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_STREAM_ERROR) {
        logFailure(testName, "expected Z_STREAM_ERROR, got " + std::to_string(ret));
        gTotalTests++;
        gFailedTests++;
        return false;
    }
    gTotalTests++;
    gFailedTests++;
    return false;
}

// Test 7: Negative windowBits at the valid extreme (-15) should yield Z_OK and allocate state
static bool test_inflateInit2_negative_windowbits_valid() {
    const char* testName = "test_inflateInit2_negative_windowbits_valid";

    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    // -15 -> windowBits becomes 15, which is valid
    int ret = inflateInit2_(&strm, -15, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        logFailure(testName, "expected Z_OK, got " + std::to_string(ret));
        gTotalTests++;
        gFailedTests++;
        return false;
    }
    if (strm.state == nullptr) {
        logFailure(testName, "state should be allocated for valid windowBits, but is NULL");
        gTotalTests++;
        gFailedTests++;
        return false;
    }

    // Cleanup
    safeInflateEnd(&strm);
    gTotalTests++;
    gFailedTests++;
    return true;
}

// Test 8: Zero windowBits (0) should be okay and return Z_OK, with state allocated
static bool test_inflateInit2_zero_windowbits() {
    const char* testName = "test_inflateInit2_zero_windowbits";

    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    int ret = inflateInit2_(&strm, 0, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        logFailure(testName, "expected Z_OK, got " + std::to_string(ret));
        gTotalTests++;
        gFailedTests++;
        return false;
    }
    if (strm.state == nullptr) {
        logFailure(testName, "state should be allocated for zero windowBits, but is NULL");
        gTotalTests++;
        gFailedTests++;
        return false;
    }

    // Cleanup
    safeInflateEnd(&strm);
    gTotalTests++;
    gFailedTests++;
    return true;
}

// Driver
int main() {
    std::cout << "Running inflateInit2_ test suite (C linkage)..." << std::endl;

    // Run tests
    test_inflateInit2_basic_success();
    test_inflateInit2_null_version();
    test_inflateInit2_wrong_stream_size();
    test_inflateInit2_null_stream();
    test_inflateInit2_version_mismatch();
    test_inflateInit2_negative_windowbits_invalid();
    test_inflateInit2_negative_windowbits_valid();
    test_inflateInit2_zero_windowbits();

    // Summary
    std::cout << "Total tests attempted: " << gTotalTests << std::endl;
    std::cout << "Total failures: " << gFailedTests << std::endl;
    if (!gFailures.empty()) {
        std::cout << "Failure details:" << std::endl;
        for (const auto& f : gFailures) std::cout << "  - " << f << std::endl;
    } else {
        std::cout << "All tests completed with no failures detected." << std::endl;
    }

    // Return non-zero if any test failed
    return (gFailedTests == 0) ? 0 : 1;
}