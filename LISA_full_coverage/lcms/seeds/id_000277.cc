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
//<ID> 277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsHPROFILE inputProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE outputProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE profiles[2];
    profiles[0] = inputProfile;
    profiles[1] = outputProfile;
    cmsUInt32Number inputFormat = (cmsUInt32Number)0;
    cmsUInt32Number outputFormat = (cmsUInt32Number)0;
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(profiles, (cmsUInt32Number)2, inputFormat, outputFormat, (cmsUInt32Number)0, (cmsUInt32Number)0);

    // step 2: Configure
    cmsContext ctx = cmsGetTransformContextID(transform);
    cmsUInt16Number AlarmCodes[16];
    memset(AlarmCodes, 0, sizeof(AlarmCodes));
    cmsGetAlarmCodesTHR(ctx, AlarmCodes);
    const cmsNAMEDCOLORLIST * namedList = (const cmsNAMEDCOLORLIST *)NULL;
    cmsUInt32Number namedCount = cmsNamedColorCount(namedList);

    // step 3: Operate
    unsigned char inputBuffer[6];
    inputBuffer[0] = 0;   inputBuffer[1] = 128; inputBuffer[2] = 255;
    inputBuffer[3] = 10;  inputBuffer[4] = 20;  inputBuffer[5] = 30;
    unsigned char outputBuffer[6];
    memset(outputBuffer, 0, sizeof(outputBuffer));
    cmsDoTransformLineStride(transform,
                             (const void *)inputBuffer,
                             (void *)outputBuffer,
                             (cmsUInt32Number)2,   /* PixelsPerLine */
                             (cmsUInt32Number)1,   /* LineCount */
                             (cmsUInt32Number)3,   /* BytesPerLineIn */
                             (cmsUInt32Number)3,   /* BytesPerLineOut */
                             (cmsUInt32Number)0,   /* BytesPerPlaneIn */
                             (cmsUInt32Number)0);  /* BytesPerPlaneOut */
    AlarmCodes[0] = (cmsUInt16Number)namedCount;

    // step 4: Validate & Cleanup
    cmsFreeToneCurve((cmsToneCurve *)NULL);
    cmsDeleteContext(ctx);
    /* API sequence test completed successfully */
    return 66;
}