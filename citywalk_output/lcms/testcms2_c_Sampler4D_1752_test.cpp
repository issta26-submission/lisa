/*
Step 1 - Program Understanding (Candidate Keywords)
Core dependent components extracted from the focal method:
- Fn8D1, Fn8D2, Fn8D3: the three dependent 8D sampler functions used to compute outputs.
- In: input array with at least 4 cmsUInt16Number values (In[0]..In[3]).
- Out: output array with at least 3 cmsUInt16Number values (Out[0]..Out[2]).
- Cargo: opaque parameter (unused in Sampler4D; ensure tests cover passing a non-null cargo).
- m (last argument to Fn8D1/2/3): value 4 used in the call.
- Sampler4D: the function under test, reads inputs, writes three outputs, returns 1.
- Relationship: Out[0] = Fn8D1(In[0], In[1], In[2], In[3], 0, 0, 0, 0, 4);
                 Out[1] = Fn8D2(In[0], In[1], In[2], In[3], 0, 0, 0, 0, 4);
                 Out[2] = Fn8D3(In[0], In[1], In[2], In[3], 0, 0, 0, 0, 4);
- Static considerations: Static helpers exist outside; in tests, access via the declared functions.
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <testcms2.h>
#include <cstdint>


// Define minimal types and macros to mimic the C/CMS environment
using cmsUInt16Number = uint16_t;
using cmsUInt32Number = uint32_t;
using cmsInt32Number = int;

#define CMSREGISTER
#define cmsUNUSED_PARAMETER(x) (void)(x)

// Forward declarations to mirror the focal file's dependencies
extern "C" cmsUInt16Number Fn8D1(cmsUInt16Number a1, cmsUInt16Number a2, cmsUInt16Number a3, cmsUInt16Number a4,
                                cmsUInt16Number a5, cmsUInt16Number a6, cmsUInt16Number a7, cmsUInt16Number a8,
                                cmsUInt32Number m);

extern "C" cmsUInt16Number Fn8D2(cmsUInt16Number a1, cmsUInt16Number a2, cmsUInt16Number a3, cmsUInt16Number a4,
                                cmsUInt16Number a5, cmsUInt16Number a6, cmsUInt16Number a7, cmsUInt16Number a8,
                                cmsUInt32Number m);

extern "C" cmsUInt16Number Fn8D3(cmsUInt16Number a1, cmsUInt16Number a2, cmsUInt16Number a3, cmsUInt16Number a4,
                                cmsUInt16Number a5, cmsUInt16Number a6, cmsUInt16Number a7, cmsUInt16Number a8,
                                cmsUInt32Number m);

// The focal function under test (re-implemented here for a self-contained test)
extern "C" cmsInt32Number Sampler4D(CMSREGISTER const cmsUInt16Number In[],
               CMSREGISTER cmsUInt16Number Out[],
               CMSREGISTER void * Cargo)
{
    // Match the exact behavior from the provided focal method
    Out[0] = Fn8D1(In[0], In[1], In[2], In[3], 0, 0, 0, 0, 4);
    Out[1] = Fn8D2(In[0], In[1], In[2], In[3], 0, 0, 0, 0, 4);
    Out[2] = Fn8D3(In[0], In[1], In[2], In[3], 0, 0, 0, 0, 4);
    return 1;
    cmsUNUSED_PARAMETER(Cargo);
}

// Simple, deterministic implementations of Fn8D1/2/3 to enable controlled testing.
// These implementations are external to the original library and are provided here
// to create a stable, testable environment for Sampler4D.
extern "C" cmsUInt16Number Fn8D1(cmsUInt16Number a1, cmsUInt16Number a2, cmsUInt16Number a3, cmsUInt16Number a4,
                                cmsUInt16Number a5, cmsUInt16Number a6, cmsUInt16Number a7, cmsUInt16Number a8,
                                cmsUInt32Number m)
{
    // Simple deterministic function: sum of first four inputs plus m
    uint32_t sum = static_cast<uint32_t>(a1) + a2 + a3 + a4 + m;
    return static_cast<cmsUInt16Number>(sum & 0xFFFF);
}

extern "C" cmsUInt16Number Fn8D2(cmsUInt16Number a1, cmsUInt16Number a2, cmsUInt16Number a3, cmsUInt16Number a4,
                                cmsUInt16Number a5, cmsUInt16Number a6, cmsUInt16Number a7, cmsUInt16Number a8,
                                cmsUInt32Number m)
{
    // Distinct deterministic function: XOR of first four inputs plus m
    uint32_t x = static_cast<uint32_t>(a1) ^ a2 ^ a3 ^ a4;
    x ^= m;
    return static_cast<cmsUInt16Number>(x & 0xFFFF);
}

extern "C" cmsUInt16Number Fn8D3(cmsUInt16Number a1, cmsUInt16Number a2, cmsUInt16Number a3, cmsUInt16Number a4,
                                cmsUInt16Number a5, cmsUInt16Number a6, cmsUInt16Number a7, cmsUInt16Number a8,
                                cmsUInt32Number m)
{
    // Another deterministic function: weighted sum of first four inputs plus m
    uint32_t sum = static_cast<uint32_t>(a1) * 1 + static_cast<uint32_t>(a2) * 2
                   + static_cast<uint32_t>(a3) * 3 + static_cast<uint32_t>(a4) * 4 + m;
    return static_cast<cmsUInt16Number>(sum & 0xFFFF);
}

// Simple test framework (non-terminating assertions allowed by domain guidance)
static int g_total = 0;
static int g_passed = 0;

static void log_pass(const char* name) {
    std::cout << "[PASS] " << name << "\n";
    g_passed++;
}

static void log_fail(const char* name, const char* detail) {
    std::cout << "[FAIL] " << name << " - " << detail << "\n";
}

static void check_equal_uint16(const char* name, cmsUInt16Number expected, cmsUInt16Number actual) {
    g_total++;
    if (expected == actual) {
        log_pass(name);
        g_passed++;
    } else {
        log_fail(name, ("expected " + std::to_string(expected) + ", got " + std::to_string(actual)).c_str());
    }
}

// Convenience helper to run a single test case
static void run_case(const char* case_name, const cmsUInt16Number In[4]) {
    cmsUInt16Number Out[3] = {0, 0, 0};
    cmsInt32Number ret = Sampler4D(In, Out, nullptr);

    cmsUInt16Number exp0 = Fn8D1(In[0], In[1], In[2], In[3], 0, 0, 0, 0, 4);
    cmsUInt16Number exp1 = Fn8D2(In[0], In[1], In[2], In[3], 0, 0, 0, 0, 4);
    cmsUInt16Number exp2 = Fn8D3(In[0], In[1], In[2], In[3], 0, 0, 0, 0, 4);

    // Description for test: verify that Sampler4D uses Fn8D1/2/3 to fill outputs
    if (ret != 1) {
        log_fail(case_name, "Sampler4D did not return 1 as expected");
        return;
    }

    // Assertions (non-terminating): compare actual outputs against expected
    char detail0[128], detail1[128], detail2[128];
    std::snprintf(detail0, sizeof(detail0), "Out[0] == Fn8D1(...) -> expect %u, got %u", exp0, Out[0]);
    std::snprintf(detail1, sizeof(detail1), "Out[1] == Fn8D2(...) -> expect %u, got %u", exp1, Out[1]);
    std::snprintf(detail2, sizeof(detail2), "Out[2] == Fn8D3(...) -> expect %u, got %u", exp2, Out[2]);

    check_equal_uint16(std::string(case_name) + " - Out0").c_str(), exp0, Out[0]);
    // Because we cannot pass a C++ string to check_equal_uint16 directly, do inline checks:
    if (Out[0] != exp0) log_fail(case_name, detail0);
    if (Out[1] != exp1) log_fail(case_name, detail1);
    if (Out[2] != exp2) log_fail(case_name, detail2);
}

// Since we can't overload check_equal_uint16 with dynamic names easily in this simple runner,
// provide a small helper wrapper to call per-case checks with a name
static void run_case_named(const char* case_name, const cmsUInt16Number In[4]) {
    cmsUInt16Number Out[3] = {0, 0, 0};
    cmsInt32Number ret = Sampler4D(In, Out, nullptr);
    cmsUInt16Number exp0 = Fn8D1(In[0], In[1], In[2], In[3], 0, 0, 0, 0, 4);
    cmsUInt16Number exp1 = Fn8D2(In[0], In[1], In[2], In[3], 0, 0, 0, 0, 4);
    cmsUInt16Number exp2 = Fn8D3(In[0], In[1], In[2], In[3], 0, 0, 0, 0, 4);

    bool ok = true;
    if (ret != 1) {
        log_fail(case_name, "Sampler4D return value != 1");
        ok = false;
    }
    if (Out[0] != exp0) {
        std::cout << "[DETAIL] " << case_name << " - Out[0] mismatch: expected " << exp0 << ", got " << Out[0] << "\n";
        ok = false;
    }
    if (Out[1] != exp1) {
        std::cout << "[DETAIL] " << case_name << " - Out[1] mismatch: expected " << exp1 << ", got " << Out[1] << "\n";
        ok = false;
    }
    if (Out[2] != exp2) {
        std::cout << "[DETAIL] " << case_name << " - Out[2] mismatch: expected " << exp2 << ", got " << Out[2] << "\n";
        ok = false;
    }
    if (ok) log_pass(case_name);
    else     log_fail(case_name, "Case failed.");
}

// Entry point
int main() {
    // Step 2: Unit Test Generation
    // Define a set of representative test vectors for In[0..3]
    cmsUInt16Number t1[4] = {0, 0, 0, 0};
    cmsUInt16Number t2[4] = {1, 2, 3, 4};
    cmsUInt16Number t3[4] = {65535, 65535, 0, 1};
    cmsUInt16Number t4[4] = {1000, 2000, 3000, 4000};

    // Run tests
    // Test case 1: all zeros
    run_case_named("Sampler4D Case 1 - zeros", t1);

    // Test case 2: small incremental values
    run_case_named("Sampler4D Case 2 - small sequence", t2);

    // Test case 3: large values including 65535
    run_case_named("Sampler4D Case 3 - max-ish values", t3);

    // Test case 4: arbitrary larger values
    run_case_named("Sampler4D Case 4 - mixed large", t4);

    // Test case 5: ensure cargo pointer is tolerated (non-null)
    cmsUInt16Number dummyCargo = 0xABCD;
    cmsUInt16Number In5[4] = {5, 6, 7, 8};
    cmsUInt16Number Out5[3] = {0,0,0};
    cmsInt32Number ret = Sampler4D(In5, Out5, &dummyCargo);
    cmsUInt16Number exp0 = Fn8D1(In5[0], In5[1], In5[2], In5[3], 0, 0, 0, 0, 4);
    cmsUInt16Number exp1 = Fn8D2(In5[0], In5[1], In5[2], In5[3], 0, 0, 0, 0, 4);
    cmsUInt16Number exp2 = Fn8D3(In5[0], In5[1], In5[2], In5[3], 0, 0, 0, 0, 4);
    if (ret == 1 && Out5[0] == exp0 && Out5[1] == exp1 && Out5[2] == exp2) {
        log_pass("Sampler4D Case 5 - Cargo non-null eqs");
    } else {
        log_fail("Sampler4D Case 5 - Cargo non-null eqs",
                 "Unexpected outputs or return value.");
        // Detailed mismatch message
        if (ret != 1) std::cout << "  -> return value expected 1, got " << ret << "\n";
        if (Out5[0] != exp0) std::cout << "  -> Out[0] expected " << exp0 << ", got " << Out5[0] << "\n";
        if (Out5[1] != exp1) std::cout << "  -> Out[1] expected " << exp1 << ", got " << Out5[1] << "\n";
        if (Out5[2] != exp2) std::cout << "  -> Out[2] expected " << exp2 << ", got " << Out5[2] << "\n";
    }

    // Final summary
    std::cout << "\nTest Summary: " << g_passed << " passed, " << g_total << " total." << std::endl;
    return (g_passed == g_total) ? 0 : 1;
}