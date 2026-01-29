// test_pngunknown_read_callback.cpp
// A carefully structured, dependency-aware unit test harness for the focal method
// read_callback(png_structp pp, png_unknown_chunkp pc) found in pngunknown.c.
// This test harness uses a lightweight, self-contained mock environment to drive
// the logic paths of read_callback. It is designed to be compatible with C++11
// (no GoogleTest) and to be drop-in friendly for projects that compile pngunknown.c
// against libpng. The tests focus on exercising true/false branches, default paths,
// and side effects captured via a mock 'display' structure.
//
// Important notes:
// - The actual libpng symbols and types (png_structp, png_unknown_chunkp, etc.) are
//   normally provided by <png.h>. In this harness we provide minimal stubs that are
//   sufficient to drive read_callback in isolation. When integrating in a real project,
//   prefer linking against the project’s existing libpng setup and adjust the mocks
//   accordingly.
// - This file is intended to be used as a dedicated test source. It should be compiled
//   together with pngunknown.c (or with the project’s build that provides read_callback).
// - The test suite uses plain asserts for non-terminating tests and prints concise
//   results. It avoids terminating the process on test failure, aligning with the
//   requirement to use non-terminating assertions in a broader test framework.

#include <cassert>
#include <setjmp.h>
#include <vector>
#include <string.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>


// Domain knowledge: provide minimal, self-contained C-style typedefs to mirror the
// public API surface that the focal function expects. This allows linking against
// the real implementation when available, while enabling isolated tests here.

extern "C" {

// Typedefs mirroring a subset of libpng-like types
typedef unsigned char png_byte;
typedef unsigned int  png_uint_32;

// Forward-declare opaque pointer types used by read_callback
typedef struct png_struct_s*       png_structp;
typedef struct png_unknown_chunk_s*  png_unknown_chunkp;

// Minimal struct shapes to enable test scaffolding
struct png_struct_s {
    void* user_chunk_ptr; // pointer provided via png_get_user_chunk_ptr(pp)
};

struct png_unknown_chunk_s {
    png_byte* name;
    png_uint_32 location;
};

// Forward declarations of functions/variables that read_callback relies on.
// In a real environment these would come from png.h and the project, but here we
// provide simple, test-scoped definitions that can be overridden by the real
// project when integrated.

struct display {
    const char* file;
    const char* test;
    int keep;            // user-defined preference for unknown - mirrors 'd->keep'
    int before_IDAT;
    int after_IDAT;
};

// The exact prototypes used by read_callback in the focal file
int read_callback(png_structp pp, png_unknown_chunkp pc);

// Helper functions that the focal code expects to resolve (stubs)
int findb(const png_byte *name); // returns chunk index >=0 or -1 if not found

// A very small, test-controlled chunk_info table. The real app has more entries.
// We expose a simple, mutable placeholder to drive different branches.
struct { int keep; png_uint_32 flag; int unknown; } chunk_info[4];

// Constants used by the control flow in read_callback
const int PNG_HANDLE_CHUNK_AS_DEFAULT = 0;
const int PNG_HANDLE_CHUNK_NEVER        = 1;
const int PNG_HANDLE_CHUNK_IF_SAFE        = 2;
const int PNG_HANDLE_CHUNK_ALWAYS         = 3;

const int PNG_AFTER_IDAT = 0x01; // location flag

// Simulated SYS/task lifetime artifacts for test observation
static int test_exit_called = 0;

// A simple display_exit that records invocation without terminating tests
void display_exit(display* /*d*/) {
    test_exit_called = 1;
}

// A mock for png_get_user_chunk_ptr(pp) used by read_callback.
// In a real test, libpng would provide this. We emulate it here.
void* png_get_user_chunk_ptr(png_structp pp) {
    return pp ? pp->user_chunk_ptr : nullptr;
}

// A tiny, deterministic implementation for findb to drive known/unknown chunks.
// For tests, we feed known names to map to indices in chunk_info, else -1.
int findb(const png_byte *name) {
    if (name == nullptr) return -1;
    // Simple mapping: "KNOWN0" -> 0, "KNOWN1" -> 1
    if (std::strcmp(reinterpret_cast<const char*>(name), "KNOWN0") == 0) return 0;
    if (std::strcmp(reinterpret_cast<const char*>(name), "KNOWN1") == 0) return 1;
    return -1;
}

// Public declarations required by read_callback (provide only what is necessary for tests)
} // extern "C"

// Helper to reset test state before each run
void reset_test_state() {
    test_exit_called = 0;
    // Reset chunk_info defaults for isolation
    for (int i = 0; i < 4; ++i) {
        chunk_info[i].keep = PNG_HANDLE_CHUNK_AS_DEFAULT;
        chunk_info[i].flag = 0;
        chunk_info[i].unknown = 0;
    }
}

// Test harness helpers
bool test_passed = true;
void report(const std::string& name, bool passed) {
    std::cout << (passed ? "[PASS] " : "[FAIL] ") << name << "\n";
    test_passed = test_passed && passed;
}

// Factory to create a minimal png_structp with a readable user_chunk_ptr
png_structp make_pp_with_display(display* d) {
    png_structp pp = new struct png_struct_s;
    pp->user_chunk_ptr = static_cast<void*>(d);
    return pp;
}

// Test 1: Unknown chunk (-1) with d->keep = PNG_HANDLE_CHUNK_IF_SAFE -> expect discard = 0
void test_read_callback_unknown_keep_if_safe_discard_zero() {
    reset_test_state();

    // Prepare display with 'keep' = IF_SAFE
    static display d;
    d.file = "testfile.c";
    d.test = "test_unknown_if_safe";
    d.keep = PNG_HANDLE_CHUNK_IF_SAFE;
    d.before_IDAT = 0;
    d.after_IDAT = 0;

    png_structp pp = make_pp_with_display(&d);
    // pc->name maps to an unknown chunk (not in findb)
    png_byte unknown_name[] = "UNKNOWN";
    png_unknown_chunkp pc = new struct png_unknown_chunk_s;
    pc->name = unknown_name;
    pc->location = 0; // before IDAT

    // Ensure tests use the fallback path: chunk < 0 -> keep = d->keep
    int ret = read_callback(pp, pc);

    // Expected: discard = 0, because IF_SAFE yields 0
    bool ok = (ret == 0) && (d.before_IDAT == 0) && (d.after_IDAT == 0) && (test_exit_called == 0);
    report("Unknown chunk with IF_SAFE keeps the chunk (discard == 0)", ok);

    delete pc;
    delete pp;
}

// Test 2: Unknown chunk (-1) with d->keep = PNG_HANDLE_CHUNK_NEVER -> expect discard = 1
void test_read_callback_unknown_never_discard_one() {
    reset_test_state();

    static display d;
    d.file = "testfile.c";
    d.test = "test_unknown_never";
    d.keep = PNG_HANDLE_CHUNK_NEVER;
    d.before_IDAT = 0;
    d.after_IDAT = 0;

    png_structp pp = make_pp_with_display(&d);
    png_byte unknown_name[] = "UNKNOWN2";
    png_unknown_chunkp pc = new struct png_unknown_chunk_s;
    pc->name = unknown_name;
    pc->location = 0;

    chunk_info[0].keep = PNG_HANDLE_CHUNK_AS_DEFAULT; // not used for unknown
    int ret = read_callback(pp, pc);

    // Unknown path: switch uses keep = d->keep => NEVER -> discard = 1
    bool ok = (ret == 1) && (d.before_IDAT == 0) && (d.after_IDAT == 0) && (test_exit_called == 0);
    report("Unknown chunk with NEVER discards (discard == 1)", ok);

    delete pc;
    delete pp;
}

// Test 3: Known chunk with unknown=true and default AS_DEFAULT -> should adopt d->keep and set IDAT flags
void test_read_callback_known_unknown_default_adopts_keep_and_sets_flag_before() {
    reset_test_state();

    static display d;
    d.file = "testfile.c";
    d.test = "test_known_unknown_default_keeps_with_keep";
    d.keep = PNG_HANDLE_CHUNK_ALWAYS; // choose ALWAYS for an observable 'discard' decision
    d.before_IDAT = 0;
    d.after_IDAT = 0;

    // Prepare chunk_info for index 0
    chunk_info[0].keep = PNG_HANDLE_CHUNK_AS_DEFAULT;
    chunk_info[0].flag = 0x04; // a fake flag
    chunk_info[0].unknown = 1; // unknown flag true

    png_structp pp = make_pp_with_display(&d);
    png_byte known_name[] = "KNOWN0"; // maps to chunk index 0
    png_unknown_chunkp pc = new struct png_unknown_chunk_s;
    pc->name = known_name;
    pc->location = 0; // before IDAT

    int ret = read_callback(pp, pc);

    // Since chunk is known (index 0) and keep == AS_DEFAULT with unknown==true,
    // keep should be d->keep (ALWAYS) per the code, but Because it's a default branch,
    // it will be treated similarly to NEVER -> discard = 1 if not overridden.
    // However, since d->keep is ALWAYS and the code path for keep==AS_DEFAULT with unknown==true
    // sets keep = d->keep, the value is ALWAYS, hence discard = 0 (IF_SAFE/ALWAYS -> discard = 0).
    // We'll assert that the flag got set (before_IDAT) because discard == 0.
    bool before_set = (d.before_IDAT & chunk_info[0].flag) != 0;
    bool ok = (ret == 0) && before_set && (test_exit_called == 0);
    report("Known+unknown with AS_DEFAULT adopts keep and sets BEFORE_IDAT flag", ok);

    delete pc;
    delete pp;
}

// Test 4: Known chunk with unknown=false -> AS_DEFAULT should map to NEVER, discard = 1
void test_read_callback_known_unknown_false_maps_to_never_discard() {
    reset_test_state();

    static display d;
    d.file = "testfile.c";
    d.test = "test_known_unknown_false_never_discard";
    d.keep = PNG_HANDLE_CHUNK_AS_DEFAULT;
    d.before_IDAT = 0;
    d.after_IDAT = 0;

    chunk_info[0].keep = PNG_HANDLE_CHUNK_AS_DEFAULT;
    chunk_info[0].flag = 0x08;
    chunk_info[0].unknown = 0; // not unknown

    png_structp pp = make_pp_with_display(&d);
    png_byte known_name[] = "KNOWN0";
    png_unknown_chunkp pc = new struct png_unknown_chunk_s;
    pc->name = known_name;
    pc->location = 0;

    int ret = read_callback(pp, pc);

    bool ok = (ret == 1) && (d.before_IDAT == 0) && (d.after_IDAT == 0) && (test_exit_called == 0);
    report("Known+not-unknown maps to NEVER -> discard", ok);

    delete pc;
    delete pp;
}

// Test 5: Invalid 'default' option triggers default path and calls display_exit
void test_read_callback_default_path_triggers_display_exit() {
    reset_test_state();

    static display d;
    d.file = "testfile.c";
    d.test = "test_invalid_default_calls_exit";
    d.keep = 9999; // invalid (not a defined PNG_HANDLE_ value)
    d.before_IDAT = 0;
    d.after_IDAT = 0;

    chunk_info[0].keep = 999; // invalid as well
    chunk_info[0].flag = 0;
    chunk_info[0].unknown = 0;

    png_structp pp = make_pp_with_display(&d);
    png_byte known_name[] = "KNOWN1";
    png_unknown_chunkp pc = new struct png_unknown_chunk_s;
    pc->name = known_name;
    pc->location = 0;

    int ret = read_callback(pp, pc);

    // Default case should call display_exit and then set discard=1
    bool ok = (ret == 1) && (test_exit_called != 0);
    report("Invalid default option triggers display_exit and discard", ok);

    delete pc;
    delete pp;
}

// Test 6: Group test for AFTER_IDAT behavior updates after_IDAT when discard == 0
void test_read_callback_updates_afterIDAT_when_discard_false() {
    reset_test_state();

    static display d;
    d.file = "testfile.c";
    d.test = "test_updates_after_IDAT";
    d.keep = PNG_HANDLE_CHUNK_IF_SAFE;
    d.before_IDAT = 0;
    d.after_IDAT = 0;

    // Known chunk with safe handling
    chunk_info[0].keep = PNG_HANDLE_CHUNK_IF_SAFE;
    chunk_info[0].flag = 0x20;
    chunk_info[0].unknown = 0;

    png_structp pp = make_pp_with_display(&d);
    png_byte known_name[] = "KNOWN0"; // index 0
    png_unknown_chunkp pc = new struct png_unknown_chunk_s;
    pc->name = known_name;
    pc->location = PNG_AFTER_IDAT; // after IDAT

    int ret = read_callback(pp, pc);

    bool after_set = (d.after_IDAT & chunk_info[0].flag) != 0;
    bool ok = (ret == 0) && after_set && (d.before_IDAT == 0);
    report("After_IDAT path updates after_IDAT flag when discard == 0", ok);

    delete pc;
    delete pp;
}

// Test 7: Ensure default path with unknown chunk triggers both exit and discard
void test_read_callback_default_path_calls_exit_and_discards() {
    reset_test_state();

    static display d;
    d.file = "testfile.c";
    d.test = "test_default_calls_exit_and_discards";
    d.keep = 7; // arbitrary non-handled value to hit default
    d.before_IDAT = 0;
    d.after_IDAT = 0;

    // Put chunk knownness to ensure default path triggers
    chunk_info[0].keep = 999; // invalid -> default path
    chunk_info[0].flag = 0x01;
    chunk_info[0].unknown = 1;

    png_structp pp = make_pp_with_display(&d);
    png_byte known_name[] = "KNOWN0";
    png_unknown_chunkp pc = new struct png_unknown_chunk_s;
    pc->name = known_name;
    pc->location = 0;

    int ret = read_callback(pp, pc);

    bool ok = (ret == 1) && (test_exit_called != 0);
    report("Default path with unknown option prints error and discards", ok);

    delete pc;
    delete pp;
}

// Entry point: run all tests
int main(void) {
    // Note: The tests rely on the actual pngunknown.c linkage. If integrating
    // into your project and libpng is linked, ensure to compile this test with the
    // project sources. The test harness uses simple asserts and a minimal test runner.

    test_read_callback_unknown_keep_if_safe_discard_zero();
    test_read_callback_unknown_never_discard_one();
    test_read_callback_known_unknown_default_adopts_keep_and_sets_flag_before();
    test_read_callback_known_unknown_false_maps_to_never_discard();
    test_read_callback_default_path_triggers_display_exit();
    test_read_callback_updates_afterIDAT_when_discard_false();
    test_read_callback_default_path_calls_exit_and_discards();

    // Final report
    if (test_passed) {
        std::cout << "[ALL TESTS PASSED]" << std::endl;
        return 0;
    } else {
        std::cout << "[SOME TESTS FAILED]" << std::endl;
        return 1;
    }
}