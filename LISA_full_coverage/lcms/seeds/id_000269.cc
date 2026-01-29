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
//<ID> 269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsUInt32Number nLUTPoints = (cmsUInt32Number)5;
    cmsFloat64Number Bright = (cmsFloat64Number)50.0;
    cmsFloat64Number Contrast = (cmsFloat64Number)1.2;
    cmsFloat64Number Hue = (cmsFloat64Number)0.0;
    cmsFloat64Number Saturation = (cmsFloat64Number)1.0;
    cmsUInt32Number TempSrc = (cmsUInt32Number)6500;
    cmsUInt32Number TempDest = (cmsUInt32Number)5000;
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ctx, nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList(ctx, (cmsUInt32Number)3, (cmsUInt32Number)4, "pre", "suf");
    char info[32];
    memset(info, 0, sizeof(info));

    // step 2: Configure
    cmsUInt32Number model = (cmsUInt32Number)0x41425354UL; /* 'ABST' */
    cmsSetHeaderModel(profile, model);
    cmsColorSpaceSignature pcs = (cmsColorSpaceSignature)0x58595A20UL; /* 'XYZ ' */
    cmsSetPCS(profile, pcs);

    // step 3: Operate
    const cmsCurveSegment * seg0 = cmsGetToneCurveSegment((cmsInt32Number)0, (const cmsToneCurve *)gamma);
    cmsUInt32Number namedCount = cmsNamedColorCount((const cmsNAMEDCOLORLIST *)ncl);
    cmsInt32Number segType = (cmsInt32Number)seg0->Type;
    cmsFloat64Number seg_x0 = (cmsFloat64Number)seg0->x0;
    info[0] = (char)(segType & 0xFFU);
    info[1] = (char)(namedCount & 0xFFU);
    /* propagate some values back into profile header bytes by touching them (harmless use) */
    info[2] = (char)((model >> 24) & 0xFFU);
    info[3] = (char)((pcs >> 0) & 0xFFU);
    (void)seg_x0;

    // step 4: Validate & Cleanup
    cmsFreeToneCurve(gamma);
    cmsFreeNamedColorList(ncl);
    cmsCloseProfile(profile);
    // API sequence test completed successfully
    return 66;
}