// This test suite validates the behavior of fill_win32_filefunc64W from iowin32.c.
// Specifically, it ensures that all function pointers are correctly assigned and opaque is NULL.
// The tests are written for C++11, without any GoogleTest framework, using a simple in-house harness.

#include <cstring>
#include <iowin32.h>
#include <cstdint>
#include <iostream>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>



// Forward declarations for the Windows-specific implementations used by the test.
// These are the functions that fill_win32_filefunc64W is expected to assign.
// We declare them with C linkage to ensure proper symbol resolution during linking.

extern "C" {
    // Prototypes must match the actual signatures declared in iowin32.h
    voidpf ZCALLBACK win32_open64_file_funcW(voidpf, const void*, int);
    uLong  ZCALLBACK win32_read_file_func(voidpf, voidpf, void*, uLong);
    uLong  ZCALLBACK win32_write_file_func(voidpf, voidpf, const void*, uLong);
    ZPOS64_T ZCALLBACK win32_tell64_file_func(voidpf, voidpf);
    long   ZCALLBACK win32_seek64_file_func(voidpf, voidpf, ZPOS64_T, int);
    int    ZCALLBACK win32_close_file_func(voidpf, voidpf);
    int    ZCALLBACK win32_error_file_func(voidpf, voidpf);
}

// Lightweight test framework (non-terminating assertions)
namespace test_suite
{
    static int g_total = 0;
    static int g_failed = 0;

    inline void log_fail(const char* msg)
    {
        std::cerr << "[TEST FAILED] " << msg << std::endl;
        ++g_failed;
    }

    inline void log_pass(const char* msg)
    {
        std::cout << "[TEST PASSED] " << msg << std::endl;
    }

    // Test 1: Verify that fill_win32_filefunc64W assigns all function pointers to the correct Windows implementations
    bool test_fill_win32_filefunc64W_pointer_assignments()
    {
        // Step 2: Test case for pointer assignments
        // Prepare a zero-initialized descriptor
        zlib_filefunc64_def def;
        std::memset(&def, 0, sizeof(def));

        // Call the function under test
        fill_win32_filefunc64W(&def);

        // Type aliases for the expected function pointer types
        typedef voidpf (ZCALLBACK *Open64Type)(voidpf, const void*, int);
        typedef uLong  (ZCALLBACK *Read64Type)(voidpf, voidpf, void*, uLong);
        typedef uLong  (ZCALLBACK *Write64Type)(voidpf, voidpf, const void*, uLong);
        typedef ZPOS64_T (ZCALLBACK *Tell64Type)(voidpf, voidpf);
        typedef long   (ZCALLBACK *Seek64Type)(voidpf, voidpf, ZPOS64_T, int);
        typedef int    (ZCALLBACK *Close64Type)(voidpf, voidpf);
        typedef int    (ZCALLBACK *Error64Type)(voidpf, voidpf);

        // Expected function pointers:
        Open64Type expectedOpen  = reinterpret_cast<Open64Type>(win32_open64_file_funcW);
        Read64Type expectedRead  = reinterpret_cast<Read64Type>(win32_read_file_func);
        Write64Type expectedWrite= reinterpret_cast<Write64Type>(win32_write_file_func);
        Tell64Type expectedTell  = reinterpret_cast<Tell64Type>(win32_tell64_file_func);
        Seek64Type expectedSeek  = reinterpret_cast<Seek64Type>(win32_seek64_file_func);
        Close64Type expectedClose= reinterpret_cast<Close64Type>(win32_close_file_func);
        Error64Type expectedError= reinterpret_cast<Error64Type>(win32_error_file_func);

        bool ok = true;

        if (def.zopen64_file != expectedOpen) {
            log_fail("zopen64_file does not point to win32_open64_file_funcW");
            ok = false;
        } else {
            log_pass("zopen64_file correctly points to win32_open64_file_funcW");
        }

        if (def.zread_file != expectedRead) {
            log_fail("zread_file does not point to win32_read_file_func");
            ok = false;
        } else {
            log_pass("zread_file correctly points to win32_read_file_func");
        }

        if (def.zwrite_file != expectedWrite) {
            log_fail("zwrite_file does not point to win32_write_file_func");
            ok = false;
        } else {
            log_pass("zwrite_file correctly points to win32_write_file_func");
        }

        if (def.ztell64_file != expectedTell) {
            log_fail("ztell64_file does not point to win32_tell64_file_func");
            ok = false;
        } else {
            log_pass("ztell64_file correctly points to win32_tell64_file_func");
        }

        if (def.zseek64_file != expectedSeek) {
            log_fail("zseek64_file does not point to win32_seek64_file_func");
            ok = false;
        } else {
            log_pass("zseek64_file correctly points to win32_seek64_file_func");
        }

        if (def.zclose_file != expectedClose) {
            log_fail("zclose_file does not point to win32_close_file_func");
            ok = false;
        } else {
            log_pass("zclose_file correctly points to win32_close_file_func");
        }

        if (def.zerror_file != expectedError) {
            log_fail("zerror_file does not point to win32_error_file_func");
            ok = false;
        } else {
            log_pass("zerror_file correctly points to win32_error_file_func");
        }

        // 8th field: opaque should be NULL
        if (def.opaque != NULL) {
            log_fail("opaque should be NULL after filling filefunc64W");
            ok = false;
        } else {
            log_pass("opaque is NULL as expected");
        }

        // Update global counters for reporting
        ++g_total;
        if (!ok) ++g_failed;
        return ok;
    }

    // Test 2: Ensure behavior with non-NULL opaque is overwritten by fill_win32_filefunc64W
    bool test_fill_win32_filefunc64W_opaque_overwrite()
    {
        zlib_filefunc64_def def;
        // Pre-fill opaque with a non-NULL sentinel
        def.opaque = (void*)0xDEADBEEF;
        // Also pre-fill function pointers with dummy values
        def.zopen64_file = (Open64Type)nullptr;
        def.zread_file = (Read64Type)nullptr;
        def.zwrite_file = (Write64Type)nullptr;
        def.ztell64_file = (Tell64Type)nullptr;
        def.zseek64_file = (Seek64Type)nullptr;
        def.zclose_file = (Close64Type)nullptr;
        def.zerror_file = (Error64Type)nullptr;

        fill_win32_filefunc64W(&def);

        bool ok = true;

        if (def.opaque != NULL) {
            log_fail("opaque should be overwritten to NULL by fill_win32_filefunc64W");
            ok = false;
        } else {
            log_pass("opaque correctly overwritten to NULL");
        }

        // pointers should be set as in previous test
        typedef voidpf (ZCALLBACK *Open64Type)(voidpf, const void*, int);
        Open64Type expectedOpen  = reinterpret_cast<Open64Type>(win32_open64_file_funcW);
        if (def.zopen64_file != expectedOpen) {
            log_fail("zopen64_file did not overwrite correctly in overwrite test");
            ok = false;
        } else {
            log_pass("zopen64_file overwritten correctly in overwrite test");
        }

        ++g_total;
        if (!ok) ++g_failed;
        return ok;
    }

} // namespace test_suite

int main()
{
    // Run tests
    using namespace test_suite;

    bool t1 = test_suite::test_fill_win32_filefunc64W_pointer_assignments();
    bool t2 = test_suite::test_fill_win32_filefunc64W_opaque_overwrite();

    // Summary
    std::cout << "\nTest Summary: " << (t1 ? "PASS" : "FAIL") // t1 result is combined in internal logging, but we provide a summary per test
              << "\nTotal tests executed: " << test_suite::g_total
              << "\nTotal failures: " << test_suite::g_failed
              << std::endl;

    // Return non-zero if any test failed
    return test_suite::g_failed ? 1 : 0;
}