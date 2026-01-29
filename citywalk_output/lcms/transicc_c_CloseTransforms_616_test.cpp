// Test suite for CloseTransforms(void) in transicc.c
// This test suite is designed to be compiled with g++ -std=c++11
// along with the original transicc.c file (as part of the project).
// It mocks the external C dependencies used by CloseTransforms and
// verifies that the correct cleanup calls are made depending on the
// state of global pointers.
//
// Notes:
// - The real project uses cmsFreeNamedColorList and cmsDeleteTransform
//   from LittleCMS (lcms). For testability in this isolated environment,
//   we mock these functions with void* parameter types to avoid tight
//   coupling to specific internal types.
// - We expose the global pointers (InputColorant, OutputColorant, hTrans,
//   hTransLab, hTransXYZ) as extern void* so we can manipulate them from tests.
// - The tests use lightweight EXPECT-style macros to continue execution
//   even after a failed assertion, enabling higher coverage.

#include <iostream>
#include <utils.h>
#include <cstdlib>


// Forward declare the external C symbols used by CloseTransforms.
// We use void* to keep the interface agnostic to internal typedefs.
extern "C" {
    // Global pointers that CloseTransforms checks
    extern void* InputColorant;
    extern void* OutputColorant;
    extern void* hTrans;
    extern void* hTransLab;
    extern void* hTransXYZ;

    // The function under test
    void CloseTransforms(void);

    // Mockable external dependencies used by CloseTransforms
    void cmsFreeNamedColorList(void*);
    void cmsDeleteTransform(void*);
}

// Global counters to observe how many times the mocks were invoked
static int g_FreeCalls = 0;
static int g_DeleteCalls = 0;

// Mock implementations of the external dependencies.
// These are declared with C linkage to match the real function definitions.
extern "C" void cmsFreeNamedColorList(void* /*ptr*/)
{
    // Increment the counter whenever a free is invoked
    ++g_FreeCalls;
}

extern "C" void cmsDeleteTransform(void* /*ptr*/)
{
    // Increment the counter whenever a transform deletion is invoked
    ++g_DeleteCalls;
}

// Helper macros to emulate a lightweight test framework without GTest.
// They allow non-terminating (EXPECT) checks while still reporting failures.
static int g_TestFailures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_TestFailures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        std::cerr << "EXPECT_EQ FAILED: " #a " != " #b " (" << (a) \
                  << " != " << (b) << "). " << (msg) \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_TestFailures; \
    } \
} while(0)

#define RESET_COUNTERS() do { g_FreeCalls = 0; g_DeleteCalls = 0; } while(0)

// Utility to reset all relevant pointers to null before each test
static void reset_pointers()
{
    InputColorant = nullptr;
    OutputColorant = nullptr;
    hTrans = nullptr;
    hTransLab = nullptr;
    hTransXYZ = nullptr;
}

// Test 1: All pointers are null -> CloseTransforms should not call any cleanup functions
void test_CloseTransforms_allNull()
{
    RESET_COUNTERS();
    reset_pointers();
    CloseTransforms();

    EXPECT_EQ(g_FreeCalls, 0, "No free should be called when InputColorant/output are null");
    EXPECT_EQ(g_DeleteCalls, 0, "No delete should be called when transforms are null");
}

// Test 2: Only InputColorant is non-null -> one free should be invoked
void test_CloseTransforms_inputColorantOnly()
{
    RESET_COUNTERS();
    reset_pointers();
    InputColorant = reinterpret_cast<void*>(0x1); // non-null sentinel

    CloseTransforms();

    EXPECT_EQ(g_FreeCalls, 1, "cmsFreeNamedColorList should be called once for InputColorant");
    EXPECT_EQ(g_DeleteCalls, 0, "No cmsDeleteTransform should be called when no transforms set");
}

// Test 3: Only OutputColorant is non-null -> one free should be invoked
void test_CloseTransforms_outputColorantOnly()
{
    RESET_COUNTERS();
    reset_pointers();
    OutputColorant = reinterpret_cast<void*>(0x2); // non-null sentinel

    CloseTransforms();

    EXPECT_EQ(g_FreeCalls, 1, "cmsFreeNamedColorList should be called once for OutputColorant");
    EXPECT_EQ(g_DeleteCalls, 0, "No cmsDeleteTransform should be called when no transforms set");
}

// Test 4: Only hTrans is non-null -> one delete should be invoked
void test_CloseTransforms_singleTransform()
{
    RESET_COUNTERS();
    reset_pointers();
    hTrans = reinterpret_cast<void*>(0x3); // non-null sentinel

    CloseTransforms();

    EXPECT_EQ(g_FreeCalls, 0, "No cmsFreeNamedColorList should be called when no colorants set");
    EXPECT_EQ(g_DeleteCalls, 1, "cmsDeleteTransform should be called once for hTrans");
}

// Test 5: All three transforms non-null -> three deletes invoked
void test_CloseTransforms_allTransforms()
{
    RESET_COUNTERS();
    reset_pointers();
    hTrans = reinterpret_cast<void*>(0x4);
    hTransLab = reinterpret_cast<void*>(0x5);
    hTransXYZ = reinterpret_cast<void*>(0x6);

    CloseTransforms();

    EXPECT_EQ(g_FreeCalls, 0, "No cmsFreeNamedColorList should be called when no colorants set");
    EXPECT_EQ(g_DeleteCalls, 3, "cmsDeleteTransform should be called for each non-null transform");
}

// Test 6: Mixed non-null colorants and transforms -> ensure correct counts
void test_CloseTransforms_mixed()
{
    RESET_COUNTERS();
    reset_pointers();
    InputColorant = reinterpret_cast<void*>(0x7);
    OutputColorant = reinterpret_cast<void*>(0x8);
    hTrans = reinterpret_cast<void*>(0x9);
    hTransLab = nullptr;
    hTransXYZ = reinterpret_cast<void*>(0xA);

    CloseTransforms();

    // Expect frees for Input and Output colorants, deletes for three non-null transforms
    // (Note: hTransLab is null, others non-null)
    EXPECT_EQ(g_FreeCalls, 2, "Two non-null colorants should trigger two cmsFreeNamedColorList calls");
    EXPECT_EQ(g_DeleteCalls, 3, "Three non-null transforms should trigger three cmsDeleteTransform calls");
}

// Main test runner
int main()
{
    std::cout << "Starting CloseTransforms unit tests (no GTest):" << std::endl;

    test_CloseTransforms_allNull();
    test_CloseTransforms_inputColorantOnly();
    test_CloseTransforms_outputColorantOnly();
    test_CloseTransforms_singleTransform();
    test_CloseTransforms_allTransforms();
    test_CloseTransforms_mixed();

    if (g_TestFailures == 0) {
        std::cout << "All CloseTransforms tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_TestFailures << " CloseTransforms tests FAILED." << std::endl;
        return 1;
    }
}