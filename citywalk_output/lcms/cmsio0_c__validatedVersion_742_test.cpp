/*
   Test Suite: _validatedVersion in cmsio0.c

   Notes:
   - The focal function uses a 32-bit word (DWord) interpreted as 4 little-endian bytes.
   - Branch coverage targets:
     a) First byte clamp: if (byte0 > 0x09) -> clamp to 0x09
     b) Second byte upper nibble clamp: if ( (byte1 & 0xF0) > 0x90 ) clamp upper nibble to 0x90
     c) Second byte lower nibble clamp: if ( (byte1 & 0x0F) > 0x09 ) clamp lower nibble to 0x09
     d) After processing, bytes 2 and 3 are zeroed.
   - We build a small non-terminating test harness (no GTest) that calls the C function
     via extern "C" and validates results with reporting (pass/fail) while continuing
     execution to maximize coverage.
   - This harness uses lightweight, non-intrusive logging and prints a final summary.
*/

#include <lcms2_internal.h>
#include <cstdio>
#include <cstdint>


// Minimal typedefs to declare the external C function.
// We assume cmsUInt32Number is a 32-bit unsigned type in the original code.
typedef unsigned int cmsUInt32Number;
typedef unsigned char cmsUInt8Number;

// Forward declaration of the focal C function.
// It uses C linkage, so we declare it as extern "C".
extern "C" cmsUInt32Number _validatedVersion(cmsUInt32Number DWord);

// Lightweight test harness
static int g_failures = 0;

// Log a passing test
static void log_pass(const char* test_name) {
    printf("PASS: %s\n", test_name);
}

// Log a failing test with details
static void log_fail(const char* test_name, uint32_t input, uint32_t expected, uint32_t actual) {
    printf("FAIL: %s | input=0x%08X, expected=0x%08X, actual=0x%08X\n", test_name, input, expected, actual);
    ++g_failures;
}

// Run a single test case
static bool run_one(const char* name, uint32_t input, uint32_t expected) {
    cmsUInt32Number out = _validatedVersion(input);
    if (out != expected) {
        log_fail(name, input, expected, static_cast<uint32_t>(out));
        return false;
    } else {
        log_pass(name);
        return true;
    }
}

// Entry point
int main() {
    printf("Starting tests for cmsUInt32Number _validatedVersion...\n\n");

    // Test 1: B0 > 0x09 triggers first branch; B1 chosen to avoid interference from temp1/temp2 surprises.
    // Expected: B0 becomes 0x09, B1 stays as calculated from input, B2=B3=0.
    // Input bytes (little-endian): b0=0x78, b1=0x56, b2=0x34, b3=0x12
    // Value: 0x12345678 -> expected 0x00005609
    run_one("Test 1: B0 clamp (0x78->0x09) with benign B1", 0x12345678, 0x00005609);

    // Test 2: temp1 > 0x90U reached by second byte upper nibble (e.g., 0xA0)
    // We also keep B0 <= 0x09 to isolate the second-byte behavior (no clamp on B0)
    // Input bytes: b0=0x05, b1=0xA0, b2=0x00, b3=0x00
    // DWord: 0xA005 -> after: b0=0x05, b1=0x90, b2=0x00, b3=0x00 => 0x00009005
    run_one("Test 2: temp1 clamp triggered (0xA0 upper nibble)", 0xA005, 0x00009005);

    // Test 3: temp2 > 0x09U triggered (lower nibble > 0x09) with no upper-nibble clamp
    // Use b1 = 0x0F (upper nibble 0x00 -> temp1=0x00; lower nibble 0x0F -> clamp to 0x09)
    // Input: b0=0x01, b1=0x0F, b2=0x00, b3=0x00 -> DWord 0x00000F01
    // Expected: b0=0x01, b1=0x09, b2=0x00, b3=0x00 -> 0x00000901
    run_one("Test 3: temp2 clamp triggered (lower nibble > 0x09)", 0x00000F01, 0x00000901);

    // Test 4: False branches for both temp1/temp2 while B0 is within range
    // Input: b0=0x01 (<=0x09), b1=0x56 (upper nibble 0x50 <= 0x90; lower nibble 0x06 <= 0x09)
    // DWord: 0x00005601
    // Expected: no changes except B2/B3 zeroed (already zero) -> 0x00005601
    run_one("Test 4: Both predicates false (no clamp on B0, B1) with benign B1", 0x00005601, 0x00005601);

    // Test 5: Clamp B0 and B1 together (b0 > 0x09 and upper nibble triggers clamp; also test lower nibble edge)
    // Input: b0=0xFF, b1=0x02 (upper nibble 0x00 -> temp1 unchanged), lower nibble 0x02 <= 0x09
    // DWord: 0x02FF -> after: b0=0x09, b1=0x02, -> 0x0209
    run_one("Test 5: B0 clamp with benign B1 lower nibble", 0x000002FF, 0x00000209);

    printf("\nTest run complete. Total failures: %d\n", g_failures);
    return (g_failures == 0) ? 0 : 1;
}