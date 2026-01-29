// Lightweight C++11 unit tests for the focal function:
// cmsBool _cmsWriteHeader(_cmsICCPROFILE* Icc, cmsUInt32Number UsedSpace)
// No GTest; a small test harness is implemented here. The tests focus on
// validating the true/false branches of the predicate logic inside _cmsWriteHeader.
// The tests rely on the Little CMS internal structures exposed via lcms2_internal.h.

#include <vector>
#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Include internal headers to access internal types used by _cmsWriteHeader

// Declaration of the focal function under test.
// It is assumed to be accessible from tests (non-static in this build).
extern "C" cmsBool _cmsWriteHeader(_cmsICCPROFILE* Icc, cmsUInt32Number UsedSpace);

// Forward-declare test helper structures and mocks
struct MockIOHandler;

// A mock IO handler structure that mimics cmsIOHANDLER used by the library.
// The first member is cmsIOHANDLER base so that it can be cast from the base pointer.
struct MockIOHandler {
    cmsIOHANDLER base;
    int callCount;              // How many times the Write function has been invoked
    int failOnCall;             // If > 0, fail on this specific call (1-based)
    std::vector<std::vector<unsigned char>> writes; // Captured data per write
};

// The actual Write function used by _cmsWriteHeader. It records writes and can simulate failures.
static cmsBool MockWrite(cmsIOHANDLER* ioh, cmsUInt32Number size, const void* Ptr) {
    // Interpret the ioh pointer as part of MockIOHandler's layout
    MockIOHandler* self = (MockIOHandler*)ioh; // base is the first member

    // Increment call counter
    self->callCount++;

    // Simulate failure on a specific call if configured
    if (self->failOnCall > 0 && self->callCount == self->failOnCall) {
        return FALSE;
    }

    // Capture the written data for inspection (copy 'size' bytes from Ptr)
    const unsigned char* data = static_cast<const unsigned char*>(Ptr);
    self->writes.emplace_back(data, data + size);

    return TRUE;
}

// Helper macro to run tests and print results
#define RUNTEST(name) test_##name()

// Test 1: Write header with zero tags (Count = 0). Expect success and exactly 2 writes:
// - one for the header
// - one for the 32-bit Count (0)
bool test_WriteHeader_NoTags() {
    // Setup mock IO handler
    MockIOHandler mock;
    mock.callCount = 0;
    mock.failOnCall = -1; // never fail
    mock.writes.clear();
    mock.base.Write = MockWrite;
    // I/O handler's contract expects a cmsIOHANDLER*; provide address of mock.base
    _cmsICCPROFILE* Icc = new _cmsICCPROFILE;
    std::memset(Icc, 0, sizeof(_cmsICCPROFILE));
    Icc->IOhandler = &mock.base;

    // No tags
    Icc->TagCount = 0;
    Icc->TagNames = nullptr;
    Icc->TagOffsets = nullptr;
    Icc->TagSizes = nullptr;
    // Minimal required fields (values not used for control flow in this test)
    Icc->CMM = 0x01;
    Icc->Version = 0x04000000;
    Icc->DeviceClass = (cmsProfileClassSignature)0;
    Icc->ColorSpace  = (cmsColorSpaceSignature)0;
    Icc->PCS         = (cmsColorSpaceSignature)0;
    Icc->Created = 0;
    Icc->platform    = 0;
    Icc->flags       = 0;
    Icc->manufacturer = 0;
    Icc->model        = 0;
    Icc->attributes   = 0;
    Icc->RenderingIntent = 0;
    Icc->creator      = 0;
    std::memset(Icc->ProfileID, 0, 16);

    // Call focal function
    cmsBool res = _cmsWriteHeader(Icc, 1024);

    // Cleanup
    delete Icc;

    // We expect success and two writes (Header + Count)
    bool ok = res ? (mock.callCount >= 2 && mock.writes.size() >= 2) : false;
    if (!ok) {
        std::cout << "FAILED: test_WriteHeader_NoTags\n";
    }
    return ok;
}

// Test 2: Write header with one valid tag (Count = 1). Expect 3 writes:
// Header, Count (1), TagEntry
bool test_WriteHeader_WithOneTag() {
    MockIOHandler mock;
    mock.callCount = 0;
    mock.failOnCall = -1;
    mock.writes.clear();
    mock.base.Write = MockWrite;

    _cmsICCPROFILE* Icc = new _cmsICCPROFILE;
    std::memset(Icc, 0, sizeof(_cmsICCPROFILE));
    Icc->IOhandler = &mock.base;

    // One valid tag
    Icc->TagCount = 1;
    Icc->TagNames = new cmsTagSignature[1];
    Icc->TagOffsets = new cmsUInt32Number[1];
    Icc->TagSizes = new cmsUInt32Number[1];

    Icc->TagNames[0] = (cmsTagSignature)0x1234;
    Icc->TagOffsets[0] = 0x10;
    Icc->TagSizes[0] = 0x20;

    Icc->CMM = 0x01;
    Icc->Version = 0x04000000;
    Icc->DeviceClass = (cmsProfileClassSignature)0;
    Icc->ColorSpace  = (cmsColorSpaceSignature)0;
    Icc->PCS         = (cmsColorSpaceSignature)0;
    Icc->Created = 0;
    Icc->platform    = 0;
    Icc->flags       = 0;
    Icc->manufacturer = 0;
    Icc->model        = 0;
    Icc->attributes   = 0;
    Icc->RenderingIntent = 0;
    Icc->creator      = 0;
    std::memset(Icc->ProfileID, 0, 16);

    cmsBool res = _cmsWriteHeader(Icc, 2048);

    // Cleanup
    delete[] Icc->TagNames;
    delete[] Icc->TagOffsets;
    delete[] Icc->TagSizes;
    delete Icc;

    bool ok = res ? (mock.callCount >= 3 && mock.writes.size() >= 3) : false;
    if (!ok) {
        std::cout << "FAILED: test_WriteHeader_WithOneTag\n";
    }
    return ok;
}

// Test 3: Header write fails on first write (simulate IO error on header write)
bool test_WriteHeader_HeaderWriteFails() {
    MockIOHandler mock;
    mock.callCount = 0;
    mock.failOnCall = 1; // fail on first write (header)
    mock.writes.clear();
    mock.base.Write = MockWrite;

    _cmsICCPROFILE* Icc = new _cmsICCPROFILE;
    std::memset(Icc, 0, sizeof(_cmsICCPROFILE));
    Icc->IOhandler = &mock.base;

    Icc->TagCount = 0;
    Icc->TagNames = nullptr;
    Icc->TagOffsets = nullptr;
    Icc->TagSizes = nullptr;

    Icc->CMM = 0x01;
    Icc->Version = 0x04000000;
    Icc->DeviceClass = (cmsProfileClassSignature)0;
    Icc->ColorSpace  = (cmsColorSpaceSignature)0;
    Icc->PCS         = (cmsColorSpaceSignature)0;
    Icc->Created = 0;
    Icc->platform    = 0;
    Icc->flags       = 0;
    Icc->manufacturer = 0;
    Icc->model        = 0;
    std::memset(Icc->ProfileID, 0, 16);

    cmsBool res = _cmsWriteHeader(Icc, 1024);

    delete Icc;

    if (res) {
        std::cout << "FAILED: test_WriteHeader_HeaderWriteFails (expected FALSE)\n";
        return false;
    }
    return true;
}

// Test 4: Count write fails (second write in sequence)
bool test_WriteHeader_CountWriteFails() {
    MockIOHandler mock;
    mock.callCount = 0;
    mock.failOnCall = 2; // fail on second write (Count)
    mock.writes.clear();
    mock.base.Write = MockWrite;

    _cmsICCPROFILE* Icc = new _cmsICCPROFILE;
    std::memset(Icc, 0, sizeof(_cmsICCPROFILE));
    Icc->IOhandler = &mock.base;

    // One valid tag (so Count = 1)
    Icc->TagCount = 1;
    Icc->TagNames = new cmsTagSignature[1];
    Icc->TagOffsets = new cmsUInt32Number[1];
    Icc->TagSizes = new cmsUInt32Number[1];

    Icc->TagNames[0] = (cmsTagSignature)0x1234;
    Icc->TagOffsets[0] = 0x10;
    Icc->TagSizes[0] = 0x20;

    Icc->CMM = 0x01;
    Icc->Version = 0x04000000;
    Icc->DeviceClass = (cmsProfileClassSignature)0;
    Icc->ColorSpace  = (cmsColorSpaceSignature)0;
    Icc->PCS         = (cmsColorSpaceSignature)0;
    Icc->Created = 0;
    Icc->platform    = 0;
    Icc->flags       = 0;
    Icc->manufacturer = 0;
    Icc->model        = 0;
    std::memset(Icc->ProfileID, 0, 16);

    cmsBool res = _cmsWriteHeader(Icc, 1024);

    delete[] Icc->TagNames;
    delete[] Icc->TagOffsets;
    delete[] Icc->TagSizes;
    delete Icc;

    if (res) {
        std::cout << "FAILED: test_WriteHeader_CountWriteFails (expected FALSE)\n";
        return false;
    }
    return true;
}

// Test 5: TagEntry write fails (third write)
bool test_WriteHeader_TagEntryWriteFails() {
    MockIOHandler mock;
    mock.callCount = 0;
    mock.failOnCall = 3; // fail on third write (TagEntry)
    mock.writes.clear();
    mock.base.Write = MockWrite;

    _cmsICCPROFILE* Icc = new _cmsICCPROFILE;
    std::memset(Icc, 0, sizeof(_cmsICCPROFILE));
    Icc->IOhandler = &mock.base;

    // One valid tag
    Icc->TagCount = 1;
    Icc->TagNames = new cmsTagSignature[1];
    Icc->TagOffsets = new cmsUInt32Number[1];
    Icc->TagSizes = new cmsUInt32Number[1];

    Icc->TagNames[0] = (cmsTagSignature)0x1234;
    Icc->TagOffsets[0] = 0x10;
    Icc->TagSizes[0] = 0x20;

    Icc->CMM = 0x01;
    Icc->Version = 0x04000000;
    Icc->DeviceClass = (cmsProfileClassSignature)0;
    Icc->ColorSpace  = (cmsColorSpaceSignature)0;
    Icc->PCS         = (cmsColorSpaceSignature)0;
    Icc->Created = 0;
    Icc->platform    = 0;
    Icc->flags       = 0;
    Icc->manufacturer = 0;
    Icc->model        = 0;
    std::memset(Icc->ProfileID, 0, 16);

    cmsBool res = _cmsWriteHeader(Icc, 1024);

    delete[] Icc->TagNames;
    delete[] Icc->TagOffsets;
    delete[] Icc->TagSizes;
    delete Icc;

    if (res) {
        std::cout << "FAILED: test_WriteHeader_TagEntryWriteFails (expected FALSE)\n";
        return false;
    }
    return true;
}

int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Starting cmsWriteHeader unit tests...\n";

    // Run tests and accumulate results
    total++; if (RUNTEST(WriteHeader_NoTags)) passed++;
    total++; if (RUNTEST(WriteHeader_WithOneTag)) passed++;
    total++; if (RUNTEST(WriteHeader_HeaderWriteFails)) passed++;
    total++; if (RUNTEST(WriteHeader_CountWriteFails)) passed++;
    total++; if (RUNTEST(WriteHeader_TagEntryWriteFails)) passed++;

    std::cout << "Tests passed: " << passed << " / " << total << "\n";
    if (passed == total) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED\n";
        return 1;
    }
}