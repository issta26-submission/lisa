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
//<ID> 309
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Allocate a Gamut Boundary Description handle (context NULL is allowed)
    cmsHANDLE gbd = cmsGBDAlloc(NULL);
    if (gbd == NULL) {
        return 1;
    }

    // Allocate an IT8 handle to demonstrate cmsIT8SetPropertyDbl usage
    cmsHANDLE it8 = cmsIT8Alloc(NULL);
    if (it8 == NULL) {
        // No dedicated free for gbd in provided API list; just return error
        return 2;
    }

    // Set a double property on the IT8 handle
    cmsFloat64Number propVal = 3.14159;
    if (!cmsIT8SetPropertyDbl(it8, "ExampleDoubleProperty", propVal)) {
        cmsIT8Free(it8);
        return 3;
    }

    // Build a simple parametric tone curve (type 1 with one parameter)
    cmsFloat64Number params[1];
    params[0] = 1.0; // simple parameter (meaning depends on parametric type)
    cmsToneCurve * curve = cmsBuildParametricToneCurve(NULL, 1, params);
    if (curve == NULL) {
        cmsIT8Free(it8);
        return 4;
    }

    // Evaluate the tone curve at a mid-level 16-bit input value
    cmsUInt16Number input = 32768;
    cmsUInt16Number output = cmsEvalToneCurve16(curve, input);
    (void)output; // suppress unused variable warning; value was computed

    // Free the tone curve
    cmsFreeToneCurve(curve);

    // Clean up the IT8 handle
    cmsIT8Free(it8);

    // Note: gbd was allocated but no explicit free was available in the provided API subset.
    // Return 66 to indicate success as requested.
    return 66;
}