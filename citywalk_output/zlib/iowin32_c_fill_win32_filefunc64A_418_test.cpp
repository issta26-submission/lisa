// Test suite for the focal method fill_win32_filefunc64A in iowin32.c
// This test is written in C++11 without using GTest. It uses a minimal
// custom test harness and focuses on verifying the behavior of
// fill_win32_filefunc64A as exercised by its contract on zlib_filefunc64_def.

#include <cstring>
#include <cstdint>
#include <iowin32.h>
#include <iostream>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>


// Include zlib and iowin32 headers to obtain required types and declarations.
extern "C" {
}

// Forward declarations for the exact functions used by fill_win32_filefunc64A.
// We declare them with C linkage so that their addresses can be compared in tests.
// The actual implementations exist in iowin32.c (part of the project under test).

extern "C" {
    voidpf ZCALLBACK win32_open64_file_funcA(voidpf opaque, const void* filename, int mode);
    uLong  ZCALLBACK win32_read_file_func(voidpf opaque, voidpf stream, void* buf, uLong size);
    uLong  ZCALLBACK win32_write_file_func(voidpf opaque, voidpf stream, const void* buf, uLong size);
    ZPOS64_T ZCALLBACK win32_tell64_file_func(voidpf opaque, voidpf stream);
    ZPOS64_T ZCALLBACK win32_seek64_file_func(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin);
    int    ZCALLBACK win32_close_file_func(voidpf opaque, voidpf stream);
    int    ZCALLBACK win32_error_file_func(voidpf opaque, voidpf stream);
}

// Utility macro to print test results without terminating on failure.
#define LOG_RESULT(test_name, condition)                                    \
    do {                                                                    \
        if (condition) {                                                    \
            std::cout << "[PASS] " << test_name << "\n";                   \
        } else {                                                            \
            std::cout << "[FAIL] " << test_name << " (false condition)\n"; \
        }                                                                   \
    } while (0)

// Test 1: Validate that fill_win32_filefunc64A populates all file function pointers
// with non-null values and that opaque is initialized to NULL. Additionally,
// verify that the pointers correspond (by address) to the expected concrete
// functions for a robust check without executing those functions (which may have
// empty bodies or undefined behavior if invoked).
bool test_fill_win32_filefunc64A_populates_pointers_and_opaque() {
    // Arrange
    zlib_filefunc64_def def;
    std::memset(&def, 0, sizeof(def)); // start from a known state

    // Act
    fill_win32_filefunc64A(&def);

    // Assert: non-null function pointers
    bool all_non_null = (def.zopen64_file != nullptr) &&
                        (def.zread_file != nullptr) &&
                        (def.zwrite_file != nullptr) &&
                        (def.ztell64_file != nullptr) &&
                        (def.zseek64_file != nullptr) &&
                        (def.zclose_file != nullptr) &&
                        (def.zerror_file != nullptr);

    // Assert: opaque should be NULL (as per implementation)
    bool opaque_is_null = (def.opaque == NULL);

    // Address checks: ensure the pointers point to the expected concrete functions.
    // We compare addresses by converting function pointers to uintptr_t.

    // Note: We rely on the exact signatures to obtain addresses. The actual
    // type of each member in zlib_filefunc64_def is a function pointer type;
    // converting to uintptr_t is a portable approach to compare function addresses.

    uintptr_t op_actual  = reinterpret_cast<uintptr_t>(def.zopen64_file);
    uintptr_t op_expected = reinterpret_cast<uintptr_t>(&win32_open64_file_funcA);

    uintptr_t rp_actual  = reinterpret_cast<uintptr_t>(def.zread_file);
    uintptr_t rp_expected = reinterpret_cast<uintptr_t>(&win32_read_file_func);

    uintptr_t wp_actual  = reinterpret_cast<uintptr_t>(def.zwrite_file);
    uintptr_t wp_expected = reinterpret_cast<uintptr_t>(&win32_write_file_func);

    uintptr_t tp_actual  = reinterpret_cast<uintptr_t>(def.ztell64_file);
    uintptr_t tp_expected = reinterpret_cast<uintptr_t>(&win32_tell64_file_func);

    uintptr_t sp_actual  = reinterpret_cast<uintptr_t>(def.zseek64_file);
    uintptr_t sp_expected = reinterpret_cast<uintptr_t>(&win32_seek64_file_func);

    uintptr_t cp_actual  = reinterpret_cast<uintptr_t>(def.zclose_file);
    uintptr_t cp_expected = reinterpret_cast<uintptr_t>(&win32_close_file_func);

    uintptr_t ep_actual  = reinterpret_cast<uintptr_t>(def.zerror_file);
    uintptr_t ep_expected = reinterpret_cast<uintptr_t>(&win32_error_file_func);

    bool addresses_match = (op_actual  == op_expected) &&
                           (rp_actual  == rp_expected) &&
                           (wp_actual  == wp_expected) &&
                           (tp_actual  == tp_expected) &&
                           (sp_actual  == sp_expected) &&
                           (cp_actual  == cp_expected) &&
                           (ep_actual  == ep_expected);

    // Combine all assertions
    bool result = all_non_null && opaque_is_null && addresses_match;

    // Detailed logs for debugging
    if (!all_non_null) {
        std::cout << "Debug: Some pointers were null after fill_win32_filefunc64A.\n";
    }
    if (!opaque_is_null) {
        std::cout << "Debug: opaque is not NULL after fill_win32_filefunc64A.\n";
    }
    if (!addresses_match) {
        std::cout << "Debug: One or more function pointers do not match the expected addresses.\n";
        std::cout << "  zopen64_file: actual=" << op_actual << " expected=" << op_expected << "\n";
        std::cout << "  zread_file: actual=" << rp_actual << " expected=" << rp_expected << "\n";
        std::cout << "  zwrite_file: actual=" << wp_actual << " expected=" << wp_expected << "\n";
        std::cout << "  ztell64_file: actual=" << tp_actual << " expected=" << tp_expected << "\n";
        std::cout << "  zseek64_file: actual=" << sp_actual << " expected=" << sp_expected << "\n";
        std::cout << "  zclose_file: actual=" << cp_actual << " expected=" << cp_expected << "\n";
        std::cout << "  zerror_file: actual=" << ep_actual << " expected=" << ep_expected << "\n";
    }

    return result;
}

int main() {
    // Run a compact suite. The project description asks for a C++11 test without GTest.
    // We log results and return non-zero on failure to aid automation.

    std::cout << "Starting test suite for fill_win32_filefunc64A...\n";

    bool t1 = test_fill_win32_filefunc64A_populates_pointers_and_opaque();
    LOG_RESULT("test_fill_win32_filefunc64A_populates_pointers_and_opaque", t1);

    if (!t1) {
        std::cout << "One or more tests failed. See details above.\n";
        return 1;
    }

    std::cout << "All tests completed.\n";
    return 0;
}