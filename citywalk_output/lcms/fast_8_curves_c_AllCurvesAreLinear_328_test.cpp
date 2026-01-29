#include <cstdio>
#include <fast_float_internal.h>


// Minimal stand-in definitions to compile and test the focal function.
// In the real project these would come from the library headers.

typedef int cmsBool;
#define TRUE 1
#define FALSE 0

struct Curves8Data {
    unsigned char Curves[3][256];
};

// Focal method under test (replicated from provided snippet for unit testing)
cmsBool AllCurvesAreLinear(Curves8Data* data)
{
{
    int i, j;
    for (i=0; i < 3; i++) {
        for (j = 0; j < 256; j++) {
            if (data ->Curves[i][j] != j) return FALSE;
        }
    }
    return TRUE;
}
}

// Helper to initialize curves to a linear identity mapping: Curves[i][j] == j
static void fill_identity(Curves8Data* d) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 256; ++j) {
            d->Curves[i][j] = static_cast<unsigned char>(j);
        }
    }
}

// Test 1: AllCurvesAreLinear should return TRUE when all curves are identity-mapped (baseline success)
static bool test_AllMatch() {
    Curves8Data d;
    fill_identity(&d);
    cmsBool res = AllCurvesAreLinear(&d);
    return res == TRUE;
}

// Test 2: AllCurvesAreLinear should return FALSE when the first curve is not identity at j=0
// This tests the function's ability to short-circuit on the first mismatch (early exit)
static bool test_MismatchFirst() {
    Curves8Data d;
    fill_identity(&d);
    d.Curves[0][0] = 1; // break identity at the very first check
    cmsBool res = AllCurvesAreLinear(&d);
    return res == FALSE;
}

// Test 3: AllCurvesAreLinear should return FALSE when a mismatch occurs in the second curve
// Validates traversal to the second curve and detection of a mismatch after some successful checks
static bool test_MismatchSecond() {
    Curves8Data d;
    fill_identity(&d);
    d.Curves[1][123] = static_cast<unsigned char>(124); // mismatch at j = 123
    cmsBool res = AllCurvesAreLinear(&d);
    return res == FALSE;
}

// Test 4: AllCurvesAreLinear should return FALSE when a mismatch occurs in the third curve
static bool test_MismatchThird() {
    Curves8Data d;
    fill_identity(&d);
    d.Curves[2][0] = static_cast<unsigned char>(2); // mismatch at j = 0
    cmsBool res = AllCurvesAreLinear(&d);
    return res == FALSE;
}

int main() {
    int total = 0;
    int passed = 0;

    auto run_test = [&](const char* name, bool result) {
        total++;
        if (result) {
            printf("PASS: %s\n", name);
            ++passed;
            return true;
        } else {
            printf("FAIL: %s\n", name);
            return false;
        }
    };

    // Execute tests with explanatory comments above each as per domain knowledge.
    run_test("test_AllMatch (baseline identity across all curves)", test_AllMatch());
    run_test("test_MismatchFirst (mismatch in first curve triggers FALSE)", test_MismatchFirst());
    run_test("test_MismatchSecond (mismatch in second curve triggers FALSE)", test_MismatchSecond());
    run_test("test_MismatchThird (mismatch in third curve triggers FALSE)", test_MismatchThird());

    printf("Summary: %d/%d tests passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}