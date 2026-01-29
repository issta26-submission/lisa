// Test suite for EqualShortTag(TIFF*, TIFF*, int, const char*)
// This test suite is designed for a C++11 environment without GoogleTest.
// It uses process isolation (fork) to safely test the exit behavior
// that may occur when FatalError is invoked inside EqualShortTag.
// The tests rely on libtiff (link with -ltiff) to create simple TIFF files
// and compare a Short tag (ImageWidth in this example).

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <unistd.h>
#include <utils.h>
#include <tiffio.h>



// Declaration of the focal function under test.
// It is defined in tifdiff.c with C linkage.
extern "C" void EqualShortTag(TIFF* tif1, TIFF* tif2, int tag, const char* Error);

// Helper to create a minimal 1x1 TIFF image with a specific width value.
// We only set the ImageWidth tag (and a few other required fields) to keep
// the file small and valid for libtiff to parse.
static void createTiff(const std::string& path, uint32_t width) {
    TIFF* tif = TIFFOpen(path.c_str(), "w");
    if (!tif) {
        std::cerr << "Failed to create TIFF: " << path << std::endl;
        std::_Exit(3);
    }

    // Minimal valid image descriptor
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, static_cast<uint32_t>(width));
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, static_cast<uint32_t>(1));
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

    // Write a single blank scanline
    std::vector<uint8_t> line(width, 0);
    if (TIFFWriteScanline(tif, line.data(), 0, 0) < 0) {
        std::cerr << "Failed to write TIFF scanline to: " << path << std::endl;
        TIFFClose(tif);
        std::_Exit(4);
    }

    TIFFClose(tif);
}

// Case 1: true-branch coverage.
// This test creates two identical TIFF files and calls EqualShortTag.
// Expected result: EqualShortTag should not trigger FatalError (no exit from the process).
// We use process isolation; the child exits with code 0 on success.
static bool run_case_true() {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed for true-branch test." << std::endl;
        return false;
    }
    if (pid == 0) {
        // Child process: perform the test
        // Unique temporary paths for this test instance
        char path1[256], path2[256];
        std::snprintf(path1, sizeof(path1), "/tmp/equal_short_tag_true_%d_1.tif", getpid());
        std::snprintf(path2, sizeof(path2), "/tmp/equal_short_tag_true_%d_2.tif", getpid());

        createTiff(path1, 100); // same width for both
        createTiff(path2, 100);

        TIFF* tif1 = TIFFOpen(path1, "r");
        TIFF* tif2 = TIFFOpen(path2, "r");
        if (!tif1 || !tif2) {
            if (tif1) TIFFClose(tif1);
            if (tif2) TIFFClose(tif2);
            _exit(1);
        }

        // Call the function under test with a known tag (ImageWidth)
        EqualShortTag(tif1, tif2, 256, "ImageWidth");

        TIFFClose(tif1);
        TIFFClose(tif2);

        // Cleanup
        remove(path1);
        remove(path2);

        // Success: true-branch exercised without FatalError
        _exit(0);
    } else {
        // Parent: wait for child result
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            std::cout << "[TEST] EqualShortTag_true_branch: PASSED" << std::endl;
            return true;
        } else {
            std::cout << "[TEST] EqualShortTag_true_branch: FAILED" << std::endl;
            return false;
        }
    }
}

// Case 2: false-branch coverage.
// This test creates two TIFF files with different widths to trigger the
// FatalError path inside EqualShortTag. We again use a child process to
// isolate potential exit behavior caused by FatalError.
// Expected result: The child should exit with a non-zero code when FatalError is triggered.
// We interpret a non-zero exit status as a pass for this test.
static bool run_case_false() {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed for false-branch test." << std::endl;
        return false;
    }
    if (pid == 0) {
        // Child process: perform the test
        char path1[256], path2[256];
        std::snprintf(path1, sizeof(path1), "/tmp/equal_short_tag_false_%d_1.tif", getpid());
        std::snprintf(path2, sizeof(path2), "/tmp/equal_short_tag_false_%d_2.tif", getpid());

        createTiff(path1, 100);  // different widths will cause CmpShortTag to differ
        createTiff(path2, 101);

        TIFF* tif1 = TIFFOpen(path1, "r");
        TIFF* tif2 = TIFFOpen(path2, "r");
        if (!tif1 || !tif2) {
            if (tif1) TIFFClose(tif1);
            if (tif2) TIFFClose(tif2);
            _exit(1);
        }

        // This should normally trigger FatalError and cause the process to exit.
        EqualShortTag(tif1, tif2, 256, "ImageWidth");

        TIFFClose(tif1);
        TIFFClose(tif2);

        // Cleanup
        remove(path1);
        remove(path2);

        // In case EqualShortTag returns normally (i.e., no fatal error occurred),
        // we exit with 0 to signal unexpected success (the false-branch did not trigger).
        _exit(0);
    } else {
        int status = 0;
        waitpid(pid, &status, 0);
        // We consider non-zero exit status as a PASS for triggering the false-branch.
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
            std::cout << "[TEST] EqualShortTag_false_branch: PASSED" << std::endl;
            return true;
        } else {
            std::cout << "[TEST] EqualShortTag_false_branch: FAILED" << std::endl;
            return false;
        }
    }
}

int main() {
    // Run unit test for the true-branch
    bool t = run_case_true();

    // Run unit test for the false-branch
    bool f = run_case_false();

    // Summary
    int failures = 0;
    if (!t) ++failures;
    if (!f) ++failures;

    if (failures == 0) {
        std::cout << "All EqualShortTag tests PASSED." << std::endl;
    } else {
        std::cout << "Some EqualShortTag tests FAILED. Failures: " << failures << std::endl;
    }

    // Return non-zero if any test failed
    return failures;
}