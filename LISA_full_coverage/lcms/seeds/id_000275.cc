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
//<ID> 275
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsUInt16Number AlarmCodes[16];
    memset(AlarmCodes, 0, sizeof(AlarmCodes));
    cmsGetAlarmCodesTHR(ctx, AlarmCodes);

    cmsCIExyY wp;
    memset(&wp, 0, sizeof(wp));
    wp.x = (cmsFloat64Number)0.3127;
    wp.y = (cmsFloat64Number)0.3290;
    wp.Y = (cmsFloat64Number)1.0;

    cmsHPROFILE inProfile = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE outProfile = cmsCreateLab2ProfileTHR(ctx, &wp);

    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);

    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList(ctx, (cmsUInt32Number)2, (cmsUInt32Number)3, "pre", "suf");
    cmsUInt16Number pcs0[3];
    pcs0[0] = pcs0[1] = pcs0[2] = (cmsUInt16Number)0;
    cmsUInt16Number colorant0[16];
    memset(colorant0, 0, sizeof(colorant0));
    cmsAppendNamedColor(ncl, "ColorA", pcs0, colorant0);
    cmsUInt16Number pcs1[3];
    pcs1[0] = pcs1[1] = pcs1[2] = (cmsUInt16Number)0;
    cmsUInt16Number colorant1[16];
    memset(colorant1, 0, sizeof(colorant1));
    cmsAppendNamedColor(ncl, "ColorB", pcs1, colorant1);

    // step 2: Configure
    cmsUInt32Number namedCount = cmsNamedColorCount((const cmsNAMEDCOLORLIST *)ncl);

    // step 3: Operate
    cmsHTRANSFORM xform = cmsCreateProofingTransform(inProfile, (cmsUInt32Number)0, outProfile, (cmsUInt32Number)0, (cmsHPROFILE)NULL, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsContext tctx = cmsGetTransformContextID(xform);
    cmsGetAlarmCodesTHR(tctx, AlarmCodes);

    unsigned char inBuffer[6];
    unsigned char outBuffer[6];
    memset(inBuffer, 0x80, sizeof(inBuffer));
    memset(outBuffer, 0, sizeof(outBuffer));
    cmsDoTransformLineStride(xform, (const void *)inBuffer, (void *)outBuffer, (cmsUInt32Number)2, (cmsUInt32Number)1, (cmsUInt32Number)3, (cmsUInt32Number)3, (cmsUInt32Number)3, (cmsUInt32Number)3);

    // step 4: Validate & Cleanup
    cmsFreeToneCurve(gamma);
    cmsFreeNamedColorList(ncl);
    cmsDeleteTransform(xform);
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);
    cmsGetAlarmCodesTHR(ctx, AlarmCodes);
    // API sequence test completed successfully
    return 66;
}