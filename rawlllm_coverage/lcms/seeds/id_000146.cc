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
//<ID> 146
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Allocate an IT8 handle (context set to 0/default)
    cmsHANDLE hIT8 = cmsIT8Alloc((cmsContext)0);

    // Prepare and store a test double value in the IT8 table at row 0, col 0
    cmsFloat64Number setValue = 123.456;
    (void) cmsIT8SetDataRowColDbl(hIT8, 0, 0, setValue);

    // Retrieve the stored value using the required getter
    cmsFloat64Number retrieved = cmsIT8GetDataRowColDbl(hIT8, 0, 0);

    // Build a simple gamma tone curve and then reverse it
    cmsToneCurve * curve = cmsBuildGamma((cmsContext)0, 2.2);
    cmsToneCurve * reversed = nullptr;
    if (curve != nullptr) {
        reversed = cmsReverseToneCurve(curve);
    }

    // Query an ICC color space signature from a small integer notation
    cmsColorSpaceSignature cs = _cmsICCcolorSpace(1);

    // Prepare a CIELab sample and run a GBD checkpoint against a handle (reuse hIT8 as a generic handle)
    cmsCIELab sampleLab;
    sampleLab.L = 50.0;
    sampleLab.a = 0.0;
    sampleLab.b = 0.0;
    cmsBool checkpointResult = cmsGDBCheckPoint(hIT8, &sampleLab);

    // Free allocated tone curves if present
    if (reversed != nullptr) {
        cmsFreeToneCurve(reversed);
    }
    if (curve != nullptr) {
        cmsFreeToneCurve(curve);
    }

    // Use variables to avoid unused warnings (no-op)
    (void) retrieved;
    (void) cs;
    (void) checkpointResult;

    // Success
    return 66;
}