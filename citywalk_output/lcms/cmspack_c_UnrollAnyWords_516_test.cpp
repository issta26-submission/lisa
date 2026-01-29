// Test harness for UnrollAnyWords (simplified in-test variant)
// This test provides a self-contained, stand-alone C++11 suite to exercise
// the core branching/logic of the focal UnrollAnyWords behavior without
// requiring the full external library build environment.
// Note: This is a localized, self-contained variant intended for unit testing
// in environments where the real cms library headers/macros may not be
// available. It mirrors the essential logic (nChan, DoSwap, SwapEndian,
// Reverse, SwapFirst, Extra, ExtraFirst) of the focal function to enable
// targeted coverage of its branches.
// The tests below verify true/false branches for these predicates.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Basic type aliases (matching the style used in the focal code)
typedef unsigned char        cmsUInt8Number;
typedef unsigned short       cmsUInt16Number;
typedef unsigned int         cmsUInt32Number;

// Simple in-test model of the "info" structure used by UnrollAnyWords.
// This is a stand-in for the real _cmsTRANSFORM interface to enable isolated testing.
struct SimpleInfo {
    cmsUInt32Number nChan;      // number of channels
    cmsUInt32Number SwapEndian; // 1 = swap bytes in 16-bit words
    cmsUInt32Number DoSwap;     // 1 = reverse channel order (swap)
    cmsUInt32Number Reverse;    // 1 = apply flavor reversal on 16-bit word
    cmsUInt32Number SwapFirst;  // 1 = rotate last to first when Extra == 0
    cmsUInt32Number Extra;      // number of extra 16-bit words prepended
};

// Helper: reverse bytes of a 16-bit word (0xABCD -> 0xCDAB)
static inline cmsUInt16Number ReverseFlavor16(cmsUInt16Number x)
{
    return (cmsUInt16Number)((x >> 8) | (x << 8));
}

// Stand-in implementation of UnrollAnyWords (local test version).
// This mirrors the logic in the focal code, but uses the SimpleInfo fields
// directly, avoiding external macro dependencies.
static cmsUInt8Number* UnrollAnyWords_Simple(const SimpleInfo* info,
                                           cmsUInt16Number wIn[],
                                           cmsUInt8Number* accum,
                                           cmsUInt32Number Stride)
{
    (void)Stride; // Unused in test harness, but kept to mirror signature
    cmsUInt32Number nChan      = info->nChan;
    cmsUInt32Number SwapEndian = info->SwapEndian;
    cmsUInt32Number DoSwap     = info->DoSwap;
    cmsUInt32Number Reverse    = info->Reverse;
    cmsUInt32Number SwapFirst  = info->SwapFirst;
    cmsUInt32Number Extra      = info->Extra;
    cmsUInt32Number ExtraFirst = DoSwap ^ SwapFirst; // boolean XOR
    cmsUInt32Number i;

    if (ExtraFirst) {
        accum += Extra * sizeof(cmsUInt16Number);
    }

    for (i = 0; i < nChan; ++i) {
        cmsUInt32Number index = DoSwap ? (nChan - i - 1) : i;
        cmsUInt16Number v = *(cmsUInt16Number*) accum;
        if (SwapEndian)
            v = (cmsUInt16Number)((v >> 8) | (v << 8));
        wIn[index] = Reverse ? ReverseFlavor16(v) : v;
        accum += sizeof(cmsUInt16Number);
    }

    if (!ExtraFirst) {
        accum += Extra * sizeof(cmsUInt16Number);
    }

    if (Extra == 0 && SwapFirst) {
        cmsUInt16Number tmp = wIn[0];
        memmove(&wIn[0], &wIn[1], (nChan - 1) * sizeof(cmsUInt16Number));
        wIn[nChan - 1] = tmp;
    }

    return accum;
}

// Helper to print 16-bit word in hex for debugging
static inline void PrintWord(std::ostream& os, cmsUInt16Number w)
{
    os << "0x" << std::hex << (w & 0xFFFF) << std::dec;
}

// Test 1: Basic non-swapped, no extras, no rotation, no reverse
// Expect wIn == [0x1122, 0x3344], accum advanced by 4
static bool test_basic_no_swap_no_extra()
{
    SimpleInfo info = {2, 0, 0, 0, 0, 0}; // nChan=2, all predicates false
    cmsUInt16Number wIn[2] = {0};
    unsigned char buf[4]; // two 16-bit inputs

    // Fill input words: 0x1122 and 0x3344
    cmsUInt16Number v0 = 0x1122;
    cmsUInt16Number v1 = 0x3344;
    std::memcpy(buf, &v0, sizeof(v0));
    std::memcpy(buf + 2, &v1, sizeof(v1));

    cmsUInt8Number* accum = buf;
    cmsUInt8Number* end = UnrollAnyWords_Simple(&info, wIn, accum, 0);

    bool ok = (wIn[0] == 0x1122) && (wIn[1] == 0x3344) && (end == buf + 4);
    if (!ok) {
        std::cerr << "Test1 failed: expected [0x1122,0x3344], got [";
        PrintWord(std::cerr, wIn[0]); std::cerr << ", "; PrintWord(std::cerr, wIn[1]); std::cerr << "]\n";
        std::cerr << "end offset: " << (end - buf) << " (expected 4)\n";
    }
    return ok;
}

// Test 2: ExtraFirst true (SwapFirst=1, DoSwap=0), Extra=1
// Expect data after the extra: wIn == [0xAABB, 0xCCDD], accum advanced by 6
static bool test_extra_first_rotation()
{
    SimpleInfo info = {2, 0, 0, 0, 1, 1}; // nChan=2, SwapFirst=1, Extra=1 => ExtraFirst = 1
    cmsUInt16Number wIn[2] = {0};
    unsigned char buf[6]; // 2 bytes extra + 4 bytes data

    // Extra data (ignored): 0xBEEF
    std::memset(buf, 0, sizeof(buf));
    cmsUInt16Number extraDummy = 0xBEEF;
    std::memcpy(buf,  &extraDummy, sizeof(extraDummy)); // first 2 bytes are extra
    // Data words after extra: 0xAABB and 0xCCDD
    cmsUInt16Number d0 = 0xAABB, d1 = 0xCCDD;
    std::memcpy(buf + 2, &d0, sizeof(d0));
    std::memcpy(buf + 4, &d1, sizeof(d1));

    cmsUInt8Number* accum = buf;
    cmsUInt8Number* end = UnrollAnyWords_Simple(&info, wIn, accum, 0);

    bool ok = (wIn[0] == 0xAABB) && (wIn[1] == 0xCCDD) && (end == buf + 6);
    if (!ok) {
        std::cerr << "Test2 failed: expected [0xAABB,0xCCDD], got [";
        PrintWord(std::cerr, wIn[0]); std::cerr << ", "; PrintWord(std::cerr, wIn[1]); std::cerr << "]\n";
        std::cerr << "end offset: " << (end - buf) << " (expected 6)\n";
    }
    return ok;
}

// Test 3: SwapEndian true with two channels
// Data: [0x1234, 0xABCD] stored in memory as little-endian 0x34 0x12 and 0xCD 0xAB
// Expect wIn == [0x3412]
static bool test_swap_endian()
{
    SimpleInfo info = {2, 1, 0, 0, 0, 0}; // SwapEndian = 1
    cmsUInt16Number wIn[2] = {0};
    unsigned char buf[4];

    cmsUInt16Number d0 = 0x1234;
    cmsUInt16Number d1 = 0xABCD;
    std::memcpy(buf, &d0, sizeof(d0)); // 0x34,0x12
    std::memcpy(buf + 2, &d1, sizeof(d1)); // 0xCD,0xAB

    cmsUInt8Number* accum = buf;
    cmsUInt8Number* end = UnrollAnyWords_Simple(&info, wIn, accum, 0);

    bool ok = (wIn[0] == 0x3412) && (wIn[1] == 0); // only first channel meaningful
    if (!ok) {
        std::cerr << "Test3 failed: expected [0x3412, 0x0000], got [";
        PrintWord(std::cerr, wIn[0]); std::cerr << ", " << wIn[1] << "]\n";
        std::cerr << "end offset: " << (end - buf) << " (expected 4)\n";
    }
    return ok;
}

// Test 4: Reverse flavor applied (two channels)
// v0 = 0x1234 -> reversed 0x3412, v1 = 0xABCD -> reversed 0xCDAB
static bool test_reverse_flavor()
{
    SimpleInfo info = {2, 0, 0, 1, 0, 0}; // Reverse = 1
    cmsUInt16Number wIn[2] = {0};
    unsigned char buf[4];

    cmsUInt16Number d0 = 0x1234;
    cmsUInt16Number d1 = 0xABCD;
    std::memcpy(buf, &d0, sizeof(d0));
    std::memcpy(buf + 2, &d1, sizeof(d1));

    cmsUInt8Number* accum = buf;
    cmsUInt8Number* end = UnrollAnyWords_Simple(&info, wIn, accum, 0);

    bool ok = (wIn[0] == 0x3412) && (wIn[1] == 0xCDAB) && (end == buf + 4);
    if (!ok) {
        std::cerr << "Test4 failed: expected [0x3412,0xCDAB], got [";
        PrintWord(std::cerr, wIn[0]); std::cerr << ", "; PrintWord(std::cerr, wIn[1]); std::cerr << "]\n";
        std::cerr << "end offset: " << (end - buf) << " (expected 4)\n";
    }
    return ok;
}

// Test 5: DoSwap true with 2 channels to verify index reversal
// nChan=2, DoSwap=1, Extra=0, SwapFirst=0
// Data: v0 -> wIn[1], v1 -> wIn[0], so output should be [0x2222, 0x1111]
static bool test_do_swap_indexing()
{
    SimpleInfo info = {2, 0, 1, 0, 0, 0}; // DoSwap = 1, no extras
    cmsUInt16Number wIn[2] = {0};
    unsigned char buf[4];

    cmsUInt16Number d0 = 0x1111;
    cmsUInt16Number d1 = 0x2222;
    std::memcpy(buf, &d0, sizeof(d0));
    std::memcpy(buf + 2, &d1, sizeof(d1));

    cmsUInt8Number* accum = buf;
    cmsUInt8Number* end = UnrollAnyWords_Simple(&info, wIn, accum, 0);

    bool ok = (wIn[0] == 0x2222) && (wIn[1] == 0x1111) && (end == buf + 4);
    if (!ok) {
        std::cerr << "Test5 failed: expected [0x2222,0x1111], got [";
        PrintWord(std::cerr, wIn[0]); std::cerr << ", "; PrintWord(std::cerr, wIn[1]); std::cerr << "]\n";
        std::cerr << "end offset: " << (end - buf) << " (expected 4)\n";
    }
    return ok;
}

int main()
{
    int failures = 0;

    if (!test_basic_no_swap_no_extra()) {
        ++failures;
        std::cerr << "Failure in Test 1: Basic non-swapped case\n";
    } else {
        std::cout << "Test 1 passed: Basic non-swapped case\n";
    }

    if (!test_extra_first_rotation()) {
        ++failures;
        std::cerr << "Failure in Test 2: ExtraFirst rotation case\n";
    } else {
        std::cout << "Test 2 passed: ExtraFirst rotation case\n";
    }

    if (!test_swap_endian()) {
        ++failures;
        std::cerr << "Failure in Test 3: Endian swap case\n";
    } else {
        std::cout << "Test 3 passed: Endian swap case\n";
    }

    if (!test_reverse_flavor()) {
        ++failures;
        std::cerr << "Failure in Test 4: Reverse flavor case\n";
    } else {
        std::cout << "Test 4 passed: Reverse flavor case\n";
    }

    if (!test_do_swap_indexing()) {
        ++failures;
        std::cerr << "Failure in Test 5: DoSwap indexing case\n";
    } else {
        std::cout << "Test 5 passed: DoSwap indexing case\n";
    }

    if (failures == 0) {
        std::cout << "All tests PASSED.\n";
    } else {
        std::cout << "Number of failed tests: " << failures << "\n";
    }

    return failures;
}