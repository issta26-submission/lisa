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
//<ID> 272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE hInput = cmsCreate_sRGBProfile();
    cmsHPROFILE hOutput = cmsCreateXYZProfile();
    cmsHPROFILE hProof = cmsCreateXYZProfile();
    cmsToneCurve * gamma = cmsBuildGamma((cmsContext)NULL, (cmsFloat64Number)2.2);
    cmsNAMEDCOLORLIST * ncl = cmsAllocNamedColorList((cmsContext)NULL, (cmsUInt32Number)2, (cmsUInt32Number)3, "pre", "suf");
    cmsUInt16Number alarmCodes[16];
    memset(alarmCodes, 0, sizeof(alarmCodes));
    cmsHTRANSFORM hTransform = cmsCreateProofingTransform(hInput, (cmsUInt32Number)0, hOutput, (cmsUInt32Number)0, hProof, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 2: Configure
    cmsUInt32Number namedCount = cmsNamedColorCount((const cmsNAMEDCOLORLIST *)ncl);
    (void)namedCount;
    unsigned char inputBuffer[9];
    unsigned char outputBuffer[9];
    memset(inputBuffer, 0x80, sizeof(inputBuffer));
    memset(outputBuffer, 0, sizeof(outputBuffer));
    cmsUInt32Number pixelsPerLine = (cmsUInt32Number)3;
    cmsUInt32Number lineCount = (cmsUInt32Number)1;
    cmsUInt32Number bytesPerLineIn = (cmsUInt32Number)sizeof(inputBuffer);
    cmsUInt32Number bytesPerLineOut = (cmsUInt32Number)sizeof(outputBuffer);
    cmsUInt32Number bytesPerPlaneIn = bytesPerLineIn;
    cmsUInt32Number bytesPerPlaneOut = bytesPerLineOut;

    // step 3: Operate
    cmsContext ctx = cmsGetTransformContextID(hTransform);
    cmsGetAlarmCodesTHR(ctx, alarmCodes);
    cmsDoTransformLineStride(hTransform, (const void *)inputBuffer, (void *)outputBuffer, pixelsPerLine, lineCount, bytesPerLineIn, bytesPerLineOut, bytesPerPlaneIn, bytesPerPlaneOut);

    // step 4: Validate & Cleanup
    cmsFreeToneCurve(gamma);
    cmsFreeNamedColorList(ncl);
    cmsDeleteTransform(hTransform);
    cmsCloseProfile(hInput);
    cmsCloseProfile(hOutput);
    cmsCloseProfile(hProof);
    // API sequence test completed successfully
    return 66;
}