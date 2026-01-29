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
//<ID> 312
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Use a NULL context (default context)
    cmsContext ctx = NULL;

    // Build three identical gamma tone curves (one per channel)
    cmsToneCurve * curves[3] = { nullptr, nullptr, nullptr };
    for (int i = 0; i < 3; ++i) {
        curves[i] = cmsBuildGamma(ctx, 2.2);
        if (curves[i] == nullptr) {
            // If creation failed, free any already created curves (fill missing with NULLs)
            cmsToneCurve *cleanup[3] = { curves[0], curves[1], curves[2] };
            cmsFreeToneCurveTriple(cleanup);
            return 1;
        }
    }

    // Allocate a stage that applies three tone curves (one per channel)
    cmsStage * stage = cmsStageAllocToneCurves(ctx, 3, curves);
    if (stage == nullptr) {
        cmsFreeToneCurveTriple(curves);
        return 2;
    }

    // Create a minimal profile to query PCS and tag signatures
    cmsHPROFILE hProfile = cmsCreateNULLProfileTHR(ctx);
    if (hProfile == nullptr) {
        cmsStageFree(stage);
        cmsFreeToneCurveTriple(curves);
        return 3;
    }

    // Query the Profile Connection Space (PCS)
    cmsColorSpaceSignature pcs = cmsGetPCS(hProfile);
    (void) pcs; // silence unused variable warning if any

    // Query the first tag signature (index 0)
    cmsTagSignature sig0 = cmsGetTagSignature(hProfile, (cmsUInt32Number)0);
    (void) sig0; // silence unused variable warning

    // Clean up stage and tone curves
    cmsStageFree(stage);
    cmsFreeToneCurveTriple(curves);

    // Success
    return 66;
}