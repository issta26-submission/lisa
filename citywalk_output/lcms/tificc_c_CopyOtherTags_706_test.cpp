// Test suite for CopyOtherTags in tificc.c using real libtiff (no GoogleTest).
// This test uses in-memory (temporary file) TIFFs to exercise CopyOtherTags
// and its interaction with various fields, including the predictor branch
// controlled by PixelDepth and compression values.
//
// Notes:
// - The test relies on the real libtiff constants (TIFFTAG_* and COMPRESSION_*).
// - PixelDepth is an extern int defined in the focal module; tests set it via extern.
// - We avoid terminating assertions; instead, we accumulate errors and print a summary.
// - The tests are designed to be executed by the program's main function (no GTest).

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <lcms2_plugin.h>
#include <vector>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <stdint.h>
#include <cassert>
#include <cstdlib>
#include <unistd.h>
#include <utils.h>
#include <tiffio.h>



extern int PixelDepth;  // PixelDepth defined in the focal module (tificc.c)

extern "C" void CopyOtherTags(TIFF* in, TIFF* out); // focal method

// Simple test harness
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total; \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << " (line " << __LINE__ << ")\n"; \
        ++g_failed; \
    } \
} while(0)

#define PRINT_HEADER(title) \
    std::cout << "=== " << (title) << " ===" << std::endl;

static std::string temp_path(const std::string& base)
{
    // Create a temp file path by using mkstemp style: we create and immediately remove
    char tmpl[256];
    std::snprintf(tmpl, sizeof(tmpl), "/tmp/%sXXXXXX.tiff", base.c_str());
    // create the file to reserve a unique path
    int fd = mkstemp(tmpl);
    if (fd >= 0) {
        close(fd);
        // remove it; we'll recreate with TIFFOpen later
        unlink(tmpl);
    }
    return std::string(tmpl);
}

// Helper to check if file exists
static bool file_exists(const std::string& path)
{
    struct stat st;
    return stat(path.c_str(), &st) == 0;
}

// Helper to perform a single CopyOtherTags test with given input field setup and expected outcome
static void run_test_basic_predictor_copy()
{
    PRINT_HEADER("Test 1: Basic field copy including predictor when PixelDepth != 32 and compression matches");

    // Prepare two temporary TIFF files: in (source) and out (destination)
    std::string inPath = temp_path("tificc_in");
    std::string outPath = temp_path("tificc_out");
    // Ensure unique names
    inPath += "1.tiff";
    outPath += "1.tiff";

    // Create input TIFF (write mode)
    TIFF* inTiff = TIFFOpen(inPath.c_str(), "w");
    if (!inTiff) {
        std::cerr << "Failed to create input TIFF for Test 1" << std::endl;
        ++g_failed;
        ++g_total;
        return;
    }

    // Setup input fields to cover multiple tags
    // Basic size fields
    TIFFSetField(inTiff, TIFFTAG_IMAGEWIDTH, (cmsUInt32Number)100);
    TIFFSetField(inTiff, TIFFTAG_IMAGELENGTH, (cmsUInt32Number)200);

    // Subfile type / Planar config / Compression
    TIFFSetField(inTiff, TIFFTAG_SUBFILETYPE, (cmsUInt32Number)0);
    TIFFSetField(inTiff, TIFFTAG_PLANARCONFIG, (cmsUInt16)1);
    TIFFSetField(inTiff, TIFFTAG_COMPRESSION, (unsigned short)34925); // COMPRESSION_LZMA (typical value per code comment)

    // Predictor we want copied (when conditions met)
    TIFFSetField(inTiff, TIFFTAG_PREDICTOR, (cmsUInt16)7);

    // Additional fields
    TIFFSetField(inTiff, TIFFTAG_THRESHHOLDING, (cmsUInt16)1);
    TIFFSetField(inTiff, TIFFTAG_FILLORDER, (cmsUInt16)2);
    TIFFSetField(inTiff, TIFFTAG_ORIENTATION, (cmsUInt16)1);
    TIFFSetField(inTiff, TIFFTAG_MINSAMPLEVALUE, (cmsUInt16)0);
    TIFFSetField(inTiff, TIFFTAG_MAXSAMPLEVALUE, (cmsUInt16)255);
    TIFFSetField(inTiff, TIFFTAG_XRESOLUTION, (double)300.0);
    TIFFSetField(inTiff, TIFFTAG_YRESOLUTION, (double)200.0);
    TIFFSetField(inTiff, TIFFTAG_RESOLUTIONUNIT, (cmsUInt16)2);
    TIFFSetField(inTiff, TIFFTAG_ROWSPERSTRIP, (cmsUInt32Number)64);

    TIFFSetField(inTiff, TIFFTAG_XPOSITION, (double)0.5);
    TIFFSetField(inTiff, TIFFTAG_YPOSITION, (double)0.75);
    TIFFSetField(inTiff, TIFFTAG_IMAGEDEPTH, (cmsUInt32Number)8);
    TIFFSetField(inTiff, TIFFTAG_TILEDEPTH, (cmsUInt32Number)4);
    TIFFSetField(inTiff, TIFFTAG_TILEWIDTH, (cmsUInt32Number)16);
    TIFFSetField(inTiff, TIFFTAG_TILELENGTH, (cmsUInt32Number)8);

    // Strings
    TIFFSetField(inTiff, TIFFTAG_ARTIST, (char*)"ArtistA");
    TIFFSetField(inTiff, TIFFTAG_IMAGEDESCRIPTION, (char*)"DescriptionA");
    TIFFSetField(inTiff, TIFFTAG_MAKE, (char*)"MakerA");
    TIFFSetField(inTiff, TIFFTAG_MODEL, (char*)"ModelA");
    TIFFSetField(inTiff, TIFFTAG_DATETIME, (char*)"2024:01:01 00:00:00");
    TIFFSetField(inTiff, TIFFTAG_HOSTCOMPUTER, (char*)"HostA");
    TIFFSetField(inTiff, TIFFTAG_PAGENAME, (char*)"PageA");
    TIFFSetField(inTiff, TIFFTAG_DOCUMENTNAME, (char*)"DocA");

    TIFFClose(inTiff);

    // Prepare for reading input
    inTiff = TIFFOpen(inPath.c_str(), "r");
    if (!inTiff) {
        std::cerr << "Failed to reopen input TIFF for Test 1" << std::endl;
        ++g_failed;
        ++g_total;
        return;
    }

    // Prepare output TIFF
    TIFF* outTiff = TIFFOpen(outPath.c_str(), "w");
    if (!outTiff) {
        std::cerr << "Failed to create output TIFF for Test 1" << std::endl;
        TIFFClose(inTiff);
        ++g_failed;
        ++g_total;
        return;
    }

    // Ensure PixelDepth is non-32 to exercise predictor copying
    PixelDepth = 24;

    // Run the focal function
    CopyOtherTags(inTiff, outTiff);

    // Close input to flush reads
    TIFFClose(inTiff);
    // Reopen output for reading to verify
    TIFF* outVerify = TIFFOpen(outPath.c_str(), "r");
    if (!outVerify) {
        std::cerr << "Failed to open output TIFF for verification (Test 1)" << std::endl;
        TIFFClose(outTiff);
        ++g_failed;
        ++g_total;
        return;
    }

    // Verify a subset of fields
    cmsUInt32Number w = 0;
    cmsUInt32Number h = 0;
    unsigned short compression = 0;
    unsigned short predictor = 0;
    char* artist = nullptr;
    char* descr = nullptr;

    TIFFGetField(outVerify, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(outVerify, TIFFTAG_IMAGELENGTH, &h);
    TIFFGetField(outVerify, TIFFTAG_COMPRESSION, &compression);
    TIFFGetField(outVerify, TIFFTAG_PREDICTOR, &predictor);
    TIFFGetField(outVerify, TIFFTAG_ARTIST, &artist);
    TIFFGetField(outVerify, TIFFTAG_IMAGEDESCRIPTION, &descr);

    EXPECT_TRUE(w == 100, "IMAGEWIDTH should be 100");
    EXPECT_TRUE(h == 200, "IMAGELENGTH should be 200");
    EXPECT_TRUE(compression == 34925, "COMPRESSION should be 34925 (LZMA)");
    EXPECT_TRUE(predictor == 7, "PREDICTOR should be copied (7)");
    EXPECT_TRUE(artist && std::strcmp(artist, "ArtistA") == 0, "ARTIST should be 'ArtistA'");
    EXPECT_TRUE(descr && std::strcmp(descr, "DescriptionA") == 0, "IMAGEDESCRIPTION should be 'DescriptionA'");

    TIFFClose(outVerify);
    TIFFClose(outTiff);

    // Cleanup
    unlink(inPath.c_str());
    unlink(outPath.c_str());

    ++g_total;
}

// Test 2: When PixelDepth == 32, predictor should not be copied even if compression matches
static void run_test_predictor_not_copied_with_pixeldepth32()
{
    PRINT_HEADER("Test 2: Predictor should not be copied when PixelDepth == 32 (even if compression matches)");

    std::string inPath = temp_path("tificc_in2");
    std::string outPath = temp_path("tificc_out2");
    inPath += "2.tiff";
    outPath += "2.tiff";

    TIFF* inTiff = TIFFOpen(inPath.c_str(), "w");
    if (!inTiff) {
        std::cerr << "Failed to create input TIFF for Test 2" << std::endl;
        ++g_failed;
        ++g_total;
        return;
    }

    TIFFSetField(inTiff, TIFFTAG_IMAGEWIDTH, (cmsUInt32Number)120);
    TIFFSetField(inTiff, TIFFTAG_IMAGELENGTH, (cmsUInt32Number)80);
    TIFFSetField(inTiff, TIFFTAG_SUBFILETYPE, (cmsUInt32Number)0);
    TIFFSetField(inTiff, TIFFTAG_PLANARCONFIG, (cmsUInt16)1);
    TIFFSetField(inTiff, TIFFTAG_COMPRESSION, (unsigned short)34925); // matching LZMA

    TIFFSetField(inTiff, TIFFTAG_PREDICTOR, (cmsUInt16)9);

    TIFFSetField(inTiff, TIFFTAG_ARTIST, (char*)"ArtistB");
    TIFFSetField(inTiff, TIFFTAG_IMAGEDESCRIPTION, (char*)"DescB");

    TIFFClose(inTiff);

    inTiff = TIFFOpen(inPath.c_str(), "r");
    if (!inTiff) {
        std::cerr << "Failed to reopen input TIFF for Test 2" << std::endl;
        ++g_failed;
        ++g_total;
        return;
    }

    TIFF* outTiff = TIFFOpen(outPath.c_str(), "w");
    if (!outTiff) {
        std::cerr << "Failed to create output TIFF for Test 2" << std::endl;
        TIFFClose(inTiff);
        ++g_failed;
        ++g_total;
        return;
    }

    // Force PixelDepth to 32 to disable predictor copy
    PixelDepth = 32;

    CopyOtherTags(inTiff, outTiff);

    TIFFClose(inTiff);
    TIFFClose(outTiff);

    // Reopen output to verify predictor not copied
    TIFF* outVerify = TIFFOpen(outPath.c_str(), "r");
    if (!outVerify) {
        std::cerr << "Failed to open output TIFF for verification (Test 2)" << std::endl;
        ++g_failed;
        ++g_total;
        return;
    }

    unsigned short predictor = 0;
    cmsUInt32Number w = 0, h = 0;
    char* artist = nullptr;
    char* descr = nullptr;

    TIFFGetField(outVerify, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(outVerify, TIFFTAG_IMAGELENGTH, &h);
    TIFFGetField(outVerify, TIFFTAG_PREDICTOR, &predictor); // may be absent if not copied
    TIFFGetField(outVerify, TIFFTAG_ARTIST, &artist);
    TIFFGetField(outVerify, TIFFTAG_IMAGEDESCRIPTION, &descr);

    // Predictor should not be present (default values are not set by CopyOtherTags)
    if (TIFFGetField(outVerify, TIFFTAG_PREDICTOR, &predictor) == 0) {
        // predictor not set, as expected
        EXPECT_TRUE(true, "PREDICTOR not set in output (as expected with PixelDepth==32)");
    } else {
        // If set, it's a failure for this test
        EXPECT_TRUE(false, "PREDICTOR unexpectedly copied when PixelDepth == 32");
    }

    // Basic fields should still be copied
    EXPECT_TRUE(w == 120, "IMAGEWIDTH should be 120");
    EXPECT_TRUE(h == 80, "IMAGELENGTH should be 80");
    EXPECT_TRUE(artist && std::strcmp(artist, "ArtistB") == 0, "ARTIST should be 'ArtistB'");
    EXPECT_TRUE(descr && std::strcmp(descr, "DescB") == 0, "IMAGEDESCRIPTION should be 'DescB'");

    TIFFClose(outVerify);

    // Cleanup
    unlink(inPath.c_str());
    unlink(outPath.c_str());

    ++g_total;
}

// Main entry: run tests
int main()
{
    // Initialize; PixelDepth is expected to be defined in focal module
    // Do not terminate on first failure; report all
    run_test_basic_predictor_copy();
    run_test_predictor_not_copied_with_pixeldepth32();

    // Summary
    std::cout << "Tests run: " << g_total << ", Failures: " << g_failed << std::endl;
    if (g_failed > 0) {
        return 1;
    } else {
        return 0;
    }
}