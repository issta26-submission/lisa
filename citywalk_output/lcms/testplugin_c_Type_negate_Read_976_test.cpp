/*
  High-quality unit tests for the focal method Type_negate_Read
  - No GTest; a lightweight, self-contained test runner is provided.
  - Tests exercise true/false branches of the read path as per the focal method.
  - Uses manual stubs for I/O read function to control inputs.
  - Assumes testplugin.c (the focal file) is part of the build and links with this test.
  - Comments describe intent and coverage for each test case.
*/

#include <cstddef>
#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Domain knowledge: minimal type definitions to match the focal function's usage.
// These definitions intentionally stay lightweight to avoid pulling in the full library.
typedef void* cmsContext;
typedef uint32_t cmsUInt32Number;
typedef uint16_t cmsUInt16Number;
typedef uint8_t  cmsUInt8Number;

// Forward declaration of the focal type struct used by Type_negate_Read.
// We assume the real library defines ContextID as the first member.
struct _cms_typehandler_struct {
    cmsContext ContextID;
};

// Mock/bridge for the IO handler type used by Type_negate_Read.
// We provide a compact, test-friendly layout for simulating input bytes.
struct cmsIOHANDLER_s {
    unsigned char* data;
    size_t size;
    size_t pos;
};
typedef struct cmsIOHANDLER_s cmsIOHANDLER;

// Forward declaration of the focal function under test.
// Declared with C linkage to match actual implementation in testplugin.c.
extern "C" void *Type_negate_Read(struct _cms_typehandler_struct* self,
                                cmsIOHANDLER* io,
                                cmsUInt32Number* nItems,
                                cmsUInt32Number SizeOfTag);

// Another C linkage bridge for the internal helper used by the focal method.
// We implement this in the test harness to control inputs.
extern "C" int _cmsReadUInt16Number(cmsIOHANDLER* io, cmsUInt16Number* value);

// We'll rely on the real StageAllocNegate from the library. If the library provides it,
// it will be linked in. We do not redefine it to avoid symbol conflicts.

// Lightweight, non-terminating test assertions.
// We'll accumulate failures and print a summary at the end.
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, desc) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        std::cerr << "[EXPECT_FALSE] " << (desc) << " : Condition failed: " #cond << std::endl; \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr, desc) do { \
    ++g_total_tests; \
    if ((ptr) == nullptr) { \
        std::cerr << "[EXPECT_NULL] " << (desc) << " : Received NULL pointer." << std::endl; \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_EQ_INT(a, b, desc) do { \
    ++g_total_tests; \
    if ((a) != (b)) { \
        std::cerr << "[EXPECT_EQ] " << (desc) << " : Expected " << (b) << ", got " << (a) << std::endl; \
        ++g_failed_tests; \
    } \
} while(0)

// Implement a minimal, controlled _cmsReadUInt16Number to feed tests.
// Reads a 16-bit little-endian value from the provided io buffer.
extern "C" int _cmsReadUInt16Number(cmsIOHANDLER* io, cmsUInt16Number* value)
{
    if (io == nullptr || value == nullptr) return 0;
    if (io->pos + 2 > io->size) return 0;
    // Little-endian interpretation (low byte first)
    unsigned int lo = static_cast<unsigned int>(io->data[io->pos]);
    unsigned int hi = static_cast<unsigned int>(io->data[io->pos + 1]);
    *value = static_cast<cmsUInt16Number>((lo) | (hi << 8));
    io->pos += 2;
    return 1;
}

// Test 1: Reading fails (simulate _cmsReadUInt16Number failure) -> Type_negate_Read should return NULL.
void test_Type_negate_Read_read_failure()
{
    // Prepare a dummy self object (ContextID can be any pointer; not used due to early return)
    _cms_typehandler_struct self;
    self.ContextID = reinterpret_cast<cmsContext>(0xA1A1);

    // IO with no data to force read failure
    unsigned char dummy[0] = {};
    cmsIOHANDLER io;
    io.data = dummy;
    io.size = 0;
    io.pos = 0;

    cmsUInt32Number nItems = 0;
    void* res = Type_negate_Read(&self, &io, &nItems, 0);

    EXPECT_TRUE(res == nullptr, "Type_negate_Read should return NULL when _cmsReadUInt16Number fails");
}

// Test 2: Chans != 3 -> Should return NULL (simulate reading 2); not reaching StageAllocNegate.
void test_Type_negate_Read_wrong_channels()
{
    _cms_typehandler_struct self;
    self.ContextID = reinterpret_cast<cmsContext>(0xB2B2);

    // IO data encodes Chans = 2 (0x02, 0x00)
    unsigned char buf[2] = { 0x02, 0x00 };
    cmsIOHANDLER io;
    io.data = buf;
    io.size = 2;
    io.pos = 0;

    cmsUInt32Number nItems = 0;
    void* res = Type_negate_Read(&self, &io, &nItems, 0);

    EXPECT_TRUE(res == nullptr, "Type_negate_Read should return NULL when Chans != 3");
}

// Test 3: Chans == 3 (success path). Verify that a non-NULL pointer is returned
// and that nItems is set to 1 as per function's contract.
void test_Type_negate_Read_success_paths()
{
    _cms_typehandler_struct self;
    self.ContextID = reinterpret_cast<cmsContext>(0xC3C3);

    // IO data encodes Chans = 3 (0x03, 0x00)
    unsigned char buf[2] = { 0x03, 0x00 };
    cmsIOHANDLER io;
    io.data = buf;
    io.size = 2;
    io.pos = 0;

    cmsUInt32Number nItems = 0;
    void* res = Type_negate_Read(&self, &io, &nItems, 0);

    EXPECT_NOT_NULL(res, "Type_negate_Read should return a non-NULL pointer when Chans == 3");
    EXPECT_EQ_INT(nItems, 1, "Type_negate_Read should set nItems = 1 on success");
}

// Helper to run all tests and print a summary
void run_all_tests()
{
    std::cout << "Starting Type_negate_Read unit tests (C++ harness)..." << std::endl;

    test_Type_negate_Read_read_failure();
    test_Type_negate_Read_wrong_channels();
    test_Type_negate_Read_success_paths();

    std::cout << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;
    if (g_failed_tests != 0) {
        std::cerr << "Some tests failed. See above messages for details." << std::endl;
    } else {
        std::cout << "All tests passed." << std::endl;
    }
}

// Entry point for the test binary.
// We deliberately avoid terminating assertions during test execution; instead, we summarize at the end.
int main()
{
    run_all_tests();
    return (g_failed_tests == 0) ? 0 : 1;
}