// test_sampler7d.cpp
// Lightweight C++11 test suite for the focal method Sampler7D.
// This test does not use GoogleTest; it uses a small, non-terminating assertion mechanism
// and a simple test runner that can be linked with the existing C modules of the project.

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// ---------------------------------------------------------------------------
// External C interfaces (prototypes against the focal file's symbols).
// We declare extern "C" to ensure correct linkage from C to C++.
// ---------------------------------------------------------------------------

// Sampler7D takes 7 input 16-bit values, fills 3 output 16-bit values, and a Cargo pointer.
// Signature approximated to match the focal method in testcms2.c:
// cmsInt32Number Sampler7D(CMSREGISTER const cmsUInt16Number In[],
//                         CMSREGISTER cmsUInt16Number Out[],
//                         CMSREGISTER void * Cargo)
extern "C" int Sampler7D(const uint16_t In[7], uint16_t Out[3], void* Cargo);

// Fn8D1, Fn8D2, Fn8D3 are used inside Sampler7D. We declare them for expected-output computation.
// Signatures:
// cmsUInt16Number Fn8D1(cmsUInt16Number a1, ..., cmsUInt16Number a8, cmsUInt32Number m);
// (9 parameters total: 8 x cmsUInt16Number, then cmsUInt32Number m)
extern "C" uint16_t Fn8D1(uint16_t a1, uint16_t a2, uint16_t a3, uint16_t a4,
                        uint16_t a5, uint16_t a6, uint16_t a7, uint16_t a8,
                        uint32_t m);

extern "C" uint16_t Fn8D2(uint16_t a1, uint16_t a2, uint16_t a3, uint16_t a4,
                        uint16_t a5, uint16_t a6, uint16_t a7, uint16_t a8,
                        uint32_t m);

extern "C" uint16_t Fn8D3(uint16_t a1, uint16_t a2, uint16_t a3, uint16_t a4,
                        uint16_t a5, uint16_t a6, uint16_t a7, uint16_t a8,
                        uint32_t m);

// ---------------------------------------------------------------------------
// Simple, non-terminating test harness
// - We avoid terminating on a failed assertion to allow all tests to run.
// - A final summary exit code reflects overall success (0) or failure (non-zero).
// ---------------------------------------------------------------------------

static int g_tests_failed = 0;

// Lightweight non-terminating assertion
#define CHECK_EQ(actual, expected, msg)                                      \
    do {                                                                       \
        if ((actual) != (expected)) {                                        \
            std::cerr << "[FAIL] " << (msg) << " | Expected: "            \
                      << (expected) << ", Actual: " << (actual) << "\n";    \
            ++g_tests_failed;                                                \
        } else {                                                             \
            std::cout << "[OK] " << (msg) << "\n";                          \
        }                                                                    \
    } while (0)

// Convenience macro for simple truthiness checks
#define CHECK_TRUE(cond, msg)                                                  \
    do {                                                                         \
        if (!(cond)) {                                                           \
            std::cerr << "[FAIL] " << (msg) << " | Condition is false\n";     \
            ++g_tests_failed;                                                  \
        } else {                                                                 \
            std::cout << "[OK] " << (msg) << "\n";                            \
        }                                                                        \
    } while (0)

// Test 1: Basic correctness - Out[i] should equal Fn8D1/2/3(In..., 0, 7)
// for several representative input vectors. This validates the forwarding behavior
// of Sampler7D and its dependency on Fn8D1/2/3.
void test_sampler7d_basic_matches_fn8d() {
    // Test vectors
    const uint16_t vecs[][7] = {
        // Zero input
        {0, 0, 0, 0, 0, 0, 0},
        // Simple incremental sequence
        {1, 2, 3, 4, 5, 6, 7},
        // Larger values
        {100, 200, 300, 400, 500, 600, 700},
        // Arbitrary mixed values
        {6550, 123, 999, 1, 4321, 7, 89}
    };

    for (size_t i = 0; i < sizeof(vecs)/sizeof(vecs[0]); ++i) {
        const uint16_t* In = vecs[i];
        uint16_t Out[3] = {0, 0, 0};
        void* cargo = nullptr;

        // Call the function under test
        int ret = Sampler7D(In, Out, cargo);

        // Expected values computed via the known Fn8D* helpers with the same inputs
        uint16_t exp0 = Fn8D1(In[0], In[1], In[2], In[3], In[4], In[5], In[6], 0, 7);
        uint16_t exp1 = Fn8D2(In[0], In[1], In[2], In[3], In[4], In[5], In[6], 0, 7);
        uint16_t exp2 = Fn8D3(In[0], In[1], In[2], In[3], In[4], In[5], In[6], 0, 7);

        // Assertions
        CHECK_EQ(ret, 1, "Sampler7D should return 1");
        CHECK_EQ(Out[0], exp0, "Sampler7D Out[0] should equal Fn8D1(In, 0,7)");
        CHECK_EQ(Out[1], exp1, "Sampler7D Out[1] should equal Fn8D2(In, 0,7)");
        CHECK_EQ(Out[2], exp2, "Sampler7D Out[2] should equal Fn8D3(In, 0,7)");
    }
}

// Test 2: Statelessness and cargo parameter handling
// Sampler7D should be stateless with respect to Cargo (as indicated by cmsUNUSED_PARAMETER).
// We verify that providing a distinct Cargo pointer does not affect the outputs.
void test_sampler7d_cargo_statelessness() {
    const uint16_t In[7] = {11, 22, 33, 44, 55, 66, 77};
    uint16_t Out1[3] = {0, 0, 0};
    uint16_t Out2[3] = {0, 0, 0};

    // First call with a null cargo
    int ret1 = Sampler7D(In, Out1, nullptr);
    // Second call with a non-null cargo (distinct value)
    int ret2 = Sampler7D(In, Out2, (void*)0x12345678);

    // Expected outputs (same as with null cargo since Cargo is unused)
    uint16_t exp0 = Fn8D1(In[0], In[1], In[2], In[3], In[4], In[5], In[6], 0, 7);
    uint16_t exp1 = Fn8D2(In[0], In[1], In[2], In[3], In[4], In[5], In[6], 0, 7);
    uint16_t exp2 = Fn8D3(In[0], In[1], In[2], In[3], In[4], In[5], In[6], 0, 7);

    CHECK_EQ(ret1, 1, "Sampler7D (null cargo) return must be 1");
    CHECK_EQ(Out1[0], exp0, "Out1[0] should match Fn8D1(In,0,7) with null cargo");
    CHECK_EQ(Out1[1], exp1, "Out1[1] should match Fn8D2(In,0,7) with null cargo");
    CHECK_EQ(Out1[2], exp2, "Out1[2] should match Fn8D3(In,0,7) with null cargo");

    CHECK_EQ(ret2, 1, "Sampler7D (non-null cargo) return must be 1");
    CHECK_EQ(Out2[0], exp0, "Out2[0] should match Fn8D1(In,0,7) regardless of cargo");
    CHECK_EQ(Out2[1], exp1, "Out2[1] should match Fn8D2(In,0,7) regardless of cargo");
    CHECK_EQ(Out2[2], exp2, "Out2[2] should match Fn8D3(In,0,7) regardless of cargo");
}

// Test 3: Repeated invocations produce consistent results (repeatability)
void test_sampler7d_repeatability() {
    const uint16_t In[7] = {9, 18, 27, 36, 45, 54, 63};
    for (int cycle = 0; cycle < 5; ++cycle) {
        uint16_t OutA[3] = {0, 0, 0};
        uint16_t OutB[3] = {0, 0, 0};

        int retA = Sampler7D(In, OutA, nullptr);
        int retB = Sampler7D(In, OutB, nullptr);

        uint16_t exp0 = Fn8D1(In[0], In[1], In[2], In[3], In[4], In[5], In[6], 0, 7);
        uint16_t exp1 = Fn8D2(In[0], In[1], In[2], In[3], In[4], In[5], In[6], 0, 7);
        uint16_t exp2 = Fn8D3(In[0], In[1], In[2], In[3], In[4], In[5], In[6], 0, 7);

        CHECK_EQ(retA, 1, "Sampler7D repeatability cycle A return must be 1");
        CHECK_EQ(OutA[0], exp0, "Repeatability A Out[0] matches Fn8D1");
        CHECK_EQ(OutA[1], exp1, "Repeatability A Out[1] matches Fn8D2");
        CHECK_EQ(OutA[2], exp2, "Repeatability A Out[2] matches Fn8D3");

        CHECK_EQ(retB, 1, "Sampler7D repeatability cycle B return must be 1");
        CHECK_EQ(OutB[0], exp0, "Repeatability B Out[0] matches Fn8D1");
        CHECK_EQ(OutB[1], exp1, "Repeatability B Out[1] matches Fn8D2");
        CHECK_EQ(OutB[2], exp2, "Repeatability B Out[2] matches Fn8D3");
    }
}

// Entry point
int main() {
    std::cout << "Starting test suite for Sampler7D...\n";

    test_sampler7d_basic_matches_fn8d();
    test_sampler7d_cargo_statelessness();
    test_sampler7d_repeatability();

    if (g_tests_failed == 0) {
        std::cout << "[TEST SUITE PASSED] All Sampler7D tests passed.\n";
        return 0;
    } else {
        std::cerr << "[TEST SUITE FAILED] " << g_tests_failed << " test(s) failed.\n";
        return 1;
    }
}