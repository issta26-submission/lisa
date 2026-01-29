/*
  Test Suite for: png_set_expand_16 (pngrtran.c)

  Purpose:
  - Validate the behavior of png_set_expand_16 by driving the focal function
    with controlled conditions and inspecting side effects on the png_ptr
    structure and the internal debug logging.

  Assumptions / Environment:
  - The project provides a libpng-like internal header pngpriv.h which defines
    png_struct and the member 'transformations', as well as the flags
    PNG_EXPAND_16, PNG_EXPAND, PNG_EXPAND_tRNS.
  - The function png_rtran_ok(png_structrp, int) and png_debug(int, const char*)
    are available (we override them in this test to control behavior and to
    capture debug output). The override uses C linkage (extern "C") to match
    the C-callable API used by pngrtran.c.
  - We do not rely on any external testing framework (as requested). A small
    in-house test harness with non-terminating assertions is provided.
  - This file compiles with C++11 (as required) and links with the project sources
    (including pngrtran.c). The test uses only the C++ standard library for the
    harness and relies on the provided library headers for the png types.

  How to run (example):
  - Build the test together with the project sources (e.g., g++ -std=c++11 -I. test_png_set_expand_16.cpp pngrtran.c -o test_png_set_expand_16)
  - Run ./test_png_set_expand_16
  - The harness prints pass/fail messages and a final summary.
*/

/* Include project-provided internal header (libpng-esque). We rely on
   the project to provide this header in its include path. */
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>



/* Test harness state:
   - g_png_debug_logs stores messages issued by png_debug during the call.
   - g_force_rtran_ok_zero, when nonzero, forces png_rtran_ok to return 0
     (i.e., "not OK"), to exercise the false-branch of png_set_expand_16.
*/
static std::vector<std::string> g_png_debug_logs;
static int g_force_rtran_ok_zero = 0;

/* Forward declaration of the focal function (to call from tests).
   Provided by the project (pngrtran.c) and linked in during build. */
extern "C" void png_set_expand_16(png_structrp png_ptr);

/* Override hooks (with C linkage) to control behavior of dependencies
   used by the focal method.

   - png_rtran_ok: return 1 by default (OK) unless test explicitly forces 0.
   - png_debug: capture debug messages for verification. */
extern "C" int png_rtran_ok(png_structrp png_ptr, int need_IHDR);
extern "C" void png_debug(int level, const char* msg);

/* Implementation of the overrides. These are provided in this translation
   unit to control the behavior of png_set_expand_16 during tests. */
extern "C" int png_rtran_ok(png_structrp png_ptr, int need_IHDR)
{
    (void)png_ptr;
    (void)need_IHDR;
    // If a test sets the flag, force a false return; otherwise default to true.
    return (g_force_rtran_ok_zero != 0) ? 0 : 1;
}

extern "C" void png_debug(int level, const char* msg)
{
    (void)level;
    if (msg) {
        g_png_debug_logs.emplace_back(msg);
    }
}

/* Helper macro for non-terminating assertions. Keeps test execution flowing. */
static int g_test_failures = 0;
#define TEST_FAIL(msg) do { std::cerr << "TEST FAILED: " << (msg) << "\n"; ++g_test_failures; } while(0)
#define TEST_OK(cond, msg) do { if(!(cond)) { TEST_FAIL(msg); } } while(0)

/* Helper to create a fresh png_struct and initialize its fields if needed.
   We rely on the internal definition from pngpriv.h which provides
   the 'transformations' member used by png_set_expand_16. */
static png_struct* make_png_struct_with_transformations(int initial)
{
    png_struct* s = new png_struct();
    // Initialize to a known state
    s->transformations = initial;
    return s;
}

/* Test 1: When png_rtran_ok returns true, png_set_expand_16 should set the
   three expansion-related transformation flags (EXPAND_16, EXPAND, EXPAND_tRNS)
   using bitwise OR, without clearing other bits. */
void test_expand_16_sets_flags_when_rtran_ok()
{
    // Prepare
    g_png_debug_logs.clear();
    g_force_rtran_ok_zero = 0; // Ensure png_rtran_ok returns 1
    png_struct* ptr = make_png_struct_with_transformations(0);

    // Act
    png_set_expand_16(ptr);

    // Expected: set all three flags
    const int expected_flags = PNG_EXPAND_16 | PNG_EXPAND | PNG_EXPAND_tRNS;

    // Assert
    TEST_OK((ptr->transformations & expected_flags) == expected_flags,
            "png_set_expand_16 should set PNG_EXPAND_16, PNG_EXPAND, and PNG_EXPAND_tRNS when png_rtran_ok returns true");

    // Also verify that debug message was emitted
    bool saw_debug = false;
    for (const auto& m : g_png_debug_logs) {
        if (m.find("in png_set_expand_16") != std::string::npos) {
            saw_debug = true;
            break;
        }
    }
    TEST_OK(saw_debug, "Expected debug message 'in png_set_expand_16' to be emitted");

    delete ptr;
}

/* Test 2: When png_rtran_ok returns false, png_set_expand_16 should do nothing
   to the transformations (i.e., preserve existing bits). Also should emit
   the debug message. */
void test_expand_16_no_change_when_rtran_not_ok()
{
    // Prepare
    g_png_debug_logs.clear();
    g_force_rtran_ok_zero = 1; // Force png_rtran_ok to return 0
    png_struct* ptr = make_png_struct_with_transformations(0x10); // some existing unrelated bit

    // Act
    png_set_expand_16(ptr);

    // Assert: transformations should remain unchanged
    TEST_OK((ptr->transformations == 0x10),
            "png_set_expand_16 should NOT modify transformations when png_rtran_ok returns false");

    // Also verify that debug message was emitted
    bool saw_debug = false;
    for (const auto& m : g_png_debug_logs) {
        if (m.find("in png_set_expand_16") != std::string::npos) {
            saw_debug = true;
            break;
        }
    }
    TEST_OK(saw_debug, "Expected debug message 'in png_set_expand_16' to be emitted even when not expanding");

    delete ptr;
    // Reset for any subsequent tests
    g_force_rtran_ok_zero = 0;
}

/* Test 3: Idempotence / existing flags are preserved and only the intended
   bits are added when rtran_ok is true. This ensures no unintended side effects
   on unrelated bits in transformations. */
void test_expand_16_preserves_other_bits_when_adding()
{
    // Prepare
    g_png_debug_logs.clear();
    g_force_rtran_ok_zero = 0; // OK
    // Start with a pre-existing, unrelated bit (e.g., 0x20)
    png_struct* ptr = make_png_struct_with_transformations(0x20);

    // Act
    png_set_expand_16(ptr);

    // Assert: LOW-level bit PIC checks
    const int expected_flags = PNG_EXPAND_16 | PNG_EXPAND | PNG_EXPAND_tRNS;
    TEST_OK((ptr->transformations & expected_flags) == expected_flags,
            "png_set_expand_16 should set expansion flags in presence of existing unrelated bits");

    // The unrelated bit (0x20) should still be set
    TEST_OK((ptr->transformations & 0x20) != 0,
            "Existing unrelated bit 0x20 should remain set after png_set_expand_16");

    // Debug
    bool saw_debug = false;
    for (const auto& m : g_png_debug_logs) {
        if (m.find("in png_set_expand_16") != std::string::npos) {
            saw_debug = true;
            break;
        }
    }
    TEST_OK(saw_debug, "Expected debug message 'in png_set_expand_16' to be emitted");

    delete ptr;
    g_force_rtran_ok_zero = 0;
}

/* Main: Run all tests and print a summary. */
int main()
{
    test_expand_16_sets_flags_when_rtran_ok();
    test_expand_16_no_change_when_rtran_not_ok();
    test_expand_16_preserves_other_bits_when_adding();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) FAILED\n";
        return g_test_failures;
    }
}