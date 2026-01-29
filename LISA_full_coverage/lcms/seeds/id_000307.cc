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
//<ID> 307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    cmsCIExyYTRIPLE primaries;
    primaries.Red.x   = 0.64;  primaries.Red.y   = 0.33;  primaries.Red.Y   = 1.0;
    primaries.Green.x = 0.30;  primaries.Green.y = 0.60;  primaries.Green.Y = 1.0;
    primaries.Blue.x  = 0.15;  primaries.Blue.y  = 0.06;  primaries.Blue.Y  = 1.0;

    // build a simple gamma tone curve and keep a triple for later free
    cmsToneCurve * gamma = cmsBuildGamma(ctx, 2.2);
    cmsToneCurve * const transferFunc[3] = { gamma, gamma, gamma };

    // create profiles
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(ctx, &whitePoint);
    cmsHPROFILE rgbProfile = cmsCreate_sRGBProfile();

    // step 2: Create transform (straight-line usage, numeric zeros for formats/intent/flags)
    cmsHTRANSFORM transform = cmsCreateTransformTHR(ctx, rgbProfile, (cmsUInt32Number)0, labProfile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 3: Core operations
    cmsUInt32Number inFmt = cmsGetTransformInputFormat(transform);
    (void)inFmt;
    unsigned char inbuf[4] = {0, 0, 0, 0};
    unsigned char outbuf[4] = {0, 0, 0, 0};
    cmsDoTransform(transform, inbuf, outbuf, (cmsUInt32Number)1);
    cmsDeleteTransform(transform);

    // free tone curves (use triple freeing helper)
    cmsToneCurve * triple[3] = { gamma, NULL, NULL };
    cmsFreeToneCurveTriple(triple);

    // try saving Lab profile to memory (query size then allocate)
    cmsUInt32Number bytesNeeded = 0;
    cmsSaveProfileToMem(labProfile, NULL, &bytesNeeded);
    unsigned char * membuf = (unsigned char *)malloc((size_t)bytesNeeded + 1);
    cmsSaveProfileToMem(labProfile, membuf, &bytesNeeded);
    free(membuf);

    // step 4: End (no cmsCloseProfile available in provided API set)
    return 66;
}