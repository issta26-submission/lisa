/*
   Unit test suite for Sampler8D function (Step 2: test generation guided by FOCAL_METHOD and dependencies).

   This test suite is written for C++11 (no GoogleTest). It relies on the C test harness
   provided by testcms2.h (as indicated by the project structure). We test:
   - Correct output mapping: Out[0] == Fn8D1(..., 8), Out[1] == Fn8D2(..., 8), Out[2] == Fn8D3(..., 8)
   - Return value is 1
   - Cargo parameter is ignored (implicit in behavior)

   The tests are designed to cover multiple input scenarios (regular, boundary, zero, random)
   to maximize coverage of the focal method's data flow. Non-terminating assertions are used:
   failures are logged but do not abort subsequent tests, so all test cases execute.

   Important: This file is intended to be compiled and linked with the C sources that define
   Fn8D1, Fn8D2, Fn8D3 and Sampler8D (e.g., testcms2.c/testcms2.h). The test relies on
   the types cmsUInt16Number, cmsInt32Number, and the function signatures provided by those headers.
*/

#include <cstdint>
#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Bring in the C declarations. Use extern "C" to avoid name mangling when used from C++.
extern "C" {
}

// Lightweight test logger with non-terminating assertion style
struct Tester {
    int failures;
    Tester() : failures(0) {}

    // Generic non-terminating assertion with printable output
    template <typename T, typename U>
    void expectEq(const char* label, const T& got, const U& expected) {
        if (static_cast<uint64_t>(got) != static_cast<uint64_t>(expected)) {
            ++failures;
            std::cout << "[FAIL] " << label
                      << " - got: " << +got << ", expected: " << +expected << "\n";
        } else {
            std::cout << "[OK]   " << label << "\n";
        }
    }

    // Overload for cmsInt32Number types (often int)
    void expectEq(const char* label, cmsInt32Number got, cmsInt32Number expected) {
        if (got != expected) {
            ++failures;
            std::cout << "[FAIL] " << label
                      << " - got: " << got << ", expected: " << expected << "\n";
        } else {
            std::cout << "[OK]   " << label << "\n";
        }
    }

    void summary() const {
        if (failures > 0) {
            std::cout << "Summary: " << failures << " test(s) FAILED.\n";
        } else {
            std::cout << "Summary: All tests PASSED.\n";
        }
    }
};

// Test 1: Basic deterministic case
void test_Sampler8D_Basic(Tester& log) {
    // Arrange: a simple, small input set
    cmsUInt16Number In[8]  = {1, 2, 3, 4, 5, 6, 7, 8};
    cmsUInt16Number Out[3]   = {0, 0, 0};
    void* Cargo = nullptr;

    // Act
    cmsInt32Number ret = Sampler8D(In, Out, Cargo);

    // Expected results computed directly from the same 8 inputs and m = 8
    cmsUInt16Number exp0 = Fn8D1(In[0], In[1], In[2], In[3], In[4], In[5], In[6], In[7], 8);
    cmsUInt16Number exp1 = Fn8D2(In[0], In[1], In[2], In[3], In[4], In[5], In[6], In[7], 8);
    cmsUInt16Number exp2 = Fn8D3(In[0], In[1], In[2], In[3], In[4], In[5], In[6], In[7], 8);

    // Assert (non-terminating)
    log.expectEq("Sampler8D_Basic_ret==1", ret, 1);
    log.expectEq("Sampler8D_Basic_Out0", Out[0], exp0);
    log.expectEq("Sampler8D_Basic_Out1", Out[1], exp1);
    log.expectEq("Sampler8D_Basic_Out2", Out[2], exp2);
}

// Test 2: Boundary values (max 16-bit) to exercise 16-bit handling
void test_Sampler8D_MaxValue(Tester& log) {
    cmsUInt16Number In[8] = {0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu};
    cmsUInt16Number Out[3]  = {0, 0, 0};
    cmsInt32Number ret = Sampler8D(In, Out, nullptr);

    cmsUInt16Number exp0 = Fn8D1(In[0], In[1], In[2], In[3], In[4], In[5], In[6], In[7], 8);
    cmsUInt16Number exp1 = Fn8D2(In[0], In[1], In[2], In[3], In[4], In[5], In[6], In[7], 8);
    cmsUInt16Number exp2 = Fn8D3(In[0], In[1], In[2], In[3], In[4], In[5], In[6], In[7], 8);

    log.expectEq("Sampler8D_MaxValue_ret==1", ret, 1);
    log.expectEq("Sampler8D_MaxValue_Out0", Out[0], exp0);
    log.expectEq("Sampler8D_MaxValue_Out1", Out[1], exp1);
    log.expectEq("Sampler8D_MaxValue_Out2", Out[2], exp2);
}

// Test 3: All zeros to verify baseline behavior
void test_Sampler8D_AllZero(Tester& log) {
    cmsUInt16Number In[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    cmsUInt16Number Out[3] = {0, 0, 0};
    cmsInt32Number ret = Sampler8D(In, Out, nullptr);

    cmsUInt16Number exp0 = Fn8D1(In[0], In[1], In[2], In[3], In[4], In[5], In[6], In[7], 8);
    cmsUInt16Number exp1 = Fn8D2(In[0], In[1], In[2], In[3], In[4], In[5], In[6], In[7], 8);
    cmsUInt16Number exp2 = Fn8D3(In[0], In[1], In[2], In[3], In[4], In[5], In[6], In[7], 8);

    log.expectEq("Sampler8D_AllZero_ret==1", ret, 1);
    log.expectEq("Sampler8D_AllZero_Out0", Out[0], exp0);
    log.expectEq("Sampler8D_AllZero_Out1", Out[1], exp1);
    log.expectEq("Sampler8D_AllZero_Out2", Out[2], exp2);
}

// Test 4: Randomish values to exercise typical usage
void test_Sampler8D_Randomish(Tester& log) {
    cmsUInt16Number In[8] = {13, 0, 255, 1024, 32768, 40000, 6550, 7};
    cmsUInt16Number Out[3] = {0, 0, 0};
    cmsInt32Number ret = Sampler8D(In, Out, nullptr);

    cmsUInt16Number exp0 = Fn8D1(In[0], In[1], In[2], In[3], In[4], In[5], In[6], In[7], 8);
    cmsUInt16Number exp1 = Fn8D2(In[0], In[1], In[2], In[3], In[4], In[5], In[6], In[7], 8);
    cmsUInt16Number exp2 = Fn8D3(In[0], In[1], In[2], In[3], In[4], In[5], In[6], In[7], 8);

    log.expectEq("Sampler8D_Randomish_ret==1", ret, 1);
    log.expectEq("Sampler8D_Randomish_Out0", Out[0], exp0);
    log.expectEq("Sampler8D_Randomish_Out1", Out[1], exp1);
    log.expectEq("Sampler8D_Randomish_Out2", Out[2], exp2);
}

// Main: run all tests
int main() {
    Tester log;
    std::cout << "Starting Sampler8D tests...\n";

    test_Sampler8D_Basic(log);
    test_Sampler8D_MaxValue(log);
    test_Sampler8D_AllZero(log);
    test_Sampler8D_Randomish(log);

    log.summary();
    // Non-terminating: do not abort on first failure; return non-zero if any test failed
    return log.failures > 0 ? 1 : 0;
}