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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Local variables using lcms types
    cmsContext ctx = NULL;
    cmsToneCurve * curve = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsCIEXYZ blackPoint;
    cmsBool detected = 0;

    // Try to obtain a context by duplicating the default (NULL). If the
    // implementation does not require a context, cmsBuildGamma may accept NULL.
    ctx = cmsDupContext(NULL, NULL);

    // Build a simple gamma tone curve (2.2)
    curve = cmsBuildGamma(ctx, 2.2);
    if (curve == NULL) {
        // cleanup context if created and fail
        if (ctx) cmsDeleteContext(ctx);
        return 1;
    }

    // Create an XYZ profile
    hProfile = cmsCreateXYZProfile();
    if (hProfile == NULL) {
        // No specific API provided here to free the tone curve or profile,
        // but free the context if we created one.
        if (ctx) cmsDeleteContext(ctx);
        return 2;
    }

    // Initialize black point structure and attempt detection
    blackPoint.X = blackPoint.Y = blackPoint.Z = 0.0;
    detected = cmsDetectBlackPoint(&blackPoint, hProfile, 0, 0);

    // We don't require detection to succeed for the test to be considered OK;
    // the point of this sequence is exercising the APIs. Still, we keep the
    // result available (could be logged in a real test).
    (void) detected; // silence unused-variable warnings in some builds

    // Clean up the context we duplicated. (Assumes cmsDeleteContext accepts NULL.)
    if (ctx) cmsDeleteContext(ctx);

    // All API calls required by the sequence were invoked successfully.
    return 66;
}