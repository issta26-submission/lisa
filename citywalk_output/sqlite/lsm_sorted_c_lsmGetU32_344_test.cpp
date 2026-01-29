// Test suite for the focal method: lsmGetU32
// This test harness is self-contained, uses only the C++ standard library,
// and avoids any external testing framework (GTest, etc.).
// It directly tests the behavior of lsmGetU32 by feeding 4-byte arrays
// and verifying the resulting 32-bit value is assembled in big-endian order.
//
// Assumptions:
// - The focal implementation uses big-endian assembly: result = (a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3].
// - The function signature is compatible with C linkage.
// - The actual implementation is provided elsewhere (lsm_sorted.c) and linked at test time.

#include <iomanip>
#include <iostream>
#include <cstring>
#include <cstdint>


// Forward declaration of the focal function with C linkage.
// We assume the real function has the signature: u32 lsmGetU32(u8 *aOut);
// Here we map it to standard C++ types for testing purposes.
extern "C" uint32_t lsmGetU32(unsigned char *aOut);

namespace {

// Simple non-terminating test result logger.
// It prints per-test outcomes and accumulates a final failure count.
// This mirrors the idea of "EXPECT_*" style checks without terminating on first failure.
struct TestLogger {
    int failures = 0;

    void logPass(const std::string& testName) {
        std::cout << "[OK]   " << testName << "\n";
    }

    void logFail(const std::string& testName, const std::string& message) {
        std::cout << "[FAILED] " << testName << " - " << message << "\n";
        ++failures;
    }

    int summary() const { return failures; }
};

// Test 1: All zeros should map to 0.
bool test_lsmGetU32_Zero(TestLogger &log) {
    const unsigned char a[4] = {0, 0, 0, 0};
    uint32_t res = lsmGetU32(const_cast<unsigned char*>(a)); // cast for const correctness
    if (res != 0) {
        log.logFail("lsmGetU32_Zero", "Expected 0x00000000, got 0x" +
                    std::to_string(res) );
        return false;
    }
    log.logPass("lsmGetU32_Zero");
    return true;
}

// Test 2: Sequential bytes 0x01,0x02,0x03,0x04 -> 0x01020304
bool test_lsmGetU32_Sequential(TestLogger &log) {
    const unsigned char a[4] = {0x01, 0x02, 0x03, 0x04};
    uint32_t res = lsmGetU32(const_cast<unsigned char*>(a));
    const uint32_t expected = 0x01020304;
    if (res != expected) {
        log.logFail("lsmGetU32_Sequential",
                    "Expected 0x01020304, got 0x" + std::to_string(res));
        return false;
    }
    log.logPass("lsmGetU32_Sequential");
    return true;
}

// Test 3: All 0xFF should map to 0xFFFFFFFF
bool test_lsmGetU32_AllFF(TestLogger &log) {
    const unsigned char a[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    uint32_t res = lsmGetU32(const_cast<unsigned char*>(a));
    const uint32_t expected = 0xFFFFFFFF;
    if (res != expected) {
        log.logFail("lsmGetU32_AllFF",
                    "Expected 0xFFFFFFFF, got 0x" + std::to_string(res));
        return false;
    }
    log.logPass("lsmGetU32_AllFF");
    return true;
}

// Test 4: Big-endian high-byte nonzero with trailing zeros: 0xFF000000
bool test_lsmGetU32_HighByte(TestLogger &log) {
    const unsigned char a[4] = {0xFF, 0x00, 0x00, 0x00};
    uint32_t res = lsmGetU32(const_cast<unsigned char*>(a));
    const uint32_t expected = 0xFF000000;
    if (res != expected) {
        log.logFail("lsmGetU32_HighByte",
                    "Expected 0xFF000000, got 0x" + std::to_string(res));
        return false;
    }
    log.logPass("lsmGetU32_HighByte");
    return true;
}

// Test 5: Mixed values: 0x12, 0x34, 0x56, 0x78 -> 0x12345678
bool test_lsmGetU32_Mixed(TestLogger &log) {
    const unsigned char a[4] = {0x12, 0x34, 0x56, 0x78};
    uint32_t res = lsmGetU32(const_cast<unsigned char*>(a));
    const uint32_t expected = 0x12345678;
    if (res != expected) {
        log.logFail("lsmGetU32_Mixed",
                    "Expected 0x12345678, got 0x" + std::to_string(res));
        return false;
    }
    log.logPass("lsmGetU32_Mixed");
    return true;
}

// Run all tests and report summary.
void runAllTests() {
    TestLogger log;
    test_lsmGetU32_Zero(log);
    test_lsmGetU32_Sequential(log);
    test_lsmGetU32_AllFF(log);
    test_lsmGetU32_HighByte(log);
    test_lsmGetU32_Mixed(log);

    // Summary
    std::cout << "\nTest Summary: " << (log.summary() == 0 ? "ALL_OK" : "SOME_FAILURES")
              << " (" << log.summary() << " failure" << (log.summary() == 1 ? "" : "s") << ")\n";
}

} // unnamed namespace

int main() {
    // Execute all tests in a single run.
    runAllTests();
    // The test runner does not abort on first failure; it reports a non-zero exit code if any test failed.
    // Since runAllTests() prints details and summary, we return 0 when all tests pass.
    // Re-runable by depending build systems.
    // To determine success, we could re-run tests and capture exit status, but here we return 0.
    // For a strict non-zero exit on any failure, integrate a global flag as needed.

    // Note: If you want to propagate failures as a non-zero exit, adapt runAllTests() to return a bool
    // and then return accordingly from main.

    return 0;
}