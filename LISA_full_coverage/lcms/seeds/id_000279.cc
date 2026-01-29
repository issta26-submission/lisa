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
//<ID> 279
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
    cmsHPROFILE outProfile = cmsCreate_sRGBProfileTHR(ctx);
    cmsHPROFILE profiles[2];
    profiles[0] = inProfile;
    profiles[1] = outProfile;
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(profiles, (cmsUInt32Number)2, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 2: Configure
    cmsContext tctx = cmsGetTransformContextID(transform);
    cmsUInt16Number AlarmCodes[16];
    memset(AlarmCodes, 0, sizeof(AlarmCodes));
    cmsGetAlarmCodesTHR(tctx, AlarmCodes);
    const cmsNAMEDCOLORLIST * originalNCL = (const cmsNAMEDCOLORLIST *)NULL;
    cmsNAMEDCOLORLIST * dupNCL = cmsDupNamedColorList(originalNCL);
    cmsUInt32Number namedCount = cmsNamedColorCount((const cmsNAMEDCOLORLIST *)dupNCL);

    // step 3: Operate
    cmsUInt32Number pixelsPerLine = (cmsUInt32Number)(namedCount + 2U);
    cmsUInt32Number lineCount = (cmsUInt32Number)1U;
    cmsUInt32Number bytesPerPixelIn = (cmsUInt32Number)3U;
    cmsUInt32Number bytesPerPixelOut = (cmsUInt32Number)3U;
    cmsUInt32Number bytesPerLineIn = pixelsPerLine * bytesPerPixelIn;
    cmsUInt32Number bytesPerLineOut = pixelsPerLine * bytesPerPixelOut;
    cmsUInt32Number bytesPerPlaneIn = bytesPerLineIn * lineCount;
    cmsUInt32Number bytesPerPlaneOut = bytesPerLineOut * lineCount;
    unsigned char inputBuf[256];
    unsigned char outputBuf[256];
    memset(inputBuf, 0x7Fu, (size_t)bytesPerPlaneIn);
    memset(outputBuf, 0, (size_t)bytesPerPlaneOut);
    inputBuf[0] = (unsigned char)255u;
    inputBuf[1] = (unsigned char)128u;
    inputBuf[2] = (unsigned char)64u;
    cmsDoTransformLineStride(transform, (const void *)inputBuf, (void *)outputBuf, pixelsPerLine, lineCount, bytesPerLineIn, bytesPerLineOut, bytesPerPlaneIn, bytesPerPlaneOut);

    // step 4: Validate & Cleanup
    cmsToneCurve * curve = (cmsToneCurve *)NULL;
    cmsFreeToneCurve(curve);
    cmsUInt32Number namedCountAfter = cmsNamedColorCount((const cmsNAMEDCOLORLIST *)dupNCL);
    (void)namedCountAfter;
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);
    /* API sequence test completed successfully */
    return 66;
}