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
//<ID> 287
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
    cmsUInt8Number profileID[16];
    memset(profileID, 0, sizeof(profileID));
    cmsGetHeaderProfileID(hProfile, profileID);
    cmsColorSpaceSignature cs = cmsGetColorSpace(hProfile);
    int csCode = _cmsLCMScolorSpace(cs);

    // step 2: Configure
    cmsUInt32Number nGridPoints = (cmsUInt32Number)2;
    cmsInt32Number chans = cmsChannelsOfColorSpace(cs);
    cmsUInt32Number inputChan = (cmsUInt32Number)chans;
    cmsUInt32Number outputChan = (cmsUInt32Number)3;
    cmsUInt16Number clutTable[24];
    memset(clutTable, 0, sizeof(clutTable));
    cmsStage * stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, clutTable);

    // step 3: Operate
    cmsCIEXYZ blackPoint;
    blackPoint.X = (cmsFloat64Number)0.0;
    blackPoint.Y = (cmsFloat64Number)0.0;
    blackPoint.Z = (cmsFloat64Number)0.0;
    cmsDetectDestinationBlackPoint(&blackPoint, hProfile, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsUInt32Number outChannels = (cmsUInt32Number)cmsStageOutputChannels(stage);
    (void)csCode;
    (void)profileID;
    (void)outChannels;

    // step 4: Validate & Cleanup
    cmsStageFree(stage);
    cmsCloseProfile(hProfile);
    // API sequence test completed successfully
    return 66;
}