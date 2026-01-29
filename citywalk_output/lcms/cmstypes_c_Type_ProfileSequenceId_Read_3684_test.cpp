// Unit test suite for Type_ProfileSequenceId_Read (cmstypes.c)
// Target: C++11, no Google Test, lightweight harness adapted to the focal
// method and its dependencies.
// Note: This test suite relies on the LittleCMS internal API surface being
// available via lcms2.h / lcms2_internal.h. It uses a minimal, deterministic
//FakeIO to drive the reading path.
//
// What this tests:
// - Path where Count is read as 0 and a non-null cmsSEQ is allocated and returned.
// - Path where reading Count fails (insufficient data), expecting NULL return.
//
// The tests are designed to be executable from main() and use simple assertions.
// They do not rely on GTest or any other testing framework per the instructions.

#include <cstring>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include LittleCMS internal interfaces.
// Adjust include paths as needed for your build environment.

using namespace std;

// Simple test harness (no GTest). Basic assertion Macros
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[TEST FAILED] " << msg << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        return false; \
    } else { \
        std::cout << "[TEST PASSED] " << msg << "\n"; \
    } \
} while (0)


// ---------------------------------------------------------------------------
// Lightweight fake IO to drive cmstypes Type_ProfileSequenceId_Read
// This fake IO provides just enough surface for _cmsReadUInt32Number(io, &Count)
// to read a 32-bit little-endian value and for io->Tell(io) to be queryable.
// For simplicity, we assume the LittleCMS cmsIOHANDLER interface defines
// function pointers: Read, Tell, and a UserData pointer to store state.
// The actual signatures rely on the internal header; we mirror the common usage:
//
// cmsBool Read(struct _cmsIOHANDLER* io, void* Buffer, cmsUInt32Number n);
// cmsUInt32Number Tell(struct _cmsIOHANDLER* io);
// void* UserData
// NOTE: If your environment uses slightly different typedefs, adjust the signatures accordingly.
// ---------------------------------------------------------------------------

// Forward declaration of helper to get access to io in tests
struct _cms_iohandler; // forward if needed (defined in lcms2_internal.h)
struct _cms_typehandler_struct; // forward if needed (defined in lcms2_internal.h)

// Fake IO holder
struct FakeIOHolder {
    cmsIOHANDLER io;                 // Io handler instance to pass to Type_ProfileSequenceId_Read
    std::vector<cmsUInt8Number> data; // Data buffer to feed into _cmsReadUInt32Number
    size_t pos;                        // Read cursor (logical position in buffer)

    // Constructor: prepare IO with a given data stream
    FakeIOHolder(const std::vector<cmsUInt8Number>& d)
        : data(d), pos(0)
    {
        // Initialize io object with our function pointers and user data
        io.UserData = this;
        io.Read = &FakeIOHolder::ReadFunc;
        io.Tell = &FakeIOHolder::TellFunc;
        // Some builds may require initialization of Write/Tell differently; we only
        // rely on Read and Tell for this focal test.
        // Note: If your environment has a different layout for cmsIOHANDLER, adapt accordingly.
    }

    // Static read function used by _cmsReadUInt32Number
    static cmsBool ReadFunc(struct _cmsIOHANDLER* io, cmsVoid* Buffer, cmsUInt32Number n)
    {
        FakeIOHolder* self = static_cast<FakeIOHolder*>(io->UserData);
        if (self == nullptr) return 0;

        // Ensure enough data in buffer
        if (self->pos + n > self->data.size()) {
            return 0; // simulate read failure
        }

        // Copy data
        std::memcpy(Buffer, &self->data[self->pos], static_cast<size_t>(n));
        self->pos += static_cast<size_t>(n);
        return 1; // success
    }

    // Static Tell function to provide current position to the caller
    static cmsUInt32Number TellFunc(struct _cmsIOHANDLER* io)
    {
        FakeIOHolder* self = static_cast<FakeIOHolder*>(io->UserData);
        if (self == nullptr) return 0;

        // The focal code computes
        // BaseOffset = io->Tell(io) - sizeof(_cmsTagBase);
        // To keep BaseOffset deterministic (avoid unsigned underflow),
        // return a value that makes BaseOffset safe for our test (e.g., 0)
        // We simulate a tag header of 8 bytes, so BaseOffset becomes 0.
        // Adjust if your environment uses a different header size.
        return static_cast<cmsUInt32Number>(self->pos + 8);
    }
};

// ---------------------------------------------------------------------------
// Test 1: Success path when Count is 0
// - Data stream provides a 4-byte little-endian zero representing Count.
// - ReadPositionTable is invoked with Count = 0; it should not fail and
//   Type_ProfileSequenceId_Read should return a non-null cmsSEQ and set nItems = 1.
// ---------------------------------------------------------------------------

bool test_ProfileSequenceId_Read_CountZero_Succeeds() {
    // Prepare IO with Count = 0 (4 bytes: 00 00 00 00)
    std::vector<cmsUInt8Number> data = { 0x00, 0x00, 0x00, 0x00 };
    FakeIOHolder fakeIO(data);

    // Prepare a minimal self ( cms_typehandler_struct ) with a ContextID.
    struct _cms_typehandler_struct* self = static_cast<struct _cms_typehandler_struct*>(
        std::malloc(sizeof(struct _cms_typehandler_struct))
    );
    if (self == nullptr) {
        std::cerr << "Failed to allocate self in test_ProfileSequenceId_Read_CountZero_Succeeds\n";
        return false;
    }
    // Initialize required member used by Type_ProfileSequenceId_Read
    // Note: The actual field name is ContextID in the library; ensure it exists.
    // If your compiler reports missing member, adapt accordingly.
    self->ContextID = 0; // arbitrary valid context

    cmsUInt32Number nItems = 0;
    void* ret = Type_ProfileSequenceId_Read(self, &fakeIO.io, &nItems, 0);

    TEST_ASSERT(ret != NULL, "Type_ProfileSequenceId_Read returns non-null when Count==0");
    TEST_ASSERT(nItems == 1, "Type_ProfileSequenceId_Read sets nItems to 1 on success");

    // Clean up
    if (ret) {
        cmsFreeProfileSequenceDescription((cmsSEQ*)ret);
    }
    std::free(self);
    return true;
}

// ---------------------------------------------------------------------------
// Test 2: Failure path when Count read fails due to insufficient data
// - IO data stream is empty; _cmsReadUInt32Number should fail to read 4 bytes.
// - Type_ProfileSequenceId_Read should return NULL and nItems remains 0.
// ---------------------------------------------------------------------------

bool test_ProfileSequenceId_Read_CountReadFailure_ReturnsNull() {
    // Empty data: Read of Count will fail
    std::vector<cmsUInt8Number> data = {}; // no data
    FakeIOHolder fakeIO(data);

    struct _cms_typehandler_struct* self = static_cast<struct _cms_typehandler_struct*>(
        std::malloc(sizeof(struct _cms_typehandler_struct))
    );
    if (self == nullptr) {
        std::cerr << "Failed to allocate self in test_ProfileSequenceId_Read_CountReadFailure_ReturnsNull\n";
        return false;
    }
    self->ContextID = 0;

    cmsUInt32Number nItems = 0;
    void* ret = Type_ProfileSequenceId_Read(self, &fakeIO.io, &nItems, 0);

    TEST_ASSERT(ret == NULL, "Type_ProfileSequenceId_Read returns NULL when Count read fails");
    TEST_ASSERT(nItems == 0, "nItems remains 0 when read fails");

    std::free(self);
    return true;
}

// ---------------------------------------------------------------------------
// Main: Run tests
// ---------------------------------------------------------------------------

int main() {
    std::cout << "Running Type_ProfileSequenceId_Read unit tests (cmstypes.c) - C++11 harness\n";

    int passed = 0;
    int total = 0;

    // Test 1
    if (test_ProfileSequenceId_Read_CountZero_Succeeds()) {
        ++passed;
    }
    ++total;

    // Test 2
    if (test_ProfileSequenceId_Read_CountReadFailure_ReturnsNull()) {
        ++passed;
    }
    ++total;

    std::cout << "Tests passed: " << passed << " / " << total << "\n";
    return (passed == total) ? 0 : 1;
}