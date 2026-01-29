// Test suite for Unroll4BytesSwapSwapFirst in cmspack.c
// This test uses plain C++11 (no GTest) and relies on the project headers
// for type definitions (cmsUInt8Number, cmsUInt16Number, cmsUInt32Number, etc.)
// It exercises the function by supplying known input bytes and validating
// the resulting 16-bit words and pointer advancement.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <iomanip>


// Bring the C declarations into C++ via extern "C"
extern "C" {
}

// Prototype for the focal function (C linkage)
extern "C" cmsUInt8Number* Unroll4BytesSwapSwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                                     CMSREGISTER cmsUInt16Number wIn[],
                                                     CMSREGISTER cmsUInt8Number* accum,
                                                     CMSREGISTER cmsUInt32Number Stride);

// Helper to mimic 8-bit to 16-bit expansion used in the function (e.g., 0xAB -> 0xABAB)
static inline cmsUInt16Number Make16From8(unsigned char b)
{
    // The macro FROM_8_TO_16 commonly expands to (b << 8) | b
    return static_cast<cmsUInt16Number>((static_cast<uint16_t>(b) << 8) | static_cast<uint16_t>(b));
}

// Basic test: verify correct byte-to-word mapping and pointer advancement
bool test_Unroll4BytesSwapSwapFirst_Basic()
{
    // Input: K=0x11, Y=0x22, M=0x33, C=0x44
    // Accumulator starts at these 4 bytes
    unsigned char input[4] = { 0x11, 0x22, 0x33, 0x44 };
    cmsUInt16Number wIn[4] = { 0, 0, 0, 0 }; // ensure overwritten

    // info and Stride are not used by the function (can be NULL/0)
    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* accPtr = input;
    cmsUInt8Number* accEnd = Unroll4BytesSwapSwapFirst(info, wIn, accPtr, Stride);

    // Expected mapping:
    // wIn[2] = Make16From8(K=0x11) -> 0x1111
    // wIn[1] = Make16From8(Y=0x22) -> 0x2222
    // wIn[0] = Make16From8(M=0x33) -> 0x3333
    // wIn[3] = Make16From8(C=0x44) -> 0x4444
    cmsUInt16Number expected[4] = { 0x3333, 0x2222, 0x1111, 0x4444 };

    bool ok = true;
    for (int i = 0; i < 4; ++i)
    {
        if (wIn[i] != expected[i])
        {
            ok = false;
            break;
        }
    }

    // Ensure the accumulator advanced by 4 bytes
    if (accEnd != input + 4)
        ok = false;

    if (!ok)
        std::cerr << "Test Basic failed: expected {3333,2222,1111,4444}, got {"
                  << std::hex << std::setw(4) << std::setfill('0') << wIn[0] << ","
                  << std::hex << std::setw(4) << std::setfill('0') << wIn[1] << ","
                  << std::hex << std::setw(4) << std::setfill('0') << wIn[2] << ","
                  << std::hex << std::setw(4) << std::setfill('0') << wIn[3]
                  << "}" << std::dec << std::endl;

    return ok;
}

// Edge test: zero and 0xFF values to exercise the macro behavior
bool test_Unroll4BytesSwapSwapFirst_EdgeValues()
{
    // Input: K=0x00, Y=0xFF, M=0x7A, C=0x01
    unsigned char input[4] = { 0x00, 0xFF, 0x7A, 0x01 };
    cmsUInt16Number wIn[4] = { 0, 0, 0, 0 };

    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* accPtr = input;
    cmsUInt8Number* accEnd = Unroll4BytesSwapSwapFirst(info, wIn, accPtr, Stride);

    cmsUInt16Number expected[4] = { 0x7A7A, 0xFFFF, 0x0000, 0x0101 };

    bool ok = true;
    for (int i = 0; i < 4; ++i)
    {
        if (wIn[i] != expected[i])
        {
            ok = false;
            break;
        }
    }
    if (accEnd != input + 4)
        ok = false;

    if (!ok)
        std::cerr << "Test EdgeValues failed: expected {7A7A,FFFF,0000,0101}, got {"
                  << std::hex << std::setw(4) << std::setfill('0') << wIn[0] << ","
                  << std::hex << std::setw(4) << std::setfill('0') << wIn[1] << ","
                  << std::hex << std::setw(4) << std::setfill('0') << wIn[2] << ","
                  << std::hex << std::setw(4) << std::setfill('0') << wIn[3]
                  << "}" << std::dec << std::endl;

    return ok;
}

// Test that non-zero Stride does not affect output (ensures accumulation logic ignores Stride)
bool test_Unroll4BytesSwapSwapFirst_NonZeroStride()
{
    unsigned char input[4] = { 0xAA, 0x55, 0x33, 0x77 };
    cmsUInt16Number wIn[4] = { 0, 0, 0, 0 };

    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 12345; // non-zero

    cmsUInt8Number* accPtr = input;
    cmsUInt8Number* accEnd = Unroll4BytesSwapSwapFirst(info, wIn, accPtr, Stride);

    cmsUInt16Number expected[4] = { Make16From8(0x33), Make16From8(0x55), Make16From8(0xAA), Make16From8(0x77) };

    bool ok = true;
    for (int i = 0; i < 4; ++i)
    {
        if (wIn[i] != expected[i])
        {
            ok = false;
            break;
        }
    }
    if (accEnd != input + 4)
        ok = false;

    if (!ok)
        std::cerr << "Test NonZeroStride failed: expected {"
                  << std::hex << expected[0] << "," << expected[1] << "," << expected[2] << "," << expected[3]
                  << "}, got {"
                  << wIn[0] << "," << wIn[1] << "," << wIn[2] << "," << wIn[3]
                  << "}" << std::dec << std::endl;

    return ok;
}

// Repeated calls: ensure subsequent blocks are processed correctly
bool test_Unroll4BytesSwapSwapFirst_MultipleRuns()
{
    // First block: K=0x12, Y=0x34, M=0x56, C=0x78
    unsigned char block1[4] = { 0x12, 0x34, 0x56, 0x78 };
    cmsUInt16Number wIn1[4] = { 0, 0, 0, 0 };

    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* acc1 = block1;
    cmsUInt8Number* end1 = Unroll4BytesSwapSwapFirst(info, wIn1, acc1, Stride);

    cmsUInt16Number expected1[4] = { Make16From8(0x56), Make16From8(0x34), Make16From8(0x12), Make16From8(0x78) };
    bool ok = true;
    for (int i = 0; i < 4; ++i)
        if (wIn1[i] != expected1[i]) { ok = false; break; }

    if (end1 != block1 + 4) ok = false;

    // Second block: K=0x9A, Y=0xBC, M=0xDE, C=0xF0
    unsigned char block2[4] = { 0x9A, 0xBC, 0xDE, 0xF0 };
    cmsUInt16Number wIn2[4] = { 0, 0, 0, 0 };

    cmsUInt8Number* acc2 = block2;
    cmsUInt8Number* end2 = Unroll4BytesSwapSwapFirst(info, wIn2, acc2, Stride);

    cmsUInt16Number expected2[4] = { Make16From8(0xDE), Make16From8(0xBC), Make16From8(0x9A), Make16From8(0xF0) };
    for (int i = 0; i < 4; ++i)
        if (wIn2[i] != expected2[i]) { ok = false; break; }

    if (end2 != block2 + 4) ok = false;

    if (!ok)
        std::cerr << "Test MultipleRuns failed: first block result or second block result mismatch." << std::endl;

    return ok;
}

int main()
{
    int total = 0;
    int passed = 0;

    std::cout << "Running tests for Unroll4BytesSwapSwapFirst...\n";

    total++;
    if (test_Unroll4BytesSwapSwapFirst_Basic()) { ++passed; std::cout << "[PASS] Basic case\n"; } else { std::cout << "[FAIL] Basic case\n"; }

    total++;
    if (test_Unroll4BytesSwapSwapFirst_EdgeValues()) { ++passed; std::cout << "[PASS] Edge values case\n"; } else { std::cout << "[FAIL] Edge values case\n"; }

    total++;
    if (test_Unroll4BytesSwapSwapFirst_NonZeroStride()) { ++passed; std::cout << "[PASS] Non-zero stride case\n"; } else { std::cout << "[FAIL] Non-zero stride case\n"; }

    total++;
    if (test_Unroll4BytesSwapSwapFirst_MultipleRuns()) { ++passed; std::cout << "[PASS] Multiple runs case\n"; } else { std::cout << "[FAIL] Multiple runs case\n"; }

    std::cout << "Test results: " << passed << " / " << total << " passed.\n";

    return (passed == total) ? 0 : 1;
}