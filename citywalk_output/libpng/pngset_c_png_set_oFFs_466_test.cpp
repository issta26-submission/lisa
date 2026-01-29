/* 
 * Minimal stub header to allow unit testing of png_set_oFFs in a self-contained
 * environment without the full libpng sources. This header provides only the
 * types and macros used by the focal function and its immediate dependencies.
 * It is intended to be included by the test translation unit together with
 * the real pngset.c so that the test can link against the function under test.
 *
 * NOTE: This header is intentionally small and only covers what's needed for the
 * test harness. It is not a drop-in replacement for the real libpng headers.
 */

#ifndef PNGPRIV_H
#define PNGPRIV_H

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


#ifdef __cplusplus
extern "C" {
#endif

// Basic type aliases used by png_set_oFFs and related code
typedef unsigned char png_byte;
typedef int png_int_32;

/* Forward-declare the core png structures used in the API.
   We provide a minimal layout for png_info so tests can verify field changes. */
struct png_struct_def { int dummy; }; // opaque, only for pointer type compatibility
typedef struct png_struct_def png_struct;
typedef struct png_struct_def* png_structrp;        // non-const pointer
typedef const struct png_struct_def* png_const_structrp; // const pointer

/* Simple info structure with fields accessed by png_set_oFFs.
   We expose the exact layout needed by the focal function to allow tests to
   observe side effects. */
struct png_info_def {
    int x_offset;
    int y_offset;
    png_byte offset_unit_type;
    unsigned int valid;
};
typedef struct png_info_def png_info;  // real type used by tests
typedef png_info* png_inforp;           // info pointer type used by API

// PNG_INFO_oFFs flag used to mark oFFs data as valid
#define PNG_INFO_oFFs 0x01

// Prototype for debug helper used by the focal method
void png_debug1(int level, const char* format, const char* arg);

#ifdef __cplusplus
}
#endif

#endif // PNGPRIV_H

/*
 * Test launcher and test cases follow. The tests are designed to be run in
 * a C++11 environment without Google Test. Instead, we implement a small
 * light-weight assertion mechanism suitable for C++ and non-terminating
 * assertions as requested.
 */

// Test file: test_png_set_oFFs.cpp
// This file contains the test harness dedicated to png_set_oFFs.
// It uses the stub types from pngpriv.h above to mimic the real environment.


// Bring in the minimal private header to define required types

extern "C" {
    // Provide a C linkage for the debug function so that the C-compiled pngset.c
    // can link against it without name mangling issues.
    void png_debug1(int level, const char* format, const char* arg) {
        // For testing, we do not need to log deeply; but we expose a small
        // side-channel to observe that the function is invoked if desired.
        // We push a simple entry to a global log (defined in test harness below).
        extern std::vector<std::string> g_png_debug_log;
        // Record only the arg to indicate the function was reached.
        g_png_debug_log.push_back(std::string("DEBUG_oFFs: ") + (arg ? arg : "<null>"));
        (void)level; // keep the signature consistent
        (void)format;
    }
}

// Global log for observing debug invocations from png_debug1
std::vector<std::string> g_png_debug_log;

// Simple non-terminating assertion mechanism
static std::vector<std::string> g_failures;
#define TEST_ASSERT(cond, msg) do { if(!(cond)) { g_failures.push_back(std::string(msg)); } } while(0)

//
// Test helpers and test cases
//

static void reset_log_and_failures() {
    g_png_debug_log.clear();
    g_failures.clear();
}

// Test 1: When png_ptr is NULL, the function should return immediately and not modify info_ptr
static bool test_null_png_ptr() {
    reset_log_and_failures();

    png_info info;
    // Initialize with non-zero values to verify no mutation occurs
    info.x_offset = 5;
    info.y_offset = -7;
    info.offset_unit_type = 2;
    info.valid = 0x00;

    // Call with NULL png_ptr
    png_set_oFFs(nullptr, &info, 123, 456, 3);

    // Expect no changes to info_ptr
    bool ok = (info.x_offset == 5) && (info.y_offset == -7) &&
              (info.offset_unit_type == 2) && (info.valid == 0x00);
    if(!ok) {
        g_failures.push_back("test_null_png_ptr: info_ptr was modified when png_ptr is NULL");
    }

    // Optional: ensure no debug log produced a meaningful entry (not strictly required)
    (void)g_png_debug_log;

    return g_failures.empty();
}

// Test 2: When info_ptr is NULL, the function should return immediately and not modify png_ptr
static bool test_null_info_ptr() {
    reset_log_and_failures();

    // Create a dummy non-null png_ptr
    static struct png_struct_def dummy;
    png_structrp dummy_ptr = &dummy;

    // Call with NULL info_ptr
    png_set_oFFs(reinterpret_cast<png_const_structrp>(dummy_ptr), nullptr, 1, 2, 3);

    // Since info_ptr is NULL, there is nothing to verify; just ensure no crash and no exception.
    // We'll verify that nothing in the global log was modified (if the implementation logs the entry)
    // Here we simply assume no crash implies success.
    // In addition, we can simulate by ensuring the log contains the expected debug entry only if it was invoked.
    // Since our stub logs on valid inputs, the absence of a crash is the positive signal.
    return g_failures.empty();
}

// Test 3: Normal update path: fields should be updated and oFFs flag should be set
static bool test_set_fields_updates_and_flag() {
    reset_log_and_failures();

    png_info info;
    info.x_offset = 0;
    info.y_offset = 0;
    info.offset_unit_type = 0;
    info.valid = 0;

    // Prepare a non-null png_ptr
    static struct png_struct_def dummy;
    png_structrp dummy_ptr = &dummy;

    // Call with sample values
    png_set_oFFs(reinterpret_cast<png_const_structrp>(dummy_ptr), &info, 100, -25, 3);

    bool ok = (info.x_offset == 100) &&
              (info.y_offset == -25) &&
              (info.offset_unit_type == static_cast<png_byte>(3)) &&
              ((info.valid & PNG_INFO_oFFs) != 0);

    if(!ok) {
        g_failures.push_back("test_set_fields_updates_and_flag: fields not set correctly or valid flag missing");
    }

    // Also, ensure that the debug path was invoked (optional observable)
    // If the log has at least one entry mentioning "oFFs", it's a good indicator.
    bool debug_seen = false;
    for (const auto &s : g_png_debug_log) {
        if (s.find("oFFs") != std::string::npos) {
            debug_seen = true;
            break;
        }
    }
    // Even if not found, the test should still pass as the primary behavior is field updates;
    // but we prefer to observe the path. We'll mark a soft fail if debug wasn't seen.
    if(!debug_seen) {
        g_failures.push_back("test_set_fields_updates_and_flag: expected png_debug1 to be invoked with 'oFFs'");
    }

    return g_failures.empty();
}

// Test 4: Unit type casting should wrap around to png_byte range (mod 256 behavior)
static bool test_unit_type_cast_wrapping() {
    reset_log_and_failures();

    png_info info;
    info.x_offset = 0;
    info.y_offset = 0;
    info.offset_unit_type = 0;
    info.valid = 0;

    static struct png_struct_def dummy;
    png_structrp dummy_ptr = &dummy;

    // Use a value that exceeds 255 to ensure truncation occurs via (png_byte)
    unsigned int unit_type = 300; // 0x012C
    png_set_oFFs(reinterpret_cast<png_const_structrp>(dummy_ptr), &info, 0, 0, static_cast<int>(unit_type));

    // The cast should result in 300 mod 256 = 44
    bool ok = (info.offset_unit_type == static_cast<png_byte>(44)) &&
              (info.x_offset == 0) && (info.y_offset == 0) &&
              (info.valid & PNG_INFO_oFFs);

    if(!ok) {
        g_failures.push_back("test_unit_type_cast_wrapping: offset_unit_type did not wrap as expected");
    }

    // Also verify debug log presence for oFFs
    bool debug_seen = false;
    for (const auto &s : g_png_debug_log) {
        if (s.find("oFFs") != std::string::npos) {
            debug_seen = true;
            break;
        }
    }
    if(!debug_seen) {
        g_failures.push_back("test_unit_type_cast_wrapping: expected png_debug1 to be invoked with 'oFFs'");
    }

    return g_failures.empty();
}

// Entry point
int main() {
    int total = 0;
    int passed = 0;

    // Run Test 1
    bool t1 = test_null_png_ptr();
    total++; if (t1) ++passed;
    // Report
    std::cout << "Test 1 (null png_ptr): " << (t1 ? "PASSED" : "FAILED") << "\n";
    if (!t1) {
        for (const auto& m : g_failures) std::cout << "  - " << m << "\n";
    }

    // Run Test 2
    bool t2 = test_null_info_ptr();
    total++; if (t2) ++passed;
    std::cout << "Test 2 (null info_ptr): " << (t2 ? "PASSED" : "FAILED") << "\n";
    if (!t2) {
        for (const auto& m : g_failures) std::cout << "  - " << m << "\n";
    }

    // Run Test 3
    bool t3 = test_set_fields_updates_and_flag();
    total++; if (t3) ++passed;
    std::cout << "Test 3 (set fields and flag): " << (t3 ? "PASSED" : "FAILED") << "\n";
    if (!t3) {
        for (const auto& m : g_failures) std::cout << "  - " << m << "\n";
    }

    // Run Test 4
    bool t4 = test_unit_type_cast_wrapping();
    total++; if (t4) ++passed;
    std::cout << "Test 4 (unit_type cast wrap): " << (t4 ? "PASSED" : "FAILED") << "\n";
    if (!t4) {
        for (const auto& m : g_failures) std::cout << "  - " << m << "\n";
    }

    // Final summary
    std::cout << "\nSummary: " << passed << "/" << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}