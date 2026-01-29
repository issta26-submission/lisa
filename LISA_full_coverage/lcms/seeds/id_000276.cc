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
//<ID> 276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsToneCurve * gamma = cmsBuildGamma(ctx, (cmsFloat64Number)2.2);
    cmsHPROFILE inProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE outProfile = cmsCreate_sRGBProfile();
    cmsHPROFILE profiles[2];
    profiles[0] = inProfile;
    profiles[1] = outProfile;
    cmsHTRANSFORM transform = cmsCreateMultiprofileTransform(profiles, (cmsUInt32Number)2, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsUInt16Number alarms[16];
    memset(alarms, 0, sizeof(alarms));
    cmsUInt32Number namedCount;
    unsigned char inputBuffer[6];
    unsigned char outputBuffer[6];
    memset(inputBuffer, 0, sizeof(inputBuffer));
    memset(outputBuffer, 0, sizeof(outputBuffer));
    inputBuffer[0] = 0xFF; inputBuffer[1] = 0x00; inputBuffer[2] = 0x00; /* pixel 0: red */
    inputBuffer[3] = 0x00; inputBuffer[4] = 0xFF; inputBuffer[5] = 0x00; /* pixel 1: green */

    // step 2: Configure
    /* configuration already performed by creating profiles and transform above */
    (void)ctx;

    // step 3: Operate
    cmsDoTransformLineStride(transform, (const void *)inputBuffer, (void *)outputBuffer, (cmsUInt32Number)2, (cmsUInt32Number)1, (cmsUInt32Number)6, (cmsUInt32Number)6, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsContext transformCtx = cmsGetTransformContextID(transform);
    cmsGetAlarmCodesTHR(transformCtx, alarms);
    namedCount = cmsNamedColorCount((const cmsNAMEDCOLORLIST *)NULL);

    // step 4: Validate & Cleanup
    cmsFreeToneCurve(gamma);
    cmsCloseProfile(inProfile);
    cmsCloseProfile(outProfile);
    (void)namedCount;
    (void)alarms;
    // API sequence test completed successfully
    return 66;
}