// Unit test suite for PCS2ITU in jpgicc.c using a lightweight, framework-free approach
// This test suite is designed to be compiled with C++11, without Google Test, and links
// against the production PCS2ITU implementation. To enable isolated testing, we provide
// simple stub implementations of the dependent color-conversion routines that PCS2ITU()
// relies on (cmsLabEncoded2Float, cmsDesaturateLab, Lab2ITU) with deterministic behavior.

#include <iccjpeg.h>
#include <cstdint>
#include <jpeglib.h>
#include <iostream>
#include <string>
#include <utils.h>
#include <cmath>


// Type aliases and small helpers to mirror the production environment loosely
typedef unsigned short cmsUInt16Number;
typedef int cmsBool;
#define TRUE 1

// Lightweight C struct mirroring cmsCIELab usage in PCS2ITU
struct cmsCIELab { double L; double a; double b; };

// Forward declaration of the focal function (assumed to be provided by the production code)
extern "C" int PCS2ITU(const cmsUInt16Number In[], cmsUInt16Number Out[], void* Cargo);

// Prototypes for dependent functions used by PCS2ITU
extern "C" void cmsLabEncoded2Float(cmsCIELab* Lab, const cmsUInt16Number In[]);
extern "C" void cmsDesaturateLab(cmsCIELab* Lab, int param1, int param2, int param3, int param4);
extern "C" void Lab2ITU(const cmsCIELab* Lab, cmsUInt16Number Out[3]);

// -------------- Mocked (stub) dependencies with deterministic behavior --------------
// These stubs are linked in place of the real library implementations to test PCS2ITU in isolation.

extern "C" void cmsLabEncoded2Float(cmsCIELab* Lab, const cmsUInt16Number In[]) {
    // Simple mapping: L <- In[0], a <- In[1] - 32768, b <- In[2] - 32768
    Lab->L = static_cast<double>(In[0]);
    Lab->a = static_cast<double>(In[1]) - 32768.0;
    Lab->b = static_cast<double>(In[2]) - 32768.0;
}

extern "C" void cmsDesaturateLab(cmsCIELab* Lab, int /*p1*/, int /*p2*/, int /*p3*/, int /*p4*/) {
    // Deterministic desaturation: scale a and b components by 0.5
    Lab->a *= 0.5;
    Lab->b *= 0.5;
    // No saturation bounds for simplicity; rely on Lab2ITU casting behavior
}

extern "C" void Lab2ITU(const cmsCIELab* Lab, cmsUInt16Number Out[3]) {
    // Simple conversion: round L, a, b to nearest integers and cast to 16-bit
    Out[0] = static_cast<cmsUInt16Number>(llround(Lab->L));
    Out[1] = static_cast<cmsUInt16Number>(llround(Lab->a));
    Out[2] = static_cast<cmsUInt16Number>(llround(Lab->b));
}

// -------------- Lightweight test harness --------------

static bool test_PCS2ITU_Positive(std::string& reason) {
    // Test with input that yields deterministic, small positive a and b after desaturation
    // In = {L=1000, a_in=4, b_in=4} mapped through In[1]-32768 and In[2]-32768
    cmsUInt16Number In[3]  = {1000, 32772, 32772}; // a_init = 4; b_init = 4
    cmsUInt16Number Out[3] = {0, 0, 0};

    int rc = PCS2ITU(In, Out, nullptr);
    if (rc != TRUE) {
        reason = "PCS2ITU did not return TRUE for positive test case.";
        return false;
    }

    // Expected: L=1000, a=4*0.5=2, b=4*0.5=2
    if (Out[0] != 1000 || Out[1] != 2 || Out[2] != 2) {
        reason = "PCS2ITU produced unexpected output for positive test case. "
                 "Expected {1000, 2, 2}, got {" +
                 std::to_string(Out[0]) + ", " +
                 std::to_string(Out[1]) + ", " +
                 std::to_string(Out[2]) + "}.";
        return false;
    }
    return true;
}

static bool test_PCS2ITU_Negative(std::string& reason) {
    // Test with input that yields negative a and b before desaturation, to observe wrap-around
    // In = {L=500, a_in=-4 -> In[1]=32764, b_in=-4 -> In[2]=32764}
    cmsUInt16Number In[3]  = {500, 32764, 32764}; // a_init = -4; b_init = -4
    cmsUInt16Number Out[3] = {0, 0, 0};

    int rc = PCS2ITU(In, Out, nullptr);
    if (rc != TRUE) {
        reason = "PCS2ITU did not return TRUE for negative test case.";
        return false;
    }

    // After desaturation: a = -2, b = -2; Casting to 16-bit unsigned yields 0xFFFE = 65534
    if (Out[0] != 500 || Out[1] != 65534 || Out[2] != 65534) {
        reason = "PCS2ITU produced unexpected output for negative test case. "
                 "Expected {500, 65534, 65534}, got {" +
                 std::to_string(Out[0]) + ", " +
                 std::to_string(Out[1]) + ", " +
                 std::to_string(Out[2]) + "}.";
        return false;
    }
    return true;
}

static bool test_PCS2ITU_MaxMin(std::string& reason) {
    // Edge case: high L value, a and b zero after encoding
    // In = {L=65535, a_in=32768, b_in=32768} -> a_init=0, b_init=0
    cmsUInt16Number In[3]  = {65535, 32768, 32768};
    cmsUInt16Number Out[3] = {0, 0, 0};

    int rc = PCS2ITU(In, Out, nullptr);
    if (rc != TRUE) {
        reason = "PCS2ITU did not return TRUE for max/min test case.";
        return false;
    }

    if (Out[0] != 65535 || Out[1] != 0 || Out[2] != 0) {
        reason = "PCS2ITU produced unexpected output for max/min test case. "
                 "Expected {65535, 0, 0}, got {" +
                 std::to_string(Out[0]) + ", " +
                 std::to_string(Out[1]) + ", " +
                 std::to_string(Out[2]) + "}.";
        return false;
    }
    return true;
}

// -------------- Main harness --------------

int main() {
    int failures = 0;
    std::string reason;

    if (!test_PCS2ITU_Positive(reason)) {
        std::cout << "[FAILED] test_PCS2ITU_Positive: " << reason << std::endl;
        ++failures;
    } else {
        std::cout << "[PASSED] test_PCS2ITU_Positive" << std::endl;
    }

    if (!test_PCS2ITU_Negative(reason)) {
        std::cout << "[FAILED] test_PCS2ITU_Negative: " << reason << std::endl;
        ++failures;
    } else {
        std::cout << "[PASSED] test_PCS2ITU_Negative" << std::endl;
    }

    if (!test_PCS2ITU_MaxMin(reason)) {
        std::cout << "[FAILED] test_PCS2ITU_MaxMin: " << reason << std::endl;
        ++failures;
    } else {
        std::cout << "[PASSED] test_PCS2ITU_MaxMin" << std::endl;
    }

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << failures << " test(s) failed.\n";
        return 1;
    }
}