// Test suite for WriteOutputTags function in tificc.c
// This test suite is written in C++11 (no GTest) and uses libtiff to verify
// that WriteOutputTags correctly sets TIFF fields for various colorspaces,
// alpha handling, and bit-depth scenarios.
// The tests create temporary TIFF files, call WriteOutputTags, then read back
// the fields using TIFFGetField to validate behavior.
// Explanatory comments accompany each unit test.

// Note: This test assumes the project under test (tificc.c) and libtiff are
// available during compilation/linking. It uses extern "C" to declare the
// function under test.

#include <cstring>
#include <cstdio>
#include <lcms2_plugin.h>
#include <iostream>
#include <string>
#include <stdint.h>
#include <cstdlib>
#include <utils.h>
#include <tiffio.h>


// Include libtiff for reading back TIFF fields

extern "C" void WriteOutputTags(TIFF* out, int Colorspace, int BytesPerSample, int AlphaChannels, int IsPremul);

// Helper test framework (non-terminating assertions)
static int g_failures = 0;
#define TEST_LOG(...) do { printf(__VA_ARGS__); printf("\n"); } while(0)
#define CHECK(cond, msg) do { if(!(cond)) { printf("Test failed: %s\n", msg); ++g_failures; } } while(0)

// Generate a simple unique temporary filename using tmpnam
static std::string TempFileName()
{
    char name[L_tmpnam] = {0};
    if (tmpnam(name) == nullptr) {
        // Fallback to a fixed name (rarely used); test will fail if file exists
        return "test_output.tif";
    }
    return std::string(name);
}

// Clean up a file if it exists
static void CleanupFile(const std::string& path)
{
    remove(path.c_str());
}

// Helper: Read and verify common fields for a given TIFF handle
static bool ReadFieldInt(TIFF* tif, ttag_t tag, int* out)
{
    if (TIFFGetField(tif, tag, out) != 1) return false;
    return true;
}

// Test 1: RGB with no alpha -> verify basic fields (BITSPERSAMPLE, SAMPLESPERPIXEL, PHOTOMETRIC)
void test_WriteOutputTags_RGB_NoAlpha()
{
    // Colorspace: PT_RGB, 1 byte per sample, no alpha
    const int Colorspace = PT_RGB;
    const int BytesPerSample = 1;  // BitsPerSample should be 8
    const int AlphaChannels = 0;
    const int IsPremul = 0;        // unused when AlphaChannels == 0

    std::string path = TempFileName();

    // Create TIFF and invoke function under test
    TIFF* tif = TIFFOpen(path.c_str(), "w");
    CHECK(tif != nullptr, "Failed to create temporary TIFF for RGB_NoAlpha");
    if (tif) {
        WriteOutputTags(tif, Colorspace, BytesPerSample, AlphaChannels, IsPremul);
        TIFFClose(tif);
    }

    // Reopen and verify fields
    TIFF* tifIn = TIFFOpen(path.c_str(), "r");
    CHECK(tifIn != nullptr, "Failed to reopen TIFF for reading RGB_NoAlpha");

    if (tifIn) {
        int BitsPerSample = 0;
        int SamplesPerPixel = 0;
        int Photometric = 0;

        bool okBits = ReadFieldInt(tifIn, TIFFTAG_BITSPERSAMPLE, &BitsPerSample);
        bool okSPP  = ReadFieldInt(tifIn, TIFFTAG_SAMPLESPERPIXEL, &SamplesPerPixel);
        bool okPhoto = ReadFieldInt(tifIn, TIFFTAG_PHOTOMETRIC, &Photometric);

        CHECK(okBits, "BITSPERSAMPLE not found for RGB_NoAlpha");
        CHECK(okSPP, "SAMPLESPERPIXEL not found for RGB_NoAlpha");
        CHECK(okPhoto, "PHOTOMETRIC not found for RGB_NoAlpha");

        if (okBits) CHECK(BitsPerSample == 8, "BitsPerSample should be 8 for 1 BytePerSample");
        if (okSPP)  CHECK(SamplesPerPixel == 3, "SAMPLESPERPIXEL should be 3 for PT_RGB with no alpha");
        if (okPhoto) CHECK(Photometric == PHOTOMETRIC_RGB, "PHOTOMETRIC should be PHOTOMETRIC_RGB for PT_RGB");

        // EXTRASAMPLES should not be present when there is no alpha
        unsigned short extraCount = 0;
        unsigned short* extras = nullptr;
        bool hasExtras = (TIFFGetField(tifIn, TIFFTAG_EXTRASAMPLES, &extraCount, &extras) == 1);
        if (hasExtras) {
            CHECK(false, "EXTRASAMPLES should not be present when AlphaChannels == 0");
        }

        TIFFClose(tifIn);
    }

    CleanupFile(path);
}

// Test 2: RGB with alpha (premultiplied) -> verify EXTRASAMPLES and SPP
void test_WriteOutputTags_RGB_WithAlphaPremul()
{
    // Colorspace: PT_RGB, 1 byte per sample, 1 alpha channel, Premultiplied
    const int Colorspace = PT_RGB;
    const int BytesPerSample = 1;  // 8-bit
    const int AlphaChannels = 1;
    const int IsPremul = 1;

    std::string path = TempFileName();

    TIFF* tif = TIFFOpen(path.c_str(), "w");
    CHECK(tif != nullptr, "Failed to create temporary TIFF for RGB_WithAlphaPremul");
    if (tif) {
        WriteOutputTags(tif, Colorspace, BytesPerSample, AlphaChannels, IsPremul);
        TIFFClose(tif);
    }

    // Reopen and verify fields
    TIFF* tifIn = TIFFOpen(path.c_str(), "r");
    CHECK(tifIn != nullptr, "Failed to reopen TIFF for reading RGB_WithAlphaPremul");
    if (!tifIn) {
        CleanupFile(path);
        return;
    }

    int BitsPerSample = 0;
    int SamplesPerPixel = 0;
    int Photometric = 0;
    bool okBits = ReadFieldInt(tifIn, TIFFTAG_BITSPERSAMPLE, &BitsPerSample);
    bool okSPP  = ReadFieldInt(tifIn, TIFFTAG_SAMPLESPERPIXEL, &SamplesPerPixel);
    bool okPhoto = ReadFieldInt(tifIn, TIFFTAG_PHOTOMETRIC, &Photometric);

    CHECK(okBits, "BITSPERSAMPLE not found for RGB_WithAlphaPremul");
    CHECK(okSPP, "SAMPLESPERPIXEL not found for RGB_WithAlphaPremul");
    CHECK(okPhoto, "PHOTOMETRIC not found for RGB_WithAlphaPremul");

    if (okBits) CHECK(BitsPerSample == 8, "BitsPerSample should be 8 for 1 BytePerSample");
    if (okSPP)  CHECK(SamplesPerPixel == 4, "SAMPLESPERPIXEL should be 4 (3 color + 1 alpha)");
    if (okPhoto) CHECK(Photometric == PHOTOMETRIC_RGB, "PHOTOMETRIC should be PHOTOMETRIC_RGB for PT_RGB with alpha");

    // Check EXTRASAMPLES presence and value
    unsigned short extraCount = 0;
    unsigned short* extras = nullptr;
    bool hasExtras = (TIFFGetField(tifIn, TIFFTAG_EXTRASAMPLES, &extraCount, &extras) == 1);
    if (hasExtras) {
        CHECK(extraCount == 1, "EXTRASAMPLES count should be 1 for single alpha channel");
        if (extras) {
            CHECK(extras[0] == EXTRASAMPLE_ASSOCALPHA, "EXTRASAMPLES[0] should be EXTRASAMPLE_ASSOCALPHA when IsPremul == 1");
        } else {
            CHECK(false, "EXTRASAMPLES array pointer is null");
        }
    } else {
        CHECK(false, "EXTRASAMPLES should be present when AlphaChannels > 0");
    }

    TIFFClose(tifIn);
    CleanupFile(path);
}

// Test 3: Gray with 16 bits per sample -> verify BITSPERSAMPLE and PHOTOMETRIC
void test_WriteOutputTags_Gray16()
{
    // Colorspace: PT_GRAY, 2 bytes per sample, no alpha
    const int Colorspace = PT_GRAY;
    const int BytesPerSample = 2;  // 16-bit
    const int AlphaChannels = 0;
    const int IsPremul = 0;

    std::string path = TempFileName();

    TIFF* tif = TIFFOpen(path.c_str(), "w");
    CHECK(tif != nullptr, "Failed to create temporary TIFF for Gray16");
    if (tif) {
        WriteOutputTags(tif, Colorspace, BytesPerSample, AlphaChannels, IsPremul);
        TIFFClose(tif);
    }

    TIFF* tifIn = TIFFOpen(path.c_str(), "r");
    CHECK(tifIn != nullptr, "Failed to reopen TIFF for reading Gray16");
    if (tifIn) {
        int BitsPerSample = 0;
        int SamplesPerPixel = 0;
        int Photometric = 0;

        bool okBits = ReadFieldInt(tifIn, TIFFTAG_BITSPERSAMPLE, &BitsPerSample);
        bool okSPP  = ReadFieldInt(tifIn, TIFFTAG_SAMPLESPERPIXEL, &SamplesPerPixel);
        bool okPhoto = ReadFieldInt(tifIn, TIFFTAG_PHOTOMETRIC, &Photometric);

        CHECK(okBits, "BITSPERSAMPLE not found for Gray16");
        CHECK(okSPP, "SAMPLESPERPIXEL not found for Gray16");
        CHECK(okPhoto, "PHOTOMETRIC not found for Gray16");

        if (okBits) CHECK(BitsPerSample == 16, "BitsPerSample should be 16 for 2-byte samples");
        if (okSPP)  CHECK(SamplesPerPixel == 1, "SAMPLESPERPIXEL should be 1 for PT_GRAY");
        if (okPhoto) CHECK(Photometric == PHOTOMETRIC_MINISBLACK, "PHOTOMETRIC should be MINISBLACK for PT_GRAY");

        TIFFClose(tifIn);
    }

    CleanupFile(path);
}

// Test 4: Lab with 8 bits per sample -> verify PHOTOMETRIC_CIELAB
void test_WriteOutputTags_Lab8()
{
    // Colorspace: PT_Lab, 1 byte per sample, no alpha
    const int Colorspace = PT_Lab;
    const int BytesPerSample = 1;
    const int AlphaChannels = 0;
    const int IsPremul = 0;

    std::string path = TempFileName();

    TIFF* tif = TIFFOpen(path.c_str(), "w");
    CHECK(tif != nullptr, "Failed to create temporary TIFF for Lab8");
    if (tif) {
        WriteOutputTags(tif, Colorspace, BytesPerSample, AlphaChannels, IsPremul);
        TIFFClose(tif);
    }

    TIFF* tifIn = TIFFOpen(path.c_str(), "r");
    CHECK(tifIn != nullptr, "Failed to reopen TIFF for reading Lab8");
    if (tifIn) {
        int Photometric = 0;
        bool okPhoto = ReadFieldInt(tifIn, TIFFTAG_PHOTOMETRIC, &Photometric);

        // For PT_Lab with 8-bit samples, the code uses PHOTOMETRIC_CIELAB
        CHECK(okPhoto, "PHOTOMETRIC not found for Lab8");
        if (okPhoto) CHECK(Photometric == PHOTOMETRIC_CIELAB, "PHOTOMETRIC should be CIELAB for PT_Lab with 8-bit samples");

        TIFFClose(tifIn);
    }

    CleanupFile(path);
}

// Test 5: MCH2 (multi-ink) without alpha -> verify SEPARATED, INKSET and NUMBEROFINKS
void test_WriteOutputTags_MCH2_NoAlpha()
{
    // Colorspace: PT_MCH2, 1 byte per sample, 0 alpha
    const int Colorspace = PT_MCH2;
    const int BytesPerSample = 1;
    const int AlphaChannels = 0;
    const int IsPremul = 0;

    std::string path = TempFileName();

    TIFF* tif = TIFFOpen(path.c_str(), "w");
    CHECK(tif != nullptr, "Failed to create temporary TIFF for MCH2_NoAlpha");
    if (tif) {
        WriteOutputTags(tif, Colorspace, BytesPerSample, AlphaChannels, IsPremul);
        TIFFClose(tif);
    }

    TIFF* tifIn = TIFFOpen(path.c_str(), "r");
    CHECK(tifIn != nullptr, "Failed to reopen TIFF for reading MCH2_NoAlpha");
    if (tifIn) {
        int Photometric = 0;
        int InkSet = 0;
        int NumberOfInks = 0;

        bool okPhoto = ReadFieldInt(tifIn, TIFFTAG_PHOTOMETRIC, &Photometric);
        bool okInkSet = ReadFieldInt(tifIn, TIFFTAG_INKSET, &InkSet);
        bool okNumInks = ReadFieldInt(tifIn, TIFFTAG_NUMBEROFINKS, &NumberOfInks);

        CHECK(okPhoto, "PHOTOMETRIC not found for MCH2_NoAlpha");
        if (okPhoto) CHECK(Photometric == PHOTOMETRIC_SEPARATED, "PHOTOMETRIC should be SEPARATED for PT_MCH2");

        // INKSET should be MULTIINK for non-alpha multi-ink cases
        if (okInkSet) CHECK(InkSet == INKSET_MULTIINK, "INKSET should be MULTIINK for PT_MCH2 when no alpha");
        else CHECK(false, "INKSET field missing for PT_MCH2");

        // NUMBEROFINKS should be > 0
        if (okNumInks) CHECK(NumberOfInks > 0, "NUMBEROFINKS should be > 0 for PT_MCH2");
        else CHECK(false, "NUMBEROFINKS field missing for PT_MCH2");

        TIFFClose(tifIn);
    }

    CleanupFile(path);
}

// Entry point: run all tests
int main()
{
    // Step: Run tests
    test_WriteOutputTags_RGB_NoAlpha();
    test_WriteOutputTags_RGB_WithAlphaPremul();
    test_WriteOutputTags_Gray16();
    test_WriteOutputTags_Lab8();
    test_WriteOutputTags_MCH2_NoAlpha();

    // Summary
    if (g_failures == 0) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Total failures: %d\n", g_failures);
        return 1;
    }
}