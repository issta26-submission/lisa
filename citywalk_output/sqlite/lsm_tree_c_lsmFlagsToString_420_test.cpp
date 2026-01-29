// Unit test suite for the focal method lsmFlagsToString (located in lsm_tree.c)
// This test suite is self-contained (no Google Test) and uses a minimal in-process test harness.
// It relies on the LSM_ bit-flag constants defined in lsmInt.h.
// Test execution is performed from main() with a lightweight reporting mechanism.

#include <lsmInt.h>
#include <iostream>
#include <string.h>
#include <string>


// Import the C header and declare the C function for C linkage
extern "C" {
void lsmFlagsToString(int flags, char *zFlags);
}

// Minimal test harness (non-terminating, continues on failures)
static int g_totalTests = 0;
static int g_passedTests = 0;

// Helper to report a failing test with details
static void reportFailure(const char* file, int line,
                          const std::string& note,
                          const std::string& expected,
                          const std::string& actual) {
    std::cerr << "Test failed at " << file << ":" << line << "\n";
    std::cerr << "  " << note << "\n";
    std::cerr << "  Expected: \"" << expected << "\"\n";
    std::cerr << "  Actual  : \"" << actual << "\"\n";
}

// Core test runner for a single scenario
static bool runTest(int flags, const std::string& expected, const char* note, const char* file, int line) {
    char z[5];
    z[0] = z[1] = z[2] = z[3] = '\0'; // initialize for clarity
    lsmFlagsToString(flags, z);
    // The function guarantees a 4-char result (z[0..3]) plus z[4] = '\0'
    std::string actual(z, z + 4);

    g_totalTests++;
    if (actual == expected) {
        g_passedTests++;
        return true;
    } else {
        reportFailure(file, line, note, expected, actual);
        return false;
    }
}

// Macro to simplify test invocation with source location
#define RUN_TEST(flag, expected, note) runTest((flag), (expected), (note), __FILE__, __LINE__)

int main() {
    // Step 2: Unit Test Generation for lsmFlagsToString
    // Goal: cover all branches of the function, including:
    // - LSM_END_DELETE toggling zFlags[0] ('[' vs ']')
    // - The 3-way switch over (LSM_POINT_DELETE|LSM_INSERT|LSM_SEPARATOR) to produce '.', '-', '+', '^', or '?'.
    // - LSM_SYSTEMKEY toggling zFlags[2] ('*' vs '.')
    // - LSM_START_DELETE toggling zFlags[3] ('[' vs '.')
    //
    // The exact bit values come from lsmInt.h; we rely on those to compose flags.

    // 1) No flags set: all output chars should be '.'
    // zFlags: [0]='.', [1]='.', [2]='.', [3]='\0'? Actually, [3]='.' and terminator
    // Expected string: "...."
    RUN_TEST(0, "....", "No flags: all fields default to '.'");

    // 2) LSM_END_DELETE set only: zFlags[0] should be ']'
    // Expected: "]..."
    RUN_TEST(LSM_END_DELETE, "]...","LSM_END_DELETE only: first char is ']'; others '.'");

    // 3) LSM_POINT_DELETE set only: zFlags[1] should be '-'
    // Expected: ".-.."
    RUN_TEST(LSM_POINT_DELETE, ".-..", "LSM_POINT_DELETE only: second char is '-'");

    // 4) LSM_INSERT set only: zFlags[1] should be '+'
    // Expected: ".+.."
    RUN_TEST(LSM_INSERT, ".+..", "LSM_INSERT only: second char is '+'");

    // 5) LSM_SEPARATOR set only: zFlags[1] should be '^'
    // Expected: ".^.."
    RUN_TEST(LSM_SEPARATOR, ".^..", "LSM_SEPARATOR only: second char is '^'");

    // 6) LSM_SYSTEMKEY set only: zFlags[2] should be '*'
    // Expected: "..*."
    RUN_TEST(LSM_SYSTEMKEY, "..*.", "LSM_SYSTEMKEY only: third char is '*'");

    // 7) LSM_START_DELETE set only: zFlags[3] should be '['
    // Expected: "...["
    RUN_TEST(LSM_START_DELETE, "...[", "LSM_START_DELETE only: fourth char is '['");

    // 8) Two bits set among (POINT_DELETE, INSERT, SEPARATOR) -> default '?'
    // Example: LSM_POINT_DELETE | LSM_INSERT
    // Expected: ".?.." (since two bits are set, the switch default -> '?')
    const int twoBits = LSM_POINT_DELETE | LSM_INSERT;
    RUN_TEST(twoBits, ".?..", "Two bits set among the 3: expected '?' in second position");

    // 9) All optional bits set: end_delete, point_delete, insert, separator, systemkey, start_delete
    // Expected: first ']', second '?', third '*', fourth '['
    const int allBits = LSM_END_DELETE | LSM_POINT_DELETE | LSM_INSERT | LSM_SEPARATOR | LSM_SYSTEMKEY | LSM_START_DELETE;
    RUN_TEST(allBits, "]?*[", "All relevant flags set: first ']', second '?', third '*', fourth '['");

    // 10) Composite: POINT_DELETE, SEPARATOR, SYSTEMKEY (three bits trigger '?' and '*' present)
    // Expected: ".?*." (second char '?', third '*', fourth '.' since START_DELETE not set)
    const int composite = LSM_POINT_DELETE | LSM_SEPARATOR | LSM_SYSTEMKEY;
    RUN_TEST(composite, ".?*.", "Composite three flags: expected '?', '*'")

    // Summary
    std::cout << "LSM lsmFlagsToString test suite completed.\n";
    std::cout << "Passed " << g_passedTests << " out of " << g_totalTests << " tests.\n";

    // Return non-zero if any test failed
    if (g_passedTests != g_totalTests) {
        return 1;
    }
    return 0;
}