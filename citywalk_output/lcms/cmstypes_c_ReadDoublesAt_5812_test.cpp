// Unit test suite for ReadDoublesAt (cmstypes.c) using a custom MemoryIO implementation.
// This test suite is written for C++11, uses no Google Test, and relies on the real
// cmsBool/read APIs from LittleCMS (lcms2). It purposefully focuses on true/false
// branch coverage and boundary conditions as described in the task.
//
// How this works (high level):
// - A MemoryIO (MemoryIOWrapper) is provided that satisfies the cmsIOHANDLER interface.
//   It feeds the underlying library with 15.16 fixed-point encoded numbers (encoded as
//   4-byte little-endian integers, representing value * 65536).
// - ReadDoublesAt is exercised with various At/n combinations to verify:
//   1) Normal operation: correct values are read and position is restored.
//   2) Seek failure: ReadDoublesAt returns FALSE if initial Seek fails.
//   3) n == 0: function returns TRUE and does not attempt reads.
//   4) Partial read failure: when data runs out mid-reading, function returns FALSE.
// - We rely on the real _cmsRead15Fixed16Number implementation in the lcms2 codebase to
//   convert the 4-byte fixed-point numbers to doubles.
// - The test uses plain asserts and prints minimal diagnostic information.

#include <cstring>
#include <vector>
#include <iostream>
#include <cmath>
#include <cassert>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include LittleCMS interface

// External function under test (defined in cmstypes.c in the real project)
extern "C" cmsBool ReadDoublesAt(cmsIOHANDLER* io, cmsUInt32Number At, cmsUInt32Number n, cmsFloat64Number* Values);

// Helper to compare doubles with tolerance
static bool nearly_equal(double a, double b, double tol = 1e-6) {
    return std::fabs(a - b) <= tol;
}

// Memory-based IO wrapper implementing cmsIOHANDLER interface
// The wrapper stores an internal buffer and a current position.
// The first member is cmsIOHANDLER so that (cmsIOHANDLER*)ptr casting works correctly.
struct MemoryIOWrapper {
    cmsIOHANDLER io;            // function pointers and vtable for I/O
    std::vector<uint8_t> buf;     // raw bytes input data (little-endian 4-byte fixed16 values)
    std::size_t pos;               // current read position (in bytes)

    MemoryIOWrapper() : pos(0) {
        // Initialize function pointers to our mocks (see below)
        io.Tell  = MemoryIO_Tell;
        io.Seek  = MemoryIO_Seek;
        io.Read  = MemoryIO_Read;
        io.Write = MemoryIO_Write;
        // Some lcms builds might expect additional fields; we ignore extra context here.
    }

    // Utility to append a 4-byte fixed16 encoding of a double value
    void pushFixed16(double v) {
        int32_t raw = static_cast<int32_t>(llround(v * 65536.0));
        uint8_t b[4];
        // Little-endian encoding
        std::memcpy(b, &raw, 4);
        buf.insert(buf.end(), b, b + 4);
    }
};

// Forward declarations of the IO callback implementations
static cmsUInt32Number MemoryIO_Tell(cmsIOHANDLER* io);
static cmsBool MemoryIO_Seek(cmsIOHANDLER* io, cmsUInt32Number pos);
static cmsBool MemoryIO_Read(cmsIOHANDLER* io, void* Buffer, cmsUInt32Number Size);
static cmsBool MemoryIO_Write(cmsIOHANDLER* io, const void* Buffer, cmsUInt32Number Size);

// Implementations of IO callbacks
static cmsUInt32Number MemoryIO_Tell(cmsIOHANDLER* io) {
    MemoryIOWrapper* self = reinterpret_cast<MemoryIOWrapper*>(io);
    return static_cast<cmsUInt32Number>(self->pos);
}

static cmsBool MemoryIO_Seek(cmsIOHANDLER* io, cmsUInt32Number pos) {
    MemoryIOWrapper* self = reinterpret_cast<MemoryIOWrapper*>(io);
    // Seek is allowed only within buffer bounds
    if (static_cast<std::size_t>(pos) > self->buf.size()) {
        return FALSE;
    }
    self->pos = static_cast<std::size_t>(pos);
    return TRUE;
}

static cmsBool MemoryIO_Read(cmsIOHANDLER* io, void* Buffer, cmsUInt32Number Size) {
    MemoryIOWrapper* self = reinterpret_cast<MemoryIOWrapper*>(io);
    if (self->pos + Size > self->buf.size()) {
        // Not enough data to satisfy read
        return FALSE;
    }
    std::memcpy(Buffer, self->buf.data() + self->pos, static_cast<size_t>(Size));
    self->pos += static_cast<size_t>(Size);
    return TRUE;
}

static cmsBool MemoryIO_Write(cmsIOHANDLER* /*io*/, const void* /*Buffer*/, cmsUInt32Number /*Size*/) {
    // Not used in tests; provide a stub that returns TRUE
    return TRUE;
}

// Test 1: Normal operation - At=0, n=3, expect proper values and position restoration
static void test_normal_operation() {
    // Prepare memory IO with three 15.16 numbers: 1.0, -2.0, 3.5
    MemoryIOWrapper mem;
    mem.buf.clear();
    mem.pos = 0;

    // Helper to insert fixed16-encoded numbers
    mem.pushFixed16(1.0);   // 1.00000
    mem.pushFixed16(-2.0);  // -2.00000
    mem.pushFixed16(3.5);   // 3.50000

    // Call ReadDoublesAt
    cmsFloat64Number out[3] = {0.0, 0.0, 0.0};
    cmsBool ok = ReadDoublesAt(reinterpret_cast<cmsIOHANDLER*>(&mem), 0, 3, out);

    // Assertions
    assert(ok == TRUE && "ReadDoublesAt should return TRUE for complete read");
    assert(nearly_equal(out[0], 1.0) && "First read value should be 1.0");
    assert(nearly_equal(out[1], -2.0) && "Second read value should be -2.0");
    assert(nearly_equal(out[2], 3.5) && "Third read value should be 3.5");
    // Position should be restored to the original position (0)
    assert(mem.pos == 0 && "IO position should be restored to original after ReadDoublesAt");
    std::cout << "Test normal operation: PASSED\n";
}

// Test 2: Seek failure - At is out of range so initial Seek fails
static void test_seek_failure() {
    // Prepare memory IO with some numbers
    MemoryIOWrapper mem;
    mem.buf.clear();
    mem.pos = 0;
    mem.pushFixed16(0.5);
    mem.pushFixed16(1.25);

    cmsFloat64Number out[2] = {0.0, 0.0};
    // Attempt to seek to an out-of-bounds address
    cmsBool ok = ReadDoublesAt(reinterpret_cast<cmsIOHANDLER*>(&mem), 10000, 2, out);

    // Expect failure
    assert(ok == FALSE && "ReadDoublesAt should fail when initial Seek cannot be satisfied");
    // Data should be untouched in terms of content (no read performed)
    // We cannot rely on Values content in a failed path; just ensure not crashing
    std::cout << "Test seek failure: PASSED\n";
}

// Test 3: n == 0 - Should return TRUE and not read anything
static void test_zero_n() {
    MemoryIOWrapper mem;
    mem.buf.clear();
    mem.pos = 0;
    // Pre-fill without reading anything
    mem.pushFixed16(0.0);
    cmsFloat64Number out[1] = {123.456}; // arbitrary initial value

    cmsBool ok = ReadDoublesAt(reinterpret_cast<cmsIOHANDLER*>(&mem), 0, 0, out);

    // Should succeed and leave out unchanged
    assert(ok == TRUE && "ReadDoublesAt should return TRUE when n == 0");
    assert(nearly_equal(out[0], 123.456) && "Output should remain unchanged when n == 0");
    // Position should still be at initial (0)
    assert(mem.pos == 0 && "IO position should remain unchanged when n == 0");
    std::cout << "Test zero n: PASSED\n";
}

// Test 4: Partial read failure - not enough data for all requested items
static void test_partial_read_failure() {
    MemoryIOWrapper mem;
    mem.buf.clear();
    mem.pos = 0;

    // Only two values available: encode two numbers
    mem.pushFixed16(0.75);
    mem.pushFixed16(-0.25);
    // Request three numbers starting at At=0
    cmsFloat64Number out[3] = {0.0, 0.0, 0.0};

    cmsBool ok = ReadDoublesAt(reinterpret_cast<cmsIOHANDLER*>(&mem), 0, 3, out);

    // Expect failure due to insufficient data for the third read
    assert(ok == FALSE && "ReadDoublesAt should fail if data runs out mid-read");
    // No guaranteed restoration of position on failure (as per implementation)
    std::cout << "Test partial read failure: PASSED\n";
}

int main() {
    // Run all tests
    test_normal_operation();
    test_seek_failure();
    test_zero_n();
    test_partial_read_failure();

    std::cout << "All tests completed.\n";
    return 0;
}