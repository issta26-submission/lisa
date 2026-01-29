/************************************************************
Test Suite for cmsBool Type_UInt8_Write in cmstypes.c
- Step 1: Program Understanding (brief)
  focal method writes nItems bytes from Ptr using _cmsWriteUInt8Number(io, value).
  Returns TRUE iff all writes succeed; otherwise FALSE (early exit).

- Step 2: Unit Test Generation (high-level)
  Tests cover:
  - nItems = 0: should return TRUE, no writes attempted.
  - nItems = 1: single write should succeed and be reported.
  - nItems > 1: multiple sequential writes should succeed and count matches.
  - Failure path: simulate _cmsWriteUInt8Number failing on a specific call to ensure early return and correct count of attempts.

- Step 3: Test Case Refinement
  Use a lightweight, dependency-mediate test harness with a mock I/O object.
  Use non-terminating assertions to allow full test run even after failures.
  All tests are kept self-contained in a single translation unit.

Note: This test harness relies on a test-local override for the _cmsWriteUInt8Number symbol to simulate success/failure scenarios. It is designed to compile with the focal function prototype and minimal type declarations. In a real project, ensure proper linking to the actual cms library implementations or provide appropriate weak/override mechanisms if required.

************************************************************/

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// ------------------------------------------------------------
// Domain knowledge: Minimal type and function declarations
// to compile in a standalone test harness environment.
// These mirror the signatures used by the focal function in
// cmstypes.c, but are intentionally lightweight.
// ------------------------------------------------------------

typedef int cmsBool;                 // Typically a typedef, using int for test harness
typedef unsigned char cmsUInt8Number;  // 8-bit unsigned
typedef unsigned int  cmsUInt32Number; // 32-bit unsigned

// Forward declaration of the type handler struct used by the focal method.
// The real project defines a richer structure; for unit tests we only need a placeholder.
struct _cms_typehandler_struct { int dummy; };

// Opaque IO handler type alias used by the focal function.
// For unit testing we provide a test-specific Mock IO type, but keep the name
// compatible with the focal function signature.
typedef struct MockIO MockIO;
typedef MockIO cmsIOHANDLER;

// Forward declaration of the focal method under test.
// The real signature is:
// cmsBool Type_UInt8_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems);
extern "C" cmsBool Type_UInt8_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems);

// We provide a test-specific mock implementation of _cmsWriteUInt8Number to simulate
// write success/failure without depending on the actual library's IO layer.
extern "C" int _cmsWriteUInt8Number(cmsIOHANDLER* io, cmsUInt8Number value);

// ------------------------------------------------------------
// Mock IO state used by the test to verify behavior.
// ------------------------------------------------------------
struct MockIO {
    bool ok;                  // if true, _cmsWriteUInt8Number succeeds; else fails
    unsigned int writes;      // number of times _cmsWriteUInt8Number was invoked
    cmsUInt8Number lastValue; // last value attempted to write (for verification)
};

// Global test control for simulating failures on a specific call number.
// If g_fail_on_nth_call == k (1-based), the next call to _cmsWriteUInt8Number
// will fail (return 0) instead of succeeding.
static int g_fail_on_nth_call = -1; // -1 means no forced failure
static const int kFailOnFirst = 1;
static const int kFailOnSecond = 2;

// ------------------------------------------------------------
// Implementations for test-time overrides
// We declare the MockIO as the concrete type for cmsIOHANDLER in tests.
extern "C" {
    // Function signature matches the focal code's expectation.
    cmsBool Type_UInt8_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems);

    // Test-time override to simulate IO write behavior.
    int _cmsWriteUInt8Number(cmsIOHANDLER* io, cmsUInt8Number value);
}

// Simple C-style cast wrapper to handle test pointer types
static MockIO* asMockIO(cmsIOHANDLER* io) {
    return reinterpret_cast<MockIO*>(io);
}

// Implementation of the test-time IO write function.
// This function is intended to mirror the behavior of the real _cmsWriteUInt8Number
// in terms of interface, but uses our MockIO to determine success/failure and
// to record state for assertions.
extern "C" int _cmsWriteUInt8Number(cmsIOHANDLER* io, cmsUInt8Number value) {
    MockIO* m = asMockIO(io);
    if (m == nullptr) return 0;

    // Increment write count for each invocation
    m->writes++;

    // Check if a deliberate failure is requested on this call
    if (g_fail_on_nth_call > 0 && m->writes == static_cast<unsigned>(g_fail_on_nth_call)) {
        return 0; // simulate failure
    }

    // On success, record last value written
    m->lastValue = value;
    // Follow the convention that non-zero means TRUE
    return m->ok ? 1 : 0;
}

// ------------------------------------------------------------
// Lightweight test harness (non-terminating assertions)
static int g_total_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { std::cerr << "EXPECT_TRUE failed: " << (msg) << "\n"; ++g_total_failures; } } while(0)

#define EXPECT_FALSE(cond, msg) \
    do { if((cond)) { std::cerr << "EXPECT_FALSE failed: " << (msg) << "\n"; ++g_total_failures; } } while(0)

#define EXPECT_EQ(expected, actual, msg) \
    do { if((expected) != (actual)) { std::cerr << "EXPECT_EQ failed: " << (msg) \
        << " (expected " << (expected) << ", got " << (actual) << ")\n"; ++g_total_failures; } } while(0)

// Helper to print test summary
static void print_summary(const std::string& name) {
    std::cout << "Test: " << name << " completed. Failures so far: " << g_total_failures << "\n";
}

// ------------------------------------------------------------
// Test Cases
// Each test focuses on a specific scenario to validate Type_UInt8_Write behavior.
// ------------------------------------------------------------

// 1) Zero items: function should return TRUE, and no writes should be attempted.
static void test_UInt8_Write_zero_items() {
    // Prepare mock IO
    MockIO io;
    io.ok = true;
    io.writes = 0;
    io.lastValue = 0;

    // Ptr can be null when nItems == 0
    void* Ptr = nullptr;
    cmsUInt32Number nItems = 0;
    struct _cms_typehandler_struct self; // unused in test

    cmsBool result = Type_UInt8_Write(&self, &io, Ptr, nItems);

    EXPECT_TRUE(result != 0, "Type_UInt8_Write should return TRUE for 0 items");
    EXPECT_EQ(0u, io.writes, "No writes should be performed when nItems == 0");
    print_summary("UInt8_Write_zero_items");
}

// 2) Single item: ensure a single write occurs and returns TRUE when successful.
static void test_UInt8_Write_single_item() {
    MockIO io;
    io.ok = true;
    io.writes = 0;
    io.lastValue = 0;

    cmsUInt8Number data[1] = { static_cast<cmsUInt8Number>(0xAB) };
    void* Ptr = static_cast<void*>(data);
    cmsUInt32Number nItems = 1;
    struct _cms_typehandler_struct self;

    // Ensure no forced failure
    g_fail_on_nth_call = -1;

    cmsBool result = Type_UInt8_Write(&self, &io, Ptr, nItems);

    EXPECT_TRUE(result != 0, "Type_UInt8_Write should return TRUE for a single successful write");
    EXPECT_EQ(1u, io.writes, "Exactly one write should have been attempted");
    EXPECT_EQ(0xAB, io.lastValue, "Last written value should match the input byte");
    print_summary("UInt8_Write_single_item");
}

// 3) Multiple items: verify all writes occur and final write is reported
static void test_UInt8_Write_multiple_items() {
    MockIO io;
    io.ok = true;
    io.writes = 0;
    io.lastValue = 0;

    cmsUInt8Number data[4] = { 1, 2, 3, 4 };
    void* Ptr = static_cast<void*>(data);
    cmsUInt32Number nItems = 4;
    struct _cms_typehandler_struct self;

    g_fail_on_nth_call = -1;

    cmsBool result = Type_UInt8_Write(&self, &io, Ptr, nItems);

    EXPECT_TRUE(result != 0, "Type_UInt8_Write should return TRUE when all writes succeed");
    EXPECT_EQ(4u, io.writes, "Should perform four write attempts for four items");
    EXPECT_EQ(4, static_cast<int>(io.lastValue), "Last written value should be the last input (4)");
    print_summary("UInt8_Write_multiple_items");
}

// 4) Failure path: simulate _cmsWriteUInt8Number failing on second write.
// The function should return FALSE and stop after the second attempt.
static void test_UInt8_Write_failure_on_second() {
    MockIO io;
    io.ok = true;
    io.writes = 0;
    io.lastValue = 0;

    cmsUInt8Number data[3] = { 0xAA, 0xBB, 0xCC };
    void* Ptr = static_cast<void*>(data);
    cmsUInt32Number nItems = 3;
    struct _cms_typehandler_struct self;

    // Force failure on the 2nd write
    g_fail_on_nth_call = kFailOnSecond;

    cmsBool result = Type_UInt8_Write(&self, &io, Ptr, nItems);

    EXPECT_FALSE(result != 0, "Type_UInt8_Write should return FALSE when a write fails");
    EXPECT_EQ(2u, io.writes, "Two write attempts should have been made before failure");
    print_summary("UInt8_Write_failure_on_second");

    // Reset for other tests
    g_fail_on_nth_call = -1;
}

// ------------------------------------------------------------
// Main: Run all tests
int main() {
    test_UInt8_Write_zero_items();
    test_UInt8_Write_single_item();
    test_UInt8_Write_multiple_items();
    test_UInt8_Write_failure_on_second();

    if (g_total_failures == 0) {
        std::cout << "ALL tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Tests finished with " << g_total_failures << " failure(s).\n";
        return 1;
    }
}

// Note: In a real-world scenario, you would link against the actual cms library
// providing Type_UInt8_Write and _cmsWriteUInt8Number. The test harness here
// overrides _cmsWriteUInt8Number via a test-time symbol to simulate success
// and controlled failures, enabling coverage of both true and false branches
// without requiring file IO or external dependencies.