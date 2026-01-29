// Self-contained unit test suite for the WritePositionTable function
// Focal method recreated with minimal dependencies to enable isolated testing.
// This test suite uses only C++11 standard library and no external test framework.

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


using namespace std;

// Minimal type aliases to resemble CMS types
typedef int cmsBool;
typedef uint32_t cmsUInt32Number;
typedef void* cmsContext;
#define TRUE 1
#define FALSE 0

// Forward declaration of focal types
struct _cms_typehandler_struct;

// Typedef for PositionTableEntryFn
typedef cmsBool (*PositionTableEntryFn)(
    struct _cms_typehandler_struct* self,
    struct /* cmsIOHANDLER */ struct cmsIOHANDLER* io,
    void* Cargo,
    cmsUInt32Number i,
    cmsUInt32Number SizeOfTag
);

// Lightweight implementation of _cms_typehandler_struct
struct _cms_typehandler_struct {};

// IO Buffer to simulate file-like writes/reads
struct IOBuffer {
    std::vector<uint8_t> data;
    size_t pos;
    bool failNextWrite; // simulate single write failure
    IOBuffer() : pos(0), failNextWrite(false) {}
};

// Lightweight cmsIOHANDLER with function pointers as in the focal code
struct cmsIOHANDLER {
    void* ContextID;
    IOBuffer* user; // our internal buffer
    cmsUInt32Number (*Tell)(cmsIOHANDLER*);
    cmsBool (*Seek)(cmsIOHANDLER*, cmsUInt32Number);
    cmsBool (*Write)(cmsIOHANDLER*, const void*, cmsUInt32Number);
};

// Global test hooks to simulate memory allocation behavior
static bool g_allocatorFailNext = false;

// Custom calloc/free wrappers to control allocation behavior in tests
static void* _cmsCalloc(void* ContextID, cmsUInt32Number Count, cmsUInt32Number Size) {
    (void)ContextID;
    if (g_allocatorFailNext) {
        // consume the flag and return NULL to simulate allocation failure
        g_allocatorFailNext = false;
        return NULL;
    }
    return calloc(Count, Size);
}
static void _cmsFree(void* ContextID, void* ptr) {
    (void)ContextID;
    free(ptr);
}

// Small helper to simulate writing a 32-bit little-endian value into the IO
static cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* io, cmsUInt32Number n) {
    uint8_t b[4] = {
        (uint8_t)(n & 0xFF),
        (uint8_t)((n >> 8) & 0xFF),
        (uint8_t)((n >> 16) & 0xFF),
        (uint8_t)((n >> 24) & 0xFF)
    };
    return io->Write(io, b, 4) ? TRUE : FALSE;
}

// IO helper functions
static cmsUInt32Number IO_Tell(struct cmsIOHANDLER* io) {
    if (io == nullptr || io->user == nullptr) return 0;
    return (cmsUInt32Number)io->user->pos;
}

static cmsBool IO_Seek(struct cmsIOHANDLER* io, cmsUInt32Number pos) {
    if (io == nullptr || io->user == nullptr) return FALSE;
    // For test simplicity, allow seeking anywhere within current buffer length or forward
    if (pos > io->user->data.size()) {
        // Expand with zeros if seeking beyond current end (mimic typical behavior)
        io->user->data.resize(pos, 0);
    }
    io->user->pos = pos;
    return TRUE;
}

static cmsBool IO_Write(struct cmsIOHANDLER* io, const void* data, cmsUInt32Number size) {
    if (io == nullptr || io->user == nullptr) return FALSE;
    if (io->user->failNextWrite) {
        io->user->failNextWrite = false;
        return FALSE;
    }
    size_t newPos = io->user->pos + size;
    if (newPos > io->user->data.size()) {
        io->user->data.resize(newPos, 0);
    }
    memcpy(&io->user->data[io->user->pos], data, size);
    io->user->pos = newPos;
    return TRUE;
}

// Focal function rewritten here (to be self-contained for the unit tests)
cmsBool WritePositionTable(struct _cms_typehandler_struct* self,
                           cmsIOHANDLER* io,
                           cmsUInt32Number SizeOfTag,
                           cmsUInt32Number Count,
                           cmsUInt32Number BaseOffset,
                           void *Cargo,
                           PositionTableEntryFn ElementFn)
{
{
    cmsUInt32Number i;
    cmsUInt32Number DirectoryPos, CurrentPos, Before;
    cmsUInt32Number *ElementOffsets = NULL, *ElementSizes = NULL;
    ElementOffsets = (cmsUInt32Number *) _cmsCalloc(io ->ContextID, Count, sizeof(cmsUInt32Number));
    if (ElementOffsets == NULL) goto Error;
    ElementSizes = (cmsUInt32Number *) _cmsCalloc(io ->ContextID, Count, sizeof(cmsUInt32Number));
    if (ElementSizes == NULL) goto Error;
    DirectoryPos = io ->Tell(io);
  
    // Write a fake directory to be filled latter on
    for (i=0; i < Count; i++) {
        if (!_cmsWriteUInt32Number(io, 0)) goto Error;  // Offset
        if (!_cmsWriteUInt32Number(io, 0)) goto Error;  // size
    }
    // Write each element. Keep track of the size as well.
    for (i=0; i < Count; i++) {
        Before = io ->Tell(io);
        ElementOffsets[i] = Before - BaseOffset;
        // Callback to write...
        if (!ElementFn(self, io, Cargo, i, SizeOfTag)) goto Error;
        // Now the size
        ElementSizes[i] = io ->Tell(io) - Before;
    }
    // Write the directory
    CurrentPos = io ->Tell(io);
    if (!io ->Seek(io, DirectoryPos)) goto Error;
    for (i=0; i <  Count; i++) {
        if (!_cmsWriteUInt32Number(io, ElementOffsets[i])) goto Error;
        if (!_cmsWriteUInt32Number(io, ElementSizes[i])) goto Error;
    }
    if (!io ->Seek(io, CurrentPos)) goto Error;
    if (ElementOffsets != NULL) _cmsFree(io ->ContextID, ElementOffsets);
    if (ElementSizes != NULL) _cmsFree(io ->ContextID, ElementSizes);
    return TRUE;
Error:
    if (ElementOffsets != NULL) _cmsFree(io ->ContextID, ElementOffsets);
    if (ElementSizes != NULL) _cmsFree(io ->ContextID, ElementSizes);
    return FALSE;
}
}

// PositionTableEntryFn implementations for tests
static cmsBool ElementFn_Succeed(struct _cms_typehandler_struct* self,
                                cmsIOHANDLER* io,
                                void* Cargo,
                                cmsUInt32Number i,
                                cmsUInt32Number SizeOfTag)
{
    (void)self; (void)Cargo; (void)SizeOfTag;
    cmsUInt32Number v = 0x01010101; // deterministic payload
    return _cmsWriteUInt32Number(io, v) ? TRUE : FALSE;
}

static int g_failIndexForElementFn = -1;
static cmsBool ElementFn_MaybeFail(struct _cms_typehandler_struct* self,
                                   cmsIOHANDLER* io,
                                   void* Cargo,
                                   cmsUInt32Number i,
                                   cmsUInt32Number SizeOfTag)
{
    (void)self; (void)Cargo; (void)SizeOfTag;
    if ((int)i == g_failIndexForElementFn) return FALSE;
    cmsUInt32Number v = 0x01010101;
    return _cmsWriteUInt32Number(io, v) ? TRUE : FALSE;
}

// Test harness helpers
static bool readUInt32LE(const uint8_t* p, cmsUInt32Number &out) {
    out = (cmsUInt32Number)p[0] | ((cmsUInt32Number)p[1] << 8) |
          ((cmsUInt32Number)p[2] << 16) | ((cmsUInt32Number)p[3] << 24);
    return true;
}

static bool test1_SuccessPath() {
    // Setup
    _cms_typehandler_struct* self = new _cms_typehandler_struct();

    IOBuffer buffer;
    cmsIOHANDLER io;
    io.ContextID = NULL;
    io.user = &buffer;
    io.Tell = IO_Tell;
    io.Seek = IO_Seek;
    io.Write = IO_Write;

    g_allocatorFailNext = false;
    // Ensure no allocation failure
    g_allocatorFailNext = false;

    // Call with Count=2, BaseOffset=0
    g_failIndexForElementFn = -1; // never fail
    cmsBool result = WritePositionTable(self, &io, 0, 2, 0, NULL, ElementFn_MaybeFail);

    bool ok = true;
    if (result != TRUE) {
        ok = false;
    }
    // Buffer should contain 24 bytes: directory (16) + two elements (8)
    if (buffer.data.size() != 24) {
        ok = false;
    }

    if (ok) {
        // Read directory: offsets and sizes
        cmsUInt32Number d0, d1, d2, d3;
        readUInt32LE(buffer.data.data() + 0, d0);
        readUInt32LE(buffer.data.data() + 4, d1);
        readUInt32LE(buffer.data.data() + 8, d2);
        readUInt32LE(buffer.data.data() + 12, d3);
        // Expected: [16,4, 20,4]
        if (d0 != 16 || d1 != 4 || d2 != 20 || d3 != 4) ok = false;
        // Element payloads
        cmsUInt32Number v0, v1;
        readUInt32LE(buffer.data.data() + 16, v0);
        readUInt32LE(buffer.data.data() + 20, v1);
        if (v0 != 0x01010101 || v1 != 0x01010101) ok = false;
    }

    delete self;
    return ok;
}

static bool test2_ElementFnFailsOnSecondElement() {
    _cms_typehandler_struct* self = new _cms_typehandler_struct();

    IOBuffer buffer;
    cmsIOHANDLER io;
    io.ContextID = NULL;
    io.user = &buffer;
    io.Tell = IO_Tell;
    io.Seek = IO_Seek;
    io.Write = IO_Write;

    g_allocatorFailNext = false;
    g_failIndexForElementFn = 1; // fail on i==1

    cmsBool result = WritePositionTable(self, &io, 0, 2, 0, NULL, ElementFn_MaybeFail);

    bool ok = true;
    if (result != FALSE) ok = false;
    // Directory should have remained zeros (16 bytes)
    if (buffer.data.size() != 16) ok = false;
    for (size_t i = 0; i < 16; ++i) {
        if (buffer.data[i] != 0) { ok = false; break; }
    }

    delete self;
    return ok;
}

static bool test3_AllocFailureFirstAllocation() {
    _cms_typehandler_struct* self = new _cms_typehandler_struct();

    IOBuffer buffer;
    cmsIOHANDLER io;
    io.ContextID = NULL;
    io.user = &buffer;
    io.Tell = IO_Tell;
    io.Seek = IO_Seek;
    io.Write = IO_Write;

    // Force first allocation to fail
    g_allocatorFailNext = true;

    cmsBool result = WritePositionTable(self, &io, 0, 2, 0, NULL, ElementFn_Succeed);

    bool ok = (result == FALSE);
    // Buffer should be untouched
    if (!buffer.data.empty()) ok = false;

    delete self;
    return ok;
}

static bool test4_WriteFailureDuringDirectory() {
    _cms_typehandler_struct* self = new _cms_typehandler_struct();

    IOBuffer buffer;
    cmsIOHANDLER io;
    io.ContextID = NULL;
    io.user = &buffer;
    io.Tell = IO_Tell;
    io.Seek = IO_Seek;
    io.Write = IO_Write;

    // Simulate failure on the first write attempt during directory creation
    buffer.failNextWrite = true; // cause first _cmsWriteUInt32Number to fail

    cmsBool result = WritePositionTable(self, &io, 0, 2, 0, NULL, ElementFn_Succeed);

    bool ok = (result == FALSE);
    // Buffer should be empty due to first write failure
    if (!buffer.data.empty()) ok = false;

    delete self;
    return ok;
}

static bool test5_BaseOffsetNonZero() {
    _cms_typehandler_struct* self = new _cms_typehandler_struct();

    IOBuffer buffer;
    cmsIOHANDLER io;
    io.ContextID = NULL;
    io.user = &buffer;
    io.Tell = IO_Tell;
    io.Seek = IO_Seek;
    io.Write = IO_Write;

    g_allocatorFailNext = false;
    g_failIndexForElementFn = -1;

    cmsBool result = WritePositionTable(self, &io, 0, 1, 4, NULL, ElementFn_Succeed);

    bool ok = (result == TRUE);
    if (ok) {
        // For Count=1 and BaseOffset=4, ElementOffsets[0] = 16 - 4 = 12
        // Directory should contain 12 and 4
        cmsUInt32Number d0, d1;
        readUInt32LE(buffer.data.data() + 0, d0);
        readUInt32LE(buffer.data.data() + 4, d1);
        if (d0 != 12 || d1 != 4) ok = false;
        // Payload value check
        cmsUInt32Number v;
        readUInt32LE(buffer.data.data() + 8, v);
        if (v != 0x01010101) ok = false;
    }

    delete self;
    return ok;
}

// Entry point
int main() {
    int passed = 0;
    int total = 5;

    cout << "Running WritePositionTable unit tests (self-contained)..." << endl;

    if (test1_SuccessPath()) {
        cout << "[PASS] Test1: Success path writes directory and payload correctly." << endl;
        ++passed;
    } else {
        cout << "[FAIL] Test1: Success path did not behave as expected." << endl;
    }

    if (test2_ElementFnFailsOnSecondElement()) {
        cout << "[PASS] Test2: Element function failure propagates to return FALSE and leaves directory zeroed." << endl;
        ++passed;
    } else {
        cout << "[FAIL] Test2: Element function failure handling incorrect." << endl;
    }

    if (test3_AllocFailureFirstAllocation()) {
        cout << "[PASS] Test3: Allocation failure on first _cmsCalloc leads to FALSE return and no buffer writes." << endl;
        ++passed;
    } else {
        cout << "[FAIL] Test3: Allocation failure handling incorrect." << endl;
    }

    if (test4_WriteFailureDuringDirectory()) {
        cout << "[PASS] Test4: Write failure during directory results in FALSE return and no partial writes." << endl;
        ++passed;
    } else {
        cout << "[FAIL] Test4: Directory write failure handling incorrect." << endl;
    }

    if (test5_BaseOffsetNonZero()) {
        cout << "[PASS] Test5: Non-zero BaseOffset correctly affects ElementOffsets calculation." << endl;
        ++passed;
    } else {
        cout << "[FAIL] Test5: BaseOffset handling incorrect." << endl;
    }

    cout << "Summary: " << passed << " / " << total << " tests passed." << endl;
    return (passed == total) ? 0 : 1;
}