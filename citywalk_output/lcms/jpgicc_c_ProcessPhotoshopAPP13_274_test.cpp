// This is a handcrafted C++11 unit test suite for the focal method:
// cmsBool ProcessPhotoshopAPP13(JOCTET *data, int datalen)
// The tests are designed to be compilable in a C++11 project without GTest.
// It relies on the existing C implementation being linked (jpgicc.c) and uses
// a minimal, self-contained test harness to exercise several execution paths.
// Notes:
// - We avoid private member access and focus on the observable return value (TRUE/FALSE).
// - The tests craft input byte arrays that exercise true/false branches of the parser.
// - The code includes explanatory comments for each test case.

#include <cstring>
#include <iccjpeg.h>
#include <jpeglib.h>
#include <iostream>
#include <utils.h>


// Domain-specific: expose the focal function signature in a C-compatible way.
typedef int cmsBool;            // Most likely CASE: cmsBool is an int in the original C code
typedef unsigned char JOCTET;    // Byte type used by the JPEG-related code
#define TRUE 1
#define FALSE 0

// Declaration of the focal function (C linkage)
extern "C" cmsBool ProcessPhotoshopAPP13(JOCTET *data, int datalen);

// Test 1: When no 8BIM marker (0x38 0x42 0x49 0x4D) is found after offset 14,
// the function should return FALSE (not recognizing an APP13 block).
bool test_no_marker_returns_false()
{
    // datalen == 14 ensures the loop never enters (i starts at 14)
    unsigned char data[14];
    std::memset(data, 0, sizeof(data));

    cmsBool res = ProcessPhotoshopAPP13(data, 14);

    // Expected: FALSE
    if (res == FALSE) {
        std::cout << "[PASS] test_no_marker_returns_false\n";
        return true;
    } else {
        std::cout << "[FAIL] test_no_marker_returns_false: expected FALSE, got " << res << "\n";
        return false;
    }
}

// Test 2: Marker present with correct type (0x03ED) and len >= 16.
// This should hit the true-branch and return TRUE (density fields set and function returns early).
bool test_marker_ed_and_len_ge16_returns_true()
{
    // Build a data buffer large enough to reach the 8BIM block and density fields.
    unsigned char data[60];
    std::memset(data, 0, sizeof(data));

    // 14..17: "8BIM" marker
    data[14] = 0x38;
    data[15] = 0x42;
    data[16] = 0x49;
    data[17] = 0x4D;

    // 18..19: type = 0x03ED
    data[18] = 0x03;
    data[19] = 0xED;

    // 20: resource name length indicator (arbitrary, even -> alignment +2)
    data[20] = 0x02;  // even
    // i after name length: i += 2 (since even) -> i becomes 22

    // 22..25: length (len) = 0x00010010 (arbitrary large enough, >= 16)
    data[22] = 0x00;
    data[23] = 0x01;
    data[24] = 0x00;
    data[25] = 0x10;

    // i += 4 -> i becomes 26

    // 26..27: X_density raw (two bytes)
    data[26] = 0x01;
    data[27] = 0x02; // value 0x0102

    // 28..29: X_density second 16-bit component (unused by test assertion)
    data[28] = 0x03;
    data[29] = 0x04; // value 0x0304

    // 34..35..37: Y_density components (not fully used by test assertion)
    data[34] = 0x05;
    data[35] = 0x06;
    data[36] = 0x07;
    data[37] = 0x08;

    // We choose a datalen large enough to satisfy loop constraints
    int datalen = 38;

    cmsBool res = ProcessPhotoshopAPP13(data, datalen);

    // Expected: TRUE (density block found and processed)
    if (res == TRUE) {
        std::cout << "[PASS] test_marker_ed_and_len_ge16_returns_true\n";
        return true;
    } else {
        std::cout << "[FAIL] test_marker_ed_and_len_ge16_returns_true: expected TRUE, got " << res << "\n";
        return false;
    }
}

// Test 3: Marker present but with an unsupported type (not 0x03ED).
// This should fall through to end of data and return FALSE.
bool test_marker_with_wrong_type_returns_false()
{
    // Build a data buffer with an 8BIM marker but wrong type.
    unsigned char data[60];
    std::memset(data, 0, sizeof(data));

    // 14..17: "8BIM" marker
    data[14] = 0x38;
    data[15] = 0x42;
    data[16] = 0x49;
    data[17] = 0x4D;

    // 18..19: type = 0x1234 (not 0x03ED)
    data[18] = 0x12;
    data[19] = 0x34;

    // 20: resource name length
    data[20] = 0x02;  // even

    // 22..25: length (len) = 16 (>=16)
    data[22] = 0x00;
    data[23] = 0x01;
    data[24] = 0x00;
    data[25] = 0x10;

    // 26..29: some data (not used since type doesn't match)
    data[26] = 0x01;
    data[27] = 0x02;
    data[28] = 0x03;
    data[29] = 0x04;

    // Fill the rest to avoid out-of-bounds; ensure there is no second 8BIM marker
    int datalen = 60;

    cmsBool res = ProcessPhotoshopAPP13(data, datalen);

    // Expected: FALSE
    if (res == FALSE) {
        std::cout << "[PASS] test_marker_with_wrong_type_returns_false\n";
        return true;
    } else {
        std::cout << "[FAIL] test_marker_with_wrong_type_returns_false: expected FALSE, got " << res << "\n";
        return false;
    }
}

// Simple runner to execute all tests and report summary.
int main()
{
    int tests_passed = 0;
    int tests_total = 3;

    if (test_no_marker_returns_false()) ++tests_passed;
    if (test_marker_ed_and_len_ge16_returns_true()) ++tests_passed;
    if (test_marker_with_wrong_type_returns_false()) ++tests_passed;

    std::cout << "Test results: " << tests_passed << "/" << tests_total << " tests passed.\n";
    return (tests_passed == tests_total) ? 0 : 1;
}