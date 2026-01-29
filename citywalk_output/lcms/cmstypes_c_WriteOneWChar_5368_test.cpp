// A lightweight, self-contained C++11 test harness for the focal function:
// cmsBool WriteOneWChar(cmsIOHANDLER* io,  _cmsDICelem* e, cmsUInt32Number i, const wchar_t * wcstr, cmsUInt32Number BaseOffset)
// This test does not depend on GoogleTest. It uses a small in-process harness and a
// minimal fake cmsIOHANDLER to capture position (Tell) and simulate writes.
// It assumes the broader cms types are available from the cms library headers
// (e.g., lcms2_internal.h or lcms2.h) when compiling and linking with the library sources
// that contain the focal function implementation (WriteOneWChar).
// The test focuses on validating the two code paths in WriteOneWChar:
//  - wcstr == NULL: Offsets[i] and Sizes[i] are reset to 0.
//  - wcstr != NULL: Offsets[i] is computed from the position, and Sizes[i] reflects how many bytes were written.

#include <lcms2_internal.h>
#include <iostream>
#include <cwchar>
#include <cstring>


// Forward-declare the focal function and types with C linkage, as defined by the library.
// We rely on the library headers to provide the actual definitions for cmsBool, cmsUInt32Number, etc.
extern "C" {
    // The actual signature comes from the focal source cmstypes.c
    // We declare it here to enable linkage with the library sources being tested.
    typedef int cmsBool;                 // usually cmsBool is an int (TRUE/FALSE) in LittleCMS
    typedef unsigned int cmsUInt32Number;

    // Forward declarations of dependent types as provided by the library headers.
    // In the real project, these will come from the included cms headers.
    typedef struct _cmsIOHANDLER cmsIOHANDLER;

    // The internal dictionary element structure used by WriteOneWChar.
    typedef struct _cmsDICelem {
        // Typical LittleCMS structure: Offsets[] and Sizes[] arrays exist in the actual type.
        // We assume at least one element is indexable (i >= 0).
        cmsUInt32Number Offsets[1024];
        cmsUInt32Number Sizes[1024];
    } _cmsDICelem;

    // The focal function under test (link against library implementation)
    cmsBool WriteOneWChar(cmsIOHANDLER* io,  _cmsDICelem* e, cmsUInt32Number i, const wchar_t * wcstr, cmsUInt32Number BaseOffset);
}

// Simple test harness control
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Simple non-terminating assertion macro
#define ASSERT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (cond) { ++g_tests_passed; } \
    else { ++g_tests_failed; std::cerr << "Test failed: " << (msg) << "\n"; } \
} while(0)

// A minimal fake IO handler to intercept Tell() and simulate Write().
// We place cmsIOHANDLER as the first member to allow safe upcasting from cmsIOHANDLER* to our FakeIO*.
struct FakeIO {
    cmsIOHANDLER base;  // must be first member
    unsigned int pos;    // current "cursor" position
};

// Forward declarations of the fake callbacks expected by cmsIOHANDLER
static cmsBool FakeWrite(struct cmsIOHANDLER* io, const void* Buffer, cmsUInt32Number Size);
static cmsUInt32Number FakeTell(struct cmsIOHANDLER* io);

// Initialize a FakeIO instance with a given initial position
static FakeIO* CreateFakeIO(unsigned int initialPosition) {
    FakeIO* f = new FakeIO();
    // Initialize function pointers. The layout of cmsIOHANDLER is provided by the library.
    // We assume the function pointers are named 'Write' and 'Tell' in the cmsIOHANDLER struct.
    f->base.Write = FakeWrite;
    f->base.Tell  = FakeTell;
    // Some implementations may require additional function pointers (Read, Seek). They can be left NULL
    // as the focal function WriteOneWChar only uses Tell/Write in combination with _cmsWriteWCharArray.
    f->base.Context = nullptr; // optional, if the library uses it
    f->pos = initialPosition;
    return f;
}

// Cleanup
static void DestroyFakeIO(FakeIO* f) {
    delete f;
}

// FakeWrite increments the internal position by the number of bytes attempted to write
static cmsBool FakeWrite(struct cmsIOHANDLER* io, const void* /*Buffer*/, cmsUInt32Number Size) {
    // io is actually the address of FakeIO::base, which is the first member.
    FakeIO* f = (FakeIO*)io;
    // Simulate a successful write by advancing the cursor
    f->pos += Size;
    return 1; // TRUE
}

// FakeTell returns the current cursor position
static cmsUInt32Number FakeTell(struct cmsIOHANDLER* io) {
    FakeIO* f = (FakeIO*)io;
    return f->pos;
}

// Utility to print a test header
static void print_header(const char* name) {
    std::cout << "\n=== Test: " << name << " ===" << std::endl;
}

// Test 1: wcstr == NULL, ensure Offsets[i] and Sizes[i] are reset to 0 and return TRUE
static void test_WriteOneWChar_null_string() {
    print_header("WriteOneWChar with NULL wcstr (should reset sizes/offsets and return TRUE)");
    // Prepare IO with initial position 0
    FakeIO* fio = CreateFakeIO(0);

    // Prepare e with a single element
    _cmsDICelem e;
    // Initialize to zero for deterministic behavior
    std::memset(&e, 0, sizeof(e));
    cmsUInt32Number index = 0;
    cmsUInt32Number baseOffset = 0;

    // Call the focal function
    cmsBool result = WriteOneWChar(&fio->base, &e, index, (const wchar_t*)nullptr, baseOffset);

    // Validate
    ASSERT_TRUE(result != 0, "WriteOneWChar should return TRUE for NULL wcstr");
    ASSERT_TRUE(e.Offsets[index] == 0, "Offsets[0] should be 0 when wcstr is NULL and BaseOffset is 0");
    ASSERT_TRUE(e.Sizes[index] == 0, "Sizes[0] should be 0 when wcstr is NULL");
    DestroyFakeIO(fio);
}

// Test 2: wcstr non-null with BaseOffset 0, ensure Offsets[0] equals initial position and Size > 0
static void test_WriteOneWChar_nonnull_simple() {
    print_header("WriteOneWChar with small non-null wcstr and BaseOffset=0");
    FakeIO* fio = CreateFakeIO(0);

    _cmsDICelem e;
    std::memset(&e, 0, sizeof(e));
    cmsUInt32Number index = 0;
    cmsUInt32Number baseOffset = 0;
    const wchar_t* wc = L"AB";

    cmsBool result = WriteOneWChar(&fio->base, &e, index, wc, baseOffset);

    // After writing, Tell should be > Before (0) and Sizes[0] should reflect that delta
    // Offsets[0] is (Before - BaseOffset) == 0
    unsigned int expectedOffset = 0;
    ASSERT_TRUE(result != 0, "WriteOneWChar should return TRUE for non-null wcstr");
    ASSERT_TRUE(e.Offsets[index] == expectedOffset, "Offsets[0] should equal initial position (Before - BaseOffset)");
    ASSERT_TRUE(e.Sizes[index] > 0, "Sizes[0] should be positive after writing non-empty string");
    // Also ensure the IO cursor advanced (simulated by FakeWrite)
    // Since initial position was 0, pos should equal Sizes[0]
    // We can't directly read pos here, but we can re-run a small assertion:
    // pos is accessible via the FakeIO, so verify that it's consistent with Sizes
    unsigned int written = fio->pos; // after WriteOneWChar
    ASSERT_TRUE(written == e.Sizes[index], "Cursor position should equal written byte count for simple case");
    DestroyFakeIO(fio);
}

// Test 3: wcstr non-null with non-zero initial position and non-zero BaseOffset
static void test_WriteOneWChar_nonnull_with_baseoffset_and_initialpos() {
    print_header("WriteOneWChar with non-null wcstr, initial pos != 0, and BaseOffset != 0");
    FakeIO* fio = CreateFakeIO(10); // initial cursor 10

    _cmsDICelem e;
    std::memset(&e, 0, sizeof(e));
    cmsUInt32Number index = 0;
    cmsUInt32Number baseOffset = 5; // non-zero
    const wchar_t* wc = L"XYZ";

    cmsBool result = WriteOneWChar(&fio->base, &e, index, wc, baseOffset);

    // Before = 10, Offsets[0] should be 10 - 5 = 5
    ASSERT_TRUE(result != 0, "WriteOneWChar should return TRUE for non-null wcstr with initial pos");
    ASSERT_TRUE(e.Offsets[index] == (fio->pos - baseOffset), "Offsets[0] should reflect (Before - BaseOffset)");
    ASSERT_TRUE(e.Offsets[index] == 5, "Offsets[0] should equal 5 when initial pos is 10 and BaseOffset is 5");
    ASSERT_TRUE(e.Sizes[index] > 0, "Sizes[0] should be positive after writing non-empty string");
    // And the internal cursor should have advanced by Sizes[0]
    unsigned int written = fio->pos;
    ASSERT_TRUE(written == e.Sizes[index] + 10, "Cursor should equal initial pos plus written bytes");
    DestroyFakeIO(fio);
}

// Entry point
int main() {
    // Run tests
    test_WriteOneWChar_null_string();
    test_WriteOneWChar_nonnull_simple();
    test_WriteOneWChar_nonnull_with_baseoffset_and_initialpos();

    // Summary
    std::cout << "\nTest summary: " << g_tests_passed << " passed, " << g_tests_failed << " failed, "
              << g_tests_run << " total tests.\n";

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}