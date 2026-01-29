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
//<ID> 281
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(ctx);
    cmsUInt8Number ProfileID[16];
    memset(ProfileID, 0, sizeof(ProfileID));
    cmsGetHeaderProfileID(hProfile, ProfileID);
    cmsColorSpaceSignature srcSpace = cmsGetColorSpace(hProfile);
    cmsColorSpaceSignature pcsSpace = cmsGetPCS(hProfile);

    // step 2: Configure
    cmsUInt32Number inChan = (cmsUInt32Number)cmsChannelsOfColorSpace(srcSpace);
    cmsUInt32Number outChan = (cmsUInt32Number)cmsChannelsOfColorSpace(pcsSpace);
    cmsUInt32Number nGridPoints = (cmsUInt32Number)2;
    cmsUInt16Number Table[24] = {
        (cmsUInt16Number)(((cmsUInt16Number)ProfileID[0] << 8) | (cmsUInt16Number)ProfileID[1]),
        (cmsUInt16Number)(((cmsUInt16Number)ProfileID[2] << 8) | (cmsUInt16Number)ProfileID[3]),
        (cmsUInt16Number)(((cmsUInt16Number)ProfileID[4] << 8) | (cmsUInt16Number)ProfileID[5]),
        (cmsUInt16Number)(((cmsUInt16Number)ProfileID[6] << 8) | (cmsUInt16Number)ProfileID[7]),
        (cmsUInt16Number)(((cmsUInt16Number)ProfileID[8] << 8) | (cmsUInt16Number)ProfileID[9]),
        (cmsUInt16Number)(((cmsUInt16Number)ProfileID[10] << 8) | (cmsUInt16Number)ProfileID[11]),
        (cmsUInt16Number)(((cmsUInt16Number)ProfileID[12] << 8) | (cmsUInt16Number)ProfileID[13]),
        (cmsUInt16Number)(((cmsUInt16Number)ProfileID[14] << 8) | (cmsUInt16Number)ProfileID[15]),
        (cmsUInt16Number)0x0000U, (cmsUInt16Number)0x8000U, (cmsUInt16Number)0xFFFFU, (cmsUInt16Number)0x4000U,
        (cmsUInt16Number)0x2000U, (cmsUInt16Number)0x6000U, (cmsUInt16Number)0xA000U, (cmsUInt16Number)0xC000U,
        (cmsUInt16Number)0x1000U, (cmsUInt16Number)0x3000U, (cmsUInt16Number)0x5000U, (cmsUInt16Number)0x7000U,
        (cmsUInt16Number)0x9000U, (cmsUInt16Number)0xB000U, (cmsUInt16Number)0xD000U, (cmsUInt16Number)0xE000U
    };

    // step 3: Operate
    cmsStage * stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inChan, outChan, Table);
    cmsUInt32Number stageOutputs = cmsStageOutputChannels(stage);
    cmsCIEXYZ BlackPoint;
    memset(&BlackPoint, 0, sizeof(BlackPoint));
    cmsBool hasBlack = cmsDetectDestinationBlackPoint(&BlackPoint, hProfile, (cmsUInt32Number)0, (cmsUInt32Number)0);
    int csType = _cmsLCMScolorSpace(srcSpace);
    (void)stageOutputs;
    (void)hasBlack;
    (void)csType;

    // step 4: Validate & Cleanup
    cmsStageFree(stage);
    cmsCloseProfile(hProfile);
    /* API sequence test completed successfully */
    return 66;
}