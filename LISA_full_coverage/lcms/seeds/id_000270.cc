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
//<ID> 270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsUInt16Number alarmCodes[16];
    memset(alarmCodes, 0, sizeof(alarmCodes));
    cmsHPROFILE hSrc = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE hDst = cmsCreate_sRGBProfileTHR(ctx);
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList(ctx, (cmsUInt32Number)2, (cmsUInt32Number)3, "pre", "suf");
    cmsUInt16Number PCS[3] = { 0, 0, 0 };
    cmsUInt16Number Colorant[16] = { 0 };
    cmsAppendNamedColor(ncl, "Neutral", PCS, Colorant);
    cmsHPROFILE profiles[2];
    profiles[0] = hSrc;
    profiles[1] = hDst;
    cmsHTRANSFORM xform = cmsCreateMultiprofileTransform(profiles, (cmsUInt32Number)2, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    unsigned char inbuf[6] = { 255, 0, 0, 0, 255, 0 };
    unsigned char outbuf[6];
    memset(outbuf, 0, sizeof(outbuf));

    // step 2: Configure
    (void)hSrc;
    (void)hDst;
    (void)profiles;

    // step 3: Operate
    cmsContext xctx = cmsGetTransformContextID(xform);
    cmsGetAlarmCodesTHR(xctx, alarmCodes);
    cmsUInt32Number namedCount = cmsNamedColorCount(ncl);
    (void)namedCount;
    cmsDoTransformLineStride(xform, inbuf, outbuf, (cmsUInt32Number)2, (cmsUInt32Number)1, (cmsUInt32Number)3, (cmsUInt32Number)3, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 4: Validate & Cleanup
    cmsFreeToneCurve(gamma);
    cmsFreeNamedColorList(ncl);
    cmsDeleteTransform(xform);
    cmsCloseProfile(hSrc);
    cmsCloseProfile(hDst);
    // API sequence test completed successfully
    return 66;
}