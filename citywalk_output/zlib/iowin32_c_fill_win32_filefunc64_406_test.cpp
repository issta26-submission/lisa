/*
Test suite for fill_win32_filefunc64 in iowin32.c

Objective:
- Verify that fill_win32_filefunc64 correctly populates the zlib_filefunc64_def structure
  with the appropriate Win32-backed file operation callbacks and sets opaque to NULL.

Keywords/Candidate Components:
- zopen64_file, zread_file, zwrite_file, ztell64_file, zseek64_file, zclose_file, zerror_file
- win32_open64_file_func, win32_read_file_func, win32_write_file_func, win32_tell64_file_func
- win32_seek64_file_func, win32_close_file_func, win32_error_file_func
- opaque (should be NULL after fill)
- zlib_filefunc64_def type
- Function pointer type equivalence checks

Notes:
- This test uses a minimal, self-contained harness (no GTest) to validate function-pointer
  assignments by comparing actual pointers against expected function addresses.
- We rely on the C interface exposed by zlib (including ZCALLBACK and voidpf) and link
  against the iowin32.c implementation that defines fill_win32_filefunc64.
- Static/internal linkage functions (e.g., static void win32_translate_open_mode, etc.)
  are not directly tested; we only verify the public-facing effect of fill_win32_filefunc64.
*/

#include <cstring>
#include <iowin32.h>
#include <iostream>
#include <ioapi.h>
#include <zlib.h>
#include <stdlib.h>


// Bring in zlib types and function pointer typedefs, with C linkage to prevent name mangling.
extern "C" {
}

// Declare the function under test (defined in iowin32.c)
extern "C" void fill_win32_filefunc64(zlib_filefunc64_def* pzlib_filefunc_def);

// Declarations for the functions whose addresses we expect to be assigned.
// These are defined in the same C module as the focal function (non-static in the C source)
extern "C" voidpf ZCALLBACK win32_open64_file_func(voidpf opaque, const void* filename, int mode);
extern "C" uLong ZCALLBACK win32_read_file_func(voidpf opaque, voidpf stream, void* buf, uLong size);
extern "C" uLong ZCALLBACK win32_write_file_func(voidpf opaque, voidpf stream, const void* buf, uLong size);
extern "C" ZPOS64_T ZCALLBACK win32_tell64_file_func(voidpf opaque, voidpf stream);
extern "C" long ZCALLBACK win32_seek64_file_func(voidpf opaque, voidpf stream, ZPOS64_T offset, int origin);
extern "C" int ZCALLBACK win32_close_file_func(voidpf opaque, voidpf stream);
extern "C" int ZCALLBACK win32_error_file_func(voidpf opaque, voidpf stream);

// Typedefs for function-pointer types to compare against the struct's fields.
// These mirror the zlib_filefunc64_def function pointer member types.
typedef voidpf (ZCALLBACK *open64_func_t)(voidpf, const void*, int);
typedef uLong  (ZCALLBACK *read64_func_t)(voidpf, voidpf, void*, uLong);
typedef uLong  (ZCALLBACK *write64_func_t)(voidpf, voidpf, const void*, uLong);
typedef ZPOS64_T (ZCALLBACK *tell64_func_t)(voidpf, voidpf);
typedef long   (ZCALLBACK *seek64_func_t)(voidpf, voidpf, ZPOS64_T, int);
typedef int    (ZCALLBACK *close64_func_t)(voidpf, voidpf);
typedef int    (ZCALLBACK *error64_func_t)(voidpf, voidpf);

static int tests_run = 0;
static int tests_failed = 0;

/* Helper to report test results in a non-terminating fashion. */
#define RUN_TEST(test_fn) do { \
    ++tests_run; \
    if (!(test_fn)) { \
        ++tests_failed; \
        std::cerr << "[TEST FAILED] " #test_fn "\n"; \
    } else { \
        std::cout << "[TEST PASSED] " #test_fn "\n"; \
    } \
} while (0)

/*
Test 1: Pointers are correctly assigned by fill_win32_filefunc64
- Create a zlib_filefunc64_def, zero it, invoke fill_win32_filefunc64, and verify that
  all 7 function pointers are exactly the address of the corresponding Win32 functions,
  and opaque is NULL.
*/
bool test_fill_win32_filefunc64_assigns_all_pointers() {
    zlib_filefunc64_def def;
    std::memset(&def, 0, sizeof(def)); // ensure no pre-existing values interfere

    fill_win32_filefunc64(&def);

    // Cast comparison helpers
    const open64_func_t expected_open = (open64_func_t)win32_open64_file_func;
    const read64_func_t expected_read = (read64_func_t)win32_read_file_func;
    const write64_func_t expected_write = (write64_func_t)win32_write_file_func;
    const tell64_func_t expected_tell = (tell64_func_t)win32_tell64_file_func;
    const seek64_func_t expected_seek = (seek64_func_t)win32_seek64_file_func;
    const close64_func_t expected_close = (close64_func_t)win32_close_file_func;
    const error64_func_t expected_error = (error64_func_t)win32_error_file_func;

    bool ok = true;
    ok &= (def.zopen64_file == expected_open);
    ok &= (def.zread_file == expected_read);
    ok &= (def.zwrite_file == expected_write);
    ok &= (def.ztell64_file == expected_tell);
    ok &= (def.zseek64_file == expected_seek);
    ok &= (def.zclose_file == expected_close);
    ok &= (def.zerror_file == expected_error);
    ok &= (def.opaque == NULL);

    return ok;
}

/*
Test 2: Existing non-null values are overwritten by fill_win32_filefunc64
- Initialize the structure with non-null function pointers (sentinel values),
  call fill_win32_filefunc64, and ensure that the values are replaced with
  the actual Win32-backed function addresses (as in Test 1).
*/
bool test_fill_win32_filefunc64_overwrites_existing_values() {
    zlib_filefunc64_def def;
    // sentinel values to simulate pre-existing non-null pointers
    const open64_func_t sentinel_open = (open64_func_t)0xDEADBEEF;
    const read64_func_t sentinel_read = (read64_func_t)0xCAFEBABE;
    const write64_func_t sentinel_write = (write64_func_t)0xFEEDFACE;
    const tell64_func_t sentinel_tell = (tell64_func_t)0xBAADF00D;
    const seek64_func_t sentinel_seek = (seek64_func_t)0x0D15EA5E;
    const close64_func_t sentinel_close = (close64_func_t)0xBADC0FFE;
    const error64_func_t sentinel_error = (error64_func_t)0xDEADC0DE;

    def.zopen64_file = sentinel_open;
    def.zread_file = sentinel_read;
    def.zwrite_file = sentinel_write;
    def.ztell64_file = sentinel_tell;
    def.zseek64_file = sentinel_seek;
    def.zclose_file = sentinel_close;
    def.zerror_file = sentinel_error;
    def.opaque = (void*)0x12345678; // non-NULL

    fill_win32_filefunc64(&def);

    // Expect overwritten values
    const open64_func_t expected_open = (open64_func_t)win32_open64_file_func;
    const read64_func_t expected_read = (read64_func_t)win32_read_file_func;
    const write64_func_t expected_write = (write64_func_t)win32_write_file_func;
    const tell64_func_t expected_tell = (tell64_func_t)win32_tell64_file_func;
    const seek64_func_t expected_seek = (seek64_func_t)win32_seek64_file_func;
    const close64_func_t expected_close = (close64_func_t)win32_close_file_func;
    const error64_func_t expected_error = (error64_func_t)win32_error_file_func;

    bool ok = true;
    ok &= (def.zopen64_file == expected_open);
    ok &= (def.zread_file == expected_read);
    ok &= (def.zwrite_file == expected_write);
    ok &= (def.ztell64_file == expected_tell);
    ok &= (def.zseek64_file == expected_seek);
    ok &= (def.zclose_file == expected_close);
    ok &= (def.zerror_file == expected_error);
    ok &= (def.opaque == NULL);

    return ok;
}

int main() {
    // Run tests; non-terminating, accumulate failures
    RUN_TEST(test_fill_win32_filefunc64_assigns_all_pointers);
    RUN_TEST(test_fill_win32_filefunc64_overwrites_existing_values);

    // Summary
    if (tests_failed == 0) {
        std::cout << "[ALL TESTS PASSED] " << tests_run << " tests run.\n";
        return 0;
    } else {
        std::cerr << "[TESTS FAILED] " << tests_failed << " of " << tests_run << " tests failed.\n";
        return 1;
    }
}