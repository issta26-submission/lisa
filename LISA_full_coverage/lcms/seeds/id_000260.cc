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
//<ID> 260
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsUInt32Number nLUTPoints = (cmsUInt32Number)17;
    cmsFloat64Number Bright = (cmsFloat64Number)1.0;
    cmsFloat64Number Contrast = (cmsFloat64Number)1.0;
    cmsFloat64Number Hue = (cmsFloat64Number)0.0;
    cmsFloat64Number Saturation = (cmsFloat64Number)1.0;
    cmsUInt32Number TempSrc = (cmsUInt32Number)6500;
    cmsUInt32Number TempDest = (cmsUInt32Number)5000;
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
    cmsUInt32Number model = (cmsUInt32Number)0x4D4F444CUL;
    cmsColorSpaceSignature pcs = (cmsColorSpaceSignature)0x58595A20; /* 'XYZ ' */

    // step 2: Configure
    cmsSetHeaderModel(profile, model);
    cmsSetPCS(profile, pcs);

    // step 3: Operate
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    const cmsCurveSegment * seg0 = cmsGetToneCurveSegment((cmsInt32Number)0, gamma);
    cmsHTRANSFORM xform = cmsCreateTransform(profile, (cmsUInt32Number)0, profile, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsNAMEDCOLORLIST * nlist = cmsGetNamedColorList(xform);
    cmsUInt32Number count = cmsNamedColorCount(nlist);
    cmsUInt32Number segGrid = seg0->nGridPoints;
    cmsUInt32Number combined = count + segGrid;
    (void)combined;

    // step 4: Validate & Cleanup
    cmsDeleteTransform(xform);
    cmsCloseProfile(profile);
    cmsFreeToneCurve(gamma);

    // API sequence test completed successfully
    return 66;
}