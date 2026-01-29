// MemoryRead Unit Test Suite for cmsio0.c
// Target: cmsUInt32Number MemoryRead(struct _cms_io_handler* iohandler, void *Buffer, cmsUInt32Number size, cmsUInt32Number count)
// Description: This test harness exercises MemoryRead by injecting a controlled FILEMEM-backed memory stream
// via the cms_io_handler interface. It validates correct behavior for in-bounds reads, out-of-bounds reads,
// boundary reads, and reads when some data remains from a prior operation.
// Note: This test relies on the library's internal type definitions (e.g., _cms_io_handler, FILEMEM, cmsUInt32Number, cmsUInt8Number).
// It uses the project-provided MemoryRead function declared in the library, and does not rely on GoogleTest.
// Compile with the project's existing build environment, linking against the library/object file that contains cmsio0.c.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdio>
#include <cstring>


// Import internal CMS/LittleCMS definitions to align with the focal method's expectations.
// The header path may differ depending on the project layout; adjust if necessary.
extern "C" {
}

// Focal method declaration (from cmsio0.c). We assume the library provides this symbol.
extern "C" cmsUInt32Number MemoryRead(struct _cms_io_handler* iohandler, void *Buffer, cmsUInt32Number size, cmsUInt32Number count);

// Test helpers: simple non-terminating assertions to maximize execution coverage.
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_PASS(msg) do { std::cout << "[PASS] " << msg << std::endl; } while (0)
#define TEST_FAIL(msg) do { std::cerr << "[FAIL] " << msg << std::endl; g_tests_failed++; } while (0)

#define ASSERT_TRUE(cond, msg) do { \
    g_tests_run++; \
    if (cond) TEST_PASS(msg); \
    else { TEST_FAIL(msg); } \
} while (0)

#define ASSERT_EQ(a, b, msg) do { \
    g_tests_run++; \
    if ((a) == (b)) TEST_PASS(msg); \
    else { \
        std::cerr << "[FAIL] " << msg << " (expected: " << (b) << ", actual: " << (a) << ")" << std::endl; \
        g_tests_failed++; \
    } \
} while (0)


// Helpers to set up the memory-based FILEMEM stream
static void setup_stream(FILEMEM& mem, const cmsUInt8Number* data, cmsUInt32Number size)
{
    mem.Block = (cmsUInt8Number*) data;
    mem.Size  = size;
    mem.Pointer = 0;
}

static void reset_stream_pointer(FILEMEM& mem, cmsUInt32Number ptr)
{
    mem.Pointer = ptr;
}


// Test 1: Reading within bounds should copy exact data and advance the pointer.
static bool test_memory_read_within_bounds()
{
    std::cout << "Running test_memory_read_within_bounds..." << std::endl;

    // Prepare in-memory data: "ABCD"
    const cmsUInt8Number srcData[4] = { 'A', 'B', 'C', 'D' };

    FILEMEM mem;
    setup_stream(mem, srcData, 4);

    _cms_io_handler handler;
    handler.stream = &mem;
    handler.ContextID = 0;

    cmsUInt8Number dest[4] = { 0x00, 0x00, 0x00, 0x00 };

    cmsUInt32Number ret = MemoryRead(&handler, dest, 1, 4);

    // Assertions
    ASSERT_EQ(ret, 4, "MemoryRead should return count on successful read");
    ASSERT_EQ(mem.Pointer, 4, "Pointer should be advanced by len (4 bytes)");
    ASSERT_TRUE(std::memcmp(dest, srcData, 4) == 0, "Destination buffer should contain the copied data");

    return (g_tests_failed == 0);
}


// Test 2: Reading beyond available data should not modify buffer or pointer, and should return 0.
static bool test_memory_read_out_of_bounds()
{
    std::cout << "Running test_memory_read_out_of_bounds..." << std::endl;

    // Prepare in-memory data: "WXYZ" (4 bytes)
    const cmsUInt8Number srcData[4] = { 'W', 'X', 'Y', 'Z' };

    FILEMEM mem;
    setup_stream(mem, srcData, 4);

    _cms_io_handler handler;
    handler.stream = &mem;
    handler.ContextID = 0;

    cmsUInt8Number dest[4] = { 'K', 'L', 'M', 'N' }; // pre-fill to verify no modification on error path

    // Request more bytes than available: size=2, count=3 => 6 bytes requested, only 4 bytes available
    cmsUInt32Number ret = MemoryRead(&handler, dest, 2, 3);

    // Assertions
    ASSERT_TRUE(ret == 0, "MemoryRead should return 0 when attempting to read beyond the buffer");
    // Buffer should remain unchanged
    ASSERT_TRUE(std::memcmp(dest, (const void[]){'K','L','M','N'}, 4) == 0, "Destination buffer should remain unchanged after failed read");
    // Pointer should remain at 0
    ASSERT_EQ(mem.Pointer, 0, "Pointer should not be advanced on failed read");

    return (g_tests_failed == 0);
}


// Test 3: Reading exactly up to the end boundary should succeed and update the pointer to end.
static bool test_memory_read_exact_boundary()
{
    std::cout << "Running test_memory_read_exact_boundary..." << std::endl;

    // Prepare in-memory data: "DATA"
    const cmsUInt8Number srcData[4] = { 'D', 'A', 'T', 'A' };

    FILEMEM mem;
    setup_stream(mem, srcData, 4);

    _cms_io_handler handler;
    handler.stream = &mem;
    handler.ContextID = 0;

    cmsUInt8Number dest[4];
    std::memset(dest, 0, 4);

    cmsUInt32Number ret = MemoryRead(&handler, dest, 1, 4);

    // Assertions
    ASSERT_EQ(ret, 4, "MemoryRead should return count when reading exactly the available data");
    ASSERT_EQ(mem.Pointer, 4, "Pointer should be advanced to the end after exact read");
    ASSERT_TRUE(std::memcmp(dest, srcData, 4) == 0, "Destination buffer should contain all bytes copied");

    return (g_tests_failed == 0);
}


// Test 4: Reading when there is a remaining segment (partial left) should fail gracefully and not modify state beyond pointers/dest.
static bool test_memory_read_partial_left()
{
    std::cout << "Running test_memory_read_partial_left..." << std::endl;

    // Prepare in-memory data: "012345"
    const cmsUInt8Number srcData[6] = { '0','1','2','3','4','5' };

    FILEMEM mem;
    setup_stream(mem, srcData, 6);

    // Simulate prior usage: set Pointer to 4 (two bytes remain)
    mem.Pointer = 4;

    _cms_io_handler handler;
    handler.stream = &mem;
    handler.ContextID = 0;

    cmsUInt8Number dest[2] = { 0xAA, 0xBB };

    // Request 2 bytes of data; there are only 2 bytes left, but the code path should detect
    // overflow when len would exceed Size and adjust len accordingly, then return 0 and not advance
    // Note: With Pointer=4 and size=1, count=2, len=2; Pointer+len > Size (4+2 > 6) is false here, so adjust size to trigger edge:
    // To trigger the error path reliably, request more than the remaining bytes: size=3, count=2 => len=6; 4+6 > 6 -> len becomes 2
    cmsUInt32Number ret = MemoryRead(&handler, dest, 3, 2);

    // Assertions
    ASSERT_TRUE(ret == 0, "MemoryRead should return 0 when read would overflow the available data");
    ASSERT_EQ(mem.Pointer, 4, "Pointer should remain unchanged on failed read");
    // Destination buffer should remain unchanged
    ASSERT_TRUE(std::memcmp(dest, (const void[]){0xAA, 0xBB}, 2) == 0, "Destination buffer should remain unchanged after failed read");

    return (g_tests_failed == 0);
}


// Main: run all tests and report summary
int main()
{
    std::cout << "MemoryRead unit test suite for cmsio0.c (C++11)\n";
    std::cout << "-----------------------------------------------\n";

    bool ok;

    ok = test_memory_read_within_bounds();
    if (!ok) TEST_FAIL("test_memory_read_within_bounds reported failures");

    ok = test_memory_read_out_of_bounds();
    if (!ok) TEST_FAIL("test_memory_read_out_of_bounds reported failures");

    ok = test_memory_read_exact_boundary();
    if (!ok) TEST_FAIL("test_memory_read_exact_boundary reported failures");

    ok = test_memory_read_partial_left();
    if (!ok) TEST_FAIL("test_memory_read_partial_left reported failures");

    // Summary
    std::cout << "-----------------------------------------------\n";
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;

    // Exit code indicates success (0) if all tests passed; otherwise non-zero.
    return (g_tests_failed == 0) ? 0 : 1;
}