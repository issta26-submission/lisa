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
//<ID> 396
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = NULL;
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    cmsHPROFILE labProfile = cmsCreateLab2ProfileTHR(ctx, &whitePoint);
    cmsUInt16Number table[2];
    table[0] = 0;
    table[1] = 65535;
    cmsToneCurve * curve = cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)2, table);
    cmsFloat64Number gamma = cmsEstimateGamma(curve, 1e-6);
    (void)gamma;
    cmsToneCurve * rev1 = cmsReverseToneCurveEx((cmsUInt32Number)256, curve);
    cmsToneCurve * rev2 = cmsReverseToneCurveEx((cmsUInt32Number)256, curve);
    cmsToneCurve * triple[3];
    triple[0] = curve;
    triple[1] = rev1;
    triple[2] = rev2;
    cmsFreeToneCurveTriple(triple);
    cmsCloseProfile(labProfile);
    return 66;
}