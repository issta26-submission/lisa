// Test suite for cmsBool Type_NamedColor_Write (focal method in cmstypes.c)
// This test suite is designed for a C++11 environment without GTest.
// It provides minimal mocks for the LittleCMS-like types and functions used by Type_NamedColor_Write.
// The goal is to exercise normal and failure branches and verify the output bytes produced by the IO handler.

#include <cstring>
#include <vector>
#include <cstddef>
#include <iostream>
#include <string>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>


// DOMAIN_KNOWLEDGE notes applied in code comments below

// -----------------------------------------------------------------------------
// Minimal domain-relevant typedefs and structures to mirror the focal function's usage
// -----------------------------------------------------------------------------

typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef int cmsBool;
#define TRUE 1
#define FALSE 0

// Forward declaration to match the signature used by Type_NamedColor_Write
struct _cms_typehandler_struct;

// Minimal _cmsDICelem, _cmsContext etc are not required for this focused test
// but Type_NamedColor_Write expects a self pointer (not used in our tests).

// Mock type for Named Color List
#define cmsMAXCHANNELS 4  // used by the original code for Colorant array size

typedef void cmsMLU; // minimal placeholder

typedef struct cmsNAMEDCOLORLIST {
    unsigned char Prefix[33];   // 33 to accommodate potential non-null-termination handling
    unsigned char Suffix[33];
    cmsUInt32Number ColorantCount;
    cmsUInt32Number Count;
} cmsNAMEDCOLORLIST;

// Mock IO handler structure used by the focal function
typedef struct cmsIOHANDLER {
    void* Context;
    cmsBool (*Write)(struct cmsIOHANDLER*, cmsUInt32Number, const void*);
} cmsIOHANDLER;

// Function prototype matching the focal function (extern "C" to avoid name mangling)
extern "C" cmsBool Type_NamedColor_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems);

// Prototypes for the dependent/internal functions used by Type_NamedColor_Write
extern "C" cmsBool _cmsWriteUInt32Number(struct cmsIOHANDLER* io, cmsUInt32Number Value);
extern "C" cmsBool _cmsWriteUInt16Array(struct cmsIOHANDLER* io, cmsUInt32Number N, const cmsUInt16Number* Values);
extern "C" cmsUInt32Number cmsNamedColorCount(const cmsNAMEDCOLORLIST* NamedColorList);
extern "C" cmsBool cmsNamedColorInfo(const cmsNAMEDCOLORLIST* NamedColorList,
                                     cmsUInt32Number i,
                                     char* Root,
                                     cmsMLU* Name,
                                     cmsMLU* MLU,
                                     cmsUInt16Number* PCS,
                                     cmsUInt16Number* Colorant);

// -----------------------------------------------------------------------------
// Test harness globals to control behavior of mocks (to cover true/false branches)
// -----------------------------------------------------------------------------

static int g_fail_next_uint32 = 0;          // when >0, cause _cmsWriteUInt32Number to fail
static int g_fail_next_uint16Array = 0;     // when >0, cause the next _cmsWriteUInt16Array to fail

// Simple buffer collector to verify the bytes written by the IO Write calls
static std::vector<uint8_t> g_writeBuffer;

// Reset the buffer before each test
static void resetBuffer() {
    g_writeBuffer.clear();
}

// Simple little-endian writer helpers (used by mocks to validate written data if needed)
static void writeBytes(const void* data, size_t n) {
    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    g_writeBuffer.insert(g_writeBuffer.end(), bytes, bytes + n);
}

// Mock implementations of the dependent functions used by Type_NamedColor_Write

extern "C" cmsBool _cmsWriteUInt32Number(struct cmsIOHANDLER* io, cmsUInt32Number Value) {
    (void)io; // not used in the mock
    if (g_fail_next_uint32 > 0) {
        --g_fail_next_uint32;
        return FALSE;
    }
    // Write 4 bytes little-endian
    uint32_t v = static_cast<uint32_t>(Value);
    uint8_t b0 = static_cast<uint8_t>(v & 0xFF);
    uint8_t b1 = static_cast<uint8_t>((v >> 8) & 0xFF);
    uint8_t b2 = static_cast<uint8_t>((v >> 16) & 0xFF);
    uint8_t b3 = static_cast<uint8_t>((v >> 24) & 0xFF);
    uint8_t bytes[4] = { b0, b1, b2, b3 };
    writeBytes(bytes, 4);
    return TRUE;
}

extern "C" cmsBool _cmsWriteUInt16Array(struct cmsIOHANDLER* io, cmsUInt32Number N, const cmsUInt16Number* Values) {
    (void)io; // not used in the mock
    if (g_fail_next_uint16Array > 0) {
        --g_fail_next_uint16Array;
        return FALSE;
    }
    // Write N 16-bit values as little-endian
    for (cmsUInt32Number i = 0; i < N; ++i) {
        uint16_t v = static_cast<uint16_t>(Values[i]);
        uint8_t b0 = static_cast<uint8_t>(v & 0xFF);
        uint8_t b1 = static_cast<uint8_t>((v >> 8) & 0xFF);
        g_writeBuffer.push_back(b0);
        g_writeBuffer.push_back(b1);
    }
    return TRUE;
}

extern "C" cmsUInt32Number cmsNamedColorCount(const cmsNAMEDCOLORLIST* NamedColorList) {
    if (!NamedColorList) return 0;
    return NamedColorList->Count;
}

// Simple NamedColorInfo mock: fills Root (32 bytes), PCS (3x16-bit), and Colorant (ColorantCount x 16-bit)
extern "C" cmsBool cmsNamedColorInfo(const cmsNAMEDCOLORLIST* NamedColorList,
                                     cmsUInt32Number i,
                                     char* Root,
                                     cmsMLU* Name,
                                     cmsMLU* MLU,
                                     cmsUInt16Number* PCS,
                                     cmsUInt16Number* Colorant) {
    (void)Name;
    (void)MLU;
    // Fill Root with "ROOTi" plus padding zeros to 32 bytes
    const char* tag = (i == 0) ? "ROOT0" : "ROOT1";
    size_t len = std::strlen(tag);
    // Zero 32 bytes first
    for (size_t k = 0; k < 32; ++k) Root[k] = 0;
    for (size_t k = 0; k < len && k < 32; ++k) Root[k] = static_cast<char>(tag[k]);
    // Fill PCS with deterministic values
    if (PCS) {
        // Example: 100 + i*10, 200 + i*10, 300 + i*10
        PCS[0] = static_cast<cmsUInt16Number>(100 + i);
        PCS[1] = static_cast<cmsUInt16Number>(200 + i);
        PCS[2] = static_cast<cmsUInt16Number>(300 + i);
    }
    // Fill Colorant values according to NamedColorList->ColorantCount
    if (Colorant) {
        for (cmsUInt32Number j = 0; j < NamedColorList->ColorantCount; ++j) {
            Colorant[j] = static_cast<cmsUInt16Number>(i * 10 + static_cast<cmsUInt16Number>(j + 1));
        }
        // fill the remainder with 0s for safety (up to cmsMAXCHANNELS)
        for (cmsUInt32Number j = NamedColorList->ColorantCount; j < cmsMAXCHANNELS; ++j) {
            Colorant[j] = 0;
        }
    }
    return TRUE;
}

// -----------------------------------------------------------------------------
// Test helpers to build a NamedColorList and to invoke the focal function
// -----------------------------------------------------------------------------

// Simple wrapper function to call the focal method
// The real function has C linkage in the original project; we declare extern "C" to call it.
extern "C" cmsBool Type_NamedColor_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems);

static cmsNAMEDCOLORLIST buildNamedColorList(cmsUInt32Number colorantCount, cmsUInt32Number count) {
    cmsNAMEDCOLORLIST l;
    // Prefix and Suffix slightly long to test proper copying and termination
    const char* p = "PREFIX_TEST_STRING_FOR_COLORLIST__";
    const char* s = "SUFFIX_TEST_STRING_FOR_COLORLIST__";
    std::size_t plen = std::min<std::size_t>(std::strlen(p), 32);
    std::size_t slen = std::min<std::size_t>(std::strlen(s), 32);
    std::memset(l.Prefix, 0, sizeof(l.Prefix));
    std::memset(l.Suffix, 0, sizeof(l.Suffix));
    std::memcpy(l.Prefix, p, plen);
    std::memcpy(l.Suffix, s, slen);
    l.ColorantCount = colorantCount;
    l.Count = count;
    return l;
}

// Simple test IO handler object
static cmsIOHANDLER createTestIO() {
    cmsIOHANDLER io;
    io.Context = nullptr;
    io.Write = nullptr;
    return io;
}

// Utility to interpret little-endian 32-bit value from buffer
static uint32_t readUint32LE(const uint8_t* data) {
    return static_cast<uint32_t>(data[0])
           | (static_cast<uint32_t>(data[1]) << 8)
           | (static_cast<uint32_t>(data[2]) << 16)
           | (static_cast<uint32_t>(data[3]) << 24);
}

// Utility to interpret little-endian 16-bit value from buffer
static uint16_t readUint16LE(const uint8_t* data) {
    return static_cast<uint16_t>(data[0])
           | (static_cast<uint16_t>(data[1]) << 8);
}

// Helper to print buffer in hex for debugging
static void dumpBuffer(const std::vector<uint8_t>& buf) {
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);
    for (size_t i = 0; i < buf.size(); ++i) {
        if (i % 16 == 0) std::cout << "\n" << std::setw(4) << i << ": ";
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)buf[i] << " ";
    }
    std::cout << std::dec << "\n";
    std::cout.copyfmt(oldState);
}

// -----------------------------------------------------------------------------
// Minimal test harness (non-terminating assertions) and test cases
// -----------------------------------------------------------------------------

static int g_failures = 0;

// Simple assertion macro that logs failures but does not terminate the test
#define TEST_ASSERT(cond, msg) \
    do { if (!(cond)) { ++g_failures; std::cerr << "TEST FAILURE: " << (msg) << " [FILE " << __FILE__ << ", LINE " << __LINE__ << "]\n"; } } while (0)


// Test 1: Normal path with 2 colors and 3 colorants per color
static void test_NormalPath_MultiColorWrites() {
    // Reset environment
    resetBuffer();
    g_fail_next_uint32 = 0;
    g_fail_next_uint16Array = 0;
    // Build a NamedColorList with 2 colors and 3 colorants (to match writes)
    cmsNAMEDCOLORLIST named = buildNamedColorList(3, 2);

    // Setup the IO handler
    cmsIOHANDLER io;
    // Simple mock Write function: uses global g_writeBuffer
    io.Context = nullptr;
    io.Write = [](struct cmsIOHANDLER* /*self*/, cmsUInt32Number /*n*/, const void* data) -> cmsBool {
        const uint8_t* p = static_cast<const uint8_t*>(data);
        // Copy to global buffer
        g_writeBuffer.insert(g_writeBuffer.end(), p, p + 0); // nothing; actual writes bypass via _cmsWrite* in mock
        // This lambda is a placeholder; actual writes go through _cmsWriteUInt32Number/_cmsWriteUInt16Array
        return TRUE;
    };

    // The Type_NamedColor_Write function will call _cmsWriteUInt32Number and _cmsWriteUInt16Array,
    // which we've implemented to push to g_writeBuffer globally. We need the io to be able to call back.
    // Because we cannot install the lambda as a C function pointer easily here, we'll set Write to a non-functional pointer.
    // In this minimal harness, we rely on the internal mocks to perform writes; the io->Write is not used in tests
    // that exercise ALL internal paths, so we'll ignore the direct Write call.

    // Instead, we will rely solely on the internal mocks to generate g_writeBuffer.
    // Create a dummy self pointer (not used)
    cms_typehandler_struct_dummy:
    struct _cms_typehandler_struct* self = nullptr;

    // Call the focal method
    cmsUInt32Number nItems = 0;
    cmsBool result = Type_NamedColor_Write(self, &io, &named, 2);

    TEST_ASSERT(result == TRUE, "Type_NamedColor_Write should return TRUE on normal path.");
    // Compute expected length: 4 (first uint32) + 4 (nColors) + 4 (ColorantCount)
    // + 32 prefix + 32 suffix + nColors*(32 Root + 3*2 PCS + 3*2 Colorant)
    // For our test: nColors=2, ColorantCount=3
    const size_t header = 4 + 4 + 4;
    const size_t perColor = 32 + (3 * 2) + (3 * 2); // Root 32, PCS 3*2, Colorant 3*2
    const size_t expected = header + 32 + 32 + 2 * perColor;
    // If g_writeBuffer length matches expected, test passes
    if (g_writeBuffer.size() != expected) {
        ++g_failures;
        std::cerr << "NORMAL PATH: Unexpected buffer size. Expected " << expected
                  << " got " << g_writeBuffer.size() << "\n";
        dumpBuffer(g_writeBuffer);
    }

    // Optional: Deep checks on specific regions (header, prefix, suffix, first Root, PCS, Colorant)
    // Check header first 12 bytes should represent 0, nColors=2, ColorantCount=3
    const uint8_t* b = g_writeBuffer.data();
    uint32_t first = readUint32LE(b);
    uint32_t second = readUint32LE(b + 4);
    uint32_t third = readUint32LE(b + 8);
    TEST_ASSERT(first == 0, "First 4 bytes should be 0");
    TEST_ASSERT(second == 2, "Second 4 bytes should be nColors (2)");
    TEST_ASSERT(third == 3, "Third 4 bytes should be ColorantCount (3)");
    // Check prefix region equals named.Prefix (first 32 bytes)
    // Note: Since our mock _cmsWriteUInt32Number writes to g_writeBuffer, and Prefix is copied in the first 32 bytes after header
    // We'll perform a simple check that the 32 bytes after header are non-zero (as a sanity check)
    const uint8_t* prefix_start = b + 12;
    bool prefix_nonzero = false;
    for (size_t i = 0; i < 32; ++i) {
        if (prefix_start[i] != 0) { prefix_nonzero = true; break; }
    }
    TEST_ASSERT(prefix_nonzero, "Prefix region should contain non-zero data (copied from NamedColorList).");
}

// Test 2: Failure in first _ cmsWriteUInt32Number call (simulate immediate write failure)
static void test_Failure_On_FirstUInt32Write() {
    resetBuffer();
    g_writeBuffer.clear();
    g_fail_next_uint32 = 1; // force the next _cmsWriteUInt32Number to fail

    cmsNAMEDCOLORLIST named = buildNamedColorList(3, 2);
    cmsIOHANDLER io;
    io.Context = nullptr;
    io.Write = nullptr; // not used due to test harness

    struct _cms_typehandler_struct* self = nullptr;

    cmsBool result = Type_NamedColor_Write(self, &io, &named, 2);

    TEST_ASSERT(result == FALSE, "Type_NamedColor_Write should return FALSE when first uint32 write fails.");
    TEST_ASSERT(g_writeBuffer.empty(), "Buffer should be empty after immediate write failure.");
}

// Test 3: Failure in PCS write (simulate failure on first _cmsWriteUInt16Array call within color loop)
static void test_Failure_On_PCSWrite() {
    resetBuffer();
    g_writeBuffer.clear();
    g_fail_next_uint32 = 0;
    g_fail_next_uint16Array = 1; // fail on first _cmsWriteUInt16Array call (PCS)

    cmsNAMEDCOLORLIST named = buildNamedColorList(3, 2);
    cmsIOHANDLER io;
    io.Context = nullptr;
    io.Write = nullptr;

    struct _cms_typehandler_struct* self = nullptr;

    cmsBool result = Type_NamedColor_Write(self, &io, &named, 2);

    TEST_ASSERT(result == FALSE, "Type_NamedColor_Write should return FALSE when PCS write fails.");
    // Buffer should contain header + prefix/suffix + first Root (color 0) region
    // Compute expected partial length: 12 (header) + 64 (prefix/suffix) + 32 (Root for color 0)
    const size_t partial = 12 + 64 + 32;
    TEST_ASSERT(g_writeBuffer.size() == partial, "Buffer should contain header + prefix/suffix + first Root on PCS failure.");
}

// Test 4: Single-color case with 1 color and 2 colorants
static void test_SingleColor_SmallCase() {
    resetBuffer();
    g_writeBuffer.clear();
    g_fail_next_uint32 = 0;
    g_fail_next_uint16Array = 0;

    cmsNAMEDCOLORLIST named = buildNamedColorList(2, 1); // 1 color, 2 colorants

    cmsIOHANDLER io;
    io.Context = nullptr;
    io.Write = nullptr;

    struct _cms_typehandler_struct* self = nullptr;

    cmsBool result = Type_NamedColor_Write(self, &io, &named, 1);

    TEST_ASSERT(result == TRUE, "Single color write should succeed.");

    // Buffer size calculation: header (12) + prefix/suffix (64) + color (Root 32 + PCS 6 + Colorant 2*2 = 10) = 32 + 6 + 4
    // Per color size = 32 + 6 + 4 = 42
    // Total = 12 + 64 + 42 = 118
    const size_t expected = 12 + 64 + (32 + 6 + 4) * 1;
    TEST_ASSERT(g_writeBuffer.size() == expected, "Single color test: buffer size should match expected layout.");
}

// -----------------------------------------------------------------------------
// Main entry: drive tests and report results
// -----------------------------------------------------------------------------

int main() {
    // Run tests
    test_NormalPath_MultiColorWrites();
    test_Failure_On_FirstUInt32Write();
    test_Failure_On_PCSWrite();
    test_SingleColor_SmallCase();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED." << std::endl;
        return 1;
    }
}