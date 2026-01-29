// Test suite for readpng_init in readpng.c with a lightweight mock of libpng.
// This file provides a minimal C mock of the libpng API to exercise
// the readpng_init function under various control-flow branches without
// requiring the real libpng library. The tests are implemented in C++11
// and run via a simple main() without GoogleTest (no GTest usage allowed).

#include <unistd.h>
#include <setjmp.h>
#include <readpng.c>
#include <string>
#include <sys/stat.h>
#include <zlib.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <readpng.h>
#include <stdio.h>
#include <cstring>
#include <png.h>


// -----------------------------------------------------------------------------
// Mock libpng header (png.h)
// We provide a small subset of libpng interfaces used by readpng.c so that
// readpng_init can be exercised in a self-contained test environment.
// We expose C interfaces (extern "C") to avoid C++ name mangling issues
// when linking with the C readpng.c code included below.
// -----------------------------------------------------------------------------

#define PNG_SIG_BYTES 8

extern "C" {

// Basic type aliases used by readpng.c
typedef unsigned char uch;
typedef unsigned long ulg;

// Forward declare mock globals (defined later)
typedef struct png_struct_def* png_ptr;
typedef struct png_info_def* png_infop;

// Mock internal structures
struct png_struct_def {
    jmp_buf jmpbuf;
};
struct png_info_def { int dummy; };

// Globals that readpng.c expects to exist
png_ptr png_ptr = NULL;
png_infop info_ptr = NULL;
ulg width = 0, height = 0;
int bit_depth = 0;
int color_type = 0;

// Mock control flags (set by tests)
static int mock_fail_create = 0;
static int mock_fail_info = 0;
static int mock_longjmp = 0;
static ulg mock_ihdr_width = 0, mock_ihdr_height = 0;
static int mock_ihdr_bit_depth = 0;
static int mock_ihdr_color_type = 0;

// Prototypes of the required png.h interfaces (C linkage)
const char* png_get_libpng_ver(void*);
png_ptr png_create_read_struct(const char*, void*, void*, void*);
png_infop png_create_info_struct(png_ptr);
void png_destroy_read_struct(png_ptr* /*pptr*/, png_infop* /*iptr*/, png_infop* /*endptr*/);
int png_sig_cmp(const uch* sig, int /*start*/, ulg /*num*/);
void png_init_io(png_ptr, FILE*);
void png_set_sig_bytes(png_ptr, int);
void png_read_info(png_ptr, png_infop);
void png_get_IHDR(png_ptr, png_infop, ulg* /*width*/, ulg* /*height*/, int* /*bit_depth*/, int* /*color_type*/, int* /*comp*/, int* /*filter*/, int* /*interlace*/);
jmp_buf* png_jmpbuf(png_ptr);

// Mock setters to control test behavior
void mock_set_fail_create(int v) { mock_fail_create = v; }
void mock_set_fail_info(int v)   { mock_fail_info = v; }
void mock_set_longjmp(int v)     { mock_longjmp = v; }
void mock_set_test_ihdr(ulg w, ulg h, int bd, int ct) {
    mock_ihdr_width = w;
    mock_ihdr_height = h;
    mock_ihdr_bit_depth = bd;
    mock_ihdr_color_type = ct;
}

// PNG signature constant for good-signature tests
static const uch MOCK_PNG_SIG[8] = { 137, 'P', 'N', 'G', 13, 10, 26, 10 };

// Implementation of the mock functions

const char* png_get_libpng_ver(void*) {
    return "mock-1.0";
}

png_ptr png_create_read_struct(const char*, void*, void*, void*) {
    if (mock_fail_create) return NULL;
    png_ptr p = (png_ptr)malloc(sizeof(struct png_struct_def));
    if (p) {
        // Initialize jsjmp buffer if needed
        // Nothing else required for the test
    }
    return p;
}

png_infop png_create_info_struct(png_ptr) {
    if (mock_fail_info) return NULL;
    png_infop ip = (png_infop)malloc(sizeof(struct png_info_def));
    if (ip) {/* ok */ }
    return ip;
}

void png_destroy_read_struct(png_ptr* /*pptr*/, png_infop* /*iptr*/, png_infop* /*endptr*/) {
    // Free any allocated objects if present
    // The test uses a straightforward scenario; keep simple
    // We can't rely on actual allocations here since we created them in the mock
    if (pptr && *pptr) {
        free(*pptr);
        *pptr = NULL;
    }
    if (iptr && *iptr) {
        free(*iptr);
        *iptr = NULL;
    }
    // endptr is ignored in tests
    (void)0;
}

int png_sig_cmp(const uch* sig, int /*start*/, ulg /*num*/) {
    // Compare with the exact PNG signature
    if (sig == NULL) return 1;
    return (memcmp(sig, MOCK_PNG_SIG, 8) == 0) ? 0 : 1;
}

void png_init_io(png_ptr, FILE*) {
    // No-op for mock
}

void png_set_sig_bytes(png_ptr, int) {
    // No-op for mock
}

void png_read_info(png_ptr, png_infop) {
    // If test asks to longjmp (simulate libpng error), do it here
    if (mock_longjmp) {
        longjmp(*png_jmpbuf(png_ptr), 1);
    }
    // Otherwise, do nothing (IHDR will be supplied by png_get_IHDR)
}

void png_get_IHDR(png_ptr, png_infop, ulg* w, ulg* h, int* bd, int* ct, int* /*comp*/, int* /*interlace*/, int* /*palette*/) {
    if (w) *w = mock_ihdr_width;
    if (h) *h = mock_ihdr_height;
    if (bd) *bd = mock_ihdr_bit_depth;
    if (ct) *ct = mock_ihdr_color_type;
}

jmp_buf* png_jmpbuf(png_ptr p) {
    // Return address of internal jmpbuf
    return &p->jmpbuf;
}

} // extern "C"

// Helper to delete a file if it exists
static void remove_file(const std::string& path) {
    if (!path.empty()) {
        remove(path.c_str());
    }
    // ignore errors
}

// -----------------------------------------------------------------------------
// Include the focal implementation (readpng.c) into this translation unit.
// This allows the test to call readpng_init directly and link against our mocks.
// -----------------------------------------------------------------------------

// The readpng.c code is provided as is in the environment. We include it here so that
// readpng_init is defined in this translation unit and uses our mocked png.* functions.

// -----------------------------------------------------------------------------
// Lightweight test harness
// -----------------------------------------------------------------------------

static int tests_passed = 0;
static int tests_failed = 0;

// Create a temporary file containing given bytes.
// Returns a path to the created file (caller must delete it).
static std::string create_temp_file_with_bytes(const unsigned char* data, size_t len) {
    char tmpname[L_TMPNAM];
    if (std::tmpnam(tmpname) == NULL) return "";
    std::string path = std::string(tmpname);
    FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) return "";
    std::fwrite(data, 1, len, f);
    std::fclose(f);
    return path;
}

// Convenience: provide a path to a good PNG signature file
static std::string create_good_signature_file() {
    return create_temp_file_with_bytes(MOCK_PNG_SIG, sizeof(MOCK_PNG_SIG));
}

// Convenience: reset mock state between tests
static void reset_mock_state() {
    mock_set_fail_create(0);
    mock_set_fail_info(0);
    mock_set_longjmp(0);
    mock_set_test_ihdr(123, 456, 8, 6);
    // width/height will be overwritten by png_get_IHDR in tests
    width = 0;
    height = 0;
    bit_depth = 0;
    color_type = 0;
}

// Test 1: Bad PNG signature should cause readpng_init to return 1
static bool test_bad_signature() {
    reset_mock_state();
    // Create a file with an invalid signature (8 zero bytes)
    unsigned char bad_sig[8] = {0,0,0,0,0,0,0,0};
    std::string path = create_temp_file_with_bytes(bad_sig, 8);
    ulg w = 0, h = 0;
    int ret = readpng_init(std::fopen(path.c_str(), "rb"), &w, &h);
    remove_file(path);

    // We expect a failure code of 1 and no assignment to width/height
    bool ok = (ret == 1) && (w == 0) && (h == 0);
    if (!ok) {
        std::cerr << "Test 1 (bad signature) failed: ret=" << ret
                  << ", w=" << w << ", h=" << h << "\n";
        ++tests_failed;
    } else {
        ++tests_passed;
    }
    return ok;
}

// Test 2: Failure to create the read struct should return 4
static bool test_fail_create_struct() {
    reset_mock_state();
    mock_set_fail_create(1);

    unsigned char good_sig[8] = {137,'P','N','G',13,10,26,10};
    std::string path = create_temp_file_with_bytes(good_sig, 8);
    ulg w = 0, h = 0;
    FILE* f = std::fopen(path.c_str(), "rb");
    int ret = readpng_init(f, &w, &h);
    std::fclose(f);
    remove_file(path);

    bool ok = (ret == 4);
    if (!ok) {
        std::cerr << "Test 2 (fail create) failed: ret=" << ret << "\n";
        ++tests_failed;
    } else {
        ++tests_passed;
    }
    return ok;
}

// Test 3: Failure to create the info struct should return 4
static bool test_fail_create_info() {
    reset_mock_state();
    mock_set_fail_info(1);

    unsigned char good_sig[8] = {137,'P','N','G',13,10,26,10};
    std::string path = create_temp_file_with_bytes(good_sig, 8);
    ulg w = 0, h = 0;
    FILE* f = std::fopen(path.c_str(), "rb");
    int ret = readpng_init(f, &w, &h);
    std::fclose(f);
    remove_file(path);

    bool ok = (ret == 4);
    if (!ok) {
        std::cerr << "Test 3 (fail create info) failed: ret=" << ret << "\n";
        ++tests_failed;
    } else {
        ++tests_passed;
    }
    return ok;
}

// Test 4: setjmp path (longjmp) triggers error path and returns 2
static bool test_longjmp_path() {
    reset_mock_state();
    mock_set_longjmp(1);

    unsigned char good_sig[8] = {137,'P','N','G',13,10,26,10};
    std::string path = create_temp_file_with_bytes(good_sig, 8);
    ulg w = 0, h = 0;
    FILE* f = std::fopen(path.c_str(), "rb");
    int ret = readpng_init(f, &w, &h);
    std::fclose(f);
    remove_file(path);

    bool ok = (ret == 2);
    if (!ok) {
        std::cerr << "Test 4 (longjmp path) failed: ret=" << ret << "\n";
        ++tests_failed;
    } else {
        ++tests_passed;
    }
    return ok;
}

// Test 5: Normal successful path sets width/height and returns 0
static bool test_success_path() {
    reset_mock_state();
    mock_set_test_ihdr(1234, 5678, 8, 6); // IHDR values to be returned

    unsigned char good_sig[8] = {137,'P','N','G',13,10,26,10};
    std::string path = create_temp_file_with_bytes(good_sig, 8);
    ulg w = 0, h = 0;
    FILE* f = std::fopen(path.c_str(), "rb");
    int ret = readpng_init(f, &w, &h);
    std::fclose(f);
    remove_file(path);

    bool ok = (ret == 0) && (w == 1234) && (h == 5678);
    if (!ok) {
        std::cerr << "Test 5 (success path) failed: ret=" << ret
                  << ", w=" << w << ", h=" << h << "\n";
        ++tests_failed;
    } else {
        ++tests_passed;
    }
    return ok;
}

// Run all tests and report
int main() {
    std::cout << "Starting readpng_init unit tests (mocked libpng)..." << std::endl;

    bool t1 = test_bad_signature();
    bool t2 = test_fail_create_struct();
    bool t3 = test_fail_create_info();
    bool t4 = test_longjmp_path();
    bool t5 = test_success_path();

    int total = 5;
    int passed = tests_passed;
    int failed = tests_failed + (t1?0:0); // counts already updated
    // The boolean results above already incremented pass/fail counts.

    std::cout << "Tests passed: " << tests_passed << "/" << total << std::endl;
    if (tests_failed > 0) {
        std::cout << "Tests failed: " << tests_failed << std::endl;
    }

    // Return non-zero if any test failed, to indicate overall failure.
    return (tests_failed == 0) ? 0 : 1;
}