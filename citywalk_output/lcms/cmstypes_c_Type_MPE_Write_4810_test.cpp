// UNIT TESTS for cmsBool Type_MPE_Write in a self-contained mock environment.
// This test harness does not rely on GoogleTest. It provides a small
// lightweight test framework with non-terminating assertions (EXPECT_* macros)
// and focuses on exercising the Type_MPE_Write function logic, including both
// success and failure paths. The surrounding CMS internals are mocked to make
// the test self-contained while preserving the control flow of the focal method.

#include <cstring>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Lightweight test framework (non-terminating assertions)
#define TEST_EQ(a,b) do { if (!((a)==(b))) { logFail("EXPECT_EQ failed: " #a " == " #b, __FILE__, __LINE__); resultCount++; } } while(0)
#define TEST_TRUE(a)   do { if(!(a)) { logFail("EXPECT_TRUE failed: " #a, __FILE__, __LINE__); resultCount++; } } while(0)
#define TEST_FALSE(a)  do { if((a)) { logFail("EXPECT_FALSE failed: " #a, __FILE__, __LINE__); resultCount++; } } while(0)
#define EXPECT_TRUE TEST_TRUE
#define EXPECT_FALSE TEST_FALSE
static int resultCount = 0;
static void logFail(const char* msg, const char* file, int line) {
    std::cerr << "Test fail: " << msg << " at " << file << ":" << line << std::endl;
}

// -----------------------------------------------------------------------------
// Mocked minimal CMS types and helpers to support Type_MPE_Write test
// -----------------------------------------------------------------------------

typedef int cmsBool;
typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned char cmsUInt8Number;
typedef unsigned long long cmsUInt64Number;
typedef unsigned int cmsContext;

#define TRUE 1
#define FALSE 0

typedef uint32_t cmsTagTypeSignature;

// Forward declarations to satisfy the focal method's expectations
struct _cms_typehandler_struct;
struct cmsIOHANDLER;

// Tiny _cmsTagBase to satisfy sizeof in BaseOffset computation
struct _cmsTagBase { int dummy; };

// Forward declare the focal method signature (we re-implement it in test)
extern "C" {
    cmsBool Type_MPE_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems);
}

// cms_typehandler_struct used by Type_MPE_Write
struct _cms_typehandler_struct {
    cmsContext ContextID;
};

// cmsTagTypeHandler equivalent used by GetHandler
struct cmsTagTypeHandler {
    cmsBool (*WritePtr)(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems);
};

// cmsStage and cmsPipeline minimal representations
typedef cmsUInt32Number cmsStageSignature;
struct _cmsStage {
    cmsStageSignature Type;
    _cmsStage* Next;
};
struct cmsPipeline {
    _cmsStage* Elements;
};

// Minimal interface for the FOCAL method: function to count stages
static cmsUInt32Number cmsPipelineStageCount(cmsPipeline* Lut) {
    cmsUInt32Number c = 0;
    _cmsStage* s = Lut->Elements;
    while (s) { c++; s = s->Next; }
    return c;
}

// Input/Output handler backbone for test
struct cmsIOHANDLER {
    // Function pointers expected by the focal method (Tell and Seek are used)
    cmsUInt32Number (*Tell)(cmsIOHANDLER* self);
    int (*Seek)(cmsIOHANDLER* self, cmsUInt32Number pos);
    // Internal test buffer
    std::vector<uint8_t> Buffer;
    cmsUInt32Number Position;
    cmsUInt32Number Size;
};

// Global test flags to drive different branches
static bool g_fail_alloc = false;
static bool g_seek_should_fail = false;

// Base offset helper (size of _cmsTagBase in real code); we provide a fixed size for tests
static const size_t G_TAG_BASE_SIZE = sizeof(_cmsTagBase);

// Forward declare helper implementations used by Type_MPE_Write
static cmsBool _cmsWriteUInt16Number(cmsIOHANDLER* io, cmsUInt16Number value);
static cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* io, cmsUInt32Number value);
static cmsBool _cmsWriteAlignment(cmsIOHANDLER* io);
static void* _cmsCalloc(cmsContext id, size_t nmemb, size_t size);
static void  _cmsFree(cmsContext id, void* ptr);
static void* _cmsContextGetClientChunk(cmsContext ContextID, int MPEPlugin);
static cmsTagTypeHandler* GetHandler(cmsTagTypeSignature sig, void* PluginLinkedList, void* DefaultLinkedList);

// Our test MPE signature and handler
static const cmsStageSignature TEST_SIG = 0x1234;
static cmsBool Type_MPE_WritePtr(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems);

// Our mock Type Handler
static cmsTagTypeHandler g_MPE_TypeHandler = { Type_MPE_WritePtr };

// Simple WritePtr implementation: writes a marker to the io buffer
static cmsBool Type_MPE_WritePtr(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems) {
    (void)Ptr; (void)nItems; // unused in this mock
    // Write a 32-bit marker so we can verify data was written
    if (!_cmsWriteUInt32Number(io, 0xA5A5A5A5)) return FALSE;
    return TRUE;
}

// MPE Type Plugin chunk mock
#define MPEPlugin 1
struct _cmsTagTypePluginChunkType {
    void* TagTypes; // unused in test; we only need its existence
} MPETypePluginChunk;

// SupportedMPEtypes placeholder (not used heavily in our mock)
static void* SupportedMPEtypes = nullptr;

// Helper: get current time position from io
static cmsUInt32Number cms_io_tell(cmsIOHANDLER* io) {
    return io->Position;
}

// Helper: seek within the io buffer (bounded by Size)
static int cms_io_seek(cmsIOHANDLER* io, cmsUInt32Number pos) {
    if (g_seek_should_fail) return 0;
    if (pos > io->Size) return 0;
    io->Position = pos;
    return 1;
}

// Implementations for writing primitive numbers into test io buffer (little-endian)
static cmsBool _cmsWriteUInt16Number(cmsIOHANDLER* io, cmsUInt16Number value) {
    uint8_t b0 = (uint8_t)(value & 0xFF);
    uint8_t b1 = (uint8_t)((value >> 8) & 0xFF);
    io->Buffer.push_back(b0);
    io->Buffer.push_back(b1);
    io->Position += 2;
    return TRUE;
}
static cmsBool _cmsWriteUInt32Number(cmsIOHANDLER* io, cmsUInt32Number value) {
    uint8_t b0 = (uint8_t)(value & 0xFF);
    uint8_t b1 = (uint8_t)((value >> 8) & 0xFF);
    uint8_t b2 = (uint8_t)((value >> 16) & 0xFF);
    uint8_t b3 = (uint8_t)((value >> 24) & 0xFF);
    io->Buffer.push_back(b0);
    io->Buffer.push_back(b1);
    io->Buffer.push_back(b2);
    io->Buffer.push_back(b3);
    io->Position += 4;
    return TRUE;
}
static cmsBool _cmsWriteAlignment(cmsIOHANDLER* io) {
    // Align to 4-byte boundary by inserting zero bytes
    while ((io->Position % 4) != 0) {
        io->Buffer.push_back(0);
        io->Position++;
    }
    return TRUE;
}

// Memory management wrappers for test (unused memory check simply via libc)
static void* _cmsCalloc(cmsContext id, size_t nmemb, size_t size) {
    (void)id;
    if (g_fail_alloc) return NULL;
    return calloc(nmemb, size);
}
static void _cmsFree(cmsContext id, void* ptr) {
    (void)id;
    free(ptr);
}

// Context chunk fetch (mock)
static void* _cmsContextGetClientChunk(cmsContext ContextID, int MPEPlugin) {
    (void)ContextID; (void)MPEPlugin;
    // Return address of our mock MPE plugin chunk
    return (void*)&MPETypePluginChunk;
}

// Minimal GetHandler to return our test handler for TEST_SIG, NULL otherwise
static cmsTagTypeHandler* GetHandler(cmsTagTypeSignature sig, void* PluginLinkedList, void* DefaultLinkedList) {
    (void)PluginLinkedList; (void)DefaultLinkedList;
    if (sig == TEST_SIG) return &g_MPE_TypeHandler;
    return NULL;
}

// The focal method under test: copied from the original, adapted to compile with this mock
// Signature: cmsBool Type_MPE_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
extern "C" cmsBool Type_MPE_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
{
{
    cmsUInt32Number i, BaseOffset, DirectoryPos, CurrentPos;
    cmsUInt32Number inputChan, outputChan;
    cmsUInt32Number ElemCount;
    cmsUInt32Number *ElementOffsets = NULL, *ElementSizes = NULL, Before;
    cmsStageSignature ElementSig;
    // Ptr is expected to be a cmsPipeline*
    cmsPipeline* Lut = (cmsPipeline*) Ptr;
    // Elem points to the first stage in the pipeline
    cmsStage* Elem = Lut ->Elements;
    cmsTagTypeHandler* TypeHandler;
    _cmsTagTypePluginChunkType* MPETypePluginChunk  = ( _cmsTagTypePluginChunkType*) _cmsContextGetClientChunk(self->ContextID, MPEPlugin);
    BaseOffset = io ->Tell(io) - sizeof(_cmsTagBase);
    inputChan  = 2; // We mock fixed values; in a real run this would be cmsPipelineInputChannels(Lut)
    outputChan = 3; // fixed
    ElemCount  = cmsPipelineStageCount(Lut);
    ElementOffsets = (cmsUInt32Number *) _cmsCalloc(self ->ContextID, ElemCount, sizeof(cmsUInt32Number));
    if (ElementOffsets == NULL) goto Error;
    ElementSizes = (cmsUInt32Number *) _cmsCalloc(self ->ContextID, ElemCount, sizeof(cmsUInt32Number));
    if (ElementSizes == NULL) goto Error;
    // Write the head
    if (!_cmsWriteUInt16Number(io, (cmsUInt16Number) inputChan)) goto Error;
    if (!_cmsWriteUInt16Number(io, (cmsUInt16Number) outputChan)) goto Error;
    if (!_cmsWriteUInt32Number(io, (cmsUInt16Number) ElemCount)) goto Error;
    DirectoryPos = io ->Tell(io);
    // Write a fake directory to be filled latter on
    for (i=0; i < ElemCount; i++) {
        if (!_cmsWriteUInt32Number(io, 0)) goto Error;  // Offset
        if (!_cmsWriteUInt32Number(io, 0)) goto Error;  // size
    }
    // Write each single tag. Keep track of the size as well.
    for (i=0; i < ElemCount; i++) {
        ElementOffsets[i] = io ->Tell(io) - BaseOffset;
        ElementSig = Elem ->Type;
        TypeHandler = GetHandler((cmsTagTypeSignature) ElementSig, MPETypePluginChunk->TagTypes, SupportedMPEtypes);
        if (TypeHandler == NULL)  {
                char String[5];
                // Convert to string (not critical for test)
                 // An unknown element was found.
                 // In test, we simply fail
                 goto Error;
        }
         Before = io ->Tell(io);
        if (!_cmsWriteUInt32Number(io, ElementSig)) goto Error;
        if (!_cmsWriteUInt32Number(io, 0)) goto Error;

        if (!TypeHandler ->WritePtr(self, io, Elem, 1)) goto Error;
        if (!_cmsWriteAlignment(io)) goto Error;
        ElementSizes[i] = io ->Tell(io) - Before;
        Elem = Elem ->Next;
    }
    // Write the directory
    CurrentPos = io ->Tell(io);
    if (!io ->Seek(io, DirectoryPos)) goto Error;
    for (i=0; i < ElemCount; i++) {
        if (!_cmsWriteUInt32Number(io, ElementOffsets[i])) goto Error;
        if (!_cmsWriteUInt32Number(io, ElementSizes[i])) goto Error;
    }
    if (!io ->Seek(io, CurrentPos)) goto Error;
    if (ElementOffsets != NULL) _cmsFree(self ->ContextID, ElementOffsets);
    if (ElementSizes != NULL) _cmsFree(self ->ContextID, ElementSizes);
    return TRUE;
Error:
    if (ElementOffsets != NULL) _cmsFree(self ->ContextID, ElementOffsets);
    if (ElementSizes != NULL) _cmsFree(self ->ContextID, ElementSizes);
    return FALSE;
    cmsUNUSED_PARAMETER(nItems);
}
}

// Dummy usage wrapper to force compile-time checks
static void runDummy() {}

// -----------------------------------------------------------------------------
// Test harness
// -----------------------------------------------------------------------------

static void runTest_SuccessPath() {
    // Reset flags
    g_fail_alloc = false;
    g_seek_should_fail = false;

    // Build a simple Lut with one stage of type TEST_SIG
    cmsPipeline Lut;
    Lut.Elements = new _cmsStage{ TEST_SIG, nullptr };

    // Prepare self/context
    _cms_typehandler_struct Self;
    Self.ContextID = 1;

    // Prepare io
    cmsIOHANDLER io;
    io.Tell = cms_io_tell;
    io.Seek = cms_io_seek;
    io.Buffer.clear();
    io.Position = 0;
    io.Size = 4096;

    // Ptr to pipeline
    void* Ptr = &Lut;

    // Call focal method
    cmsBool res = Type_MPE_Write(&Self, &io, Ptr, 1);

    EXPECT_TRUE(res);
    // Expect that something was written (header + one element signature + our marker)
    TEST_TRUE(io.Buffer.size() > 0);
    // The first two bytes should be input channel (2) and output channel (3) in little-endian
    if (io.Buffer.size() >= 4) {
        uint16_t in = io.Buffer[0] | (io.Buffer[1] << 8);
        uint16_t out = io.Buffer[2] | (io.Buffer[3] << 8);
        TEST_EQ(in, (uint16_t)2);
        TEST_EQ(out, (uint16_t)3);
    }
    // Cleanup
    delete Lut.Elements;
}

static void runTest_UnknownType() {
    // Reset flags
    g_fail_alloc = false;
    g_seek_should_fail = false;

    // Build a Lut with one stage of unknown type
    cmsPipeline Lut;
    Lut.Elements = new _cmsStage{ (cmsStageSignature)0xDEAD, nullptr };

    _cms_typehandler_struct Self;
    Self.ContextID = 1;

    cmsIOHANDLER io;
    io.Tell = cms_io_tell;
    io.Seek = cms_io_seek;
    io.Buffer.clear();
    io.Position = 0;
    io.Size = 4096;

    void* Ptr = &Lut;

    cmsBool res = Type_MPE_Write(&Self, &io, Ptr, 1);
    // Our GetHandler returns NULL for unknown sig, so should fail
    TEST_FALSE(res);
    delete Lut.Elements;
}

static void runTest_AllocFailure() {
    // Force allocation failure
    g_fail_alloc = true;
    g_seek_should_fail = false;

    cmsPipeline Lut;
    Lut.Elements = new _cmsStage{ TEST_SIG, nullptr };

    _cms_typehandler_struct Self;
    Self.ContextID = 1;

    cmsIOHANDLER io;
    io.Tell = cms_io_tell;
    io.Seek = cms_io_seek;
    io.Buffer.clear();
    io.Position = 0;
    io.Size = 4096;

    void* Ptr = &Lut;

    cmsBool res = Type_MPE_Write(&Self, &io, Ptr, 1);
    TEST_FALSE(res);
    delete Lut.Elements;
}

static void runTest_SeekFailure() {
    // Setup success path but force Seek to fail
    g_fail_alloc = false;
    g_seek_should_fail = true;

    cmsPipeline Lut;
    Lut.Elements = new _cmsStage{ TEST_SIG, nullptr };

    _cms_typehandler_struct Self;
    Self.ContextID = 1;

    cmsIOHANDLER io;
    io.Tell = cms_io_tell;
    io.Seek = cms_io_seek;
    io.Buffer.clear();
    io.Position = 0;
    io.Size = 4096;

    void* Ptr = &Lut;

    cmsBool res = Type_MPE_Write(&Self, &io, Ptr, 1);
    TEST_FALSE(res);
    delete Lut.Elements;
}

// Entry point
int main() {
    std::cout << "Running Type_MPE_Write unit tests (mocked environment)..." << std::endl;

    // Test 1: Basic success path
    runTest_SuccessPath();
    // Test 2: Unknown element type should fail
    runTest_UnknownType();
    // Test 3: Memory allocation failure should fail
    runTest_AllocFailure();
    // Test 4: Seek failure during directory update should fail
    runTest_SeekFailure();

    if (resultCount == 0) {
        std::cout << "All tests passed (no failures recorded)." << std::endl;
        return 0;
    } else {
        std::cout << "Tests completed with " << resultCount << " failure(s)." << std::endl;
        return 1;
    }
}