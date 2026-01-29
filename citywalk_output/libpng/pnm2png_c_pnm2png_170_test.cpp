/*
  Test suite for the focal method: pnm2png (C function in pnm2png.c)

  This test suite is designed to compile in a C++11 environment without using GTest.
  It exercises the focal function pnm2png by providing real FILE* streams and
  validating behavior under both success and failure scenarios.

  Highlights and design notes:
  - Candidate Keywords (reflecting core dependencies and behavior)
    pnm2png, do_pnm2png, libpng (via png_create_write_struct, png_init_io, etc.),
    PNM formats (PNM header parsing), interlace flag, alpha flag, png_ptr, info_ptr
  - We keep tests focused on public behavior of pnm2png:
    1) A small, valid PNM input (1x1 P5/PGM) producing a PNG file should succeed
       and the resulting PNG file should begin with the PNG signature.
    2) An invalid PNM header should cause pnm2png to return FALSE (failure path).
    3) A valid minimal PNM input with alpha requested (alpha = TRUE) and a valid
       alpha_file stream should also succeed (if the underlying conversion supports it).
  - We avoid mocking private or static internals. We rely on the public C API
    exposed by pnm2png.c and its expected runtime behavior.
  - Tests are written using plain C I/O (FILE*, tmpfile, fopen) and C-style
    return codes, wrapped in a small C++ test harness.
  - The tests require linking with libpng (e.g., -lpng) when building.

  Important:
  - If your environment differs (e.g., Windows), you may need to replace tmpfile()
    with a cross-platform alternative for in-memory streams or create temporary
    files on disk for the PNG output.
  - We assume that pnm2png.c does not define a conflicting main() when compiled as part
    of this test suite. If your build system exposes a main() inside pnm2png.c, you may
    need to compile the tests with pnm2png.c as a library or adjust build flags accordingly.
*/

#include <string>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <png.h>


using namespace std;

/*
  Provide an extern declaration for the focal function with C linkage.
  The original code uses a BOOL type defined as unsigned char in C.
  For compatibility in C++, we declare the parameter and return types as unsigned char.
  (0 -> FALSE, non-zero -> TRUE)
*/
extern "C" unsigned char pnm2png(FILE* pnm_file,
                                FILE* png_file,
                                FILE* alpha_file,
                                unsigned char interlace,
                                unsigned char alpha);

/*
  Helper: verify that the PNG signature is present at the start of the given FILE*
*/
static bool check_png_signature(FILE* f) {
    if (!f) return false;
    if (fseek(f, 0, SEEK_SET) != 0) return false;

    unsigned char sig[8];
    if (fread(sig, 1, 8, f) != 8) return false;

    const unsigned char expected[8] = {
        0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A
    };
    return (memcmp(sig, expected, 8) == 0);
}

/*
  Test 1: Basic valid PNM -> PNG conversion (1x1 P5/PGM)
  - Create a tiny valid PNM (P5, 1x1, 255, single pixel 0)
  - Create an in-memory PNG file with tmpfile()
  - Call pnm2png(pnm_file, png_file, NULL, interlace=false, alpha=false)
  - Expect TRUE and a valid PNG signature in the output file
*/
static bool test_pnm2png_basic_valid() {
    cout << "Test 1: Basic valid PNM -> PNG conversion (no alpha, no interlace)" << endl;

    // Prepare a tiny valid PNM (P5, 1x1, 255) with a single pixel value 0
    const char* input_path = "test_input_basic.pnm";
    {
        FILE* f = fopen(input_path, "wb");
        if (!f) {
            cerr << "Failed to create input file for Test 1" << endl;
            return false;
        }
        const char* header = "P5\n1 1\n255\n";
        fwrite(header, 1, strlen(header), f);
        unsigned char pixel = 0; // black pixel
        fwrite(&pixel, 1, 1, f);
        fclose(f);
    }

    FILE* pnm_file = fopen(input_path, "rb");
    if (!pnm_file) {
        cerr << "Failed to open input file for Test 1" << endl;
        remove(input_path);
        return false;
    }

    FILE* png_file = tmpfile();
    if (!png_file) {
        cerr << "Failed to create temporary PNG file for Test 1" << endl;
        fclose(pnm_file);
        remove(input_path);
        return false;
    }

    unsigned char result = pnm2png(pnm_file, png_file, NULL, 0, 0);
    fclose(pnm_file);
    // Do not fclose png_file yet; we may read from it

    bool ok = (result != 0) && check_png_signature(png_file);

    // Cleanup
    fclose(png_file);
    remove(input_path);

    if (!ok) {
        cerr << "Test 1 FAILED: pnm2png did not return TRUE or PNG signature not detected" << endl;
        return false;
    }

    cout << "Test 1 PASSED" << endl;
    return true;
}

/*
  Test 2: Invalid PNM header should cause conversion to fail
  - Create a file with an invalid PNM header (not a recognized magic)
  - Expect pnm2png to return FALSE
*/
static bool test_pnm2png_invalid_header() {
    cout << "Test 2: Invalid PNM header should fail conversion" << endl;

    const char* input_path = "test_input_invalid.pnm";
    {
        FILE* f = fopen(input_path, "wb");
        if (!f) {
            cerr << "Failed to create input file for Test 2" << endl;
            return false;
        }
        // Invalid magic header
        const char* header = "XYZ\n1 1\n255\n";
        fwrite(header, 1, strlen(header), f);
        unsigned char pixel = 0;
        fwrite(&pixel, 1, 1, f);
        fclose(f);
    }

    FILE* pnm_file = fopen(input_path, "rb");
    if (!pnm_file) {
        cerr << "Failed to open input file for Test 2" << endl;
        remove(input_path);
        return false;
    }

    FILE* png_file = tmpfile();
    if (!png_file) {
        cerr << "Failed to create temporary PNG file for Test 2" << endl;
        fclose(pnm_file);
        remove(input_path);
        return false;
    }

    unsigned char result = pnm2png(pnm_file, png_file, NULL, 0, 0);
    fclose(pnm_file);
    fclose(png_file);
    remove(input_path);

    // Expect FALSE (0)
    if (result != 0) {
        cerr << "Test 2 FAILED: expected FALSE return on invalid header" << endl;
        return false;
    }

    cout << "Test 2 PASSED" << endl;
    return true;
}

/*
  Test 3: Valid PNM with alpha requested (alpha_file provided)
  - Use same small valid PNM as Test 1
  - Provide an empty alpha_file (tmpfile) and set alpha flag to TRUE
  - Expect TRUE and a valid PNG signature
  - This exercises the path where alpha is requested (non-fatal for a minimal test)
*/
static bool test_pnm2png_valid_with_alpha() {
    cout << "Test 3: Valid PNM with alpha requested (alpha_file provided)" << endl;

    // Prepare a tiny valid PNM (P5, 1x1, 255) with a single pixel value 0
    const char* input_path = "test_input_alpha.pnm";
    {
        FILE* f = fopen(input_path, "wb");
        if (!f) {
            cerr << "Failed to create input file for Test 3" << endl;
            return false;
        }
        const char* header = "P5\n1 1\n255\n";
        fwrite(header, 1, strlen(header), f);
        unsigned char pixel = 0;
        fwrite(&pixel, 1, 1, f);
        fclose(f);
    }

    FILE* pnm_file = fopen(input_path, "rb");
    if (!pnm_file) {
        cerr << "Failed to open input file for Test 3" << endl;
        remove(input_path);
        return false;
    }

    FILE* png_file = tmpfile();
    if (!png_file) {
        cerr << "Failed to create temporary PNG file for Test 3" << endl;
        fclose(pnm_file);
        remove(input_path);
        return false;
    }

    // Create an empty alpha file to pass when alpha=true
    FILE* alpha_file = tmpfile();
    if (!alpha_file) {
        cerr << "Failed to create alpha temp file for Test 3" << endl;
        fclose(pnm_file);
        fclose(png_file);
        remove(input_path);
        return false;
    }

    unsigned char result = pnm2png(pnm_file, png_file, alpha_file, 0 /* interlace */, 1 /* alpha */);

    fclose(pnm_file);
    fclose(alpha_file);
    // PNG file remains; we can inspect
    bool ok = (result != 0) && check_png_signature(png_file);
    fclose(png_file);
    remove(input_path);

    if (!ok) {
        cerr << "Test 3 FAILED: pnm2png with alpha flag did not succeed or produced non-PNG output" << endl;
        return false;
    }

    cout << "Test 3 PASSED" << endl;
    return true;
}

/*
  Main driver: run all tests and report summary
*/
int main() {
    int failures = 0;

    if (!test_pnm2png_basic_valid()) {
        ++failures;
    }

    if (!test_pnm2png_invalid_header()) {
        ++failures;
    }

    if (!test_pnm2png_valid_with_alpha()) {
        ++failures;
    }

    if (failures == 0) {
        cout << "ALL TESTS PASSED" << endl;
        return 0;
    } else {
        cout << failures << " TEST(S) FAILED" << endl;
        return 1;
    }
}