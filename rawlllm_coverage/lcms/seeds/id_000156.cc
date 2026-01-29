#include <lcms2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a context required by some IT8 helper functions
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (!ctx) return 1;

    // Allocate an IT8 handle
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (!hIT8) {
        cmsDeleteContext(ctx);
        return 2;
    }

    // Set a sample data row/column in the IT8 structure
    if (!cmsIT8SetDataRowCol(hIT8, 0, 0, "SampleValue")) {
        cmsDeleteContext(ctx);
        return 3;
    }

    // Query required buffer size by calling save with NULL buffer
    cmsUInt32Number bytesNeeded = 0;
    if (!cmsIT8SaveToMem(hIT8, nullptr, &bytesNeeded) || bytesNeeded == 0) {
        cmsDeleteContext(ctx);
        return 4;
    }

    // Allocate memory and actually save IT8 data into it
    void * buffer = malloc((size_t)bytesNeeded);
    if (!buffer) {
        cmsDeleteContext(ctx);
        return 5;
    }
    if (!cmsIT8SaveToMem(hIT8, buffer, &bytesNeeded)) {
        free(buffer);
        cmsDeleteContext(ctx);
        return 6;
    }
    free(buffer);

    // Create a NULL profile (required by the task)
    cmsHPROFILE nullProfile = cmsCreateNULLProfile();
    if (!nullProfile) {
        cmsDeleteContext(ctx);
        return 7;
    }

    // Build a simple gamma curve and compute its reverse
    cmsToneCurve * gamma = cmsBuildGamma(ctx, 2.2);
    if (!gamma) {
        cmsDeleteContext(ctx);
        return 8;
    }

    cmsToneCurve * reverse = cmsReverseToneCurveEx(256, gamma);
    if (reverse) {
        cmsFreeToneCurve(reverse);
    }

    // Free the original tone curve
    cmsFreeToneCurve(gamma);

    // Clean up context
    cmsDeleteContext(ctx);

    // Success
    return 66;
}