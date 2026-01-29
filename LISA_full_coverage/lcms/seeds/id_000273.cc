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
//<ID> 273
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsHPROFILE inProfile = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE outProfile = cmsCreateXYZProfile();
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList(ctx, (cmsUInt32Number)1, (cmsUInt32Number)4, "pre", "suf");
    cmsUInt16Number pcs[3] = { 0, 0, 0 };
    cmsUInt16Number colorants[16];
    memset(colorants, 0, sizeof(colorants));
    cmsAppendNamedColor(ncl, "SampleColor", pcs, colorants);

    // step 2: Configure
    cmsHPROFILE profiles[2];
    profiles[0] = inProfile;
    profiles[1] = outProfile;
    cmsHTRANSFORM xform = cmsCreateMultiprofileTransform(profiles, (cmsUInt32Number)2, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 3: Operate
    cmsContext txCtx = cmsGetTransformContextID(xform);
    cmsUInt16Number alarms[16];
    memset(alarms, 0, sizeof(alarms));
    cmsGetAlarmCodesTHR(txCtx, alarms);
    cmsUInt32Number namedCount = cmsNamedColorCount(ncl);
    (void)namedCount;
    unsigned char inBuffer[12];
    unsigned char outBuffer[12];
    memset(inBuffer, 0, sizeof(inBuffer));
    memset(outBuffer, 0, sizeof(outBuffer));
    cmsDoTransformLineStride(xform, (const void *)inBuffer, (void *)outBuffer, (cmsUInt32Number)1, (cmsUInt32Number)1, (cmsUInt32Number)sizeof(inBuffer), (cmsUInt32Number)sizeof(outBuffer), (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 4: Validate & Cleanup
    cmsFreeToneCurve(gamma);
    cmsFreeNamedColorList(ncl);
    cmsDeleteTransform(xform);
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);
    return 66;
}
// API sequence test completed successfully