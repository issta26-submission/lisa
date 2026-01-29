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
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    cmsUInt8Number profileID[16];
    memset(profileID, 0, sizeof(profileID));
    cmsCIEXYZ blackPoint;
    memset(&blackPoint, 0, sizeof(blackPoint));
    cmsUInt32Number nGridPoints = (cmsUInt32Number)2;
    cmsUInt32Number outputChan = (cmsUInt32Number)3;
    static cmsUInt16Number clutTable[24];
    clutTable[0] = 0; clutTable[1] = 1; clutTable[2] = 2; clutTable[3] = 3;
    clutTable[4] = 4; clutTable[5] = 5; clutTable[6] = 6; clutTable[7] = 7;
    clutTable[8] = 8; clutTable[9] = 9; clutTable[10] = 10; clutTable[11] = 11;
    clutTable[12] = 12; clutTable[13] = 13; clutTable[14] = 14; clutTable[15] = 15;
    clutTable[16] = 16; clutTable[17] = 17; clutTable[18] = 18; clutTable[19] = 19;
    clutTable[20] = 20; clutTable[21] = 21; clutTable[22] = 22; clutTable[23] = 23;

    // step 2: Configure
    cmsColorSpaceSignature profileSpace = cmsGetColorSpace(profile);
    cmsUInt32Number inputChan = cmsChannelsOfColorSpace(profileSpace);
    int cs_index = _cmsLCMScolorSpace(profileSpace);
    (void)cs_index;

    // step 3: Operate
    cmsStage * stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, clutTable);
    cmsBool detected = cmsDetectDestinationBlackPoint(&blackPoint, profile, (cmsUInt32Number)0, (cmsUInt32Number)0);
    (void)detected;
    cmsGetHeaderProfileID(profile, profileID);

    // step 4: Validate & Cleanup
    cmsStageFree(stage);
    cmsCloseProfile(profile);
    /* API sequence test completed successfully */
    return 66;
}