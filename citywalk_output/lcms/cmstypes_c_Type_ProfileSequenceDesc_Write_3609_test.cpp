// Unit tests for the focal method Type_ProfileSequenceDesc_Write from cmstypes.c
// This test suite is designed for a C++11 environment without GTest.
// It relies on the Little CMS (lcms2) internal types and IO mechanism.
// The tests use a minimal in-memory cmsIOHANDLER (Write path) to exercise the
// write path of the Type_ProfileSequenceDesc_Write function.
// Note: The test assumes that SaveDescription handles a NULL cmsMLU* gracefully
// (i.e., it does not write anything for NULL and returns TRUE). If your
// environment differs, adjust the NULL Manufacturer/Model handling accordingly.

#include <cstring>
#include <vector>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Attempt to pull in the LittleCMS core API.
// Paths may vary; adjust include path if necessary in your environment.

extern "C" {
    // Forward declaration of the focal function from cmstypes.c.
    // The real project provides this symbol; we declare it here for linkage.
    // We only need the declaration to call it from our tests.
    typedef struct _cms_typehandler_struct cms_typehandler_struct;
    cmsBool Type_ProfileSequenceDesc_Write(struct _cms_typehandler_struct* self,
                                            cmsIOHANDLER* io,
                                            void* Ptr,
                                            cmsUInt32Number nItems);
}

// Forward declare the opaque type used by the function.
struct _cms_typehandler_struct;

// Minimal in-memory IO context to capture writes performed by _cmsWriteUInt... helpers.
struct MockIOContext {
    std::vector<uint8_t> buffer;
    int writeCalls = 0;
    // Option to simulate failure on first write (to cover false branch in the focal method).
    bool failOnFirstWrite = false;
};

// Mock write function matching the cmsIOHANDLER::Write signature used by the library.
// It appends written bytes to the in-memory buffer and can intentionally fail on first call.
extern "C" cmsBool MockWrite(struct cmsIOHANDLER* io, cmsUInt32Number Size, const cmsUInt8Number* Buffer)
{
    if (io == nullptr) return FALSE;
    MockIOContext* ctx = static_cast<MockIOContext*>(io->Context);
    if (ctx == nullptr) return FALSE;
    if (ctx->failOnFirstWrite && ctx->writeCalls == 0) {
        ++ctx->writeCalls;
        return FALSE; // simulate a write error on the very first write
    }
    ++ctx->writeCalls;
    // Append the bytes to internal buffer
    ctx->buffer.insert(ctx->buffer.end(), Buffer, Buffer + Size);
    return TRUE;
}

// Helper to read little-endian 32-bit value from buffer
static uint32_t read_u32(const uint8_t* p) {
    return static_cast<uint32_t>(p[0])
        | (static_cast<uint32_t>(p[1]) << 8)
        | (static_cast<uint32_t>(p[2]) << 16)
        | (static_cast<uint32_t>(p[3]) << 24);
}

// Helper to read little-endian 64-bit value from buffer
static uint64_t read_u64(const uint8_t* p) {
    return static_cast<uint64_t>(p[0])
        | (static_cast<uint64_t>(p[1]) << 8)
        | (static_cast<uint64_t>(p[2]) << 16)
        | (static_cast<uint64_t>(p[3]) << 24)
        | (static_cast<uint64_t>(p[4]) << 32)
        | (static_cast<uint64_t>(p[5]) << 40)
        | (static_cast<uint64_t>(p[6]) << 48)
        | (static_cast<uint64_t>(p[7]) << 56);
}

// Simple assertion helper with message
#define ASSERT_EQ(actual, expected, message)                                  \
    do {                                                                        \
        if ((actual) != (expected)) {                                         \
            std::cerr << "Assertion failed: " << (message)                    \
                      << " | actual: " << (actual) << ", expected: " << (expected) << std::endl; \
            return false;                                                     \
        }                                                                       \
    } while (0)

// Test 1: Basic successful write with a single profile entry.
// Verifies that the first 4+20 bytes match the expected layout for one entry
// and that Type_ProfileSequenceDesc_Write returns TRUE.
bool test_ProfileSequenceDesc_Write_basic() {
    // Prepare IO handler with our mock write function
    MockIOContext ctx;
    memset(&ctx, 0, sizeof(ctx)); // zero init
    cmsIOHANDLER io;
    memset(&io, 0, sizeof(io));
    io.Write = MockWrite;
    io.Context = &ctx;

    // Prepare a cmsSEQ with one entry
    cmsSEQ Seq;
    Seq.n = 1;
    Seq.seq = new cmsPSEQDESC[1];
    Seq.seq[0].deviceMfg = 0x00000057;   // 87
    Seq.seq[0].deviceModel = 0x0000009B; // 155
    Seq.seq[0].attributes = 0x1122334455667788ULL;
    Seq.seq[0].technology = 0x0000000A;  // 10
    Seq.seq[0].Manufacturer = NULL;     // No MLU data for this test
    Seq.seq[0].Model = NULL;

    // Minimal self object (opaque to the test, but required by the API)
    struct _cms_typehandler_struct self;
    memset(&self, 0, sizeof(self));

    // Call the focal function
    cmsBool res = Type_ProfileSequenceDesc_Write(&self, &io, &Seq, 1);

    // Cleanup
    delete[] Seq.seq;

    // Assertions
    if (!res) {
        std::cerr << "test_ProfileSequenceDesc_Write_basic: Function returned FALSE unexpectedly." << std::endl;
        return false;
    }

    // Expected total bytes: 4 (n) + 20 bytes for one entry fields (deviceMfg, deviceModel, attributes, technology)
    // SaveDescription is not writing because Manufacturer/Model are NULL.
    if (ctx.buffer.size() < 24) {
        std::cerr << "test_ProfileSequenceDesc_Write_basic: Insufficient bytes written: " << ctx.buffer.size() << std::endl;
        return false;
    }

    // Validate content (little-endian)
    const uint8_t* data = ctx.buffer.data();
    uint32_t n = read_u32(data);
    ASSERT_EQ(n, 1, "n should be 1");

    uint32_t off = 4;
    uint32_t deviceMfg = read_u32(data + off); off += 4;
    uint32_t deviceModel = read_u32(data + off); off += 4;
    uint64_t attributes = read_u64(data + off); off += 8;
    uint32_t technology = read_u32(data + off); off += 4;

    ASSERT_EQ(deviceMfg, 0x57, "deviceMfg should be 0x57");
    ASSERT_EQ(deviceModel, 0x9B, "deviceModel should be 0x9B");
    ASSERT_EQ(attributes, 0x1122334455667788ULL, "attributes mismatch");
    ASSERT_EQ(technology, 0xA, "technology should be 10");

    return true;
}

// Test 2: Simulate a write failure on the very first write to exercise the FALSE return path.
// Expect Type_ProfileSequenceDesc_Write to return FALSE and not crash.
bool test_ProfileSequenceDesc_Write_failure_on_first_write() {
    // Prepare IO handler with a failing first write
    MockIOContext ctx;
    // Enable first-write failure
    ctx.failOnFirstWrite = true;
    cmsIOHANDLER io;
    memset(&io, 0, sizeof(io));
    io.Write = MockWrite;
    io.Context = &ctx;

    // Prepare a cmsSEQ with one entry
    cmsSEQ Seq;
    Seq.n = 1;
    Seq.seq = new cmsPSEQDESC[1];
    Seq.seq[0].deviceMfg = 111;
    Seq.seq[0].deviceModel = 222;
    Seq.seq[0].attributes = 0;
    Seq.seq[0].technology = 3;
    Seq.seq[0].Manufacturer = NULL;
    Seq.seq[0].Model = NULL;

    struct _cms_typehandler_struct self;
    memset(&self, 0, sizeof(self));

    cmsBool res = Type_ProfileSequenceDesc_Write(&self, &io, &Seq, 1);

    delete[] Seq.seq;

    if (res) {
        std::cerr << "test_ProfileSequenceDesc_Write_failure_on_first_write: Expected FALSE due to first write failure." << std::endl;
        return false;
    }

    // In case of failure, buffer should be either empty or partially filled, but function should not crash.
    return true;
}

// Test 3: Write two entries to ensure multiple-item handling.
// Verifies that the sequence length is reflected and the second entry is serialized properly.
bool test_ProfileSequenceDesc_Write_two_entries() {
    MockIOContext ctx;
    cmsIOHANDLER io;
    memset(&io, 0, sizeof(io));
    io.Write = MockWrite;
    io.Context = &ctx;

    cmsSEQ Seq;
    Seq.n = 2;
    Seq.seq = new cmsPSEQDESC[2];

    // Entry 0
    Seq.seq[0].deviceMfg = 1;
    Seq.seq[0].deviceModel = 2;
    Seq.seq[0].attributes = 0x0102030405060708ULL;
    Seq.seq[0].technology = 5;
    Seq.seq[0].Manufacturer = NULL;
    Seq.seq[0].Model = NULL;

    // Entry 1
    Seq.seq[1].deviceMfg = 10;
    Seq.seq[1].deviceModel = 20;
    Seq.seq[1].attributes = 0x1122334455667788ULL;
    Seq.seq[1].technology = 7;
    Seq.seq[1].Manufacturer = NULL;
    Seq.seq[1].Model = NULL;

    struct _cms_typehandler_struct self;
    memset(&self, 0, sizeof(self));

    cmsBool res = Type_ProfileSequenceDesc_Write(&self, &io, &Seq, 2);

    delete[] Seq.seq;

    if (!res) {
        std::cerr << "test_ProfileSequenceDesc_Write_two_entries: Function returned FALSE unexpectedly." << std::endl;
        return false;
    }

    // Expect 4 bytes for n + 2 * 20 bytes for the two entries = 4 + 40 = 44 bytes minimum
    if (ctx.buffer.size() < 44) {
        std::cerr << "test_ProfileSequenceDesc_Write_two_entries: Insufficient bytes written: " << ctx.buffer.size() << std::endl;
        return false;
    }

    // Validate header
    const uint8_t* data = ctx.buffer.data();
    uint32_t n = read_u32(data); // first 4 bytes
    if (n != 2) {
        std::cerr << "test_ProfileSequenceDesc_Write_two_entries: n should be 2, got " << n << std::endl;
        return false;
    }

    uint32_t off = 4;
    // Entry 0
    uint32_t d0_mfg = read_u32(data + off); off += 4;
    uint32_t d0_model = read_u32(data + off); off += 4;
    uint64_t d0_attrs = read_u64(data + off); off += 8;
    uint32_t d0_tech = read_u32(data + off); off += 4;

    ASSERT_EQ(d0_mfg, 1, "Entry0 mfg mismatch");
    ASSERT_EQ(d0_model, 2, "Entry0 model mismatch");
    ASSERT_EQ(d0_attrs, 0x0102030405060708ULL, "Entry0 attrs mismatch");
    ASSERT_EQ(d0_tech, 5, "Entry0 tech mismatch");

    // Entry 1
    uint32_t d1_mfg = read_u32(data + off); off += 4;
    uint32_t d1_model = read_u32(data + off); off += 4;
    uint64_t d1_attrs = read_u64(data + off); off += 8;
    uint32_t d1_tech = read_u32(data + off); off += 4;

    ASSERT_EQ(d1_mfg, 10, "Entry1 mfg mismatch");
    ASSERT_EQ(d1_model, 20, "Entry1 model mismatch");
    ASSERT_EQ(d1_attrs, 0x1122334455667788ULL, "Entry1 attrs mismatch");
    ASSERT_EQ(d1_tech, 7, "Entry1 tech mismatch");

    return true;
}

// Simple test runner
int main() {
    bool allPassed = true;

    std::cout << "Running tests for Type_ProfileSequenceDesc_Write...\n";

    if (!test_ProfileSequenceDesc_Write_basic()) {
        std::cerr << "Test 1 failed.\n";
        allPassed = false;
    } else {
        std::cout << "Test 1 passed.\n";
    }

    if (!test_ProfileSequenceDesc_Write_failure_on_first_write()) {
        std::cerr << "Test 2 failed.\n";
        allPassed = false;
    } else {
        std::cout << "Test 2 passed.\n";
    }

    if (!test_ProfileSequenceDesc_Write_two_entries()) {
        std::cerr << "Test 3 failed.\n";
        allPassed = false;
    } else {
        std::cout << "Test 3 passed.\n";
    }

    if (allPassed) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED.\n";
        return 1;
    }
}