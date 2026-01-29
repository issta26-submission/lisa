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
//<ID> 221
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // Step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsUInt16Number alarms[16];
    alarms[0] = (cmsUInt16Number)0x0100U;
    alarms[1] = (cmsUInt16Number)0x0200U;
    alarms[2] = (cmsUInt16Number)0;
    alarms[3] = (cmsUInt16Number)0;
    alarms[4] = (cmsUInt16Number)0;
    alarms[5] = (cmsUInt16Number)0;
    alarms[6] = (cmsUInt16Number)0;
    alarms[7] = (cmsUInt16Number)0;
    alarms[8] = (cmsUInt16Number)0;
    alarms[9] = (cmsUInt16Number)0;
    alarms[10] = (cmsUInt16Number)0;
    alarms[11] = (cmsUInt16Number)0;
    alarms[12] = (cmsUInt16Number)0;
    alarms[13] = (cmsUInt16Number)0;
    alarms[14] = (cmsUInt16Number)0;
    alarms[15] = (cmsUInt16Number)0;

    // Step 2: Configure alarm codes (THR variant using context)
    cmsSetAlarmCodesTHR(ctx, alarms);

    // Step 3: Create profiles
    cmsHPROFILE hProfile = cmsCreateXYZProfileTHR(ctx);
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();

    // Step 4: Query profile properties
    cmsProfileClassSignature devClass = cmsGetDeviceClass(hProfile);
    cmsColorSpaceSignature srgbSpace = cmsGetColorSpace(hSRGB);
    cmsInt32Number srgbChannels = cmsChannelsOfColorSpace(srgbSpace);

    // Step 5: Allocate an identity stage (use 3 channels to keep logic similar)
    cmsStage * mpe = cmsStageAllocIdentity(ctx, 3);

    // Step 6: Inspect stage and context
    cmsUInt32Number stageInChannels = cmsStageInputChannels(mpe);
    cmsContext stageCtx = cmsGetStageContextID(mpe);

    // Step 7: Query supported intents via THR API
    cmsUInt32Number intentCodes[4];
    char * intentDescs[4];
    cmsUInt32Number nIntents = cmsGetSupportedIntentsTHR(ctx, 4U, intentCodes, intentDescs);

    // Step 8: Close profiles (cleanup of created resources available in API list)
    cmsCloseProfile(hProfile);
    cmsCloseProfile(hSRGB);

    // Silence unused variable warnings (no control flow; straight-line sequence)
    (void)devClass;
    (void)srgbChannels;
    (void)stageInChannels;
    (void)stageCtx;
    (void)nIntents;
    (void)intentCodes;
    (void)intentDescs;
    (void)mpe;
    (void)alarms;
    (void)ctx;

    // API sequence test completed successfully
    return 66;
}