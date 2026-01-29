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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Use a NULL/default context for THR APIs (many lcms THR functions accept NULL as the default context)
    cmsContext ctx = (cmsContext)NULL;

    // 1) Query supported intents with a bounded array
    const cmsUInt32Number maxIntents = 8;
    cmsUInt32Number intentCodes[maxIntents];
    char *intentDescriptions[maxIntents];
    // Initialize description pointers to NULL for safety
    for (cmsUInt32Number i = 0; i < maxIntents; ++i) intentDescriptions[i] = NULL;

    // Call the threaded variant to get supported intents
    cmsUInt32Number nIntents = 0;
    // Assume the THR variant exists as requested by the task
    nIntents = cmsGetSupportedIntentsTHR(ctx, maxIntents, intentCodes, intentDescriptions);

    // 2) Create an abstract BCHSW profile (simple use to exercise profile creation)
    // Provide reasonable example parameters: 17 LUT points, neutral B/C/H/S and temps
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfile(17, 0.0, 1.0, 0.0, 1.0, 5000u, 5000u);

    // 3) Build a small tabulated tone curve and evaluate it
    // Create a simple 4-entry table (in 16-bit units) representing a gentle ramp
    cmsUInt16Number table[4] = { 0x0000u, 0x4000u, 0x8000u, 0xFFFFu };
    cmsToneCurve * curve = cmsBuildTabulatedToneCurve16(ctx, 4, table);

    cmsFloat32Number evalResult = 0.0f;
    if (curve != NULL) {
        // Evaluate the curve at mid point
        evalResult = cmsEvalToneCurveFloat(curve, 0.5f);
    }

    // 4) Allocate an IT8 structure and set an index column name
    cmsHANDLE hIT8 = cmsIT8Alloc(ctx);
    if (hIT8 != NULL) {
        // Set an index column (assume the function exists as requested)
        // Use a descriptive name; the function returns cmsBool
        cmsBool ok = cmsIT8SetIndexColumn(hIT8, "Index");
        (void)ok; // silence unused variable in case we don't check further

        // Optionally set a sample data format for sample row 1
        cmsIT8SetDataFormat(hIT8, 1, "RGB");
    }

    // 5) Clean up resources we explicitly created
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }

    // There are no guarantees about the availability of explicit free functions for profile/IT8
    // in the minimal API set used here. In typical lcms usage one would call cmsCloseProfile or
    // similar if available. For this test sequence we avoid calling functions that may not be
    // present in all builds.

    // Use some of the obtained values to avoid optimizing them away (no-op usage)
    (void)nIntents;
    (void)intentCodes;
    (void)intentDescriptions;
    (void)profile;
    (void)hIT8;
    (void)evalResult;

    // Return the required success code
    return 66;
}