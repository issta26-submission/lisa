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
//<ID> 7
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // 1) Convert a sample LCh color to Lab
    cmsCIELCh lch = { 50.0, 30.0, 120.0 };
    cmsCIELab lab;
    cmsLCh2Lab(&lab, &lch);

    // 2) Create an sRGB profile to operate on
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == NULL) {
        return 1;
    }

    // 3) Obtain context from the profile and build a small tabulated tone curve
    cmsContext ctx = cmsGetProfileContextID(hProfile);
    cmsUInt16Number table[3] = { 0, 32768, 65535 };
    cmsToneCurve * curve = cmsBuildTabulatedToneCurve16(ctx, 3, table);

    // 4) Create the reverse (inverse) tone curve from the original
    cmsToneCurve * revCurve = cmsReverseToneCurve(curve);

    // 5) Detect black point of the profile
    cmsCIEXYZ blackPoint = { 0.0, 0.0, 0.0 };
    cmsBool hasBlack = cmsDetectBlackPoint(&blackPoint, hProfile, 0, 0);

    // 6) Query two tag signatures (indices 0 and 1) and attempt to link them
    cmsTagSignature sig0 = cmsGetTagSignature(hProfile, 0);
    cmsTagSignature sig1 = cmsGetTagSignature(hProfile, 1);
    cmsBool linked = cmsLinkTag(hProfile, sig0, sig1);

    // Use variables to avoid unused warnings in some build configurations
    (void)lab.L; (void)lab.a; (void)lab.b;
    (void)curve; (void)revCurve;
    (void)hasBlack; (void)blackPoint.X; (void)linked;

    return 66;
}