/*
 * Unit test suite for the focal method:
 *   Type_Text_Description_Read (in cmstypes.c)
 *
 * Notes and assumptions:
 - This test suite is designed to be compiled in a C++11 capable environment.
 - It follows a lightweight, homegrown test harness (no GoogleTest).
 - The tests are designed as black-box style: they exercise the focal function by
   constructing a compatible environment (fake cmsIOHANDLER, a minimal self-like
   structure) and by feeding crafted input data to drive code branches.
 - The tests rely on the LittleCMS internal interfaces to the extent necessary to
   call Type_Text_Description_Read. If your build environment does not expose the
   internal types, you may need to adapt the test Harness to your setup.
 - The test harness uses non-terminating assertions (on failure it reports and
   exits with a non-zero code to aid automated runs).
 - Each test case includes comments explaining the intent and the branch coverage.
 - This file focuses on correctness and coverage rather than performance.
 - This test is intended to be run as a standalone executable. It is not a GTest
   based test; all tests are invoked from main().
*/

#include <cstring>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


using namespace std;

/* ------------------------------------------------------------------------- */
/* Domain-specific lightweight test harness utilities                        */
/* ------------------------------------------------------------------------- */

// Simple non-terminating assertion macro: prints failure and aborts on false
#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        cerr << "ASSERTION FAILED: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << endl; \
        std::exit(EXIT_FAILURE); \
    } \
} while (0)

// Helpers to print test progress
#define TEST_PASSED(name) do { \
    cerr << "[OK] " << name << " passed." << endl; \
} while (0)

#define TEST_FAILED(name, reason) do { \
    cerr << "[FAILED] " << name << " - " << reason << endl; \
    std::exit(EXIT_FAILURE); \
} while (0)

/* ------------------------------------------------------------------------- */
/* FOCAL METHOD INTERFACE AND DEPENDENCIES (minimal scaffolding)               */
/* ------------------------------------------------------------------------- */
/*
 * The real Type_Text_Description_Read has a complex integration with the
 * LittleCMS internal structures. Here we declare only the minimal interface
 * necessary for the test harness to compile and invoke the focal function.
 *
 * Important: This is a test harness portion. If your build environment provides
 * the exact internal headers (lcms2_internal.h) with struct _cms_typehandler_struct
 * and cmsIOHANDLER definitions, you should replace the minimal declarations
 * below with those from your environment.
 *
 * We assume:
 *  - struct _cms_typehandler_struct has a member 'ContextID' (used by the function).
 *  - cmsIOHANDLER provides a Read function pointer with a signature compatible
 *    with the library's usage: cmsUInt32Number (*Read)(cmsIOHANDLER*, void*, cmsUInt32Number, cmsUInt32Number)
 *  - The focal function is externally visible (not static) so that we can declare
 *    and call it from this test harness.
 *
 * If your environment uses different types or a different visibility for the
 * function, you must adjust the test harness accordingly.
 */

// Minimal stand-ins for the types used by the focal function.
// These are intentionally simple and are meant to match the layout expected by
// the focal function under test when compiled in an environment with the real lib.
typedef uint32_t cmsUInt32Number;
typedef uint16_t cmsUInt16Number;
typedef uint8_t  cmsUInt8Number;

/* Forward declare the internal type handler struct (layout-controlled in the real lib) */
typedef struct _cms_typehandler_struct _cms_typehandler_struct;

/* Forward declare the opaque I/O handler used by the focal method */
typedef struct cmsIOHANDLER cmsIOHANDLER;

/* Prototype of the focal method under test (as provided in cmstypes.c) */
extern "C" void* Type_Text_Description_Read(_cms_typehandler_struct* self,
                                          cmsIOHANDLER* io,
                                          cmsUInt32Number* nItems,
                                          cmsUInt32Number SizeOfTag);

/* Since we cannot reliably re-create the real internal structure layout here
   without the exact internal headers, we declare a minimal, compatible proxy
   structure. In environments where the real layout is available, you should include
   the proper internal headers and drop these proxy definitions. */
struct _cms_typehandler_struct {
    void* ContextID; // expected by Type_Text_Description_Read
};

/* Lightweight in-memory IO device to feed the focal function with crafted data */
struct MemoryIO {
    cmsIOHANDLER* io;          // pointer to a cmsIOHANDLER compatible interface
    vector<uint8_t> buffer;      // data to be read by the focal method
    size_t pos;                   // current read position within buffer
};

// The cmsIOHANDLER expected signature for Read
typedef cmsUInt32Number (*ReadFn)(cmsIOHANDLER* io, void* Buffer, cmsUInt32Number size, cmsUInt32Number count);

// A very small wrapper that simulates the library's cmsIOHANDLER
// This is a highly simplified version for testing purposes.
// Note: In a real environment this should be replaced with the actual cmsIOHANDLER
// from the library (lcms2) and wired via its plugin mechanism.
struct cmsIOHANDLER {
    ReadFn Read;          // function pointer used by the focal method
    void*  Context;         // user data or pointer (not strictly needed here)
    uint8_t* data;          // raw data for this test IO
    size_t   size;            // total size of data
    size_t   pos;             // current position
};

// Simple Read implementation that conforms to the expected signature.
// It copies data from the in-memory buffer into the provided destination.
// Returns number of elements read (not bytes), consistent with calls like
// Read(io, buffer, sizeof(char), N).
static cmsUInt32Number test_Read(cmsIOHANDLER* io, void* Buffer, cmsUInt32Number size, cmsUInt32Number count)
{
    if (!io || !Buffer || size == 0 || count == 0) return 0;
    size_t bytesRequested = (size_t)size * (size_t)count;
    MemoryIO* mio = reinterpret_cast<MemoryIO*>(io->Context);
    if (!mio) return 0;
    size_t remaining = mio->buffer.size() - mio->pos;
    size_t toCopy = bytesRequested <= remaining ? bytesRequested : remaining;
    if (toCopy == 0) return 0;
    memcpy(Buffer, mio->buffer.data() + mio->pos, toCopy);
    mio->pos += toCopy;
    // Return how many elements were read (bytesCopied/size)
    return (cmsUInt32Number)(toCopy / size);
}

/* Helper to configure a MemoryIO object for a given data payload */
static cmsIOHANDLER* makeMemoryIO(const vector<uint8_t>& data)
{
    MemoryIO* mio = new MemoryIO();
    cmsIOHANDLER* io = new cmsIOHANDLER();
    io->Read = test_Read;
    io->Context = mio;
    mio->io = io;
    mio->buffer = data;
    mio->pos = 0;
    // For compatibility with the test harness tying, place raw data directly
    io->data = const_cast<uint8_t*>(data.data());
    io->size = data.size();
    io->pos = 0;
    return io;
}

/* Helper to clean up MemoryIO */
static void destroyMemoryIO(cmsIOHANDLER* io)
{
    if (!io) return;
    MemoryIO* mio = reinterpret_cast<MemoryIO*>(io->Context);
    delete io;
    delete mio;
}

/* Helper to create a small assertion failure with a message and return code */
static void fail(const string& where, const string& reason)
{
    cerr << "Test failure at " << where << ": " << reason << endl;
    exit(EXIT_FAILURE);
}

/* ------------------------------------------------------------------------- */
/* Test 1: SizeOfTag too small => Type_Text_Description_Read should return NULL  */
/* ------------------------------------------------------------------------- */
static void Test_SizeOfTagTooSmall()
{
    // Build a tiny tag that is smaller than cmsUInt32Number (4 bytes expected)
    vector<uint8_t> data;
    // AsciiCount (4 bytes) would be read only if we had whole size; but the
    // first guard is SizeOfTag < sizeof(cmsUInt32Number).
    cmsUInt32Number SizeOfTag = 2; // deliberately too small

    // Prepare a fake I/O to satisfy signature; content is irrelevant due to guard.
    data.resize(SizeOfTag);
    for (size_t i=0; i<SizeOfTag; ++i) data[i] = 0;

    cmsIOHANDLER* io = makeMemoryIO(data);

    // Build a minimal self object; ContextID can be NULL for this test as the guard triggers first.
    _cms_typehandler_struct self;
    self.ContextID = NULL;

    cmsUInt32Number nItems = 0;

    void* res = Type_Text_Description_Read(&self, io, &nItems, SizeOfTag);
    if (res != NULL || nItems != 0) {
        fail("Test_SizeOfTagTooSmall", "Expected NULL return and nItems==0 when SizeOfTag is too small");
    }

    destroyMemoryIO(io);
    TEST_PASSED("Test_SizeOfTagTooSmall");
}

/* ------------------------------------------------------------------------- */
/* Test 2: AsciiCount too large => Type_Text_Description_Read should return NULL  */
/* ------------------------------------------------------------------------- */
static void Test_AsciiCountTooLarge()
{
    // Build a data sequence that simulates a large AsciiCount (> 0x7ffff)
    // Layout (little-endian):
    // [0..3] AsciiCount
    // then AsciiCount bytes ASCII content
    vector<uint8_t> data;

    cmsUInt32Number AsciiCount = 0x80000; // 524,288 (> 0x7ffff)
    data.push_back((uint8_t)(AsciiCount & 0xFF));
    data.push_back((uint8_t)((AsciiCount >> 8) & 0xFF));
    data.push_back((uint8_t)((AsciiCount >> 16) & 0xFF));
    data.push_back((uint8_t)((AsciiCount >> 24) & 0xFF));

    // Minimal ASCII payload of AsciiCount bytes
    data.insert(data.end(), AsciiCount, 'A');
    // SizeOfTag large enough to pass the first check
    cmsUInt32Number SizeOfTag = (cmsUInt32Number)(4 + AsciiCount + 8); // ascii + 8 bytes for Unicode header
    cmsIOHANDLER* io = makeMemoryIO(data);

    _cms_typehandler_struct self;
    self.ContextID = NULL;
    cmsUInt32Number nItems = 0;

    void* res = Type_Text_Description_Read(&self, io, &nItems, SizeOfTag);
    if (res != NULL || nItems != 0) {
        fail("Test_AsciiCountTooLarge", "Expected NULL return and nItems==0 when AsciiCount is too large");
    }

    destroyMemoryIO(io);
    TEST_PASSED("Test_AsciiCountTooLarge");
}

/* ------------------------------------------------------------------------- */
/* Test 3: Simple success path without Unicode and ScriptCode (best-case)     */
/*       ASCII string is read, MLU is allocated, and pointer is returned.     */
/* ------------------------------------------------------------------------- */
static void Test_SimpleSuccess_NoUnicode_NoScript()
{
    // Layout plan:
    // [0..3]  AsciiCount (e.g., 5)
    // [4..8]  ASCII bytes "Hello"
    // [9..12] UnicodeCode (ignored in "no unicode present" path)
    // [13..16] UnicodeCount (e.g., 0x00000004 -> test uses 3 for visible)
    // [17..]  Unicode data (UnicodeCount * 2 bytes) - but we set UnicodeCount to 0 to skip
    // For the "No Unicode" path, we need to make sure the branch "if (SizeOfTag < 2*sizeof(UInt32))" triggers
    // and we go to Done after reading ASCII.
    vector<uint8_t> data;

    cmsUInt32Number AsciiCount = 5;
    // ASCII count
    data.push_back((uint8_t)(AsciiCount & 0xFF));
    data.push_back((uint8_t)((AsciiCount >> 8) & 0xFF));
    data.push_back((uint8_t)((AsciiCount >> 16) & 0xFF));
    data.push_back((uint8_t)((AsciiCount >> 24) & 0xFF));

    // ASCII bytes "Hello"
    const char* ascii = "Hello";
    data.insert(data.end(), ascii, ascii + AsciiCount);

    // UnicodeCode (we'll ignore by making SizeOfTag small)
    uint32_t UnicodeCode = 0;
    data.push_back((uint8_t)(UnicodeCode & 0xFF));
    data.push_back((uint8_t)((UnicodeCode >> 8) & 0xFF));
    data.push_back((uint8_t)((UnicodeCode >> 16) & 0xFF));
    data.push_back((uint8_t)((UnicodeCode >> 24) & 0xFF));

    uint32_t UnicodeCount = 0; // zero unicode entries
    data.push_back((uint8_t)(UnicodeCount & 0xFF));
    data.push_back((uint8_t)((UnicodeCount >> 8) & 0xFF));
    data.push_back((uint8_t)((UnicodeCount >> 16) & 0xFF));
    data.push_back((uint8_t)((UnicodeCount >> 24) & 0xFF));

    // No Unicode data since count is 0
    // Small extra leading space so that the "ScriptCode" path does not trigger
    // If SizeOfTag is small enough, we won't attempt ScriptCode reading.
    cmsUInt32Number SizeOfTag = (cmsUInt32Number)(data.size());

    cmsIOHANDLER* io = makeMemoryIO(data);

    _cms_typehandler_struct self;
    self.ContextID = NULL;

    cmsUInt32Number nItems = 0;
    void* res = Type_Text_Description_Read(&self, io, &nItems, SizeOfTag);
    if (res == NULL || nItems != 1) {
        fail("Test_SimpleSuccess_NoUnicode_NoScript", "Expected non-NULL mlu and nItems==1 for simple ASCII path");
    }

    // Clean up
    destroyMemoryIO(io);
    TEST_PASSED("Test_SimpleSuccess_NoUnicode_NoScript");
}

/* ------------------------------------------------------------------------- */
/* Test 4: Unicode present path (reads Unicode as wide chars)                  */
/* ------------------------------------------------------------------------- */
static void Test_UnicodePresent()
{
    // This test builds a scenario with Unicode data present and a Unicode count > 0.
    vector<uint8_t> data;

    cmsUInt32Number AsciiCount = 3;
    data.push_back((uint8_t)(AsciiCount & 0xFF));
    data.push_back((uint8_t)((AsciiCount >> 8) & 0xFF));
    data.push_back((uint8_t)((AsciiCount >> 16) & 0xFF));
    data.push_back((uint8_t)((AsciiCount >> 24) & 0xFF));

    data.insert(data.end(), "Txt" , "Txt" + 3);

    // UnicodeCode + UnicodeCount
    uint32_t UnicodeCode = 0;
    data.push_back((uint8_t)(UnicodeCode & 0xFF));
    data.push_back((uint8_t)((UnicodeCode >> 8) & 0xFF));
    data.push_back((uint8_t)((UnicodeCode >> 16) & 0xFF));
    data.push_back((uint8_t)((UnicodeCode >> 24) & 0xFF));

    uint32_t UnicodeCount = 2;
    data.push_back((uint8_t)(UnicodeCount & 0xFF));
    data.push_back((uint8_t)((UnicodeCount >> 8) & 0xFF));
    data.push_back((uint8_t)((UnicodeCount >> 16) & 0xFF));
    data.push_back((uint8_t)((UnicodeCount >> 24) & 0xFF));

    // UnicodeString: two 16-bit characters, little-endian
    uint16_t ch1 = 0x0041; // 'A'
    uint16_t ch2 = 0x0042; // 'B'
    data.push_back((uint8_t)(ch1 & 0xFF));
    data.push_back((uint8_t)((ch1 >> 8) & 0xFF));
    data.push_back((uint8_t)(ch2 & 0xFF));
    data.push_back((uint8_t)((ch2 >> 8) & 0xFF));

    // ScriptCode block: 2 bytes code, 1 byte count, 67 bytes of padding
    uint16_t ScriptCodeCode = 0x0010;
    uint8_t ScriptCodeCount = 0;
    data.push_back((uint8_t)(ScriptCodeCode & 0xFF));
    data.push_back((uint8_t)((ScriptCodeCode >> 8) & 0xFF));
    data.push_back(ScriptCodeCount);
    data.insert(data.end(), 67, 0);

    // SizeOfTag large enough to include ScriptCode block
    cmsUInt32Number SizeOfTag = (cmsUInt32Number)data.size();

    cmsIOHANDLER* io = makeMemoryIO(data);

    _cms_typehandler_struct self;
    self.ContextID = NULL;

    cmsUInt32Number nItems = 0;
    void* res = Type_Text_Description_Read(&self, io, &nItems, SizeOfTag);

    if (res == NULL || nItems != 1) {
        fail("Test_UnicodePresent", "Expected non-NULL mlu and nItems==1 for Unicode-present path");
    }

    destroyMemoryIO(io);
    TEST_PASSED("Test_UnicodePresent");
}

/* ------------------------------------------------------------------------- */
/* Test 5: ScriptCode present; branch takes 67-byte skip path; ensures we read and skip 67 bytes  */
/* ------------------------------------------------------------------------- */
static void Test_ScriptCodeSkipPath()
{
    // Similar to Test_UnicodePresent but sized to ensure the ScriptCode skip path is taken.
    vector<uint8_t> data;

    cmsUInt32Number AsciiCount = 4;
    data.push_back((uint8_t)(AsciiCount & 0xFF));
    data.push_back((uint8_t)((AsciiCount >> 8) & 0xFF));
    data.push_back((uint8_t)((AsciiCount >> 16) & 0xFF));
    data.push_back((uint8_t)((AsciiCount >> 24) & 0xFF));

    data.insert(data.end(), "More" , "More" + 4);

    uint32_t UnicodeCode = 0;
    data.push_back((uint8_t)(UnicodeCode & 0xFF));
    data.push_back((uint8_t)((UnicodeCode >> 8) & 0xFF));
    data.push_back((uint8_t)((UnicodeCode >> 16) & 0xFF));
    data.push_back((uint8_t)((UnicodeCode >> 24) & 0xFF));

    uint32_t UnicodeCount = 1;
    data.push_back((uint8_t)(UnicodeCount & 0xFF));
    data.push_back((uint8_t)((UnicodeCount >> 8) & 0xFF));
    data.push_back((uint8_t)((UnicodeCount >> 16) & 0xFF));
    data.push_back((uint8_t)((UnicodeCount >> 24) & 0xFF));

    uint16_t ch = 0x0048; // 'H'
    data.push_back((uint8_t)(ch & 0xFF));
    data.push_back((uint8_t)((ch >> 8) & 0xFF));

    // ScriptCode block to trigger 67-byte skip
    uint16_t ScriptCodeCode = 0x0011;
    uint8_t ScriptCodeCount = 1;
    data.push_back((uint8_t)(ScriptCodeCode & 0xFF));
    data.push_back((uint8_t)((ScriptCodeCode >> 8) & 0xFF));
    data.push_back(ScriptCodeCount);
    data.insert(data.end(), 67, 0);

    cmsUInt32Number SizeOfTag = (cmsUInt32Number)data.size();
    cmsIOHANDLER* io = makeMemoryIO(data);

    _cms_typehandler_struct self;
    self.ContextID = NULL;

    cmsUInt32Number nItems = 0;
    void* res = Type_Text_Description_Read(&self, io, &nItems, SizeOfTag);
    if (res == NULL || nItems != 1) {
        fail("Test_ScriptCodeSkipPath", "Expected non-NULL mlu and nItems==1 when ScriptCode path is present");
    }

    destroyMemoryIO(io);
    TEST_PASSED("Test_ScriptCodeSkipPath");
}

/* ------------------------------------------------------------------------- */
/* Main: run all tests                                                        */
/* ------------------------------------------------------------------------- */
int main()
{
    // Run each test in turn and report
    Test_SizeOfTagTooSmall();
    Test_AsciiCountTooLarge();
    Test_SimpleSuccess_NoUnicode_NoScript();
    Test_UnicodePresent();
    Test_ScriptCodeSkipPath();

    cout << "All tests completed." << endl;
    return 0;
}