// Unit test suite for cmsBool WriteOneElem(cmsIOHANDLER* io, _cmsDICelem* e, cmsUInt32Number i)
// Target: Validate the behavior of WriteOneElem in both success and failure paths.
// This test suite does not rely on Google Test. It uses a minimal, self-contained test harness
// and a C-like fake cmsIOHANDLER Write callback to simulate success/failure of underlying IO.

// Note: This test assumes the cms2 internal headers are available and that _cmsDICelem
// contains members Offsets and Sizes that are indexable as Offsets[i] and Sizes[i].
// The tests use only the public/c-compatible interface provided by the focal method.

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// Include the CMS internal header that provides the declarations used by the focal method.
// This is needed to compile WriteOneElem and related types in a real cms environment.

// Global test result counter
static int g_failures = 0;

// Lightweight test assertion helpers (non-terminating)
static void expect_true(bool cond, const char* msg)
{
    if (!cond) {
        std::cerr << "EXPECT_TRUE failed: " << (msg ? msg : "") << std::endl;
        ++g_failures;
    }
}

static void expect_equal_uint32(uint32_t a, uint32_t b, const char* msg)
{
    if (a != b) {
        std::cerr << "EXPECT_EQUAL_UINT32 failed: " << (msg ? msg : "")
                  << " (" << a << " != " << b << ")" << std::endl;
        ++g_failures;
    }
}

// Fake IO user data for capturing writes
struct FakeIOUserData {
    std::vector<uint8_t> written;
    int extra; // for potential stateful tests
};

//  Test 1: Success path - WriteOneElem should return TRUE and write 8 bytes (Offsets[i] and Sizes[i])
static cmsBool fake_write_always_true(struct cmsIOHANDLER* io, const cmsUInt8Number* Buffer, cmsUInt32Number Count)
{
    if (io == nullptr) return FALSE;
    FakeIOUserData* ud = static_cast<FakeIOUserData*>(io->UserData);
    if (ud && Buffer && Count > 0) {
        ud->written.insert(ud->written.end(), Buffer, Buffer + Count);
    }
    return TRUE;
}

//  Test 2: Failure on first write - _cmsWriteUInt32Number should trigger FALSE immediately
static cmsBool fake_write_always_false(struct cmsIOHANDLER* io, const cmsUInt8Number* Buffer, cmsUInt32Number Count)
{
    (void)io; (void)Buffer; (void)Count;
    return FALSE;
}

//  Test 3: Failure on second write - first write succeeds, second write fails
struct ToggleIOUserData {
    int state;
};

static cmsBool fake_write_toggle(struct cmsIOHANDLER* io, const cmsUInt8Number* Buffer, cmsUInt32Number Count)
{
    ToggleIOUserData* ud = static_cast<ToggleIOUserData*>(io->UserData);
    if (ud == nullptr) return FALSE;
    if (ud->state == 0) {
        ud->state = 1;
        (void)Buffer; (void)Count; // ignore content
        return TRUE; // first write succeeds
    } else {
        return FALSE; // second write fails
    }
}

// Test runner helpers
static void RunTest_SuccessPath()
{
    // Prepare IO with success callback and capture buffer
    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));

    FakeIOUserData* ud = new FakeIOUserData();
    ud->written.clear();
    ud->extra = 0;
    io.Write = fake_write_always_true;
    io.UserData = ud;

    // Prepare element with one entry
    _cmsDICelem* e = new _cmsDICelem();
    // Initialize arrays for at least index 0
    // The actual structure in lcms2 defines Offsets and Sizes; we assume they are indexable.
    e->Offsets[0] = 1234u;
    e->Sizes[0] = 5678u;

    cmsUInt32Number idx = 0;
    cmsBool res = WriteOneElem(&io, e, idx);

    // Validate
    expect_true(res == TRUE, "WriteOneElem should return TRUE on successful writes");
    // We expect exactly two 4-byte writes = 8 bytes recorded
    expect_true(ud->written.size() == 8u, "WriteOneElem should cause exactly 8 bytes to be written for one element (Offsets and Sizes)");

    // Validate the exact bytes (little-endian)
    // Offsets[0] = 1234 -> 0x000004D2 -> bytes D2 04 00 00
    // Sizes[0]   = 5678 -> 0x0000162E -> bytes 2E 16 00 00
    const uint8_t expected1[4] = { 0xD2, 0x04, 0x00, 0x00 };
    const uint8_t expected2[4] = { 0x2E, 0x16, 0x00, 0x00 };
    const uint8_t* buf = ud->written.data();

    expect_true(std::memcmp(buf, expected1, 4) == 0, "First 4 bytes should be little-endian of Offsets[0] (1234)");
    expect_true(std::memcmp(buf + 4, expected2, 4) == 0, "Next 4 bytes should be little-endian of Sizes[0] (5678)");

    delete e;
    delete ud;
}

static void RunTest_FailureFirstWrite()
{
    // Prepare IO where the first write fails
    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));

    io.Write = fake_write_always_false;
    io.UserData = nullptr; // not used in this test

    _cmsDICelem* e = new _cmsDICelem();
    e->Offsets[0] = 1;
    e->Sizes[0] = 2;

    cmsUInt32Number idx = 0;
    cmsBool res = WriteOneElem(&io, e, idx);

    expect_true(res == FALSE, "WriteOneElem should return FALSE when the first underlying write fails");

    delete e;
}

static void RunTest_FailureSecondWrite()
{
    // Prepare IO where the second write fails (first succeeds)
    cmsIOHANDLER io;
    std::memset(&io, 0, sizeof(io));

    ToggleIOUserData* ud = new ToggleIOUserData();
    ud->state = 0;
    io.Write = fake_write_toggle;
    io.UserData = ud;

    _cmsDICelem* e = new _cmsDICelem();
    e->Offsets[0] = 0x11223344u;
    e->Sizes[0] = 0x55667788u;

    cmsUInt32Number idx = 0;
    cmsBool res = WriteOneElem(&io, e, idx);

    expect_true(res == FALSE, "WriteOneElem should return FALSE when the second underlying write fails");

    delete e;
    delete ud;
}

// Entry point
int main()
{
    RunTest_SuccessPath();
    RunTest_FailureFirstWrite();
    RunTest_FailureSecondWrite();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}