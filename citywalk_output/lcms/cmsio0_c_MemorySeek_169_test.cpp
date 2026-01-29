/*
Unit test suite for MemorySeek(struct _cms_io_handler*, cmsUInt32Number)
- Focus: verify true/false branches of the offset boundary check.
- Approach: drive MemorySeek using the real data structures defined by lcms2_internal.h
  (FILEMEM and _cms_io_handler) to exercise the exact logic in cmsio0.c.
- No GoogleTest; use a lightweight, non-terminating test harness with simple
  pass/fail reporting suitable for C++11.
- This test assumes cmsio0.c is compiled alongside and linked with this test,
  and that lcms2_internal.h provides the necessary type definitions.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cstdio>


// Import necessary dependencies (real project would provide these)

// Ensure we can link against the C-implemented MemorySeek from cmsio0.c
// Use extern "C" to avoid name mangling across C/C++ boundary.
extern "C" cmsBool MemorySeek(struct _cms_io_handler* iohandler, cmsUInt32Number offset);

///////////////////////////////////////////////////////////////////
// Lightweight test harness (non-terminating assertions)
///////////////////////////////////////////////////////////////////
static int g_total_tests = 0;
static int g_failed_tests = 0;

inline void report_pass(const std::string& testName) {
    std::cout << "[PASSED] " << testName << std::endl;
}

inline void report_fail(const std::string& testName, const std::string& reason) {
    std::cerr << "[FAILED] " << testName << " - " << reason << std::endl;
    ++g_failed_tests;
}

static bool run_and_report(const std::string& testName, bool condition, const std::string& reasonIfFail = "") {
    ++g_total_tests;
    if (!condition) {
        report_fail(testName, reasonIfFail);
        return false;
    } else {
        report_pass(testName);
        return true;
    }
}

///////////////////////////////////////////////////////////////////
// Test helpers using real project types
///////////////////////////////////////////////////////////////////
// We rely on the project's internal definitions provided by lcms2_internal.h.
// FILEMEM and struct _cms_io_handler are defined there and used by MemorySeek.

/*
Test 1: MemorySeek with offset strictly inside the data size
- Given Size > offset, function should return TRUE and advance the Pointer to offset.
*/
static bool test_MemorySeek_inside_size() {
    const std::string testName = "MemorySeek_inside_size";

    // Prepare a fake in-memory data block
    FILEMEM mem;
    mem.Size = 100;      // Total available data
    mem.Pointer = 0;     // Start at 0

    // Prepare IO handler wrapper
    struct _cms_io_handler handler;
    handler.stream = &mem;  // io stream points to FILEMEM
    handler.ContextID = 1;   // arbitrary context id

    // Execute: seek to a valid offset
    cmsUInt32Number offset = 50;
    cmsBool result = MemorySeek(&handler, offset);

    // Validate: should succeed and Pointer updated to offset
    bool ok = (result != FALSE) && (mem.Pointer == offset);
    return run_and_report(testName, ok, "offset within size should succeed and update Pointer");
}

/*
Test 2: MemorySeek with offset equal to size (boundary condition)
- Given Size == offset, function should return TRUE and Pointer becomes Size.
*/
static bool test_MemorySeek_at_boundary() {
    const std::string testName = "MemorySeek_at_boundary";

    FILEMEM mem;
    mem.Size = 100;
    mem.Pointer = 0;

    struct _cms_io_handler handler;
    handler.stream = &mem;
    handler.ContextID = 2;

    cmsUInt32Number offset = 100; // boundary: equal to Size
    cmsBool result = MemorySeek(&handler, offset);

    bool ok = (result != FALSE) && (mem.Pointer == offset);
    return run_and_report(testName, ok, "offset at boundary should succeed and Pointer set to Size");
}

/*
Test 3: MemorySeek with offset beyond size
- Given offset > Size, function should return FALSE and Pointer should remain unchanged.
*/
static bool test_MemorySeek_out_of_bounds() {
    const std::string testName = "MemorySeek_out_of_bounds";

    FILEMEM mem;
    mem.Size = 100;
    mem.Pointer = 0;

    struct _cms_io_handler handler;
    handler.stream = &mem;
    handler.ContextID = 3;

    // Set a non-zero Pointer to ensure it remains unchanged on failure
    mem.Pointer = 42;

    cmsUInt32Number offset = 101; // beyond Size
    cmsBool result = MemorySeek(&handler, offset);

    bool ok = (result == FALSE) && (mem.Pointer == 42);
    return run_and_report(testName, ok, "offset beyond size should fail and not modify Pointer");
}

///////////////////////////////////////////////////////////////////
// Main runner
///////////////////////////////////////////////////////////////////
int main() {
    std::cout << "Starting MemorySeek unit tests (CMS IO) for C++11 environment" << std::endl;

    bool a = test_MemorySeek_inside_size();
    bool b = test_MemorySeek_at_boundary();
    bool c = test_MemorySeek_out_of_bounds();

    int total = g_total_tests;
    int failures = g_failed_tests;

    std::cout << "MemorySeek Tests Completed. Passed: " << (total - failures) << "/" << total
              << ". Failures: " << failures << std::endl;

    // Non-terminating: return non-zero if any failure occurred
    return failures == 0 ? 0 : 1;
}