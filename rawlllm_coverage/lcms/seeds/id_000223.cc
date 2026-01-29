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
//<ID> 223
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Create a context for threaded APIs
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 1;

    // Build a simple parametric tone curve (type 1 with one parameter)
    cmsFloat64Number params[1] = { 2.2 }; // typical gamma-like value
    cmsToneCurve * curve = cmsBuildParametricToneCurve(ctx, 1, params);
    if (!curve) {
        cmsDeleteContext(ctx);
        return 1;
    }

    // Create a NULL profile
    cmsHPROFILE nullProfile = cmsCreateNULLProfile();
    if (!nullProfile) {
        cmsFreeToneCurve(curve);
        cmsDeleteContext(ctx);
        return 1;
    }

    // Create a BCHSW abstract profile using the threaded variant
    // (use conservative values for brightness/contrast/hue/saturation)
    cmsHPROFILE bchProfile = cmsCreateBCHSWabstractProfileTHR(ctx,
                                                              (cmsUInt32Number)17,  // nLUTPoints
                                                              (cmsFloat64Number)0.0, // Bright
                                                              (cmsFloat64Number)1.0, // Contrast
                                                              (cmsFloat64Number)0.0, // Hue
                                                              (cmsFloat64Number)1.0, // Saturation
                                                              (cmsUInt32Number)0,    // TempSrc
                                                              (cmsUInt32Number)0);   // TempDest
    // bchProfile may be NULL on failure, but proceed to demonstrate IT8 save below.
    // Allocate an IT8 handle and add a small comment to it
    cmsHANDLE it8 = cmsIT8Alloc(ctx);
    if (!it8) {
        // cleanup
        cmsFreeToneCurve(curve);
        cmsDeleteContext(ctx);
        return 1;
    }

    // Add a comment (ignore result; if it fails we can still try to save)
    cmsIT8SetComment(it8, "lcms fuzz driver test");

    // First call to get required buffer size by passing NULL MemPtr
    cmsUInt32Number bytesNeeded = 0;
    if (!cmsIT8SaveToMem(it8, NULL, &bytesNeeded) || bytesNeeded == 0) {
        // cleanup
        cmsFreeToneCurve(curve);
        cmsDeleteContext(ctx);
        return 1;
    }

    // Allocate memory and save the IT8 structure into it
    void * buffer = malloc((size_t)bytesNeeded);
    if (!buffer) {
        cmsFreeToneCurve(curve);
        cmsDeleteContext(ctx);
        return 1;
    }

    if (!cmsIT8SaveToMem(it8, buffer, &bytesNeeded)) {
        free(buffer);
        cmsFreeToneCurve(curve);
        cmsDeleteContext(ctx);
        return 1;
    }

    // Successfully created and saved IT8 structure in memory; clean up
    free(buffer);
    cmsFreeToneCurve(curve);
    // Note: there are no explicit profile free functions used here from the provided API list.
    // Deleting the context to release thread-local resources and plugins
    cmsDeleteContext(ctx);

    // Indicate success as requested
    return 66;
}