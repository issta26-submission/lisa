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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    // step 1: Declarations & Initialization
    cmsContext ctx = (cmsContext)NULL;
    cmsUInt32Number nGridPoints = (cmsUInt32Number)2;
    cmsUInt32Number inputChan = (cmsUInt32Number)3;
    cmsUInt32Number outputChan = (cmsUInt32Number)3;
    cmsHPROFILE profile = cmsCreate_sRGBProfileTHR(ctx);
    cmsUInt32Number clutEntries = (cmsUInt32Number)(1u << inputChan);
    cmsUInt32Number tableSize = clutEntries * outputChan;
    cmsUInt16Number * table = (cmsUInt16Number *)malloc((size_t)tableSize * sizeof(cmsUInt16Number));
    memset(table, 0, (size_t)tableSize * sizeof(cmsUInt16Number));

    // step 2: Configure
    cmsUInt8Number profileID[16];
    memset(profileID, 0, sizeof(profileID));
    cmsGetHeaderProfileID(profile, profileID);
    cmsColorSpaceSignature profileSpace = cmsGetColorSpace(profile);
    int csType = _cmsLCMScolorSpace(profileSpace);
    (void)csType;

    // step 3: Operate
    cmsStage * stage = cmsStageAllocCLut16bit(ctx, nGridPoints, inputChan, outputChan, table);
    cmsCIEXYZ blackPoint;
    memset(&blackPoint, 0, sizeof(blackPoint));
    cmsBool hasBP = cmsDetectDestinationBlackPoint(&blackPoint, profile, (cmsUInt32Number)0, (cmsUInt32Number)0);
    cmsUInt32Number stageOut = cmsStageOutputChannels(stage);
    (void)hasBP;
    (void)stageOut;

    // step 4: Validate & Cleanup
    cmsGetHeaderProfileID(profile, profileID);
    cmsStageFree(stage);
    free(table);
    cmsCloseProfile(profile);
    /* API sequence test completed successfully */
    return 66;
}