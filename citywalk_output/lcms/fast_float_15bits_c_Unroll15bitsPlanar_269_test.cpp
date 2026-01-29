/*
 * Unit test suite for Unroll15bitsPlanar (fast_float_15bits.c)
 * 
 * Notes:
 * - This test file is written to be compiled in a C++11 environment.
 * - It relies on the project's internal types and the Unroll15bitsPlanar symbol
 *   as exposed by the project's headers (e.g., fast_float_internal.h).
 * - We provide a minimal test harness (no GoogleTest). Tests are designed to be
 *   non-terminating on failure (continue execution and report all failures).
 * - Test cases exercise basic, non-swapped and swapped scenarios for a 2-channel
 *   planar path. The exact encoding of InputFormat macros is project-specific; we
 *   set up the _cmstransform_struct/head so that the code paths are activated.
 * - Static members and private/internal implementation remain untouched. We access
 *   static-like behavior through public-facing function entry points.
 * - All tests are wrapped with explanatory comments describing intent, setup, and
 *   expected outcomes.
 */

#include <cstring>
#include <iostream>
#include <cstdint>
#include <fast_float_internal.h>


// Include the project's internal header for type and macro definitions.
// This is environment-specific; ensure the include path is correct in your build.

// Extern declaration of the function under test. Use the project's macro-protected
// signature to preserve ABI. If CMSREGISTER is defined differently in your build,
// the macro should be defined by the included header. We rely on the environment
// to provide the correct prototype.
extern "C" cmsUInt8Number* Unroll15bitsPlanar(CMSREGISTER struct _cmstransform_struct* CMMcargo,
                                             CMSREGISTER cmsUInt16Number wIn[],
                                             CMSREGISTER cmsUInt8Number* accum,
                                             CMSREGISTER cmsUInt32Number Stride);

/* Simple non-terminating assertion macros.
   They print a message on failure and continue execution. */
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": " << (msg) << std::endl; \
    } \
} while (0)

#define EXPECT_EQ(val1, val2, msg) do { \
    ++g_tests_run; \
    if ((val1) != (val2)) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": " << (msg) \
                  << " | Expected: " << (val2) \
                  << ", Actual: " << (val1) << std::endl; \
    } \
} while (0)

/* Helper to reset a known-good environment for each test. */
static void reset_buffers(cmsUInt16Number* wIn, size_t wIn_len,
                          cmsUInt8Number* accum_buf, size_t accum_len)
{
    if (wIn && wIn_len) {
        std::memset(wIn, 0, wIn_len * sizeof(cmsUInt16Number));
    }
    if (accum_buf && accum_len) {
        std::memset(accum_buf, 0, accum_len * sizeof(cmsUInt8Number));
    }
}

/* Test 1: Basic planar Unroll15bitsPlanar with 2 channels, no swapping, no endianness flip.
   Intent:
   - Ensure that, when DoSwap is disabled, the function reads two consecutive 16-bit
     values from accum and stores them into wIn in channel order (0 then 1).
   - Verify the function returns Init + 2 as per implementation.
*/
static void test_Unroll15bitsPlanar_noSwap_twoChannels()
{
    // Local material to describe a minimal transformation payload.
    // The library expects _cmstransform_struct as the cargo. The first bytes/cell
    // should resemble _xform_head so macros can read InputFormat.
    struct _cmstransform_struct cmmcargo;

    // Point to the first member as _xform_head to set InputFormat.
    _xform_head* head = (_xform_head*) &cmmcargo;
    // Encoding: we rely on the project's macros. Here we attempt to encode DoSwap=0,
    // Reverse=0, SwapEndian=0, Extra=0 and nChan=2. The exact bits are project-specific;
    // setting zero is a common starting point; if the environment encodes 2 channels
    // differently, adjust accordingly in your setup.
    head->InputFormat = 0x0000; // Placeholder: set according to your project-specific encoding.

    cmsUInt16Number wIn[2] = {0, 0};

    // Prepare accum buffer with two 16-bit samples: 0x1234 and 0x5678.
    cmsUInt8Number accum_buf[8];
    cmsUInt16Number* accum16 = reinterpret_cast<cmsUInt16Number*>(accum_buf);
    accum16[0] = 0x1234;
    accum16[1] = 0x5678;

    cmsUInt8Number* Init = accum_buf;
    cmsUInt32Number Stride = 1u;

    // Call the function under test.
    cmsUInt8Number* ret = Unroll15bitsPlanar(&cmmcargo, wIn, accum_buf, Stride);

    // Assertions
    // - wIn should contain the two input samples in channel order.
    EXPECT_EQ(wIn[0], 0x1234, "wIn[0] should read first accum 16-bit value when DoSwap is false.");
    EXPECT_EQ(wIn[1], 0x5678, "wIn[1] should read second accum 16-bit value when DoSwap is false.");

    // - Return value should point to Init + 2 (two samples consumed).
    EXPECT_TRUE(ret == Init + 2, "Return pointer should be Init + 2 after planar unroll.");

}\n\n
/* Test 2: Basic planar Unroll15bitsPlanar with 2 channels, swapped order (DoSwap enabled).
   Intent:
   - Ensure that, when DoSwap is true, the channel order is reversed in wIn.
   - We place two samples after the initial accum offset and verify wIn[0] corresponds to
     the second sample, wIn[1] to the first.
*/
static void test_Unroll15bitsPlanar_withSwap_twoChannels()
{
    struct _cmstransform_struct cmmcargo;
    _xform_head* head = (_xform_head*) &cmmcargo;

    // Placeholder encoding with DoSwap = true. The actual value must reflect your
    // project's encoding; adjust if necessary.
    head->InputFormat = 0x0001; // Placeholder: assume LSB bit toggles DoSwap.

    cmsUInt16Number wIn[2] = {0, 0};

    cmsUInt8Number accum_buf[8];
    cmsUInt16Number* accum16 = reinterpret_cast<cmsUInt16Number*>(accum_buf);
    accum16[0] = 0x9ABC;  // First sample in memory
    accum16[1] = 0xDEF0;  // Second sample in memory

    cmsUInt8Number* Init = accum_buf;
    cmsUInt32Number Stride = 1u;

    cmsUInt8Number* ret = Unroll15bitsPlanar(&cmmcargo, wIn, accum_buf, Stride);

    // Expect swapped behavior: wIn[1] should be first sample (0x9ABC) and wIn[0]
    // should be second sample (0xDEF0), given DoSwap swaps indices.
    EXPECT_EQ(wIn[0], 0xDEF0, "With DoSwap, wIn[0] should be the second sample.");
    EXPECT_EQ(wIn[1], 0x9ABC, "With DoSwap, wIn[1] should be the first sample.");

    EXPECT_TRUE(ret == Init + 2, "Return pointer should be Init + 2 after planar unroll (swap case).");
}\n\n
/* Test 3: Edge case (minimal channel count) - ensure function handles single channel
   gracefully. We expect same value to be written to wIn[0] and return Init+2.
   Intent:
   - Validate the function's loop works for nChan = 1.
*/
static void test_Unroll15bitsPlanar_singleChannel()
{
    struct _cmstransform_struct cmmcargo;
    _xform_head* head = (_xform_head*) &cmmcargo;

    // Placeholder: DoSwap off; single channel (nChan=1)
    head->InputFormat = 0x0000;

    cmsUInt16Number wIn[1] = {0};

    cmsUInt8Number accum_buf[4];
    cmsUInt16Number* accum16 = reinterpret_cast<cmsUInt16Number*>(accum_buf);
    accum16[0] = 0x0F0F;

    cmsUInt8Number* Init = accum_buf;
    cmsUInt32Number Stride = 1u;

    cmsUInt8Number* ret = Unroll15bitsPlanar(&cmmcargo, wIn, accum_buf, Stride);

    EXPECT_EQ(wIn[0], 0x0F0F, "Single channel: wIn[0] should read the only accum value.");
    EXPECT_TRUE(ret == Init + 2, "Return pointer should be Init + 2 for single channel case.");
}\n\n
/* Main entry: run all tests and print a final summary. */
int main() {
    // Run tests
    test_Unroll15bitsPlanar_noSwap_twoChannels();
    test_Unroll15bitsPlanar_withSwap_twoChannels();
    test_Unroll15bitsPlanar_singleChannel();

    // Summary
    std::cout << "Unit test summary: ";
    std::cout << g_tests_run << " tests run, ";
    std::cout << g_tests_failed << " failures." << std::endl;
    return (g_tests_failed == 0) ? 0 : 1;
}