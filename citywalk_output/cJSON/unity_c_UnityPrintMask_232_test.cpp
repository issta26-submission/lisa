// Test suite for UnityPrintMask in unity.c
// This test harness uses a custom, non-GTest approach suitable for C++11.
// It captures stdout output produced by UnityPrintMask by redirecting stdout
// to a pipe, invokes the function, then reconstructs the expected string bit-by-bit
// based on the same width logic used in the focal method.
//
// Domain knowledge notes:
// - UnityPrintMask prints UNITY_INT_WIDTH characters per call, one per bit from MSB to LSB.
// - For each bit: if mask has the bit set, print '1' if number has that bit, else '0'; if mask
//   bit is not set, print 'X'.
// - UNITY_UINT and UNITY_INT_WIDTH come from the Unity framework (bit-width dependent on platform).
//
// Candidate Keywords (Step 1):
// - UNITY_UINT, UNITY_INT_WIDTH, UNITY_OUTPUT_CHAR, mask, number, bitwise operations, MSB, LSB,
// - width, output validation, stdout redirection, capture and compare, non-terminating assertions,
// - UnityPrintMask, C linkage, cross-language test bridging, POSIX pipe, dup2, read.

#include <stddef.h>
#include <unistd.h>
#include <unity.h>
#include <cstring>
#include <sys/types.h>
#include <iostream>
#include <functional>
#include <vector>
#include <sys/stat.h>
#include <string>
#include <fcntl.h>


// Bring in Unity's C function prototype. UnityPrintMask is defined in unity.c
#ifdef __cplusplus
extern "C" {
#endif
// Declaration matching the Unity-typical typedefs
// UNITY_UINT is typically unsigned int.
// We declare the function with plain unsigned int types for compatibility.
void UnityPrintMask(const unsigned int mask, const unsigned int number);
#ifdef __cplusplus
}
#endif

// Helper: capture stdout output of UnityPrintMask(mask, number)
static std::string CaptureUnityPrintMaskOutput(unsigned int mask, unsigned int number)
{
    // Create a pipe to capture stdout
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, return empty output
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(1);
    if (saved_stdout == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to pipe's write end
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        // Restore and cleanup
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Close the pipe's write end as it's now duplicated to stdout
    close(pipefd[1]);

    // Invoke the function under test
    UnityPrintMask(mask, number);

    // Flush to ensure all output is written to the pipe
    fflush(stdout);

    // Restore original stdout
    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        // Cleanup
        close(saved_stdout);
        close(pipefd[0]);
        return "";
    }
    close(saved_stdout);

    // Read all bytes from the read end
    std::string output;
    char buffer[4096];
    ssize_t bytes;
    // Important: the write end was closed earlier; the reader should see EOF after last write
    while ((bytes = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(bytes));
    }

    // Cleanup read end
    close(pipefd[0]);

    return output;
}

// Helper: Build expected string given mask, number, and implied width
static std::string BuildExpectedFromMask(unsigned int mask, unsigned int number, int width)
{
    std::string expected;
    unsigned int current_bit = (unsigned int)1 << (width - 1);
    for (int i = 0; i < width; ++i) {
        if (mask & current_bit) {
            expected.push_back((number & current_bit) ? '1' : '0');
        } else {
            expected.push_back('X');
        }
        current_bit >>= 1;
    }
    return expected;
}

// Test 1: Top bit set in mask and number -> '1' followed by (width-1) 'X'
static bool Test_TopBit_SameNumber()
{
    unsigned int mask = 0x80000000u;
    unsigned int number = 0x80000000u;
    std::string actual = CaptureUnityPrintMaskOutput(mask, number);
    int width = (int)actual.size();
    if (width == 0) {
        std::cerr << "Test_TopBit_SameNumber: Failed to capture output (width 0).\n";
        return false;
    }
    std::string expected = BuildExpectedFromMask(mask, number, width);
    bool ok = (actual == expected);
    // Explanatory comment for test
    // Verifies that the MSB is printed as '1' when both mask and number have that bit set,
    // and that all remaining bits are 'X' when the corresponding mask bits are not set.
    if (!ok) {
        std::cerr << "Test_TopBit_SameNumber failed. Expected: " << expected
                  << " Actual: " << actual << "\n";
    }
    return ok;
}

// Test 2: Top bit set in mask, number's top bit not set -> '0' at MSB, rest 'X'
static bool Test_TopBit_MaskSet_NumberClear()
{
    unsigned int mask = 0x80000000u;
    unsigned int number = 0x00000000u;
    std::string actual = CaptureUnityPrintMaskOutput(mask, number);
    int width = (int)actual.size();
    if (width == 0) {
        std::cerr << "Test_TopBit_MaskSet_NumberClear: Failed to capture output (width 0).\n";
        return false;
    }
    std::string expected = BuildExpectedFromMask(mask, number, width);
    bool ok = (actual == expected);
    if (!ok) {
        std::cerr << "Test_TopBit_MaskSet_NumberClear failed. Expected: " << expected
                  << " Actual: " << actual << "\n";
    }
    return ok;
}

// Test 3: Mixed mask and number in top bits: mask top 4 bits = 1010, number top 4 bits = 1010
static bool Test_Top4Bits_Pattern()
{
    unsigned int mask = 0xF0000000u;      // top 4 bits set
    unsigned int number = 0xA0000000u;    // top 4 bits pattern: 1010
    std::string actual = CaptureUnityPrintMaskOutput(mask, number);
    int width = (int)actual.size();
    if (width == 0) {
        std::cerr << "Test_Top4Bits_Pattern: Failed to capture output (width 0).\n";
        return false;
    }
    std::string expected = BuildExpectedFromMask(mask, number, width);
    bool ok = (actual == expected);
    if (!ok) {
        std::cerr << "Test_Top4Bits_Pattern failed. Expected: " << expected
                  << " Actual: " << actual << "\n";
    }
    return ok;
}

// Test 4: Mask is zero => all 'X'
static bool Test_MaskZero_AllX()
{
    // Choose some non-trivial mask and number; only mask is relevant
    unsigned int mask = 0x00000000u;
    unsigned int number = 0xDEADBEEFu;
    std::string actual = CaptureUnityPrintMaskOutput(mask, number);
    int width = (int)actual.size();
    if (width == 0) {
        std::cerr << "Test_MaskZero_AllX: Failed to capture output (width 0).\n";
        return false;
    }
    std::string expected = BuildExpectedFromMask(mask, number, width);
    bool ok = (actual == expected);
    if (!ok) {
        std::cerr << "Test_MaskZero_AllX failed. Expected: " << expected
                  << " Actual: " << actual << "\n";
    }
    return ok;
}

// Test 5: All bits masked, number arbitrary => binary representation of number
static bool Test_AllBitsMasked_NumberPattern()
{
    unsigned int mask = 0xFFFFFFFFu;  // all bits considered
    unsigned int number = 0xF0F0F0F0u;
    std::string actual = CaptureUnityPrintMaskOutput(mask, number);
    int width = (int)actual.size();
    if (width == 0) {
        std::cerr << "Test_AllBitsMasked_NumberPattern: Failed to capture output (width 0).\n";
        return false;
    }
    std::string expected = BuildExpectedFromMask(mask, number, width);
    bool ok = (actual == expected);
    if (!ok) {
        std::cerr << "Test_AllBitsMasked_NumberPattern failed. Expected: " << expected
                  << " Actual: " << actual << "\n";
    }
    return ok;
}

// Main: Run all tests (non-terminating; reports summary)
int main()
{
    int total = 0;
    int passed = 0;

    // Test 1
    {
        bool ok = Test_TopBit_SameNumber();
        total++;
        if (ok) ++passed;
        std::cout << (ok ? "PASS" : "FAIL") << ": Test_TopBit_SameNumber" << std::endl;
    }

    // Test 2
    {
        bool ok = Test_TopBit_MaskSet_NumberClear();
        total++;
        if (ok) ++passed;
        std::cout << (ok ? "PASS" : "FAIL") << ": Test_TopBit_MaskSet_NumberClear" << std::endl;
    }

    // Test 3
    {
        bool ok = Test_Top4Bits_Pattern();
        total++;
        if (ok) ++passed;
        std::cout << (ok ? "PASS" : "FAIL") << ": Test_Top4Bits_Pattern" << std::endl;
    }

    // Test 4
    {
        bool ok = Test_MaskZero_AllX();
        total++;
        if (ok) ++passed;
        std::cout << (ok ? "PASS" : "FAIL") << ": Test_MaskZero_AllX" << std::endl;
    }

    // Test 5
    {
        bool ok = Test_AllBitsMasked_NumberPattern();
        total++;
        if (ok) ++passed;
        std::cout << (ok ? "PASS" : "FAIL") << ": Test_AllBitsMasked_NumberPattern" << std::endl;
    }

    // Summary (non-terminating assertion style)
    std::cout << "Test Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}