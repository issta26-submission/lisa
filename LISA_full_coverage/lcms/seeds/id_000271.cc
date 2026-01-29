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
//<ID> 271
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
    cmsToneCurve * gamma = (cmsToneCurve *)NULL;
    gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsHPROFILE prof_in = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE prof_out = cmsCreateXYZProfile();
    cmsNAMEDCOLORLIST * ncl = (cmsNAMEDCOLORLIST *)NULL;
    ncl = cmsAllocNamedColorList(ctx, (cmsUInt32Number)2, (cmsUInt32Number)3, "pre", "suf");

    // step 2: Configure
    cmsAppendNamedColor(ncl, "Red", (cmsUInt16Number[]){0, 0, 0}, (cmsUInt16Number[]){65535, 0, 0});
    cmsAppendNamedColor(ncl, "Green", (cmsUInt16Number[]){0, 0, 0}, (cmsUInt16Number[]){0, 65535, 0});
    cmsUInt32Number namedCount = cmsNamedColorCount((const cmsNAMEDCOLORLIST *)ncl);
    cmsUInt32Number InputFormat = (cmsUInt32Number)0x00000000;
    cmsUInt32Number OutputFormat = (cmsUInt32Number)0x00000000;
    cmsUInt32Number Intent = (cmsUInt32Number)0;
    cmsHTRANSFORM xform = cmsCreateProofingTransform(prof_in, InputFormat, prof_out, OutputFormat, (cmsHPROFILE)NULL, Intent, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 3: Operate
    cmsContext ctx_from_transform = cmsGetTransformContextID(xform);
    cmsGetAlarmCodesTHR(ctx_from_transform, AlarmCodes);
    unsigned char inBuf[12];
    unsigned char outBuf[12];
    memset(inBuf, 0x80, sizeof(inBuf));
    memset(outBuf, 0, sizeof(outBuf));
    cmsDoTransformLineStride(xform, (const void *)inBuf, (void *)outBuf, (cmsUInt32Number)4, (cmsUInt32Number)1, (cmsUInt32Number)12, (cmsUInt32Number)12, (cmsUInt32Number)12, (cmsUInt32Number)12);

    // step 4: Validate & Cleanup
    (void)namedCount;
    (void)AlarmCodes;
    cmsFreeToneCurve(gamma);
    cmsFreeNamedColorList(ncl);
    cmsDeleteTransform(xform);
    cmsCloseProfile(prof_in);
    cmsCloseProfile(prof_out);
    // API sequence test completed successfully
    return 66;
}