// Test suite for the focal method Reverse in mkcmy.c
// This file provides a self-contained C++11-compliant unit test harness
// without using Google Test. It mocks the necessary dependencies and
// exercises the Reverse function behavior with deterministic stubs.

#include <iostream>
#include <cstdint>
#include <lcms.h>


// Domain-specific typedefs (simplified for test harness)
typedef uint16_t WORD;
typedef void* LPVOID;

// Mimic cargo structure used by the focal method
typedef struct Cargo {
    void* RGB2Lab;  // Transform handle passed to cmsDoTransform
} Cargo;
typedef Cargo* LPCARGO;

#define TRUE 1

// Global hook to verify cmsDoTransform invocation arguments
static void* gLastTransformArg = nullptr;

// Mock implementation of cmsDoTransform used by Reverse during tests.
// It performs a deterministic bitwise transformation on RGB input to produce Out.
// Out[i] = RGB[i] XOR (lower 16 bits of Transform pointer value)
void cmsDoTransform(void* Transform, void* Source, void* Destination, unsigned int /*Count*/)
{
    WORD* rgb = static_cast<WORD*>(Source);
    WORD* out = static_cast<WORD*>(Destination);

    // Use the lower 16 bits of the pointer value as the "transform" key
    uint16_t t = reinterpret_cast<uintptr_t>(Transform) & 0xFFFF;
    for (int i = 0; i < 3; ++i)
        out[i] = rgb[i] ^ t;

    // Record the last transform argument for assertion in tests
    gLastTransformArg = Transform;
}

// Focal method under test replicated as provided
int Reverse(register WORD In[], register WORD Out[], register LPVOID Cargo)
{	
{	
	LPCARGO C = (LPCARGO) Cargo;	
	WORD RGB[3];
  
	RGB[0] = 0xFFFF - In[0];
	RGB[1] = 0xFFFF - In[1];
	RGB[2] = 0xFFFF - In[2];
	cmsDoTransform(C ->RGB2Lab, &RGB, Out, 1);
	
	return TRUE;
}
}

// Lightweight test framework predicates (non-terminating assertions)
static int gTotalTests = 0;
static int gFailedTests = 0;

static void failMsg(const char* msg)
{
    std::cerr << "Test failure: " << msg << std::endl;
}

// Test 1: Basic zero input with a non-null transform pointer.
// Expectation: Reverse returns TRUE, LastTransformArg matches the cargo pointer,
// and Out equals {0xFFFF ^ transform, 0xFFFF ^ transform, 0xFFFF ^ transform}.
bool test1_BasicZeroInput()
{
    gTotalTests++;

    // Arrange
    WORD In[3]  = { 0, 0, 0 };
    WORD Out[3] = { 0, 0, 0 };
    Cargo C;
    C.RGB2Lab = (void*)0x1000;  // sentinel transform pointer

    gLastTransformArg = nullptr;

    // Act
    int res = Reverse(In, Out, &C);

    // Compute expected
    WORD expected = 0xFFFF ^ 0x1000; // 0xEFFF

    // Assert
    bool ok = true;
    if (res != TRUE) { failMsg("Reverse did not return TRUE on test1"); ok = false; }
    if (gLastTransformArg != (void*)0x1000) {
        failMsg("test1: cmsDoTransform received unexpected Transform argument");
        ok = false;
    }
    for (int i = 0; i < 3; ++i) {
        if (Out[i] != expected) {
            std::cerr << "test1: Out[" << i << "] = " << std::hex << Out[i]
                      << " (expected " << expected << ")" << std::dec << "\n";
            ok = false;
            break;
        }
    }

    if (!ok) ++gFailedTests;
    return ok;
}

// Test 2: Non-zero inputs with same transform; verify per-element calculation.
bool test2_NonZeroInputs()
{
    gTotalTests++;

    WORD In[3]  = { 1, 2, 3 };
    WORD Out[3] = { 0, 0, 0 };
    Cargo C;
    C.RGB2Lab = (void*)0x1000;

    gLastTransformArg = nullptr;

    int res = Reverse(In, Out, &C);

    // Expected Out values using rule: Out[i] = (0xFFFF - In[i]) XOR 0x1000
    // Which simplifies to: 0xFFFE ^ 0x1000 = 0xEFFE, 0xFFFD ^ 0x1000 = 0xEFFD, 0xFFFC ^ 0x1000 = 0xEFFC
    WORD expected0 = 0xEFFE;
    WORD expected1 = 0xEFFD;
    WORD expected2 = 0xEFFC;

    bool ok = true;
    if (res != TRUE) { failMsg("Reverse did not return TRUE on test2"); ok = false; }
    if (gLastTransformArg != (void*)0x1000) {
        failMsg("test2: cmsDoTransform received unexpected Transform argument");
        ok = false;
    }
    if (Out[0] != expected0 || Out[1] != expected1 || Out[2] != expected2) {
        std::cerr << "test2: Out mismatch. Got {"
                  << std::hex << Out[0] << ", "
                  << Out[1] << ", "
                  << Out[2] << "} expected {"
                  << expected0 << ", "
                  << expected1 << ", "
                  << expected2 << "}\n" << std::dec;
        ok = false;
    }

    if (!ok) ++gFailedTests;
    return ok;
}

// Test 3: Edge case with mixed inputs including maximum and mid-range values.
bool test3_EdgeCases()
{
    gTotalTests++;

    WORD In[3]  = { 0xFFFF, 0x0000, 0x8000 };
    WORD Out[3] = { 0, 0, 0 };
    Cargo C;
    C.RGB2Lab = (void*)0x1000;

    gLastTransformArg = nullptr;

    int res = Reverse(In, Out, &C);

    // Expected: RGB = {0x0000, 0xFFFF, 0x7FFF}
    // So Out = {0x1000, 0xEFFF, 0x6FFF}
    WORD expected0 = 0x1000;
    WORD expected1 = 0xEFFF;
    WORD expected2 = 0x6FFF;

    bool ok = true;
    if (res != TRUE) { failMsg("Reverse did not return TRUE on test3"); ok = false; }
    if (gLastTransformArg != (void*)0x1000) {
        failMsg("test3: cmsDoTransform received unexpected Transform argument");
        ok = false;
    }
    if (Out[0] != expected0 || Out[1] != expected1 || Out[2] != expected2) {
        std::cerr << "test3: Out mismatch. Got {"
                  << std::hex << Out[0] << ", "
                  << Out[1] << ", "
                  << Out[2] << "} expected {"
                  << expected0 << ", "
                  << expected1 << ", "
                  << expected2 << "}\n" << std::dec;
        ok = false;
    }

    if (!ok) ++gFailedTests;
    return ok;
}

int main(void)
{
    // Run tests
    bool t1 = test1_BasicZeroInput();
    bool t2 = test2_NonZeroInputs();
    bool t3 = test3_EdgeCases();

    // Summary
    std::cout << "\nTest Summary:\n";
    std::cout << "Total tests executed: " << gTotalTests << "\n";
    std::cout << "Failures: " << gFailedTests << "\n";

    // Return non-zero if any test failed
    return gFailedTests != 0 ? 1 : 0;
}