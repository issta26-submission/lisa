// Test suite for the focal method read_tag in jpgicc.c
// Note: This test suite is designed to be compiled and linked with the
// C/C++ project that provides the C function: int read_tag(uint8_t* arr, int pos, int swapBytes, void* dest, size_t max)
// The tests avoid any external testing framework (GTest, etc.) and implement a lightweight harness.

#include <cstring>
#include <iccjpeg.h>
#include <jpeglib.h>
#include <cstddef>
#include <vector>
#include <iostream>
#include <cmath>
#include <utils.h>
#include <cstdint>


extern "C" int read_tag(uint8_t* arr, int pos, int swapBytes, void* dest, size_t max);

// Lightweight assertion helper that does not abort the test on failure
static bool assert_true(bool cond, const char* msg, int& failCount) {
    if (!cond) {
        std::cerr << "  [ASSERT FAILED] " << msg << "\n";
        ++failCount;
        return false;
    }
    return true;
}

// Helper to compare doubles with exact equality (safe for exact integral results from read_tag in tests)
static bool almost_equal(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps;
}

/*
Test 1: Format == 3 (uint16) with 1 component
- pos = 0
- format read from arr[2..3] = 3 (little-endian)
- components arr[4..7] = 1
- offset is pos+8 (since format == 3)
- value at offset (16-bit) arr[8..9] = 1234
- after read_tag, dest should be 1234
*/
static bool test_format3_success() {
    std::vector<uint8_t> arr(12, 0); // ensure indices 0..11 are available

    // Set format = 3
    arr[2] = 0x03; // low byte
    arr[3] = 0x00; // high byte

    // Set components = 1 (little-endian 32-bit)
    arr[4] = 0x01;
    arr[5] = 0x00;
    arr[6] = 0x00;
    arr[7] = 0x00;

    // Since format == 3, offset = pos+8 => at arr[8..9]
    // Set value = 1234 (0x04D2) as little-endian
    arr[8] = 0xD2; // low byte
    arr[9] = 0x04; // high byte

    double dest = 0.0;
    int res = read_tag(arr.data(), 0, 0, &dest, arr.size());

    bool ok = assert_true(res == 1, "read_tag should return 1 for valid format 3 with 1 component", /*failCount*/ *(new int(0)));
    // We can't access the ephemeral failCount here; instead, just check with a local counter approach
    // Use a small inline approach to accumulate failures instead of relying on the outer harness here.
    // To keep consistent, re-implement using a local fail counter:
    int localFails = 0;
    if (!assert_true(res == 1, "read_tag did not return 1 for format 3 with 1 component", localFails)) {
        // nothing
    }
    if (!assert_true(almost_equal(dest, 1234.0), "dest should be exactly 1234.0 for format 3 test", localFails)) {
        // nothing
    }

    return (localFails == 0);
}

/*
Test 2: Format == 5 (rational) with 1 component
- pos = 0
- format read from arr[2..3] = 5
- components = 1
- offset = read32(arr, pos+8) => we place 16 at arr[8..11]
- numerator at arr[16..19] = 8
- denominator at arr[20..23] = 2
- dest should be 8/2 = 4.0
*/
static bool test_format5_rational_success() {
    // Need at least 24 bytes to place numerator/denominator
    std::vector<uint8_t> arr(24, 0);

    // format = 5
    arr[2] = 0x05;
    arr[3] = 0x00;

    // components = 1
    arr[4] = 0x01;
    arr[5] = 0x00;
    arr[6] = 0x00;
    arr[7] = 0x00;

    // offset at arr[8..11] = 16 (0x00000010)
    arr[8]  = 0x10;
    arr[9]  = 0x00;
    arr[10] = 0x00;
    arr[11] = 0x00;

    // numerator at arr[16..19] = 8
    arr[16] = 0x08;
    arr[17] = 0x00;
    arr[18] = 0x00;
    arr[19] = 0x00;

    // denominator at arr[20..23] = 2
    arr[20] = 0x02;
    arr[21] = 0x00;
    arr[22] = 0x00;
    arr[23] = 0x00;

    double dest = 0.0;
    int res = read_tag(arr.data(), 0, 0, &dest, arr.size());

    int localFails = 0;
    if (!assert_true(res == 1, "read_tag should return 1 for format 5 with 1 component", localFails)) {
        // nothing
    }
    if (!assert_true(almost_equal(dest, 4.0), "dest should be 4.0 for format 5 rational", localFails)) {
        // nothing
    }

    return (localFails == 0);
}

/*
Test 3: Unknown format (default branch)
- pos = 0
- format = 7 (unknown)
- components = 1
- offset = 0 (not used)
- dest should remain unchanged and read_tag should return 0
*/
static bool test_unknown_format_returns_zero_no_write() {
    std::vector<uint8_t> arr(12, 0);
    // format = 7
    arr[2] = 0x07;
    arr[3] = 0x00;

    // components = 1
    arr[4] = 0x01;
    arr[5] = 0x00;
    arr[6] = 0x00;
    arr[7] = 0x00;

    // offset (not used since format is unknown) set to 0
    arr[8] = 0x00;
    arr[9] = 0x00;
    arr[10] = 0x00;
    arr[11] = 0x00;

    double dest = -7.0; // ensure we can detect any unintended modification
    int res = read_tag(arr.data(), 0, 0, &dest, arr.size());

    int localFails = 0;
    if (!assert_true(res == 0, "read_tag should return 0 for unknown format", localFails)) {
        // nothing
    }
    if (!assert_true(almost_equal(dest, -7.0), "dest should remain unchanged when format is unknown", localFails)) {
        // nothing
    }

    return (localFails == 0);
}

/*
Test 4: Components != 1 should cause early exit with no write
- pos = 0
- format = 3 so the branch related to format==3 is taken, but components != 1 should return 0
- dest remains unchanged
*/
static bool test_components_not_one_returns_zero_no_write() {
    std::vector<uint8_t> arr(12, 0);

    // format = 3
    arr[2] = 0x03;
    arr[3] = 0x00;

    // components = 2 (not 1)
    arr[4] = 0x02;
    arr[5] = 0x00;
    arr[6] = 0x00;
    arr[7] = 0x00;

    // offset path doesn't matter due to early exit, set to zero
    arr[8] = 0x00;
    arr[9] = 0x00;
    arr[10] = 0x00;
    arr[11] = 0x00;

    double dest = 1.23;
    int res = read_tag(arr.data(), 0, 0, &dest, arr.size());

    int localFails = 0;
    if (!assert_true(res == 0, "read_tag should return 0 when components != 1", localFails)) {
        // nothing
    }
    if (!assert_true(almost_equal(dest, 1.23), "dest should remain unchanged when components != 1", localFails)) {
        // nothing
    }

    return (localFails == 0);
}

int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool (*fn)()) {
        ++total;
        bool ok = fn();
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    // Run tests
    run("test_format3_success", test_format3_success);
    run("test_format5_rational_success", test_format5_rational_success);
    run("test_unknown_format_returns_zero_no_write", test_unknown_format_returns_zero_no_write);
    run("test_components_not_one_returns_zero_no_write", test_components_not_one_returns_zero_no_write);

    std::cout << "Test results: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}