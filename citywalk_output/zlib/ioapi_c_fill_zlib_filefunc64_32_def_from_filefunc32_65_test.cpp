// C++11 test suite for the focal method:
// fill_zlib_filefunc64_32_def_from_filefunc32
// - No GTest usage; a lightweight custom test harness is implemented.
// - Tests verify correct propagation of function pointers from zlib_filefunc_def
//   to zlib_filefunc64_32_def as performed by fill_zlib_filefunc64_32_def_from_filefunc32.
// - Extern "C" is used for C-style function pointers to ensure proper linkage.

#include <iostream>
#include <ioapi.h>
#include <cstdint>
#include <cstring>


// Import C dependencies (minizip/ioapi). Use C linkage to avoid name mangling issues.
extern "C" {
}

// Forward declarations of test helper functions (C linkage to match expected signatures)
extern "C" {
voidpf ZCALLBACK zopen_file_test(voidpf opaque, const char* filename, int mode);
uLong ZCALLBACK zread_file_test(voidpf opaque, voidpf stream, void* buf, uLong size);
uLong ZCALLBACK zwrite_file_test(voidpf opaque, voidpf stream, const void* buf, uLong size);
int ZCALLBACK zclose_file_test(voidpf opaque, voidpf stream);
int ZCALLBACK zerror_file_test(voidpf opaque, voidpf stream);
long ZCALLBACK zseek_file_test(voidpf opaque, voidpf stream, uLong offset, int origin);
long ZCALLBACK ztell_file_test(voidpf opaque, voidpf stream);
}

// Implement test stub functions to provide distinct, identifiable pointers.
// These do not need to be invoked; only their addresses are compared.
extern "C" {
voidpf ZCALLBACK zopen_file_test(voidpf opaque, const char* filename, int mode) {
    // Return a unique non-null pointer to identify this function
    return reinterpret_cast<void*>(0x1001);
}
uLong ZCALLBACK zread_file_test(voidpf opaque, voidpf stream, void* buf, uLong size) {
    (void)opaque; (void)stream; (void)buf; (void)size;
    return 0;
}
uLong ZCALLBACK zwrite_file_test(voidpf opaque, voidpf stream, const void* buf, uLong size) {
    (void)opaque; (void)stream; (void)buf; (void)size;
    return 0;
}
int ZCALLBACK zclose_file_test(voidpf opaque, voidpf stream) {
    (void)opaque; (void)stream;
    return 0;
}
int ZCALLBACK zerror_file_test(voidpf opaque, voidpf stream) {
    (void)opaque; (void)stream;
    return 0;
}
long ZCALLBACK zseek_file_test(voidpf opaque, voidpf stream, uLong offset, int origin) {
    (void)opaque; (void)stream; (void)offset; (void)origin;
    return 0;
}
long ZCALLBACK ztell_file_test(voidpf opaque, voidpf stream) {
    (void)opaque; (void)stream;
    return 0;
}
}

static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[TEST FAILED] " << msg << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Test 1: Basic propagation of non-null function pointers from zlib_filefunc_def to
//          the zlib_filefunc64_32_def structure via fill_zlib_filefunc64_32_def_from_filefunc32.
//          Verifies both 64-bit zfile_func64 and 32-bit zseek/ztell pointers are set
//          as described in the source implementation.
void test_fill_zlib_filefunc64_32_def_from_filefunc32_basic_propagation() {
    // Prepare source (32-bit) function table with identifiable non-null pointers.
    zlib_filefunc_def src = {};
    src.zopen_file   = zopen_file_test;
    src.zread_file   = zread_file_test;
    src.zwrite_file  = zwrite_file_test;
    src.zseek_file   = zseek_file_test;
    src.ztell_file   = ztell_file_test;
    src.zclose_file  = zclose_file_test;
    src.zerror_file  = zerror_file_test;
    src.opaque       = reinterpret_cast<void*>(0xDEADBEEF);

    // Prepare destination (64/32-bit hybrid) structure with initial dummy values.
    zlib_filefunc64_32_def dest = {};
    // Initialize some fields to non-null values to ensure they get overwritten
    dest.zfile_func64.zopen64_file = reinterpret_cast<void*>(0xB001);
    dest.zopen32_file = reinterpret_cast<void*>(0xB002);
    dest.zfile_func64.zread_file = reinterpret_cast<void*>(0xB003);
    dest.zfile_func64.zwrite_file = reinterpret_cast<void*>(0xB004);
    dest.zfile_func64.ztell64_file = reinterpret_cast<void*>(0xB005);
    dest.zfile_func64.zseek64_file = reinterpret_cast<void*>(0xB006);
    dest.zfile_func64.zclose_file = reinterpret_cast<void*>(0xB007);
    dest.zfile_func64.zerror_file = reinterpret_cast<void*>(0xB008);
    dest.zfile_func64.opaque = reinterpret_cast<void*>(0xB009);
    dest.zseek32_file = reinterpret_cast<void*>(0xB00A);
    dest.ztell32_file = reinterpret_cast<void*>(0xB00B);

    // Act
    fill_zlib_filefunc64_32_def_from_filefunc32(&dest, &src);

    // Assert: 64-bit open must be NULL; 32-bit open must be copied from src
    CHECK(dest.zfile_func64.zopen64_file == nullptr, "dest.zfile_func64.zopen64_file should be NULL after fill");
    CHECK(dest.zopen32_file == src.zopen_file, "dest.zopen32_file should be src.zopen_file");
    // 64-bit read/write must be copied
    CHECK(dest.zfile_func64.zread_file == src.zread_file, "dest.zfile_func64.zread_file should be src.zread_file");
    CHECK(dest.zfile_func64.zwrite_file == src.zwrite_file, "dest.zfile_func64.zwrite_file should be src.zwrite_file");
    // 64-bit tell/seek should be NULL
    CHECK(dest.zfile_func64.ztell64_file == nullptr, "dest.zfile_func64.ztell64_file should be NULL after fill");
    CHECK(dest.zfile_func64.zseek64_file == nullptr, "dest.zfile_func64.zseek64_file should be NULL after fill");
    // 64-bit close/error/opaque should be copied
    CHECK(dest.zfile_func64.zclose_file == src.zclose_file, "dest.zfile_func64.zclose_file should be src.zclose_file");
    CHECK(dest.zfile_func64.zerror_file == src.zerror_file, "dest.zfile_func64.zerror_file should be src.zerror_file");
    CHECK(dest.zfile_func64.opaque == src.opaque, "dest.zfile_func64.opaque should be src.opaque");

    // 32-bit function pointers on the dest should reflect src's 32-bit counterparts
    CHECK(dest.zseek32_file == src.zseek_file, "dest.zseek32_file should be src.zseek_file");
    CHECK(dest.ztell32_file == src.ztell_file, "dest.ztell32_file should be src.ztell_file");
}

// Test 2: Null-propagation scenario where all fields in the 32-bit table are NULL.
// Verifies that the destination structure receives NULLs accordingly.
void test_fill_zlib_filefunc64_32_def_from_filefunc32_null_propagation() {
    // Source 32-bit table with all fields NULL
    zlib_filefunc_def src = {};
    src.zopen_file = nullptr;
    src.zread_file = nullptr;
    src.zwrite_file = nullptr;
    src.zseek_file = nullptr;
    src.ztell_file = nullptr;
    src.zclose_file = nullptr;
    src.zerror_file = nullptr;
    src.opaque = nullptr;

    // Destination pre-filled with non-null values to ensure overwriting occurs
    zlib_filefunc64_32_def dest = {};
    dest.zfile_func64.zopen64_file = reinterpret_cast<void*>(0xD001);
    dest.zopen32_file = reinterpret_cast<void*>(0xD002);
    dest.zfile_func64.zread_file = reinterpret_cast<void*>(0xD003);
    dest.zfile_func64.zwrite_file = reinterpret_cast<void*>(0xD004);
    dest.zfile_func64.ztell64_file = reinterpret_cast<void*>(0xD005);
    dest.zfile_func64.zseek64_file = reinterpret_cast<void*>(0xD006);
    dest.zfile_func64.zclose_file = reinterpret_cast<void*>(0xD007);
    dest.zfile_func64.zerror_file = reinterpret_cast<void*>(0xD008);
    dest.zfile_func64.opaque = reinterpret_cast<void*>(0xD009);
    dest.zseek32_file = reinterpret_cast<void*>(0xD00A);
    dest.ztell32_file = reinterpret_cast<void*>(0xD00B);

    // Act
    fill_zlib_filefunc64_32_def_from_filefunc32(&dest, &src);

    // Assert: All relevant fields should be NULL after fill
    CHECK(dest.zfile_func64.zopen64_file == nullptr, "dest.zfile_func64.zopen64_file should be NULL (null src)");
    CHECK(dest.zopen32_file == nullptr, "dest.zopen32_file should be NULL (null src)");
    CHECK(dest.zfile_func64.zread_file == nullptr, "dest.zfile_func64.zread_file should be NULL (null src)");
    CHECK(dest.zfile_func64.zwrite_file == nullptr, "dest.zfile_func64.zwrite_file should be NULL (null src)");
    CHECK(dest.zfile_func64.ztell64_file == nullptr, "dest.zfile_func64.ztell64_file should be NULL (null src)");
    CHECK(dest.zfile_func64.zseek64_file == nullptr, "dest.zfile_func64.zseek64_file should be NULL (null src)");
    CHECK(dest.zfile_func64.zclose_file == nullptr, "dest.zfile_func64.zclose_file should be NULL (null src)");
    CHECK(dest.zfile_func64.zerror_file == nullptr, "dest.zfile_func64.zerror_file should be NULL (null src)");
    CHECK(dest.zfile_func64.opaque == nullptr, "dest.zfile_func64.opaque should be NULL (null src)");
    CHECK(dest.zseek32_file == nullptr, "dest.zseek32_file should be NULL (null src)");
    CHECK(dest.ztell32_file == nullptr, "dest.ztell32_file should be NULL (null src)");
}

// Entry point: run tests and report summary.
// Domain knowledge: use only standard facilities and provided headers; no private members.
// The tests are designed for C++11 compatibility; avoid GTest. Use a lightweight harness with non-terminating assertions.
int main() {
    std::cout << "Starting tests for fill_zlib_filefunc64_32_def_from_filefunc32...\n";

    test_fill_zlib_filefunc64_32_def_from_filefunc32_basic_propagation();
    test_fill_zlib_filefunc64_32_def_from_filefunc32_null_propagation();

    if(g_failures == 0) {
        std::cout << "[ALL TESTS PASSED] " << std::endl;
    } else {
        std::cout << "[TESTS FAILED] " << g_failures << " failure(s) detected." << std::endl;
    }

    // Return non-zero if any test failed.
    return g_failures ? 1 : 0;
}