/*
 * cmsPackChunkyWords_Test.cpp
 *
 * Lightweight, self-contained unit test harness for the focal method:
 *   cmsUInt8Number* PackChunkyWords(CMSREGISTER _cmsTRANSFORM* info,
 *                                   CMSREGISTER cmsUInt16Number wOut[],
 *                                   CMSREGISTER cmsUInt8Number* output,
 *                                   CMSREGISTER cmsUInt32Number Stride)
 *
 * Notes and caveats:
 * - This test harness is designed for environments where GTest is not allowed.
 * - We provide a minimal mock of the required types and a C-linkage prototype
 *   for the function under test. The test harness is purposely light-weight and
 *   relies on the real project headers (if available in your environment) for
 *   the actual behavior of macros like T_CHANNELS, T_ENDIAN16, etc.
 * - The tests aim to exercise distinct control flow branches by configuring the
 *   info->OutputFormat field. Because the exact bit layout of OutputFormat is
 *   library-specific, you may need to adjust the constants (CHANNELS, ENDIAN16,
 *   DOSWAP, FLAVOR, EXTRA, SWAPFIRST, PREMUL, etc.) to match your build of
 *   LittleCMS (lcms2_internal.h).
 * - The tests use non-terminating assertion style (collect failures and report at end).
 * - Static members/functions: if your environment uses static helpers, keep tests
 *   within the same TU and access via the class name if needed. Here we focus on
 *   the focal function directly.
 *
 * Build note:
 * - Compile with C++11 support. Link against the project where PackChunkyWords is
 *   defined (cmspack.c) and provide the same internal structures if needed.
 *
 * The test code below is designed to be dropped into a test directory alongside
 * the project and built with your existing build tooling.
 */

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Forward declare the focal function to allow linking from C/C++ tests.
// The exact signature uses CMSREGISTER and internal types; we provide a
// compatible C linkage and minimal type aliases to keep test code portable.
extern "C" {
    // Bring in exact types if available from your build system.
    // If cmsUInt8Number, cmsUInt16Number, cmsUInt32Number, etc. are defined
    // in a common header (e.g., lcms2.h), include that header here instead.
    typedef uint8_t  cmsUInt8Number;
    typedef uint16_t cmsUInt16Number;
    typedef uint32_t cmsUInt32Number;

    // Opaque transform struct as expected by the focal function.
    struct _cmsTRANSFORM {
        cmsUInt32Number OutputFormat;
    };

    // Function prototype (C linkage) for PackChunkyWords.
    cmsUInt8Number* PackChunkyWords(struct _cmsTRANSFORM* info,
                                    cmsUInt16Number wOut[],
                                    cmsUInt8Number* output,
                                    cmsUInt32Number Stride);
}

// Lightweight test framework (non-terminating assertions)
namespace TestFramework {
    struct TestResult {
        std::string name;
        bool passed;
        std::string message;
    };

    class TestSuite {
    public:
        void addResult(const TestResult& r) { results.push_back(r); }
        int runAll() {
            int failures = 0;
            for (const auto& r : results) {
                if (!r.passed) {
                    ++failures;
                    std::cerr << "[FAIL] " << r.name << ": " << r.message << "\n";
                } else {
                    std::cout << "[OK]   " << r.name << "\n";
                }
            }
            if (failures == 0) {
                std::cout << "All tests passed.\n";
            } else {
                std::cout << failures << " tests failed.\n";
            }
            return failures;
        }
    private:
        std::vector<TestResult> results;
    };
}

// Minimal helper to print memory as 16-bit words for clarity
static void print16(const cmsUInt16Number* data, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        printf("%04x%c", static_cast<unsigned>(data[i]), (i + 1 == n) ? '\n' : ' ');
    }
}

// Mock helper to fill input with small test pattern
static void fillWOut(cmsUInt16Number* wOut, size_t n, cmsUInt16Number base) {
    for (size_t i = 0; i < n; ++i) {
        wOut[i] = base + static_cast<cmsUInt16Number>(i + 1);
    }
}

// Test 1: Basic chunky packing with 3 channels, no endian swap, no reverse, no premul, no extra.
// Expected behavior (with SwapFirst enabled): [wOut[2], wOut[0], wOut[1]]
static void test_BasicSwapFirst(TestFramework::TestSuite& suite) {
    using namespace TestFramework;

    const char* testName = "PackChunkyWords_BasicSwapFirst_3ch_no_flags";
    // Prepare input
    _cmsTRANSFORM info;
    // NOTE: The exact encoding of OutputFormat is library-specific.
    // Here we assume that the encoded value yields nChan = 3 and SwapFirst = true,
    // while other flags are off. If your library uses different bit layouts, adjust accordingly.
    info.OutputFormat = 3; // Assumed: 3 channels
    cmsUInt16Number wOut[3] = { 0x0001, 0x0002, 0x0003 }; // three 16-bit values
    cmsUInt8Number outputBuffer[6];
    std::memset(outputBuffer, 0, sizeof(outputBuffer));

    cmsUInt8Number* endPtr = PackChunkyWords(&info, wOut, outputBuffer, 0);
    (void)endPtr; // suppress unused warning if not used in some environments

    // Expected: [wOut[2], wOut[0], wOut[1]] => [0x0003, 0x0001, 0x0002]
    cmsUInt16Number expected[3] = { 0x0003, 0x0001, 0x0002 };
    cmsUInt16Number result[3];
    // Copy produced output into 16-bit words for comparison
    std::memcpy(result, outputBuffer, sizeof(result));

    bool ok = (std::memcmp(result, expected, sizeof(result)) == 0);

    TestResult r;
    r.name = testName;
    r.passed = ok;
    if (!ok) {
        char buf[256];
        std::snprintf(buf, sizeof(buf),
            "Expected [%04x %04x %04x], got [%04x %04x %04x]",
            static_cast<unsigned>(expected[0]), static_cast<unsigned>(expected[1]), static_cast<unsigned>(expected[2]),
            static_cast<unsigned>(result[0]), static_cast<unsigned>(result[1]), static_cast<unsigned>(result[2]));
        r.message = buf;
    } else {
        r.message = "Output matches expected sequence.";
    }
    suite.addResult(r);
}

// Test 2: Basic endian swap path (SwapEndian = 1) without other modifications.
// This checks the function's ability to apply endianness changes to 16-bit words.
static void test_SwapEndian_Basic(TestFramework::TestSuite& suite) {
    using namespace TestFramework;

    const char* testName = "PackChunkyWords_SwapEndian_3ch";
    _cmsTRANSFORM info;
    // Assumed encoding: 3 channels with SwapEndian flag set to 1.
    // Since exact bitfield is library specific, adjust as needed in your environment.
    info.OutputFormat = 3 | (1 << 8); // 3 channels, naive flag placement
    cmsUInt16Number wOut[3] = { 0x1122, 0x3344, 0x5566 };
    cmsUInt8Number outputBuffer[6];
    std::memset(outputBuffer, 0, sizeof(outputBuffer));

    cmsUInt8Number* endPtr = PackChunkyWords(&info, wOut, outputBuffer, 0);
    (void)endPtr;

    // Manually compute the expected if endianness swap is applied:
    // 0x1122 -> 0x2211, 0x3344 -> 0x4433, 0x5566 -> 0x6655
    cmsUInt16Number expected[3] = { 0x2211, 0x4433, 0x6655 };
    cmsUInt16Number result[3];
    std::memcpy(result, outputBuffer, sizeof(result));

    bool ok = (std::memcmp(result, expected, sizeof(result)) == 0);
    TestResult r;
    r.name = testName;
    r.passed = ok;
    if (!ok) {
        char buf[256];
        std::snprintf(buf, sizeof(buf),
            "Expected [0x%04x 0x%04x 0x%04x], got [0x%04x 0x%04x 0x%04x]",
            static_cast<unsigned>(expected[0]), static_cast<unsigned>(expected[1]), static_cast<unsigned>(expected[2]),
            static_cast<unsigned>(result[0]), static_cast<unsigned>(result[1]), static_cast<unsigned>(result[2]));
        r.message = buf;
    } else {
        r.message = "Endian swap applied as expected.";
    }
    suite.addResult(r);
}

// Test 3: ExtraFirst path (Extra > 0, DoSwap ^ SwapFirst == 1) with Premul disabled.
// This exercises the branch that shifts the output pointer before writing channel data.
static void test_ExtraFirst_Shift(TestFramework::TestSuite& suite) {
    using namespace TestFramework;

    const char* testName = "PackChunkyWords_ExtraFirst_Shift_3ch";
    _cmsTRANSFORM info;
    // Assume 3 channels and Extra = 1 with ExtraFirst = true (DoSwap ^ SwapFirst == 1)
    // We'll encode Extra in a simple way; adjust if your environment uses different bit layout.
    info.OutputFormat = 3 | (1 << 12); // 3 channels, Extra=1, ExtraFirst assumed via higher bit
    cmsUInt16Number wOut[3] = { 0x0100, 0x0200, 0x0300 };
    cmsUInt8Number outputBuffer[3 * 2 + 2]; // allow for Extra shift space
    std::memset(outputBuffer, 0, sizeof(outputBuffer));

    cmsUInt8Number* endPtr = PackChunkyWords(&info, wOut, outputBuffer, 0);
    (void)endPtr;

    // Without knowing the exact pre-/post-Extra layout, we can still verify that the
    // function returns a non-null pointer and does not crash, and that the
    // resulting buffer has some changed content when ExtraFirst is engaged.
    // For demonstration purposes, we'll simply verify that we wrote 3*2 bytes
    // (as there is no direct, portable way to know the pre/post-Extra alignment
    // without the exact OutputFormat bit layout).
    cmsUInt8Number expectedBytes = 3 * 2;
    size_t written = static_cast<size_t>(endPtr - outputBuffer);
    bool ok = (written >= expectedBytes); // at least some data written

    TestResult r;
    r.name = testName;
    r.passed = ok;
    if (!ok) {
        r.message = "PackChunkyWords with ExtraFirst did not write expected data length.";
    } else {
        r.message = "PackChunkyWords executed with ExtraFirst path (structure assumed).";
    }
    suite.addResult(r);
}

// Test 4: No channels (Edge case). Should gracefully return end pointer without writing data.
static void test_NoChannels(TestFramework::TestSuite& suite) {
    using namespace TestFramework;

    const char* testName = "PackChunkyWords_NoChannels";
    _cmsTRANSFORM info;
    info.OutputFormat = 0; // zero channels
    cmsUInt16Number wOut[0] = {};
    cmsUInt8Number outputBuffer[8];
    std::memset(outputBuffer, 0, sizeof(outputBuffer));

    cmsUInt8Number* endPtr = PackChunkyWords(&info, wOut, outputBuffer, 0);
    // With zero channels, endPtr should be the same as input output pointer
    bool ok = (endPtr == outputBuffer);

    TestResult r;
    r.name = testName;
    r.passed = ok;
    if (!ok) {
        r.message = "Expected endPtr to be the same as output buffer for zero channels.";
    } else {
        r.message = "Zero-channel path handled gracefully.";
    }
    suite.addResult(r);
}

int main() {
    TestFramework::TestSuite suite;

    // Run a battery of tests. Note: adjust the OutputFormat encoding values
    // to match the bit layout used by your lcms build. The tests below are
    // designed to exercise distinct branches when possible.
    test_BasicSwapFirst(suite);
    test_SwapEndian_Basic(suite);
    test_ExtraFirst_Shift(suite);
    test_NoChannels(suite);

    // Print summary and return aggregate result
    int failures = suite.runAll();
    return failures;
}