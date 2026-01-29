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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList(ctx, (cmsUInt32Number)3, (cmsUInt32Number)4, "PFX", "SFX");
    cmsHPROFILE profile = cmsCreateBCHSWabstractProfileTHR(ctx, (cmsUInt32Number)17, (cmsFloat64Number)50.0, (cmsFloat64Number)10.0, (cmsFloat64Number)0.0, (cmsFloat64Number)1.2, (cmsUInt32Number)3000, (cmsUInt32Number)6500);
    char info[32];
    memset(info, 0, sizeof(info));

    // step 2: Configure
    const cmsCurveSegment * seg = cmsGetToneCurveSegment((cmsInt32Number)0, gamma);
    cmsUInt32Number nColors = cmsNamedColorCount(ncl);
    cmsUInt32Number model = (cmsUInt32Number)0x4D4F444C; /* 'MODL' */
    model ^= (cmsUInt32Number)(seg->Type & 0xFFU);
    cmsSetHeaderModel(profile, model);
    cmsColorSpaceSignature pcs = (cmsColorSpaceSignature)0x58595A20; /* 'XYZ ' */
    cmsSetPCS(profile, pcs);

    // step 3: Operate
    cmsUInt32Number segGrid = seg->nGridPoints;
    cmsUInt32Number combined = segGrid + (nColors & 3U);
    info[0] = (char)(seg->Type & 0xFFU);
    info[1] = (char)(nColors & 0xFFU);
    cmsSetHeaderModel(profile, model ^ combined);

    // step 4: Validate & Cleanup
    (void)combined;
    (void)info;
    cmsFreeToneCurve(gamma);
    cmsFreeNamedColorList(ncl);
    cmsCloseProfile(profile);
    cmsDeleteContext(ctx);

    // API sequence test completed successfully
    return 66;
}