/*
  C++11 test suite for the focal method 'extract' in iccfrompng.c
  - Uses a lightweight, non-GTest test harness
  - Provides a minimal fake libpng layer to drive true/false ICCP behavior
  - Exercises:
      1) ICCP branch when ICCP is present
      2) No ICCP branch when ICCP is absent
      3) Handling of library pointer NULL (simulated mismatch)
  - Note: This test targets the logic around ICC profile extraction without requiring real PNG data.
*/

// Enable C linkage for the focal C file under test when linking
#include <setjmp.h>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>


// Include real libpng header for type compatibility

/*
  Lightweight fake libpng layer to drive extract() behavior without needing real PNG data.
  We override a subset of libpng symbols to control ICCP presence and error paths.
  The real code uses: png_create_read_struct, png_destroy_read_struct, png_init_io,
  png_read_info, png_get_iCCP, png_error, and the png_jmpbuf mechanism via setjmp/longjmp.
  We emulate the minimal surface required for successful execution of extract().
*/

// State flags to control fake libpng behavior
static bool g_fake_png_iCCP_present = false; // controls ICCP presence
static bool g_fake_png_ptr_null = false;     // when true, png_create_read_struct returns NULL

// ICC profile data to return when ICCP is present
static const unsigned char g_fake_iccp_profile[] = { 0x11, 0x22, 0x33, 0x44 };
static const size_t g_fake_iccp_profile_len = sizeof(g_fake_iccp_profile);

// Forward declarations of fake implementations using libpng's types
// We rely on the actual libpng typedefs (png_structp, png_infop, etc.) defined by <png.h>

// Internal fake storage for the png_struct and png_info allocated by the fake API
struct png_struct_struct_fake {
    jmp_buf jmpbuf; // required by setjmp(png_jmpbuf(png_ptr))
};
struct png_info_struct_fake { /* intentionally empty for test */ };

static struct png_struct_struct_fake g_fake_png_struct;
static struct png_info_struct_fake   g_fake_info_struct;

// Typedefs mirroring libpng's, using real header definitions
// (struct tags used by libpng are provided here to satisfy the opaque pointers)
static struct png_struct_struct_fake*   g_fake_struct_ptr = &g_fake_png_struct;
static struct png_info_struct_fake*     g_fake_info_ptr   = &g_fake_info_struct;

// For the sake of compatibility with libpng's expected typedefs, expose concrete objects
// The real code uses: png_structp, png_infop, png_structpp, png_infopp
// We implement required functions with proper linkage and signatures below.

// A potential external "no_profile" symbol used by iccfrompng.c when ICCP is missing.
// We declare it here to allow linking against the focal code; it is expected to be NULL in many builds.
extern "C" png_bytep no_profile = NULL;

// Utility helpers to configure the fake PNG behavior from tests
extern "C" void fake_set_iCCP_present(int present)
{
    g_fake_png_iCCP_present = (present != 0);
}
extern "C" void fake_set_ptr_null(int v)
{
    g_fake_png_ptr_null = (v != 0);
}

// fake function prototypes (C linkage) to override libpng behavior
extern "C" png_structp
png_create_read_struct(const char*, void*, void*, void*)
{
    // Simulate library mismatch scenario
    if (g_fake_png_ptr_null) return NULL;
    return reinterpret_cast<png_structp>(&g_fake_png_struct);
}

extern "C" png_infop
png_create_info_struct(png_structp)
{
    return reinterpret_cast<png_infop>(&g_fake_info_struct);
}

extern "C" void
png_destroy_read_struct(png_structpp, png_infopp, void*)
{
    // No-op for test purposes
    (void)png_destroy_read_struct; // suppress unused warnings in some compilers
}

extern "C" void
png_init_io(png_structp, FILE*)
{
    // No-op: our fake ignores actual IO
}

extern "C" void
png_read_info(png_structp, png_infop)
{
    // No-op
}

extern "C" png_uint_32
png_get_iCCP(png_structp png_ptr, png_infop info_ptr,
             png_charpp name, int* compression_type,
             png_bytep* profile, png_uint_32* proflen)
{
    // Provide ICCP data when requested; otherwise indicate no ICCP
    (void)png_ptr; (void)info_ptr; // unused in fake
    if (g_fake_png_iCCP_present)
    {
        static const char *icc_name = "icc;fake";
        *name = const_cast<png_charp>(icc_name);
        *compression_type = 0;
        *profile = const_cast<png_bytep>(g_fake_iccp_profile);
        *proflen = static_cast<png_uint_32>(g_fake_iccp_profile_len);
        return PNG_INFO_iCCP;
    }
    else
    {
        *name = NULL;
        *compression_type = 0;
        *profile = NULL;
        *proflen = 0;
        return 0;
    }
}

// libpng error handler: no-op in tests
extern "C" void
png_error(png_structp, const char*)
{
    // In real code this would longjmp; for tests we keep execution
}

// We provide a minimal, fake png_jmpbuf accessor if the code uses it.
// libpng typically uses a macro: "png_jmpbuf(png_ptr)" returning a jmp_buf* to pass to setjmp.
extern "C" jmp_buf* png_jmpbuf(png_structp png_ptr)
{
    // Our fake png_struct_struct_fake has a jmpbuf member
    return &g_fake_png_struct.jmpbuf;
}

// The following declarations ensure linkage for the focal function 'extract' from iccfrompng.c
extern "C" unsigned char* extract(FILE* fp, png_uint_32* proflen);

int main(); // forward for test harness

// Expected to be linked with iccfrompng.c which uses real/no_profile symbol.
// No additional externs needed beyond above.


// Simple test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define TEST_EXPECT(cond, desc) do { \
    ++g_tests_run; \
    if (cond) { \
        std::cout << "[PASS] " << desc << std::endl; \
        ++g_tests_passed; \
    } else { \
        std::cout << "[FAIL] " << desc << std::endl; \
    } \
} while(0)

// Helpers to create a tiny in-memory PNG file (content is irrelevant for fake libpng)
static void write_dummy_file(const char* path)
{
    FILE* f = std::fopen(path, "wb");
    if (!f) return;
    // Write a tiny, non-meaningful file body; libpng fake ignores content
    const unsigned char dummy[] = { 0x89, 'P', 'N', 'G', 0x0A, 0x1A, 0x0A, 0x00 };
    std::fwrite(dummy, 1, sizeof(dummy), f);
    std::fclose(f);
}

// Best-effort path to generate a unique temp filename
static void generate_temp_filename(char* buf, size_t len)
{
#if defined(_WIN32)
    std::snprintf(buf, len, "icc_test_%d.png", rand());
#else
    std::snprintf(buf, len, "/tmp/icc_test_%d.png", rand());
#endif
}

// Test 1: ICCP present should return a non-null profile buffer with expected data
static bool test_iccp_present()
{
    fake_set_iCCP_present(1);
    fake_set_ptr_null(0);

    char tmpname[256];
    generate_temp_filename(tmpname, sizeof(tmpname));
    write_dummy_file(tmpname);

    FILE* f = std::fopen(tmpname, "rb");
    if (!f) {
        std::cout << "[ERR] Unable to create test PNG file for ICCP present" << std::endl;
        std::remove(tmpname);
        return false;
    }

    png_uint_32 proflen = 0;
    unsigned char* prof = extract(f, &proflen);

    std::fclose(f);
    std::remove(tmpname);

    bool ok = true;
    if (prof == NULL) {
        ok = false;
        std::cout << "[DBG] ICCP present: profile pointer is NULL" << std::endl;
    } else {
        ok = (proflen == (png_uint_32)sizeof(g_fake_iccp_profile)) && (std::memcmp(prof, g_fake_iccp_profile, proflen) == 0);
        if (!ok) {
            std::cout << "[DBG] ICCP present: profile data mismatch or length mismatch" << std::endl;
        }
        std::free(prof);
    }

    return ok;
}

// Test 2: No ICCP should lead to no profile and proflen == 0
static bool test_no_iccp()
{
    fake_set_iCCP_present(0);
    fake_set_ptr_null(0);

    char tmpname[256];
    generate_temp_filename(tmpname, sizeof(tmpname));
    write_dummy_file(tmpname);

    FILE* f = std::fopen(tmpname, "rb");
    if (!f) {
        std::cout << "[ERR] Unable to create test PNG file for no ICCP" << std::endl;
        std::remove(tmpname);
        return false;
    }

    png_uint_32 proflen = 0;
    unsigned char* prof = extract(f, &proflen);

    std::fclose(f);
    std::remove(tmpname);

    bool ok = true;
    // We cannot rely on the exact sentinel value of no_profile here; only verify proflen == 0
    if (proflen != 0) ok = false;
    // Also, ensure that the returned profile pointer respect the library contract (NULL or sentinel)
    // We avoid hard-coding the sentinel; we simply ensure no ICCP data is returned
    if (prof != NULL) {
        // If a non-null buffer is returned, this would still be valid ICCP absent behavior only if crafted by libpng.
        // Since proflen == 0, a non-null buffer would be unexpected.
        ok = false;
    }

    return ok;
}

// Test 3: Library mismatch path (png_ptr == NULL) should cause extract to return NULL
static bool test_ptr_null_path()
{
    fake_set_iCCP_present(1); // ICCP would be prepared if not for NULL ptr
    fake_set_ptr_null(1);     // Force png_create_read_struct to return NULL

    char tmpname[256];
    generate_temp_filename(tmpname, sizeof(tmpname));
    write_dummy_file(tmpname);

    FILE* f = std::fopen(tmpname, "rb");
    if (!f) {
        std::cout << "[ERR] Unable to create test PNG file for NULL ptr path" << std::endl;
        std::remove(tmpname);
        return false;
    }

    png_uint_32 proflen = 0;
    unsigned char* prof = extract(f, &proflen);

    std::fclose(f);
    std::remove(tmpname);

    bool ok = (prof == NULL);
    if (!ok) {
        std::cout << "[DBG] NULL ptr path: extract did not return NULL as expected" << std::endl;
    }
    return ok;
}

// Entry point for test harness
int main()
{
    std::cout << "Starting iccfrompng extract() unit tests (C++11 harness)..." << std::endl;

    bool r1 = test_iccp_present();
    TEST_EXPECT(r1, "ICCP present should return non-null profile with correct data");

    bool r2 = test_no_iccp();
    TEST_EXPECT(r2, "No ICCP should yield zero proflen and no profile data");

    bool r3 = test_ptr_null_path();
    TEST_EXPECT(r3, "Library ptr-null path should yield NULL profile (simulated mismatch)");

    std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_passed << std::endl;

    // Return non-zero if any test failed
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}