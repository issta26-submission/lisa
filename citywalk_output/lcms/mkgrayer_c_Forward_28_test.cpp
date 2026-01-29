// -------------------------------------------------------------
// Test suite for Forward function (focal method) from mkgrayer.c
// This test suite is implemented as a standalone C++11 program
// without Google Test. It uses a minimal stub of the cmsLCMS
// dependency (cmsCIELab and cmsLabEncoded2Float) to drive
// the Forward logic deterministically.
// The focus is to cover true/false branches of the condition
// (fabs(Lab.a) < 3 && fabs(Lab.b) < 3) and to validate the
// computed outputs accordingly.
// -------------------------------------------------------------

#include <iostream>
#include <cmath>
#include <lcms.h>


// Domain-specific type aliases and stubs to emulate focal dependencies.
typedef unsigned short WORD;
typedef void* LPVOID;

#define TRUE 1

// Minimal representation of the cmsCIELab structure used by Forward.
struct cmsCIELab {
    double L;
    double a;
    double b;
};

// Forward declaration of the dependency function.
// In the original project this would come from "lcms.h".
void cmsLabEncoded2Float(struct cmsCIELab* Lab, const WORD In[]);

// Stub implementation of cmsLabEncoded2Float to provide deterministic Lab values
// based on the input encoded Lab values. This maps the 3 WORDs to Lab.L, Lab.a, Lab.b.
void cmsLabEncoded2Float(struct cmsCIELab* Lab, const WORD In[]) {
    // Map to a normalized Lab:
    // - L in [0,100] mapped from In[0] in [0,65535]
    // - a in [-3, +3] mapped from In[1] in [0,65535]
    // - b in [-3, +3] mapped from In[2] in [0,65535]
    Lab->L = (double)In[0] / 65535.0 * 100.0;
    Lab->a = (double)In[1] / 65535.0 * 6.0 - 3.0;
    Lab->b = (double)In[2] / 65535.0 * 6.0 - 3.0;
}

// The focal Forward method reproduced for testing purposes.
// It mirrors the logic exactly as provided in the problem statement.
int Forward(register WORD In[], register WORD Out[], register LPVOID Cargo)
{
{	
    cmsCIELab Lab;

    cmsLabEncoded2Float(&Lab, In);
	if (fabs(Lab.a) < 3 && fabs(Lab.b) < 3) {
		
		double L_01 = Lab.L / 100.0;
	    WORD K;
		if (L_01 > 1) L_01 = 1;
		K = (WORD) floor(L_01* 65535.0 + 0.5);
		Out[0] = Out[1] = Out[2] = K; 
	}
	else {
		Out[0] = 0xFFFF; Out[1] = 0; Out[2] = 0; 
	}
	return TRUE;
}
}

// Simple non-terminating assertion mechanism.
// It prints a diagnostic message on failure but does not abort the test run.
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: " << (msg) \
                      << " in " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_failures; \
        } \
    } while (0)

// Test 1: True branch of the condition (fabs(Lab.a) < 3 && fabs(Lab.b) < 3)
// With Lab.a = 0 and Lab.b = 0, L = 50 => L_01 = 0.5 => K = floor(0.5*65535 + 0.5) = 32768.
// Expected Out = {32768, 32768, 32768} and return value TRUE.
void test_forward_true_branch() {
    WORD In[3];
    WORD Out[3] = {0, 0, 0};

    // Set encoded Lab to L=50, a=0, b=0
    In[0] = 32768; // L encoding ~ 0.5 of 100 -> L = 50
    In[1] = 32768; // a encoding ~ 0 -> a = 0
    In[2] = 32768; // b encoding ~ 0 -> b = 0

    int ret = Forward(In, Out, nullptr);

    EXPECT_TRUE(ret == TRUE, "Forward should return TRUE on true-branch input");
    EXPECT_TRUE(Out[0] == 32768 && Out[1] == 32768 && Out[2] == 32768,
                "Out should be all 32768 when in the true-branch case");
}

// Test 2: False branch of the condition (fabs(Lab.a) < 3 && fabs(Lab.b) < 3 must fail)
// We set Lab.a = 3 and Lab.b = -3 (boundary values) to trigger the else-branch.
// Expected Out = {0xFFFF, 0, 0} and return value TRUE.
void test_forward_false_branch() {
    WORD In[3];
    WORD Out[3] = {0, 0, 0};

    // Set encoded Lab to L=0, a=3, b=-3
    In[0] = 0;       // L encoding -> L = 0
    In[1] = 65535;   // a encoding -> a = +3
    In[2] = 0;       // b encoding -> b = -3

    int ret = Forward(In, Out, nullptr);

    EXPECT_TRUE(ret == TRUE, "Forward should return TRUE on false-branch input");
    EXPECT_TRUE(Out[0] == 0xFFFF && Out[1] == 0 && Out[2] == 0,
                "Out should be {0xFFFF, 0, 0} when in the false-branch case");
}

// Step 3: Test runner invoking all tests and reporting summary.
int main() {
    std::cout << "Starting Forward() unit tests...\n";

    test_forward_true_branch();
    test_forward_false_branch();

    if (g_failures == 0) {
        std::cout << "All tests PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED\n";
        return 1;
    }
}