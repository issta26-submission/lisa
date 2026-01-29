// Unit tests for the focal method Unroll3BytesSwap from cmspack.c
// This test suite is designed for a C++11 environment without Google Test.
// It relies on the project's internal types/macros (e.g., cmsUInt8Number, cmsUInt16Number, cmsUInt32Number, FROM_8_TO_16, CMSREGISTER, _cmsTRANSFORM).
// The tests execute the real Unroll3BytesSwap function (as compiled in cmspack.c) and verify
// the proper behavior: the function reads three input bytes in order B, G, R from accum,
// writes into wIn as [FROM_8_TO_16(R), FROM_8_TO_16(G), FROM_8_TO_16(B)], and advances accum by 3.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Include project headers to obtain all necessary type/macro definitions.
// If your test environment places cmspack.c together with headers, this header should expose
// cmsUInt8Number, cmsUInt16Number, cmsUInt32Number, FROM_8_TO_16, CMSREGISTER, and _cmsTRANSFORM.

// Declare the function prototype for Unroll3BytesSwap so the linker can resolve it from cmspack.c.
// Rely on the actual header's typedefs/macros to be in scope.
extern "C" cmsUInt8Number* Unroll3BytesSwap(CMSREGISTER _cmsTRANSFORM* info,
                                           CMSREGISTER cmsUInt16Number wIn[],
                                           CMSREGISTER cmsUInt8Number* accum,
                                           CMSREGISTER cmsUInt32Number Stride);

// Simple test harness (non-terminating assertions)
static int g_failure_count = 0;

// Generic expectation macro: reports mismatch but does not abort the test.
// Uses value printing suitable for integer types via operator<<.
#define EXPECT_EQ(a, b, msg) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "[FAIL] " << (msg) \
                      << " | Expected: " << (b) << "  Got: " << (a) \
                      << "  (in " << __FUNCTION__ << ")" << std::endl; \
            ++g_failure_count; \
        } \
    } while (0)

#define EXPECT_PTR_EQ(a, b, msg) \
    do { \
        if ((void*)(a) != (void*)(b)) { \
            std::cerr << "[FAIL] " << (msg) \
                      << " | Expected pointer: " << (void*)(b) \
                      << "  Got pointer: " << (void*)(a) \
                      << "  (in " << __FUNCTION__ << ")" << std::endl; \
            ++g_failure_count; \
        } \
    } while (0)

// Helper to run a single test and print a short header.
static void run_test_header(const char* name) {
    std::cout << "Running test: " << name << " ..." << std::endl;
}

// Test 1: Basic swap behavior with known input bytes.
// Verifies that accum advances by 3 and wIn receives [FROM_8_TO_16(R), FROM_8_TO_16(G), FROM_8_TO_16(B)].
// Uses a deterministic B,G,R trio: B=0x11, G=0x22, R=0x33.
static void test_Unroll3BytesSwap_basic_swap() {
    run_test_header("test_Unroll3BytesSwap_basic_swap");

    cmsUInt8Number input[3] = { 0x11, 0x22, 0x33 }; // accum starts here: B, G, R
    cmsUInt8Number* accum = input;
    cmsUInt16Number wIn[3] = { 0, 0, 0 };
    // Pass a null info and zero Stride to exercise the function.
    cmsUInt8Number* ret = Unroll3BytesSwap(nullptr, wIn, accum, 0);

    // Expected ordering: wIn[2] <- FROM_8_TO_16(B=0x11)
    //                    wIn[1] <- FROM_8_TO_16(G=0x22)
    //                    wIn[0] <- FROM_8_TO_16(R=0x33)
    // After the call, accum should point past the 3 consumed bytes.
    cmsUInt16Number expected2 = FROM_8_TO_16(input[0]); // B -> wIn[2]
    cmsUInt16Number expected1 = FROM_8_TO_16(input[1]); // G -> wIn[1]
    cmsUInt16Number expected0 = FROM_8_TO_16(input[2]); // R -> wIn[0]

    EXPECT_PTR_EQ(ret, input + 3, "accum should be advanced by 3 bytes after Unroll3BytesSwap");
    EXPECT_EQ(wIn[0], expected0, "wIn[0] should receive FROM_8_TO_16(R)");
    EXPECT_EQ(wIn[1], expected1, "wIn[1] should receive FROM_8_TO_16(G)");
    EXPECT_EQ(wIn[2], expected2, "wIn[2] should receive FROM_8_TO_16(B)");
}

// Test 2: Different input bytes to ensure swap logic remains consistent.
// Verifies the reversed mapping: B' and G' and R' are correctly assigned according to the macro.
static void test_Unroll3BytesSwap_variant_bytes() {
    run_test_header("test_Unroll3BytesSwap_variant_bytes");

    cmsUInt8Number input[3] = { 0xAA, 0x55, 0x01 }; // accum: B=0xAA, G=0x55, R=0x01
    cmsUInt8Number* accum = input;
    cmsUInt16Number wIn[3] = { 0, 0, 0 };
    cmsUInt8Number* ret = Unroll3BytesSwap(nullptr, wIn, accum, 0);

    cmsUInt16Number expected2 = FROM_8_TO_16(input[0]); // B -> wIn[2]
    cmsUInt16Number expected1 = FROM_8_TO_16(input[1]); // G -> wIn[1]
    cmsUInt16Number expected0 = FROM_8_TO_16(input[2]); // R -> wIn[0]

    EXPECT_PTR_EQ(ret, input + 3, "accum should be advanced by 3 bytes after Unroll3BytesSwap (variant)");
    EXPECT_EQ(wIn[0], expected0, "wIn[0] should equal FROM_8_TO_16(R=0x01)");
    EXPECT_EQ(wIn[1], expected1, "wIn[1] should equal FROM_8_TO_16(G=0x55)");
    EXPECT_EQ(wIn[2], expected2, "wIn[2] should equal FROM_8_TO_16(B=0xAA)");
}

// Test 3: Non-zero Stride value should have no effect (unused parameter).
static void test_Unroll3BytesSwap_stride_ignored() {
    run_test_header("test_Unroll3BytesSwap_stride_ignored");

    cmsUInt8Number input[3] = { 0x7F, 0x80, 0x01 }; // accum: B=0x7F, G=0x80, R=0x01
    cmsUInt8Number* accum = input;
    cmsUInt16Number wIn[3] = { 0, 0, 0 };
    cmsUInt32Number stride = 42; // arbitrary non-zero stride to ensure it's ignored

    cmsUInt8Number* ret = Unroll3BytesSwap(nullptr, wIn, accum, stride);

    cmsUInt16Number expected2 = FROM_8_TO_16(input[0]); // B
    cmsUInt16Number expected1 = FROM_8_TO_16(input[1]); // G
    cmsUInt16Number expected0 = FROM_8_TO_16(input[2]); // R

    EXPECT_PTR_EQ(ret, input + 3, "accum should be advanced by 3 bytes even when Stride is non-zero");
    EXPECT_EQ(wIn[0], expected0, "wIn[0] should equal FROM_8_TO_16(R)");
    EXPECT_EQ(wIn[1], expected1, "wIn[1] should equal FROM_8_TO_16(G)");
    EXPECT_EQ(wIn[2], expected2, "wIn[2] should equal FROM_8_TO_16(B)");
}

// Run all tests and report summary
static void run_all_tests() {
    test_Unroll3BytesSwap_basic_swap();
    test_Unroll3BytesSwap_variant_bytes();
    test_Unroll3BytesSwap_stride_ignored();

    std::cout << "Test summary: failures = " << g_failure_count
              << " / 3 tests" << std::endl;
}

int main() {
    run_all_tests();
    return (g_failure_count == 0) ? 0 : 1;
}