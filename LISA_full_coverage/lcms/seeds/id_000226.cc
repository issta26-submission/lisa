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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_lcms_api_sequence() {
    cmsContext ctx = (cmsContext)NULL;

    /* Prepare CLUT data and allocate a CLUT stage */
    cmsUInt32Number clutPoints[3];
    clutPoints[0] = 2U;
    clutPoints[1] = 2U;
    clutPoints[2] = 2U;
    cmsFloat32Number clutTable[8];
    memset(clutTable, 0, sizeof(clutTable));
    cmsStage * stage = cmsStageAllocCLutFloatGranular(ctx, clutPoints, 3U, 3U, clutTable);

    /* Query stage properties */
    cmsUInt32Number stageInputs = cmsStageInputChannels(stage);
    cmsStageSignature stageType = cmsStageType(stage);
    cmsContext stageCtx = cmsGetStageContextID(stage);

    /* Create a profile and read / modify a few header values */
    cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(ctx);
    cmsUInt32Number headerCreator = cmsGetHeaderCreator(hProfile);
    cmsUInt32Number headerFlags = cmsGetHeaderFlags(hProfile);
    cmsSetHeaderFlags(hProfile, headerFlags | 0x10U);
    cmsSetHeaderRenderingIntent(hProfile, 0U);
    cmsUInt32Number encVer = cmsGetEncodedICCversion(hProfile);
    cmsProfileClassSignature devClass = cmsGetDeviceClass(hProfile);
    cmsColorSpaceSignature pcs = cmsGetPCS(hProfile);
    cmsSetPCS(hProfile, pcs);

    /* Silences for unused variables while keeping straight-line code */
    (void)stage;
    (void)stageInputs;
    (void)stageType;
    (void)stageCtx;
    (void)clutPoints;
    (void)clutTable;
    (void)hProfile;
    (void)headerCreator;
    (void)headerFlags;
    (void)encVer;
    (void)devClass;
    (void)pcs;
    (void)ctx;

    return 66;
}