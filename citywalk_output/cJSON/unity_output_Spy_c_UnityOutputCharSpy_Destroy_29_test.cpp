/*
 * UnityOutputCharSpy_Destroy Unit Test Suite (C++11)
 *
 * This test suite targets the focal method UnityOutputCharSpy_Destroy located in
 * unity_output_Spy.c. The tests are designed to be compiled with a C++11
 * compiler without using GoogleTest. They rely on the C interfaces provided by
 * the UnityOutputCharSpy_* family.
 *
 * Notes and assumptions:
 * - The internal state (size, buffer) is manipulated by UnityOutputCharSpy_Create,
 *   UnityOutputCharSpy_OutputChar, and UnityOutputCharSpy_Destroy. We access only
 *   the public API (Create, Destroy, OutputChar, Get) to keep tests robust
 *   against internal static state changes.
 * - The tests focus on ensuring that UnityOutputCharSpy_Destroy can be called safely
 *   in various states (before any Create, after Create with some output, and
 *   multiple consecutive calls).
 * - Since we do not rely on a full GTest framework, we implement a lightweight
 *   test harness that reports PASS/FAIL and continues execution (non-terminating
 *   assertions as requested).
 * - All test code is in a single file and uses extern "C" to link with the C
 *   implementation.
 */

// Include C headers/classes with C linkage
#include <cstring>
#include <unity_output_Spy.h>
#include <iostream>
#include <string.h>
#include <unity_fixture.h>
#include <stdio.h>


// Forward declarations of C APIs (in case headers are unavailable or unsuitable for direct use in C++)
extern "C" {
    // Core Unity Output Char Spy API (as observed in the provided focal class deps)
    void UnityOutputCharSpy_Create(int s);
    void UnityOutputCharSpy_Destroy(void);
    void UnityOutputCharSpy_OutputChar(int c);
    const char * UnityOutputCharSpy_Get(void);
    void UnityOutputCharSpy_Enable(int enable);
}

using namespace std;

/*
 * Helper: compare two C-strings safely (handling null pointers)
 */
static bool safe_str_equal(const char* a, const char* b) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;
    return strcmp(a, b) == 0;
}

/*
 * Test 1: Destroy without prior Create
 * Purpose: Ensure UnityOutputCharSpy_Destroy can be called safely even if no
 *          initialization has occurred. This validates that the function can be
 *          invoked in the default/zeroed state without crashing.
 */
bool test_Destroy_without_Create(void)
{
    // No prior initialization
    UnityOutputCharSpy_Destroy();

    // Additional safeguard: calling destroy again should also be safe
    UnityOutputCharSpy_Destroy();

    // If control reaches here, test passes (no crash observed)
    return true;
}

/*
 * Test 2: Create and inspect initial Get() state
 * Purpose: After UnityOutputCharSpy_Create, ensure Get() returns a valid string
 *          pointer (non-null). This checks that the buffer is initialized and
 *          accessible prior to any character output.
 */
bool test_Create_then_Get_initial_state(void)
{
    UnityOutputCharSpy_Create(16); // allocate with a sample size

    const char* s = UnityOutputCharSpy_Get();
    bool ok = (s != nullptr); // should at least return a valid pointer

    UnityOutputCharSpy_Destroy();
    return ok;
}

/*
 * Test 3: Create, output a single character, and verify Get() content
 * Purpose: Validate that OutputChar appends content to the internal buffer and
 *          that Get() reflects the output prior to Destroy().
 */
bool test_Create_Output_Get(void)
{
    UnityOutputCharSpy_Create(16);

    UnityOutputCharSpy_OutputChar('A');
    const char* s = UnityOutputCharSpy_Get();

    bool ok = (s != nullptr) && (strcmp(s, "A") == 0);

    UnityOutputCharSpy_Destroy();
    return ok;
}

/*
 * Test 4: Double destroy and restart safety
 * Purpose: Ensure multiple consecutive calls to Destroy do not crash and the API
 *          remains usable afterwards (e.g., subsequent Create may still function).
 */
bool test_Double_Destroy_and_Restart(void)
{
    UnityOutputCharSpy_Create(8);
    UnityOutputCharSpy_Destroy();
    // Second destroy should be safe
    UnityOutputCharSpy_Destroy();

    // Re-create to ensure API can be used after prior destroy calls
    UnityOutputCharSpy_Create(8);
    const char* s = UnityOutputCharSpy_Get();
    bool ok = (s != nullptr);

    // Optional: write something to verify further usage doesn't crash
    UnityOutputCharSpy_OutputChar('B');
    const char* s2 = UnityOutputCharSpy_Get();
    ok = ok && (s2 != nullptr) && (strcmp(s2, "B") == 0);

    UnityOutputCharSpy_Destroy();
    return ok;
}

/*
 * Test 5: Sequence of multiple characters
 * Purpose: Validate that a sequence of OutputChar calls results in a buffer
 *          content that matches the expected prefix (e.g., "Hello").
 */
bool test_Sequence_of_chars(void)
{
    UnityOutputCharSpy_Create(32);

    // Emit a known sequence
    const char sequence[] = { 'H', 'e', 'l', 'l', 'o', '\0' };
    for (size_t i = 0; sequence[i] != '\0'; ++i) {
        UnityOutputCharSpy_OutputChar(sequence[i]);
    }

    const char* s = UnityOutputCharSpy_Get();
    bool ok = (s != nullptr) && (strncmp(s, "Hello", 5) == 0);

    UnityOutputCharSpy_Destroy();
    return ok;
}

/*
 * Minimal test runner
 * Runs all tests and reports PASS/FAIL with explanatory comments.
 */
int main(void)
{
    int total = 0;
    int passed = 0;

    auto runTest = [&](const string& name, bool result) {
        total++;
        if (result) {
            ++passed;
            cout << "[TEST PASS] " << name << endl;
        } else {
            cout << "[TEST FAIL] " << name << endl;
        }
    };

    // Run tests
    runTest("Destroy_without_Create", test_Destroy_without_Create());
    runTest("Create_then_Get_initial_state", test_Create_then_Get_initial_state());
    runTest("Create_Output_Get", test_Create_Output_Get());
    runTest("Double_Destroy_and_Restart", test_Double_Destroy_and_Restart());
    runTest("Sequence_of_chars", test_Sequence_of_chars());

    // Summary
    cout << "Summary: " << passed << " / " << total << " tests passed." << endl;
    return (passed == total) ? 0 : 1;
}